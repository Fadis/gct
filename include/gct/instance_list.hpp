#ifndef GCT_INSTANCE_LIST_HPP
#define GCT_INSTANCE_LIST_HPP
#include <optional>
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
#include <gct/property.hpp>
#include <gct/syncable.hpp>
#include <gct/render_pass_begin_info.hpp>

namespace gct {
  class command_buffer_recorder_t;
  class query_pool_t;
}

namespace gct::scene_graph {

struct instance_list_create_info {
  LIBGCT_SETTER(parallel_mode)
  LIBGCT_SETTER(parallel_mode2)
  LIBGCT_SETTER(parallel_mode3)
  LIBGCT_SETTER(all_lods)
  LIBGCT_SETTER(unique_prim_list)
  bool parallel_mode = false;
  bool parallel_mode2 = false;
  bool parallel_mode3 = false;
  bool all_lods = false;
  bool unique_prim_list = false;
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

class instance_list :
  public property< instance_list_create_info > {
public:
  instance_list(
    const instance_list_create_info &ci,
    const scene_graph &graph
  );
  void update_device_side_list();
  void setup_resource_pair_buffer(
    command_buffer_recorder_t &rec
  ) const;
  void setup_resource_pair_buffer(
    bool use_meshlet,
    const compute&
  ) const;
  void setup_resource_pair_buffer(
    bool use_meshlet,
    const std::vector< resource_pair >::const_iterator &i,
    const compute&
  ) const;
  void setup_resource_pair_buffer(
    bool use_meshlet,
    const graphics&
  ) const;
  void setup_resource_pair_buffer(
    bool use_meshlet,
    const std::vector< resource_pair >::const_iterator &i,
    const graphics&
  ) const;
  void operator()(
    command_buffer_recorder_t&,
    const compiled_scene_graph &compiled,
    bool conditional = false
  ) const;
  void operator()(
    command_buffer_recorder_t&,
    const compiled_scene_graph &compiled,
    const std::optional< render_pass_begin_info_t > &rp,
    const syncable &barrier_target,
    bool conditional = false
  ) const;
  void operator()(
    command_buffer_recorder_t&,
    const compiled_aabb_scene_graph &compiled
  ) const;
  void operator()(
    command_buffer_recorder_t&,
    const graphics &compiled
  ) const;
  void operator()(
    command_buffer_recorder_t&,
    const graphics &compiled,
    const std::vector< resource_pair >::const_iterator &i
  ) const;
  void for_each_unique_vertex(
    command_buffer_recorder_t &rec,
    const compute &compiled,
    const std::vector< resource_pair >::const_iterator &i
  ) const;
  void to_json( nlohmann::json& ) const;
  [[nodiscard]] std::vector< resource_pair > &get_draw_list() {
    return draw_list;
  }
  [[nodiscard]] const std::vector< resource_pair > &get_draw_list() const {
    return draw_list;
  }
  [[nodiscard]] const std::shared_ptr< scene_graph_resource > &get_resource() const {
    return resource;
  }
  [[nodiscard]] std::vector< resource_pair > get_last_visible_list() const;
  [[nodiscard]] std::uint32_t get_mesh_count() const {
    return draw_list.size();
  }
  [[nodiscard]] std::uint32_t get_max_primitive_count() const {
    return max_primitive_count;
  }
  [[nodiscard]] const std::optional< spv_member_pointer > &get_push_constant_member_pointer() const {
    return resource->push_constant_mp;
  }
private:
  std::shared_ptr< scene_graph_resource > resource;
  std::vector< resource_pair > draw_list;
  bool enable_conditional = false;
  buffer_pool::buffer_descriptor device_side_list;
  buffer_pool::buffer_descriptor meshlet_list;
  std::uint32_t max_primitive_count = 0u;
  std::uint32_t meshlet_list_size;
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
