#include <nlohmann/json.hpp>
#include <gct/extension.hpp>
#include <gct/linear_allocator_create_info.hpp>
namespace gct {
void to_json( nlohmann::json &dest, const linear_allocator_create_info &src ) {
  dest = nlohmann::json::object();
  dest[ "max" ] = src.max;
}
void from_json( const nlohmann::json &src, linear_allocator_create_info &dest ) {
  if( !src.is_object() ) {
    throw incompatible_json( "The JSON is incompatible to linear_allocator_create_info", __FILE__, __LINE__ );
  }
  dest = linear_allocator_create_info();
  if( src.find( "max" ) != src.end() ) {
    dest.set_max( std::uint32_t( src[ "max" ] ) );
  }
}
}

