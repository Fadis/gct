#include <vulkan/vulkan.hpp>
#if defined(VK_VERSION_1_2) || defined(VK_KHR_CREATE_RENDERPASS_2_EXTENSION_NAME)

#include <nlohmann/json.hpp>
#ifdef VK_VERSION_1_2
#include <vulkan2json/SubpassDependency2.hpp>
#else
#include <vulkan2json/SubpassDependency2KHR.hpp>
#endif
#ifdef VK_VERSION_1_3
#include <vulkan2json/MemoryBarrier2.hpp>
#elif defined(VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME)
#include <vulkan2json/MemoryBarrier2KHR.hpp>
#endif
#include <gct/exception.hpp>
#include <gct/subpass_dependency2.hpp>

namespace gct {
  void to_json( nlohmann::json &root, const subpass_dependency2_t &v ) {
    root = nlohmann::json::object();
    root[ "basic" ] = v.get_basic();
#if defined(VK_VERSION_1_3) || defined(VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( memory_barrier )
#endif
  }
  void from_json( const nlohmann::json &root, subpass_dependency2_t &v ) {
    if( !root.is_object() ) throw incompatible_json( "The JSON is incompatible to subpass_dependency2_t", __FILE__, __LINE__ );
    LIBGCT_EXTENSION_FROM_JSON( basic )
#if defined(VK_VERSION_1_3) || defined(VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME)
    LIBGCT_EXTENSION_FROM_JSON( memory_barrier )
#endif
  }
  subpass_dependency2_t &subpass_dependency2_t::rebuild_chain() {
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
#if defined(VK_VERSION_1_3) || defined(VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( memory_barrier )
#endif
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
}

#endif

