#ifndef GCT_SHADER_LIGHTING_H
#define GCT_SHADER_LIGHTING_H

#include "constants.h"

float fresnel( vec3 V, vec3 N ) {
  float c = 1 - clamp( dot( V, N ), 0, 1 );
  float c2 = c * c;
  return c2 * c2 * c;
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

#endif
