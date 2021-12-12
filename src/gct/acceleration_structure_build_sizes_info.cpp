#include <gct/acceleration_structure_build_sizes_info.hpp>
#ifdef VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME
#include <nlohmann/json.hpp>
#include <gct/extension.hpp>
#include <vulkan2json/AccelerationStructureBuildSizesInfoKHR.hpp>
namespace gct {
  acceleration_structure_build_sizes_info_t &acceleration_structure_build_sizes_info_t::rebuild_chain() {
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
  void acceleration_structure_build_sizes_info_t::to_json( nlohmann::json &root ) const {
    root = nlohmann::json::object();
    root[ "basic" ] = basic;
  }
  void to_json( nlohmann::json &root, const acceleration_structure_build_sizes_info_t &v ) {
    v.to_json( root );
  }
}
#endif

