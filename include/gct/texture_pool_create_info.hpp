#ifndef GCT_TEXTURE_POOL_CREATE_INFO_HPP
#define GCT_TEXTURE_POOL_CREATE_INFO_HPP
#include <cstdint>
#include <memory>
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>

namespace gct {

class allocator_t;
class pipeline_cache_t;
class descriptor_set_t;
class sampler_pool;
class image_pool;
struct texture_pool_create_info {
  LIBGCT_SETTER( allocator )
  LIBGCT_SETTER( max_texture_count )
  LIBGCT_SETTER( descriptor_set )
  LIBGCT_SETTER( descriptor_name )
  LIBGCT_SETTER( sampler )
  LIBGCT_SETTER( image )
  std::shared_ptr< allocator_t > allocator;
  std::uint32_t max_texture_count = 1024u;
  std::shared_ptr< descriptor_set_t > descriptor_set;
  std::string descriptor_name = "texture_pool";
  std::shared_ptr< sampler_pool > sampler;
  std::shared_ptr< image_pool > image;
};
void to_json( nlohmann::json &dest, const texture_pool_create_info &src );

}

#endif

