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


const std::shared_ptr< gct::image_t > &canvas::get_color() {
  return color;
}

canvas::canvas(
  const std::shared_ptr< gct::device_t > &device_,
  const std::shared_ptr< gct::allocator_t > &allocator_,
  const std::shared_ptr< gct::pipeline_cache_t > &pipeline_cache_,
  const std::shared_ptr< gct::descriptor_pool_t > &descriptor_pool_,
  const std::shared_ptr< gct::shader_module_t > &vs_,
  const std::shared_ptr< gct::shader_module_t > &fs_,
  const vk::Extent2D &size_,
  const std::array< float, 4u > &clear_color,
  const std::vector< simple_shape > &shape_,
  const std::vector< font > &font_,
  const std::vector< std::shared_ptr< image_view_t > > &input_images_
) :
  device( device_ ),
  allocator( allocator_ ),
  pipeline_cache( pipeline_cache_ ),
  descriptor_pool( descriptor_pool_ ),
  vs( vs_ ),
  fs( fs_ ),
  size( size_ ),
  shape( shape_ ),
  font_info( font_ ),
  input_images( input_images_ )
{
  render_pass = device->get_render_pass(
    gct::render_pass_create_info_t()
      .add_attachment(
        vk::AttachmentDescription()
          .setFormat( vk::Format::eR16G16B16A16Sfloat )
          .setSamples( vk::SampleCountFlagBits::e1 )
          .setLoadOp( vk::AttachmentLoadOp::eClear )
          .setStoreOp( vk::AttachmentStoreOp::eStore )
          .setStencilLoadOp( vk::AttachmentLoadOp::eDontCare )
          .setStencilStoreOp( vk::AttachmentStoreOp::eDontCare )
          .setInitialLayout( vk::ImageLayout::eUndefined )
          .setFinalLayout( vk::ImageLayout::eColorAttachmentOptimal )
      )
      .add_attachment(
        vk::AttachmentDescription()
          .setFormat( vk::Format::eR32G32Sfloat )
          .setSamples( vk::SampleCountFlagBits::e1 )
          .setLoadOp( vk::AttachmentLoadOp::eClear )
          .setStoreOp( vk::AttachmentStoreOp::eStore )
          .setStencilLoadOp( vk::AttachmentLoadOp::eDontCare )
          .setStencilStoreOp( vk::AttachmentStoreOp::eDontCare )
          .setInitialLayout( vk::ImageLayout::eUndefined )
          .setFinalLayout( vk::ImageLayout::eColorAttachmentOptimal )
      )
      .add_attachment(
        vk::AttachmentDescription()
          .setFormat( vk::Format::eD16Unorm )
          .setSamples( vk::SampleCountFlagBits::e1 )
          .setLoadOp( vk::AttachmentLoadOp::eClear )
          .setStoreOp( vk::AttachmentStoreOp::eStore )
          .setStencilLoadOp( vk::AttachmentLoadOp::eDontCare )
          .setStencilStoreOp( vk::AttachmentStoreOp::eDontCare )
          .setInitialLayout( vk::ImageLayout::eUndefined )
          .setFinalLayout( vk::ImageLayout::eDepthStencilAttachmentOptimal )
      )
      .add_subpass(
        gct::subpass_description_t()
          .add_color_attachment( 0, vk::ImageLayout::eColorAttachmentOptimal )
          .add_color_attachment( 1, vk::ImageLayout::eColorAttachmentOptimal )
          .set_depth_stencil_attachment( 2, vk::ImageLayout::eDepthStencilAttachmentOptimal )
          .rebuild_chain()
      )
      .rebuild_chain()
  );
  color = allocator->create_image(
    gct::image_create_info_t()
      .set_basic(
        vk::ImageCreateInfo()
          .setImageType( vk::ImageType::e2D )
          .setFormat( vk::Format::eR16G16B16A16Sfloat )
          .setExtent( vk::Extent3D{ size.width, size.height, 1u } )
          .setUsage(
            vk::ImageUsageFlagBits::eColorAttachment|
            vk::ImageUsageFlagBits::eSampled|
            vk::ImageUsageFlagBits::eTransferSrc
          )
      )
      .rebuild_chain(),
    VMA_MEMORY_USAGE_GPU_ONLY
  );
  color_view = color->get_view( vk::ImageAspectFlagBits::eColor );
  semantic = allocator->create_image(
    gct::image_create_info_t()
      .set_basic(
        vk::ImageCreateInfo()
          .setImageType( vk::ImageType::e2D )
          .setFormat( vk::Format::eR32G32Sfloat )
          .setExtent( vk::Extent3D{ size.width, size.height, 1u } )
          .setUsage(
            vk::ImageUsageFlagBits::eColorAttachment |
            vk::ImageUsageFlagBits::eSampled|
            vk::ImageUsageFlagBits::eTransferSrc
          )
      )
      .rebuild_chain(),
    VMA_MEMORY_USAGE_GPU_ONLY
  );
  semantic_view = semantic->get_view( vk::ImageAspectFlagBits::eColor );
  depth = allocator->create_image(
    gct::image_create_info_t()
      .set_basic(
        vk::ImageCreateInfo()
          .setImageType( vk::ImageType::e2D )
          .setFormat( vk::Format::eD16Unorm )
          .setExtent( vk::Extent3D{ size.width, size.height, 1u } )
          .setUsage(
            vk::ImageUsageFlagBits::eDepthStencilAttachment|
            vk::ImageUsageFlagBits::eSampled|
            vk::ImageUsageFlagBits::eTransferSrc
          )
      )
      .rebuild_chain(),
    VMA_MEMORY_USAGE_GPU_ONLY
  );
  depth_view = depth->get_view( vk::ImageAspectFlagBits::eDepth );
  framebuffer = render_pass->get_framebuffer(
    gct::framebuffer_create_info_t()
      .add_attachment( color_view )
      .add_attachment( semantic_view )
      .add_attachment( depth_view )
  );
  render_pass_begin_info = gct::render_pass_begin_info_t()
    .set_basic(
      vk::RenderPassBeginInfo()
        .setRenderPass( **render_pass )
        .setFramebuffer( **framebuffer )
        .setRenderArea( vk::Rect2D( vk::Offset2D(0, 0), size ) )
    )
    .add_clear_value( vk::ClearColorValue( std::array< float, 4u >{
      clear_color[ 0 ], clear_color[ 1 ], clear_color[ 2 ], clear_color[ 3 ]
    } ) )
    .add_clear_value( vk::ClearColorValue( std::array< float, 4u >{
      0.0f, 0.0f, 0.0f, 0.0f
    } ) )
    .add_clear_value( vk::ClearDepthStencilValue( 1.f, 0 ) )
    .rebuild_chain();
  for( const auto &s: shape ) {
    rects.push_back(
      std::vector< rect_renderer >{
        rect_renderer(
          device,
          allocator,
          pipeline_cache,
          descriptor_pool,
          vs,
          fs,
          render_pass,
          size,
          s,
          input_images
        )
      }
    );
  }
  std::uint32_t font_id = 0u;
  for( const auto &f: font_info ) {
    std::vector< text_renderer > trs;
    trs.push_back(
      text_renderer(
        device,
        allocator,
        pipeline_cache,
        descriptor_pool,
        vs,
        fs,
        render_pass,
        size,
        f,
        input_images
      )
    );
    texts.insert(
      std::make_pair(
        font_id,
        std::move( trs )
      )
    );
    ++font_id;
  }
}
void canvas::reset() {
  std::scoped_lock< std::mutex > lock( guard );
  rects.clear();
  texts.clear();
  for( const auto &s: shape ) {
    rects.push_back(
      std::vector< rect_renderer >{
        rect_renderer(
          device,
          allocator,
          pipeline_cache,
          descriptor_pool,
          vs,
          fs,
          render_pass,
          size,
          s,
          input_images
        )
      }
    );
  }
  std::uint32_t font_id = 0u;
  for( const auto &f: font_info ) {
    std::vector< text_renderer > trs;
    trs.push_back(
      text_renderer(
        device,
        allocator,
        pipeline_cache,
        descriptor_pool,
        vs,
        fs,
        render_pass,
        size,
        f,
        input_images
      )
    );
    texts.insert(
      std::make_pair(
        font_id,
        std::move( trs )
      )
    );
    ++font_id;
  }
  modified = true;
}
const std::shared_ptr< gct::image_t > &canvas::get_semantic() {
  return semantic;
}
const std::shared_ptr< render_pass_t > &canvas::get_render_pass() {
  return render_pass;
}
void canvas::operator()( gct::command_buffer_recorder_t &recorder ) {
  std::scoped_lock< std::mutex > lock( guard );
  if( modified ) {
    draw( recorder, render_pass_begin_info );
    modified = false;
  }
}
void canvas::draw(
  gct::command_buffer_recorder_t &recorder,
  const render_pass_begin_info_t &render_pass_begin_info_
) {
  for( auto &rect: rects ) {
    for( auto &r: rect ) {
      r.load( recorder );
    }
  }
  for( auto &[id,text]: texts ) {
    for( auto &t: text ) {
      t.load( recorder );
    }
  }
  recorder.convert_image(
    color,
    vk::ImageLayout::eColorAttachmentOptimal
  );
  recorder.convert_image(
    semantic,
    vk::ImageLayout::eColorAttachmentOptimal
  );
  {
    auto render_pass_token = recorder.begin_render_pass(
      render_pass_begin_info_,
      vk::SubpassContents::eInline
    );
    for( auto &rect: rects ) {
      for( auto &r: rect ) {
        r( recorder );
      }
    }
    for( auto &[id,text]: texts ) {
      for( auto &t: text ) {
        t( recorder );
      }
    }
  }
  recorder.convert_image(
    semantic,
    vk::ImageLayout::eShaderReadOnlyOptimal
  );
  recorder.convert_image(
    color,
    vk::ImageLayout::eShaderReadOnlyOptimal
  );
}
void canvas::add_rectangle( const vk::Rect2D &rect, const std::array< float, 4u > &color, std::uint16_t depth, bool fill, std::uint32_t border_width, std::uint16_t texid, std::uint16_t semid ) {
  std::scoped_lock< std::mutex > lock( guard );
  auto &r = rects[ 0 ];
  if( r.back().full() ) {
    r.push_back(
      rect_renderer(
        device,
        allocator,
        pipeline_cache,
        descriptor_pool,
        vs,
        fs,
        render_pass,
        size,
        shape[ 0 ],
        input_images
      )
    );
  }
  r.back().add_rectangle( rect, color, depth, fill, border_width, texid, semid );
  modified = true;
}
void canvas::add_glyph( std::uint32_t font_id, std::uint32_t glyph_id, float x, float y, float size, const std::array< float, 4u > &color, std::uint16_t depth, std::uint16_t texid, std::uint16_t semid ) {
  std::scoped_lock< std::mutex > lock( guard );
  auto f = texts.find( font_id );
  if( f != texts.end() ) {
    if( f->second.back().full() ) {
      f->second.push_back(
        text_renderer(
          device,
          allocator,
          pipeline_cache,
          descriptor_pool,
          vs,
          fs,
          render_pass,
          size,
          font_info[ f->first ],
          input_images
        )
      );
    }
    f->second.back().add_glyph( glyph_id, x, y, size, color, depth, texid, semid );
  }
  modified = true;
  
}

}

