#include <gct/surface.hpp>
#include <gct/swapchain_create_info.hpp>

namespace gct {
#ifdef VK_KHR_SWAPCHAIN_EXTENSION_NAME
  swapchain_create_info_t &swapchain_create_info_t::rebuild_chain() {
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
#if defined(VK_VERSION_1_1) || defined(VK_KHR_DEVICE_GROUP_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( device_group )
#endif
#ifdef VK_VERSION_1_2
    LIBGCT_EXTENSION_REBUILD_CHAIN( format_list )
#elif defined(VK_KHR_IMAGE_FORMAT_LIST_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( format_list )
#endif
#ifdef VK_EXT_FULL_SCREEN_EXCLUSIVE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( full_screen_exclusive )
#ifdef VK_KHR_WIN32_SURFACE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( full_screen_exclusive_win32 )
#endif
#endif
#ifdef VK_EXT_DISPLAY_CONTROL_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( counter )
#endif
#ifdef VK_AMD_DISPLAY_NATIVE_HDR_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( display_antive_hdr )
#endif
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
  swapchain_create_info_t &swapchain_create_info_t::set_surface(
    const std::shared_ptr< surface_t > &surface_,
    const surface_format_t &format,
    std::uint32_t image_count
  ) {
    surface = surface_;
    auto modified = get_basic();
    modified
      .setSurface( **surface )
      .setMinImageCount(
        std::max(
          std::min(
            image_count,
            surface->get_caps().get_basic().maxImageCount
          ),
          surface->get_caps().get_basic().minImageCount
        )
      )
      .setImageFormat( format.basic.format )
      .setImageColorSpace( format.basic.colorSpace )
      .setImageExtent( surface->get_caps().get_basic().currentExtent )
      .setImageArrayLayers( 1 )
      .setImageUsage( vk::ImageUsageFlagBits::eColorAttachment )
      .setPreTransform(
        ( surface->get_caps().get_basic().supportedTransforms & vk::SurfaceTransformFlagBitsKHR::eIdentity ) ?
        vk::SurfaceTransformFlagBitsKHR::eIdentity :
        surface->get_caps().get_basic().currentTransform
      );
    set_basic( std::move( modified ) );
    return *this;
  }
  swapchain_create_info_t &swapchain_create_info_t::set_surface(
    const std::shared_ptr< surface_t > &surface_
  ) {
    const auto &formats = surface_->get_caps().get_formats();
    if( formats.empty() ) {
      throw -1;
    }
    static const std::vector< vk::Format > supported_formats {
      vk::Format::eA2B10G10R10UnormPack32,
      vk::Format::eA2R10G10B10UnormPack32,
      vk::Format::eB8G8R8A8Unorm,
      vk::Format::eR8G8B8A8Unorm,
      vk::Format::eA8B8G8R8UnormPack32,
      vk::Format::eB5G5R5A1UnormPack16,
      vk::Format::eA1R5G5B5UnormPack16
    };
    for( const auto &s: supported_formats ) {
      auto selected_format = std::find_if( formats.begin(), formats.end(), [&]( const auto &v ) { return v.basic.format == s && v.basic.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear; } );
      if( selected_format != formats.end() ) {
        return set_surface( surface_, *selected_format, surface_->get_caps().get_basic().minImageCount );
      }
    }
    throw -1;
  }
#endif
}

