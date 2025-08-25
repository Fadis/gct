#include <algorithm>
#include <gct/simplify_buffer_copy.hpp>

namespace gct {

bool simplify_buffer_copy(
  std::vector< vk::BufferCopy > &copy
) {
  if( copy.empty() ) return true;
  std::sort(
    copy.begin(), copy.end(),
    []( const vk::BufferCopy &l, const vk::BufferCopy &r ) -> bool {
      return
        ( l.srcOffset < r.srcOffset ) ||
        ( l.srcOffset == r.srcOffset && l.dstOffset < r.dstOffset );
    }
  );
  bool overlap = false;
  for( unsigned int i = 0u; i < copy.size() - 1u; ++i ) {
    if( copy[ i ].size != 0u ) {
      if( copy[ i ].srcOffset + copy[ i ].size >= copy[ i + 1u ].srcOffset ) {
        if( copy[ i ].dstOffset + ( copy[ i + 1u ].srcOffset - copy[ i ].srcOffset ) == copy[ i + 1u ].dstOffset ) {
          if( copy[ i ].dstOffset + copy[ i ].size < copy[ i + 1u ].dstOffset + copy[ i + 1u ].size ) {
            copy[ i + 1u ].size = ( copy[ i + 1u ].dstOffset + copy[ i + 1u ].size ) - copy[ i ].dstOffset;
          }
          else {
            copy[ i + 1u ].size = copy[ i ].size;
          }
          copy[ i + 1u ].srcOffset = copy[ i ].srcOffset;
          copy[ i + 1u ].dstOffset = copy[ i ].dstOffset;
          copy[ i ].size = 0u;
        }
        else if( copy[ i ].srcOffset + copy[ i ].size > copy[ i + 1u ].srcOffset ) {
          overlap = true;
        }
      }
    }
  }
  copy.erase( std::remove_if(
    copy.begin(), copy.end(),
    []( const vk::BufferCopy &v ) -> bool {
      return v.size == 0u;
    }
  ), copy.end() );
  return !overlap;
}

}
