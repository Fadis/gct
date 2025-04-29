#ifndef GCT_SHADER_SCENE_GRAPH_ACCESSOR_TYPE_H
#define GCT_SHADER_SCENE_GRAPH_ACCESSOR_TYPE_H

#include <gct/type_id.h>

struct accessor_type {
  uint enabled;
  uint vertex_buffer;
  uint type;
  uint normalized;
  uint component_count;
  uint offset;
  uint stride;
  uint reserved;
};

#endif

