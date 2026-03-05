#ifndef GCT_SHADER_SCENE_GRAPH_IMAGE_METADATA_TYPE_H
#define GCT_SHADER_SCENE_GRAPH_IMAGE_METADATA_TYPE_H

#include <gct/color_space.h>
#include <gct/scene_graph/matrix_pool.h>

struct image_metadata_type {
  color_profile_type color_profile;
  color_space_matrix_type color_space_matrix;
};

vec3 from_color_profile( image_metadata_type m, vec3 color ) {
  if( m.color_profile.space == GCT_SHADER_COLOR_SPACE_CIE_XYZ ) return color;
  return ( matrix_pool[ m.color_space_matrix.from ] * vec4( color, 1.0 ) ).xyz;
}

vec4 from_color_profile( image_metadata_type m, vec4 color ) {
  if( m.color_profile.space == GCT_SHADER_COLOR_SPACE_CIE_XYZ ) return color;
  return vec4( ( matrix_pool[ m.color_space_matrix.from ] * vec4( color.xyz, 1.0 ) ).xyz, color.w );
}

vec3 to_color_profile( image_metadata_type m, vec3 color ) {
  if( m.color_profile.space == GCT_SHADER_COLOR_SPACE_CIE_XYZ ) return color;
  return ( matrix_pool[ m.color_space_matrix.to ] * vec4( color, 1.0 ) ).xyz;
}

vec4 to_color_profile( image_metadata_type m, vec4 color ) {
  if( m.color_profile.space == GCT_SHADER_COLOR_SPACE_CIE_XYZ ) return color;
  return vec4( ( matrix_pool[ m.color_space_matrix.to ] * vec4( color.xyz, 1.0 ) ).xyz, color.w );
}

#endif

