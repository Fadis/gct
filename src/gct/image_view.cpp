#include <nlohmann/json.hpp>
#include <gct/allocator.hpp>
#include <gct/device.hpp>
#include <gct/image_view_create_info.hpp>
#include <gct/image_view.hpp>
#include <gct/image.hpp>
#include <gct/exception.hpp>

namespace gct {
  vk::ImageViewType add_array( vk::ImageViewType t ) {
    if( t == vk::ImageViewType::e1D ) {
      return vk::ImageViewType::e1DArray;
    }
    else if( t == vk::ImageViewType::e2D ) {
      return vk::ImageViewType::e2DArray;
    }
    else if( t == vk::ImageViewType::eCube ) {
      return vk::ImageViewType::eCubeArray;
    }
    else {
      return t;
    }
  }
  vk::ImageViewType remove_array( vk::ImageViewType t ) {
    if( t == vk::ImageViewType::e1DArray ) {
      return vk::ImageViewType::e1D;
    }
    else if( t == vk::ImageViewType::e2DArray ) {
      return vk::ImageViewType::e2D;
    }
    else if( t == vk::ImageViewType::eCubeArray ) {
      return vk::ImageViewType::eCube;
    }
    else {
      return t;
    }
  }
  image_view_t::image_view_t(
    const std::shared_ptr< image_t > &image,
    const image_view_create_info_t &create_info
  ) :
    property_type( create_info ),
    created_from< image_t >( image ) {
    auto basic = props.get_basic();
    basic.setImage( **image );
    if( basic.subresourceRange.levelCount == 0u ) {
      basic.subresourceRange.levelCount = image->get_props().get_basic().mipLevels;
    }
    if( basic.subresourceRange.layerCount == 0u ) {
      basic.subresourceRange.layerCount = image->get_props().get_basic().arrayLayers;
    }
#if defined(VK_VERSION_1_2) || defined(VK_KHR_IMAGE_FORMAT_LIST_EXTENSION_NAME)
    if( !image->get_props().has_format_list() )
      basic.setFormat( image->get_props().get_basic().format );
#else
    basic.setFormat( image->get_props().get_basic().format );
#endif
    props
      .set_basic( basic )
      .rebuild_chain();
    handle = (*image->get_device())->createImageViewUnique(
      props.get_basic()
    );
  }
  std::shared_ptr< image_view_t > image_view_t::subview(
    const subview_range &r
  ) const {
    auto new_props = get_props();
    auto basic = new_props.get_basic();
    if( r.mip_offset + r.mip_count > basic.subresourceRange.levelCount ) {
      throw gct::exception::out_of_range( "image_view_t::subview : The mipmap range of subview is outside of original image view", __FILE__, __LINE__ );
    }
    if( r.layer_offset + r.layer_count > basic.subresourceRange.layerCount ) {
      throw gct::exception::out_of_range( "image_view_t::subview : The layer range of subview is outside of original image view", __FILE__, __LINE__ );
    }
    basic.subresourceRange.baseMipLevel += r.mip_offset;
    basic.subresourceRange.setLevelCount( r.mip_count );
    basic.subresourceRange.baseArrayLayer += r.layer_offset;
    basic.subresourceRange.setLayerCount( r.layer_count );
    if( r.layer_count == 1 ) {
      if( r.force_array ) {
        basic.viewType = add_array( basic.viewType );
      }
      else {
        basic.viewType = remove_array( basic.viewType );
      }
    }
    new_props.set_basic( basic );
    return get_factory()->get_view( new_props );
  }
  void to_json( nlohmann::json &root, const image_view_t &v ) {
    root = nlohmann::json::object();
    root[ "props" ] = v.get_props();
  }
}

