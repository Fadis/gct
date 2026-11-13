#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
#extension GL_EXT_nonuniform_qualifier : enable
#extension GL_ARB_fragment_shader_interlock : enable
#extension GL_EXT_shader_image_load_formatted : enable

#define GCT_SHADER_SCENE_GRAPH_DISABLE_PUSH_CONSTANT
#define GCT_USE_IMAGE_POOL_WITHOUT_FORMAT
#define GCT_MAKE_IMAGE_COHERENT
#include <gct/scene_graph.h>
#include <gct/global_uniforms.h>
#include <gct/simplex_noise.h>
#include <gct/cellular_noise.h>
#include <gct/random.h>

layout(early_fragment_tests) in;

layout (location = 0) in vec4 input_position;
layout (location = 1) in vec3 input_normal;
layout (location = 2) in vec2 input_texcoord;

layout(push_constant) uniform PushConstants {
  uint gbuffer_format;
  uint gbuffer;
  uint position;
  uint height_map;
  uint normal_map;
} push_constants;

void main() {

  float a = texture( texture_pool_2d[ nonuniformEXT( push_constants.height_map ) ], input_texcoord.xy ).r;

  float snow_noise = noise_simplex2d( input_position.xz * 0.005 );
  float grass_noise = noise_simplex2d_fbm5( input_position.xz * 0.0005 ) * 0.5 + 0.5;
  vec2 rockface_noise =
    //noise_cellular2d( input_position.xy * 0.00512 ) *
    //noise_cellular2d( input_position.yz * 0.00564 ) *
    noise_cellular2d( input_position.xz * 0.0131 );

  float snow_level = min( max( ( -input_position.y - 1400.f ) / 200.f + snow_noise, 0.0f ), 1.0f );
  float grass_level = min( max( ( dot( input_normal.xyz, vec3( 0, -1, 0 ) ) - 0.6 ) / 0.1, 0.0f ), 1.0f );


  vec3 albedo =
    snow_level * vec3( 1, 1, 1 ) +
    ( 1.0f - snow_level ) * (
      grass_level * mix( vec3( 0.15, 0.3, 0.05 ), vec3( 0.3, 0.32, 0.05 ), grass_noise ) +
      ( 1.0f - grass_level ) * vec3( 0.3, 0.32, 0.25 )
    );
    // 0.3 0.32 0.2

  /*vec3 albedo = 
    ( input_position.y < -1200.f ) ?
    vec3( 1, 1, 1 ) :
      (
        ( dot( input_normal.xyz, vec3( 0, -1, 0 ) ) > 0.95f ) ?
        vec3( 0.15, 0.3, 0.05 ) :
        vec3( 0.4, 0.35, 0.3 )
      );
*/
  primitive_value p;
  p.pos = input_position.xyz;
  p.normal = normalize(input_normal.xyz);
  p.albedo = vec4( albedo, 1.0f );
  p.emissive = vec3( 0.0f, 0.0f, 0.0f );
  p.metallic = 0.0f;
  p.roughness = 8.0f;
  p.occlusion = 1.0f;
  p.tangent = vec3( 0.0f, 0.0f, 1.0f );

  p.normal =
    normalize(
      p.normal +
      ( 1.0f - snow_level ) * ( grass_level ) *
      vec3(
        rand1( input_position.xy ) * 0.2 - 0.1,
        rand1( input_position.yz ) * 0.2 - 0.1,
        rand1( input_position.zx ) * 0.2 - 0.1
      )
    );

  p.occlusion =
    snow_level * 1.0f +
    ( 1.0f - snow_level ) * (
      grass_level * (0.5 + rand1( input_position.xz ) * 0.5 ) +
      ( 1.0f - grass_level ) * max( -rockface_noise.x + rockface_noise.y, 0.3 )
    );

  p.roughness =
    snow_level * 0.7f +
    ( 1.0f - snow_level ) * ( 8.0 - 8.0 * rand1( input_position.xy ) );

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
    vec4( 0.0f, 0.0f, 0.0f, 0.0f )
  );
  endInvocationInterlockARB();

}

