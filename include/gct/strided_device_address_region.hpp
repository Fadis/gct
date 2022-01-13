#ifndef GCT_STRIDED_DEVICE_ADDRESS_REGION_HPP
#define GCT_STRIDED_DEVICE_ADDRESS_REGION_HPP
#include <variant>
#include <memory>
#include <vulkan/vulkan.hpp>
#ifdef VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME
#include <nlohmann/json_fwd.hpp>
#include <gct/created_from.hpp>
namespace gct {
  class device_address_t;
  class buffer_t;
  class acceleration_structure_t;
  class strided_device_address_region_t {
  public:
    using self_type = strided_device_address_region_t;
    strided_device_address_region_t(
      const std::variant<
        std::shared_ptr< buffer_t >,
        std::shared_ptr< acceleration_structure_t >
      > &from_,
      vk::DeviceAddress address_,
      vk::DeviceSize stride_,
      vk::DeviceSize size_
    );
    strided_device_address_region_t();
    vk::StridedDeviceAddressRegionKHR &operator*() {
      return raw;
    }
    const vk::StridedDeviceAddressRegionKHR &operator*() const {
      return raw;
    }
    vk::StridedDeviceAddressRegionKHR *operator->() {
      return &raw;
    }
    const vk::StridedDeviceAddressRegionKHR *operator->() const {
      return &raw;
    }
    void to_json( nlohmann::json& ) const;
    operator bool() const {
      return raw.deviceAddress != 0u;
    }
  private:
    std::variant<
      std::shared_ptr< buffer_t >,
      std::shared_ptr< acceleration_structure_t >
    > from;
    vk::StridedDeviceAddressRegionKHR raw;
  };
  void to_json( nlohmann::json&, const strided_device_address_region_t& );
}
#endif

#endif


