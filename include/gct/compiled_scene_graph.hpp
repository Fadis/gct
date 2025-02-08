#ifndef GCT_COMPILED_SCENE_GRAPH_HPP
#define GCT_COMPILED_SCENE_GRAPH_HPP
#include <unordered_map>
#include <memory>
#include <filesystem>
#include <gct/setter.hpp>
#include <gct/graphics_pipeline.hpp>
#include <gct/graphics.hpp>
#include <gct/render_pass.hpp>
#include <gct/scene_graph.hpp>

namespace gct {
  class command_buffer_recorder_t;
}

namespace gct::scene_graph {

struct compiled_scene_graph_create_info {
  LIBGCT_SETTER( shader_mask )
  LIBGCT_SETTER( shader )
  LIBGCT_SETTER( render_pass )
  LIBGCT_SETTER( subpass )
  LIBGCT_SETTER( stencil_op )
  LIBGCT_SETTER( dynamic_cull_mode )
  LIBGCT_SETTER( resources )
  shader_flag_t shader_mask = shader_flag_t( 0 );
  std::filesystem::path shader;
  std::shared_ptr< render_pass_t > render_pass;
  std::uint32_t subpass = 0u;
  vk::StencilOpState stencil_op = vk::StencilOpState()
    .setFailOp( vk::StencilOp::eKeep )
    .setPassOp( vk::StencilOp::eKeep )
    .setCompareOp( vk::CompareOp::eAlways );
  bool dynamic_cull_mode = false;
  std::vector< named_resource > resources;
};

struct compiled_primitive {
  LIBGCT_SETTER( vertex )
  LIBGCT_SETTER( prim )
  LIBGCT_SETTER( pipeline )
  std::shared_ptr< vertex_buffer_pool > vertex;
  std::shared_ptr< primitive > prim;
  std::shared_ptr< graphics_pipeline_t > pipeline;
  void operator()( command_buffer_recorder_t& ) const;
};

void to_json( nlohmann::json &dest, const compiled_primitive& );

class compiled_scene_graph {
public:
  compiled_scene_graph(
    const compiled_scene_graph_create_info &ci,
    const scene_graph &graph
  );
  [[nodiscard]] const std::unordered_map< pool< std::shared_ptr< primitive > >::descriptor, compiled_primitive >&
  get_primitive() const {
    return prim;
  }
private:
  void load_graph(
    const scene_graph &graph
  );
  [[nodiscard]] compiled_primitive load_primitive(
    const scene_graph &graph,
    const std::shared_ptr< primitive > &p,
    const std::unordered_map< shader_flag_t, std::shared_ptr< shader_module_t > >&
  );
  [[nodiscard]] std::unordered_map< shader_flag_t, std::shared_ptr< shader_module_t > > load_shader(
    const scene_graph &graph
  );
  compiled_scene_graph_create_info props;
  std::shared_ptr< scene_graph_resource > resource;
  std::unordered_map< pool< std::shared_ptr< primitive > >::descriptor, compiled_primitive > prim;
};

}

#endif

