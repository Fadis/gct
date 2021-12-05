#include <gct/fence_create_info.hpp>
#include <vulkan2json/FenceCreateInfo.hpp>
#ifdef VK_VERSION_1_1
#include <vulkan2json/ExportFenceCreateInfo.hpp>
#elif defined(VK_KHR_EXTERNAL_FENCE_EXTENSION_NAME)
#include <vulkan2json/ExportFenceCreateInfoKHR.hpp>
#endif
#ifdef VK_KHR_EXTERNAL_FENCE_WIN32_EXTENSION_NAME
#include <vulkan2json/ExportFenceWin32HandleInfoKHR.hpp>
#endif

namespace gct {
  void to_json( nlohmann::json &root, const fence_create_info_t &v ) {
    root = nlohmann::json::object();
    root[ "basic" ] = v.get_basic();
#if defined(VK_VERSION_1_1) || defined(VK_KHR_EXTERNAL_FENCE_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( export_fence ) 
#endif
#ifdef VK_KHR_EXTERNAL_FENCE_WIN32_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( export_fence_win32_handle ) 
#endif
  }
  void from_json( const nlohmann::json &root, fence_create_info_t &v ) {
    if( !root.is_object() ) throw incompatible_json( "The JSON is incompatible to fence_create_info_t", __FILE__, __LINE__ );
    LIBGCT_EXTENSION_FROM_JSON( basic )
#if defined(VK_VERSION_1_1) || defined(VK_KHR_EXTERNAL_FENCE_EXTENSION_NAME)
    LIBGCT_EXTENSION_FROM_JSON( export_fence ) 
#endif
#ifdef VK_KHR_EXTERNAL_FENCE_WIN32_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( export_fence_win32_handle ) 
#endif
  }
  fence_create_info_t &fence_create_info_t::rebuild_chain() {
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
#if defined(VK_VERSION_1_1) || defined(VK_KHR_EXTERNAL_FENCE_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( export_fence ) 
#endif
#ifdef VK_KHR_EXTERNAL_FENCE_WIN32_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( export_fence_win32_handle ) 
#endif
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
}

