#include <nlohmann/json.hpp>
#include <gct/numeric_types.hpp>
#include <gct/buffer.hpp>
#include <gct/nnef_data.hpp>

namespace gct {

void to_json( nlohmann::json &dest, const nnef_data_t &src ) {
  dest = nlohmann::json::object();
  dest[ "type" ] = src.type;
  dest[ "dim" ] = src.dim;
  dest[ "buffer" ] = src.buffer->get_props();
}

}
