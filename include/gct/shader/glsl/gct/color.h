#ifndef GCT_SHADER_COLOR_H
#define GCT_SHADER_COLOR_H
/*
vec3 distinguishable_color_palette[ 8 ] = vec3[8](
vec3(0.4876017 , 0.23227803, 0.8869847 ),
vec3(0.76119182, 0.74122304, 1.0),
vec3(0.73004231, 0.8094791 , 1.0),
vec3(0.6820066 , 0.77989407, 0.11461076),
vec3(0.83866277, 0.92561889, 0.64617487),
vec3(0.40123724, 0.55730215, 0.88969337),
vec3(0.5791739 , 0.49337188, 0.33342828),
vec3(0.40949384, 0.43085104, 0.46911341)
);
*/
vec3 distinguishable_color_palette[ 16 ] = vec3[16](
vec3( 0.2, 0, 0 ),
vec3( 0.4, 0, 0 ),
vec3( 0.6, 0, 0 ),
vec3( 0, 0.2, 0 ),
vec3( 0, 0.4, 0 ),
vec3( 0, 0.6, 0 ),
vec3( 0, 0, 0.2 ),
vec3( 0, 0, 0.4 ),
vec3( 0, 0, 0.6 ),
vec3( 0.2, 0.2, 0 ),
vec3( 0.4, 0.4, 0 ),
vec3( 0.6, 0.6, 0 ),
vec3( 0, 0.2, 0.2 ),
vec3( 0, 0.4, 0.4 ),
vec3( 0, 0.6, 0.6 ),
vec3( 0.2, 0, 0.2 )
);

vec3 distinguishable_color( uint index ) {
  return distinguishable_color_palette[ index & 0xF ];
}

#endif


