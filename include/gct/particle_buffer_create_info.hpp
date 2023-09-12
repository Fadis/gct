#ifndef GCT_PARTICLE_BUFFER_CREATE_INFO_HPP
#define GCT_PARTICLE_BUFFER_CREATE_INFO_HPP

#include <cstdint>
#include <memory>
#include <vector>
#include <filesystem>
#include <gct/setter.hpp>

namespace gct {

class allocator_t;
class descriptor_pool_t;
class pipeline_cache_t;
struct particle_buffer_create_info {
  LIBGCT_SETTER( allocator )
  LIBGCT_SETTER( descriptor_pool )
  LIBGCT_SETTER( pipeline_cache )
  LIBGCT_SETTER( filename )
  LIBGCT_SETTER( vamap )
  LIBGCT_SETTER( stride )
  std::shared_ptr< allocator_t > allocator;
  std::shared_ptr< descriptor_pool_t > descriptor_pool;
  std::shared_ptr< pipeline_cache_t > pipeline_cache;
  std::filesystem::path filename;
  std::unordered_map< vertex_attribute_usage_t, vertex_input_detail_t > vamap;
  std::uint32_t stride;
};

}

#endif



