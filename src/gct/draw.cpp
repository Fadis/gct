#include <fstream>
#include <gct/device.hpp>
#include <gct/command_pool.hpp>
#include <gct/command_buffer.hpp>
#include <gct/render_pass_begin_info.hpp>
#include <gct/render_pass.hpp>
#include <gct/framebuffer.hpp>
#include <gct/image.hpp>
#include <gct/image_view.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/buffer.hpp>

namespace gct {
void command_buffer_recorder_t::draw(
  std::uint32_t vertex_count,
  std::uint32_t instnce_count,
  std::uint32_t first_vertex,
  std::uint32_t first_instance
) {
  (*get_factory())->draw(
    vertex_count,
    instnce_count,
    first_vertex,
    first_instance
  );
  update_framebuffer_image_layout();
}

void command_buffer_recorder_t::draw_indexed(
  std::uint32_t index_count,
  std::uint32_t instnce_count,
  std::uint32_t first_index,
  std::int32_t vertex_offset,
  std::uint32_t first_instance
) {
  (*get_factory())->drawIndexed(
    index_count,
    instnce_count,
    first_index,
    vertex_offset,
    first_instance
  );
  update_framebuffer_image_layout();
}

void command_buffer_recorder_t::draw_indirect(
  const std::shared_ptr< buffer_t > &buffer,
  vk::DeviceSize offset,
  std::uint32_t raw_count,
  std::uint32_t stride
) {
  (*get_factory())->drawIndirect(
    **buffer,
    offset,
    raw_count,
    stride
  );
  update_framebuffer_image_layout();
  get_factory()->unbound()->keep.push_back( buffer );
}

void command_buffer_recorder_t::draw_indexed_indirect(
  const std::shared_ptr< buffer_t > &buffer,
  vk::DeviceSize offset,
  std::uint32_t raw_count,
  std::uint32_t stride
) {
  (*get_factory())->drawIndexedIndirect(
    **buffer,
    offset,
    raw_count,
    stride
  );
  update_framebuffer_image_layout();
  get_factory()->unbound()->keep.push_back( buffer );
}

void command_buffer_recorder_t::update_framebuffer_image_layout() {
  return;
  const auto rp = get_factory()->get_current_render_pass();
  if( !rp ) {
    throw -1;
  }
  const auto &rp_attachment = rp->get_render_pass()->get_props().get_attachment();
  const auto &fb_attachment = rp->get_framebuffer()->get_props().get_attachment();
  if( rp_attachment.size() != fb_attachment.size() ) {
    throw -1;
  }
  for( unsigned int i = 0u; i != rp_attachment.size(); ++i ) {
    auto &image_view = fb_attachment[ i ];
    const auto &subres = image_view->get_props().get_basic().subresourceRange;
    auto &image = image_view->get_factory();
    image->get_layout().set_layout(
      subres.baseMipLevel,
      subres.levelCount,
      subres.baseArrayLayer,
      subres.layerCount,
      rp_attachment[ i ].finalLayout
    );
  }
}

}

