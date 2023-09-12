#include <nlohmann/json.hpp>
#include <gct/allocator.hpp>
#include <gct/render_pass.hpp>
#include <gct/render_pass_create_info.hpp>
#include <gct/image_view.hpp>
#include <gct/image_view_create_info.hpp>
#include <gct/framebuffer.hpp>
#include <gct/framebuffer_create_info.hpp>
#include <gct/device.hpp>
#include <gct/color.hpp>
#include <gct/image.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/gbuffer.hpp>

namespace gct {

gbuffer::gbuffer(
  const std::shared_ptr< allocator_t > &allocator,
  unsigned int width,
  unsigned int height,
  unsigned int swapchain_image_count,
  unsigned int color_buffer_count
) : gbuffer(
  gbuffer_create_info()
    .set_allocator( allocator )
    .set_width( width )
    .set_height( height )
    .set_swapchain_image_count( swapchain_image_count )
    .set_color_buffer_count( color_buffer_count )
) {}

gbuffer::gbuffer(
  const gbuffer_create_info &ci
) :
  props( ci ),
  created_from< allocator_t >( ci.allocator ) {
  {
    auto rpci = render_pass_create_info_t();
    auto sd = subpass_description_t();
    for( std::size_t i = 0u; i != ci.color_buffer_count; ++i ) {
      rpci.add_attachment(
        vk::AttachmentDescription()
          .setFormat( ci.format )
          .setSamples( vk::SampleCountFlagBits::e1 )
          .setLoadOp( ci.clear_color[ 3 ] == 1.0 ? vk::AttachmentLoadOp::eClear : vk::AttachmentLoadOp::eLoad )
          .setStoreOp( vk::AttachmentStoreOp::eStore )
          .setStencilLoadOp( vk::AttachmentLoadOp::eDontCare )
          .setStencilStoreOp( vk::AttachmentStoreOp::eDontCare )
          .setInitialLayout( ci.clear_color[ 3 ] == 1.0 ? vk::ImageLayout::eUndefined : ci.final_layout )
          .setFinalLayout( ci.final_layout )
      );
      sd.add_color_attachment(
        i, vk::ImageLayout::eColorAttachmentOptimal
      );
    }
    if( ci.has_depth ) {
      rpci.add_attachment(
        vk::AttachmentDescription()
          .setFormat( vk::Format::eD32Sfloat )
          .setSamples( vk::SampleCountFlagBits::e1 )
          .setLoadOp( ci.external_depth.empty() ? vk::AttachmentLoadOp::eClear : vk::AttachmentLoadOp::eLoad )
          .setStoreOp( vk::AttachmentStoreOp::eStore )
          .setStencilLoadOp( vk::AttachmentLoadOp::eDontCare )
          .setStencilStoreOp( vk::AttachmentStoreOp::eDontCare )
          .setInitialLayout( ci.external_depth.empty() ? vk::ImageLayout::eUndefined : vk::ImageLayout::eDepthStencilAttachmentOptimal )
          .setFinalLayout( vk::ImageLayout::eDepthStencilAttachmentOptimal )
      );
      sd.set_depth_stencil_attachment( ci.color_buffer_count, vk::ImageLayout::eDepthStencilAttachmentOptimal );
    }
    rpci.add_subpass( std::move( sd ) );
    render_pass = ci.allocator->get_factory()->get_render_pass( rpci );
  }
  for( std::size_t i = 0u; i != ci.swapchain_image_count; ++i ) {
    auto gbuf = ci.allocator->create_image(
      image_create_info_t()
        .set_basic(
          vk::ImageCreateInfo()
            .setImageType( vk::ImageType::e2D )
            .setFlags( ci.flags )
            .setFormat( ci.format )
            .setExtent( { ci.width, ci.height, 1 } )
            .setArrayLayers( ci.color_buffer_count * ci.layer )
            .setUsage(
              vk::ImageUsageFlagBits::eColorAttachment|
              vk::ImageUsageFlagBits::eSampled|
              vk::ImageUsageFlagBits::eStorage|
              vk::ImageUsageFlagBits::eTransferSrc|
              vk::ImageUsageFlagBits::eTransferDst
            )
        )
        .rebuild_chain(),
      VMA_MEMORY_USAGE_GPU_ONLY
    );
    image_views.push_back(
      gbuf->get_view( vk::ImageAspectFlagBits::eColor )
    );
    auto framebuffer_create_info =
      framebuffer_create_info_t()
        .set_basic(
          vk::FramebufferCreateInfo()
            .setLayers( ci.layer )
        );
    auto rpbi = render_pass_begin_info_t();
    for( unsigned int i = 0u; i != ci.color_buffer_count; ++i ) {
      framebuffer_create_info.add_attachment(
        gbuf->get_view(
          image_view_create_info_t()
            .set_basic(
              vk::ImageViewCreateInfo()
                .setViewType( ci.layer > 1u ? vk::ImageViewType::e2DArray : vk::ImageViewType::e2D )
                .setSubresourceRange(
                  vk::ImageSubresourceRange()
                    .setAspectMask( vk::ImageAspectFlagBits::eColor )
                    .setBaseMipLevel( 0 )
                    .setLevelCount( 1 )
                    .setBaseArrayLayer( i * ci.layer )
                    .setLayerCount( ci.layer )
                )
            )
        )
      );
      rpbi.add_clear_value( vk::ClearColorValue( ci.clear_color ) );
    }
    if( ci.has_depth ) {
      if( !ci.external_depth.empty() ) {
        framebuffer_create_info.add_attachment(
          ci.external_depth[ i ]
        );
      }
      else {
        const auto depth = ci.allocator->create_image(
          gct::image_create_info_t()
            .set_basic(
              vk::ImageCreateInfo()
                .setImageType( vk::ImageType::e2D )
                .setFlags( ci.flags )
                .setFormat( vk::Format::eD32Sfloat )
                .setExtent( { ci.width, ci.height, 1 } )
                .setArrayLayers( ci.layer )
                .setUsage( vk::ImageUsageFlagBits::eDepthStencilAttachment )
            )
            .rebuild_chain(),
          VMA_MEMORY_USAGE_GPU_ONLY
        );
        const auto depth_view = 
          depth->get_view(
            image_view_create_info_t()
              .set_basic(
                vk::ImageViewCreateInfo()
                  .setViewType( ci.layer > 1u ? vk::ImageViewType::e2DArray : vk::ImageViewType::e2D )
                  .setSubresourceRange(
                    vk::ImageSubresourceRange()
                      .setAspectMask( vk::ImageAspectFlagBits::eDepth )
                      .setBaseMipLevel( 0 )
                      .setLevelCount( 1 )
                      .setBaseArrayLayer( 0 )
                      .setLayerCount( ci.layer )
                  )
              )
          );
        framebuffer_create_info.add_attachment( depth_view );
        depth_views.push_back( depth_view );
      }
      rpbi.add_clear_value( props.initial_depth );
    }
    auto framebuffer = render_pass->get_framebuffer( framebuffer_create_info );
    rpbi
      .set_render_pass( render_pass )
      .set_framebuffer( framebuffer );
    rpbis.emplace_back( std::move( rpbi ) );
    rpbis.back().rebuild_chain();
    viewport =
      vk::Viewport()
        .setWidth( ci.width )
        .setHeight( ci.height )
        .setMinDepth( 0.0f )
        .setMaxDepth( 1.0f );
    scissor = vk::Rect2D( vk::Offset2D(0, 0), vk::Extent2D( ci.width, ci.height ) );
  }
}
void gbuffer::clear_color(
  command_buffer_recorder_t &rec,
  unsigned int image_index,
  const std::vector< vk::ImageSubresourceRange > &range
) {
  for( const auto &r: range ) {
    rec.convert_image(
      image_views[ image_index ]->get_factory(),
      r.baseMipLevel,
      r.levelCount,
      r.baseArrayLayer,
      r.layerCount,
      vk::ImageLayout::eTransferDstOptimal
    );
  }
  rec->clearColorImage(
    **image_views[ image_index ]->get_factory(),
    vk::ImageLayout::eTransferDstOptimal,
    props.clear_color,
    range
  );
  for( const auto &r: range ) {
    rec.convert_image(
      image_views[ image_index ]->get_factory(),
      r.baseMipLevel,
      r.levelCount,
      r.baseArrayLayer,
      r.layerCount,
      props.final_layout
    );
  }
}
void gbuffer::clear_color(
  command_buffer_recorder_t &rec,
  unsigned int image_index
) {
  rec.convert_image(
    image_views[ image_index ]->get_factory(),
    vk::ImageLayout::eTransferDstOptimal
  );
  rec->clearColorImage(
    **image_views[ image_index ]->get_factory(),
    vk::ImageLayout::eTransferDstOptimal,
    props.clear_color,
    {
      vk::ImageSubresourceRange()
        .setAspectMask( vk::ImageAspectFlagBits::eColor )
        .setBaseMipLevel( 0 )
        .setLevelCount( 1 )
        .setBaseArrayLayer( 0 )
        .setLayerCount( props.color_buffer_count * props.layer )
    }
  );
  rec.convert_image(
    image_views[ image_index ]->get_factory(),
    props.final_layout
  );
}
const std::shared_ptr< image_t > &gbuffer::get_image( unsigned int i ) const {
  return get_image_view( i )->get_factory();
}

void to_json( nlohmann::json &dest, const gbuffer &src ) {
  dest = src.get_props();
}

}

