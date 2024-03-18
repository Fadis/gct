#ifndef GCT_TEXTURE_POOL_CREATE_INFO_HPP
#define GCT_TEXTURE_POOL_CREATE_INFO_HPP
#include <cstdint>
#include <memory>
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>

namespace gct {

class allocator_t;
class descriptor_pool_t;
class pipeline_cache_t;
struct texture_pool_create_info {
  LIBGCT_SETTER( allocator )
  LIBGCT_SETTER( descriptor_pool )
  LIBGCT_SETTER( pipeline_cache )
  LIBGCT_SETTER( max_texture_count )
  LIBGCT_SETTER( descriptor_set )
  LIBGCT_SETTER( descriptor_name )
  std::shared_ptr< allocator_t > allocator;
  std::shared_ptr< descriptor_pool_t > descriptor_pool;
  std::shared_ptr< pipeline_cache_t > pipeline_cache;
  std::uint32_t max_texture_count = 65536u;
  std::shared_ptr< descriptor_set_t > descriptor_set;
  std::string descriptor_name = "textures";
};
void to_json( nlohmann::json &dest, const texture_pool_create_info &src );

}

#endif

