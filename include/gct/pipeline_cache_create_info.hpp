#ifndef GCT_PIPELINE_CACHE_CREATE_INFO_HPP
#define GCT_PIPELINE_CACHE_CREATE_INFO_HPP
#include <string>
#include <memory>
#include <unordered_map>
#include <vector>
#include <vulkan/vulkan.hpp>
#include <gct/extension.hpp>

namespace gct {
  class pipeline_cache_create_info_t {
  public:
    using self_type = pipeline_cache_create_info_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
    LIBGCT_BASIC_SETTER( vk::PipelineCacheCreateInfo )
  private:
    std::vector< std::uint8_t > data;
  public:
    pipeline_cache_create_info_t &load( const std::string &filename );
  };
}

#endif

