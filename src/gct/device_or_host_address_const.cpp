#include <gct/device_or_host_address_const.hpp>
#ifdef VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME
#include <nlohmann/json.hpp>
#include <gct/device_address.hpp>
namespace gct {
  device_or_host_address_const_t::device_or_host_address_const_t() : address( std::shared_ptr< void >() ) {}
  device_or_host_address_const_t &device_or_host_address_const_t::rebuild_chain() {
    if( chained ) return *this;
    if( std::holds_alternative< std::shared_ptr< device_address_t > >( address ) ) {
      value.deviceAddress = **std::get< std::shared_ptr< device_address_t > >( address );
    }
    else if( std::holds_alternative< std::shared_ptr< void > >( address ) ) {
      value.hostAddress = std::get< std::shared_ptr< void > >( address ).get();
    }
    else {
      value.hostAddress = nullptr;
    }
    chained = true;
    return *this;
  }
  device_or_host_address_const_t &device_or_host_address_const_t::set_host_address( const std::shared_ptr< void > &v ) {
    address = v;
    chained = false;
    return *this;
  }
  device_or_host_address_const_t &device_or_host_address_const_t::set_device_address( const std::shared_ptr< device_address_t > &v ) {
    address = v;
    chained = false;
    return *this;
  }
  device_or_host_address_const_t &device_or_host_address_const_t::clear_address() {
    address = std::shared_ptr< void >();
    chained = false;
    return *this;
  }
  void device_or_host_address_const_t::to_json( nlohmann::json &j ) const {
    j = nlohmann::json::object();
    if( std::holds_alternative< std::shared_ptr< device_address_t > >( address ) ) {
      j[ "device_address" ] = *std::get< std::shared_ptr< device_address_t > >( address );
    }
    else if( std::holds_alternative< std::shared_ptr< void > >( address ) ) {
      j[ "host_address" ] = std::intptr_t( std::get< std::shared_ptr< void > >( address ).get() );
    }
  }
  void to_json( nlohmann::json &j, const device_or_host_address_const_t &v ) {
    v.to_json( j );
  }
}
#endif

