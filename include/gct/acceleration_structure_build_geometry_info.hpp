#ifndef GCT_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_HPP
#define GCT_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_HPP

#include <vulkan/vulkan.hpp>
#ifdef VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME
#include <variant>
#include <memory>
#include <nlohmann/json_fwd.hpp>
#include <gct/extension.hpp>
#include <gct/device_or_host_address.hpp>
namespace gct {
  class acceleration_structure_geometry_t;
  class acceleration_structure_t;
  class acceleration_structure_build_geometry_info_t : public chained_t {
  public:
    using self_type = acceleration_structure_build_geometry_info_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
    LIBGCT_BASIC_SETTER( vk::AccelerationStructureBuildGeometryInfoKHR )
  public:
    acceleration_structure_build_geometry_info_t &add_geometry(
      const acceleration_structure_geometry_t&
    );
    acceleration_structure_build_geometry_info_t &clear_geometry();
    acceleration_structure_build_geometry_info_t &set_src( const std::shared_ptr< acceleration_structure_t >& );
    acceleration_structure_build_geometry_info_t &clear_src();
    acceleration_structure_build_geometry_info_t &set_dst( const std::shared_ptr< acceleration_structure_t >& );
    acceleration_structure_build_geometry_info_t &clear_dst();
    acceleration_structure_build_geometry_info_t &set_scratch( const device_or_host_address_t& );
    acceleration_structure_build_geometry_info_t &clear_scratch();
    void to_json( nlohmann::json &root ) const;
  private:
    std::vector< acceleration_structure_geometry_t > geometry;
    std::vector< const vk::AccelerationStructureGeometryKHR* > raw_geometry;
    std::shared_ptr< acceleration_structure_t > src;
    std::shared_ptr< acceleration_structure_t > dst;
    device_or_host_address_t scratch;
  };
  void to_json( nlohmann::json &root, const acceleration_structure_build_geometry_info_t &v );
}
#endif

#endif


