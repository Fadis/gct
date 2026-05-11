#include <cctype>
#include <algorithm>
#include <charconv>
#include <array>
#include <nlohmann/json.hpp>
#include <gct/vertex_attribute_id.hpp>

namespace gct {
vertex_attribute_id to_vertex_attribute_id( const std::string &src ) {
  std::string simplified;
  for( const auto &c: src ) {
    if( std::isalnum( c ) ) {
      simplified.push_back( std::tolower( c ) );
    }
  }
  if( simplified == "index" )
    return vertex_attribute_id::index;
  else if( simplified == "position" )
    return vertex_attribute_id::position;
  else if( simplified == "pos" )
    return vertex_attribute_id::position;
  else if( simplified == "normal" )
    return vertex_attribute_id::normal;
  else if( simplified == "tangent" )
    return vertex_attribute_id::tangent;
  else if( simplified == "texcoord0" )
    return vertex_attribute_id::texcoord_0;
  else if( simplified == "texcoord" )
    return vertex_attribute_id::texcoord_0;
  else if( simplified == "color0" )
    return vertex_attribute_id::color_0;
  else if( simplified == "color" )
    return vertex_attribute_id::color_0;
  else if( simplified == "joint0" )
    return vertex_attribute_id::joint_0;
  else if( simplified == "joint" )
    return vertex_attribute_id::joint_0;
  else if( simplified == "weight0" )
    return vertex_attribute_id::weight_0;
  else if( simplified == "weight" )
    return vertex_attribute_id::weight_0;
  else if( simplified == "lodmorph" )
    return vertex_attribute_id::lod_morph;
  else {
    int temp = -1;
    std::from_chars( simplified.data(), simplified.data() + simplified.size(), temp );
    return vertex_attribute_id( temp );
  }
}

void to_json( nlohmann::json &dest, const vertex_attribute_id &src ) {
  if( src == vertex_attribute_id::index )
    dest = "index";
  else if( src == vertex_attribute_id::position )
    dest = "position";
  else if( src == vertex_attribute_id::normal )
    dest = "normal";
  else if( src == vertex_attribute_id::tangent )
    dest = "tangent";
  else if( src == vertex_attribute_id::texcoord_0 )
    dest = "texcoord_0";
  else if( src == vertex_attribute_id::color_0 )
    dest = "color_0";
  else if( src == vertex_attribute_id::joint_0 )
    dest = "joint_0";
  else if( src == vertex_attribute_id::weight_0 )
    dest = "weight_0";
  else if( src == vertex_attribute_id::lod_morph )
    dest = "lod_morph";
  else {
    std::array< char, 22u > temp;
    std::fill( temp.begin(), temp.end(), char( 0 ) );
    std::to_chars( temp.begin(), temp.end(), int( src ) );
    dest = temp.data();
  }
}

void from_json( const nlohmann::json &src, vertex_attribute_id &dest ) {
  if( src == "index" )
    dest = vertex_attribute_id::index;
  else if( src == "position" )
    dest = vertex_attribute_id::position;
  else if( src == "normal" )
    dest = vertex_attribute_id::normal;
  else if( src == "tangent" )
    dest = vertex_attribute_id::tangent;
  else if( src == "texcoord_0" )
    dest = vertex_attribute_id::texcoord_0;
  else if( src == "color_0" )
    dest = vertex_attribute_id::color_0;
  else if( src == "joint_0" )
    dest = vertex_attribute_id::joint_0;
  else if( src == "weight_0" )
    dest = vertex_attribute_id::weight_0;
  else if( src == "lod_morph" )
    dest = vertex_attribute_id::lod_morph;
  else {
    const std::string &src_ = src;
    int temp = -1;
    std::from_chars( src_.data(), src_.data() + src_.size(), temp );
    dest = vertex_attribute_id( temp );
  }
}

}

