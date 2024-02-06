#ifndef GCT_ATTACHMENT_DESCRIPTION2_HPP
#define GCT_ATTACHMENT_DESCRIPTION2_HPP
#include <memory>
#include <vector>
#include <cstdint>
#include <vulkan/vulkan.hpp>
#include <nlohmann/json_fwd.hpp>
#include <gct/extension.hpp>
#include <gct/array_of.hpp>

namespace gct {
#if defined(VK_VERSION_1_2) || defined(VK_KHR_CREATE_RENDERPASS_2_EXTENSION_NAME)
  class attachment_description2_t : public chained_t {
  public:
    using self_type = attachment_description2_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
#ifdef VK_VERSION_1_2
    LIBGCT_BASIC_SETTER( vk::AttachmentDescription2 )
#else
    LIBGCT_BASIC_SETTER( vk::AttachmentDescription2KHR )
#endif
#ifdef VK_VERSION_1_2
    LIBGCT_EXTENSION_SETTER( vk::AttachmentDescriptionStencilLayout , stencil_layout )
#elif defined(VK_KHR_SEPARATE_DEPTH_STENCIL_LAYOUTS_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::AttachmentDescriptionStencilLayoutKHR , stencil_layout )
#endif
#ifdef VK_ANDROID_EXTERNAL_MEMORY_ANDROID_HARDWARE_BUFFER_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::ExternalFormatANDROID , external_format )
#endif
  };
  void to_json( nlohmann::json &root, const attachment_description2_t &v );
  void from_json( const nlohmann::json &root, attachment_description2_t &v );
}
#endif

#endif



