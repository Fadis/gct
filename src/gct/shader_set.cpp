#include <nlohmann/json.hpp>
#include <gct/shader_set.hpp>

void to_json( nlohmann::json &dest, const gct::shader_set_t &src ) {
  dest = nlohmann::json::object();
  for( const auto &v: src ) {
    dest[ v.first ] = v.second.string();
  }
}

