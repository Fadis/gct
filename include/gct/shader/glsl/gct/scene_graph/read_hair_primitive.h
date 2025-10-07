#ifndef GCT_SHADER_SCENE_GRAPH_READ_HAIR_PRIMITIVE_H
#define GCT_SHADER_SCENE_GRAPH_READ_HAIR_PRIMITIVE_H

#include <gct/primitive_value.h>
#include <gct/scene_graph/primitive_resource_index.h>
#include <gct/scene_graph/texture_pool.h>

primitive_value read_hair_fin_primitive(
  uint primitive_id,
  vec4 vert_position,
  vec3 vert_tangent,
  vec2 vert_texcoord,
  vec4 vert_optflow,
  vec4 vert_previous_position,
  uint fin_texture
) {
  const primitive_resource_index_type prim =
    primitive_resource_index[ primitive_id ];
  const vec3 tangent_ = normalize( vert_tangent.xyz );

  const vec3 pos = vert_position.xyz;
  vec4 albedo =
    texture( texture_pool[ nonuniformEXT(fin_texture) ], vert_texcoord );
  const vec3 emissive =
    ( prim.emissive_texture != 0 ) ?
    texture( texture_pool[ nonuniformEXT(prim.emissive_texture) ], vert_texcoord ).rgb :
    prim.emissive.rgb;
  float metallic;
  float roughness;
  if( prim.metallic_roughness_texture != 0 ) {
    vec4 mr = texture( texture_pool[ nonuniformEXT(prim.metallic_roughness_texture) ], vert_texcoord );
    metallic = mr.b;
    roughness = mr.g;
  }
  else {
    metallic = prim.metallic;
    roughness = prim.roughness;
  }
  float occlusion =
    ( prim.occlusion_texture != 0 ) ?
    mix( 1 - prim.occlusion_strength, 1, texture( texture_pool[ nonuniformEXT(prim.occlusion_texture) ], vert_texcoord ).r ) :
    1.0;
  const vec4 screen_pos = vert_optflow;
  const vec3 screen_pos_ = screen_pos.xyz / screen_pos.w;
  const vec4 previous_screen_pos = vert_previous_position;
  const vec3 previous_screen_pos_ = previous_screen_pos.xyz / previous_screen_pos.w;
  const vec3 optflow = screen_pos_.xyz - previous_screen_pos_.xyz;

  primitive_value temp;
  temp.pos = pos;
  temp.albedo = albedo;
  temp.emissive = emissive;
  temp.metallic = metallic;
  temp.roughness = roughness;
  temp.occlusion = occlusion;
  temp.optflow = optflow;
  temp.tangent = tangent_;
  temp.texcoord[ 0 ] = vert_texcoord;
  return temp;
}

#endif
