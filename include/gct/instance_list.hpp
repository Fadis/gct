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
#include <gct/compiled_aabb_scene_graph.hpp>
#include <gct/kdtree.hpp>
#include <gct/nohc_parameter.hpp>

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
  LIBGCT_SETTER( visible )
  LIBGCT_SETTER( frame_count )
  pool< std::shared_ptr< instance > >::descriptor inst;
  pool< std::shared_ptr< primitive > >::descriptor prim;
  bool visible = false;
  std::uint32_t frame_count = 0u;
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
  void setup_resource_pair_buffer(
    command_buffer_recorder_t &rec
  ) const;
  void operator()(
    command_buffer_recorder_t&,
    const compiled_scene_graph &compiled,
    bool conditional = false
  ) const;
  void operator()(
    command_buffer_recorder_t&,
    const compiled_aabb_scene_graph &compiled
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
  std::vector< resource_pair > get_last_visible_list() const;
private:
  instance_list_create_info props;
  std::shared_ptr< scene_graph_resource > resource;
  std::vector< resource_pair > draw_list;
  bool enable_conditional = false;
};

void to_json( nlohmann::json &dest, const instance_list &src );

template< typename T >
void append(
  const scene_graph_resource &resource,
  const std::vector< resource_pair > &l,
  T &kd
) {
  for( const auto &v: l ) {
    const auto inst = resource.inst.get( v.inst );
    if( inst ) {
      kd.insert( inst->initial_world_aabb, v );
    }
  }
}

template< typename T >
void sort_by_distance(
  T &dest,
  const glm::vec3 &eye_pos
) {
  std::vector< std::pair< float, typename T::value_type > > temp;
  temp.reserve( dest.size() );
  std::transform(
    dest.begin(),
    dest.end(),
    std::back_inserter( temp ),
    [&]( const auto &v ) {
      const auto distance = glm::distance( get_center( v.get_aabb() ), eye_pos );
      return std::make_pair( distance, v );
    }
  );
  std::sort( temp.begin(), temp.end(), []( const auto &l, const auto &r ) { return l.first < r.first; } );
  std::transform(
    temp.begin(),
    temp.end(),
    dest.begin(),
    [&]( const auto &v ) {
      return v.second;
    }
  );
}

}

#endif
