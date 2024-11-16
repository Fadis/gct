__kernel void test(__global float *a, __global float *b) {
  const auto index = get_global_id( 0 );
  const auto add = []( auto x, auto y ) __private {
    return x + y;
  };
  a[index] = add( b[index], b[index + 1] );
}

