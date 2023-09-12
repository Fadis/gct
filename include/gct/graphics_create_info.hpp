#ifndef GCT_GRAPHICS_CREATE_INFO_HPP
#define GCT_GRAPHICS_CREATE_INFO_HPP

#include <memory>
#include <vector>
#include <filesystem>
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>
#include <gct/named_resource.hpp>
#include <gct/graphics_pipeline_create_info.hpp>

namespace gct {

class allocator_t;
class descriptor_pool_t;
class pipeline_cache_t;
class render_pass_t;
struct graphics_create_info {
  LIBGCT_SETTER( allocator )
  LIBGCT_SETTER( descriptor_pool )
  LIBGCT_SETTER( pipeline_cache )
  LIBGCT_SETTER( shaders )
  LIBGCT_SETTER( pipeline_create_info )
  LIBGCT_SETTER( resources )
  LIBGCT_SETTER( swapchain_image_count )
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
  graphics_create_info &clear_shader();
  std::shared_ptr< allocator_t > allocator;
  std::shared_ptr< descriptor_pool_t > descriptor_pool;
  std::shared_ptr< pipeline_cache_t > pipeline_cache;
  std::vector< std::filesystem::path > shaders;
  graphics_pipeline_create_info_t pipeline_create_info;
  std::vector< named_resource > resources;
  unsigned int swapchain_image_count = 1u;
};

}

#endif


