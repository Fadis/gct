#include <nlohmann/json.hpp>
#include <gct/shader_module.hpp>
#include <gct/pipeline_depth_stencil_state_create_info.hpp>
#include <vulkan2json/PipelineDepthStencilStateCreateInfo.hpp>

namespace gct {
  void to_json( nlohmann::json &root, const pipeline_depth_stencil_state_create_info_t &v ) {
     root = nlohmann::json::object();
     root[ "basic" ] = v.get_basic();
  }
  void from_json( const nlohmann::json &root, pipeline_depth_stencil_state_create_info_t &v ) {
    if( !root.is_object() ) throw incompatible_json( "The JSON is incompatible to pipeline_depth_stencil_state_create_info_t", __FILE__, __LINE__ );
    LIBGCT_EXTENSION_FROM_JSON( basic )
  }
  pipeline_depth_stencil_state_create_info_t &pipeline_depth_stencil_state_create_info_t::rebuild_chain() {
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
  pipeline_depth_stencil_state_create_info_t::pipeline_depth_stencil_state_create_info_t() {
    static const auto stencil_op = vk::StencilOpState()
      .setCompareOp( vk::CompareOp::eAlways )
      .setFailOp( vk::StencilOp::eKeep )
      .setPassOp( vk::StencilOp::eKeep );
    set_basic(
      vk::PipelineDepthStencilStateCreateInfo()
        .setDepthTestEnable( true )
        .setDepthWriteEnable( true )
        .setDepthCompareOp( vk::CompareOp::eLessOrEqual )
        .setDepthBoundsTestEnable( false )
        .setStencilTestEnable( false )
        .setFront( stencil_op )
        .setBack( stencil_op )
    );
  }
  pipeline_depth_stencil_state_create_info_t &pipeline_depth_stencil_state_create_info_t::disable_depth() {
    basic
      .setDepthTestEnable( false )
      .setDepthWriteEnable( false )
      .setDepthBoundsTestEnable( false );
    return *this;
  }

}

