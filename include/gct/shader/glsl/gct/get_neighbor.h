#ifndef GCT_SHADER_GET_NEIGHBOR_H
#define GCT_SHADER_GET_NEIGHBOR_H

#ifdef __cplusplus
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
using namespace glm;
#endif

const ivec3 neighbor_offset_3d[26] =
#ifdef __cplusplus
{
#else
ivec3[](
#endif
  ivec3(   0,   0, + 1 ),
  ivec3(   0,   0, - 1 ),
  ivec3(   0, + 1,   0 ),
  ivec3(   0, + 1, + 1 ),
  ivec3(   0, + 1, - 1 ),
  ivec3(   0, - 1,   0 ),
  ivec3(   0, - 1, + 1 ),
  ivec3(   0, - 1, - 1 ),
  ivec3( + 1,   0,   0 ),
  ivec3( + 1,   0, + 1 ),
  ivec3( + 1,   0, - 1 ),
  ivec3( + 1, + 1,   0 ),
  ivec3( + 1, + 1, + 1 ),
  ivec3( + 1, + 1, - 1 ),
  ivec3( + 1, - 1,   0 ),
  ivec3( + 1, - 1, + 1 ),
  ivec3( + 1, - 1, - 1 ),
  ivec3( - 1,   0,   0 ),
  ivec3( - 1,   0, + 1 ),
  ivec3( - 1,   0, - 1 ),
  ivec3( - 1, + 1,   0 ),
  ivec3( - 1, + 1, + 1 ),
  ivec3( - 1, + 1, - 1 ),
  ivec3( - 1, - 1,   0 ),
  ivec3( - 1, - 1, + 1 ),
  ivec3( - 1, - 1, - 1 )
#ifdef __cplusplus
};
#else
);
#endif

ivec3 get_neighbor( ivec3 pos, uint d, uint i ) {
  return pos + neighbor_offset_3d[ i ] * int( d );
}

const ivec2 neighbor_offset_2d[8] =
#ifdef __cplusplus
{
#else
ivec2[](
#endif
  ivec2(   0, + 1 ),
  ivec2(   0, - 1 ),
  ivec2( + 1,   0 ),
  ivec2( + 1, + 1 ),
  ivec2( + 1, - 1 ),
  ivec2( - 1,   0 ),
  ivec2( - 1, + 1 ),
  ivec2( - 1, - 1 )
#ifdef __cplusplus
};
#else
);
#endif

ivec2 get_neighbor( ivec2 pos, uint d, uint i ) {
  return pos + neighbor_offset_2d[ i ] * int( d );
}

#endif

