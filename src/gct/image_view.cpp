#include <nlohmann/json.hpp>
#include <gct/allocator.hpp>
#include <gct/device.hpp>
#include <gct/image_view_create_info.hpp>
#include <gct/image_view.hpp>
#include <gct/image.hpp>

namespace gct {
  image_view_t::image_view_t(
    const std::shared_ptr< image_t > &image,
    const image_view_create_info_t &create_info
  ) :
    created_from< image_t >( image ),
    props( create_info ) {
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
  void to_json( nlohmann::json &root, const image_view_t &v ) {
    root = nlohmann::json::object();
    root[ "props" ] = v.get_props();
  }
}

