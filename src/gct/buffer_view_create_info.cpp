#include <nlohmann/json.hpp>
#include <gct/buffer_view_create_info.hpp>
#include <vulkan2json/BufferViewCreateInfo.hpp>
namespace gct {
  void to_json( nlohmann::json &root, const buffer_view_create_info_t &v ) {
    root = nlohmann::json::object();
    root[ "basic" ] = v.get_basic();
  }
  void from_json( const nlohmann::json &root, buffer_view_create_info_t &v ) {
    if( !root.is_object() ) throw incompatible_json( "The JSON is incompatible to buffer_view_create_info_t", __FILE__, __LINE__ );
    LIBGCT_EXTENSION_FROM_JSON( basic )
  }
  buffer_view_create_info_t &buffer_view_create_info_t::rebuild_chain() {
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
}

