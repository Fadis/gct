#ifndef GCT_SCENE_GRAPH_HPP
#define GCT_SCENE_GRAPH_HPP

#include <gct/setter.hpp>
#include <gct/matrix_pool.hpp>
#include <gct/aabb_pool.hpp>

namespace gct {

struct scene_graph_create_info {
  LIBGCT_SETTER( matrix )
  LIBGCT_SETTER( aabb )
  matrix_pool matrix;
  aabb_pool aabb;
};

struct primitive_create_info {
};

class primitive {
public:
  primitive(
    const std::shared_ptr< scene_graph_create_info > &sci,
    const matrix_pool::matrix_descriptor matrix,
    const primitive_create_info &pci
  );
  aabb_pool::aabb_descriptor matrix;
};

class node {
public:
  node(
    const std::shared_ptr< scene_graph_create_info > &ci,
    const glm::mat4 &local_matrix
  ) : props( ci ) {
    matrix = matrix_pool->allocate( local_matrix );
  }
  node(
    const std::shared_ptr< matrix_pool > &mp,
    const matrix_pool::matrix_descriptor &parent,
    const glm::mat4 &local_matrix
  ) : matrix_pool( mp ) {
    matrix = matrix_pool->allocate( parent, local_matrix );
  }
  const std::vector< std::shared_ptr< node > > &get_child() const { return child; }
  const std::vector< std::shared_ptr< primitive > > &get_primitive() const { return prim; }
  std::vector< std::shared_ptr< node > > get_child() { return child; }
  std::vector< std::shared_ptr< primitive > > get_primitive() { return prim; }
  std::shared_ptr< node > add_child( const glm::mat4 &local_matrix ) {
    child.push_back( std::make_shared< node >(
      props,
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
  std::string name;
  std::vector< std::shared_ptr< node > > child;
  std::vector< std::shared_ptr< primitive > > prim;
  std::shared_ptr< matrix_pool > matrix_pool;
  matrix_pool::matrix_descriptor matrix;
};

class scene_graph {
public:
  scene_graph(
    const scene_graph_create_info &ci
  ) : props( new scene_graph_create_info( ci ) ) {
    root_node.reset( new node(
      props,
      glm::mat4(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
      )
    ) );
  }
  const scene_graph_create_info &get_props() const {
    return *props;
  }
  std::shared_ptr< node > get_root_node() const {
    return root_node;
  }
private:
  std::shared_ptr< scene_graph_create_info > props;
  std::shared_ptr< node > root_node;
};

}

#endif

