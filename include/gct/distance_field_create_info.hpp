#ifndef GCT_DISTANCE_FIELD_CREATE_INFO_HPP
#define GCT_DISTANCE_FIELD_CREATE_INFO_HPP

#include <filesystem>
#include <gct/setter.hpp>

namespace gct {

class allocator_t;
class descriptor_pool_t;
class pipeline_cache_t;
class image_view_t;
struct distance_field_create_info {
  LIBGCT_SETTER( allocator )
  LIBGCT_SETTER( descriptor_pool )
  LIBGCT_SETTER( pipeline_cache )
  LIBGCT_SETTER( clear_shader )
  LIBGCT_SETTER( voronoi_shader )
  LIBGCT_SETTER( distance_field_shader )
  LIBGCT_SETTER( size_factor )
  std::shared_ptr< allocator_t > allocator;
  std::shared_ptr< descriptor_pool_t > descriptor_pool;
  std::shared_ptr< pipeline_cache_t > pipeline_cache;
  std::filesystem::path clear_shader;
  std::filesystem::path voronoi_shader;
  std::filesystem::path distance_field_shader;
  unsigned int size_factor = 9u;
};

}

#endif

