#include <nlohmann/json.hpp>
#include <gct/semaphore_create_info.hpp>
#include <vulkan2json/SemaphoreCreateInfo.hpp>
#ifdef VK_EXT_METAL_OBJECTS_EXTENSION_NAME
#include <vulkan2json/ExportMetalObjectCreateInfoEXT.hpp>
#endif
#ifdef VK_VERSION_1_1
#include <vulkan2json/ExportSemaphoreCreateInfo.hpp>
#elif defined(VK_KHR_EXTERNAL_SEMAPHORE_EXTENSION_NAME)
#include <vulkan2json/ExportSemaphoreCreateInfoKHR.hpp>
#endif
#ifdef VK_KHR_EXTERNAL_SEMAPHORE_WIN32_EXTENSION_NAME
#include <vulkan2json/ExportSemaphoreWin32HandleInfoKHR.hpp>
#endif
#ifdef VK_EXT_METAL_OBJECTS_EXTENSION_NAME
#include <vulkan2json/ImportMetalSharedEventInfoEXT.hpp>
#endif
#ifdef VK_NV_LOW_LATENCY_EXTENSION_NAME
#include <vulkan2json/QueryLowLatencySupportNV.hpp>
#endif
#ifdef VK_VERSION_1_2
#include <vulkan2json/SemaphoreTypeCreateInfo.hpp>
#elif defined(VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME)
#include <vulkan2json/SemaphoreTypeCreateInfoKHR.hpp>
#endif

namespace gct {
  void to_json( nlohmann::json &root, const semaphore_create_info_t &v ) {
     root = nlohmann::json::object();
     root[ "basic" ] = v.get_basic();
#ifdef VK_EXT_METAL_OBJECTS_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( export_metal_object )
#endif
#if defined(VK_VERSION_1_1) || defined(VK_KHR_EXTERNAL_SEMAPHORE_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( export_semaphore ) 
#endif
#ifdef VK_KHR_EXTERNAL_SEMAPHORE_WIN32_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( export_semaphore_win32_handle ) 
#endif
#ifdef VK_EXT_METAL_OBJECTS_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( import_metal_shared_event )
#endif
#ifdef VK_NV_LOW_LATENCY_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( query_low_latency_support )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( type ) 
#endif
  }
  void from_json( const nlohmann::json &root, semaphore_create_info_t &v ) {
    if( !root.is_object() ) throw incompatible_json( "The JSON is incompatible to semaphore_create_info_t", __FILE__, __LINE__ );
    LIBGCT_EXTENSION_FROM_JSON( basic )
#ifdef VK_EXT_METAL_OBJECTS_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( export_metal_object )
#endif
#if defined(VK_VERSION_1_1) || defined(VK_KHR_EXTERNAL_SEMAPHORE_EXTENSION_NAME)
    LIBGCT_EXTENSION_FROM_JSON( export_semaphore ) 
#endif
#ifdef VK_KHR_EXTERNAL_SEMAPHORE_WIN32_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( export_semaphore_win32_handle ) 
#endif
#ifdef VK_EXT_METAL_OBJECTS_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( import_metal_shared_event )
#endif
#ifdef VK_NV_LOW_LATENCY_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( query_low_latency_support )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME)
    LIBGCT_EXTENSION_FROM_JSON( type ) 
#endif
  }
  semaphore_create_info_t &semaphore_create_info_t::rebuild_chain() {
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
#ifdef VK_EXT_METAL_OBJECTS_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( export_metal_object )
#endif
#if defined(VK_VERSION_1_1) || defined(VK_KHR_EXTERNAL_SEMAPHORE_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( export_semaphore ) 
#endif
#ifdef VK_KHR_EXTERNAL_SEMAPHORE_WIN32_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( export_semaphore_win32_handle ) 
#endif
#ifdef VK_EXT_METAL_OBJECTS_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( import_metal_shared_event )
#endif
#ifdef VK_NV_LOW_LATENCY_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( query_low_latency_support )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( type ) 
#endif
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
}

