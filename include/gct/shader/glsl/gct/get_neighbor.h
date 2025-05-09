#ifndef GCT_SHADER_GET_NEIGHBOR_H
#define GCT_SHADER_GET_NEIGHBOR_H

const ivec3 neighbor_offset_3d[26] = ivec3[](
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
);

ivec3 get_neighbor( ivec3 pos, uint d, uint i ) {
  return pos + neighbor_offset_3d[ i ] * int( d );
}

const ivec2 neighbor_offset_2d[8] = ivec2[](
  ivec2(   0, + 1 ),
  ivec2(   0, - 1 ),
  ivec2( + 1,   0 ),
  ivec2( + 1, + 1 ),
  ivec2( + 1, - 1 ),
  ivec2( - 1,   0 ),
  ivec2( - 1, + 1 ),
  ivec2( - 1, - 1 )
);

ivec2 get_neighbor( ivec2 pos, uint d, uint i ) {
  return pos + neighbor_offset_2d[ i ] * int( d );
}

#endif

