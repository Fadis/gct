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
  const float pi = 3.141592653589793;
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

vec3 simple_ambient_light_r0(
  vec3 V,
  vec3 N,
  float roughness
) {
  vec3 D = normalize( reflect( normalize( V ), normalize( N ) ) );
  float pos = ( -D.y * 0.5 + 0.5 ) * 8.0;
  int index = int( floor( pos ) );
  float level = fract( pos );
  float mix_level = grad( vec2( fract( D.x ), fract( D.z ) ), pow( 2.0, floor( mix( -6.0, 8.0, 1 - roughness ) ) ) );
  float mix_tangent = ( 1 - roughness ) * 5.0;
  float mix_shift = -mix_tangent * 0.5;
  float clamped_mix_level = min( 1.0, max( 0.0, ( mix_level * mix_tangent + mix_shift ) ) );
  const vec3[ 10 ] c1 = vec3[10](
    vec3( 0.0, 0.19215686274509803, 0.0 ), // darkgreen
    vec3( 0.0, 0.19215686274509803, 0.0 ), // darkgreen
    vec3( 0.0, 0.19215686274509803, 0.0 ), // darkgreen
    vec3( 0.03333333333333333, 0.2450980392156862, 0.03333333333333333 ), // forestgreen
    vec3( 0.2117647058823529, 0.2117647058823529, 0.2117647058823529 ), // dimgray
    vec3( 0.5294117647058824, 0.807843137254902, 0.9803921568627451 ), // lightskyblue
    vec3( 0.3, 0.8490196078431373, 1.0 ), // deepskyblue
    vec3( 0.3, 0.8490196078431373, 1.0 ), // deepskyblue
    vec3( 0.3, 0.8490196078431373, 1.0 ), // deepskyblue
    //vec3( 0.3, 0.8490196078431373, 1.0 ), // deepskyblue
    vec3( 0.3, 0.8490196078431373, 1.0 ) // deepskyblue
  );
  const vec3[ 10 ] c2 = vec3[10](
    vec3( 0.3450980392156862, 0.17058823529411763, 0.07450980392156863 ), // saddlebrown
    vec3( 0.3450980392156862, 0.17058823529411763, 0.07450980392156863 ), // saddlebrown
    vec3( 0.3450980392156862, 0.17058823529411763, 0.07450980392156863 ), // saddlebrown
    vec3( 0.4705882352941177, 0.3215686274509804, 0.2294117647058824 ), // burlywood
    vec3( 0.2117647058823529, 0.2117647058823529, 0.2117647058823529 ), // dimgray
    vec3( 1.0, 1.0, 1.0 ), // white
    vec3( 1.0, 1.0, 1.0 ), // white
    vec3( 1.0, 1.0, 1.0 ), // white
    vec3( 1.0, 1.0, 1.0 ), // white
    //vec3( 1.0, 1.0, 1.0 ), // white
    vec3( 1.0, 1.0, 1.0 ) // white
  );
  vec3 c1i = mix( c1[ index ], c1[ index + 1 ], level );
  vec3 c2i = mix( c2[ index ], c2[ index + 1 ], level );
  return mix( c1i, c2i, clamped_mix_level );
}
vec3 simple_ambient_light_r1(
  vec3 V,
  vec3 N
) {
  vec3 D = normalize( reflect( normalize( V ), normalize( N ) ) );
  float pos = ( -D.y * 0.5 + 0.5 ) * 2;
  int index = int( floor( pos ) );
  float level = fract( pos );
  const vec3[ 3 ] c1 = vec3[3](
    vec3( 0.0, 0.19215686274509803, 0.0 ), // darkgreen
    vec3( 0.2117647058823529, 0.2117647058823529, 0.2117647058823529 ), // dimgray
    vec3( 1.0, 1.0, 1.0 ) // white
  );
  return mix( c1[ index ], c1[ index + 1 ], level );
}

vec3 simple_ambient_light(
  vec3 V,
  vec3 N,
  float roughness,
  vec3 diffuse_color,
  float metallicness
) {
  return simple_ambient_light_r0( V, N, roughness ) * diffuse_color;
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
  float ambient,
  vec3 emissive,
  float light_energy
) {
  const float pi = 3.141592653589793;
  vec3 specular = walter( L, V, N, roughness, mix( vec3( 1, 1, 1 ), diffuse_color, metallicness ) );
  float diffuse = max( dot( L, N ), 0 ) /pi * burley( L, V, N, roughness );
  vec3 linear = ( max( dot( L, N ), 0 ) * ( max( specular, vec3( 0, 0, 0 ) ) + ( 1 - metallicness ) * diffuse * diffuse_color ) + simple_ambient_light( WV, WN, roughness, diffuse_color, metallicness ) ) * ambient * light_energy + emissive;
  //return ;
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
  float ambient,
  vec3 emissive,
  float light_energy,
  float masked
) {
  const float pi = 3.141592653589793;
  vec3 specular = walter( L, V, N, roughness, mix( vec3( 1, 1, 1 ), diffuse_color, metallicness ) );
  float diffuse = max( dot( L, N ), 0 ) /pi * burley( L, V, N, roughness );
  vec3 linear = mix( ( max( dot( L, N ), 0 ) * ( max( specular, vec3( 0, 0, 0 ) ) + ( 1 - metallicness ) * diffuse * diffuse_color ) + simple_ambient_light( WV, WN, roughness, diffuse_color, metallicness ) ) * ambient * light_energy + emissive, ( ambient * simple_ambient_light( WV, WN, roughness, diffuse_color, metallicness ) ) * light_energy + emissive, masked );
  return linear;
}
