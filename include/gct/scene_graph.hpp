#ifndef GCT_SCENE_GRAPH_HPP
#define GCT_SCENE_GRAPH_HPP

#include <gct/setter.hpp>
#include <gct/matrix_pool.hpp>
#include <gct/aabb_pool.hpp>
#include <gct/matrix_pool.hpp>
#include <gct/aabb_pool.hpp>
#include <gct/texture_pool.hpp>
#include <gct/sampler_pool.hpp>
#include <gct/buffer_pool.hpp>

namespace gct {

struct scene_graph_create_info {
  LIBGCT_SETTER( allocator )
  LIBGCT_SETTER( descriptor_pool )
  LIBGCT_SETTER( pipeline_cache )
  LIBGCT_SETTER( master_shader )
  LIBGCT_SETTER( matrix )
  LIBGCT_SETTER( aabb )
  LIBGCT_SETTER( texture )
  LIBGCT_SETTER( sampler )
  LIBGCT_SETTER( resource_index )
  LIBGCT_SETTER( visibility )
  std::shared_ptr< allocator_t > allocator;
  std::shared_ptr< descriptor_pool_t > descriptor_pool;
  std::shared_ptr< pipeline_cache_t > pipeline_cache;
  std::filesystem::path master_shader;
  matrix_pool_create_info matrix;
  aabb_pool_create_info aabb;
  texture_pool_create_info texture;
  sampler_pool_create_info sampler;
  buffer_pool_create_info resource_index;
  buffer_pool_create_info visibility;
};

struct scene_graph_resource {
  LIBGCT_SETTER( matrix )
  LIBGCT_SETTER( aabb )
  LIBGCT_SETTER( texture )
  LIBGCT_SETTER( sampler )
  LIBGCT_SETTER( resource_index )
  LIBGCT_SETTER( visibility )
  LIBGCT_SETTER( descriptor_set_layout )
  LIBGCT_SETTER( descriptor_set )
  std::shared_ptr< matrix_pool > matrix;
  std::shared_ptr< aabb_pool > aabb;
  std::shared_ptr< texture_pool > texture;
  std::shared_ptr< sampler_pool > sampler;
  std::shared_ptr< buffer_pool > resource_index;
  std::shared_ptr< buffer_pool > visibility;
  std::shared_ptr< descriptor_set_layout_t > descriptor_set_layout;
  std::shared_ptr< descriptor_set_t > descriptor_set;
};


struct primitive_create_info {
};

class primitive {
public:
  primitive(
    const std::shared_ptr< scene_graph_create_info > &sci,
    const std::shared_ptr< scene_graph_resource > &res,
    const matrix_pool::matrix_descriptor &matrix,
    const primitive_create_info &pci
  );
private:
  aabb_pool::aabb_descriptor matrix;
};

class node {
public:
  node(
    const std::shared_ptr< scene_graph_create_info > &ci,
    const std::shared_ptr< scene_graph_resource > &res,
    const glm::mat4 &local_matrix
  ) : props( ci ), resource( res ) {
    matrix = resource->matrix->allocate( local_matrix );
  }
  node(
    const std::shared_ptr< scene_graph_create_info > &ci,
    const std::shared_ptr< scene_graph_resource > &res,
    const matrix_pool::matrix_descriptor &parent,
    const glm::mat4 &local_matrix
  ) : props( ci ), resource( res ) {
    matrix = resource->matrix->allocate( parent, local_matrix );
  }
  const std::vector< std::shared_ptr< node > > &get_child() const { return child; }
  const std::vector< std::shared_ptr< primitive > > &get_primitive() const { return prim; }
  std::vector< std::shared_ptr< node > > get_child() { return child; }
  std::vector< std::shared_ptr< primitive > > get_primitive() { return prim; }
  std::shared_ptr< node > add_child( const glm::mat4 &local_matrix ) {
    child.push_back( std::make_shared< node >(
      props,
      resource,
      matrix,
      local_matrix
    ) );
    return child.back();
  }
  std::shared_ptr< primitive > add_primitive(
    const primitive_create_info &ci
  ) {
    prim.push_back( std::make_shared< primitive >(
      props,
      resource,
      matrix,
      ci
    ) );
    return prim.back();
  }
  node &set_name( const std::string n ) {
    name = n;
    return *this;
  }
  const std::string get_name() const {
    return name;
  }
private:
  std::shared_ptr< scene_graph_create_info > props;
  std::shared_ptr< scene_graph_resource > resource;
  std::string name;
  std::vector< std::shared_ptr< node > > child;
  std::vector< std::shared_ptr< primitive > > prim;
  matrix_pool::matrix_descriptor matrix;
};

class scene_graph {
public:
  scene_graph(
    const scene_graph_create_info &ci
  );
  const scene_graph_create_info &get_props() const {
    return *props;
  }
  std::shared_ptr< node > get_root_node() const {
    return root_node;
  }
private:
  std::shared_ptr< scene_graph_create_info > props;
  std::shared_ptr< node > root_node;
  std::shared_ptr< scene_graph_resource > resource;
};

}

#endif

