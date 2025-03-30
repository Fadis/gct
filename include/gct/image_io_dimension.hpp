#ifndef GCT_IMAGE_IO_DIMENSION_HPP
#define GCT_IMAGE_IO_DIMENSION_HPP

#include <string>
#include <optional>
#include <glm/mat4x4.hpp>
#include <glm/mat2x2.hpp>
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>

namespace gct {
struct image_io_dimension {
  LIBGCT_SETTER( relative_to )
  LIBGCT_SETTER( size_transform )
  LIBGCT_SETTER( preserve_layer_count )
  LIBGCT_SETTER( layer_transform )
  std::optional< std::string > relative_to;
  glm::mat4x4 size_transform = glm::mat4x4(
    1.f, 0.f, 0.f, 0.f,
    0.f, 1.f, 0.f, 0.f,
    0.f, 0.f, 1.f, 0.f,
    0.f, 0.f, 0.f, 1.f
  );
  bool preserve_layer_count = true;
  glm::mat2x2 layer_transform = glm::mat2x2(
    1.f, 0.f,
    0.f, 1.f
  );
};

void to_json( nlohmann::json&, const image_io_dimension& );

}

#endif

