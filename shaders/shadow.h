layout(binding = 6) uniform sampler2D shadow0;
layout(binding = 8) uniform sampler2D shadow1;
layout(binding = 9) uniform sampler2D shadow2;
layout(binding = 10) uniform sampler2D shadow3;

int poisson_disk_sample_count = 12;
vec2 poisson_disk[12]=vec2[](
  vec2( 0.0191375, 0.635275 ),
  vec2( 0.396322, 0.873851 ),
  vec2( -0.588224, 0.588251 ),
  vec2( -0.3404, 0.0154557 ),
  vec2( 0.510869, 0.0278614 ),
  vec2( -0.15801, -0.659996 ),
  vec2( 0.120268, -0.200636 ),
  vec2( -0.925312, -0.0306309 ),
  vec2( -0.561635, -0.32798 ),
  vec2( 0.424297, -0.852628 ),
  vec2( 0.923275, -0.191526 ),
  vec2( 0.703181, 0.556563 )
);

float rand1( vec2 n ) {
  return fract( sin( dot( n.xy, vec2( 12.9898, 78.233 ) ) ) * 43758.5453 );
}

mat2 get_rotate_matrix2( float angle ) {
  float s = sin( angle );
  float c = cos( angle );
  return mat2( c, s, -s, c );
}

float pcf( vec3 light_proj_pos, float light_size, float frustum_size, float bias ) {
  mat2 rot = get_rotate_matrix2( rand1( light_proj_pos.xy ) * 2 * pi );
  float sum = 0;
  float uv_light_size = light_size / frustum_size;
  float receiver_distance = light_proj_pos.z - bias;
  for( int i = 0; i < poisson_disk_sample_count; i++ ) {
    vec2 offset = ( rot * poisson_disk[ i ] ) * uv_light_size;
    float occluder_distance = texture( shadow0, ( light_proj_pos.xy * 0.5 + 0.5 ) + offset ).r;
    sum += ( occluder_distance < receiver_distance ) ? 1 : 0;
  }
  return sum / poisson_disk_sample_count;
}

float pcss( vec3 light_proj_pos, float light_size, float frustum_size, float bias ) {
  mat2 rot = get_rotate_matrix2( rand1( light_proj_pos.xy ) * 2 * pi );
  float uv_light_size = light_size / frustum_size;
  float p33 = dynamic_uniforms.light_vp_matrix0[ 2 ][ 2 ];
  float p34 = dynamic_uniforms.light_vp_matrix0[ 3 ][ 2 ];
  float receiver_distance = light_proj_pos.z - bias;
  float average_occluder_distance = 0;
  int occluder_count = 0;
  for( int i = 0; i < poisson_disk_sample_count; i++ ) {
    vec2 offset = ( rot * poisson_disk[ i ] ) * uv_light_size;
    float occluder_distance = texture( shadow0, ( light_proj_pos.xy * 0.5 + 0.5 ) + offset ).r;
    if( occluder_distance < receiver_distance ) {
      average_occluder_distance += occluder_distance;
      occluder_count++;
    }
  }
  if( occluder_count == 0 ) return 0.0;
  else average_occluder_distance /= occluder_count;
  float penumbra_width = ( 1/(p33-receiver_distance*p34) - 1/(p33-average_occluder_distance*p34) ) / ( 1/(p33-average_occluder_distance*p34) );
  float uv_filter_size = penumbra_width * uv_light_size;
  float sum = 0;
  for( int i = 0; i < poisson_disk_sample_count; i++ ) {
    vec2 offset = ( rot * poisson_disk[ i ] ) * uv_filter_size;
    float occluder_distance = texture( shadow0, ( light_proj_pos.xy * 0.5 + 0.5 ) + offset ).r;
    sum += ( occluder_distance < receiver_distance ) ? 1 : 0;
  }
  return sum / poisson_disk_sample_count;
}

float vsm( vec3 light_proj_pos, float light_size, float frustum_size, float bias ) {
  float uv_light_size = light_size / frustum_size;
  float p33 = dynamic_uniforms.light_vp_matrix0[ 2 ][ 2 ];
  float p34 = dynamic_uniforms.light_vp_matrix0[ 3 ][ 2 ];
  float receiver_distance = light_proj_pos.z;
  float uv_light_lod = -10.0/log2( uv_light_size );
  float average_occluder_distance = 1.0 - textureLod( shadow0, light_proj_pos.xy * 0.5 + 0.5, uv_light_lod ).x;
  float penumbra_width = ( 1/(p33-receiver_distance*p34) - 1/(p33-average_occluder_distance*p34) ) / ( 1/(p33-average_occluder_distance*p34) );
  float uv_filter_size = penumbra_width * uv_light_size;
  float uv_filter_lod = -10.0/log2( -uv_filter_size );
  vec2 v = textureLod( shadow0, light_proj_pos.xy * 0.5 + 0.5, uv_filter_lod ).xy;
  float d2 = v.x * v.x;
  float variance = v.y - d2;
  float md = receiver_distance - v.x;
  float p = variance / ( variance + ( md * md ) );
  return clamp( 1.0 - p, 0.0, 1.0 );
}

float simple_shadow( vec3 proj_pos, float bias ) {
  float shadow_distance = max( ( texture( shadow0, proj_pos.xy * 0.5 + 0.5 ).r ), 0.0 );
  float distance = proj_pos.z - bias;
  if( shadow_distance < distance ) return 1.0;
  else return 0.0;
}

float logarithmic_split(
  float i,
  float count,
  float near,
  float far
) {
  return near * pow( ( far / near ), ( i / count ) );
}

float practical_split(
  float i,
  float count,
  float near,
  float far,
  float a
) {
 return a * logarithmic_split( i, count, near, far ) + ( 1.0 - a ) * ( near + i / count * ( far - near ) );
}

float pssm( vec3 proj_pos0, vec3 proj_pos1, vec3 proj_pos2, vec3 proj_pos3, float depth_bias ) {
  if( abs( proj_pos0.x ) < 1.0 && abs( proj_pos0.y ) < 1.0 && abs( proj_pos0.z ) < 1.0 ) {
    float shadow_distance = max( ( texture( shadow0, proj_pos0.xy * 0.5 + 0.5 ).r ), 0.0 );
    float distance = proj_pos0.z - depth_bias;
    if( shadow_distance < distance ) return 1.0;
    else return 0.0;
  }
  else if( abs( proj_pos1.x ) < 1.0 && abs( proj_pos1.y ) < 1.0 && abs( proj_pos1.z ) < 1.0 ) {
    float shadow_distance = max( ( texture( shadow1, proj_pos1.xy * 0.5 + 0.5 ).r ), 0.0 );
    float distance = proj_pos1.z - depth_bias;
    if( shadow_distance < distance ) return 1.0;
    else return 0.0;
  }
  else if( abs( proj_pos2.x ) < 1.0 && abs( proj_pos2.y ) < 1.0 && abs( proj_pos2.z ) < 1.0 ) {
    float shadow_distance = max( ( texture( shadow2, proj_pos2.xy * 0.5 + 0.5 ).r ), 0.0 );
    float distance = proj_pos2.z - depth_bias;
    if( shadow_distance < distance ) return 1.0;
    else return 0.0;
  }
  else {
    float shadow_distance = max( ( texture( shadow3, proj_pos3.xy * 0.5 + 0.5 ).r ), 0.0 );
    float distance = proj_pos3.z - depth_bias;
    if( shadow_distance < distance ) return 1.0;
    else return 0.0;
  }
}

float shadow( vec4 pos0, vec4 pos1, vec4 pos2, vec4 pos3 ) {
  vec4 proj_pos0 = pos0;
  proj_pos0 /= proj_pos0.w;
  vec4 proj_pos1 = pos1;
  proj_pos1 /= proj_pos1.w;
  vec4 proj_pos2 = pos2;
  proj_pos2 /= proj_pos2.w;
  vec4 proj_pos3 = pos3;
  proj_pos3 /= proj_pos3.w;
  float bias = 0.001;
  if( dynamic_uniforms.shadow_mode == 0 ) {
    return simple_shadow( proj_pos0.xyz, bias );
  }
  else if( dynamic_uniforms.shadow_mode == 1 ) {
    return pcf( proj_pos0.xyz, dynamic_uniforms.light_size, dynamic_uniforms.light_frustum_width, bias );
  }
  else if( dynamic_uniforms.shadow_mode == 2 ) {
    return pcss( proj_pos0.xyz, dynamic_uniforms.light_size, dynamic_uniforms.light_frustum_width, bias );
  }
  else if( dynamic_uniforms.shadow_mode == 3 ) {
    return vsm( proj_pos0.xyz, dynamic_uniforms.light_size, dynamic_uniforms.light_frustum_width, bias );
  }
  else if( dynamic_uniforms.shadow_mode == 4 ) {
    return pssm( proj_pos0.xyz, proj_pos1.xyz, proj_pos2.xyz, proj_pos3.xyz, bias );
  }
  else return 1.0;
}


