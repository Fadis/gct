#ifndef GCT_SHADER_LIGHTING_H
#define GCT_SHADER_LIGHTING_H

#include <gct/constants.h>
#include <gct/project.h>

float schlick_fresnel( float angle ) {
    float c = 1 - clamp( angle, 0, 1 );
    float c2 = c * c;
    float c5 = c2 * c2 * c;
    return c5;
}

float schlick_fresnel( vec3 V, vec3 N ) {
  return schlick_fresnel( max( dot( V, N ), 0.0 ) );
}

float schlick_fresnel( float angle, float refractive_index )
{
    const float coeff_1 = ( 1 - refractive_index ) / ( 1 + refractive_index );
    const float coeff_2 = coeff_1 * coeff_1;
    float c = 1 - clamp( angle, 0, 1 );
    float c2 = c * c;
    float c5 = c2 * c2 * c;
    return ( 1 - coeff_2 ) + ( 1 - coeff_2 ) * c5;
}

float schlick_fresnel( vec3 V, vec3 N, float refractive_index ) {
  return schlick_fresnel( max( dot( V, N ), 0.0 ), refractive_index );
}

float GGX_D( vec3 N, vec3 H, float roughness ) {
  float a2 = roughness * roughness;
  float NH = max( dot( N, H ), 0 );
  float t = 1 + ( a2 - 1 )* NH;
  float NH2 = NH*NH;
  float t1 = tan( acos( NH ) );
  float t2 = roughness*roughness + t1 * t1;
  return roughness*roughness/(pi*NH2*NH2*t2*t2);
}

float GGX_G1( vec3 V, vec3 N, float roughness ) {
  roughness = max( roughness, 0.01 );
  float VN = max( dot( V, N ), 0 );
  float l = sqrt(roughness * roughness + ( 1 - roughness * roughness ) * VN * VN );
  return ( 2 * VN )/( VN + l );
}

float GGX_G2( vec3 L, vec3 V, vec3 N, float roughness ) {
  return GGX_G1( L, N , roughness ) * GGX_G1( V, N , roughness );
}

vec3 walter( vec3 L, vec3 V, vec3 N, float roughness, vec3 fres ) {
  vec3 H = normalize(V + L);
  float D = GGX_D( N, H, roughness );
  vec3 F = fres + ( vec3( 1, 1, 1 ) - fres ) * schlick_fresnel( L, N );
  float G = GGX_G2( L, V, N, /*min(*/ roughness/*, 1.0 )*/ );
  float scale = 4 * dot( L, N ) * dot( V, N );
  vec3 specular = F * D * G / scale;
  return specular;
}

float burley( vec3 L, vec3 V, vec3 N, float roughness ) {
  float fl = schlick_fresnel( L, N );
  float fv = schlick_fresnel( V, N );
  vec3 H = ( L + V )/ length( L + V );
  float LH = dot( L, H );
  float fd90l = 0.5 + 2 * LH * LH * roughness;
  float VH = dot( V, H );
  float fd90v = 0.5 + 2 * VH * VH * roughness;
  return mix( 1.0, fd90l, fl ) * mix( 1.0, fd90v, fv );
}

float rand( vec2 i ) {
  vec2 s = vec2( 12.9898, 78.233 );
  float t = 43758.5453;
  return fract( t * sin( dot( i, s ) ) );
}
float rand( vec2 i, float l ) {
  return rand( vec2( rand( i ), l ) );
}
float rand( vec2 i, float l, float t ) {
  return rand( vec2( rand( i, l ), t ) );
}

float perlin( vec2 p, float l, float t ) {
  vec2 pos = floor( p * l );
  vec2 posx = pos + vec2( 1.0, 0.0 );
  vec2 posy = pos + vec2( 0.0, 1.0 );
  vec2 posxy = pos + vec2( 1.0 );

  float c = rand( pos, l, t );
  float cx = rand( posx, l, t );
  float cy = rand( posy, l, t );
  float cxy = rand( posxy, l, t );

  vec2 d = fract( p * l );
  d = -0.5 * cos( d * pi ) + 0.5;

  float ccx = mix( c, cx, d.x );
  float cycxy = mix( cy, cxy, d.x );
  float center = mix( ccx, cycxy, d.y );

  return center * 2.0 - 1.0;
}
float perlin( vec2 p, float l ) {
  return perlin( p, l, 0.0 );
}


float grad(vec2 p, float scale ){
  float x1 = fract( floor( p.x * scale ) / scale );
  float y1 = fract( floor( p.y * scale ) / scale );
  float x2 = fract( x1 + 1.0 / scale );
  float y2 = fract( y1 + 1.0 / scale );
  float p1 = rand( vec2( x1, y1 ) );
  float p2 = rand( vec2( x1, y2 ) );
  float p3 = rand( vec2( x2, y1 ) );
  float p4 = rand( vec2( x2, y2 ) );
  float x_level = fract( p.x * scale );
  float y_level = fract( p.y * scale );
  return mix(
    mix( p1, p3, x_level ),
    mix( p2, p4, x_level ),
    y_level
  );
}

vec3 simple_light(
  vec3 L,
  vec3 V,
  vec3 N,
  vec3 diffuse_color,
  float roughness,
  float metallicness,
  float light_energy
) {
  vec3 specular = walter( L, V, N, roughness, mix( vec3( 1, 1, 1 ), diffuse_color, metallicness ) );
  float diffuse = max( dot( L, N ), 0 ) /pi * burley( L, V, N, roughness );
  vec3 linear = ( max( dot( L, N ), 0 ) * ( max( specular, vec3( 0, 0, 0 ) ) + ( 1 - metallicness ) * diffuse * diffuse_color ) ) * light_energy;
  return linear;
}

vec3 light(
  vec3 L,
  vec3 V,
  vec3 N,
  vec3 WV,
  vec3 WN,
  vec3 diffuse_color,
  float roughness,
  float metallicness,
  vec3 emissive,
  float light_energy
) {
  vec3 specular = walter( L, V, N, roughness, mix( vec3( 1, 1, 1 ), diffuse_color, metallicness ) );
  float diffuse = max( dot( L, N ), 0 ) /pi * burley( L, V, N, roughness );
  vec3 linear = ( max( dot( L, N ), 0 ) * ( max( specular, vec3( 0, 0, 0 ) ) + ( 1 - metallicness ) * diffuse * diffuse_color ) ) * light_energy + emissive;
  linear = linear * 0.5;
  return linear;
}

vec3 light_with_mask(
  vec3 L,
  vec3 V,
  vec3 N,
  vec3 WV,
  vec3 WN,
  vec3 diffuse_color,
  float roughness,
  float metallicness,
  vec3 emissive,
  float light_energy,
  float masked
) {
  vec3 specular = walter( L, V, N, roughness, mix( vec3( 1, 1, 1 ), diffuse_color, metallicness ) );
  float diffuse = max( dot( L, N ), 0 ) /pi * burley( L, V, N, roughness );
  vec3 linear = mix(
    emissive,
    (
      max( dot( L, N ), 0 ) * (
        max( specular, vec3( 0, 0, 0 ) ) +
        ( 1 - metallicness ) * diffuse * diffuse_color
      )
    ) * light_energy +
    emissive,
    masked
  );
  return linear;
}

vec3 specular_with_mask(
  vec3 L,
  vec3 V,
  vec3 N,
  vec3 diffuse_color,
  float roughness,
  float metallicness,
  float light_energy,
  float masked
) {
  vec3 specular = walter( L, V, N, roughness, mix( vec3( 1, 1, 1 ), diffuse_color, metallicness ) );
  vec3 linear = mix(
    vec3( 0.0, 0.0, 0.0 ),
    (
      max( dot( L, N ), 0 ) * (
        max( specular, vec3( 0, 0, 0 ) )
      )
    ) * light_energy,
    masked
  );
  return linear;
}

vec3 specular_with_mask(
  vec3 L,
  vec3 V,
  vec3 N,
  vec3 diffuse_color,
  float roughness,
  float metallicness,
  vec3 light_energy,
  float masked
) {
  vec3 specular = walter( L, V, N, roughness, mix( vec3( 1, 1, 1 ), diffuse_color, metallicness ) );
  vec3 linear = mix(
    vec3( 0.0, 0.0, 0.0 ),
    (
      max( dot( L, N ), 0 ) * (
        max( specular, vec3( 0, 0, 0 ) )
      )
    ) * light_energy,
    masked
  );
  return linear;
}

vec3 diffuse_with_mask(
  vec3 L,
  vec3 V,
  vec3 N,
  vec3 diffuse_color,
  float roughness,
  float metallicness,
  vec3 emissive,
  float light_energy,
  float masked
) {
  float diffuse = max( dot( L, N ), 0 ) /pi * burley( L, V, N, roughness );
  vec3 linear = mix(
    emissive,
    (
      max( dot( L, N ), 0 ) * (
        ( 1 - metallicness ) * diffuse * diffuse_color
      )
    ) * light_energy +
    emissive,
    masked
  );
  return linear;
}

vec3 diffuse_with_mask(
  vec3 L,
  vec3 V,
  vec3 N,
  vec3 diffuse_color,
  float roughness,
  float metallicness,
  vec3 emissive,
  vec3 light_energy,
  float masked
) {
  float diffuse = max( dot( L, N ), 0 ) /pi * burley( L, V, N, roughness );
  vec3 linear = mix(
    emissive,
    (
      max( dot( L, N ), 0 ) * (
        ( 1 - metallicness ) * diffuse * diffuse_color
      )
    ) * light_energy +
    emissive,
    masked
  );
  return linear;
}

// EONモデルFON相当部分
float diffuse_eon_E_FON_approx( float mu, float r ) {
  const float constant1_FON = 0.5f - 2.0f / ( 3.0f * pi );
  const float mucomp = 1.0f - mu;
  const float mucomp2 =mucomp * mucomp;
  const mat2 Gcoeffs = mat2(
    0.0571085289f, -0.332181442f,
    0.491881867f, 0.0714429953f
  );
  const float GoverPi = dot( Gcoeffs * vec2( mucomp, mucomp2 ), vec2( 1.0f, mucomp2 ) );
  return ( 1.0f + r * GoverPi ) / ( 1.0f + constant1_FON * r );
}

// EONモデル
vec3 diffuse_eon(
  vec3 L,
  vec3 V,
  vec3 N,
  vec3 diffuse_color,
  float roughness,
  float metallicness,
  vec3 light_energy,
  float masked
) {
  const float constant1_FON = 0.5f - 2.0f / ( 3.0f * pi );
  const float constant2_FON = 2.0f / 3.0f - 28.0f / ( 15.0f * pi );
  const float r = min( roughness * 0.5, 1.0 );
  const float mu_i = dot( L, N );
  if( mu_i <= 0.0 ) return vec3( 0, 0, 0 );
  const float mu_o = dot( V, N );
  if( mu_o <= 0.0 ) return vec3( 0, 0, 0 );
  const float s = dot( L, V ) -  mu_i * mu_o;
  const float sovertF = s > 0.0f ? s / max( mu_i, mu_o ) : s;
  const float AF = 1.0f / ( 1.0f + constant1_FON * r );
  const vec3 f_ss = ( diffuse_color / pi ) * AF * ( 1.0f + r * sovertF );
  const float EFo = diffuse_eon_E_FON_approx( mu_o, r );
  const float EFi = diffuse_eon_E_FON_approx( mu_o, r );
  const float avgEF = AF * ( 1.0f + constant2_FON * r );
  const vec3 rho_ms = ( diffuse_color * diffuse_color ) * avgEF / ( vec3( 1.0f ) - diffuse_color * ( 1.0f - avgEF ) );
  const float eps = 1.0e-7f;
  const vec3 f_ms =
    ( rho_ms / pi ) *
    max( eps, 1.0f - EFo ) *
    max( eps, 1.0f - EFi ) /
    max( eps, 1.0f - avgEF );
  const vec3 diffuse = f_ss + f_ms;
  return mix(
    vec3( 0, 0, 0 ),
    max( dot( L, N ), 0 ) * ( 1 - metallicness ) * diffuse * light_energy,
    masked
  );
}

// Kajiya-Kayモデル 拡散
vec3 diffuse_kajiya_kay(
  vec3 L,
  vec3 V,
  vec3 T,
  vec3 diffuse_color,
  float roughness,
  float metallicness,
  vec3 light_energy,
  float masked
) {
  const float u = max( dot( L, T ), 0.0 );
  const float diffuse = sqrt( 1.0 - u * u );
  return 1.0/pi * mix(
    vec3( 0, 0, 0 ),
    ( 1 - metallicness ) * diffuse * diffuse_color * light_energy,
    masked
  );
}

float roughness_to_blinn_phong_shininess( float roughness ) {
  return - 2.0 * ( -1.0 + roughness * roughness ) / ( 0.01 + roughness * roughness );
}

// Kajiya-Kayモデル 反射
vec3 specular_kajiya_kay(
  vec3 L,
  vec3 V,
  vec3 T,
  vec3 diffuse_color,
  float roughness,
  float metallicness,
  vec3 light_energy,
  float masked
) {
  const float cos_tl = dot( T, L );
  const float cos_tv = dot( T, V );
  const float sin_tl = sqrt( 1.0 - cos_tl * cos_tl );
  const float sin_tv = sqrt( 1.0 - cos_tv * cos_tv );
  const float shininess = roughness_to_blinn_phong_shininess( roughness )/4;
  const float specular = pow( max( cos_tl * cos_tv + sin_tl * sin_tv, 0.0 ), shininess );
  return 1.0/pi * mix(
    vec3( 0, 0, 0 ),
    mix( vec3( 1, 1, 1 ), diffuse_color, metallicness ) * specular * light_energy,
    masked
  );
}

vec3 diffuse_stalling(
  vec3 L,
  vec3 V,
  vec3 T,
  vec3 diffuse_color,
  float roughness,
  float metallicness,
  vec3 light_energy,
  float masked
) {
  return diffuse_kajiya_kay(
    L,
    V,
    T,
    diffuse_color,
    roughness,
    metallicness,
    light_energy,
    masked
  );
}

// Stallingモデル
vec3 specular_stalling(
  vec3 L,
  vec3 V,
  vec3 T,
  vec3 diffuse_color,
  float roughness,
  float metallicness,
  vec3 light_energy,
  float masked
) {
  const float cos_lt = dot( L, T );
  const float cos_vt = dot( V, T );
  const float shininess = roughness_to_blinn_phong_shininess( roughness )/4;
  const float specular = pow( max( sqrt( 1.0 - cos_lt * cos_lt ) * sqrt( 1.0 - cos_vt * cos_vt ) - cos_lt * cos_vt, 0.0 ), shininess );
  return 1.0/pi * mix(
    vec3( 0, 0, 0 ),
    mix( vec3( 1, 1, 1 ), diffuse_color, metallicness ) * specular * light_energy,
    masked
  );
}

// Kajiya-Kay-Blinn-Phongモデル
vec3 specular_kajiya_kay_blinn_phong(
  vec3 L,
  vec3 V,
  vec3 T,
  vec3 diffuse_color,
  float roughness,
  float metallicness,
  vec3 light_energy,
  float masked
) {
  const vec3 H = normalize( ( L + V ) * 0.5 );
  const float u = max( dot( H, T ), 0.0 );
  const float iu2 = ( 1.0 - u * u );
  const float specular = iu2 * iu2;
  return 1.0/pi * mix(
    vec3( 0, 0, 0 ),
    mix( vec3( 1, 1, 1 ), diffuse_color, metallicness ) * specular * light_energy,
    masked
  );
}

float gaussian_lobe( float alpha , float beta, float sin_theta_i, float sin_theta_r ) {
  return exp( -0.5 * pow( sin_theta_i + sin_theta_r - alpha, 2 ) / pow( beta, 2 ) ) * sqrt_two_pi_inv / beta;
}

// Marschner-Karisモデル
vec3 specular_marschner_karis(
  vec3 L,
  vec3 V,
  vec3 T,
  vec3 diffuse_color,
  float roughness,
  float metallicness,
  vec3 light_energy,
  float masked,
  float shift,
  float refractive_index,
  float proc_scale_r,
  float proc_scale_tt,
  float proc_scale_trt
) {
  const float sin_theta_i = dot( L, T );
  const float sin_theta_r = dot( V, T );
  const float cos_theta_i = sqrt( 1 - sin_theta_i * sin_theta_i );
  const float cos_theta_r = sqrt( 1 - sin_theta_r * sin_theta_r );
  const vec3 projected_L = normalize( project( L, T ) );
  const vec3 projected_V = normalize( project( V, T ) );
  const float cos_phi_d = dot( projected_L, projected_V );
  const vec3 projected_H = normalize( projected_L + projected_V );
  const float cos_half_phi = dot( projected_H, projected_V );
  const float cos_theta_d = ( 1 + cos_theta_i * cos_theta_r + sin_theta_i * sin_theta_r ) * 0.5;

  // M0
  float mr = gaussian_lobe( shift, roughness, sin_theta_i, sin_theta_r );
  // M1
  float mtt = gaussian_lobe( -shift * 0.5, roughness * 0.5, sin_theta_i, sin_theta_r );
  // M2
  float mtrt = gaussian_lobe( -3 * shift * 0.5, roughness * 2, sin_theta_i, sin_theta_r );

  float nr;
  {
    // f
    const float fresnel = schlick_fresnel( max( dot( L, V ), 0.0 ), refractive_index );
    // N0
    nr = 0.25 * cos_half_phi * fresnel;
  }

  vec3 ntt;
  {
    // alpha
    const float a = 1.55 / ( refractive_index * ( 1.19 / cos_theta_d + 0.36 * cos_theta_d ) );
    // hTT
    const float h = ( 1 + a * ( 0.6 - 0.8 * cos_phi_d ) ) * cos_half_phi;
    // T1
    const vec3 absorption = pow( diffuse_color.xyz, vec3( sqrt( 1 - h * h * a * a ) / 2 * cos_theta_d ) );
    // D1
    const float distribution = exp( -3.65 * cos_phi_d - 3.98 );
    // f
    const float fresnel = schlick_fresnel( cos_theta_d * sqrt( 1 - h * h ), refractive_index );
    // A1
    const vec3 attenuation = pow( 1 - fresnel , 2 ) * absorption;
    // N1
    ntt = 0.5 * attenuation * distribution;
  }

  vec3 ntrt;
  {
    // T2
    const vec3 absorption = pow( diffuse_color, vec3( 0.8 / cos_theta_d ) );
    // D2
    const float distribution = exp( 17 * cos_phi_d - 16.78 );
    // f
    const float fresnel = schlick_fresnel( cos_theta_d * 0.5, refractive_index );
    // A2
    const vec3 attenuation = pow( 1 - fresnel , 2 ) * fresnel * absorption * absorption; 
    // N2
    ntrt = 0.5 * attenuation * distribution;
  }

  const vec3 specular = vec3( mr * nr * proc_scale_r ) + mtt * ntt * proc_scale_tt + mtrt * ntrt * proc_scale_trt;
  return mix(
    vec3( 0, 0, 0 ),
    mix( vec3( 1, 1, 1 ), diffuse_color, metallicness ) * specular * light_energy,
    masked
  );
}

// Marschner-Karisモデル マルチスキャッタリング
vec3 diffuse_marschner_karis(
  vec3 L,
  vec3 V,
  vec3 T,
  vec3 diffuse_color,
  float roughness,
  float metallicness,
  vec3 light_energy,
  float masked,
  float diffuse_fall_off,
  float diffuse_azimuth_falloff,
  float scale_diffuse
) {
  const float sin_theta_i = dot( L, T );
  const float sin_theta_r = dot( V, T );
  const float cos_theta_i = sqrt( 1 - sin_theta_i * sin_theta_i );
  const vec3 projected_L = normalize( project( L, T ) );
  const vec3 projected_V = normalize( project( V, T ) );
  const vec3 projected_H = normalize( projected_L + projected_V );
  const float cos_half_phi = dot( projected_H, projected_V );

  const float cos_l = dot( T, L );
  const float sin_l = clamp( sqrt( 1.0 - cos_l * cos_l ), 0.0, 1.0 );

  const float diffuse = mix( 1.0, cos_theta_i, diffuse_fall_off ) * mix( 1.0, cos_half_phi, diffuse_azimuth_falloff ) * scale_diffuse * sin_l;
  return mix(
    vec3( 0, 0, 0 ),
    ( 1 - metallicness ) * diffuse * diffuse_color * light_energy,
    masked
  );
}

vec3 specular_marschner_karis(
  vec3 L,
  vec3 V,
  vec3 T,
  vec3 diffuse_color,
  float roughness,
  float metallicness,
  vec3 light_energy,
  float masked
) {
  return specular_marschner_karis(
    L, V, T,
    diffuse_color,
    roughness,
    metallicness,
    light_energy,
    masked,
    -5.0f*pi/180.0,
    1.55f,
    30.0f*0.005,
    10.0f*1.0,
    20.0f*4.0
  );
}

vec3 diffuse_marschner_karis(
  vec3 L,
  vec3 V,
  vec3 T,
  vec3 diffuse_color,
  float roughness,
  float metallicness,
  vec3 light_energy,
  float masked
) {
  return diffuse_marschner_karis(
    L, V, T,
    diffuse_color,
    roughness,
    metallicness,
    light_energy,
    masked,
    1.0f*0.5,//1.0,
    0.4f*0.5,//1.0,
    1.0f*0.25//0.5
  );
}

#endif

