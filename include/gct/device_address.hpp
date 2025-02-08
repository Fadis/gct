#ifndef GCT_DEVICE_ADDRESS_HPP
#define GCT_DEVICE_ADDRESS_HPP

#include <vulkan/vulkan.hpp>
#ifdef VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME
#include <memory>
#include <variant>
#include <nlohmann/json_fwd.hpp>
#include <gct/created_from.hpp>
#include <gct/extension.hpp>
namespace gct {
  class buffer_t;
  class acceleration_structure_t;
  class strided_device_address_region_t;
  class device_address_t {
  public:
    device_address_t(
      const std::shared_ptr< buffer_t > &buffer_,
      vk::DeviceAddress address_
    );
    device_address_t(
      const std::shared_ptr< acceleration_structure_t > &buffer_,
      vk::DeviceAddress address_
    );
    [[nodiscard]] const vk::DeviceAddress &operator*() const {
      return address;
    }
    device_address_t &operator+=( std::int64_t value );
    device_address_t &operator-=( std::int64_t value ) {
      operator+=( -value );
      return *this;
    }
    device_address_t &operator++() {
      operator+=( 1 );
      return *this;
    }
    device_address_t &operator--() {
      operator-=( 1 );
      return *this;
    }
    [[nodiscard]] std::int64_t operator-( const device_address_t &r ) const {
      return std::int64_t( r.offset ) - std::int64_t( offset );
    }
    [[nodiscard]] device_address_t operator+( std::int64_t value ) const;
    [[nodiscard]] device_address_t operator-( std::int64_t value ) const;
#ifdef VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME
    [[nodiscard]] strided_device_address_region_t get_strided(
      vk::DeviceSize stride,
      vk::DeviceSize size
    );
#endif
    void to_json( nlohmann::json& ) const;
  private:
    std::variant<
      std::shared_ptr< buffer_t >,
      std::shared_ptr< acceleration_structure_t >
    > from;
    vk::DeviceAddress address = 0u;
    std::int64_t offset = 0u;
  };
  void to_json( nlohmann::json&, const device_address_t& );
}
#endif

#endif

