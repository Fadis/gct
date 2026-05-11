#ifndef GCT_VERTEX_ATTRIBUTE_HPP
#define GCT_VERTEX_ATTRIBUTE_HPP

#include <cstdint>
#include <vector>
#include <glm/vec4.hpp>
#include <gct/setter.hpp>
#include <gct/array_of.hpp>

namespace gct {
struct vertex_attribute {
  LIBGCT_SETTER( index )
  LIBGCT_SETTER( position )
  LIBGCT_SETTER( normal )
  LIBGCT_SETTER( tangent )
  LIBGCT_SETTER( tex_coord0 )
  LIBGCT_SETTER( color0 )
  LIBGCT_SETTER( joint0 )
  LIBGCT_SETTER( weight0 )
  LIBGCT_SETTER( lod_morph )
  std::uint32_t index = 0u;
  glm::vec4 position = glm::vec4{ 0.0f, 0.0f, 0.0f, 1.0f };
  glm::vec4 normal = glm::vec4{ 0.0f, 0.0f, 0.0f, 1.0f };
  glm::vec4 tangent = glm::vec4{ 0.0f, 0.0f, 0.0f, 1.0f };
  glm::vec4 tex_coord0 = glm::vec4{ 0.0f, 0.0f, 0.0f, 1.0f };
  glm::vec4 color0 = glm::vec4{ 0.0f, 0.0f, 0.0f, 1.0f };
  glm::vec4 joint0 = glm::vec4{ 0.0f, 0.0f, 0.0f, 1.0f };
  glm::vec4 weight0 = glm::vec4{ 0.0f, 0.0f, 0.0f, 1.0f };
  glm::vec4 lod_morph = glm::vec4{ 0.0f, 0.0f, 0.0f, 1.0f };
};

struct face_attribute {
  LIBGCT_SETTER( vertex )
  LIBGCT_SETTER( primitive_id )
  LIBGCT_SETTER( valid )
  std::vector< vertex_attribute > vertex;
  std::uint32_t primitive_id = 0u;
  bool valid = false;
};

}

#endif

