#ifndef GCT_SPV_MEMBER_POINTER_TO_JSON_HPP
#define GCT_SPV_MEMBER_POINTER_TO_JSON_HPP
#include <gct/spv_member_pointer.hpp>
#include <nlohmann/json.hpp>
#include <gct/half.hpp>

namespace gct {

template< typename T >
void to_json( nlohmann::json &dest, const spv_reference< T > &src ) {
  const auto &pcmp = src.get_member_pointer();
  if( pcmp.is_struct() ) {
    dest = nlohmann::json::object();
    for( const auto &mp: pcmp.get_member() ) {
      dest[ mp.first ] = nlohmann::json( src.get_head()->*(mp.second) );
    }
  }
  else if( pcmp.is_array() ) {
    dest = nlohmann::json::array();
    for( const auto &mp : pcmp ) {
      dest.push_back( nlohmann::json( src.get_head()->*mp ) );
    }
  }
  else {
    auto nt = pcmp.get_numeric();
    std::uint32_t component_count = 1u;
    if( nt.composite == numeric_composite_type_t::vector ) {
      component_count = nt.rows;
    }
    if( nt.composite == numeric_composite_type_t::matrix ) {
      component_count = nt.rows * nt.cols;
    }
    if( nt.component == numeric_component_type_t::int_ && nt.sign && nt.depth == 8u ) {
      const auto p = src.template get< std::int8_t >();
      if( component_count == 1u ) {
        dest = nlohmann::json( std::int32_t( *p ) );
      }
      else {
        dest = nlohmann::json::array();
        for( std::uint32_t i = 0u; i != component_count; ++i ) {
          dest.push_back( nlohmann::json( std::int32_t( p[ i ] ) ) );
        }
      }
    }
    else if( nt.component == numeric_component_type_t::int_ && nt.sign && nt.depth == 16u ) {
      const auto p = src.template get< std::int16_t >();
      if( component_count == 1u ) {
        dest = nlohmann::json( *p );
      }
      else {
        dest = nlohmann::json::array();
        for( std::uint32_t i = 0u; i != component_count; ++i ) {
          dest.push_back( nlohmann::json( p[ i ] ) );
        }
      }
    }
    else if( nt.component == numeric_component_type_t::int_ && nt.sign && nt.depth == 32u ) {
      const auto p = src.template get< std::int32_t >();
      if( component_count == 1u ) {
        dest = nlohmann::json( *p );
      }
      else {
        dest = nlohmann::json::array();
        for( std::uint32_t i = 0u; i != component_count; ++i ) {
          dest.push_back( nlohmann::json( p[ i ] ) );
        }
      }
    }
    else if( nt.component == numeric_component_type_t::int_ && nt.sign && nt.depth == 64u ) {
      const auto p = src.template get< std::int64_t >();
      if( component_count == 1u ) {
        dest = nlohmann::json( *p );
      }
      else {
        dest = nlohmann::json::array();
        for( std::uint32_t i = 0u; i != component_count; ++i ) {
          dest.push_back( nlohmann::json( p[ i ] ) );
        }
      }
    }
    else if( nt.component == numeric_component_type_t::int_ && !nt.sign && nt.depth == 8u ) {
      const auto p = src.template get< std::int8_t >();
      if( component_count == 1u ) {
        dest = nlohmann::json( std::uint32_t( *p ) );
      }
      else {
        dest = nlohmann::json::array();
        for( std::uint32_t i = 0u; i != component_count; ++i ) {
          dest = nlohmann::json( std::uint32_t( p[ i ] ) );
        }
      }
    }
    else if( nt.component == numeric_component_type_t::int_ && !nt.sign && nt.depth == 16u ) {
      const auto p = src.template get< std::uint16_t >();
      if( component_count == 1u ) {
        dest = nlohmann::json( *p );
      }
      else {
        dest = nlohmann::json::array();
        for( std::uint32_t i = 0u; i != component_count; ++i ) {
          dest.push_back( nlohmann::json( p[ i ] ) );
        }
      }
    }
    else if( nt.component == numeric_component_type_t::int_ && !nt.sign && nt.depth == 32u ) {
      const auto p = src.template get< std::uint32_t >();
      if( component_count == 1u ) {
        dest = nlohmann::json( *p );
      }
      else {
        dest = nlohmann::json::array();
        for( std::uint32_t i = 0u; i != component_count; ++i ) {
          dest = nlohmann::json( p[ i ] );
        }
      }
    }
    else if( nt.component == numeric_component_type_t::int_ && !nt.sign && nt.depth == 64u ) {
      const auto p = src.template get< std::uint64_t >();
      if( component_count == 1u ) {
        dest = nlohmann::json( *p );
      }
      else {
        dest = nlohmann::json::array();
        for( std::uint32_t i = 0u; i != component_count; ++i ) {
          dest.push_back( nlohmann::json( p[ i ] ) );
        }
      }
    }
    else if( nt.component == numeric_component_type_t::float_ && nt.depth == 16u ) {
      const auto p = src.template get< half_float::half >();
      if( component_count == 1u ) {
        dest = nlohmann::json( float( *p ) );
      }
      else {
        dest = nlohmann::json::array();
        for( std::uint32_t i = 0u; i != component_count; ++i ) {
          dest.push_back( nlohmann::json( float( p[ i ] ) ) );
        }
      }
    }
    else if( nt.component == numeric_component_type_t::float_ && nt.depth == 32u ) {
      const auto p = src.template get< float >();
      if( component_count == 1u ) {
        dest = nlohmann::json( *p );
      }
      else {
        dest = nlohmann::json::array();
        for( std::uint32_t i = 0u; i != component_count; ++i ) {
          dest.push_back( nlohmann::json( p[ i ] ) );
        }
      }
    }
    else if( nt.component == numeric_component_type_t::float_ && nt.depth == 64u ) {
      const auto p = src.template get< double >();
      if( component_count == 1u ) {
        dest = nlohmann::json( *p );
      }
      else {
        dest = nlohmann::json::array();
        for( std::uint32_t i = 0u; i != component_count; ++i ) {
          dest.push_back( nlohmann::json( p[ i ] ) );
        }
      }
    }
    else if( nt.component == numeric_component_type_t::bool_ ) {
      const auto p = src.template get< std::uint32_t >();
      if( component_count == 1u ) {
        dest = nlohmann::json( bool( *p ) );
      }
      else {
        dest = nlohmann::json::array();
        for( std::uint32_t i = 0u; i != component_count; ++i ) {
          dest.push_back( nlohmann::json( bool( p[ i ] ) ) );
        }
      }
    }
    else {
      throw exception::invalid_argument( "to_json : Invalid value type.", __FILE__, __LINE__ );
    }
  }
}

}

#endif

