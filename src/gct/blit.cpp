#include <fstream>
#include <gct/image.hpp>
#include <gct/allocator.hpp>
#include <gct/device.hpp>
#include <gct/command_pool.hpp>
#include <gct/command_buffer.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/format.hpp>

namespace gct {
  void command_buffer_recorder_t::blit(
    const std::shared_ptr< image_t > &src,
    const std::shared_ptr< image_t > &dest,
    const std::vector< vk::ImageBlit > &range,
    vk::Filter filter
  ) {
    (*get_factory())->blitImage(
      **src,
      src->get_props().get_basic().initialLayout,
      **dest,
      dest->get_props().get_basic().initialLayout,
      range,
      filter
    );
    get_factory()->unbound()->keep.push_back( src );
    get_factory()->unbound()->keep.push_back( dest );
  }
  void command_buffer_recorder_t::blit(
    const std::shared_ptr< image_t > &src,
    const std::shared_ptr< image_t > &dest,
    const vk::ImageBlit &range,
    vk::Filter filter
  ) {
    blit( src, dest, std::vector< vk::ImageBlit >{ range }, filter );
  }
}
