#ifndef GCT_DRAW_PARTICLE_CREATE_INFO_HPP
#define GCT_DRAW_PARTICLE_CREATE_INFO_HPP

#include <memory>
#include <vector>
#include <filesystem>
#include <gct/setter.hpp>
#include <gct/named_resource.hpp>
#include <gct/allocator_set.hpp>

namespace gct {

class gbuffer;
struct draw_particle_create_info {
  LIBGCT_SETTER( allocator_set )
  LIBGCT_ALLOCATOR_SET_LEGACY_SETTER( allocator_set )
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
  allocator_set_t allocator_set;
  std::shared_ptr< gbuffer > output;
  std::vector< std::filesystem::path > shaders;
  std::vector< named_resource > resources;
};

void to_json( nlohmann::json &dest, const draw_particle_create_info &src );

}

#endif


