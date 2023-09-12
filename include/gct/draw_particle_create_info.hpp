#ifndef GCT_DRAW_PARTICLE_CREATE_INFO_HPP
#define GCT_DRAW_PARTICLE_CREATE_INFO_HPP

#include <cstdint>
#include <memory>
#include <vector>
#include <filesystem>
#include <gct/setter.hpp>
#include <gct/named_resource.hpp>

namespace gct {

class allocator_t;
class descriptor_pool_t;
class pipeline_cache_t;
class gbuffer;
struct draw_particle_create_info {
  LIBGCT_SETTER( allocator )
  LIBGCT_SETTER( descriptor_pool )
  LIBGCT_SETTER( pipeline_cache )
  LIBGCT_SETTER( output )
  LIBGCT_SETTER( shaders )
  LIBGCT_SETTER( resources )
  draw_particle_create_info &add_resource(
    const named_resource &n
  );
  draw_particle_create_info &clear_resource();
  draw_particle_create_info &add_draw_mesh_shader(
    const std::filesystem::path&
  );
  draw_particle_create_info &add_shader(
    const std::filesystem::path&
  );
  draw_particle_create_info &add_shader(
    const std::vector< std::filesystem::path >&
  );
  draw_particle_create_info &clear_shader();
  std::shared_ptr< allocator_t > allocator;
  std::shared_ptr< descriptor_pool_t > descriptor_pool;
  std::shared_ptr< pipeline_cache_t > pipeline_cache;
  std::shared_ptr< gbuffer > output;
  std::vector< std::filesystem::path > shaders;
  std::vector< named_resource > resources;
};

}

#endif


