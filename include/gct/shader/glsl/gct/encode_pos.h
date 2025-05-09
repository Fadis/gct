#ifndef GCT_SHADER_ENCODE_POS_H
#define GCT_SHADER_ENCODE_POS_H

ivec3 decode_pos_3d( uint v ) {
  return ivec3(
    ( v >> 20 ) & 0x3FF,
    ( v >> 10 ) & 0x3FF,
    ( v ) & 0x3FF
  );
}

uint encode_pos_3d( ivec3 v ) {
  return
    ( v.x << 20 ) |
    ( v.y << 10 ) |
    v.z;
}

ivec2 decode_pos_2d( uint v ) {
  return ivec2(
    ( v >> 15 ) & 0x7fff,
    v & 0x7fff
  );
}

uint encode_pos_2d( ivec2 v ) {
  return
    ( v.x << 15 ) |
    v.y;
}

bool decode_pos_empty( uint v ) {
  return ( v >> 30 ) != 0;
}

uint encode_pos_empty() {
  return 0x40000000;
}

#endif

