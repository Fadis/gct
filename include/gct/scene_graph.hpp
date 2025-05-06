#ifndef GCT_SCENE_GRAPH_HPP
#define GCT_SCENE_GRAPH_HPP
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>
#include <gct/shader_flag.hpp>
#include <gct/graphics_pipeline_create_info.hpp>
#include <gct/raw_resource_pair_type.hpp>
#include <gct/scene_graph_create_info.hpp>
#include <gct/scene_graph_resource.hpp>
#include <gct/matrix_pool.hpp>
#include <gct/light_pool.hpp>
#include <gct/aabb_pool.hpp>
#include <gct/scene_graph_primitive.hpp>
#include <gct/scene_graph_instance.hpp>

namespace gct::scene_graph {

struct node {
  node(
    const std::shared_ptr< scene_graph_create_info > &ci,
    const std::shared_ptr< scene_graph_resource > &res,
    const glm::mat4 &local_matrix
  ) : props( ci ), resource( res ) {
    matrix = resource->matrix->allocate( local_matrix );
    initial_world_matrix = local_matrix;
  }
  node(
    const std::shared_ptr< scene_graph_create_info > &ci,
    const std::shared_ptr< scene_graph_resource > &res,
    const matrix_pool::matrix_descriptor &parent,
    const glm::mat4 &parent_matrix,
    const glm::mat4 &local_matrix
  ) : props( ci ), resource( res ) {
    matrix = resource->matrix->allocate( parent, local_matrix );
    initial_world_matrix = parent_matrix * local_matrix;
  }
  std::shared_ptr< node > add_child( const glm::mat4 &local_matrix ) {
    child.push_back( std::make_shared< node >(
      props,
      resource,
      matrix,
      initial_world_matrix,
      local_matrix
    ) );
    return child.back();
  }
  std::shared_ptr< node > add_child_without_link( const glm::mat4 &local_matrix ) {
    auto c = std::make_shared< node >(
      props,
      resource,
      matrix,
      initial_world_matrix,
      local_matrix
    );
    return c;
  }
  void update_matrix( const glm::mat4 &m ) {
    resource->matrix->set( matrix, m );
    for( const auto &v: child ) {
      v->touch();
    }
  }
  void touch() {
    for( auto &v: child ) {
      v->touch();
    }
    for( auto &v: inst ) {
      const auto &i = resource->inst.get( v );
      resource->aabb->touch( i->descriptor.aabb );
    }
  }
  [[nodiscard]] std::optional< aabb4 > get_initial_world_aabb() const;
  void add_lod_node(
    const std::shared_ptr< node > &n
  ) {
    lod.push_back( n );
  }
  void to_json( nlohmann::json& ) const;
  std::string name;
  std::vector< std::shared_ptr< node > > child;
  std::vector< pool< std::shared_ptr< instance > >::descriptor > inst;
  std::vector< light_pool::light_descriptor > light;
  glm::mat4 initial_world_matrix;
  aabb4 prim_aabb;
  matrix_pool::matrix_descriptor matrix;
  std::shared_ptr< scene_graph_create_info > props;
  std::shared_ptr< scene_graph_resource > resource;
  std::vector< std::shared_ptr< node > > lod;
};

void to_json( nlohmann::json&, const node& );

class scene_graph {
public:
  scene_graph(
    const scene_graph_create_info &ci
  );
  [[nodiscard]] const scene_graph_create_info &get_props() const {
    return *props;
  }
  [[nodiscard]] std::shared_ptr< node > get_root_node() const {
    return root_node;
  }
  [[nodiscard]] const std::shared_ptr< vertex_buffer_pool > &get_vertex() const {
    return resource->vertex;
  }
  [[nodiscard]] const std::shared_ptr< image_pool > &get_image() const {
    return resource->image;
  }
  [[nodiscard]] const std::shared_ptr< texture_pool > &get_texture() const {
    return resource->texture;
  }
  [[nodiscard]] const std::shared_ptr< sampler_pool > &get_sampler() const {
    return resource->sampler;
  }
  [[nodiscard]] const std::shared_ptr< scene_graph_resource > &get_resource() const {
    return resource;
  }
  void to_json( nlohmann::json& ) const;
  void operator()( command_buffer_recorder_t& ) const;
  void rotate_visibility( command_buffer_recorder_t &rec ) const;
  void clear_visibility( command_buffer_recorder_t &rec ) const;
  void fill_visibility( command_buffer_recorder_t &rec, std::uint32_t ) const;
private:
  std::shared_ptr< scene_graph_create_info > props;
  std::shared_ptr< node > root_node;
  std::shared_ptr< scene_graph_resource > resource;
  mutable bool init_visibility = true;
  bool use_conditional = false;
};

void to_json( nlohmann::json &, const scene_graph& );

}

#endif

