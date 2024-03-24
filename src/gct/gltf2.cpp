#include <fstream>
#include <iterator>
#include <algorithm>
#include <gct/device.hpp>
#include <gct/shader_module_reflection.hpp>
#include <gct/descriptor_pool_create_info.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/descriptor_set_layout_create_info.hpp>
#include <gct/descriptor_set.hpp>
#include <gct/sampler_create_info.hpp>
#include <gct/sampler.hpp>
#include <gct/allocator.hpp>
#include <gct/mappable_buffer.hpp>
#include <gct/get_device.hpp>
#include <gct/gltf2.hpp>
#include <gct/gltf.hpp>
#include <vulkan/vulkan_structs.hpp>
namespace gct::gltf {

gltf2::gltf2(
  const gltf2_create_info &ci
) : props( ci ) {
  fx::gltf::Document doc = fx::gltf::LoadFromText( props.filename.string() );
  cd = props.filename.parent_path();
  load_buffer( doc );
  load_sampler( doc );
}

void gltf2::load_buffer(
  const fx::gltf::Document &doc
) {
  const auto allocator = props.graph->get_props().allocator;
  for( const auto &b: doc.buffers ) {
    auto buffer_path = std::filesystem::path( b.uri );
    if( buffer_path.is_relative() ) buffer_path = cd / buffer_path;
    const auto size = std::filesystem::file_size( buffer_path );
    auto buf = allocator->create_mappable_buffer(
      buffer_create_info_t()
        .set_basic(
          vk::BufferCreateInfo()
            .setSize( size )
            .setUsage(
              vk::BufferUsageFlagBits::eVertexBuffer|
              vk::BufferUsageFlagBits::eIndexBuffer
            )
        )
    );
    {
      std::ifstream file( buffer_path.string(), std::ios::in | std::ios::binary );
      std::vector< std::uint8_t > data(
        ( std::istreambuf_iterator< char >( file ) ),
        std::istreambuf_iterator< char >()
      );
      auto range = buf->map< std::uint8_t >();
      std::copy(
        data.begin(),
        data.end(),
        range.begin()
      );
    }
    buffer.push_back(
      buffer_state()
        .set_filename( buffer_path )
        .set_buffer( std::move( buf ) )
    );
  }
}

std::unordered_map< shader_flag_t, std::shared_ptr< shader_module_t > > gltf2::load_shader(
  const fx::gltf::Document &doc,
  const std::filesystem::path &shader_dir
) {
  const auto allocator = props.graph->get_props().allocator;
  std::unordered_map< shader_flag_t, std::shared_ptr< shader_module_t > > shader;
  for( auto &path: std::filesystem::directory_iterator( shader_dir ) ) {
    auto flag = get_shader_flag( path.path() );
    if( flag ) {
      shader.emplace(
        *flag,
        get_device( *allocator ).get_shader_module(
          path.path().string()
        )
      );
    }
  }
  return shader;
}

void gltf2::load_sampler(
  const fx::gltf::Document &doc
) {
  const auto allocator = props.graph->get_props().allocator;
  for( unsigned int i = 0u; i != doc.samplers.size(); ++i ) {
    const auto &s = doc.samplers[ i ];
    sampler.push_back(
      get_device( *allocator ).get_sampler(
        sampler_create_info_t()
          .set_basic(
            vk::SamplerCreateInfo()
              .setMagFilter( to_vulkan_mag_filter( s.magFilter ) )
              .setMinFilter( to_vulkan_min_filter( s.minFilter ) )
              .setMipmapMode( to_vulkan_mipmap_mode( s.minFilter ) )
              .setAddressModeU( to_vulkan_address_mode( s.wrapS ) )
              .setAddressModeV( to_vulkan_address_mode( s.wrapT ) )
              .setAddressModeW( to_vulkan_address_mode( s.wrapT ) )
              .setAnisotropyEnable( false )
              .setCompareEnable( false )
              .setMipLodBias( 0.f )
              .setMinLod( 0.f )
              .setMaxLod( 32.f )
              .setBorderColor( vk::BorderColor::eFloatTransparentBlack )
              .setUnnormalizedCoordinates( false )
          )
      )
    );
  }
}

/*
gltf2::create_pipeline() {
  std::unordered_map< uint32_t, buffer_window_t > vertex_buffer;
  std::vector< vk::VertexInputBindingDescription > vertex_input_binding;
  std::vector< vk::VertexInputAttributeDescription > vertex_input_attribute;
  world.reset( new graphics(
    graphics_create_info()
      .set_allocator( props.allocator )
      .set_pipeline_cache( props.pipeline_cache )
      .set_descriptor_pool( props.descriptor_pool )
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
}
*/
}

