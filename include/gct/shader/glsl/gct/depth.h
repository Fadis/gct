#ifndef GCT_SHADER_DEPTH_H
#define GCT_SHADER_DEPTH_H

float decode_depth(float d,float zNear,float zFar)
{
  return zNear * zFar / (zFar + d * (zNear - zFar));
}

float encode_depth(float d,float zNear,float zFar)
{
  return zFar * ( zNear - d )/( d * ( zNear - zFar ) );
}


vec4 depth_to_world_position(
  uvec2 screen_pos,
  uvec2 image_size,
  float depth,
  mat4 screen_to_world
) {
  vec4 lpos4 = screen_to_world * vec4( vec2( screen_pos )/vec2( image_size ), depth, 1.0 );
  lpos4 /= lpos4.w;
  return lpos4;
}

#endif

