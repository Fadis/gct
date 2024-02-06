#include <nlohmann/json.hpp>
#include <gct/buffer_view_create_info.hpp>
#include <vulkan2json/BufferViewCreateInfo.hpp>
#ifdef VK_KHR_MAINTENANCE_5_EXTENSION_NAME
#include <vulkan2json/BufferUsageFlags2CreateInfoKHR.hpp>
#endif
#ifdef VK_EXT_METAL_OBJECTS_EXTENSION_NAME
#include <vulkan2json/ExportMetalObjectCreateInfoEXT.hpp>
#endif
namespace gct {
  void to_json( nlohmann::json &root, const buffer_view_create_info_t &v ) {
    root = nlohmann::json::object();
    root[ "basic" ] = v.get_basic();
#ifdef VK_KHR_MAINTENANCE_5_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( buffer_usage_flags2 )
#endif
#ifdef VK_EXT_METAL_OBJECTS_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( export_metal_object )
#endif
  }
  void from_json( const nlohmann::json &root, buffer_view_create_info_t &v ) {
    if( !root.is_object() ) throw incompatible_json( "The JSON is incompatible to buffer_view_create_info_t", __FILE__, __LINE__ );
    LIBGCT_EXTENSION_FROM_JSON( basic )
#ifdef VK_KHR_MAINTENANCE_5_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( buffer_usage_flags2 )
#endif
#ifdef VK_EXT_METAL_OBJECTS_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( export_metal_object )
#endif
  }
  buffer_view_create_info_t &buffer_view_create_info_t::rebuild_chain() {
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
#ifdef VK_KHR_MAINTENANCE_5_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( buffer_usage_flags2 )
#endif
#ifdef VK_EXT_METAL_OBJECTS_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( export_metal_object )
#endif
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
}

