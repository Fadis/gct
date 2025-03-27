#ifndef GCT_IMAGE_IO_DIMENSION_HPP
#define GCT_IMAGE_IO_DIMENSION_HPP

#include <string>
#include <optional>
#include <glm/mat4x4.hpp>
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>

namespace gct {
struct image_io_dimension {
  LIBGCT_SETTER( relative_to )
  LIBGCT_SETTER( size_transform )
  std::optional< std::string > relative_to;
  glm::mat4x4 size_transform = glm::mat4x4(
    1.f, 0.f, 0.f, 0.f,
    0.f, 1.f, 0.f, 0.f,
    0.f, 0.f, 1.f, 0.f,
    0.f, 0.f, 0.f, 1.f
  );
};

void to_json( nlohmann::json&, const image_io_dimension& );

}

#endif

