#include <gct/specialization_map.hpp>

namespace gct {

encoded_specialization_info_t encode( const specialization_map &map ) {
  std::uint32_t head = 0u;
  std::uint32_t size = 0u;
  for( const auto &v: map ) {
    if( v.second.value.index() == 2u ) {
      size += 8u;
    }
    else {
      size += 4u;
    }
  }
  std::vector< std::uint8_t > temp( size, 0u );
  encoded_specialization_info_t encoded;
  for( const auto &v: map ) {
    if( v.second.value.index() == 0u ) {
      *reinterpret_cast< std::uint32_t* >( temp.data() + head ) = std::get< 0 >( v.second.value );
      encoded.add_map( v.first, head, 4u );
      head += 4u;
    }
    else if( v.second.value.index() == 1u ) {
      *reinterpret_cast< float* >( temp.data() + head ) = std::get< 1 >( v.second.value );
      encoded.add_map( v.first, head, 4u );
      head += 4u;
    }
    else {
      *reinterpret_cast< double* >( temp.data() + head ) = std::get< 2 >( v.second.value );
      encoded.add_map( v.first, head, 8u );
      head += 8u;
    }
  }
  encoded.set_data( std::move( temp ) );
  return encoded;
}

}
