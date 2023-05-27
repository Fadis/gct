#include <nlohmann/json.hpp>
#include <gct/shader_module.hpp>
#include <gct/pipeline_dynamic_state_create_info.hpp>
#include <vulkan2json/PipelineDynamicStateCreateInfo.hpp>
namespace gct {
  void to_json( nlohmann::json &root, const pipeline_dynamic_state_create_info_t &v ) {
     root = nlohmann::json::object();
     root[ "basic" ] = v.get_basic();
  }
  void from_json( const nlohmann::json &root, pipeline_dynamic_state_create_info_t &v ) {
    if( !root.is_object() ) throw incompatible_json( "The JSON is incompatible to pipeline_dynamic_state_create_info_t", __FILE__, __LINE__ );
    LIBGCT_EXTENSION_FROM_JSON( basic )
  }
  pipeline_dynamic_state_create_info_t &pipeline_dynamic_state_create_info_t::rebuild_chain() {
    basic
      .setDynamicStateCount( dynamic_state.size() )
      .setPDynamicStates( dynamic_state.data() );
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
  pipeline_dynamic_state_create_info_t &pipeline_dynamic_state_create_info_t::add_dynamic_state( vk::DynamicState v ) {
    dynamic_state.push_back( v );
    chained = false;
    return *this;
  }
  pipeline_dynamic_state_create_info_t &pipeline_dynamic_state_create_info_t::clear_dynamic_state() {
    dynamic_state.clear();
    chained = false;
    return *this;
  }
}

