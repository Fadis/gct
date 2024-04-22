#ifndef GCT_KDTREE_HPP
#define GCT_KDTREE_HPP
#include <iostream>
#include <memory>
#include <gct/aabb.hpp>

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
private:
  struct node_type {
    axis_type axis = axis_type::x_begin;
    float threshold = 0.0;
    std::shared_ptr< node_type > less_than;
    std::shared_ptr< node_type > greater_equal;
    std::shared_ptr< leaf_type > leaf;
  };
public:
  void insert( const aabb &r, const T &v ) {
    insert( root, axis_type::x_begin, r, v );
  }
  std::vector< std::shared_ptr< leaf_type > > find( const aabb &r ) const {
    return find( root, r );
  }
  std::string to_string() const {
    return to_string( root );
  }
private:
  std::string to_string( const std::shared_ptr< node_type > &node ) const {
    if( !node ) return "null";
    else if( node->leaf ) {
      return "{\"range\":\"" + gct::to_string( node->leaf->range ) + "}";
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
        if( existing_axis_value < threshold ) {
          new_node->less_than = node;
          insert( new_node->greater_equal, next( axis ), r, v );
        }
        else {
          insert( new_node->less_than, next( axis ), r, v );
          new_node->greater_equal = node;
        }
        node = new_node;
      }
    }
    else {
      const auto axis_value = get_axis_value( node->axis, r );
      if( axis_value < node->threshold ) {
        insert( node->less_than, next( axis ), r, v );
      }
      else {
        insert( node->greater_equal, next( axis ), r, v );
      }
    }
  }
  static std::vector< std::shared_ptr< leaf_type > > find( const std::shared_ptr< node_type > &node, const aabb &r ) {
    if( !node ) {
      return {};
    }
    else if( node->leaf ) {
      if( node->leaf->range && r ) {
        return { node->leaf };
      }
      else {
        return {};
      }
    }
    else {
      if( node->axis == axis_type::x_begin ) {
        auto lt = find( node->less_than, r );
        if( r.max.x >= node->threshold ) {
          auto ge = find( node->greater_equal, r );
          lt.insert( lt.end(), ge.begin(), ge.end() );
        }
        return lt;
      }
      else if( node->axis == axis_type::y_begin ) {
        auto lt = find( node->less_than, r );
        if( r.max.y >= node->threshold ) {
          auto ge = find( node->greater_equal, r );
          lt.insert( lt.end(), ge.begin(), ge.end() );
        }
        return lt;
      }
      else if( node->axis == axis_type::z_begin ) {
        auto lt = find( node->less_than, r );
        if( r.max.z >= node->threshold ) {
          auto ge = find( node->greater_equal, r );
          lt.insert( lt.end(), ge.begin(), ge.end() );
        }
        return lt;
      }
      else if( node->axis == axis_type::x_end ) {
        auto ge = find( node->greater_equal, r );
        if( r.min.x < node->threshold ) {
          auto lt = find( node->less_than, r );
          ge.insert( ge.end(), lt.begin(), lt.end() );
        }
        return ge;
      }
      else if( node->axis == axis_type::y_end ) {
        auto ge = find( node->greater_equal, r );
        if( r.min.y < node->threshold ) {
          auto lt = find( node->less_than, r );
          ge.insert( ge.end(), lt.begin(), lt.end() );
        }
        return ge;
      }
      else if( node->axis == axis_type::z_end ) {
        auto ge = find( node->greater_equal, r );
        if( r.min.z < node->threshold ) {
          auto lt = find( node->less_than, r );
          ge.insert( ge.end(), lt.begin(), lt.end() );
        }
        return ge;
      }
      else {
        return {};
      }
    }
    return {};
  }
  std::shared_ptr< node_type > root;
};

bool test_kdtree();

}

#endif
