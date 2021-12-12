#ifndef GCT_ACCELERATION_STRUCTURE_GEOMETRY_AABBS_DATA_HPP
#define GCT_ACCELERATION_STRUCTURE_GEOMETRY_AABBS_DATA_HPP

#include <vulkan/vulkan.hpp>
#ifdef VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME
#include <variant>
#include <memory>
#include <optional>
#include <nlohmann/json_fwd.hpp>
#include <gct/extension.hpp>
namespace gct {
  class device_or_host_address_const_t;
  class acceleration_structure_geometry_aabbs_data_t : public chained_t {
  public:
    using self_type = acceleration_structure_geometry_aabbs_data_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
    LIBGCT_BASIC_SETTER( vk::AccelerationStructureGeometryAabbsDataKHR )
  public:
    acceleration_structure_geometry_aabbs_data_t &set_data( const std::shared_ptr< device_or_host_address_const_t >& );
    acceleration_structure_geometry_aabbs_data_t &clear_data();
    void to_json( nlohmann::json &j ) const;
    vk::AccelerationStructureGeometryAabbsDataKHR &operator*() {
      return basic;
    }
    const vk::AccelerationStructureGeometryAabbsDataKHR &operator*() const {
      return basic;
    }
    vk::AccelerationStructureGeometryAabbsDataKHR *operator->() {
      return &basic;
    }
    const vk::AccelerationStructureGeometryAabbsDataKHR *operator->() const {
      return &basic;
    }
  private:
    std::shared_ptr< device_or_host_address_const_t > data;
  };
  void to_json( nlohmann::json &root, const acceleration_structure_geometry_aabbs_data_t &v );
}
#endif

#endif

