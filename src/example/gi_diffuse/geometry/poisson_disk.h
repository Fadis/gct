#ifndef GCT_SHADER_POISSON_DISK_H
#define GCT_SHADER_POISSON_DISK_H

int poisson_disk_sample_count = 12;
vec2 poisson_disk[12]=vec2[](
  vec2( 0.0191375, 0.635275 ),
  vec2( 0.396322, 0.873851 ),
  vec2( -0.588224, 0.588251 ),
  vec2( -0.3404, 0.0154557 ),
  vec2( 0.510869, 0.0278614 ),
  vec2( -0.15801, -0.659996 ),
  vec2( 0.120268, -0.200636 ),
  vec2( -0.925312, -0.0306309 ),
  vec2( -0.561635, -0.32798 ),
  vec2( 0.424297, -0.852628 ),
  vec2( 0.923275, -0.191526 ),
  vec2( 0.703181, 0.556563 )
);

#endif

