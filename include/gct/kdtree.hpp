#ifndef GCT_KDTREE_HPP
#define GCT_KDTREE_HPP
#include <memory>
#include <algorithm>
#include <functional>
#include <unordered_set>
#include <gct/aabb.hpp>
#include <gct/matrix.hpp>
#include <gct/projection_matrix.hpp>

namespace gct {

template< typename T >
class kdtree {
  enum class axis_type {
    x_begin,
    y_begin,
    z_begin,
    x_end,
    y_end,
    z_end
  };
public:
  struct leaf_type {
    aabb range;
    std::vector< T > value;
  };
  struct node_type {
    axis_type axis = axis_type::x_begin;
    float threshold = 0.0;
    aabb range;
    std::shared_ptr< node_type > less_than;
    std::shared_ptr< node_type > greater_equal;
    std::shared_ptr< leaf_type > leaf;
  };
  kdtree() {}
  kdtree( const std::shared_ptr< node_type > &node ) : root( node ) {}
  void insert( const aabb &r, const T &v ) {
    insert( root, axis_type::x_begin, r, v );
  }
  void insert( const aabb &r, const std::vector< T > &v ) {
    insert( root, axis_type::x_begin, r, v );
  }
  std::vector< std::shared_ptr< leaf_type > > find( const aabb &r ) const {
    std::vector< std::shared_ptr< leaf_type > > temp;
    find( root, r, temp );
    return temp;
  }
  void find(
    const aabb &r,
    std::vector< std::shared_ptr< leaf_type > > &dest
  ) const {
    find( root, r, dest );
  }
  std::vector< std::shared_ptr< leaf_type > > find( const std::vector< aabb > &r ) const {
    std::vector< std::shared_ptr< leaf_type > > temp;
    for( const auto &v: r ) {
      find( root, v, temp );
    }
    std::sort( temp.begin(), temp.end() );
    temp.erase( std::unique( temp.begin(), temp.end() ), temp.end() );
    temp.shrink_to_fit();
    return temp;
  }
  void find(
    const std::vector< aabb > &r,
    std::vector< std::shared_ptr< leaf_type > > &dest
  ) const {
    for( const auto &v: r ) {
      find( root, v, dest );
    }
    std::sort( dest.begin(), dest.end() );
    dest.erase( std::unique( dest.begin(), dest.end() ), dest.end() );
  }
  std::string to_string() const {
    return to_string( root );
  }
  std::vector< std::shared_ptr< leaf_type > > get() const {
    std::vector< std::shared_ptr< leaf_type > > temp;
    get( root, temp );
    return temp;
  }
  void clear() {
    root.reset();
  }
  bool empty() const {
    return !root;
  }
  std::pair< kdtree, kdtree > subtree() const {
    if( !root ) {
      return std::pair< kdtree, kdtree >{ kdtree(), kdtree() };
    }
    const auto branch = get_branch( root );
    if( !branch ) {
      return std::pair< kdtree, kdtree >{ kdtree(), kdtree() };
    }
    if( branch->leaf ) {
      return std::pair< kdtree, kdtree >{ kdtree( branch->leaf ), kdtree() };
    }
    if( branch->less_than && branch->greater_equal ) {
      return std::pair< kdtree, kdtree >{ kdtree( branch->less_than ), kdtree( branch->greater_equal ) };
    }
    return std::pair< kdtree, kdtree >{ kdtree(), kdtree() };
  }
private:
  const std::shared_ptr< node_type > &get_branch( const std::shared_ptr< node_type > &node ) const {
    if( !node ) {
      return node;
    }
    if( node.leaf ) {
      return node;
    }
    if( node.less_than && node.greater_equal ) {
      return node;
    }
    if( node.less_than ) {
      return get_branch( node.less_than );
    }
    if( node.greater_equal ) {
      return get_branch( node.greater_equal );
    }
    return node;
  }
  std::string to_string( const std::shared_ptr< node_type > &node ) const {
    if( !node ) return "null";
    else if( node->leaf ) {
      return "{\"range\":\"" + gct::to_string( node->leaf->range ) + "\"}";
    }
    else {
      std::string temp = "{\"axis\":";
      temp += std::to_string( int( node->axis ) );
      temp += ",\"threshold\":";
      temp += std::to_string( node->threshold );
      temp += ",\"less_than\":";
      temp += to_string( node->less_than );
      temp += ",\"greater_equal\":";
      temp += to_string( node->greater_equal );
      temp += "}";
      return temp;
    }
  }
  axis_type next( axis_type axis ) {
    if( axis == axis_type::z_end ) {
      return axis_type::x_begin;
    }
    else {
      return axis_type( int( axis ) + 1 );
    }
  }
  static float get_axis_value( axis_type axis, const aabb &r ) {
    if( axis == axis_type::x_begin ) {
      return r.min.x;
    }
    else if( axis == axis_type::y_begin ) {
      return r.min.y;
    }
    else if( axis == axis_type::z_begin ) {
      return r.min.z;
    }
    else if( axis == axis_type::x_end ) {
      return r.max.x;
    }
    else if( axis == axis_type::y_end ) {
      return r.max.y;
    }
    else if( axis == axis_type::z_end ) {
      return r.max.z;
    }
    else {
      throw -1;
    }
  }
  void insert( std::shared_ptr< node_type > &node, axis_type axis, const aabb &r, const T &v ) {
    if( !node ) {
      auto new_leaf = std::make_shared< leaf_type >();
      new_leaf->range = r;
      new_leaf->value = { v };
      auto new_node = std::make_shared< node_type >();
      new_node->axis = axis;
      new_node->range = r;
      new_node->leaf = new_leaf;
      node = new_node;
    }
    else if( node->leaf ) {
      const auto existing_axis_value = get_axis_value( node->axis, node->leaf->range );
      const auto new_axis_value = get_axis_value( node->axis, r );
      if( existing_axis_value == new_axis_value ) {
        if( node->leaf->range == r ) {
          node->leaf->value.push_back( v );
        }
        else {
          const float threshold = existing_axis_value;
          auto new_node = std::make_shared< node_type >();
          new_node->axis = axis;
          new_node->threshold = threshold;
          new_node->range = node->range | r;
          new_node->greater_equal = node;
          new_node->greater_equal->axis = next( axis );
          insert( new_node->greater_equal, next( axis ), r, v );
          node = new_node;
        }
      }
      else {
        const float threshold = ( existing_axis_value + new_axis_value ) / 2.0f;
        auto new_node = std::make_shared< node_type >();
        new_node->axis = axis;
        new_node->threshold = threshold;
        new_node->range = node->range | r;
        if( existing_axis_value < threshold ) {
          new_node->less_than = node;
          new_node->less_than->axis = next( new_node->less_than->axis );
          insert( new_node->greater_equal, next( axis ), r, v );
        }
        else {
          insert( new_node->less_than, next( axis ), r, v );
          new_node->greater_equal = node;
          new_node->greater_equal->axis = next( new_node->greater_equal->axis );
        }
        node = new_node;
      }
    }
    else {
      const auto axis_value = get_axis_value( node->axis, r );
      node->range = node->range | r;
      if( axis_value < node->threshold ) {
        insert( node->less_than, next( axis ), r, v );
      }
      else {
        insert( node->greater_equal, next( axis ), r, v );
      }
    }
  }
  void insert( std::shared_ptr< node_type > &node, axis_type axis, const aabb &r, const std::vector< T > &v ) {
    if( !node ) {
      auto new_leaf = std::make_shared< leaf_type >();
      new_leaf->range = r;
      new_leaf->value = v;
      auto new_node = std::make_shared< node_type >();
      new_node->axis = axis;
      new_node->range = r;
      new_node->leaf = new_leaf;
      node = new_node;
    }
    else if( node->leaf ) {
      const auto existing_axis_value = get_axis_value( node->axis, node->leaf->range );
      const auto new_axis_value = get_axis_value( node->axis, r );
      if( existing_axis_value == new_axis_value ) {
        if( node->leaf->range == r ) {
          node->leaf->value.insert( node->leaf->value.end(), v.begin(), v.end() );
        }
        else {
          const float threshold = existing_axis_value;
          auto new_node = std::make_shared< node_type >();
          new_node->axis = axis;
          new_node->threshold = threshold;
          new_node->range = node->range | r;
          new_node->greater_equal = node;
          new_node->greater_equal->axis = next( axis );
          insert( new_node->greater_equal, next( axis ), r, v );
          node = new_node;
        }
      }
      else {
        const float threshold = ( existing_axis_value + new_axis_value ) / 2.0f;
        auto new_node = std::make_shared< node_type >();
        new_node->axis = axis;
        new_node->threshold = threshold;
        new_node->range = node->range | r;
        if( existing_axis_value < threshold ) {
          new_node->less_than = node;
          new_node->less_than->axis = next( new_node->less_than->axis );
          insert( new_node->greater_equal, next( axis ), r, v );
        }
        else {
          insert( new_node->less_than, next( axis ), r, v );
          new_node->greater_equal = node;
          new_node->greater_equal->axis = next( new_node->greater_equal->axis );
        }
        node = new_node;
      }
    }
    else {
      const auto axis_value = get_axis_value( node->axis, r );
      node->range = node->range | r;
      if( axis_value < node->threshold ) {
        insert( node->less_than, next( axis ), r, v );
      }
      else {
        insert( node->greater_equal, next( axis ), r, v );
      }
    }
  }
  static void find(
    const std::shared_ptr< node_type > &node,
    const aabb &r,
    std::vector< std::shared_ptr< leaf_type > > &dest
  ) {
    if( !node ) {
      return;
    }
    else if( node->leaf ) {
      if( node->leaf->range && r ) {
        dest.push_back( node->leaf );
        return;
      }
      else {
        return;
      }
    }
    else {
      if( node->axis == axis_type::x_begin ) {
        find( node->less_than, r, dest );
        if( r.max.x >= node->threshold ) {
          find( node->greater_equal, r, dest );
        }
        return;
      }
      else if( node->axis == axis_type::y_begin ) {
        find( node->less_than, r, dest );
        if( r.max.y >= node->threshold ) {
          find( node->greater_equal, r, dest );
        }
        return;
      }
      else if( node->axis == axis_type::z_begin ) {
        find( node->less_than, r, dest );
        if( r.max.z >= node->threshold ) {
          find( node->greater_equal, r, dest );
        }
        return;
      }
      else if( node->axis == axis_type::x_end ) {
        find( node->greater_equal, r, dest );
        if( r.min.x < node->threshold ) {
          find( node->less_than, r, dest );
        }
        return;
      }
      else if( node->axis == axis_type::y_end ) {
        find( node->greater_equal, r, dest );
        if( r.min.y < node->threshold ) {
          find( node->less_than, r, dest );
        }
        return;
      }
      else if( node->axis == axis_type::z_end ) {
        find( node->greater_equal, r, dest );
        if( r.min.z < node->threshold ) {
          find( node->less_than, r, dest );
        }
        return;
      }
      else {
        std::abort();
      }
    }
    std::abort();
  }
  static bool judge(
    const std::shared_ptr< node_type > &node,
    std::vector< std::shared_ptr< node_type > > &dest,
    const std::function< bool( const node_type& ) > &cont
  ) {
    if( !node ) {
      return false;
    }
    if( node->leaf ) {
      dest.push_back( node );
      return false;
    }
    if( cont( *node ) ) {
      return true;
    }
    else {
      dest.push_back( node );
      return false;
    }
  }
  static void find(
    const std::shared_ptr< node_type > &node,
    const aabb &r,
    std::vector< std::shared_ptr< node_type > > &dest,
    const std::function< bool( const node_type& ) > &cont
  ) {
    if( !node ) {
      return;
    }
    else if( node->leaf ) {
      if( node->leaf->range && r ) {
        dest.push_back( node );
        return;
      }
      else {
        return;
      }
    }
    else {
      if( node->axis == axis_type::x_begin ) {
        if( r.max.x >= node->threshold ) {
          const bool cont_lt = judge( node->less_than, dest, cont );
          const bool cont_ge = judge( node->greater_equal, dest, cont );
          if( cont_lt ) {
            find( node->less_than, r, dest, cont );
          }
          if( cont_ge ) {
            find( node->greater_equal, r, dest, cont );
          }
        }
        else {
          find( node->less_than, r, dest, cont );
        }
        return;
      }
      else if( node->axis == axis_type::y_begin ) {
        if( r.max.y >= node->threshold ) {
          const bool cont_lt = judge( node->less_than, dest, cont );
          const bool cont_ge = judge( node->greater_equal, dest, cont );
          if( cont_lt ) {
            find( node->less_than, r, dest, cont );
          }
          if( cont_ge ) {
            find( node->greater_equal, r, dest, cont );
          }
        }
        else {
          find( node->less_than, r, dest, cont );
        }
        return;
      }
      else if( node->axis == axis_type::z_begin ) {
        if( r.max.z >= node->threshold ) {
          const bool cont_lt = judge( node->less_than, dest, cont );
          const bool cont_ge = judge( node->greater_equal, dest, cont );
          if( cont_lt ) {
            find( node->less_than, r, dest, cont );
          }
          if( cont_ge ) {
            find( node->greater_equal, r, dest, cont );
          }
        }
        else {
          find( node->less_than, r, dest, cont );
        }
        return;
      }
      else if( node->axis == axis_type::x_end ) {
        if( r.min.x < node->threshold ) {
          const bool cont_lt = judge( node->less_than, dest, cont );
          const bool cont_ge = judge( node->greater_equal, dest, cont );
          if( cont_lt ) {
            find( node->greater_equal, r, dest, cont );
          }
          if( cont_ge ) {
            find( node->less_than, r, dest, cont );
          }
        }
        else {
          find( node->greater_equal, r, dest, cont );
        }
        return;
      }
      else if( node->axis == axis_type::y_end ) {
        if( r.min.y < node->threshold ) {
          const bool cont_lt = judge( node->less_than, dest, cont );
          const bool cont_ge = judge( node->greater_equal, dest, cont );
          if( cont_lt ) {
            find( node->greater_equal, r, dest, cont );
          }
          if( cont_ge ) {
            find( node->less_than, r, dest, cont );
          }
        }
        else {
          find( node->greater_equal, r, dest, cont );
        }
        return;
      }
      else if( node->axis == axis_type::z_end ) {
        if( r.min.z < node->threshold ) {
          const bool cont_lt = judge( node->less_than, dest, cont );
          const bool cont_ge = judge( node->greater_equal, dest, cont );
          if( cont_lt ) {
            find( node->greater_equal, r, dest );
          }
          if( cont_ge ) {
            find( node->less_than, r, dest );
          }
        }
        else {
          find( node->greater_equal, r, dest );
        }
        return;
      }
      else {
        return;
      }
    }
    return;  
  }
  static void get(
    const std::shared_ptr< node_type > &node,
    std::vector< std::shared_ptr< leaf_type > > &dest
  ) {
    if( !node ) {
    }
    else if( node->leaf ) {
      dest.push_back( node->leaf );
    }
    else {
      if( node->less_than ) {
        get( node->less_than, dest );
      }
      if( node->greater_equal ) {
        get( node->greater_equal, dest );
      }
    }
  }
  std::shared_ptr< node_type > root;
};

template< typename T >
void frustum_culling(
  const glm::mat4 &projection,
  const glm::mat4 &camera,
  const kdtree< T > &src,
  kdtree< T > &dest,
  std::uint32_t m
) {
  const auto proj_cam = projection * camera;  
  for( const auto &view: get_clipping_aabb( projection, camera, m ) ) {
    for( const auto box: src.find( view ) ) {
      if( is_visible( proj_cam, box->range ) ) {
        dest.insert( box->range, box->value );
      }
    }
  }
}
template< typename T >
void frustum_culling(
  const glm::mat4 &projection,
  const glm::mat4 &camera,
  const glm::vec3 &camera_pos,
  const kdtree< T > &src,
  std::vector< T > &dest,
  std::uint32_t m
) {
  const auto proj_cam = projection * camera;
  std::unordered_set< typename kdtree< T >::leaf_type* > known;
  for( const auto &view: get_clipping_aabb( projection, camera, m ) ) {
    for( const auto box: src.find( view ) ) {
      if( known.find( box.get() ) == known.end() ) {
        if( is_visible( proj_cam, box->range ) ) {
          dest.insert( dest.end(), box->value.begin(), box->value.end() );
          known.insert( box.get() );
        }
      }
    }
  }
}



bool test_kdtree();

}

#endif
