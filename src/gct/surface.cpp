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
      vk::ObjectDestroy< vk::Instance, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE >(
        **get_factory(), nullptr, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE ()
      )
    ),
    caps(
      pdev,
      raw_handle
    )
  {}
#endif
}

