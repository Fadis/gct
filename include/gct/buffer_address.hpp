#ifndef GCT_DEVICE_ADDRESS_HPP
#define GCT_DEVICE_ADDRESS_HPP

#include <vulkan/vulkan.hpp>
#ifdef VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME
#include <memory>
#include <nlohmann/json_fwd.hpp>
#include <gct/created_from.hpp>
#include <gct/extension.hpp>
namespace gct {
  class buffer_t;
  class buffer_address_t :
    public created_from< buffer_t >,
    public std::enable_shared_from_this< buffer_address_t > {
  public:
    buffer_address_t(
      const std::shared_ptr< buffer_t > &buffer_,
      vk::DeviceAddress address_,
      std::int64_t offset_ = 0ll
    );
    vk::DeviceAddress operator*() {
      return address;
    }
    buffer_address_t &operator+=( std::int64_t value );
    buffer_address_t &operator-=( std::int64_t value ) {
      operator+=( -value );
      return *this;
    }
    buffer_address_t &operator++() {
      operator+=( 1 );
      return *this;
    }
    buffer_address_t &operator--() {
      operator-=( 1 );
      return *this;
    }
    std::int64_t operator-( const buffer_address_t &r ) const {
      return std::int64_t( r.offset ) - std::int64_t( offset );
    }
    buffer_address_t operator+( std::int64_t value ) const {
      return buffer_address_t( get_factory(), offset + value );
    }
    buffer_address_t operator-( std::int64_t value ) const {
      return buffer_address_t( get_factory(), offset - value );
    }
    void to_json( nlohmann::json& ) const;
  private:
    vk::DeviceAddress address = 0u;
    std::int64_t offset = 0u;
  };
  void to_json( nlohmann::json&, const buffer_address_t& );
}
#endif

#endif

