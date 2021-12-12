#include <gct/acceleration_structure_device_address_info.hpp>
#ifdef VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME
#include <vulkan2json/AccelerationStructureDeviceAddressInfoKHR.hpp>
#include <gct/acceleration_structure.hpp>
#include <nlohmann/json.hpp>

namespace gct {
  acceleration_structure_device_address_info_t &acceleration_structure_device_address_info_t::rebuild_chain() {
    if( chained ) return *this;
    if( acceleration_structure ) {
      auto basic = get_basic();
      basic.setAccelerationStructure( **acceleration_structure );
      set_basic( std::move( basic ) );
    }
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
  acceleration_structure_device_address_info_t &acceleration_structure_device_address_info_t::set_acceleration_structure( const std::shared_ptr< acceleration_structure_t > &v ) {
    acceleration_structure = v;
    chained = false;
    return *this;
  }
  acceleration_structure_device_address_info_t &acceleration_structure_device_address_info_t::clear_acceleration_structure() {
    acceleration_structure.reset();
    chained = false;
    return *this;
  }
  void acceleration_structure_device_address_info_t::to_json( nlohmann::json &root ) const {
    root = nlohmann::json::object();
    root[ "basic" ] = basic;
    if( acceleration_structure ) {
      root[ "basic" ][ "accelerationStructure" ] = *acceleration_structure;
    }
  }
  void to_json( nlohmann::json &root, const acceleration_structure_device_address_info_t &v ) {
    v.to_json( root );
  }
}
#endif



