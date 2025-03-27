#ifndef GCT_IMAGE_MODE_HPP
#define GCT_IMAGE_MODE_HPP

#include <nlohmann/json_fwd.hpp>

namespace gct {

enum class image_mode {
  writable_without_sync = 0,
  readable_after_sync = 1,
  readable_without_sync_writable_after_sync = 2,
  readable_after_sync_writable_after_sync = 3
};

void to_json( nlohmann::json &dest, const image_mode& );

}

#endif

