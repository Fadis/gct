#ifndef GCT_VERTEX_BUFFER_POOL_CREATE_INFO_HPP
#define GCT_VERTEX_BUFFER_POOL_CREATE_INFO_HPP
#include <memory>
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>

namespace gct {

class allocator_t;
class descriptor_set_t;
struct vertex_buffer_pool_create_info {
  LIBGCT_SETTER( allocator )
  LIBGCT_SETTER( max_vertex_buffer_count )
  LIBGCT_SETTER( descriptor_set )
  LIBGCT_SETTER( descriptor_name )
  std::shared_ptr< allocator_t > allocator;
  std::uint32_t max_vertex_buffer_count = 65536u;
  std::shared_ptr< descriptor_set_t > descriptor_set;
  std::string descriptor_name = "vertices";
};
void to_json( nlohmann::json &dest, const vertex_buffer_pool_create_info &src );

}

#endif

