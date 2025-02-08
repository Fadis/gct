#ifndef GCT_BUFFER_DEVICE_ADDRESS_INFO_HPP
#define GCT_BUFFER_DEVICE_ADDRESS_INFO_HPP
#include <vulkan/vulkan.hpp>
#if defined(VK_VERSION_1_2) || defined(VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME)
#include <memory>
#include <gct/extension.hpp>
#include <nlohmann/json_fwd.hpp>

namespace gct {
  class buffer_t;
  class buffer_device_address_info_t : public chained_t {
  public:
    using self_type = buffer_device_address_info_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
#ifdef VK_VERSION_1_2
    LIBGCT_BASIC_SETTER( vk::BufferDeviceAddressInfo )
#elif defined(VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME)
    LIBGCT_BASIC_SETTER( vk::BufferDeviceAddressInfoKHR )
#endif
  public:
    buffer_device_address_info_t &set_buffer( const std::shared_ptr< buffer_t >& );
    buffer_device_address_info_t &clear_buffer();
    [[nodiscard]] const std::shared_ptr< buffer_t > &get_buffer() const { return buffer; }
    void to_json( nlohmann::json& ) const;
  private:
    std::shared_ptr< buffer_t > buffer;
  };
  void to_json( nlohmann::json &root, const buffer_device_address_info_t &v );
}
#endif

#endif

