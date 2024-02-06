#include <vulkan/vulkan.hpp>
#if defined(VK_VERSION_1_2) || defined(VK_KHR_CREATE_RENDERPASS_2_EXTENSION_NAME)

#include <nlohmann/json.hpp>
#ifdef VULKAN_1_2
#include <vulkan2json/AttachmentReference2.hpp>
#include <vulkan2json/AttachmentReferenceStencilLayout.hpp>
#else
#include <vulkan2json/AttachmentReference2KHR.hpp>
#ifdef VK_KHR_SEPARATE_DEPTH_STENCIL_LAYOUTS_EXTENSION_NAME 
#include <vulkan2json/AttachmentReferenceStencilLayoutKHR.hpp>
#endif
#endif
#include <gct/attachment_reference2.hpp>

namespace gct {
  void to_json( nlohmann::json &root, const attachment_reference2_t &v ) {
    root = nlohmann::json::object();
    root[ "basic" ] = v.get_basic();
#if defined(VK_VERSION_1_2) || defined(VK_KHR_MAINTENANCE2_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( stencil_layout )
#endif
  }
  void from_json( const nlohmann::json &root, attachment_reference2_t &v ) {
    if( !root.is_object() ) throw incompatible_json( "The JSON is incompatible to attachment_reference2_t", __FILE__, __LINE__ );
    LIBGCT_EXTENSION_FROM_JSON( basic )
#if defined(VK_VERSION_1_2) || defined(VK_KHR_MAINTENANCE2_EXTENSION_NAME)
    LIBGCT_EXTENSION_FROM_JSON( stencil_layout )
#endif
  }
  attachment_reference2_t &attachment_reference2_t::rebuild_chain() {
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
#if defined(VK_VERSION_1_2) || defined(VK_KHR_SEPARATE_DEPTH_STENCIL_LAYOUTS_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( stencil_layout )
#endif
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
}

#endif




