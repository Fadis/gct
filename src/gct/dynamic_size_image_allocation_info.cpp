#include <nlohmann/json.hpp>
#include <gct/dynamic_size_image_allocate_info.hpp>

namespace gct {
void to_json( nlohmann::json &dest, const dynamic_size_image_allocate_info &src ) {
  dest = nlohmann::json::object();
  dest[ "allocate_info" ] = src.allocate_info;
  dest[ "dim" ] = src.dim;
}

}
