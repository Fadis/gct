#ifndef GCT_SUBPASS_DEPENDENCY2_HPP
#define GCT_SUBPASS_DEPENDENCY2_HPP
#include <vulkan/vulkan.hpp>
#include <nlohmann/json_fwd.hpp>
#include <gct/extension.hpp>
#include <gct/array_of.hpp>
#include <vulkan/vulkan_structs.hpp>

namespace gct {
#if defined(VK_VERSION_1_2) || defined(VK_KHR_CREATE_RENDERPASS_2_EXTENSION_NAME)
  class subpass_dependency2_t : public chained_t {
  public:
    using self_type = subpass_dependency2_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
#ifdef VK_VERSION_1_2
    LIBGCT_BASIC_SETTER( vk::SubpassDependency2 )
#else
    LIBGCT_BASIC_SETTER( vk::SubpassDependency2KHR )
#endif
#ifdef VK_VERSION_1_3
    LIBGCT_EXTENSION_SETTER( vk::MemoryBarrier2 , memory_barrier )
#elif defined(VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::MemoryBarrier2KHR , memory_barrier )
#endif
  };
  void to_json( nlohmann::json &root, const subpass_dependency2_t &v );
  void from_json( const nlohmann::json &root, subpass_dependency2_t &v );
}
#endif

#endif



