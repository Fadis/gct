#ifndef GCT_DUMMY_SUBGROUP_HPP
#define GCT_DUMMY_SUBGROUP_HPP

#include <cstdint>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace gct {

constexpr std::uint32_t gl_SubgroupID = 0u;
constexpr std::uint32_t gl_SubgroupSize = 1u;
constexpr std::uint32_t gl_SubgroupInvocationID = 0u;

void subgroupBarrier();
void subgroupMemoryBarrier();
void subgroupMemoryBarrierBuffer();
void subgroupMemoryBarrierShared();
void subgroupMemoryBarrierImage();
bool subgroupElect();
bool subgroupAll( bool );
bool subgroupAny( bool );
template< typename T >
bool subgroupAllEqual( const T& ) {
  return true;
}
template< typename T >
T subgroupBroadcast( const T &value, std::uint32_t index ) {
  return ( index == 0 ) ? value : T( 0 );
}
template< typename T >
T subgroupBroadcastFirst( const T &value ) {
  return value;
}
glm::uvec4 subgroupBallot( bool value );
bool subgroupInverseBallot( glm::uvec4 value );
bool subgroupBallotBitExtract( glm::uvec4 value, std::uint32_t index );
std::uint32_t subgroupBallotBitCount( glm::uvec4 value );
std::uint32_t subgroupBallotInclusiveBitCount( glm::uvec4 value );
std::uint32_t subgroupBallotExclusiveBitCount( glm::uvec4 value );
std::uint32_t subgroupBallotFindLSB( glm::uvec4 value );
std::uint32_t subgroupBallotFindMSB( glm::uvec4 value );

template< typename T >
T subgroupAdd( const T &value ) {
  return value;
}
template< typename T >
T subgroupMul( const T &value ) {
  return value;
}
template< typename T >
T subgroupMin( const T &value ) {
  return value;
}
template< typename T >
T subgroupMax( const T &value ) {
  return value;
}
template< typename T >
T subgroupAnd( const T &value ) {
  return value;
}
template< typename T >
T subgroupOr( const T &value ) {
  return value;
}
template< typename T >
T subgroupXor( const T &value ) {
  return value;
}

template< typename T >
T subgroupInclusiveAdd( const T &value ) {
  return value;
}
template< typename T >
T subgroupInclusiveMul( const T &value ) {
  return value;
}
template< typename T >
T subgroupInclusiveMin( const T &value ) {
  return value;
}
template< typename T >
T subgroupInclusiveMax( const T &value ) {
  return value;
}
template< typename T >
T subgroupInclusiveAnd( const T &value ) {
  return value;
}
template< typename T >
T subgroupInclusiveOr( const T &value ) {
  return value;
}
template< typename T >
T subgroupInclusiveXor( const T &value ) {
  return value;
}
template< typename T >
T subgroupExclusiveAdd( const T &value ) {
  return 0;
}
template< typename T >
T subgroupExclusiveMul( const T &value ) {
  return 0;
}
template< typename T >
T subgroupExclusiveMin( const T &value ) {
  return 0;
}
template< typename T >
T subgroupExclusiveMax( const T &value ) {
  return 0;
}
template< typename T >
T subgroupExclusiveAnd( const T &value ) {
  return 0;
}
template< typename T >
T subgroupExclusiveOr( const T &value ) {
  return 0;
}
template< typename T >
T subgroupExclusiveXor( const T &value ) {
  return 0;
}


template< typename T >
T subgroupShuffle( const T &value, std::uint32_t index ) {
  return ( index == 0 ) ? value : T( 0 );
}

/*

T subgroupShuffleXor(T value, uint mask)
T subgroupShuffleUp(T value, uint delta)
T subgroupShuffleDown(T value, uint delta)

T subgroupClusteredAdd(T value, uint clusterSize)
T subgroupClusteredMul(T value, uint clusterSize)
T subgroupClusteredMin(T value, uint clusterSize)
T subgroupClusteredMax(T value, uint clusterSize)
T subgroupClusteredAnd(T value, uint clusterSize)
T subgroupClusteredOr(T value, uint clusterSize)
T subgroupClusteredXor(T value, uint clusterSize)

T subgroupQuadBroadcast(T value, uint id)
T subgroupQuadSwapHorizontal(T value)
T subgroupQuadSwapVertical(T value)
T subgroupQuadSwapDiagonal(T value)


*/

}

#endif

