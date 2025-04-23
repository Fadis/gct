#ifndef GCT_SHADER_SCENE_GRAPH_ACCESSOR_TYPE_H
#define GCT_SHADER_SCENE_GRAPH_ACCESSOR_TYPE_H

#include <gct/type_id.h>

struct accessor_type {
  uint vertex_buffer_id;
  uint type;
  uint normalized;
  uint length;
  uint offset;
  uint stride;
  uint reserved[ 2 ];
};

#endif

