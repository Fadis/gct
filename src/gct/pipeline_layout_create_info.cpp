#include <algorithm>
#include <nlohmann/json.hpp>
#include <gct/descriptor_set_layout.hpp>
#include <gct/pipeline_layout_create_info.hpp>
#include <gct/shader_module.hpp>
#include <gct/shader_module_reflection.hpp>
#include <gct/spv2vk.hpp>
#include <gct/type_traits/has_rebuild_chain.hpp>
#include <vulkan2json/PipelineLayoutCreateInfo.hpp>
#include <vulkan2json/PushConstantRange.hpp>
namespace gct {
  void to_json( nlohmann::json &root, const pipeline_layout_create_info_t &v ) {
    root = nlohmann::json::object();
    root[ "basic" ] = v.get_basic();
    LIBGCT_ARRAY_OF_TO_JSON_WRAPPED( basic, pSetLayouts, descriptor_set_layout )
    LIBGCT_ARRAY_OF_TO_JSON( basic, pPushConstantRanges, push_constant_range )
  }
  void from_json( const nlohmann::json &root, pipeline_layout_create_info_t &v ) {
    if( !root.is_object() ) throw incompatible_json( "The JSON is incompatible to pipeline_layout_create_info_t", __FILE__, __LINE__ );
    LIBGCT_EXTENSION_FROM_JSON( basic )
    LIBGCT_ARRAY_OF_FROM_JSON( basic, pPushConstantRanges, push_constant_range )
  }
  pipeline_layout_create_info_t &pipeline_layout_create_info_t::rebuild_chain() {
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
    LIBGCT_ARRAY_OF_REBUILD_CHAIN_WRAPPED( basic, SetLayoutCount, PSetLayouts, descriptor_set_layout )
    LIBGCT_ARRAY_OF_REBUILD_CHAIN( basic, PushConstantRangeCount, PPushConstantRanges, push_constant_range )
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
  pipeline_layout_create_info_t &pipeline_layout_create_info_t::add_push_constant_range(
    const std::shared_ptr< shader_module_t > &v
  ) {
    if( !v->get_props().has_reflection() )
      throw exception::invalid_argument( "Reflection is required to set shader directly to pipeline", __FILE__, __LINE__ );
    if( v->get_props().get_reflection()->push_constant_block_count == 1u ) {
      const auto existing = std::find_if(
        push_constant_range.begin(),
        push_constant_range.end(),
        [&]( const auto e ) {
          return
            e.offset == v->get_props().get_reflection()->push_constant_blocks[ 0 ].offset &&
            e.size == v->get_props().get_reflection()->push_constant_blocks[ 0 ].size;
        }
      );
      if( existing != push_constant_range.end() ) {
        existing->stageFlags |= spv2vk( v->get_props().get_reflection()->shader_stage );
        return *this;
      }
      else {
        return add_push_constant_range(
          vk::PushConstantRange()
            .setStageFlags( spv2vk( v->get_props().get_reflection()->shader_stage ) )
            .setOffset( v->get_props().get_reflection()->push_constant_blocks[ 0 ].offset )
            .setSize( v->get_props().get_reflection()->push_constant_blocks[ 0 ].size )
        );
      }
    }
    return *this;
  }
}

