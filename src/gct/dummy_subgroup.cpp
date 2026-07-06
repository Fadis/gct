#include <bit>
#include <gct/dummy_subgroup.hpp>

namespace gct {

void subgroupBarrier() {
}
void subgroupMemoryBarrier() {
}
void subgroupMemoryBarrierBuffer() {
}
void subgroupMemoryBarrierShared() {
}
void subgroupMemoryBarrierImage() {
}
bool subgroupElect() {
  return true;
}
bool subgroupAll( bool value ) {
  return value;
}
bool subgroupAny( bool value ) {
  return value;
}

glm::uvec4 subgroupBallot( bool value ) {
  return glm::uvec4( std::uint32_t( value ? 1u : 0u ), 0u, 0u, 0u );
}
bool subgroupInverseBallot( glm::uvec4 value ) {
  return ( value.x & 0x1u );
}
bool subgroupBallotBitExtract( glm::uvec4 value, std::uint32_t index ) {
  if( index < 32u ) {
    return ( value.x >> index ) & 0x1u;
  }
  else if( index < 64u ) {
    return ( value.y >> ( index - 32u ) ) & 0x1u;
  }
  else if( index < 96u ) {
    return ( value.z >> ( index - 64u ) ) & 0x1u;
  }
  else {
    return ( value.w >> ( index - 96u ) ) & 0x1u;
  }
}

std::uint32_t subgroupBallotBitCount( glm::uvec4 value ) {
  return 
    std::popcount( value.x ) +
    std::popcount( value.y ) +
    std::popcount( value.z ) +
    std::popcount( value.w );
}

std::uint32_t subgroupBallotInclusiveBitCount( glm::uvec4 value ) {
  return ( value.x & 0x1u ) ? 1u : 0u;
}

std::uint32_t subgroupBallotExclusiveBitCount( glm::uvec4 value ) {
  return 0u;
}
std::uint32_t subgroupBallotFindLSB( glm::uvec4 value ) {
  std::uint32_t pos = std::countr_zero( value.x );
  if( pos == 32u ) {
    pos += std::countr_zero( value.y );
  }
  if( pos == 64u ) {
    pos += std::countr_zero( value.z );
  }
  if( pos == 96u ) {
    pos += std::countr_zero( value.w );
  }
  return pos;
}
std::uint32_t subgroupBallotFindMSB( glm::uvec4 value ) {
  std::uint32_t pos = std::countl_zero( value.w );
  if( pos == 32u ) {
    pos += std::countl_zero( value.z );
  }
  if( pos == 64u ) {
    pos += std::countl_zero( value.y );
  }
  if( pos == 96u ) {
    pos += std::countl_zero( value.x );
  }
  return pos;
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

