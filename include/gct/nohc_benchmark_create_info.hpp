#ifndef GCT_NOHC_BENCHMARK_CREATE_INFO_HPP
#define GCT_NOHC_BENCHMARK_CREATE_INFO_HPP

#include <vector>
#include <filesystem>
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>
#include <gct/named_resource.hpp>
#include <gct/allocator_set.hpp>

namespace gct {

struct nohc_benchmark_create_info {
  LIBGCT_SETTER( allocator_set )
  LIBGCT_ALLOCATOR_SET_LEGACY_SETTER( allocator_set )
  LIBGCT_SETTER( shader )
  LIBGCT_SETTER( width )
  LIBGCT_SETTER( height )
  LIBGCT_SETTER( color_buffer_count )
  LIBGCT_SETTER( resources )
  LIBGCT_NAMED_RESOURCE_SETTER( resources )
  allocator_set_t allocator_set;
  std::filesystem::path shader;
  unsigned int width = 1920u;
  unsigned int height = 1080u;
  unsigned int color_buffer_count = 1u;
  std::vector< named_resource > resources;
};

void to_json( nlohmann::json &dest, const nohc_benchmark_create_info& );

}

#endif


