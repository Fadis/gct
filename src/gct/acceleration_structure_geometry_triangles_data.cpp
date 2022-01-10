#include <iostream>
#include <gct/acceleration_structure_geometry_triangles_data.hpp>
#ifdef VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME
#include <variant>
#include <memory>
#include <optional>
#include <vulkan/vulkan.hpp>
#include <nlohmann/json_fwd.hpp>
#include <gct/extension.hpp>
#include <gct/device_or_host_address_const.hpp>
#include <vulkan2json/AccelerationStructureGeometryTrianglesDataKHR.hpp>
#ifdef VK_NV_RAY_TRACING_MOTION_BLUR_EXTENSION_NAME
#include <vulkan2json/AccelerationStructureGeometryMotionTrianglesDataNV.hpp>
#endif
namespace gct {
  acceleration_structure_geometry_triangles_data_t &acceleration_structure_geometry_triangles_data_t::rebuild_chain() {
    if( chained ) return *this;
    if( vertex_data )
      vertex_data.rebuild_chain();
    if( index_data )
      index_data.rebuild_chain();
    if( transform_data )
      transform_data.rebuild_chain();
    {
      auto basic = get_basic();
      if( vertex_data ) {
        basic.setVertexData( *vertex_data );
      }
      if( index_data )
        basic.setIndexData( *index_data );
      if( transform_data )
        basic.setTransformData( *transform_data );
      set_basic( std::move( basic ) );
    }
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
#ifdef VK_NV_RAY_TRACING_MOTION_BLUR_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( motion ) 
#endif
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
  acceleration_structure_geometry_triangles_data_t &acceleration_structure_geometry_triangles_data_t::set_vertex_data( const device_or_host_address_const_t &v ) {
    vertex_data = v;
    chained = false;
    return *this;
  }
  acceleration_structure_geometry_triangles_data_t &acceleration_structure_geometry_triangles_data_t::clear_vertex_data() {
    vertex_data.reset();
    chained = false;
    return *this;
  }
  acceleration_structure_geometry_triangles_data_t &acceleration_structure_geometry_triangles_data_t::set_index_data( const device_or_host_address_const_t &v ) {
    index_data = v;
    chained = false;
    return *this;
  }
  acceleration_structure_geometry_triangles_data_t &acceleration_structure_geometry_triangles_data_t::clear_index_data() {
    index_data.reset();
    chained = false;
    return *this;
  }
  acceleration_structure_geometry_triangles_data_t &acceleration_structure_geometry_triangles_data_t::set_transform_data( const device_or_host_address_const_t &v ) {
    transform_data = v;
    chained = false;
    return *this;
  }
  acceleration_structure_geometry_triangles_data_t &acceleration_structure_geometry_triangles_data_t::clear_transform_data() {
    transform_data.reset();
    chained = false;
    return *this;
  }
  void to_json( nlohmann::json &root, const acceleration_structure_geometry_triangles_data_t &v ) {
    root = nlohmann::json::object();
    root[ "basic" ] = v.get_basic();
#ifdef VK_NV_RAY_TRACING_MOTION_BLUR_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( motion )
#endif
  }
}
#endif



