#include <gct/device.hpp>
#include <gct/allocator.hpp>
#include <gct/image_create_info.hpp>
#include <gct/swapchain.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/descriptor_set_layout.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/pipeline_layout_create_info.hpp>
#include <gct/pipeline_viewport_state_create_info.hpp>
#include <gct/pipeline_dynamic_state_create_info.hpp>
#include <gct/pipeline_input_assembly_state_create_info.hpp>
#include <gct/pipeline_vertex_input_state_create_info.hpp>
#include <gct/pipeline_multisample_state_create_info.hpp>
#include <gct/pipeline_tessellation_state_create_info.hpp>
#include <gct/pipeline_rasterization_state_create_info.hpp>
#include <gct/pipeline_depth_stencil_state_create_info.hpp>
#include <gct/pipeline_color_blend_state_create_info.hpp>
#include <gct/sampler_create_info.hpp>
#include <gct/graphics_pipeline_create_info.hpp>
#include <gct/graphics_pipeline.hpp>
#include <gct/pipeline_layout.hpp>
#include <gct/vertex_attributes.hpp>
#include <gct/render_pass_begin_info.hpp>
#include <gct/image.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/primitive.hpp>
#include <gct/canvas.hpp>

namespace gct {

rect_renderer::rect_renderer(
  const std::shared_ptr< device_t > &device_,
  const std::shared_ptr< allocator_t > &allocator_,
  const std::shared_ptr< pipeline_cache_t > &pipeline_cache,
  const std::shared_ptr< descriptor_pool_t > &descriptor_pool,
  const std::shared_ptr< shader_module_t > &vs_,
  const std::shared_ptr< shader_module_t > &fs_,
  const std::shared_ptr< render_pass_t > &render_pass,
  const vk::Extent2D &size_,
  const simple_shape &shape_,
  const std::vector< std::shared_ptr< image_view_t > > &input_images_
) :
  device( device_ ),
  allocator( allocator_ ),
  vs( vs_ ),
  fs( fs_ ),
  size( size_ ),
  uniforms_host( size_ ),
  shape( shape_ ),
  input_images( input_images_ )
{
  const auto length = std::max( size.width, size.height );
  scale = 1u;
  for( unsigned int i = 0u; i != 15; ++i ) {
    if( length <= ( 1 << i ) ) {
      scale = 1u << ( 15 - i );
      break;
    }
  }
  uniforms_host.scale_factor = 1.f / float( scale );
  {
    descriptor_set_layout = device->get_descriptor_set_layout(
      gct::descriptor_set_layout_create_info_t()
        .add_binding( vs->get_props().get_reflection() )
        .add_binding( fs->get_props().get_reflection() )
    );
  }
  descriptor_set = descriptor_pool->allocate( descriptor_set_layout );
  pipeline_layout = device->get_pipeline_layout(
    gct::pipeline_layout_create_info_t()
      .add_descriptor_set_layout( descriptor_set_layout )
  );
  auto [vistat,vamap,stride] = get_vertex_attributes(
    *device,
    vs->get_props().get_reflection()
  );
  //const auto [input_assembly,host_vertex_buffer_,vertex_count] = gct::primitive::create_plane( vamap, stride, true );
  //host_vertex_buffer = std::move( host_vertex_buffer_ );
  const auto viewport =
    gct::pipeline_viewport_state_create_info_t()
      .add_viewport(
        vk::Viewport()
          .setWidth( size.width )
          .setHeight( size.height )
          .setMinDepth( 0.0f )
          .setMaxDepth( 1.0f )
      )
      .add_scissor(
        vk::Rect2D()
          .setOffset( { 0, 0 } )
          .setExtent( size )
      )
      .rebuild_chain();

  const auto rasterization =
    gct::pipeline_rasterization_state_create_info_t()
      .set_basic(
        vk::PipelineRasterizationStateCreateInfo()
          .setDepthClampEnable( false )
          .setRasterizerDiscardEnable( false )
          .setPolygonMode( vk::PolygonMode::eFill )
          .setCullMode( vk::CullModeFlagBits::eNone )
          .setFrontFace( vk::FrontFace::eClockwise )
          .setDepthBiasEnable( false )
          .setLineWidth( 1.0f )
      );

  const auto multisample =
    gct::pipeline_multisample_state_create_info_t()
      .set_basic(
        vk::PipelineMultisampleStateCreateInfo()
      );

  const auto stencil_op = vk::StencilOpState()
    .setCompareOp( vk::CompareOp::eAlways )
    .setFailOp( vk::StencilOp::eKeep )
    .setPassOp( vk::StencilOp::eKeep );

  const auto depth_stencil =
    gct::pipeline_depth_stencil_state_create_info_t()
      .set_basic(
        vk::PipelineDepthStencilStateCreateInfo()
          .setDepthTestEnable( true )
          .setDepthWriteEnable( true )
          .setDepthCompareOp( vk::CompareOp::eLessOrEqual )
          .setDepthBoundsTestEnable( false )
          .setStencilTestEnable( false )
          .setFront( stencil_op )
          .setBack( stencil_op )
      );

  const auto color_blend =
    gct::pipeline_color_blend_state_create_info_t()
      .add_attachment(
        vk::PipelineColorBlendAttachmentState()
          .setBlendEnable( false )
          .setColorWriteMask(
            vk::ColorComponentFlagBits::eR |
            vk::ColorComponentFlagBits::eG |
            vk::ColorComponentFlagBits::eB |
            vk::ColorComponentFlagBits::eA
          )
      )
      .add_attachment(
        vk::PipelineColorBlendAttachmentState()
          .setBlendEnable( false )
          .setColorWriteMask(
            vk::ColorComponentFlagBits::eR |
            vk::ColorComponentFlagBits::eG
          )
      );
  const auto input_assembly = pipeline_input_assembly_state_create_info_t()
    .set_basic(
      vk::PipelineInputAssemblyStateCreateInfo()
        .setTopology( vk::PrimitiveTopology::eTriangleList )
    );
  pipeline = pipeline_cache->get_pipeline(
    gct::graphics_pipeline_create_info_t()
      .add_stage( vs )
      .add_stage( fs )
      .set_vertex_input( vistat )
      .set_input_assembly( input_assembly )
      .set_viewport( viewport )
      .set_rasterization( rasterization )
      .set_multisample( multisample )
      .set_depth_stencil( depth_stencil )
      .set_color_blend( color_blend )
      .set_dynamic(
        gct::pipeline_dynamic_state_create_info_t()
      )
      .set_layout( pipeline_layout )
      .set_render_pass( render_pass, 0 )
  );
  uniforms_staging = allocator->create_buffer(
    gct::buffer_create_info_t()
      .set_basic(
        vk::BufferCreateInfo()
          .setSize( sizeof( uniform_t ) )
          .setUsage( vk::BufferUsageFlagBits::eTransferSrc )
      ),
    VMA_MEMORY_USAGE_CPU_TO_GPU
  );
  uniforms_device = allocator->create_buffer(
    gct::buffer_create_info_t()
      .set_basic(
        vk::BufferCreateInfo()
          .setSize( sizeof( uniform_t ) )
          .setUsage( vk::BufferUsageFlagBits::eTransferDst|vk::BufferUsageFlagBits::eStorageBuffer )
      ),
    VMA_MEMORY_USAGE_GPU_ONLY
  );
  sampler = device->get_sampler(
    gct::sampler_create_info_t()
      .set_basic(
        vk::SamplerCreateInfo()
          .setMagFilter( vk::Filter::eNearest )
          .setMinFilter( vk::Filter::eNearest )
          .setMipmapMode( vk::SamplerMipmapMode::eNearest )
          .setAddressModeU( vk::SamplerAddressMode::eClampToBorder )
          .setAddressModeV( vk::SamplerAddressMode::eClampToBorder )
          .setAddressModeW( vk::SamplerAddressMode::eClampToBorder )
          .setAnisotropyEnable( false )
          .setCompareEnable( false )
          .setMipLodBias( 0.f )
          .setMinLod( 0.f )
          .setMaxLod( 0.f )
          .setBorderColor( vk::BorderColor::eFloatTransparentBlack )
          .setUnnormalizedCoordinates( false )
      )
  );

  std::vector< gct::write_descriptor_set_t > updates;
  updates.push_back(
    gct::write_descriptor_set_t()
      .set_basic(
        (*descriptor_set)[ "uniforms" ]
      )
      .add_buffer(
        gct::descriptor_buffer_info_t()
          .set_buffer( uniforms_device )
          .set_basic(
            vk::DescriptorBufferInfo()
              .setOffset( 0 )
              .setRange( sizeof( uniform_t ) )
          )
      )
  );
  std::uint32_t tex_id = 0u;
  for( const auto &image: input_images ) {
    updates.push_back(
      gct::write_descriptor_set_t()
        .set_basic(
          (*descriptor_set)[ "tex" ]
            .setDstArrayElement( tex_id )
            .setDescriptorCount( 1 )
        )
        .add_image(
          gct::descriptor_image_info_t()
            .set_sampler( sampler )
            .set_image_view( image )
            .set_basic(
              vk::DescriptorImageInfo()
                .setImageLayout(
                  vk::ImageLayout::eShaderReadOnlyOptimal
                )
            )
        )
    );
    ++tex_id;
  }
  descriptor_set->update( updates );
}

void rect_renderer::load(
  gct::command_buffer_recorder_t &recorder
) {
  /*if( !vertex_loaded ) {
    load_vertex_buffer( recorder );
    vertex_loaded = true;
  }*/
  transfer_uniforms( recorder );
}
void rect_renderer::operator()(
  gct::command_buffer_recorder_t &recorder
) {
  draw( recorder );
}
/*void rect_renderer::load_vertex_buffer(
  gct::command_buffer_recorder_t &recorder
) {
  vertex_buffer = recorder.load_buffer(
    allocator,
    host_vertex_buffer.data(),
    host_vertex_buffer.size(),
    vk::BufferUsageFlagBits::eVertexBuffer
  );
  recorder.barrier(
    vk::AccessFlagBits::eTransferWrite,
    vk::AccessFlagBits::eVertexAttributeRead,
    vk::PipelineStageFlagBits::eTransfer,
    vk::PipelineStageFlagBits::eVertexInput,
    vk::DependencyFlagBits( 0 ),
    { vertex_buffer },
    {}
  );
}*/
void rect_renderer::transfer_uniforms( gct::command_buffer_recorder_t &recorder ) {
  recorder.copy(
    uniforms_host,
    uniforms_staging,
    uniforms_device
  );
  recorder.barrier(
    vk::AccessFlagBits::eTransferWrite,
    vk::AccessFlagBits::eShaderRead,
    vk::PipelineStageFlagBits::eTransfer,
    vk::PipelineStageFlagBits::eVertexShader,
    vk::DependencyFlagBits( 0 ),
    { uniforms_device },
    {}
  );
}
void rect_renderer::draw(
  gct::command_buffer_recorder_t &recorder
) {
  recorder.bind_pipeline( pipeline );
  recorder.bind_descriptor_set(
    vk::PipelineBindPoint::eGraphics,
    pipeline_layout,
    descriptor_set
  );
  recorder.bind_vertex_buffer( shape.vertex_buffer );
  recorder->draw( shape.vertex_count, instance_count, 0, 0 );
}
void rect_renderer::add_rectangle_internal(
  const vk::Rect2D &rect,
  const std::array< float, 4u > &color, 
  std::uint16_t depth,
  std::uint16_t semantic_id,
  std::uint16_t texid
) {
  const auto instance_id = instance_count++;
  uniforms_host.rects[ instance_id ].offset[ 0 ] = rect.offset.x * scale;
  uniforms_host.rects[ instance_id ].offset[ 1 ] = rect.offset.y * scale;
  uniforms_host.rects[ instance_id ].extent[ 0 ] = rect.extent.width * scale;
  uniforms_host.rects[ instance_id ].extent[ 1 ] = rect.extent.height * scale;
  uniforms_host.rects[ instance_id ].color = glm::vec4( color[ 0 ], color[ 1 ], color[ 2 ], color[ 3 ] );
  uniforms_host.rects[ instance_id ].depth = depth;
  uniforms_host.rects[ instance_id ].texid = texid;
  uniforms_host.rects[ instance_id ].semantic[ 0 ] = 0;
  uniforms_host.rects[ instance_id ].semantic[ 1 ] = semantic_id;
}

void rect_renderer::add_rectangle( const vk::Rect2D &rect, const std::array< float, 4u > &color, std::uint16_t depth, bool fill, std::uint32_t border_width, std::uint16_t texid, std::uint16_t semid ) {
  if( fill ) {
    add_rectangle_internal( rect, color, depth, semid, texid );
  }
  else {
    add_rectangle_internal(
      vk::Rect2D{
        vk::Offset2D{
          rect.offset.x,
          rect.offset.y
        },
        vk::Extent2D{
          rect.extent.width,
          border_width
        }
      },
      color, depth, semid, texid
    );
    add_rectangle_internal(
      vk::Rect2D{
        vk::Offset2D{
          rect.offset.x,
          std::int32_t( rect.offset.y + rect.extent.height - border_width )
        },
        vk::Extent2D{
          rect.extent.width,
          border_width
        }
      },
      color, depth, semid, texid
    );
    add_rectangle_internal(
      vk::Rect2D{
        vk::Offset2D{
          rect.offset.x,
          std::int32_t( rect.offset.y + border_width )
        },
        vk::Extent2D{
          border_width,
          std::uint32_t( rect.extent.height - border_width * 2u )
        }
      },
      color, depth, semid, texid
    );
    add_rectangle_internal(
      vk::Rect2D{
        vk::Offset2D{
          std::int32_t( rect.offset.x + rect.extent.width - border_width ),
          std::int32_t( rect.offset.y + border_width )
        },
        vk::Extent2D{
          border_width,
          std::uint32_t( rect.extent.height - border_width * 2u )
        }
      },
      color, depth, semid, texid
    );
  }
}
void rect_renderer::reset() {
  instance_count = 0;
}

bool rect_renderer::full() const {
  return instance_count == 65536;
}

}

