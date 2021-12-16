#ifndef GCT_STRIDED_DEVICE_ADDRESS_REGION_HPP
#define GCT_STRIDED_DEVICE_ADDRESS_REGION_HPP

#include <vulkan/vulkan.hpp>
#ifdef VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME
#include <nlohmann/json_fwd.hpp>
#include <gct/created_from.hpp>
namespace gct {
  class device_address_t;
  class strided_device_address_region_t :
    public created_from< device_address_t >,
    public std::enable_shared_from_this< strided_device_address_region_t > {
  public:
    using self_type = strided_device_address_region_t;
    strided_device_address_region_t(
      const std::shared_ptr< device_address_t > &head_,
      vk::DeviceSize stride_,
      vk::DeviceSize size_
    );
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
  private:
    vk::StridedDeviceAddressRegionKHR raw;
  };
  void to_json( nlohmann::json&, const device_address_t& );
}
#endif

#endif


