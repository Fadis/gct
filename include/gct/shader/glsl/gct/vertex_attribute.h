#ifndef GCT_SHADER_VERTEX_ATTRIBUTE_H
#define GCT_SHADER_VERTEX_ATTRIBUTE_H

#ifdef __cplusplus
#include <cstdint>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
using namespace glm;
using namespace std;
using uint = std::uint32_t;
#endif

struct vertex_attribute {
  vec4 position;
  vec4 normal;
  vec4 tangent;
  vec4 tex_coord0;
  vec4 color0;
  vec4 joint0;
  vec4 weight0;
  vec4 lod_morph;
  uint index;
};

struct face_attribute {
  vertex_attribute vertex[ 3 ];
  uint primitive_id;
  bool valid;
};

struct face_position {
  vec4 position[ 3 ];
  uint primitive_id;
  bool valid;
};

struct rasterizable_vertex_attribute {
  vec4 position;
  vec3 normal;
  vec4 tangent;
  vec2 texcoord;
  vec4 optflow;
  vec4 id;
  vec4 previous_position;
};

struct rasterizable_face_attribute {
  rasterizable_vertex_attribute vertex[ 3 ];
};


#endif

