#include <fstream>
#include <gct/buffer.hpp>
#include <gct/allocator.hpp>
#include <gct/device.hpp>
#include <gct/command_pool.hpp>
#include <gct/command_buffer.hpp>
#include <gct/command_buffer_recorder.hpp>

namespace gct {
  void command_buffer_recorder_t::copy(
    const std::shared_ptr< buffer_t > &src,
    const std::shared_ptr< buffer_t > &dest,
    const std::vector< vk::BufferCopy > &range
  ) {
    (*get_factory())->copyBuffer(
      **src,
      **dest,
      range
    );
    get_factory()->unbound()->keep.push_back( src );
    get_factory()->unbound()->keep.push_back( dest );
  }
  void command_buffer_recorder_t::copy(
    const std::shared_ptr< buffer_t > &src,
    const std::shared_ptr< buffer_t > &dest,
    const vk::BufferCopy &range
  ) {
    copy( src, dest, std::vector< vk::BufferCopy >{ range } );
  }
  void command_buffer_recorder_t::copy(
    const std::shared_ptr< buffer_t > &src,
    const std::shared_ptr< buffer_t > &dest
  ) {
    copy(
      src,
      dest,
      vk::BufferCopy()
        .setSrcOffset( 0 )
        .setDstOffset( 0 )
        .setSize( std::min( src->get_props().get_basic().size, dest->get_props().get_basic().size ) )

    );
  }
  void command_buffer_recorder_t::copy(
    const boost::iterator_range< const std::uint8_t* > &src,
    const std::shared_ptr< buffer_t > &staging,
    const std::shared_ptr< buffer_t > &dest
  ) {
    if( staging->get_props().get_basic().size < src.size() ) throw -1;
    if( staging->get_props().get_basic().size < src.size() ) throw -1;
    {
      auto mapped = staging->map< std::uint8_t >();
      std::copy( src.begin(), src.end(), mapped.begin() );
    }
    copy( staging, dest );
  }
}
