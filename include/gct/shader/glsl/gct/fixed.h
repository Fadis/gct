#ifndef GCT_SHADER_FIXED_H
#define GCT_SHADER_FIXED_H

float decode_fixed( int v ) {
  return float( v ) * 6.103515625e-05f;
}

int encode_fixed( float v ) {
  return int( v / 6.103515625e-05 );
}

#endif

