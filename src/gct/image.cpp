#include <gct/image_create_info.hpp>
#include <gct/image_view_create_info.hpp>
#include <gct/image.hpp>

namespace gct {
  vk::ImageViewType to_image_view_type( vk::ImageType v ) {
    if( v == vk::ImageType::e1D )
      return vk::ImageViewType::e1D;
    else if( v == vk::ImageType::e2D )
      return vk::ImageViewType::e2D;
    else if( v == vk::ImageType::e3D )
      return vk::ImageViewType::e3D;
    else
      throw -1;
  }
  image_t::image_t(
    const image_create_info_t &create_info
  ) :
    props( create_info ) {
    props.rebuild_chain();
  }
}

