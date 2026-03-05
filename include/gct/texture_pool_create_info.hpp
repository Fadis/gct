#ifndef GCT_TEXTURE_POOL_CREATE_INFO_HPP
#define GCT_TEXTURE_POOL_CREATE_INFO_HPP
#include <cstdint>
#include <memory>
#include <filesystem>
#include <vector>
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>
#include <gct/allocator_set.hpp>
#include <gct/named_resource.hpp>
#include <gct/color_space.hpp>
#include <gct/alignment.hpp>

namespace gct {

class allocator_t;
class pipeline_cache_t;
class descriptor_set_t;
class sampler_pool;
class image_pool;
struct texture_pool_create_info {
  LIBGCT_SETTER( allocator_set )
  LIBGCT_ALLOCATOR_SET_LEGACY_SETTER( allocator_set )
  LIBGCT_SETTER( max_texture_count )
  LIBGCT_SETTER( max_request_count )
  LIBGCT_SETTER( descriptor_set )
  LIBGCT_SETTER( descriptor_name )
  LIBGCT_SETTER( sampler )
  LIBGCT_SETTER( image )
  LIBGCT_SETTER( csmat )
  LIBGCT_SETTER( dummy_shader )
  LIBGCT_SETTER( metadata_buffer_name )
  LIBGCT_SETTER( metadata_layout )
  LIBGCT_NAMED_RESOURCE_SETTER( resources )
  texture_pool_create_info &set_shader(
    const std::filesystem::path &
  );
  bool shader_exists() const;
  allocator_set_t allocator_set;
  std::filesystem::path dummy_shader;
  std::uint32_t max_texture_count = 1024u;
  std::uint32_t max_request_count = 1024u;
  std::shared_ptr< descriptor_set_t > descriptor_set;
  std::string descriptor_name = "texture_pool";
  std::shared_ptr< sampler_pool > sampler;
  std::shared_ptr< image_pool > image;
  color_space_matrix csmat;
  std::string metadata_buffer_name = "texture_metadata_pool";
  memory_layout metadata_layout = memory_layout::std430;
  std::vector< named_resource > resources;
};
void to_json( nlohmann::json &dest, const texture_pool_create_info &src );

}

#endif

