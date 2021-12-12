#include <gct/buffer_device_address_info.hpp>
#if defined(VK_VERSION_1_2) || defined(VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME)
#include <gct/extension.hpp>
#include <gct/buffer.hpp>
#include <nlohmann/json.hpp>
#include <vulkan2json/BufferDeviceAddressInfo.hpp>
namespace gct {
  buffer_device_address_info_t &buffer_device_address_info_t::rebuild_chain() {
    if( chained ) return *this;
    if( buffer ) {
      auto basic = get_basic();
      basic
        .setBuffer( **buffer );
      set_basic( std::move( basic ) );
    }
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
  buffer_device_address_info_t &buffer_device_address_info_t::set_buffer( const std::shared_ptr< buffer_t > &v ) {
    buffer = v;
    chained = false;
    return *this;
  }
  buffer_device_address_info_t &buffer_device_address_info_t::clear_buffer() {
    buffer.reset();
    chained = false;
    return *this;
  }
  void buffer_device_address_info_t::to_json( nlohmann::json &root ) const {
    root = nlohmann::json::object();
    root[ "basic" ] = basic;
    if( buffer ) {
      root[ "basic" ][ "buffer" ] = *buffer;
    }
  }
  void to_json( nlohmann::json &root, const buffer_device_address_info_t &v ) {
    v.to_json( root );
  }
}
#endif

