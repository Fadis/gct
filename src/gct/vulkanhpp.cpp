#include <vulkan/vulkan.hpp>
#include <gct/vulkanhpp.hpp>
namespace gct {
  vulkanhpp::vulkanhpp() {
#if VK_HEADER_VERSION >= 301
    vk::detail::DynamicLoader dl;
#else
    vk::DynamicLoader dl;
#endif
    PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr =
    dl.getProcAddress<PFN_vkGetInstanceProcAddr>( "vkGetInstanceProcAddr" );
    VULKAN_HPP_DEFAULT_DISPATCHER.init( vkGetInstanceProcAddr );
  }
  void vulkanhpp::init() {
    static const auto instance = vulkanhpp();
  }
}
