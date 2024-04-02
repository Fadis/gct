#ifndef GCT_INSTANCE_LIST_HPP
#define GCT_INSTANCE_LIST_HPP
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

namespace gct {
  class command_buffer_recorder_t;
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
  void to_json( nlohmann::json& ) const;
private:
  instance_list_create_info props;
  std::shared_ptr< scene_graph_resource > resource;
  mutable std::vector< std::uint8_t > push_constant;
  std::vector< resource_pair > draw_list;
};

void to_json( nlohmann::json &dest, const instance_list &src );

}

#endif
