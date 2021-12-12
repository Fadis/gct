#include <vulkan/vulkan.hpp>
#include <gct/vulkanhpp.hpp>
namespace gct {
  vulkanhpp::vulkanhpp() {
    vk::DynamicLoader dl;
    PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr =
    dl.getProcAddress<PFN_vkGetInstanceProcAddr>( "vkGetInstanceProcAddr" );
    VULKAN_HPP_DEFAULT_DISPATCHER.init( vkGetInstanceProcAddr );
  }
  void vulkanhpp::init() {
    static const auto instance = vulkanhpp();
  }
}
