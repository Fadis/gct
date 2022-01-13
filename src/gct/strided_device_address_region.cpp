#include <gct/strided_device_address_region.hpp>
#ifdef VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME
#include <nlohmann/json.hpp>
#include <gct/device_address.hpp>
#include <vulkan2json/StridedDeviceAddressRegionKHR.hpp>
namespace gct {
  strided_device_address_region_t::strided_device_address_region_t(
    const std::variant<
      std::shared_ptr< buffer_t >,
      std::shared_ptr< acceleration_structure_t >
    > &from_,
    vk::DeviceAddress address_,
    vk::DeviceSize stride_,
    vk::DeviceSize size_
  ) :
    from( from_ ) {
      raw
        .setDeviceAddress( address_ )
        .setStride( stride_ )
        .setSize( size_ );
  }
  strided_device_address_region_t::strided_device_address_region_t() {
      raw
        .setDeviceAddress( 0u )
        .setStride( 0u )
        .setSize( 0u );
  }
  void strided_device_address_region_t::to_json( nlohmann::json &root ) const {
    root = raw;
  }
  void to_json( nlohmann::json &root, const strided_device_address_region_t &v ) {
    v.to_json( root );
  }
}
#endif


