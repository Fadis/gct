#include <nlohmann/json.hpp>
#include <gct/allocator.hpp>
#include <gct/vertex_buffer_pool_create_info.hpp>

namespace gct {

void to_json( nlohmann::json &dest, const vertex_buffer_pool_create_info &src ) {
  dest = nlohmann::json::object();
  dest[ "allocator" ] = *src.allocator;
  dest[ "max_vertex_buffer_count" ] = src.max_vertex_buffer_count;
}

}


