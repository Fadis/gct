#ifndef GCT_EPIPOLAR_LIGHT_SCATTERING_CREATE_INFO_HPP
#define GCT_EPIPOLAR_LIGHT_SCATTERING_CREATE_INFO_HPP

#include <cstdint>
#include <memory>
#include <vector>
#include <filesystem>
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>
#include <gct/named_resource.hpp>

namespace gct {

class allocator_t;
class descriptor_pool_t;
class pipeline_cache_t;
struct epipolar_light_scattering_create_info {
  LIBGCT_SETTER( allocator )
  LIBGCT_SETTER( descriptor_pool )
  LIBGCT_SETTER( pipeline_cache )
  LIBGCT_SETTER( pole_count )
  LIBGCT_SETTER( vertex_count )
  LIBGCT_SETTER( input )
  LIBGCT_SETTER( shadow )
  LIBGCT_SETTER( texel_per_sample )
  LIBGCT_SETTER( draw_mesh_shaders )
  LIBGCT_SETTER( generate_mesh2_shader )
  LIBGCT_SETTER( generate_mesh3_shader )
  LIBGCT_SETTER( generate_mesh4_shader )
  LIBGCT_SETTER( volumetric_light_shader )
  LIBGCT_SETTER( clear_texcoord_shader )
  LIBGCT_SETTER( generate_texcoord_shader )
  LIBGCT_SETTER( fill_texcoord_shader )
  LIBGCT_SETTER( hgauss_shader )
  LIBGCT_SETTER( vgauss_shader )
  LIBGCT_SETTER( resources )
  epipolar_light_scattering_create_info &add_resource(
    const named_resource &n
  );
  epipolar_light_scattering_create_info &clear_resource();
  epipolar_light_scattering_create_info &add_draw_mesh_shader(
    const std::filesystem::path&
  );
  epipolar_light_scattering_create_info &clear_draw_mesh_shader();
  std::shared_ptr< allocator_t > allocator;
  std::shared_ptr< descriptor_pool_t > descriptor_pool;
  std::shared_ptr< pipeline_cache_t > pipeline_cache;
  std::uint32_t pole_count = 256u;
  std::uint32_t vertex_count = 16u;
  std::vector< std::shared_ptr< image_view_t > > input;
  std::vector< std::shared_ptr< image_view_t > > shadow;
  std::uint32_t texel_per_sample = 16u;
  std::vector< std::filesystem::path > draw_mesh_shaders;
  std::filesystem::path generate_mesh2_shader;
  std::filesystem::path generate_mesh3_shader;
  std::filesystem::path generate_mesh4_shader;
  std::filesystem::path volumetric_light_shader;
  std::filesystem::path clear_texcoord_shader;
  std::filesystem::path generate_texcoord_shader;
  std::filesystem::path fill_texcoord_shader;
  std::filesystem::path hgauss_shader;
  std::filesystem::path vgauss_shader;
  std::vector< named_resource > resources;
};

}

#endif

