#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
#extension GL_EXT_nonuniform_qualifier : enable
#extension GL_ARB_fragment_shader_interlock : enable
#extension GL_EXT_shader_image_load_formatted : enable

#include "io_with_tangent.h"
#define GCT_SHADER_SCENE_GRAPH_DISABLE_PUSH_CONSTANT
#define GCT_USE_IMAGE_POOL_WITHOUT_FORMAT
#define GCT_MAKE_IMAGE_COHERENT
#define GCT_USE_GET_LOD_LEVEL
#include <gct/scene_graph.h>
#include <gct/global_uniforms.h>

layout(push_constant) uniform PushConstants {
  uint offset;
  uint count;
  uint gbuffer_format;
  uint gbuffer;
  uint position;
  uint lock;
} push_constants;

void main() {
  const ivec2 image_pos = ivec2( gl_FragCoord.x, gl_FragCoord.y );

  rasterizable_vertex_attribute p;
  p.position = input_position;
  p.normal = input_normal.xyz;
  p.tangent = input_tangent;
  p.texcoord = vec3( input_texcoord, get_lod_level( uint( input_id.y ), input_texcoord.xy ) );

  beginInvocationInterlockARB();
  bool keep_waiting = true;
  while( keep_waiting ) {
    if( imageAtomicExchange( image_pool_2dua[ push_constants.lock ], image_pos, 1u ) != 1u ) {
      kplus_iter iter =
      kplus_begin(
        kplus_image( push_constants.gbuffer, push_constants.position ),
        image_pos,
        push_constants.gbuffer_format,
        0
      );
      kplus_insert_lazy_compress(
        iter,
        p,
        gl_FragCoord.z,
        input_id
      );
      imageAtomicExchange( image_pool_2dua[ push_constants.lock ], image_pos, uint( 0 ) );
      keep_waiting = false;
    }
  }
  endInvocationInterlockARB();
}

