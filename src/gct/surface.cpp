#include <gct/instance.hpp>
#include <gct/physical_device.hpp>
#include <gct/surface.hpp>
#ifdef VK_KHR_SURFACE_EXTENSION_NAME
#include <vulkan2json/SurfaceCapabilitiesKHR.hpp>
#endif

namespace gct {
#ifdef VK_KHR_SURFACE_EXTENSION_NAME
  void to_json( nlohmann::json &root, const surface_t &v ) {
    root = nlohmann::json::object();
    root[ "caps" ] = v.get_caps();
  }

  surface_t::surface_t(
    const physical_device_t &pdev,
    vk::SurfaceKHR raw_handle
  ) :
    created_from< instance_t >( pdev.get_factory() ),
    handle(
      raw_handle,
      **pdev.get_factory()
    ),
    caps(
      pdev,
      raw_handle
    )
  {}
#if defined(VK_KHR_SURFACE_EXTENSION_NAME) && defined(VK_KHR_DISPLAY_EXTENSION_NAME)
  surface_t::surface_t(
    const physical_device_t &pdev,
    vk::UniqueHandle< vk::SurfaceKHR, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE > &&h
  ) :
    created_from< instance_t >( pdev.get_factory() ),
    handle( std::move( h ) ),
    caps(
      pdev,
      handle.get()
    )
  {}
#endif
#endif
}

