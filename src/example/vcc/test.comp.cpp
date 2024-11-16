#include <shady.h>

using namespace vcc;

extern "C"
compute_shader local_size(256, 1, 1)
void main( float *a, float *b ) {
  a[ gl_GlobalInvocationID.x ] =
    b[ gl_GlobalInvocationID.x ] +
    b[ gl_GlobalInvocationID.x + 1 ];
}

