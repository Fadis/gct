#ifndef GCT_ACCELERATION_STRUCTURE_BUILD_SIZES_INFO_HPP
#define GCT_ACCELERATION_STRUCTURE_BUILD_SIZES_INFO_HPP

#include <vulkan/vulkan.hpp>
#ifdef VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME
#include <nlohmann/json_fwd.hpp>
#include <gct/extension.hpp>
namespace gct {
  class acceleration_structure_build_sizes_info_t : public chained_t {
  public:
    using self_type = acceleration_structure_build_sizes_info_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
    LIBGCT_BASIC_SETTER( vk::AccelerationStructureBuildSizesInfoKHR )
  public:
    void to_json( nlohmann::json &j ) const;
    [[nodiscard]] vk::AccelerationStructureBuildSizesInfoKHR &operator*() {
      return basic;
    }
    [[nodiscard]] const vk::AccelerationStructureBuildSizesInfoKHR &operator*() const {
      return basic;
    }
    [[nodiscard]] vk::AccelerationStructureBuildSizesInfoKHR *operator->() {
      return &basic;
    }
    [[nodiscard]] const vk::AccelerationStructureBuildSizesInfoKHR *operator->() const {
      return &basic;
    }
  };
  void to_json( nlohmann::json &root, const acceleration_structure_build_sizes_info_t &v );
}
#endif

#endif

