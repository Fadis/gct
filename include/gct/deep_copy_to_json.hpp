#ifndef GCT_BUFFER_DEEP_COPY_TO_JSON_HPP
#define GCT_BUFFER_DEEP_COPY_TO_JSON_HPP

#include <nlohmann/json.hpp>
#include <gct/deep_copy.hpp>

namespace gct {
  template< typename T >
  void to_json( nlohmann::json &root, const deep_copy_unique_ptr< T > &v ) {
    if( v )
      root = *v;
    else
      root = nullptr;
  }
  template< typename T >
  void from_json( const nlohmann::json &root, deep_copy_unique_ptr< T > &v ) {
    if( root.is_null() )
      v.reset();
    else
      v.reset( T( root ) );
  }
}

#endif

