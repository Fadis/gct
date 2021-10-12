#ifndef GCT_FENCE_CREATE_INFO_HPP
#define GCT_FENCE_CREATE_INFO_HPP
#include <memory>
#include <optional>
#include <vulkan/vulkan.hpp>
#include <gct/extension.hpp>

namespace gct {
  class fence_create_info_t {
  public:
    using self_type = fence_create_info_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
    LIBGCT_BASIC_SETTER( vk::FenceCreateInfo )
#ifdef VK_VERSION_1_1
    LIBGCT_EXTENSION_SETTER( vk::ExportFenceCreateInfo, export_fence )
#elif defined(VK_KHR_EXTERNAL_FENCE_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::ExportFenceCreateInfoKHR, export_fence )
#endif
#ifdef VK_KHR_EXTERNAL_FENCE_WIN32_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::ExportFenceWin32HandleInfoKHR, export_fence_win32_handle )
#endif
  };
}

#endif

