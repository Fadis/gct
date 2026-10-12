#ifndef GCT_SHADER_PHASOR_NOISE_H
#define GCT_SHADER_PHASOR_NOISE_H

#ifdef __cplusplus
#include <gct/shader/glsl/gct/simplex_noise.h>
#include <gct/shader/glsl/gct/cellular_noise.h>
#include <glm/common.hpp>
#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
using namespace glm;
#else
#include <gct/simplex_noise.h>
#include <gct/cellular_noise.h>
#endif



#define M_PI 3.14159265358979323846f

float _f = 4.130f*4;
float _b = 1.37834187f*4;
float _o = 1.0f;
float _impPerKernel = 4;
float _pwmRatio = 0.5;

float _kr;
int _seed = 1;
//phasor noise parameters

///////////////////////////////////////////////
//prng
///////////////////////////////////////////////

int N = 15487469;
int x_;
void seed(int s){x_ = s;}
int next() { x_ *= 3039177861; x_ = x_ % N;return x_; }
float uni_0_1() {return  float(next()) / float(N);}
float uni(float min, float max){ return min + (uni_0_1() * (max - min));}


int morton(int x, int y)
{
  int z = 0;
  for (int i = 0 ; i < 32* 4 ; i++) {
    z |= ((x & (1 << i)) << i) | ((y & (1 << i)) << (i + 1));
  }
  return z;
}

void init_noise()
{
    _kr = sqrt(-log(0.05f) / M_PI) / _b;
}


vec2 phasor_kernel(vec2 x, float f, float b, float o, float phi)
{
    float a = exp(-M_PI * (b * b) * ((x.x * x.x) + (x.y * x.y)));
    float s = sin (2.0f* M_PI * f  * (x.x*cos(o) + x.y*sin(o))+phi);
    float c = cos (2.0f* M_PI * f  * (x.x*cos(o) + x.y*sin(o))+phi);
    //std::cout << "debug " << x.x << " " << x.y << " " << f << " " << b << " " << o << " " << phi << " " << a << " " << s << " " << c << std::endl;
    return vec2(a*c,a*s);
}



vec2 phasor_cell(ivec2 ij, vec2 uv)
{
	int s= morton(ij.x,ij.y) + 333;
	s = s==0? 1: s +_seed;
	seed(s);
	int impulse  =0;
	int nImpulse = _impPerKernel;
	float  cellsz = 2.0f * _kr;
	vec2 noise = vec2(0.0f,0.0f);
	while (impulse <= nImpulse){
		vec2 impulse_centre = vec2(uni_0_1(),uni_0_1());
    //std::cout << "debug2 " << uv.x << " " << uv.y << " " << impulse_centre.x << " " << impulse_centre.y << " " << cellsz << std::endl;
		vec2 d = (uv - impulse_centre) *cellsz;
		float rp = uni(0.0f,2.0f*M_PI) ;
        vec2 trueUv = ((vec2(ij) + impulse_centre) *cellsz);
		trueUv.y = -trueUv.y;
		noise += phasor_kernel(d, _f, _b ,_o,rp );
		impulse++;
	}
	return noise;
}

vec2 phasor_noise(vec2 uv)
{   
	float cellsz = 2.0f *_kr;
	vec2 _ij = uv / cellsz;
	ivec2  ij = ivec2(_ij);
	vec2  fij = _ij - vec2(ij);
	vec2 noise = vec2(0.0f,0.0f);
	for (int j = -2; j <= 2; j++) {
		for (int i = -2; i <= 2; i++) {
			ivec2 nij = ivec2(i, j);
      //std::cout << "debug3 " << fij.x << " " << fij.y << " " << nij.x << " " << nij.y << std::endl;
			noise += phasor_cell(ij + nij , fij - vec2(nij));
		}
	}
    return noise;
}

float phasor_noise_saw( vec2 uv ) {
  seed( 0 );
  init_noise();
  vec2 noise = phasor_noise( uv );
  float phi = atan( noise.y,noise.x ) + M_PI;
  return phi/(2.0f * M_PI);
}

float phasor_noise_pwm( vec2 uv ) {
  seed( 0 );
  init_noise();
  vec2 noise = phasor_noise( uv );
  float phi = atan( noise.y,noise.x ) + M_PI;
  return phi < _pwmRatio * 2.0f * M_PI ? 1.0f : -1.0f;
}


#endif

