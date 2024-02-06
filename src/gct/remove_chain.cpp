#include <nlohmann/json.hpp>
namespace gct {
nlohmann::json remove_chain( const nlohmann::json &src ) {
  nlohmann::json dest;
  if( src.is_object() ) {
    for( const auto &[k,v]: src.items() ) {
      if( k == "pNext" );
      else if( k == "sType" );
      else dest[ k ] = remove_chain( v );
    }
  }
  else if( src.is_array() ) {
    for( const auto &e: src ) {
      dest.push_back( remove_chain( e ) );
    }
  }
  else {
    dest = src;
  }
  return dest;
}
}

