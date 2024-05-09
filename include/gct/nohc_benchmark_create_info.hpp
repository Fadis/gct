#ifndef GCT_NOHC_BENCHMARK_CREATE_INFO_HPP
#define GCT_NOHC_BENCHMARK_CREATE_INFO_HPP

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
struct nohc_benchmark_create_info {
  LIBGCT_SETTER( allocator )
  LIBGCT_SETTER( descriptor_pool )
  LIBGCT_SETTER( pipeline_cache )
  LIBGCT_SETTER( shader )
  LIBGCT_SETTER( width )
  LIBGCT_SETTER( height )
  LIBGCT_SETTER( color_buffer_count )
  LIBGCT_SETTER( resources )
  nohc_benchmark_create_info &add_resource(
    const named_resource &n
  );
  nohc_benchmark_create_info &clear_resource();
  std::shared_ptr< allocator_t > allocator;
  std::shared_ptr< descriptor_pool_t > descriptor_pool;
  std::shared_ptr< pipeline_cache_t > pipeline_cache;
  std::filesystem::path shader;
  unsigned int width = 1920u;
  unsigned int height = 1080u;
  unsigned int color_buffer_count = 1u;
  std::vector< named_resource > resources;
};

}

#endif


