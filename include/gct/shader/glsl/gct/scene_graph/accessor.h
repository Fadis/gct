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


// ストレージバッファから頂点の情報を1つ読む
vec4 read_vertex( accessor_type a, uint i, vec4 d ) {
  // 情報がなかったらデフォルト値を返す
  if( a.enabled == 0 ) return d;
  // float型の場合
  else if( a.type == GCT_SHADER_TYPE_ID_FLOAT ) {
    vec4 v = vec4(
      vertex_buffer_f32[ a.vertex_buffer ].data[ a.offset + i * a.stride ],
      ( a.component_count >= 2u ) ? vertex_buffer_f32[ a.vertex_buffer ].data[ a.offset + i * a.stride + 1u ] : d.y,
      ( a.component_count >= 3u ) ? vertex_buffer_f32[ a.vertex_buffer ].data[ a.offset + i * a.stride + 2u ] : d.z,
      ( a.component_count >= 4u ) ? vertex_buffer_f32[ a.vertex_buffer ].data[ a.offset + i * a.stride + 3u ] : d.w
    );
    return v;
  }
  // 32bit整数型の場合
  else if( a.type == GCT_SHADER_TYPE_ID_U32 ) {
    const float scale = ( a.normalized == 0 ) ? 1.0 : 1.0/float(0xFFFFFFFFu);
    return vec4(
      vertex_buffer_f32[ a.vertex_buffer ].data[ a.offset + i * a.stride ] * scale,
      ( a.component_count >= 2u ) ? vertex_buffer_u32[ a.vertex_buffer ].data[ a.offset + i * a.stride + 1u ] * scale : d.y,
      ( a.component_count >= 3u ) ? vertex_buffer_u32[ a.vertex_buffer ].data[ a.offset + i * a.stride + 2u ] * scale : d.z,
      ( a.component_count >= 4u ) ? vertex_buffer_u32[ a.vertex_buffer ].data[ a.offset + i * a.stride + 3u ] * scale : d.w
    );
  }
  // 16bit整数型の場合
  else if( a.type == GCT_SHADER_TYPE_ID_U16 ) {
    const float scale = ( a.normalized == 0 ) ? 1.0 : 1.0/float(0xFFFFu);
    return vec4(
      uint( vertex_buffer_u16[ a.vertex_buffer ].data[ a.offset + i * a.stride ] ) * scale,
      ( a.component_count >= 2u ) ? uint( vertex_buffer_u16[ a.vertex_buffer ].data[ a.offset + i * a.stride + 1u ] ) * scale : d.y,
      ( a.component_count >= 3u ) ? uint( vertex_buffer_u16[ a.vertex_buffer ].data[ a.offset + i * a.stride + 2u ] ) * scale : d.z,
      ( a.component_count >= 4u ) ? uint( vertex_buffer_u16[ a.vertex_buffer ].data[ a.offset + i * a.stride + 3u ] ) * scale : d.w
    );
  }
  // 8bit整数型の場合
  else if( a.type == GCT_SHADER_TYPE_ID_U8 ) {
    const float scale = ( a.normalized == 0 ) ? 1.0 : 1.0/float(0xFFu);
    return vec4(
      uint( vertex_buffer_u8[ a.vertex_buffer ].data[ a.offset + i * a.stride ] ) * scale,
      ( a.component_count >= 2u ) ? uint( vertex_buffer_u8[ a.vertex_buffer ].data[ a.offset + i * a.stride + 1u ] ) * scale : d.y,
      ( a.component_count >= 3u ) ? uint( vertex_buffer_u8[ a.vertex_buffer ].data[ a.offset + i * a.stride + 2u ] ) * scale : d.z,
      ( a.component_count >= 4u ) ? uint( vertex_buffer_u8[ a.vertex_buffer ].data[ a.offset + i * a.stride + 3u ] ) * scale : d.w
    );
  }
  return d;
}

void write_vertex( accessor_type a, uint i, vec4 v ) {
  if( a.enabled == 0 ) return;
  else if( a.type == GCT_SHADER_TYPE_ID_FLOAT ) {
    vertex_buffer_f32[ a.vertex_buffer ].data[ a.offset + i * a.stride ] = v.x;
    if( a.component_count >= 2u ) {
      vertex_buffer_f32[ a.vertex_buffer ].data[ a.offset + i * a.stride + 1u ] = v.y;
    }
    if( a.component_count >= 3u ) {
      vertex_buffer_f32[ a.vertex_buffer ].data[ a.offset + i * a.stride + 2u ] = v.z;
    }
    if( a.component_count >= 4u ) {
      vertex_buffer_f32[ a.vertex_buffer ].data[ a.offset + i * a.stride + 3u ] = v.w;
    }
  }
  else if( a.type == GCT_SHADER_TYPE_ID_U32 ) {
    const float scale = ( a.normalized == 0 ) ? 1.0 : float(0xFFFFFFFFu);
    vertex_buffer_u32[ a.vertex_buffer ].data[ a.offset + i * a.stride ] = uint( v.x * scale );
    if( a.component_count >= 2u ) {
      vertex_buffer_u32[ a.vertex_buffer ].data[ a.offset + i * a.stride + 1u ] = uint( v.y * scale );
    }
    if( a.component_count >= 3u ) {
      vertex_buffer_u32[ a.vertex_buffer ].data[ a.offset + i * a.stride + 2u ] = uint( v.z * scale );
    }
    if( a.component_count >= 4u ) {
      vertex_buffer_u32[ a.vertex_buffer ].data[ a.offset + i * a.stride + 3u ] = uint( v.w * scale );
    }
  }
  else if( a.type == GCT_SHADER_TYPE_ID_U16 ) {
    const float scale = ( a.normalized == 0 ) ? 1.0 : float(0xFFFFu);
    vertex_buffer_u16[ a.vertex_buffer ].data[ a.offset + i * a.stride ] = uint16_t( uint( v.x * scale ) );
    if( a.component_count >= 2u ) {
      vertex_buffer_u16[ a.vertex_buffer ].data[ a.offset + i * a.stride + 1u ] = uint16_t( uint( v.y * scale ) );
    }
    if( a.component_count >= 3u ) {
      vertex_buffer_u16[ a.vertex_buffer ].data[ a.offset + i * a.stride + 2u ] = uint16_t( uint( v.z * scale ) );
    }
    if( a.component_count >= 4u ) {
      vertex_buffer_u16[ a.vertex_buffer ].data[ a.offset + i * a.stride + 3u ] = uint16_t( uint( v.w * scale ) );
    }
  }
  else if( a.type == GCT_SHADER_TYPE_ID_U8 ) {
    const float scale = ( a.normalized == 0 ) ? 1.0 : float(0xFFu);
    vertex_buffer_u8[ a.vertex_buffer ].data[ a.offset + i * a.stride ] = uint8_t( uint( v.x * scale ) );
    if( a.component_count >= 2u ) {
      vertex_buffer_u8[ a.vertex_buffer ].data[ a.offset + i * a.stride + 1u ] = uint8_t( uint( v.y * scale ) );
    }
    if( a.component_count >= 3u ) {
      vertex_buffer_u8[ a.vertex_buffer ].data[ a.offset + i * a.stride + 2u ] = uint8_t( uint( v.z * scale ) );
    }
    if( a.component_count >= 4u ) {
      vertex_buffer_u8[ a.vertex_buffer ].data[ a.offset + i * a.stride + 3u ] = uint8_t( uint( v.w * scale ) );
    }
  }
}

struct vertex_attribute {
  vec4 position;
  vec4 normal;
  vec4 tangent;
  vec4 tex_coord0;
  vec4 color0;
  vec4 joint0;
  vec4 weight0;
  vec4 lod_morph;
};


// 1つの頂点の情報を全て読む
vertex_attribute read_vertex_attribute( mesh_type mesh, uint i ) {
  // 頂点インデックスがある場合はインデックスを変換する
  const uint vertex_index = read_index( accessor_pool[ mesh.accessor + 0 ], i );
  vertex_attribute v;
  // 頂点の情報を読む
  v.position = read_vertex( accessor_pool[ mesh.accessor + 1 ], vertex_index, vec4( 0.0, 0.0, 0.0, 1.0 ) );
  v.normal = read_vertex( accessor_pool[ mesh.accessor + 2 ], vertex_index, vec4( 0.0, 0.0, 0.0, 1.0 ) );
  v.tangent = read_vertex( accessor_pool[ mesh.accessor + 3 ], vertex_index, vec4( 0.0, 0.0, 0.0, 1.0 ) );
  v.tex_coord0 = read_vertex( accessor_pool[ mesh.accessor + 4 ], vertex_index, vec4( 0.0, 0.0, 0.0, 1.0 ) );
  v.color0 = read_vertex( accessor_pool[ mesh.accessor + 5 ], vertex_index, vec4( 1.0, 1.0, 1.0, 1.0 ) );
  v.joint0 = read_vertex( accessor_pool[ mesh.accessor + 6 ], vertex_index, vec4( 0.0, 0.0, 0.0, 0.0 ) );
  v.weight0 = read_vertex( accessor_pool[ mesh.accessor + 7 ], vertex_index, vec4( 0.0, 0.0, 0.0, 0.0 ) );
  v.lod_morph = read_vertex( accessor_pool[ mesh.accessor + 8 ], vertex_index, vec4( 0.0, 0.0, 0.0, 1.0 ) );
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
  vec4( 0, 0, 0, 0 ),
  vec4( 0, 0, 0, 1 )
);

#endif

