#include <gct/vertex_attributes.hpp>
namespace gct {
  std::string get_normalized_vertex_attribute_name( const std::string &name ) {
    std::string converted;
    converted.reserve( name.size() );
    for( const auto c: name ) {
      if( c >= 'A' && c <= 'Z' )
        converted += c - ( 'Z' - 'z' );
      else if( c >= 'a' && c <= 'z' )
        converted += c;
    }
    return converted;
  }
  unsigned int get_normalized_vertex_attribute_index( const std::string &name ) {
    std::string converted;
    converted.reserve( name.size() );
    for( const auto c: name ) {
      if( c >= '0' && c <= '9' ) {
        converted += c;
      }
    }
    return std::stoi( converted );
  }
}

