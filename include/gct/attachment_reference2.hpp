#ifndef GCT_ATTACHMENT_REFERENCE2_HPP
#define GCT_ATTACHMENT_REFERENCE2_HPP
#include <memory>
#include <vector>
#include <cstdint>
#include <vulkan/vulkan.hpp>
#include <nlohmann/json_fwd.hpp>
#include <gct/extension.hpp>
#include <gct/array_of.hpp>

namespace gct {
#if defined(VK_VERSION_1_2) || defined(VK_KHR_CREATE_RENDERPASS_2_EXTENSION_NAME)
  class attachment_reference2_t : public chained_t {
  public:
    using self_type = attachment_reference2_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
#ifdef VK_VERSION_1_2
    LIBGCT_BASIC_SETTER( vk::AttachmentReference2 )
#else
    LIBGCT_BASIC_SETTER( vk::AttachmentReference2KHR )
#endif
#ifdef VK_VERSION_1_2
    LIBGCT_EXTENSION_SETTER( vk::AttachmentReferenceStencilLayout , stencil_layout )
#elif defined(VK_KHR_SEPARATE_DEPTH_STENCIL_LAYOUTS_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::AttachmentReferenceStencilLayoutKHR , stencil_layout )
#endif
  };
  void to_json( nlohmann::json &root, const attachment_reference2_t &v );
  void from_json( const nlohmann::json &root, attachment_reference2_t &v );
}
#endif

#endif



