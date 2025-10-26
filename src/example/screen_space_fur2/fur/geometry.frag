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
#include <gct/scene_graph.h>
#include <gct/global_uniforms.h>
#include <gct/random.h>

layout(push_constant) uniform PushConstants {
  uint offset;
  uint count;
  uint gbuffer_format;
  uint gbuffer;
  uint position;
} push_constants;

void main() {
  primitive_value p = read_primitive(
    uint( input_id.y ),
    input_position,
    input_normal,
    input_tangent,
    input_texcoord,
    input_optflow,
    input_previous_position
  );
  
  if( p.albedo.a <= 0.0 ) discard;

  p.normal =
    normalize(
      p.normal +
      vec3(
        rand1( input_position.xy ) * 0.4 - 0.2,
        rand1( input_position.yz ) * 0.4 - 0.2,
        rand1( input_position.zx ) * 0.4 - 0.2
      )
    );

  p.occlusion = (0.5 + rand1( input_position.xz ) * 0.5 );

  const uint visibility_index = instance_resource_index[ uint( input_id.x ) ].visibility;
  visibility_pool[ visibility_index ] = 1;

  const ivec2 image_pos = ivec2( gl_FragCoord.x, gl_FragCoord.y );
  beginInvocationInterlockARB();
  kplus_iter iter = kplus_begin(
    kplus_image( push_constants.gbuffer, push_constants.position ),
    image_pos,
    push_constants.gbuffer_format
  );
  kplus_insert(
    iter,
    p,
    gl_FragCoord.z,
    input_id.xy
  );
  endInvocationInterlockARB();
}

