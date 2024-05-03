#ifndef GCT_COMPILED_AABB_SCENE_GRAPH_HPP
#define GCT_COMPILED_AABB_SCENE_GRAPH_HPP
#include <unordered_map>
#include <memory>
#include <filesystem>
#include <gct/setter.hpp>
#include <gct/graphics_pipeline.hpp>
#include <gct/graphics.hpp>
#include <gct/render_pass.hpp>
#include <gct/scene_graph.hpp>
#include <gct/vertex_buffer_pool.hpp>

namespace gct {
  class command_buffer_recorder_t;
}

namespace gct::scene_graph {

struct compiled_aabb_scene_graph_create_info {
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

struct compiled_aabb_primitive {
  LIBGCT_SETTER( prim )
  std::shared_ptr< primitive > prim;
  void operator()( command_buffer_recorder_t& ) const;
};

void to_json( nlohmann::json &dest, const compiled_aabb_primitive& );

class compiled_aabb_scene_graph {
public:
  compiled_aabb_scene_graph(
    const compiled_aabb_scene_graph_create_info &ci,
    const scene_graph &graph
  );
  const std::unordered_map< pool< std::shared_ptr< primitive > >::descriptor, compiled_aabb_primitive >&
  get_primitive() const {
    return prim;
  }
  void operator()( command_buffer_recorder_t& ) const;
private:
  void load_graph(
    const scene_graph &graph
  );
  compiled_aabb_primitive load_primitive(
    const std::shared_ptr< primitive > &p
  );
  std::unordered_map< shader_flag_t, std::shared_ptr< shader_module_t > > load_shader(
    const scene_graph &graph
  );
  void create_pipeline(
    const scene_graph &graph
  );
  void create_vertex_buffer(
    const scene_graph &graph
  );
  compiled_aabb_scene_graph_create_info props;
  std::shared_ptr< scene_graph_resource > resource;
  vertex_buffer_pool::vertex_buffer_descriptor vertex_buffer_desc;
  std::shared_ptr< graphics_pipeline_t > pipeline;
  std::shared_ptr< vertex_buffer_pool > vertex;
  std::unordered_map< pool< std::shared_ptr< primitive > >::descriptor, compiled_aabb_primitive > prim;
};

}

#endif

