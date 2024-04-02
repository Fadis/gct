#ifndef GCT_IMAGE_POOL_CREATE_INFO_HPP
#define GCT_IMAGE_POOL_CREATE_INFO_HPP
#include <cstdint>
#include <memory>
#include <nlohmann/json_fwd.hpp>
#include <gct/descriptor_set.hpp>
#include <gct/setter.hpp>

namespace gct {

class allocator_t;
class descriptor_pool_t;
struct image_pool_create_info {
  LIBGCT_SETTER( allocator )
  LIBGCT_SETTER( max_image_count )
  LIBGCT_SETTER( descriptor_set )
  LIBGCT_SETTER( descriptor_name )
  LIBGCT_SETTER( layout )
  std::shared_ptr< allocator_t > allocator;
  std::uint32_t max_image_count = 65536u;
  std::shared_ptr< descriptor_set_t > descriptor_set;
  std::string descriptor_name = "image_pool";
  vk::ImageLayout layout = vk::ImageLayout::eShaderReadOnlyOptimal;
};
void to_json( nlohmann::json &dest, const image_pool_create_info &src );

}

#endif


