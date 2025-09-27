#include <nlohmann/json.hpp>
#include <gct/shader_module.hpp>
#include <gct/pipeline_dynamic_state_create_info.hpp>
#include <vulkan2json/PipelineDynamicStateCreateInfo.hpp>
#include <vulkan2json/DynamicState.hpp>
namespace gct {
  void to_json( nlohmann::json &root, const pipeline_dynamic_state_create_info_t &v ) {
    root = nlohmann::json::object();
    root[ "basic" ] = v.get_basic();
    LIBGCT_ARRAY_OF_TO_JSON( basic, pDynamicStates, dynamic_state );
  }
  void from_json( const nlohmann::json &root, pipeline_dynamic_state_create_info_t &v ) {
    if( !root.is_object() ) throw incompatible_json( "The JSON is incompatible to pipeline_dynamic_state_create_info_t", __FILE__, __LINE__ );
    LIBGCT_EXTENSION_FROM_JSON( basic )
    LIBGCT_ARRAY_OF_FROM_JSON( basic, pDynamicStates, dynamic_state );
  }
  pipeline_dynamic_state_create_info_t &pipeline_dynamic_state_create_info_t::rebuild_chain() {
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
    std::sort( dynamic_state.begin(), dynamic_state.end() );
    dynamic_state.erase( std::unique( dynamic_state.begin(), dynamic_state.end() ), dynamic_state.end() );
    LIBGCT_ARRAY_OF_REBUILD_CHAIN( basic, DynamicStateCount, PDynamicStates, dynamic_state )
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
}

