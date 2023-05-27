#ifndef LIBGCT_INCLUDE_GCT_SURFACE_CAPABILITIES_HPP
#define LIBGCT_INCLUDE_GCT_SURFACE_CAPABILITIES_HPP

#include <vector>
#include <optional>
#include <nlohmann/json_fwd.hpp>
#include <vulkan/vulkan.hpp>
#include <gct/extension.hpp>

namespace gct {

#ifdef VK_KHR_SURFACE_EXTENSION_NAME
  struct surface_capabilities_t;
  void to_json( nlohmann::json &root, const surface_capabilities_t &v );

  struct surface_format_t {
  public:
    vk::SurfaceFormatKHR basic;
  };

  using surface_formats_t = std::vector< surface_format_t >;

  class physical_device_t;

  struct surface_capabilities_t : public chained_t {
  public:
    surface_capabilities_t(
      const physical_device_t&,
      vk::SurfaceKHR
    );

private:
    surface_formats_t formats;
public:
    const surface_formats_t &get_formats() const { return formats; }

    using self_type = surface_capabilities_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
#ifdef VK_KHR_GET_SURFACE_CAPABILITIES_2_EXTENSION_NAME
private:
    using basic_t = vk::SurfaceCapabilitiesKHR;
    using basic2_t = vk::SurfaceCapabilities2KHR;
    basic2_t basic; 
public: 
    LIBGCT_BASIC_SETTER_INDIRECT( surfaceCapabilities )
#else
    LIBGCT_BASIC_SETTER( vk::SurfaceCapabilitiesKHR )
#endif
#ifdef VK_AMD_DISPLAY_NATIVE_HDR_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::DisplayNativeHdrSurfaceCapabilitiesAMD , display_native_hdr )
#endif
#ifdef VK_KHR_SHARED_PRESENTABLE_IMAGE_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::SharedPresentSurfaceCapabilitiesKHR , shared_presentable_image )
#endif
#ifdef VK_EXT_FULL_SCREEN_EXCLUSIVE_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::SurfaceCapabilitiesFullScreenExclusiveEXT , full_screen_exclusive )
#endif
#ifdef VK_KHR_SURFACE_PROTECTED_CAPABILITIES_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::SurfaceProtectedCapabilitiesKHR , surface_protected_capabilities )
#endif
  };
  const surface_format_t &select_simple_surface_format( const surface_formats_t &formats );
#endif
}

#endif

