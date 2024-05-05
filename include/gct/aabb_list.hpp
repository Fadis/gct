#ifndef GCT_AABB_LIST_HPP
#define GCT_AABB_LIST_HPP
#include <unordered_map>
#include <memory>
#include <filesystem>
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>
#include <gct/graphics_pipeline.hpp>
#include <gct/graphics.hpp>
#include <gct/render_pass.hpp>
#include <gct/scene_graph.hpp>
#include <gct/compiled_scene_graph.hpp>
#include <gct/compiled_aabb_scene_graph.hpp>
#include <gct/kdtree.hpp>

namespace gct {
  class command_buffer_recorder_t;
  class query_pool_t;
}

namespace gct::scene_graph {

struct instance_list_create_info {
};

struct resource_pair {
  LIBGCT_SETTER( inst )
  LIBGCT_SETTER( prim )
  pool< std::shared_ptr< instance > >::descriptor inst;
  pool< std::shared_ptr< primitive > >::descriptor prim;
};

bool operator==( const resource_pair &l, const resource_pair &r );
bool operator!=( const resource_pair &l, const resource_pair &r );

void to_json( nlohmann::json &dest, const resource_pair &src );

class instance_list {
public:
  instance_list(
    const instance_list_create_info &ci,
    const scene_graph &graph
  );
  void operator()(
    command_buffer_recorder_t&,
    const compiled_scene_graph &compiled
  ) const;
  void operator()(
    command_buffer_recorder_t&,
    const compiled_aabb_scene_graph &compiled,
    const std::shared_ptr< query_pool_t > &query_pool
  ) const;
  void to_json( nlohmann::json& ) const;
  std::vector< resource_pair > &get_draw_list() {
    return draw_list;
  }
  const std::vector< resource_pair > &get_draw_list() const {
    return draw_list;
  }
  const std::shared_ptr< scene_graph_resource > &get_resource() const {
    return resource;
  }
  const instance_list_create_info &get_props() const {
    return props;
  }
private:
  instance_list_create_info props;
  std::shared_ptr< scene_graph_resource > resource;
  std::vector< resource_pair > draw_list;
};

void append(
  const scene_graph_resource &resource,
  const std::vector< resource_pair > &l,
  kdtree< resource_pair > &kd
);

void to_json( nlohmann::json &dest, const instance_list &src );

}

#endif
