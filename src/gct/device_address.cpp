#include <gct/device_address.hpp>
#ifdef VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME
#include <nlohmann/json.hpp>
#include <gct/buffer.hpp>
#include <gct/acceleration_structure.hpp>
namespace gct {
  device_address_t::device_address_t(
      const std::shared_ptr< buffer_t > &buffer_,
      vk::DeviceAddress address_
  ) : from( buffer_ ), address( address_ ) {
  }
  device_address_t::device_address_t(
      const std::shared_ptr< acceleration_structure_t > &acceleration_structure_,
      vk::DeviceAddress address_
  ) : from( acceleration_structure_ ), address( address_ ) {
  }
  device_address_t &device_address_t::operator+=( std::int64_t value ) {
    if( std::holds_alternative< std::shared_ptr< buffer_t > >( from ) ) {
      auto &buffer = std::get< std::shared_ptr< buffer_t > >( from );
      auto new_offset = offset + value;
      if( new_offset < 0 ) throw -1;
      if( new_offset > buffer->get_props().get_basic().size ) throw -1;
      if( value < 0 )
        address -= std::uint64_t( -value );
      else 
        address += std::uint64_t( value );
      offset = new_offset;
    }
    else throw -1;
    return *this;
  }
  void device_address_t::to_json( nlohmann::json &j ) const {
   j = address;
  }
  void to_json( nlohmann::json &j, const device_address_t &v ) {
    v.to_json( j );
  }
  device_address_t device_address_t::operator+( std::int64_t value ) const {
    if( std::holds_alternative< std::shared_ptr< buffer_t > >( from ) ) {
      auto &buffer = std::get< std::shared_ptr< buffer_t > >( from );
      auto v = device_address_t( buffer, address - offset );
      v += offset + value;
      return v;
    }
    else throw -1;
  }
  device_address_t device_address_t::operator-( std::int64_t value ) const {
    if( std::holds_alternative< std::shared_ptr< buffer_t > >( from ) ) {
      auto &buffer = std::get< std::shared_ptr< buffer_t > >( from );
      auto v = device_address_t( buffer, address - offset );
      v += offset - value;
      return v;
    }
    else throw -1;
  }
}
#endif

