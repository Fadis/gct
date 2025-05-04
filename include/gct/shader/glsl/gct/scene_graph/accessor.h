#ifndef GCT_SHADER_SCENE_GRAPH_ACCESSOR_H
#define GCT_SHADER_SCENE_GRAPH_ACCESSOR_H
#include <gct/scene_graph/vertex_buffer_pool.h>
#include <gct/scene_graph/mesh_type.h>
#include <gct/scene_graph/accessor_type.h>
#include <gct/scene_graph/accessor_pool.h>
#include <gct/type_id.h>

uint read_index( accessor_type a, uint i ) {
  if( a.enabled == 0 ) return i;
  else if( a.type == GCT_SHADER_TYPE_ID_U32 ) {
    return vertex_buffer_u32[ a.vertex_buffer ].data[ a.offset + i * a.stride ];
  }
  else if( a.type == GCT_SHADER_TYPE_ID_U16 ) {
    uint index = uint( vertex_buffer_u16[ a.vertex_buffer ].data[ a.offset + i * a.stride ] );
    return index;
  }
  else if( a.type == GCT_SHADER_TYPE_ID_U8 ) {
    return uint( vertex_buffer_u8[ a.vertex_buffer ].data[ a.offset + i * a.stride ] );
  }
  return i;
}

vec4 read_vertex( accessor_type a, uint i, vec4 d ) {
  if( a.enabled == 0 ) return d;
  else if( a.type == GCT_SHADER_TYPE_ID_FLOAT ) {
    vec4 v = vec4(
      vertex_buffer_f32[ a.vertex_buffer ].data[ a.offset + i * a.stride ],
      ( a.component_count >= 2u ) ? vertex_buffer_f32[ a.vertex_buffer ].data[ a.offset + i * a.stride + 1u ] : d.y,
      ( a.component_count >= 3u ) ? vertex_buffer_f32[ a.vertex_buffer ].data[ a.offset + i * a.stride + 2u ] : d.z,
      ( a.component_count >= 4u ) ? vertex_buffer_f32[ a.vertex_buffer ].data[ a.offset + i * a.stride + 3u ] : d.w
    );
    return v;
  }
  else if( a.type == GCT_SHADER_TYPE_ID_U32 ) {
    const float scale = ( a.normalized == 0 ) ? 1.0 : 1.0/float(0xFFFFFFFFu);
    return vec4(
      vertex_buffer_f32[ a.vertex_buffer ].data[ a.offset + i * a.stride ] * scale,
      ( a.component_count >= 2u ) ? vertex_buffer_u32[ a.vertex_buffer ].data[ a.offset + i * a.stride + 1u ] * scale : d.y,
      ( a.component_count >= 3u ) ? vertex_buffer_u32[ a.vertex_buffer ].data[ a.offset + i * a.stride + 2u ] * scale : d.z,
      ( a.component_count >= 4u ) ? vertex_buffer_u32[ a.vertex_buffer ].data[ a.offset + i * a.stride + 3u ] * scale : d.w
    );
  }
  else if( a.type == GCT_SHADER_TYPE_ID_U16 ) {
    const float scale = ( a.normalized == 0 ) ? 1.0 : 1.0/float(0xFFFFu);
    return vec4(
      vertex_buffer_f32[ a.vertex_buffer ].data[ a.offset + i * a.stride ] * scale,
      ( a.component_count >= 2u ) ? vertex_buffer_u32[ a.vertex_buffer ].data[ a.offset + i * a.stride + 1u ] * scale : d.y,
      ( a.component_count >= 3u ) ? vertex_buffer_u32[ a.vertex_buffer ].data[ a.offset + i * a.stride + 2u ] * scale : d.z,
      ( a.component_count >= 4u ) ? vertex_buffer_u32[ a.vertex_buffer ].data[ a.offset + i * a.stride + 3u ] * scale : d.w
    );
  }
  else if( a.type == GCT_SHADER_TYPE_ID_U8 ) {
    const float scale = ( a.normalized == 0 ) ? 1.0 : 1.0/float(0xFFu);
    return vec4(
      vertex_buffer_f32[ a.vertex_buffer ].data[ a.offset + i * a.stride ] * scale,
      ( a.component_count >= 2u ) ? vertex_buffer_u32[ a.vertex_buffer ].data[ a.offset + i * a.stride + 1u ] * scale : d.y,
      ( a.component_count >= 3u ) ? vertex_buffer_u32[ a.vertex_buffer ].data[ a.offset + i * a.stride + 2u ] * scale : d.z,
      ( a.component_count >= 4u ) ? vertex_buffer_u32[ a.vertex_buffer ].data[ a.offset + i * a.stride + 3u ] * scale : d.w
    );
  }
  return d;
}

struct vertex_attribute {
  vec4 position;
  vec4 normal;
  vec4 tangent;
  vec4 tex_coord0;
  vec4 color0;
  vec4 joint0;
  vec4 weight0;
};


vertex_attribute read_vertex_attribute( mesh_type mesh, uint i ) {
  const uint vertex_index = read_index( accessor_pool[ mesh.accessor + 0 ], i );
  vertex_attribute v;
  v.position = read_vertex( accessor_pool[ mesh.accessor + 1 ], vertex_index, vec4( 0.1, 0.2, 0.3, 1.0 ) );
  v.normal = read_vertex( accessor_pool[ mesh.accessor + 2 ], vertex_index, vec4( 0.0, 0.0, 0.0, 1.0 ) );
  v.tangent = read_vertex( accessor_pool[ mesh.accessor + 3 ], vertex_index, vec4( 0.0, 0.0, 0.0, 1.0 ) );
  v.tex_coord0 = read_vertex( accessor_pool[ mesh.accessor + 4 ], vertex_index, vec4( 0.0, 0.0, 0.0, 1.0 ) );
  v.color0 = read_vertex( accessor_pool[ mesh.accessor + 5 ], vertex_index, vec4( 1.0, 1.0, 1.0, 1.0 ) );
  v.joint0 = read_vertex( accessor_pool[ mesh.accessor + 6 ], vertex_index, vec4( 0.0, 0.0, 0.0, 0.0 ) );
  v.weight0 = read_vertex( accessor_pool[ mesh.accessor + 7 ], vertex_index, vec4( 0.0, 0.0, 0.0, 0.0 ) );
  return v; 
}

vec4 read_vertex_position( mesh_type mesh, uint i ) {
  const uint vertex_index = read_index( accessor_pool[ mesh.accessor + 0 ], i );
  return read_vertex( accessor_pool[ mesh.accessor + 1 ], vertex_index, vec4( 0.1, 0.2, 0.3, 1.0 ) );
}

const vertex_attribute null_attr = vertex_attribute(
  vec4( 0, 0, 0, 1 ),
  vec4( 0, 0, 0, 1 ),
  vec4( 0, 0, 0, 1 ),
  vec4( 0, 0, 0, 1 ),
  vec4( 0, 0, 0, 1 ),
  vec4( 0, 0, 0, 0 ),
  vec4( 0, 0, 0, 0 )
);

#endif

