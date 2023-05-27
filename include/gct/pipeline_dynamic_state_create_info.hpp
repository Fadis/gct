#ifndef GCT_PIPELINE_DYNAMIC_STATE_CREATE_INFO_HPP
#define GCT_PIPELINE_DYNAMIC_STATE_CREATE_INFO_HPP
#include <memory>
#include <vector>
#include <nlohmann/json_fwd.hpp>
#include <vulkan/vulkan.hpp>
#include <gct/extension.hpp>

namespace gct {
  class pipeline_dynamic_state_create_info_t : public chained_t {
  public:
    using self_type = pipeline_dynamic_state_create_info_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
    LIBGCT_BASIC_SETTER( vk::PipelineDynamicStateCreateInfo )
  private:
    std::vector< vk::DynamicState > dynamic_state;
  public:
    pipeline_dynamic_state_create_info_t &add_dynamic_state( vk::DynamicState );
    pipeline_dynamic_state_create_info_t &clear_dynamic_state();
  };
  void to_json( nlohmann::json &root, const pipeline_dynamic_state_create_info_t &v );
  void from_json( const nlohmann::json &root, pipeline_dynamic_state_create_info_t &v );
}

#endif

