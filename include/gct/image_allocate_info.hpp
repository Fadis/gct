#ifndef GCT_IMAGE_ALLOCATION_INFO_HPP
#define GCT_IMAGE_ALLOCATION_INFO_HPP

#include <optional>
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>
#include <gct/image_create_info.hpp>
#include <gct/subview_range.hpp>

namespace gct {
struct image_allocate_info {
  LIBGCT_SETTER( create_info )
  LIBGCT_SETTER( range )
  LIBGCT_SETTER( layout )
  image_create_info_t create_info;
  std::optional< subview_range > range;
  std::optional< vk::ImageLayout > layout;
};

bool operator==( const image_allocate_info&, const image_allocate_info& );
bool operator!=( const image_allocate_info&, const image_allocate_info& );
void to_json( nlohmann::json&, const image_allocate_info& );

}

#endif

