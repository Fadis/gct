#include <gct/descriptor_set_layout.hpp>
#include <gct/pipeline_layout_create_info.hpp>
#include <vulkan2json/PipelineLayoutCreateInfo.hpp>
namespace gct {
  void to_json( nlohmann::json &root, const pipeline_layout_create_info_t &v ) {
     root = nlohmann::json::object();
     root[ "basic" ] = v.get_basic();
  }
  void from_json( const nlohmann::json &root, pipeline_layout_create_info_t &v ) {
    if( !root.is_object() ) throw incompatible_json( "The JSON is incompatible to pipeline_layout_create_info_t", __FILE__, __LINE__ );
    LIBGCT_EXTENSION_FROM_JSON( basic )
  }
  pipeline_layout_create_info_t &pipeline_layout_create_info_t::rebuild_chain() {
    raw_descriptor_set_layout.clear();
    std::transform(
      descriptor_set_layout.begin(),
      descriptor_set_layout.end(),
      std::back_inserter( raw_descriptor_set_layout ),
      []( const auto &v ) { return **v; }
    );
    basic
      .setSetLayoutCount( raw_descriptor_set_layout.size() )
      .setPSetLayouts( raw_descriptor_set_layout.data() )
      .setPushConstantRangeCount( push_constant_range.size() )
      .setPPushConstantRanges( push_constant_range.data() );
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
  pipeline_layout_create_info_t &pipeline_layout_create_info_t::add_descriptor_set_layout( const std::shared_ptr< descriptor_set_layout_t > &v ) {
    descriptor_set_layout.push_back( v );
    chained = false;
    return *this;
  }
  pipeline_layout_create_info_t &pipeline_layout_create_info_t::clear_descriptor_set_layout() {
    descriptor_set_layout.clear();
    chained = false;
    return *this;
  }
  pipeline_layout_create_info_t &pipeline_layout_create_info_t::add_push_constant_range( const vk::PushConstantRange &v ) {
    push_constant_range.push_back( v );
    chained = false;
    return *this;
  }
  pipeline_layout_create_info_t &pipeline_layout_create_info_t::clear_push_constant_range() {
    push_constant_range.clear();
    chained = false;
    return *this;
  }
}

