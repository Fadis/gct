#ifndef LIBGCT_INCLUDE_GCT_GLM_TO_JSON_HPP
#define LIBGCT_INCLUDE_GCT_GLM_TO_JSON_HPP

#include <nlohmann/json_fwd.hpp>
#include <glm/mat4x4.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat2x2.hpp>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

namespace gct {

nlohmann::json glm_to_json( const glm::mat4 &m );
nlohmann::json glm_to_json( const glm::mat3 &m );
nlohmann::json glm_to_json( const glm::mat2 &m );
nlohmann::json glm_to_json( const glm::vec4 &v );
nlohmann::json glm_to_json( const glm::vec3 &v );
nlohmann::json glm_to_json( const glm::vec2 &v );

}

#endif

