RWStructuredBuffer<float> a;
RWStructuredBuffer<float> b;

template<typename T> T add( T x, T y ) {
  return x + y;
}

[numthreads(256, 1, 1)]
void main( uint2 id : SV_DispatchThreadID ) {
  uint index = id.x;
  a[index] = add(b[index], b[index + 1]);
}

