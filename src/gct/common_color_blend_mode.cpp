#include <nlohmann/json.hpp>
#include <gct/common_color_blend_mode.hpp>
#include <gct/extension.hpp>

namespace gct {
void to_json( nlohmann::json &dest, const common_color_blend_mode &src ) {
  if( src == common_color_blend_mode::none ) {
    dest = "none";
  }
  else if( src == common_color_blend_mode::rgb ) {
    dest = "rgb";
  }
  else if( src == common_color_blend_mode::rgbi ) {
    dest = "rgbi";
  }
  else {
    dest = "unknown";
  }
}
void from_json( const nlohmann::json &src, common_color_blend_mode &dest ) {
  if( src == "none" ) {
    dest = common_color_blend_mode::none;
  }
  else if( src == "rgb" ) {
    dest = common_color_blend_mode::rgb;
  }
  else if( src == "rgbi" ) {
    dest = common_color_blend_mode::rgbi;
  }
  else {
    throw incompatible_json( "from_json to common_color_blend_mode : Unknown value " + src.dump(), __FILE__, __LINE__ );
  }
}
}

