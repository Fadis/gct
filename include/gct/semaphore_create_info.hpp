#ifndef GCT_SEMAPHORE_CREATE_INFO_HPP
#define GCT_SEMAPHORE_CREATE_INFO_HPP
#include <memory>
#include <optional>
#include <nlohmann/json_fwd.hpp>
#include <vulkan/vulkan.hpp>
#include <gct/extension.hpp>

namespace gct {
  class semaphore_create_info_t : public chained_t {
  public:
    using self_type = semaphore_create_info_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
    LIBGCT_BASIC_SETTER( vk::SemaphoreCreateInfo )
#ifdef VK_VERSION_1_1
    LIBGCT_EXTENSION_SETTER( vk::ExportSemaphoreCreateInfo, export_semaphore )
#elif defined(VK_KHR_EXTERNAL_SEMAPHORE_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::ExportSemaphoreCreateInfoKHR, export_semaphore )
#endif
#ifdef VK_KHR_EXTERNAL_SEMAPHORE_WIN32_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::ExportSemaphoreWin32HandleInfoKHR, export_semaphore_win32_handle )
#endif
#ifdef VK_VERSION_1_2
    LIBGCT_EXTENSION_SETTER( vk::SemaphoreTypeCreateInfo, type )
#elif defined(VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::SemaphoreTypeCreateInfoKHR, type )
#endif
  };
  void to_json( nlohmann::json &root, const semaphore_create_info_t &v );
  void from_json( const nlohmann::json &root, semaphore_create_info_t &v );
}

#endif

