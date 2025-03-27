#ifndef GCT_DYNAMIC_SIZE_IMAGE_ALLOCATE_INFO_HPP
#define GCT_DYNAMIC_SIZE_IMAGE_ALLOCATE_INFO_HPP
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>
#include <gct/image_allocate_info.hpp>
#include <gct/image_io_dimension.hpp>

namespace gct {
struct dynamic_size_image_allocate_info {
  LIBGCT_SETTER( allocate_info )
  LIBGCT_SETTER( dim )
  image_allocate_info allocate_info;
  image_io_dimension dim;
};

void to_json( nlohmann::json &dest, const dynamic_size_image_allocate_info &src );

}
#endif

