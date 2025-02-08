#ifndef GCT_DEVICE_OR_HOST_ADDRESS_CONST_HPP
#define GCT_DEVICE_OR_HOST_ADDRESS_CONST_HPP

#include <vulkan/vulkan.hpp>
#ifdef VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME
#include <variant>
#include <memory>
#include <nlohmann/json_fwd.hpp>
#include <gct/extension.hpp>
#include <gct/device_address.hpp>
namespace gct {
  class device_or_host_address_const_t {
  public:
    device_or_host_address_const_t();
    device_or_host_address_const_t( const std::shared_ptr< void >& );
    device_or_host_address_const_t( const device_address_t& );
    device_or_host_address_const_t &rebuild_chain();
    device_or_host_address_const_t &set_host_address( const std::shared_ptr< void > &v );
    device_or_host_address_const_t &set_device_address( const device_address_t &v );
    device_or_host_address_const_t &clear_address();
    void to_json( nlohmann::json& ) const;
    [[nodiscard]] vk::DeviceOrHostAddressConstKHR &operator*() {
      return value;
    }
    [[nodiscard]] const vk::DeviceOrHostAddressConstKHR &operator*() const {
      return value;
    }
    void reset() {
      address = std::shared_ptr< void >();
    }
    [[nodiscard]] operator bool() const;
  private:
    vk::DeviceOrHostAddressConstKHR value;
    std::variant<
      std::shared_ptr< void >,
      device_address_t
    > address;
    bool chained = true;
  };
  void to_json( nlohmann::json&, const device_or_host_address_const_t& );
}
#endif

#endif

