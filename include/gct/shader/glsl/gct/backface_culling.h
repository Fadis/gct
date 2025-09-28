#ifndef GCT_SHADER_BACKFACE_CULLING_H
#define GCT_SHADER_BACKFACE_CULLING_H

#define GCT_ENABLE_8BIT_16BIT_STORAGE
#define GCT_USE_IMAGE_POOL_WITHOUT_FORMAT
#include <gct/scene_graph.h>
#include <gct/scene_graph/accessor.h>

bool backface_culling(
  vec4 wp0,
  vec4 wp1,
  vec4 wp2,
  vec3 eye_pos
) {
  const vec3 fn = normalize( cross( normalize( wp2.xyz - wp0.xyz ), normalize( wp1.xyz - wp0.xyz ) ) );
  return dot( fn.xyz, normalize( eye_pos - wp0.xyz ) ) >= 0;
}

bool backface_culling(
  mesh_type mesh,
  uint primitive_id,
  mat4 l2w,
  vec3 eye_pos
) {
  const vec4 wp0 = l2w * read_vertex_position( mesh, primitive_id * 3u + 0u );
  const vec4 wp1 = l2w * read_vertex_position( mesh, primitive_id * 3u + 1u );
  const vec4 wp2 = l2w * read_vertex_position( mesh, primitive_id * 3u + 2u );
  return backface_culling( wp0, wp1, wp2, eye_pos );
}

#endif

