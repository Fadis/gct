#ifndef GCT_ACCELERATION_STRUCTURE_GEOMETRY_HPP
#define GCT_ACCELERATION_STRUCTURE_GEOMETRY_HPP

#include <vulkan/vulkan.hpp>
#ifdef VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME
#include <variant>
#include <memory>
#include <nlohmann/json_fwd.hpp>
#include <gct/extension.hpp>
#include <gct/acceleration_structure_geometry_triangles_data.hpp>
#include <gct/acceleration_structure_geometry_aabbs_data.hpp>
#include <gct/acceleration_structure_geometry_instances_data.hpp>
namespace gct {
  class acceleration_structure_geometry_triangles_data_t;
  class acceleration_structure_geometry_aabbs_data_t;
  class acceleration_structure_geometry_instances_data_t;
  class acceleration_structure_geometry_t : public chained_t {
  public:
    using self_type = acceleration_structure_geometry_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
    LIBGCT_BASIC_SETTER( vk::AccelerationStructureGeometryKHR )
  public:
    acceleration_structure_geometry_t &set_triangle(
      const acceleration_structure_geometry_triangles_data_t&
    );
    acceleration_structure_geometry_t &set_aabb(
      const acceleration_structure_geometry_aabbs_data_t&
    );
    acceleration_structure_geometry_t &set_instance(
      const acceleration_structure_geometry_instances_data_t&
    );
    acceleration_structure_geometry_t &clear_geometry_data();
    void to_json( nlohmann::json &root ) const;
  private:
    std::variant<
      int,
      acceleration_structure_geometry_triangles_data_t,
      acceleration_structure_geometry_aabbs_data_t,
      acceleration_structure_geometry_instances_data_t
    > geometry;
  };
  void to_json( nlohmann::json &root, const acceleration_structure_geometry_t &v );
}
#endif

#endif


