vec3 gamma( vec3 v ) {
  return min( max( v / (v + 0.155 ) * 1.019, vec3( 0, 0, 0 ) ), vec3( 1, 1, 1 ) );
}

float fresnel( vec3 V, vec3 N ) {
  float c = 1 - clamp( dot( V, N ), 0, 1 );
  float c2 = c * c;
  return c2 * c2 * c;
}

float GGX_D( vec3 N, vec3 H, float roughness ) {
  const float pi = 3.141592653589793;
  float a2 = roughness * roughness;
  float NH = max( dot( N, H ), 0 );
  float t = 1 + ( a2 - 1 )* NH;
  float NH2 = NH*NH;
  float t1 = tan( acos( NH ) );
  float t2 = roughness*roughness + t1 * t1;
  return roughness*roughness/(pi*NH2*NH2*t2*t2);
}

float GGX_G1( vec3 V, vec3 N, float roughness ) {
  float VN = max( dot( V, N ), 0 );
  float t = tan( acos( VN ) );
  float l = ( sqrt(roughness * roughness + ( 1 - roughness * roughness ) * t * t )/VN - 1 )/2;
  return 1/(1 + l);
}

float GGX_G2( vec3 L, vec3 V, vec3 N, float roughness ) {
  return GGX_G1( L, N , roughness ) * GGX_G1( V, N , roughness );
}

vec3 walter( vec3 L, vec3 V, vec3 N, float roughness, vec3 fres ) {
  vec3 H = normalize(V + L);
  float D = GGX_D( N, H, roughness );
  vec3 F = fres + ( vec3( 1, 1, 1 ) - fres ) * fresnel( L, N );
  float G = GGX_G2( L, V, N, roughness );
  float scale = 4 * dot( L, N ) * dot( V, N );
  vec3 specular = F * D * G / scale;
  return specular;
}

float burley( vec3 L, vec3 V, vec3 N, float roughness ) {
  float fl = fresnel( L, N );
  float fv = fresnel( V, N );
  vec3 H = ( L + V )/ length( L + V );
  float LH = dot( L, H );
  float fd90l = 0.5 + 2 * LH * LH * roughness;
  float VH = dot( V, H );
  float fd90v = 0.5 + 2 * VH * VH * roughness;
  return mix( 1.0, fd90l, fl ) * mix( 1.0, fd90v, fv );
}

vec3 light(
  vec3 L,
  vec3 V,
  vec3 N,
  vec3 diffuse_color,
  float roughness,
  float metallicness,
  float ambient,
  vec3 emissive,
  float light_energy
) {
  const float pi = 3.141592653589793;
  vec3 specular = walter( L, V, N, roughness, mix( vec3( 0.04, 0.04, 0.04 ), diffuse_color, metallicness ) );
  float diffuse = max( dot( L, N ), 0 ) /pi * burley( L, V, N, roughness );
  vec3 linear = ( max( dot( L, N ), 0 ) * ( max( specular, vec3( 0, 0, 0 ) ) + ( 1 - metallicness ) * diffuse * diffuse_color ) + ambient * diffuse_color ) * light_energy + emissive;
  return linear;
}

vec3 light_with_mask(
  vec3 L,
  vec3 V,
  vec3 N,
  vec3 diffuse_color,
  float roughness,
  float metallicness,
  float ambient,
  vec3 emissive,
  float light_energy,
  float masked
) {
  const float pi = 3.141592653589793;
  vec3 specular = walter( L, V, N, roughness, mix( vec3( 0.04, 0.04, 0.04 ), diffuse_color, metallicness ) );
  float diffuse = max( dot( L, N ), 0 ) /pi * burley( L, V, N, roughness );
  vec3 linear = mix( ( max( dot( L, N ), 0 ) * ( max( specular, vec3( 0, 0, 0 ) ) + ( 1 - metallicness ) * diffuse * diffuse_color ) + ambient * diffuse_color ) * light_energy + emissive, ( ambient * diffuse_color ) * light_energy + emissive, masked );
  return linear;
}
