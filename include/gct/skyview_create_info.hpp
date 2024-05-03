#ifndef GCT_SKYVIEW_CREATE_INFO_HPP
#define GCT_SKYVIEW_CREATE_INFO_HPP

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
struct skyview_create_info {
  LIBGCT_SETTER( allocator )
  LIBGCT_SETTER( descriptor_pool )
  LIBGCT_SETTER( pipeline_cache )
  LIBGCT_SETTER( transmittance_width )
  LIBGCT_SETTER( transmittance_height )
  LIBGCT_SETTER( multiscat_size )
  LIBGCT_SETTER( skyview_width )
  LIBGCT_SETTER( skyview_height )
  LIBGCT_SETTER( transmittance_shader )
  LIBGCT_SETTER( multiscat_shader )
  LIBGCT_SETTER( skyview_shader )
  LIBGCT_SETTER( resources )
  skyview_create_info &add_resource(
    const named_resource &n
  );
  skyview_create_info &clear_resource();
  skyview_create_info &set_shader(
    const std::filesystem::path &
  );
  std::shared_ptr< allocator_t > allocator;
  std::shared_ptr< descriptor_pool_t > descriptor_pool;
  std::shared_ptr< pipeline_cache_t > pipeline_cache;
  unsigned int transmittance_width = 256u;
  unsigned int transmittance_height = 64u;
  unsigned int multiscat_size = 32u;
  unsigned int skyview_width = 256u;
  unsigned int skyview_height = 128u;
  std::filesystem::path transmittance_shader;
  std::filesystem::path multiscat_shader;
  std::filesystem::path skyview_shader;
  std::vector< named_resource > resources;
};

}

#endif

