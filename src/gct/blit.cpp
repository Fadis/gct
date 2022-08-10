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
    std::vector< vk::ImageMemoryBarrier > old_src;
    std::vector< vk::ImageMemoryBarrier > old_dest;
    if( !( src->get_layout().is_uniform() && src->get_layout().is_copyable_source_layout() ) )
      old_src = convert_image( src, vk::ImageLayout::eTransferSrcOptimal );
    if( !( dest->get_layout().is_uniform() && dest->get_layout().is_copyable_source_layout() ) )
      old_dest = convert_image( dest, vk::ImageLayout::eTransferDstOptimal );
    (*get_factory())->blitImage(
      **src,
      src->get_layout().get_uniform_layout(),
      **dest,
      dest->get_layout().get_uniform_layout(),
      range,
      filter
    );
    if( !( dest->get_layout().is_uniform() && dest->get_layout().is_copyable_source_layout() ) )
      revert_convert_image( dest, old_dest );
    if( !( src->get_layout().is_uniform() && src->get_layout().is_copyable_source_layout() ) )
      revert_convert_image( src, old_src );
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
