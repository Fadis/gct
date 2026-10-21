#ifndef GCT_HOST_IMAGE_HPP
#define GCT_HOST_IMAGE_HPP

#include <vector>
#include <cstdint>
#include <string>
#include <gct/setter.hpp>

namespace gct {

struct host_image {
  LIBGCT_SETTER( loaded )
  LIBGCT_SETTER( image )
  LIBGCT_SETTER( width )
  LIBGCT_SETTER( height )
  LIBGCT_SETTER( channels )
  bool loaded = false;
  std::vector< float > image;
  std::uint32_t width = 0u;
  std::uint32_t height = 0u;
  std::uint32_t channels = 0u;
};

host_image
create_host_image_from_file(
  const std::string &filename,
  bool is_srgb,
  bool degamma,
  bool grayscale
);

host_image
create_host_image(
  std::uint32_t width,
  std::uint32_t height,
  std::uint32_t channels
);

void save_host_image(
  const host_image &image,
  const std::string &filename
);

}

#endif

