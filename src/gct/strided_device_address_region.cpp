#include <gct/strided_device_address_region.hpp>
#ifdef VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME
#include <nlohmann/json.hpp>
#include <gct/device_address.hpp>
#include <vulkan2json/StridedDeviceAddressRegionKHR.hpp>
namespace gct {
  strided_device_address_region_t::strided_device_address_region_t(
    const std::shared_ptr< device_address_t > &head_,
    vk::DeviceSize stride_,
    vk::DeviceSize size_
  ) :
    created_from( head_ ) {
      raw
        .setDeviceAddress( **head_ )
        .setStride( stride_ )
        .setSize( size_ );
  }
  void strided_device_address_region_t::to_json( nlohmann::json &root ) const {
    root = raw;
  }
  void to_json( nlohmann::json &root, const strided_device_address_region_t &v ) {
    v.to_json( root );
  }
}
#endif


