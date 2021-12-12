#include <gct/acceleration_structure_geometry.hpp>
#ifdef VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME
#include <nlohmann/json.hpp>
#include <vulkan2json/AccelerationStructureGeometryKHR.hpp>
namespace gct {
  acceleration_structure_geometry_t &acceleration_structure_geometry_t::rebuild_chain() {
    if( chained ) throw -1;
    auto basic = get_basic();
    if( std::holds_alternative< int >( geometry ) ) {}
    else if( std::holds_alternative< acceleration_structure_geometry_triangles_data_t >( geometry ) ) {
      std::get< acceleration_structure_geometry_triangles_data_t >( geometry ).rebuild_chain();
      basic.setGeometryType( vk::GeometryTypeKHR::eTriangles );
      basic.geometry.setTriangles( *std::get< acceleration_structure_geometry_triangles_data_t >( geometry ) );
    }
    else if( std::holds_alternative< acceleration_structure_geometry_aabbs_data_t >( geometry ) ) {
      std::get< acceleration_structure_geometry_aabbs_data_t >( geometry ).rebuild_chain();
      basic.setGeometryType( vk::GeometryTypeKHR::eAabbs );
      basic.geometry.setAabbs( *std::get< acceleration_structure_geometry_aabbs_data_t >( geometry ) );
    }
    else if( std::holds_alternative< acceleration_structure_geometry_instances_data_t >( geometry ) ) {
      std::get< acceleration_structure_geometry_instances_data_t >( geometry ).rebuild_chain();
      basic.setGeometryType( vk::GeometryTypeKHR::eInstances );
      basic.geometry.setInstances( *std::get< acceleration_structure_geometry_instances_data_t >( geometry ) );
    }
    else throw -1;
    set_basic( std::move( basic ) );
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
  acceleration_structure_geometry_t &acceleration_structure_geometry_t::set_triangle(
    const acceleration_structure_geometry_triangles_data_t &v
  ) {
    geometry = v;
    chained = false;
    return *this;
  }
  acceleration_structure_geometry_t &acceleration_structure_geometry_t::set_aabb(
    const acceleration_structure_geometry_aabbs_data_t &v
  ) {
    geometry = v;
    chained = false;
    return *this;
  }
  acceleration_structure_geometry_t &acceleration_structure_geometry_t::set_instance(
    const acceleration_structure_geometry_instances_data_t &v
  ) {
    geometry = v;
    chained = false;
    return *this;
  }
  acceleration_structure_geometry_t &acceleration_structure_geometry_t::clear_geometry_data() {
    geometry = 0;
    chained = false;
    return *this;
  }
  void acceleration_structure_geometry_t::to_json( nlohmann::json &root ) const {
    root = nlohmann::json::object();
    root[ "basic" ] = basic;
    if( std::holds_alternative< acceleration_structure_geometry_triangles_data_t >( geometry ) ) {
      root[ "basic" ][ "geometry" ] = std::get< acceleration_structure_geometry_triangles_data_t >( geometry );
    }
    else if( std::holds_alternative< acceleration_structure_geometry_aabbs_data_t >( geometry ) ) {
      root[ "basic" ][ "geometry" ] = std::get< acceleration_structure_geometry_aabbs_data_t >( geometry );
    }
    else if( std::holds_alternative< acceleration_structure_geometry_instances_data_t >( geometry ) ) {
      root[ "basic" ][ "geometry" ] = std::get< acceleration_structure_geometry_instances_data_t >( geometry );
    }
  }
  void to_json( nlohmann::json &root, const acceleration_structure_geometry_t &v ) {
    v.to_json( root );
  }
}
#endif

