#include <nlohmann/json.hpp>
#include <gct/allocator.hpp>
#include <gct/device.hpp>
#include <gct/buffer.hpp>
#include <gct/acceleration_structure_create_info.hpp>
#include <gct/acceleration_structure.hpp>
#include <gct/acceleration_structure_device_address_info.hpp>

namespace gct {
#ifdef VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME
  acceleration_structure_t::acceleration_structure_t(
    const std::shared_ptr< buffer_t > &buffer,
    const acceleration_structure_create_info_t &create_info
  ) :
    created_from< buffer_t >( buffer ),
    props( create_info ) {
    props.set_buffer( buffer ).rebuild_chain();
    handle = (*get_factory()->get_device())->createAccelerationStructureKHRUnique(
      props.get_basic()
    );
  }
  void to_json( nlohmann::json &root, const acceleration_structure_t &v ) {
    root = nlohmann::json::object();
    root[ "props" ] = v.get_props();
  }
#if defined(VK_VERSION_1_2) || defined(VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME)
  device_address_t acceleration_structure_t::get_address( const acceleration_structure_device_address_info_t &info ) {
    auto copied = info;
    copied.set_acceleration_structure( shared_from_this() ).rebuild_chain();
    return device_address_t(
      shared_from_this(),
      (*get_factory()->get_device())->getAccelerationStructureAddressKHR(
        copied.get_basic()
      )
    );
  }
  device_address_t acceleration_structure_t::get_address() {
    return get_address(
      acceleration_structure_device_address_info_t()
    );
  }
#endif
#endif
}

