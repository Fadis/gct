#include <gct/allocator.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/image.hpp>
#include <gct/image_view.hpp>
#include <gct/gbuffer.hpp>
#include <gct/graphics.hpp>
#include <gct/compute.hpp>
#include <gct/get_device.hpp>
#include <gct/device.hpp>
#include <gct/sampler_create_info.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/compute_pipeline.hpp>
#include <gct/pipeline_layout.hpp>
#include <gct/mappable_buffer.hpp>
#include <gct/epipolar_mesh.hpp>
#include <gct/epipolar_light_scattering.hpp>


namespace gct {

epipolar_light_scattering::epipolar_light_scattering(
  const epipolar_light_scattering_create_info &ci
) : props( ci ) {
  output.reset( new gbuffer(
    gbuffer_create_info()
      .set_allocator( props.allocator_set.allocator )
      .set_width(
        props.input[ 0 ]->get_factory()->get_props().get_basic().extent.width
      )
      .set_height(
        props.input[ 0 ]->get_factory()->get_props().get_basic().extent.height
      )
      .set_swapchain_image_count( props.input.size() )
      .set_color_buffer_count( 1 )
      .disable_depth()
      .set_format( vk::Format::eR32G32B32A32Sfloat )
      .set_final_layout( vk::ImageLayout::eColorAttachmentOptimal )
      .set_clear_color( color::special::transparent )
  ) );
  volumetric_light = props.allocator_set.allocator->create_image_views(
    image_create_info_t()
      .set_basic(
        basic_2d_image( props.pole_count, props.vertex_count )
          .setUsage(
            vk::ImageUsageFlagBits::eTransferSrc|
            vk::ImageUsageFlagBits::eStorage|
            vk::ImageUsageFlagBits::eSampled
          )
      ),
    VMA_MEMORY_USAGE_GPU_ONLY,
    props.input.size()
  );
  volumetric_light_texcoord = props.allocator_set.allocator->create_image_views(
    image_create_info_t()
      .set_basic(
        basic_2d_image( props.pole_count, props.vertex_count * props.texel_per_sample )
          .setUsage(
            vk::ImageUsageFlagBits::eTransferSrc|
            vk::ImageUsageFlagBits::eStorage|
            vk::ImageUsageFlagBits::eSampled
          )
      ),
    VMA_MEMORY_USAGE_GPU_ONLY,
    props.input.size()
  );
  gauss_temp = props.allocator_set.allocator->create_image_views(
    image_create_info_t()
      .set_basic(
        basic_2d_image(
          props.input[ 0 ]->get_factory()->get_props().get_basic().extent.width,
          props.input[ 0 ]->get_factory()->get_props().get_basic().extent.height
        )
          .setUsage(
            vk::ImageUsageFlagBits::eTransferSrc|
            vk::ImageUsageFlagBits::eStorage
          )
      ),
    VMA_MEMORY_USAGE_GPU_ONLY,
    props.input.size()
  );
  auto linear_sampler = get_device( *props.allocator_set.pipeline_cache ).get_sampler(
    get_basic_linear_sampler_create_info()
  );

  draw_mesh.reset( new graphics(
    graphics_create_info()
      .set_allocator_set( props.allocator_set )
      .set_pipeline_create_info(
        graphics_pipeline_create_info_t()
          .set_input_assembly(
            pipeline_input_assembly_state_create_info_t()
              .set_basic(
                vk::PipelineInputAssemblyStateCreateInfo()
                  .setTopology( vk::PrimitiveTopology::eTriangleList )
              )
          )
          .set_viewport(
            pipeline_viewport_state_create_info_t()
              .add_size(
                props.input[ 0 ]->get_factory()->get_props().get_basic().extent.width,
                props.input[ 0 ]->get_factory()->get_props().get_basic().extent.height
              )
          )
          .set_depth_stencil(
            pipeline_depth_stencil_state_create_info_t()
              .disable_depth()
          )
          .set_render_pass( output->get_render_pass(), 0 )
      )
      .set_swapchain_image_count( props.input.size() )
      .add_shader( props.draw_mesh_shaders )
      .set_resources( props.resources )
      .add_resource( { "volumetric_light", linear_sampler, volumetric_light, vk::ImageLayout::eShaderReadOnlyOptimal } )
      .add_resource( { "volumetric_light_texcoord", linear_sampler, volumetric_light_texcoord, vk::ImageLayout::eShaderReadOnlyOptimal } )
  ) );

  mesh.reset( new epipolar_mesh(
    epipolar_mesh_create_info()
      .set_allocator_set( props.allocator_set )
      .set_generate2_shader( props.generate_mesh2_shader )
      .set_generate3_shader( props.generate_mesh3_shader )
      .set_generate4_shader( props.generate_mesh4_shader )
      .set_pole_count( props.pole_count )
      .set_vertex_count( props.vertex_count )
      .set_attrs( draw_mesh->get_vamap() )
      .set_stride( draw_mesh->get_stride() )
      .set_swapchain_image_count( props.input.size() )
  ) );

  calc_volumetric_light.reset( new compute(
    compute_create_info()
      .set_allocator_set( props.allocator_set )
      .set_shader( props.volumetric_light_shader )
      .set_swapchain_image_count( props.input.size() )
      .set_resources( props.resources )
      .add_resource( { "gbuffer", props.input, vk::ImageLayout::eGeneral } )
      .add_resource( { "dest_image", volumetric_light, vk::ImageLayout::eGeneral } )
      .add_resource( { "vertex_buffer", mesh->get_vertex_buffer() } )
      .add_resource( { "shadow", linear_sampler, props.shadow, vk::ImageLayout::eShaderReadOnlyOptimal } )
  ) );
  const auto &vamap = draw_mesh->get_vamap();
  const auto vamap_position = vamap.find( vertex_attribute_usage_t::POSITION );
  if( vamap_position == vamap.end() ) {
    throw -1;
  }
  const auto vamap_position_offset = vamap_position->second.offset / sizeof( float );
  stride = draw_mesh->get_stride()/sizeof(float);
  vertex_offset = vamap_position_offset;

  clear_volumetric_light_texcoord.reset( new compute(
    compute_create_info()
      .set_allocator_set( props.allocator_set )
      .set_shader( props.clear_texcoord_shader )
      .set_swapchain_image_count( props.input.size() )
      .set_resources( props.resources )
      .add_resource( { "dest_image", volumetric_light_texcoord, vk::ImageLayout::eGeneral } )
  ) );
  
  calc_volumetric_light_texcoord.reset( new compute(
    compute_create_info()
      .set_allocator_set( props.allocator_set )
      .set_shader( props.generate_texcoord_shader )
      .set_swapchain_image_count( props.input.size() )
      .set_resources( props.resources )
      .add_resource( { "gbuffer", props.input, vk::ImageLayout::eGeneral } )
      .add_resource( { "dest_image", volumetric_light_texcoord, vk::ImageLayout::eGeneral } )
      .add_resource( { "vertex_buffer", mesh->get_vertex_buffer() } )
  ) );
  
  fill_volumetric_light_texcoord.reset( new compute(
    compute_create_info()
      .set_allocator_set( props.allocator_set )
      .set_shader( props.fill_texcoord_shader )
      .set_swapchain_image_count( props.input.size() )
      .set_resources( props.resources )
      .add_resource( { "dest_image", volumetric_light_texcoord, vk::ImageLayout::eGeneral } )
  ) );

  if( !props.hgauss_shader.empty() && !props.vgauss_shader.empty() ) {
    hgauss.reset( new compute(
      compute_create_info()
        .set_allocator_set( props.allocator_set )
        .set_shader( props.hgauss_shader )
        .set_swapchain_image_count( props.input.size() )
        .set_resources( props.resources )
        .add_resource( { "gbuffer", props.input, vk::ImageLayout::eGeneral } )
        .add_resource( { "src_image", output->get_image_views(), vk::ImageLayout::eGeneral } )
        .add_resource( { "dest_image", gauss_temp, vk::ImageLayout::eGeneral } )
    ) );
    
    vgauss.reset( new compute(
      compute_create_info()
        .set_allocator_set( props.allocator_set )
        .set_shader( props.vgauss_shader )
        .set_swapchain_image_count( props.input.size() )
        .set_resources( props.resources )
        .add_resource( { "gbuffer", props.input, vk::ImageLayout::eGeneral } )
        .add_resource( { "src_image", gauss_temp, vk::ImageLayout::eGeneral } )
        .add_resource( { "dest_image", output->get_image_views(), vk::ImageLayout::eGeneral } )
    ) );
  }
}

void epipolar_light_scattering::init(
  command_buffer_recorder_t &rec
) const {
  rec.set_image_layout( volumetric_light, vk::ImageLayout::eGeneral );
  rec.set_image_layout( volumetric_light_texcoord, vk::ImageLayout::eGeneral );
  rec.set_image_layout( gauss_temp, vk::ImageLayout::eGeneral );
  mesh->init( rec );
}

void epipolar_light_scattering::operator()(
  command_buffer_recorder_t &rec,
  unsigned int image_index,
  const epipolar_light_scattering_parameters_t &params_,
  const epipolar_light_scattering_texcoord_parameters_t &texcoord_params_,
  const glm::vec2 &light_pos_on_screen
) const {
  rec.set_image_layout( output->get_image_views(), vk::ImageLayout::eColorAttachmentOptimal );
  auto params = params_;
  params
    .set_stride( stride )
    .set_vertex_offset( vertex_offset );
  auto texcoord_params = texcoord_params_;
  texcoord_params
    .set_stride( stride )
    .set_vertex_offset( vertex_offset );
  (*mesh)( rec, image_index, light_pos_on_screen );

  rec.convert_image(
    volumetric_light[ image_index ]->get_factory(),
    vk::ImageLayout::eGeneral
  );

  rec->pushConstants(
    **calc_volumetric_light->get_pipeline()->get_props().get_layout(),
    vk::ShaderStageFlagBits::eCompute,
    0u,
    sizeof( epipolar_light_scattering_parameters_t ),
    reinterpret_cast< void* >( &params )
  );

  (*calc_volumetric_light)( rec, image_index, props.pole_count, props.vertex_count, 1u );
  
  rec.convert_image(
    volumetric_light_texcoord[ image_index ]->get_factory(),
    vk::ImageLayout::eGeneral
  );
  
  rec->pushConstants(
    **calc_volumetric_light_texcoord->get_pipeline()->get_props().get_layout(),
    vk::ShaderStageFlagBits::eCompute,
    0u,
    sizeof( epipolar_light_scattering_texcoord_parameters_t ),
    reinterpret_cast< void* >( &texcoord_params )
  );


  (*clear_volumetric_light_texcoord)( rec, image_index, props.pole_count, props.vertex_count * props.texel_per_sample, 1u );

  rec.compute_barrier(
    {},
    { volumetric_light_texcoord[ image_index ]->get_factory() }
  );
  
  (*calc_volumetric_light_texcoord)( rec, image_index, props.pole_count, props.vertex_count, 1u );
  
  rec.compute_barrier(
    {},
    { volumetric_light_texcoord[ image_index ]->get_factory() }
  );
  
  (*fill_volumetric_light_texcoord)( rec, image_index, props.pole_count, props.vertex_count * props.texel_per_sample, 1u );
  
  rec.convert_image(
    volumetric_light_texcoord[ image_index ]->get_factory(),
    vk::ImageLayout::eShaderReadOnlyOptimal
  );

  rec.convert_image(
    volumetric_light[ image_index ]->get_factory(),
    vk::ImageLayout::eShaderReadOnlyOptimal
  );

  {
    auto render_pass_token = rec.begin_render_pass(
      output->get_render_pass_begin_info( image_index ),
      vk::SubpassContents::eInline
    );
    draw_mesh->bind( rec, image_index );
    rec.bind_index_buffer( mesh->get_index_buffer(), vk::IndexType::eUint16 );
    rec.bind_vertex_buffer( mesh->get_vertex_buffer()[ image_index ]->get_buffer() );
    rec->drawIndexed( mesh->get_index_count(), 1u, 0u, 0, 0u );
  }
  rec.convert_image(
    output->get_image( image_index ),
    vk::ImageLayout::eGeneral
  );

  if( !props.hgauss_shader.empty() && !props.vgauss_shader.empty() ) {
    (*hgauss)( rec, image_index,
      props.input[ 0 ]->get_factory()->get_props().get_basic().extent.width,
      props.input[ 0 ]->get_factory()->get_props().get_basic().extent.height,
      1u
    );
 
    rec.compute_barrier(
      {},
      { gauss_temp[ image_index ]->get_factory() }
    );
    
    (*vgauss)( rec, image_index,
      props.input[ 0 ]->get_factory()->get_props().get_basic().extent.width,
      props.input[ 0 ]->get_factory()->get_props().get_basic().extent.height,
      1u
    );
    
    rec.compute_barrier(
      {},
      { output->get_image( image_index ) }
    );
  }

}

const std::vector< std::shared_ptr< image_view_t > > &epipolar_light_scattering::get_output() const {
  return output->get_image_views();
}

}

