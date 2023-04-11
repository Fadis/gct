#include <gct/allocator.hpp>
#include <gct/render_pass.hpp>
#include <gct/render_pass_create_info.hpp>
#include <gct/image_view.hpp>
#include <gct/image_view_create_info.hpp>
#include <gct/framebuffer.hpp>
#include <gct/framebuffer_create_info.hpp>
#include <gct/device.hpp>
#include <gct/color.hpp>
#include <gct/gbuffer.hpp>

namespace gct {

gbuffer::gbuffer(
  const std::shared_ptr< allocator_t > &allocator,
  unsigned int width,
  unsigned int height,
  unsigned int swapchain_image_count,
  unsigned int color_buffer_count
) :
  created_from< allocator_t >( allocator ) {
  {
    auto ci = render_pass_create_info_t();
    auto sd = subpass_description_t();
    for( std::size_t i = 0u; i != color_buffer_count; ++i ) {
      ci.add_attachment(
        vk::AttachmentDescription()
          .setFormat( vk::Format::eR32G32B32A32Sfloat )
          .setSamples( vk::SampleCountFlagBits::e1 )
          .setLoadOp( vk::AttachmentLoadOp::eClear )
          .setStoreOp( vk::AttachmentStoreOp::eStore )
          .setStencilLoadOp( vk::AttachmentLoadOp::eDontCare )
          .setStencilStoreOp( vk::AttachmentStoreOp::eDontCare )
          .setInitialLayout( vk::ImageLayout::eUndefined )
          .setFinalLayout( vk::ImageLayout::eGeneral )
      );
      sd.add_color_attachment(
        i, vk::ImageLayout::eColorAttachmentOptimal
      );
    }
    ci.add_attachment(
      vk::AttachmentDescription()
        .setFormat( vk::Format::eD32Sfloat )
        .setSamples( vk::SampleCountFlagBits::e1 )
        .setLoadOp( vk::AttachmentLoadOp::eClear )
        .setStoreOp( vk::AttachmentStoreOp::eStore )
        .setStencilLoadOp( vk::AttachmentLoadOp::eDontCare )
        .setStencilStoreOp( vk::AttachmentStoreOp::eDontCare )
        .setInitialLayout( vk::ImageLayout::eUndefined )
        .setFinalLayout( vk::ImageLayout::eDepthStencilAttachmentOptimal )
    );
    sd.set_depth_stencil_attachment( color_buffer_count, vk::ImageLayout::eDepthStencilAttachmentOptimal );
    ci.add_subpass( std::move( sd ) );
    render_pass = allocator->get_factory()->get_render_pass( ci );
  }
  for( std::size_t i = 0u; i != swapchain_image_count; ++i ) {
    auto gbuf = allocator->create_image(
      image_create_info_t()
        .set_basic(
          vk::ImageCreateInfo()
            .setImageType( vk::ImageType::e2D )
            .setFormat( vk::Format::eR32G32B32A32Sfloat )
            .setExtent( { width, height, 1 } )
            .setArrayLayers( color_buffer_count )
            .setUsage(
              vk::ImageUsageFlagBits::eColorAttachment|
              vk::ImageUsageFlagBits::eStorage
            )
        )
        .rebuild_chain(),
      VMA_MEMORY_USAGE_GPU_ONLY
    );
    image_views.push_back(
      gbuf->get_view( vk::ImageAspectFlagBits::eColor )
    );
    auto framebuffer_create_info = framebuffer_create_info_t();
    auto rpbi = render_pass_begin_info_t();
    for( unsigned int i = 0u; i != color_buffer_count; ++i ) {
      framebuffer_create_info.add_attachment(
        gbuf->get_view(
          image_view_create_info_t()
            .set_basic(
              vk::ImageViewCreateInfo()
                .setViewType( vk::ImageViewType::e2D )
                .setSubresourceRange(
                  vk::ImageSubresourceRange()
                    .setAspectMask( vk::ImageAspectFlagBits::eColor )
                    .setBaseMipLevel( 0 )
                    .setLevelCount( 1 )
                    .setBaseArrayLayer( i )
                    .setLayerCount( 1 )
                )
            )
        )
      );
      rpbi.add_clear_value( vk::ClearColorValue( gct::color::web::black ) );
    }
    auto depth = allocator->create_image(
      gct::image_create_info_t()
        .set_basic(
          vk::ImageCreateInfo()
            .setImageType( vk::ImageType::e2D )
            .setFormat( vk::Format::eD32Sfloat )
            .setExtent( { width, height, 1 } )
            .setUsage( vk::ImageUsageFlagBits::eDepthStencilAttachment )
        )
        .rebuild_chain(),
      VMA_MEMORY_USAGE_GPU_ONLY
    );
    framebuffer_create_info.add_attachment(
      depth->get_view( vk::ImageAspectFlagBits::eDepth )
    );
    rpbi.add_clear_value( vk::ClearDepthStencilValue( 1.f, 0 ) );
    auto framebuffer = render_pass->get_framebuffer( framebuffer_create_info );
    rpbi
      .set_render_pass( render_pass )
      .set_framebuffer( framebuffer );
    rpbis.emplace_back( std::move( rpbi ) );
    rpbis.back().rebuild_chain();
  }
}
}

