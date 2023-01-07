#include <gct/instance.hpp>
#include <gct/physical_device.hpp>
#include <gct/display_mode.hpp>

namespace gct {

/*
 * This doesn't work due to lack of UniqueHandleTraits<DisplayModeKHR>
 */

#if 0
#ifdef VK_KHR_DISPLAY_EXTENSION_NAME
  void to_json( nlohmann::json &dest, const display_mode_t &v ) {
    dest = v.get_props();
  }
  display_mode_t::display_mode_t(
    const std::shared_ptr< instance_t > &instance,
    const physical_device_t &physical_device,
    vk::DisplayKHR display,
    const display_mode_create_info_t &create_info
  ) :
    created_from< instance_t >( instance ),
    props( create_info ) {
    props.rebuild_chain();
    handle = physical_device->createDisplayModeKHRUnique(
      display,
      props.get_basic()
    );
  }
#endif
#endif

}

