#ifndef GCT_ACCELERATION_STRUCTURE_DEVICE_ADDRESS_INFO_HPP
#define GCT_ACCELERATION_STRUCTURE_DEVICE_ADDRESS_INFO_HPP
#include <vulkan/vulkan.hpp>
#ifdef VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME
#include <memory>
#include <gct/extension.hpp>
#include <nlohmann/json_fwd.hpp>

namespace gct {
  class acceleration_structure_t;
  class acceleration_structure_device_address_info_t : public chained_t {
  public:
    using self_type = acceleration_structure_device_address_info_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
    LIBGCT_BASIC_SETTER( vk::AccelerationStructureDeviceAddressInfoKHR )
  public:
    acceleration_structure_device_address_info_t &set_acceleration_structure( const std::shared_ptr< acceleration_structure_t >& );
    acceleration_structure_device_address_info_t &clear_acceleration_structure();
    void to_json( nlohmann::json &root ) const;
  private:
    std::shared_ptr< acceleration_structure_t > acceleration_structure;
  };
  void to_json( nlohmann::json &root, const acceleration_structure_device_address_info_t &v );
}
#endif

#endif


