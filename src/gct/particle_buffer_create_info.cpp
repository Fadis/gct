#include <nlohmann/json.hpp>
#include <gct/particle_buffer_create_info.hpp>

namespace gct {

void to_json( nlohmann::json &dest, const particle_buffer_create_info &src ) {
  dest = nlohmann::json::object();
  dest[ "allocator_set" ] = src.allocator_set;
  dest[ "filename" ] = src.filename;
  dest[ "vamap" ] = nlohmann::json::object();
  for( const auto &p: src.vamap ) {
    dest[ "vamap" ][ to_string( p.first ) ] = p.second;
  }
  dest[ "stride" ] = src.stride;
}

}

