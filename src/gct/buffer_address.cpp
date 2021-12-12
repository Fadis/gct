#include <gct/buffer_address.hpp>
#ifdef VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME
#include <nlohmann/json.hpp>
#include <gct/buffer.hpp>
namespace gct {
  buffer_address_t::buffer_address_t(
      const std::shared_ptr< buffer_t > &buffer_,
      vk::DeviceAddress address_,
      std::int64_t offset_
  ) : created_from< buffer_t >( buffer_ ), address( address_ ), offset( offset_ ) {
    if( offset < 0 ) throw -1;
    if( offset > get_factory()->get_props().get_basic().size ) throw -1;
  }
  buffer_address_t &buffer_address_t::operator+=( std::int64_t value ) {
    auto new_offset = offset + value;
    if( new_offset < 0 ) throw -1;
    if( new_offset > get_factory()->get_props().get_basic().size ) throw -1;
    if( value < 0 )
      address -= std::uint64_t( -value );
    else 
      address += std::uint64_t( value );
    offset = new_offset;
    return *this;
  }
  void buffer_address_t::to_json( nlohmann::json &j ) const {
   j = address;
  }
  void to_json( nlohmann::json &j, const buffer_address_t &v ) {
    v.to_json( j );
  }
}
#endif

