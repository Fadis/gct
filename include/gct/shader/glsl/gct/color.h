#ifndef GCT_SHADER_COLOR_H
#define GCT_SHADER_COLOR_H

#ifdef __cplusplus
#include <glm/vec3.hpp>
using namespace glm;
#endif

vec3 distinguishable_color_palette[ 8 ] =
#ifdef __cplusplus
{
#else
vec3[8](
#endif
vec3(0.240, 0.110, 0.625),
vec3(0.153, 0.257, 0.204),
vec3(0.349, 0.405, 0.831),
vec3(0.450, 0.416, 0.057),
vec3(0.647, 0.744, 0.161),
vec3(0.141, 0.152, 0.443),
vec3(0.329, 0.176, 0.022),
vec3(0.020, 0.021, 0.023)
#ifdef __cplusplus
};
#else
);
#endif

vec3 distinguishable_color( uint index ) {
  return distinguishable_color_palette[ index & 0x7 ];
}

#endif


