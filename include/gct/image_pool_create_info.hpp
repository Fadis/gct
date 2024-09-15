#ifndef GCT_IMAGE_POOL_CREATE_INFO_HPP
#define GCT_IMAGE_POOL_CREATE_INFO_HPP
#include <cstdint>
#include <memory>
#include <filesystem>
#include <nlohmann/json_fwd.hpp>
#include <gct/descriptor_set.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/setter.hpp>
#include <gct/named_resource.hpp>
#include <gct/color_space.hpp>

namespace gct {

class allocator_t;
class descriptor_pool_t;
class pipeline_layout_t;
struct image_pool_create_info {
  LIBGCT_SETTER( allocator )
  LIBGCT_SETTER( descriptor_pool )
  LIBGCT_SETTER( pipeline_cache )
  LIBGCT_SETTER( max_image_count )
  LIBGCT_SETTER( descriptor_set_layout )
  LIBGCT_SETTER( external_descriptor_set )
  LIBGCT_SETTER( external_pipeline_layout )
  LIBGCT_SETTER( descriptor_name )
  LIBGCT_SETTER( layout )
  LIBGCT_SETTER( matrix_pool )
  LIBGCT_SETTER( matrix_buffer_name )
  LIBGCT_SETTER( enable_linear )
  LIBGCT_SETTER( csmat )
  LIBGCT_SETTER( rgba8_shader )
  LIBGCT_SETTER( rgba16_shader )
  LIBGCT_SETTER( rgba16f_shader )
  LIBGCT_SETTER( rgba32f_shader )
  LIBGCT_SETTER( resources )
  LIBGCT_SETTER( image_descriptor_set_id )
  image_pool_create_info &add_resource(
    const named_resource &n
  );
  image_pool_create_info &clear_resource();
  image_pool_create_info &set_shader(
    const std::filesystem::path &
  );
  image_pool_create_info &set_external_descriptor_set(
    const std::shared_ptr< descriptor_set_t > &v
  ) {
    external_descriptor_set.insert( std::make_pair( 0u, v ) );
    return *this;
  }
  image_pool_create_info &add_external_descriptor_set(
    unsigned int id,
    const std::shared_ptr< descriptor_set_t > &v
  ) {
    external_descriptor_set.insert( std::make_pair( id, v ) );
    return *this;
  }
  std::shared_ptr< allocator_t > allocator;
  std::shared_ptr< descriptor_pool_t > descriptor_pool;
  std::shared_ptr< pipeline_cache_t > pipeline_cache;
  std::uint32_t max_image_count = 1024u;
  std::vector< std::shared_ptr< descriptor_set_layout_t > > descriptor_set_layout;
  std::unordered_map< unsigned int, std::shared_ptr< descriptor_set_t > > external_descriptor_set;
  std::shared_ptr< pipeline_layout_t > external_pipeline_layout;
  std::uint32_t image_descriptor_set_id = 3u;
  std::string descriptor_name = "image_pool16f";
  vk::ImageLayout layout = vk::ImageLayout::eShaderReadOnlyOptimal;
  std::shared_ptr< buffer_t > matrix_pool;
  std::string matrix_buffer_name = "matrix_pool";
  bool enable_linear = false;
  color_space_matrix csmat;
  std::filesystem::path rgba8_shader;
  std::filesystem::path rgba16_shader;
  std::filesystem::path rgba16f_shader;
  std::filesystem::path rgba32f_shader;
  std::vector< named_resource > resources;
};
void to_json( nlohmann::json &dest, const image_pool_create_info &src );

}

#endif


