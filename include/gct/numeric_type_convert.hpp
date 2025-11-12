#ifndef GCT_NUMERIC_TYPE_CONVERT_HPP
#define GCT_NUMERIC_TYPE_CONVERT_HPP

#include <cstdint>
#include <gct/exception.hpp>
#include <gct/alignment.hpp>
#include <gct/numeric_types.hpp>
#include <gct/numeric_type_match.hpp>

namespace gct {

template< typename T, typename Enable = void >
struct numeric_type_convert {
void operator()(
  const numeric_type_t &dest_type,
  std::uint8_t *dest,
  const T &src
) {
  if( numeric_type_match< T >()( dest_type ) ) {
    *reinterpret_cast< std::remove_cvref_t< T >* >( dest ) = src;
  }
  else {
    throw exception::invalid_argument( "spv_reference::operator= : Incompatible value.", __FILE__, __LINE__ );
  }
}
};

template< typename T >
struct numeric_type_convert<
  T,
  std::enable_if<
    std::is_integral_v< std::remove_cvref_t< T > > ||
    std::is_floating_point_v< std::remove_cvref_t< T > >
  >
>{
void operator()(
  const numeric_type_t &dest_type,
  std::uint8_t *dest,
  const T &src
) {
  if( numeric_type_match< T >()( dest_type ) ) {
    *reinterpret_cast< std::remove_cvref_t< T >* >( dest ) = src;
  }
  else if( numeric_type_match< T >().convertible( dest_type ) ) {
    if( dest_type.component == numeric_component_type_t::bool_ ) {
      *reinterpret_cast< std::remove_cvref_t< bool >* >( dest ) = bool( src );
    }
    else if( dest_type.component == numeric_component_type_t::int_ ) {
      if( dest_type.sign ) {
        if( dest_type.depth == 8u ) {
          *reinterpret_cast< std::remove_cvref_t< std::int8_t >* >( dest ) = std::int8_t( src );
        }
        else if( dest_type.depth == 16u ) {
          *reinterpret_cast< std::remove_cvref_t< std::int16_t >* >( dest ) = std::int16_t( src );
        }
        else if( dest_type.depth == 32u ) {
          *reinterpret_cast< std::remove_cvref_t< std::int32_t >* >( dest ) = std::int32_t( src );
        }
        else if( dest_type.depth == 64u ) {
          *reinterpret_cast< std::remove_cvref_t< std::int64_t >* >( dest ) = std::int64_t( src );
        }
        else {
          throw exception::invalid_argument( "spv_reference::operator= : Incompatible value.", __FILE__, __LINE__ );
        }
      }
      else {
        if( dest_type.depth == 8u ) {
          *reinterpret_cast< std::remove_cvref_t< std::uint8_t >* >( dest ) = std::uint8_t( src );
        }
        else if( dest_type.depth == 16u ) {
          *reinterpret_cast< std::remove_cvref_t< std::uint16_t >* >( dest ) = std::uint16_t( src );
        }
        else if( dest_type.depth == 32u ) {
          *reinterpret_cast< std::remove_cvref_t< std::uint32_t >* >( dest ) = std::uint32_t( src );
        }
        else if( dest_type.depth == 64u ) {
          *reinterpret_cast< std::remove_cvref_t< std::uint64_t >* >( dest ) = std::uint64_t( src );
        }
        else {
          throw exception::invalid_argument( "spv_reference::operator= : Incompatible value.", __FILE__, __LINE__ );
        }
      }
    }
    else if( dest_type.component == numeric_component_type_t::float_ ) {
      if( dest_type.depth == 32u ) {
        *reinterpret_cast< std::remove_cvref_t< float >* >( dest ) = float( src );
      }
      else if( dest_type.depth == 64u ) {
        *reinterpret_cast< std::remove_cvref_t< double >* >( dest ) = double( src );
      }
      else {
        throw exception::invalid_argument( "spv_reference::operator= : Incompatible value.", __FILE__, __LINE__ );
      }
    }
  }
  else {
    throw exception::invalid_argument( "spv_reference::operator= : Incompatible value.", __FILE__, __LINE__ );
  }
}
};

}

#endif

