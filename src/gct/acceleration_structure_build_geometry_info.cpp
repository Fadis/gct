#include <gct/acceleration_structure_build_geometry_info.hpp>
#ifdef VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME
#include <gct/acceleration_structure_geometry.hpp>
#include <gct/acceleration_structure.hpp>
#include <nlohmann/json.hpp>
#include <gct/extension.hpp>
#include <vulkan2json/AccelerationStructureBuildGeometryInfoKHR.hpp>

namespace gct {
  acceleration_structure_build_geometry_info_t &acceleration_structure_build_geometry_info_t::rebuild_chain() {
    if( chained ) throw -1;
    auto basic = get_basic();
    if( src )
      basic.setSrcAccelerationStructure( **src );
    if( dst )
      basic.setDstAccelerationStructure( **dst );
    raw_geometry.clear();
    raw_geometry.reserve( geometry.size() );
    std::transform( geometry.begin(), geometry.end(), std::back_inserter( raw_geometry ), []( auto &v ) {
      v.rebuild_chain();
      return &v.get_basic();
    } );
    basic
      .setPGeometries( nullptr )
      .setGeometryCount( raw_geometry.size() )
      .setPpGeometries( raw_geometry.data() );
    set_basic( std::move( basic ) );
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
  acceleration_structure_build_geometry_info_t &acceleration_structure_build_geometry_info_t::add_geometry(
    const acceleration_structure_geometry_t &v
  ) {
    geometry.push_back( v );
    chained = false;
    return *this;
  }
  acceleration_structure_build_geometry_info_t &acceleration_structure_build_geometry_info_t::clear_geometry() {
    geometry.clear();
    chained = false;
    return *this;
  }
  acceleration_structure_build_geometry_info_t &acceleration_structure_build_geometry_info_t::set_src( const std::shared_ptr< acceleration_structure_t > &v ) {
    src = v;
    chained = false;
    return *this;
  }
  acceleration_structure_build_geometry_info_t &acceleration_structure_build_geometry_info_t::clear_src() {
    src.reset();
    chained = false;
    return *this;
  }
  acceleration_structure_build_geometry_info_t &acceleration_structure_build_geometry_info_t::set_dst( const std::shared_ptr< acceleration_structure_t > &v ) {
    dst = v;
    chained = false;
    return *this;
  }
  acceleration_structure_build_geometry_info_t &acceleration_structure_build_geometry_info_t::clear_dst() {
    dst.reset();
    chained = false;
    return *this;
  }
  void acceleration_structure_build_geometry_info_t::to_json( nlohmann::json &root ) const {
    root = nlohmann::json::object();
    root[ "basic" ] = basic;
    root[ "basic" ][ "geometry" ] = geometry;
  }
  void to_json( nlohmann::json &root, const acceleration_structure_build_geometry_info_t &v ) {
    v.to_json( root );
  }
}
#endif


