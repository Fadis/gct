#ifndef GCT_ACCELERATION_STRUCTURE_GEOMETRY_TRIANGLES_DATA_HPP
#define GCT_ACCELERATION_STRUCTURE_GEOMETRY_TRIANGLES_DATA_HPP

#include <vulkan/vulkan.hpp>
#ifdef VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME
#include <memory>
#include <nlohmann/json_fwd.hpp>
#include <gct/extension.hpp>
#include <gct/device_or_host_address_const.hpp>
namespace gct {
  class device_or_host_address_const_t;
  class acceleration_structure_geometry_triangles_data_t : public chained_t {
  public:
    using self_type = acceleration_structure_geometry_triangles_data_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
    LIBGCT_BASIC_SETTER( vk::AccelerationStructureGeometryTrianglesDataKHR )
#ifdef VK_NV_RAY_TRACING_MOTION_BLUR_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::AccelerationStructureGeometryMotionTrianglesDataNV , motion )
#endif
  public:
    acceleration_structure_geometry_triangles_data_t &set_vertex_data( const device_or_host_address_const_t& );
    acceleration_structure_geometry_triangles_data_t &clear_vertex_data();
    acceleration_structure_geometry_triangles_data_t &set_index_data( const device_or_host_address_const_t& );
    acceleration_structure_geometry_triangles_data_t &clear_index_data();
    acceleration_structure_geometry_triangles_data_t &set_transform_data( const device_or_host_address_const_t& );
    acceleration_structure_geometry_triangles_data_t &clear_transform_data();
    [[nodiscard]] vk::AccelerationStructureGeometryTrianglesDataKHR &operator*() {
      return basic;
    }
    [[nodiscard]] const vk::AccelerationStructureGeometryTrianglesDataKHR &operator*() const {
      return basic;
    }
    [[nodiscard]] vk::AccelerationStructureGeometryTrianglesDataKHR *operator->() {
      return &basic;
    }
    [[nodiscard]] const vk::AccelerationStructureGeometryTrianglesDataKHR *operator->() const {
      return &basic;
    }
  private:
    device_or_host_address_const_t vertex_data;
    device_or_host_address_const_t index_data;
    device_or_host_address_const_t transform_data;
  };
  void to_json( nlohmann::json &root, const acceleration_structure_geometry_triangles_data_t &v );
}
#endif

#endif



