#ifndef GCT_SHADER_AABB_TYPE_H
#define GCT_SHADER_AABB_TYPE_H

struct aabb_type {
  vec4 min;
  vec4 max;
};

vec4 get_aabb_vertex( aabb_type aabb, uint i ) {
  return vec4(
    ( ( i & 4 ) != 0 ) ? aabb.max.x : aabb.min.x,
    ( ( i & 2 ) != 0 ) ? aabb.max.y : aabb.min.y,
    ( ( i & 1 ) != 0 ) ? aabb.max.z : aabb.min.z,
    1.0
  );
}

aabb_type transform_aabb( mat4 m, aabb_type b ) {
  aabb_type result;
  vec4 p = m * get_aabb_vertex( b, 0 );
  p /= p.w;
  result.min = p;
  result.max = p;
  for( uint i = 1u; i != 8u; ++i ) {
    p = m * get_aabb_vertex( b, i );
    p /= p.w;
    result.min = min( result.min, p );
    result.max = max( result.max, p );
  }
  return result;
}

aabb_type clip_aabb( aabb_type b ) {
  return aabb_type(
    max( b.min, vec4( -1, -1, -1, 1 ) ),
    min( b.max, vec4(  1,  1,  1, 1 ) )
  );
}

#endif

