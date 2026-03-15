#include <nlohmann/json.hpp>
#include <glm/mat4x4.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat2x2.hpp>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <gct/glm_to_json.hpp>

namespace gct {
nlohmann::json glm_to_json( const glm::mat4 &m ) {
  auto temp = nlohmann::json::array();
  {
    temp.push_back( m[ 0 ][ 0 ] );
    temp.push_back( m[ 0 ][ 1 ] );
    temp.push_back( m[ 0 ][ 2 ] );
    temp.push_back( m[ 0 ][ 3 ] );
    temp.push_back( m[ 1 ][ 0 ] );
    temp.push_back( m[ 1 ][ 1 ] );
    temp.push_back( m[ 1 ][ 2 ] );
    temp.push_back( m[ 1 ][ 3 ] );
    temp.push_back( m[ 2 ][ 0 ] );
    temp.push_back( m[ 2 ][ 1 ] );
    temp.push_back( m[ 2 ][ 2 ] );
    temp.push_back( m[ 2 ][ 3 ] );
    temp.push_back( m[ 3 ][ 0 ] );
    temp.push_back( m[ 3 ][ 1 ] );
    temp.push_back( m[ 3 ][ 2 ] );
    temp.push_back( m[ 3 ][ 3 ] );
  }
  return temp;
}

nlohmann::json glm_to_json( const glm::mat3 &m ) {
  auto temp = nlohmann::json::array();
  {
    temp.push_back( m[ 0 ][ 0 ] );
    temp.push_back( m[ 0 ][ 1 ] );
    temp.push_back( m[ 0 ][ 2 ] );
    temp.push_back( m[ 1 ][ 0 ] );
    temp.push_back( m[ 1 ][ 1 ] );
    temp.push_back( m[ 1 ][ 2 ] );
    temp.push_back( m[ 2 ][ 0 ] );
    temp.push_back( m[ 2 ][ 1 ] );
    temp.push_back( m[ 2 ][ 2 ] );
  }
  return temp;
}

nlohmann::json glm_to_json( const glm::mat2 &m ) {
  auto temp = nlohmann::json::array();
  {
    temp.push_back( m[ 0 ][ 0 ] );
    temp.push_back( m[ 0 ][ 1 ] );
    temp.push_back( m[ 1 ][ 0 ] );
    temp.push_back( m[ 1 ][ 1 ] );
  }
  return temp;
}

nlohmann::json glm_to_json( const glm::vec4 &v ) {
  auto temp = nlohmann::json::array();
  {
    temp.push_back( v[ 0 ] );
    temp.push_back( v[ 1 ] );
    temp.push_back( v[ 2 ] );
    temp.push_back( v[ 3 ] );
  }
  return temp;
}

nlohmann::json glm_to_json( const glm::vec3 &v ) {
  auto temp = nlohmann::json::array();
  {
    temp.push_back( v[ 0 ] );
    temp.push_back( v[ 1 ] );
    temp.push_back( v[ 2 ] );
  }
  return temp;
}

nlohmann::json glm_to_json( const glm::vec2 &v ) {
  auto temp = nlohmann::json::array();
  {
    temp.push_back( v[ 0 ] );
    temp.push_back( v[ 1 ] );
  }
  return temp;
}

}
