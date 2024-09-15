#ifndef GCT_GRAPHICS_CREATE_INFO_HPP
#define GCT_GRAPHICS_CREATE_INFO_HPP

#include <memory>
#include <vector>
#include <filesystem>
#include <unordered_map>
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>
#include <gct/named_resource.hpp>
#include <gct/graphics_pipeline_create_info.hpp>

namespace gct {

class allocator_t;
class descriptor_pool_t;
class pipeline_cache_t;
class render_pass_t;
class shader_module_t;
class descriptor_set_t;
class pipeline_layout_t;
struct graphics_create_info {
  LIBGCT_SETTER( allocator )
  LIBGCT_SETTER( descriptor_pool )
  LIBGCT_SETTER( pipeline_cache )
  LIBGCT_SETTER( shaders )
  LIBGCT_SETTER( shader_module )
  LIBGCT_SETTER( pipeline_create_info )
  LIBGCT_SETTER( external_descriptor_set )
  LIBGCT_SETTER( external_pipeline_layout )
  LIBGCT_SETTER( resources )
  LIBGCT_SETTER( swapchain_image_count )
  LIBGCT_SETTER( ignore_unused_descriptor )
  graphics_create_info &add_resource(
    const named_resource &n
  );
  graphics_create_info &clear_resource(
    const named_resource &n
  );
  graphics_create_info &add_shader(
    const std::filesystem::path&
  );
  graphics_create_info &add_shader(
    const std::vector< std::filesystem::path >&
  );
  graphics_create_info &add_shader(
    const std::shared_ptr< shader_module_t >&
  );
  graphics_create_info &set_external_descriptor_set(
    const std::shared_ptr< descriptor_set_t > &v
  ) {
    external_descriptor_set.insert( std::make_pair( 0u, v ) );
    return *this;
  }
  graphics_create_info &add_external_descriptor_set(
    unsigned int id,
    const std::shared_ptr< descriptor_set_t > &v
  ) {
    external_descriptor_set.insert( std::make_pair( id, v ) );
    return *this;
  }
  graphics_create_info &clear_shader();
  std::shared_ptr< allocator_t > allocator;
  std::shared_ptr< descriptor_pool_t > descriptor_pool;
  std::shared_ptr< pipeline_cache_t > pipeline_cache;
  std::vector< std::filesystem::path > shaders;
  std::vector< std::shared_ptr< shader_module_t > > shader_module;
  graphics_pipeline_create_info_t pipeline_create_info;
  std::unordered_map< unsigned int, std::shared_ptr< descriptor_set_t > > external_descriptor_set;
  std::shared_ptr< pipeline_layout_t > external_pipeline_layout;
  std::vector< named_resource > resources;
  unsigned int swapchain_image_count = 1u;
  bool ignore_unused_descriptor = false;
};

}

#endif


