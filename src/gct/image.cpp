#include <gct/image_create_info.hpp>
#include <gct/image_view_create_info.hpp>
#include <gct/image.hpp>

namespace gct {
  vk::ImageViewType to_image_view_type( vk::ImageType v, std::uint32_t layers ) {
    if( layers == 1u ) {
      if( v == vk::ImageType::e1D )
        return vk::ImageViewType::e1D;
      else if( v == vk::ImageType::e2D )
        return vk::ImageViewType::e2D;
      else if( v == vk::ImageType::e3D )
        return vk::ImageViewType::e3D;
      else
        throw -1;
    }
    else if( layers >= 2u ) {
      if( v == vk::ImageType::e1D )
        return vk::ImageViewType::e1DArray;
      else if( v == vk::ImageType::e2D )
        return vk::ImageViewType::e2DArray;
      else
        throw -1;
    }
    else
      throw -1;
  }
  image_t::image_t(
    const image_create_info_t &create_info
  ) :
    props( create_info ),
    layout(
      create_info.get_basic().mipLevels,
      create_info.get_basic().arrayLayers,
      create_info.get_basic().initialLayout
    ) {
    props.rebuild_chain();
  }
  std::vector< std::shared_ptr< image_view_t > >
  get_views(
    const std::vector< std::shared_ptr< image_t > > &images
  ) {
    std::vector< std::shared_ptr< image_view_t > > temp;
    temp.reserve( temp.size() );
    for( unsigned int i = 0u; i != images.size(); ++i ) {
      temp.push_back( images[ i ]->get_view() );
    }
    return temp;
  }
}

