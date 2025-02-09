#ifndef GCT_SHADER_GAUSS_H
#define GCT_SHADER_GAUSS_H

int adaptive_gauss8_16_size = 33;
float adaptive_gauss8_16[33*58] = float[](
#include "adaptive_gauss8_16.inc"
);

#endif

