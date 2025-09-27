#ifndef GCT_COMMON_COLOR_BLEND_MODE_HPP
#define GCT_COMMON_COLOR_BLEND_MODE_HPP

#include <nlohmann/json_fwd.hpp>

namespace gct {
enum class common_color_blend_mode {
  none = 0,
  rgb,
  rgbi
};

void to_json( nlohmann::json&, const common_color_blend_mode& );
void from_json( const nlohmann::json&, common_color_blend_mode& );

}

#endif

