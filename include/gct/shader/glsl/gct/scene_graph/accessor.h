#ifndef GCT_SHADER_SCENE_GRAPH_ACCESSOR_H
#define GCT_SHADER_SCENE_GRAPH_ACCESSOR_H

#define GCT_ENABLE_8BIT_16BIT_STORAGE
#define GCT_USE_IMAGE_POOL_WITHOUT_FORMAT

#include <gct/scene_graph/vertex_buffer_pool.h>
#include <gct/scene_graph/byte_address_buffer.h>
#include <gct/scene_graph/mesh_type.h>
#include <gct/scene_graph/mesh_pool.h>
#include <gct/scene_graph/accessor_type.h>
#include <gct/scene_graph/accessor_pool.h>
#include <gct/type_id.h>
#include <gct/fixed.h>
#include <gct/dgf.h>
#include <gct/n31.h>
#include <gct/n21t11.h>
#include <gct/n20t11b1.h>
#include <gct/generate_normal.h>
#include <gct/mikktspace.h>

uint read_index( accessor_type a, uint i ) {
  if( a.enabled == 0 ) return i;
  else if( a.type == GCT_SHADER_TYPE_ID_U32 ) {
    return vertex_buffer_load( a.vertex_buffer, ( a.offset + i * a.stride ) << 2 );
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
  // half型の場合
  else if( a.type == GCT_SHADER_TYPE_ID_HALF ) {
    vec4 v = vec4(
      float( vertex_buffer_f16[ a.vertex_buffer ].data[ a.offset + i * a.stride ] ),
      ( a.component_count >= 2u ) ? float( vertex_buffer_f16[ a.vertex_buffer ].data[ a.offset + i * a.stride + 1u ] ) : d.y,
      ( a.component_count >= 3u ) ? float( vertex_buffer_f16[ a.vertex_buffer ].data[ a.offset + i * a.stride + 2u ] ) : d.z,
      ( a.component_count >= 4u ) ? float( vertex_buffer_f16[ a.vertex_buffer ].data[ a.offset + i * a.stride + 3u ] ) : d.w
    );
    return v;
  }
  // fixed型の場合
  else if( a.type == GCT_SHADER_TYPE_ID_FIXED ) {
    vec4 v = vec4(
      decode_fixed( int( vertex_buffer_i16[ a.vertex_buffer ].data[ a.offset + i * a.stride ] ) ),
      ( a.component_count >= 2u ) ? decode_fixed( int( vertex_buffer_i16[ a.vertex_buffer ].data[ a.offset + i * a.stride + 1u ] ) ) : d.y,
      ( a.component_count >= 3u ) ? decode_fixed( int( vertex_buffer_i16[ a.vertex_buffer ].data[ a.offset + i * a.stride + 2u ] ) ) : d.z,
      ( a.component_count >= 4u ) ? decode_fixed( int( vertex_buffer_i16[ a.vertex_buffer ].data[ a.offset + i * a.stride + 3u ] ) ) : d.w
    );
    return v;
  }
  // n31型の場合
  else if( a.type == GCT_SHADER_TYPE_ID_N31 ) {
    const vec3 decoded = n31_decode( vertex_buffer_u32[ a.vertex_buffer ].data[ a.offset + i * a.stride ] );
    vec4 v = vec4( decoded, d.w );
    return v;
  }
  else if( a.type == GCT_SHADER_TYPE_ID_N21T11 ) {
    const vec3 decoded = n21t11_decode_normal( vertex_buffer_u32[ a.vertex_buffer ].data[ a.offset + i * a.stride ] );
    vec4 v = vec4( decoded, d.w );
    return v;
  }
  else if( a.type == GCT_SHADER_TYPE_ID_N20T11B1 ) {
    const vec3 decoded = n20t11b1_decode_normal( vertex_buffer_u32[ a.vertex_buffer ].data[ a.offset + i * a.stride ] );
    vec4 v = vec4( decoded, d.w );
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

const face_attribute null_face_attr = face_attribute(
  vertex_attribute[ 3 ](
    null_attr,
    null_attr,
    null_attr
  ),
  0u,
  false
);

const face_position null_face_pos = face_position(
  vec4[ 3 ](
    vec4( 0, 0, 0, 1 ),
    vec4( 0, 0, 0, 1 ),
    vec4( 0, 0, 0, 1 )
  ),
  0u,
  false
);

struct meshlet_reader {
  uint mesh_id;
  uint meshlet_id;
  uint face_count;
  DGFBlockInfo dgf_info;
  bool wave_mode;
  bool valid;
};

meshlet_reader init_meshlet_reader( uint mesh_id, uint meshlet_id, bool wave_mode ) {
  const mesh_type mesh = mesh_pool[ mesh_id ];
  const uint max_face_count = gl_SubgroupSize;
  meshlet_reader reader;
  reader.mesh_id = mesh_id;
  reader.meshlet_id = meshlet_id;
  if( accessor_pool[ mesh.accessor + 1 ].type == GCT_SHADER_TYPE_ID_DGF ) {
    reader.dgf_info = DGFInit(
      accessor_pool[ mesh.accessor + 1 ].vertex_buffer,
      meshlet_id
    );
    reader.face_count = reader.dgf_info.header.numTriangles;
    reader.wave_mode = wave_mode;
    reader.valid = true;
  }
  else {
    const uint face_count = min( mesh.vertex_count/3u - meshlet_id * max_face_count, max_face_count );
    reader.face_count = face_count;
    reader.valid = mesh.vertex_count/3u > meshlet_id * max_face_count;
  }
  return reader;
}

meshlet_reader init_meshlet_reader( uint mesh_id, uint meshlet_id ) {
  return init_meshlet_reader(
    mesh_id, meshlet_id,
    subgroupAllEqual( mesh_id ) && subgroupAllEqual( meshlet_id )
  );
}

// 1つの頂点の情報を全て読む
vertex_attribute read_vertex_attribute( mesh_type mesh, uint i ) {
  // 頂点インデックスがある場合はインデックスを変換する
  const uint vertex_index = read_index( accessor_pool[ mesh.accessor + 0 ], i );
  vertex_attribute v;
  // 頂点の情報を読む
  v.position = read_vertex( accessor_pool[ mesh.accessor + 1 ], vertex_index, vec4( 0.0, 0.0, 0.0, 1.0 ) );

  const accessor_type normal_accessor = accessor_pool[ mesh.accessor + 2 ];
  v.normal = read_vertex( normal_accessor, vertex_index, vec4( 0.0, 0.0, 0.0, 1.0 ) );
  if( accessor_pool[ mesh.accessor + 3 ].enabled != 0 ) {
    v.tangent = read_vertex( accessor_pool[ mesh.accessor + 3 ], vertex_index, vec4( 0.0, 0.0, 0.0, 1.0 ) );
  }
  else if( normal_accessor.enabled != 0 ) {
    if( normal_accessor.type == GCT_SHADER_TYPE_ID_N21T11 ) {
      v.tangent = vec4( n21t11_decode_tangent( vertex_buffer_u32[ normal_accessor.vertex_buffer ].data[ normal_accessor.offset + vertex_index * normal_accessor.stride ], v.normal.xyz ), 1.0 );
    }
    else if( accessor_pool[ mesh.accessor + 2 ].type == GCT_SHADER_TYPE_ID_N20T11B1 ) {
      v.tangent = n20t11b1_decode_tangent( vertex_buffer_u32[ normal_accessor.vertex_buffer ].data[ normal_accessor.offset + vertex_index * normal_accessor.stride ], v.normal.xyz );
    }
  }
  
  v.tex_coord0 = read_vertex( accessor_pool[ mesh.accessor + 4 ], vertex_index, vec4( 0.0, 0.0, 0.0, 1.0 ) );
  v.color0 = read_vertex( accessor_pool[ mesh.accessor + 5 ], vertex_index, vec4( 1.0, 1.0, 1.0, 1.0 ) );
  v.joint0 = read_vertex( accessor_pool[ mesh.accessor + 6 ], vertex_index, vec4( 0.0, 0.0, 0.0, 0.0 ) );
  v.weight0 = read_vertex( accessor_pool[ mesh.accessor + 7 ], vertex_index, vec4( 0.0, 0.0, 0.0, 0.0 ) );
  v.lod_morph = read_vertex( accessor_pool[ mesh.accessor + 8 ], vertex_index, vec4( 0.0, 0.0, 0.0, 1.0 ) );
  return v; 
}

// 1つの面の情報を全て読む
face_attribute read_face_attribute( meshlet_reader reader, uint face_id ) {
  if( !reader.valid ) return null_face_attr;
  const mesh_type mesh = mesh_pool[ reader.mesh_id ];
  const accessor_type normal_accessor = accessor_pool[ mesh.accessor + 2 ];
  const accessor_type tangent_accessor = accessor_pool[ mesh.accessor + 3 ];

  face_attribute f;
  uvec3 vertex_index;
  if( accessor_pool[ mesh.accessor + 1 ].type == GCT_SHADER_TYPE_ID_DGF ) {
    uvec3 index;
    if( reader.wave_mode ) {
      index = DGFGetTriangle_BitScan_Wave( reader.dgf_info, face_id );
    }
    else {
      index = DGFGetTriangle_BitScan_Lane( reader.dgf_info, face_id );
    }
    for( uint i = 0u; i != 3u; ++i ) {
      f.vertex[ i ].position = vec4( DGFGetVertex( reader.dgf_info, index[ 0 ] ), 1.0f );
    }
    f.primitive_id = reader.dgf_info.header.userData + face_id;
    const uint global_vertex_id = f.primitive_id * 3u;
    vertex_index = uvec3(
      read_index( accessor_pool[ mesh.accessor + 0 ], global_vertex_id + 0u ),
      read_index( accessor_pool[ mesh.accessor + 0 ], global_vertex_id + 1u ),
      read_index( accessor_pool[ mesh.accessor + 0 ], global_vertex_id + 2u )
    );
  }
  else {
    if( face_id >= reader.face_count ) return null_face_attr;
    f.primitive_id = ( reader.meshlet_id * 32u + face_id );
    const uint global_vertex_id = f.primitive_id * 3u;
    vertex_index = uvec3(
      read_index( accessor_pool[ mesh.accessor + 0 ], global_vertex_id + 0u ),
      read_index( accessor_pool[ mesh.accessor + 0 ], global_vertex_id + 1u ),
      read_index( accessor_pool[ mesh.accessor + 0 ], global_vertex_id + 2u )
    );
    for( uint i = 0u; i != 3u; ++i ) {
      f.vertex[ i ].position = read_vertex( accessor_pool[ mesh.accessor + 1 ], vertex_index[ i ], vec4( 0.0, 0.0, 0.0, 1.0 ) );
    }
  }
 
  bool gen_normal = false;
  bool gen_tangent = false;
  for( uint i = 0u; i != 3u; ++i ) {
    if( normal_accessor.enabled != 0 ) {
      f.vertex[ i ].normal = read_vertex( normal_accessor, vertex_index[ i ], vec4( 0.0, 0.0, 0.0, 1.0 ) );
      if( tangent_accessor.enabled != 0 ) {
        f.vertex[ i ].tangent = read_vertex( tangent_accessor, vertex_index[ i ], vec4( 0.0, 0.0, 0.0, 1.0 ) );
      }
      else if( normal_accessor.enabled != 0 ) {
        if( normal_accessor.type == GCT_SHADER_TYPE_ID_N21T11 ) {
          f.vertex[ i ].tangent = vec4( n21t11_decode_tangent( vertex_buffer_u32[ normal_accessor.vertex_buffer ].data[ normal_accessor.offset + vertex_index[ i ] * normal_accessor.stride ], f.vertex[ i ].normal.xyz ), 1.0 );
        }
        else if( normal_accessor.type == GCT_SHADER_TYPE_ID_N20T11B1 ) {
          f.vertex[ i ].tangent = n20t11b1_decode_tangent( vertex_buffer_u32[ normal_accessor.vertex_buffer ].data[ normal_accessor.offset + vertex_index[ i ] * normal_accessor.stride ], f.vertex[ i ].normal.xyz );
        }
        else {
          gen_tangent = true;
        }
      }
      else {
        gen_tangent = true;
      }
    }
    else {
      if( tangent_accessor.enabled != 0 ) {
        f.vertex[ i ].tangent = read_vertex( tangent_accessor, vertex_index[ i ], vec4( 0.0, 0.0, 0.0, 1.0 ) );
      }
      else {
        gen_tangent = true;
      }
      gen_normal = true;
    }

    f.vertex[ i ].tex_coord0 = read_vertex( accessor_pool[ mesh.accessor + 4 ], vertex_index[ i ], vec4( 0.0, 0.0, 0.0, 1.0 ) );
    f.vertex[ i ].color0 = read_vertex( accessor_pool[ mesh.accessor + 5 ], vertex_index[ i ], vec4( 1.0, 1.0, 1.0, 1.0 ) );
    f.vertex[ i ].joint0 = read_vertex( accessor_pool[ mesh.accessor + 6 ], vertex_index[ i ], vec4( 0.0, 0.0, 0.0, 0.0 ) );
    f.vertex[ i ].weight0 = read_vertex( accessor_pool[ mesh.accessor + 7 ], vertex_index[ i ], vec4( 0.0, 0.0, 0.0, 0.0 ) );
    f.vertex[ i ].lod_morph = read_vertex( accessor_pool[ mesh.accessor + 8 ], vertex_index[ i ], vec4( 0.0, 0.0, 0.0, 1.0 ) );
  }

  if( gen_normal ) {
    if(
      f.vertex[ 0 ].position != f.vertex[ 1 ].position &&
      f.vertex[ 0 ].position != f.vertex[ 2 ].position &&
      f.vertex[ 1 ].position != f.vertex[ 2 ].position
    ) {
      const vec3 normal = generate_normal(
        f.vertex[ 0 ].position.xyz,
        f.vertex[ 0 ].position.xyz,
        f.vertex[ 0 ].position.xyz
      );
      f.vertex[ 0 ].normal = vec4( normal, 1.0 );
      f.vertex[ 1 ].normal = vec4( normal, 1.0 );
      f.vertex[ 2 ].normal = vec4( normal, 1.0 );
    }
    else {
      f.vertex[ 0 ].normal = vec4( 0.0, 0.0, 0.0, 1.0 );
      f.vertex[ 1 ].normal = vec4( 0.0, 0.0, 0.0, 1.0 );
      f.vertex[ 2 ].normal = vec4( 0.0, 0.0, 0.0, 1.0 );
    }
  }

  if( gen_tangent ) {
    if(
      f.vertex[ 0 ].position != f.vertex[ 1 ].position &&
      f.vertex[ 0 ].position != f.vertex[ 2 ].position &&
      f.vertex[ 1 ].position != f.vertex[ 2 ].position &&
      f.vertex[ 0 ].tex_coord0 != f.vertex[ 1 ].tex_coord0 &&
      f.vertex[ 0 ].tex_coord0 != f.vertex[ 2 ].tex_coord0 &&
      f.vertex[ 1 ].tex_coord0 != f.vertex[ 2 ].tex_coord0
    ) {
      const vec3 tangent = mikktspace(
        f.vertex[ 0 ].position.xyz,
        f.vertex[ 1 ].position.xyz,
        f.vertex[ 2 ].position.xyz,
        f.vertex[ 0 ].tex_coord0.xy,
        f.vertex[ 1 ].tex_coord0.xy,
        f.vertex[ 2 ].tex_coord0.xy
      );
      f.vertex[ 0 ].tangent = vec4( tangent, 1.0 );
      f.vertex[ 1 ].tangent = vec4( tangent, 1.0 );
      f.vertex[ 2 ].tangent = vec4( tangent, 1.0 );
    }
    else {
      f.vertex[ 0 ].tangent = vec4( 0.0, 0.0, 0.0, 1.0 );
      f.vertex[ 1 ].tangent = vec4( 0.0, 0.0, 0.0, 1.0 );
      f.vertex[ 2 ].tangent = vec4( 0.0, 0.0, 0.0, 1.0 );

    }
  }

  f.valid = true;
  return f; 
}
face_attribute read_face_attribute( meshlet_reader reader ) {
  return read_face_attribute( reader, gl_SubgroupInvocationID );
}

face_position read_face_position( meshlet_reader reader, uint face_id ) {
  const mesh_type mesh = mesh_pool[ reader.mesh_id ];
  if( !reader.valid || face_id >= reader.face_count ) return null_face_pos;
  face_position f;
  f.primitive_id = ( reader.meshlet_id * 32u + face_id );
  const uint global_vertex_id = f.primitive_id * 3u;
  const uint i0 = read_index( accessor_pool[ mesh.accessor + 0 ], global_vertex_id + 0u );
  f.position[ 0 ] = read_vertex( accessor_pool[ mesh.accessor + 1 ], i0, vec4( 0.0, 0.0, 0.0, 1.0 ) );
  const uint i1 = read_index( accessor_pool[ mesh.accessor + 0 ], global_vertex_id + 1u );
  f.position[ 1 ] = read_vertex( accessor_pool[ mesh.accessor + 1 ], i1, vec4( 0.0, 0.0, 0.0, 1.0 ) );
  const uint i2 = read_index( accessor_pool[ mesh.accessor + 0 ], global_vertex_id + 2u );
  f.position[ 2 ] = read_vertex( accessor_pool[ mesh.accessor + 1 ], i2, vec4( 0.0, 0.0, 0.0, 1.0 ) );
  f.valid = true;
  return f; 
}
face_position read_face_position( meshlet_reader reader ) {
  return read_face_position( reader, gl_SubgroupInvocationID );
}



vec4 read_vertex_position( mesh_type mesh, uint i ) {
  const uint vertex_index = read_index( accessor_pool[ mesh.accessor + 0 ], i );
  return read_vertex( accessor_pool[ mesh.accessor + 1 ], vertex_index, vec4( 0.0, 0.0, 0.0, 1.0 ) );
}

vec4 read_vertex_position_by_vertex_id( mesh_type mesh, uint i ) {
  return read_vertex( accessor_pool[ mesh.accessor + 1 ], i, vec4( 0.1, 0.2, 0.3, 1.0 ) );
}

vec3 read_primitive_center( mesh_type mesh, uint primitive_id ) {
  return
    (
      read_vertex_position( mesh, primitive_id * 3u + 0u ).xyz +
      read_vertex_position( mesh, primitive_id * 3u + 1u ).xyz +
      read_vertex_position( mesh, primitive_id * 3u + 2u ).xyz
    ) / 3.0;
}

#endif

