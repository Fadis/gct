#include <gct/acceleration_structure_geometry_aabbs_data.hpp>
#ifdef VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME
#include <nlohmann/json.hpp>
#include <gct/extension.hpp>
#include <gct/device_or_host_address_const.hpp>
#include <vulkan2json/AccelerationStructureGeometryAabbsDataKHR.hpp>
namespace gct {
  acceleration_structure_geometry_aabbs_data_t &acceleration_structure_geometry_aabbs_data_t::rebuild_chain() {
    if( chained ) return *this;
    if( data )
      data.rebuild_chain();
    {
      auto basic = get_basic();
      if( data )
        basic
          .setData( *data );
      set_basic( std::move( basic ) );
    }
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
  acceleration_structure_geometry_aabbs_data_t &acceleration_structure_geometry_aabbs_data_t::set_data( const device_or_host_address_const_t &v ) {
    data = v;
    chained = false;
    return *this;
  }
  acceleration_structure_geometry_aabbs_data_t &acceleration_structure_geometry_aabbs_data_t::clear_data() {
    data.reset();
    chained = false;
    return *this;
  }
  void acceleration_structure_geometry_aabbs_data_t::to_json( nlohmann::json &root ) const {
    root = nlohmann::json::object();
    root[ "basic" ] = basic;
    root[ "basic" ][ "data" ] = data;
  }
  void to_json( nlohmann::json &root, const acceleration_structure_geometry_aabbs_data_t &v ) {
    v.to_json( root );
  }
}
#endif

