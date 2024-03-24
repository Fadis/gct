#ifndef GCT_SAMPLER_POOL_CREATE_INFO_HPP
#define GCT_SAMPLER_POOL_CREATE_INFO_HPP
#include <cstdint>
#include <memory>
#include <nlohmann/json_fwd.hpp>
#include <gct/descriptor_set.hpp>
#include <gct/setter.hpp>

namespace gct {

class allocator_t;
class descriptor_pool_t;
struct sampler_pool_create_info {
  LIBGCT_SETTER( allocator )
  LIBGCT_SETTER( descriptor_pool )
  LIBGCT_SETTER( max_sampler_count )
  LIBGCT_SETTER( descriptor_set )
  LIBGCT_SETTER( descriptor_name )
  std::shared_ptr< allocator_t > allocator;
  std::shared_ptr< descriptor_pool_t > descriptor_pool;
  std::uint32_t max_sampler_count = 65536u;
  std::shared_ptr< descriptor_set_t > descriptor_set;
  std::string descriptor_name = "samplers";
};
void to_json( nlohmann::json &dest, const sampler_pool_create_info &src );

}

#endif

