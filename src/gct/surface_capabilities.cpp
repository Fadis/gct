#include <nlohmann/json.hpp>
#include <gct/instance.hpp>
#include <gct/surface_capabilities.hpp>
#ifdef VK_KHR_SURFACE_EXTENSION_NAME
#include <vulkan2json/SurfaceFormatKHR.hpp>
#ifdef VK_KHR_GET_SURFACE_CAPABILITIES_2_EXTENSION_NAME
#include <vulkan2json/SurfaceCapabilitiesKHR.hpp>
#include <vulkan2json/SurfaceCapabilities2KHR.hpp>
#else
#include <vulkan2json/SurfaceCapabilitiesKHR.hpp>
#endif
#ifdef VK_AMD_DISPLAY_NATIVE_HDR_EXTENSION_NAME
#include <vulkan2json/DisplayNativeHdrSurfaceCapabilitiesAMD.hpp>
#endif
#ifdef VK_KHR_SHARED_PRESENTABLE_IMAGE_EXTENSION_NAME
#include <vulkan2json/SharedPresentSurfaceCapabilitiesKHR.hpp>
#endif
#ifdef VK_EXT_FULL_SCREEN_EXCLUSIVE_EXTENSION_NAME
#include <vulkan2json/SurfaceCapabilitiesFullScreenExclusiveEXT.hpp>
#endif
#ifdef VK_KHR_SURFACE_PROTECTED_CAPABILITIES_EXTENSION_NAME
#include <vulkan2json/SurfaceProtectedCapabilitiesKHR.hpp>
#endif
#endif

namespace gct {

#ifdef VK_KHR_SURFACE_EXTENSION_NAME

  void to_json( nlohmann::json &root, const surface_format_t &v ) {
    root = nlohmann::json::object();
    root[ "basic" ] = v.basic;
  }
  
  void to_json( nlohmann::json &root, const surface_capabilities_t &v ) {
    root = nlohmann::json::object();
    root[ "basic" ] = v.get_basic();
    root[ "formats" ] = v.get_formats();
#ifdef VK_AMD_DISPLAY_NATIVE_HDR_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( display_native_hdr )
#endif
#ifdef VK_KHR_SHARED_PRESENTABLE_IMAGE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( shared_presentable_image )
#endif
#ifdef VK_EXT_FULL_SCREEN_EXCLUSIVE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( full_screen_exclusive )
#endif
#ifdef VK_KHR_SURFACE_PROTECTED_CAPABILITIES_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( surface_protected_capabilities )
#endif
  }

  surface_capabilities_t::surface_capabilities_t(
    const physical_device_t &pdev,
    vk::SurfaceKHR surface
  ) {
#ifdef VK_KHR_GET_SURFACE_CAPABILITIES_2_EXTENSION_NAME
    if(
      pdev.get_factory()->get_activated_extensions().find( "VK_KHR_get_surface_capabilities2" ) != pdev.get_factory()->get_activated_extensions().end()
    ) {
      const auto &props = pdev.get_props();
      const auto &ext = props.get_activated_extensions();
#ifdef VK_AMD_DISPLAY_NATIVE_HDR_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( display_native_hdr, "VK_AMD_display_native_hdr" )
#endif
#ifdef VK_KHR_SHARED_PRESENTABLE_IMAGE_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( shared_presentable_image, "VK_KHR_shared_presentable_image" )
#endif
#ifdef VK_EXT_FULL_SCREEN_EXCLUSIVE_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( full_screen_exclusive, "VK_EXT_full_screen_exclusive" )
#endif
#ifdef VK_KHR_SURFACE_PROTECTED_CAPABILITIES_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( surface_protected_capabilities, "VK_KHR_surface_protected_capabilities" )
#endif
      rebuild_chain();
      const auto info = vk::PhysicalDeviceSurfaceInfo2KHR()
        .setSurface( surface );
      auto err = pdev->getSurfaceCapabilities2KHR( &info, &basic );
      if( err == vk::Result::eSuccess ) {
#if VK_HEADER_VERSION >= 256
        vk::detail::throwResultException( vk::Result( err ), "getSurfaceCapabilities2KHR failed" );
#else
        vk::throwResultException( vk::Result( err ), "getSurfaceCapabilities2KHR failed" );
#endif
      }
      {
        std::uint32_t format_count = 0u;
        err = pdev->getSurfaceFormats2KHR( &info, &format_count, nullptr );
        if( err != vk::Result::eSuccess ) {
#if VK_HEADER_VERSION >= 256
          vk::detail::throwResultException( vk::Result( err ), "getSurfaceFormats2KHR failed" );
#else
          vk::throwResultException( vk::Result( err ), "getSurfaceFormats2KHR failed" );
#endif
        }
        std::vector< vk::SurfaceFormat2KHR > root( format_count );
        formats.resize( format_count );
        err = pdev->getSurfaceFormats2KHR( &info, &format_count, root.data() );
        if( err != vk::Result::eSuccess ) {
#if VK_HEADER_VERSION >= 256
          vk::detail::throwResultException( vk::Result( err ), "getSurfaceFormats2KHR failed" );
#else
          vk::throwResultException( vk::Result( err ), "getSurfaceFormats2KHR failed" );
#endif
}
        for( std::uint32_t i = 0u; i != format_count; ++i ) {
          formats[ i ].basic = root[ i ].surfaceFormat;
        }
      }
    }
    else {
      basic.surfaceCapabilities = pdev->getSurfaceCapabilitiesKHR( surface );
      {
        std::uint32_t format_count = 0u;
        auto err = pdev->getSurfaceFormatsKHR( surface, &format_count, nullptr );
        if( err != vk::Result::eSuccess ) {
#if VK_HEADER_VERSION >= 256
          vk::detail::throwResultException( vk::Result( err ), "getSurfaceFormatsKHR failed" );
#else
          vk::throwResultException( vk::Result( err ), "getSurfaceFormatsKHR failed" );
#endif
        }
        std::vector< vk::SurfaceFormatKHR > root( format_count );
        formats.resize( format_count );
        err = pdev->getSurfaceFormatsKHR( surface, &format_count, root.data() );
        if( err != vk::Result::eSuccess ) {
#if VK_HEADER_VERSION >= 256
          vk::detail::throwResultException( vk::Result( err ), "getSurfaceFormatsKHR failed" );
#else
          vk::throwResultException( vk::Result( err ), "getSurfaceFormatsKHR failed" );
#endif
        }
        for( std::uint32_t i = 0u; i != format_count; ++i ) {
          formats[ i ].basic = root[ i ];
        }
      }
    }
#else
    {
      basic = pdev->getSurfaceCapabilitiesKHR( surface );
      {
        std::uint32_t format_count = 0u;
        auto err = pdev->getSurfaceFormatsKHR( surface, &format_count, nullptr );
        if( err != vk::Result::eSuccess ) {
#if VK_HEADER_VERSION >= 256
          vk::detail::throwResultException( vk::Result( err ), "getSurfaceFormatsKHR failed" );
#else
          vk::throwResultException( vk::Result( err ), "getSurfaceFormatsKHR failed" );
#endif
        }
        std::vector< vk::SurfaceFormatKHR > root( format_count );
        formats.resize( format_count );
        err = pdev->getSurfaceFormatsKHR( surface, &format_count, root.data() );
        if( err != vk::Result::eSuccess ) {
#if VK_HEADER_VERSION >= 256
          vk::detail::throwResultException( vk::Result( err ), "getSurfaceFormatsKHR failed" );
#else
          vk::throwResultException( vk::Result( err ), "getSurfaceFormatsKHR failed" );
#endif
        }
        for( std::uint32_t i = 0u; i != format_count; ++i ) {
          formats[ i ].basic = root[ i ];
        }
      }
    }
#endif
  }
  surface_capabilities_t &surface_capabilities_t::rebuild_chain() {
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
#ifdef VK_AMD_DISPLAY_NATIVE_HDR_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( display_native_hdr )
#endif
#ifdef VK_KHR_SHARED_PRESENTABLE_IMAGE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( shared_presentable_image )
#endif
#ifdef VK_EXT_FULL_SCREEN_EXCLUSIVE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( full_screen_exclusive )
#endif
#ifdef VK_KHR_SURFACE_PROTECTED_CAPABILITIES_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( surface_protected_capabilities )
#endif
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
  const surface_format_t &select_simple_surface_format( const surface_formats_t &formats ) {
    if( formats.empty() ) throw -1;
    const std::vector< vk::Format > supported_formats {
      vk::Format::eA2B10G10R10UnormPack32,
      vk::Format::eA2R10G10B10UnormPack32,
      vk::Format::eB8G8R8A8Unorm,
      vk::Format::eR8G8B8A8Unorm,
      vk::Format::eA8B8G8R8UnormPack32,
      vk::Format::eB5G5R5A1UnormPack16,
      vk::Format::eA1R5G5B5UnormPack16
    };
    bool surface_format_selected = false;
    for( const auto &s: supported_formats ) {
      auto selected_format = std::find_if( formats.begin(), formats.end(), [&]( const auto &v ) { return v.basic.format == s && v.basic.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear; } );
      if( selected_format != formats.end() )
        return *selected_format;
    }
    throw -1;
  }
#endif
}

