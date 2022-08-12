#include <gct/vertex_attributes.hpp>
#include <gct/extension.hpp>
#include <nlohmann/json.hpp>

namespace gct {
  std::string to_string( const vertex_attribute_usage_t &src ) {
    if( src == vertex_attribute_usage_t::POSITION ) return "position";
    else if( src == vertex_attribute_usage_t::NORMAL ) return "normal";
    else if( src == vertex_attribute_usage_t::TANGENT ) return "tangent";
    else if( src == vertex_attribute_usage_t::TEXCOORD ) return "texcoord";
    else return "unknown";
  }
  bool from_string( const std::string &src, vertex_attribute_usage_t &dest ) {
    if( src == "position" ) dest = vertex_attribute_usage_t::POSITION;
    if( src == "normal" ) dest = vertex_attribute_usage_t::NORMAL;
    if( src == "tangent" ) dest = vertex_attribute_usage_t::TANGENT;
    if( src == "texcoord" ) dest = vertex_attribute_usage_t::TEXCOORD;
    else return false;
    return true;
  }
  void to_json( nlohmann::json &dest, const vertex_attribute_usage_t &src ) {
    if( src == vertex_attribute_usage_t::POSITION ) dest = "position";
    else if( src == vertex_attribute_usage_t::NORMAL ) dest = "normal";
    else if( src == vertex_attribute_usage_t::TANGENT ) dest = "tangent";
    else if( src == vertex_attribute_usage_t::TEXCOORD ) dest = "texcoord";
    else throw -1;
  }
  void to_json( nlohmann::json &dest, const vertex_input_detail_t &src ) {
    dest = nlohmann::json::object();
    dest[ "location" ] = src.location;
    dest[ "offset" ] = src.offset;
    dest[ "format" ] = src.format;
  }
  void to_json( nlohmann::json &dest, const std::unordered_map< vertex_attribute_usage_t, vertex_input_detail_t > &src ) {
    dest = nlohmann::json::object();
    for( const auto &[key,value]: src ) {
      dest[ to_string( key ) ] = value;
    }
  }
  void from_json( const nlohmann::json &src, vertex_attribute_usage_t &dest ) {
    if( src == "position" ) dest = vertex_attribute_usage_t::POSITION;
    else if( src == "normal" ) dest = vertex_attribute_usage_t::NORMAL;
    else if( src == "tangent" ) dest = vertex_attribute_usage_t::TANGENT;
    else if( src == "texcoord" ) dest = vertex_attribute_usage_t::TEXCOORD;
    else throw incompatible_json( "The JSON is incompatible to pipeline_vertex_input_state_create_info_t", __FILE__, __LINE__ );
  }
  void from_json( const nlohmann::json &src, vertex_input_detail_t &dest ) {
    if( !src.is_object() ) throw incompatible_json( "The JSON is incompatible to pipeline_vertex_input_state_create_info_t", __FILE__, __LINE__ );
    dest.location = src[ "location" ];
    dest.offset = src[ "offset" ];
    dest.format = src[ "format" ];
  }
  void from_json( const nlohmann::json &src, std::unordered_map< vertex_attribute_usage_t, vertex_input_detail_t > &dest ) {
    if( !src.is_object() ) throw incompatible_json( "The JSON is incompatible to pipeline_vertex_input_state_create_info_t", __FILE__, __LINE__ );
    for( auto it = src.begin(); it != src.end(); ++it ) {
      vertex_attribute_usage_t temp;
      if( from_string( it.key(), temp ) ) {
        dest.insert( std::make_pair(
          temp,
          vertex_input_detail_t( it.value() )
        ) );
      }
      else throw incompatible_json( "The JSON is incompatible to pipeline_vertex_input_state_create_in  fo_t", __FILE__, __LINE__ );
    }
  }
}

