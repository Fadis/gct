#ifndef GCT_SHADER_NEIGHBOR_H
#define GCT_SHADER_NEIGHBOR_H

#ifdef __cplusplus
#include <cstdint>
#include <glm/vec2.hpp>
using uint = std::uint32_t;
using namespace glm;
#endif

const uint moore_neighbor_count = 8u;
const ivec2 moore_neighbor[ moore_neighbor_count ] = 
#ifdef __cplusplus
{
#else
ivec2[](
#endif
  ivec2( -1, -1 ),
  ivec2(  0, -1 ),
  ivec2(  1, -1 ),
  ivec2( -1,  0 ),
  ivec2(  1,  0 ),
  ivec2( -1,  1 ),
  ivec2(  0,  1 ),
  ivec2(  1,  1 )
#ifdef __cplusplus
};
#else
);
#endif

const uint neumann_neighbor_count = 4u;
const ivec2 neumann_neighbor[ neumann_neighbor_count ] = 
#ifdef __cplusplus
{
#else
ivec2[](
#endif
  ivec2(  0, -1 ),
  ivec2( -1,  0 ),
  ivec2(  1,  0 ),
  ivec2(  0,  1 )
#ifdef __cplusplus
};
#else
);
#endif

const uint modified_neumann_neighbor_count = 4u;
const ivec2 modified_neumann_neighbor[ modified_neumann_neighbor_count ] = 
#ifdef __cplusplus
{
#else
ivec2[](
#endif
  ivec2( -1, -1 ),
  ivec2(  1, -1 ),
  ivec2( -1,  1 ),
  ivec2(  1,  1 )
#ifdef __cplusplus
};
#else
);
#endif

#endif

