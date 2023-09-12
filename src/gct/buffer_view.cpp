#include <gct/allocator.hpp>
#include <gct/device.hpp>
#include <gct/buffer.hpp>
#include <gct/buffer_view_create_info.hpp>
#include <gct/buffer_view.hpp>

namespace gct {
  buffer_view_t::buffer_view_t(
    const std::shared_ptr< buffer_t > &buffer,
    const buffer_view_create_info_t &create_info
  ) :
    created_from< buffer_t >( buffer ),
    props( create_info ) {
    auto basic = props.get_basic();
    basic
      .setBuffer( **buffer );
    props
      .set_basic( basic )
      .rebuild_chain();
    handle = (*buffer->get_device())->createBufferViewUnique(
      props.get_basic()
    );
  }
}

