#include <gct/allocator.hpp>
#include <gct/device.hpp>
#include <gct/image_view_create_info.hpp>
#include <gct/image_view.hpp>

namespace gct {
  image_view_t::image_view_t(
    const std::shared_ptr< image_t > &image,
    const image_view_create_info_t &create_info
  ) :
    created_from< image_t >( image ),
    props( create_info ) {
    auto basic = props.get_basic();
    basic
      .setImage( **image )
      .setFormat( image->get_props().get_basic().format );
    props
      .set_basic( basic )
      .rebuild_chain();
    handle = (*image->get_device())->createImageViewUnique(
      props.get_basic()
    );
  }
}

