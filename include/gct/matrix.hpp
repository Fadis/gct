#ifndef GCT_MATRIX_HPP
#define GCT_MATRIX_HPP
#include <glm/mat4x4.hpp>
#include <nlohmann/json_fwd.hpp>

namespace glm {
void to_json( nlohmann::json&, const glm::mat4& );
}

#endif

