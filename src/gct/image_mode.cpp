#include <nlohmann/json.hpp>
#include <gct/image_mode.hpp>

namespace gct {

void to_json( nlohmann::json &dest, const image_mode &src ) {
  if( src == image_mode::writable_without_sync ) {
    dest = "writable_without_sync";
  }
  else if( src == image_mode::readable_after_sync ) {
    dest = "readable_after_sync";
  }
  else if( src == image_mode::readable_without_sync_writable_after_sync ) {
    dest = "readable_without_sync_writable_after_sync";
  }
  else if( src == image_mode::readable_after_sync_writable_after_sync ) {
    dest = "readable_after_sync_writable_after_sync";
  }
  else {
    dest = "unknown";
  }
}

}

