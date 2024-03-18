#ifndef GCT_COMPUTE_CREATE_INFO_HPP
#define GCT_COMPUTE_CREATE_INFO_HPP

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
struct compute_create_info {
  LIBGCT_SETTER( allocator )
  LIBGCT_SETTER( descriptor_pool )
  LIBGCT_SETTER( pipeline_cache )
  LIBGCT_SETTER( shader )
  LIBGCT_SETTER( resources )
  LIBGCT_SETTER( swapchain_image_count )
  compute_create_info &add_resource(
    const named_resource &n
  );
  compute_create_info &clear_resource(
    const named_resource &n
  );
  std::shared_ptr< allocator_t > allocator;
  std::shared_ptr< descriptor_pool_t > descriptor_pool;
  std::shared_ptr< pipeline_cache_t > pipeline_cache;
  std::filesystem::path shader;
  std::vector< named_resource > resources;
  unsigned int swapchain_image_count = 1u;
};

void to_json( nlohmann::json&, const compute_create_info& );

}

#endif

