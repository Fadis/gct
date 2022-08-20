#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(binding = 0) uniform Uniforms {
  mat4 projection_matrix;
  mat4 camera_matrix;
  mat4 world_matrix;
  vec4 eye_pos;
  vec4 light_pos;
  float light_energy;
} uniforms;

layout (location = 0) in vec3 input_position;
layout (location = 1) in vec3 input_normal;
layout (location = 2) in vec3 input_color;
layout (location = 0) out vec4 output_color;

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

vec3 eotf( vec3 v ) {
  return min( max( v / (v + 0.155 ) * 1.019, vec3( 0, 0, 0 ) ), vec3( 1, 1, 1 ) );
}

void main()  {
  const float pi = 3.141592653589793;
  vec3 light_dir = normalize( uniforms.light_pos.xyz - input_position );
  vec3 eye_dir = normalize( uniforms.eye_pos.xyz - input_position );
  vec3 normal = normalize( input_normal );
  vec3 diffuse = input_color * ( max( dot( light_dir, normal ), 0 ) /pi );
  vec3 specular = max( dot( light_dir, normal ), 0 ) * max( walter( light_dir, eye_dir, normal, 0.3, vec3( 1.f, 1.f, 1.f ) ), vec3( 0.0, 0.0, 0.0 ) );
  vec3 ambient = input_color * 0.01;
  output_color = vec4( eotf( ( diffuse + specular + ambient ) * uniforms.light_energy ), 1.0 );
}


