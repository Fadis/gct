#ifndef GCT_LENS_FLARE_CREATE_INFO_HPP
#define GCT_LENS_FLARE_CREATE_INFO_HPP

#include <memory>
#include <vector>
#include <unordered_map>
#include <filesystem>
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>
#include <gct/named_resource.hpp>
#include <gct/texture_pool.hpp>
#include <gct/allocator_set.hpp>

namespace gct {

class shader_module_t;
class descriptor_set_t;
struct lens_flare_create_info {
  LIBGCT_SETTER( allocator_set )
  LIBGCT_ALLOCATOR_SET_LEGACY_SETTER( allocator_set )
  LIBGCT_SETTER( descriptor_set_layout )
  LIBGCT_SETTER( width )
  LIBGCT_SETTER( height )
  LIBGCT_SETTER( shaders )
  LIBGCT_SETTER( texture )
  LIBGCT_SETTER( lens_size )
  LIBGCT_SETTER( sensor_size )
  LIBGCT_SETTER( matrix_count )
  LIBGCT_SETTER( external_descriptor_set )
  LIBGCT_SETTER( resources )
  lens_flare_create_info &add_resource(
    const named_resource &n
  );
  lens_flare_create_info &clear_resource();
  lens_flare_create_info &set_shader(
    const std::filesystem::path &
  );
  lens_flare_create_info &add_shader(
    const std::filesystem::path&
  );
  lens_flare_create_info &add_shader(
    const std::vector< std::filesystem::path >&
  );
  lens_flare_create_info &set_external_descriptor_set(
    const std::shared_ptr< descriptor_set_t > &v
  ) {
    external_descriptor_set.insert( std::make_pair( 0u, v ) );
    return *this;
  }
  lens_flare_create_info &add_external_descriptor_set(
    unsigned int id,
    const std::shared_ptr< descriptor_set_t > &v
  ) {
    external_descriptor_set.insert( std::make_pair( id, v ) );
    return *this;
  }
  lens_flare_create_info &clear_shader();
  allocator_set_t allocator_set;
  std::vector< std::filesystem::path > shaders;
  std::vector< std::shared_ptr< descriptor_set_layout_t > > descriptor_set_layout;
  std::uint32_t width = 1920u;
  std::uint32_t height = 1080u;
  texture_pool::texture_descriptor texture;
  float lens_size = 0.050f / 2.8f; // 50mm lens with F2.8
  glm::vec2 sensor_size = glm::vec2( 0.036f, 0.036f*9.0f/16.0f );
  std::uint32_t matrix_count = 0u;
  std::unordered_map< unsigned int, std::shared_ptr< descriptor_set_t > > external_descriptor_set;
  std::vector< named_resource > resources;
};
void to_json( nlohmann::json &dest, const lens_flare_create_info &src );

}

#endif


