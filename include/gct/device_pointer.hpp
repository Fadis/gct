#ifndef GCT_DEVICE_POINTER_HPP
#define GCT_DEVICE_POINTER_HPP

#include <vulkan/vulkan.hpp>
#ifdef VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME
#include <gct/device_address.hpp>
namespace gct {
  template< typename T >
  class device_pointer_t : public device_address_t {
  public:
    constexpr std::int64_t value_size = sizeof( T );
    using device_address_t::device_address_t;
    device_address_t &operator+=( std::int64_t value ) {
      device_address_t::operator+=( value * value_size );
      return *this;
    }
    device_address_t &operator-=( std::int64_t value ) {
      device_address_t::operator-=( value * value_size );
      return *this;
    }
    device_address_t &operator++() {
      device_address_t::operator+=( value_size );
      return *this;
    }
    device_address_t &operator--() {
      device_address_t::operator-=( value_size );
      return *this;
    }
    [[nodiscard]] std::int64_t operator-( const device_address_t &r ) const {
      return ( std::int64_t( r.offset ) - std::int64_t( offset ) ) / value_size;
    }
    [[nodiscard]] device_pointer_t operator+( std::int64_t value ) const {
      return device_pointer_t( get_factory(), ( offset + value ) * value_size );
    }
    [[nodiscard]] device_pointer_t operator-( std::int64_t value ) const {
      return device_pointer_t( get_factory(), ( offset - value ) * value_size );
    }
  };
#endif

#endif

