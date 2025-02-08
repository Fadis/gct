#ifndef GCT_NUMERIC_TYPE_MATCH_HPP
#define GCT_NUMERIC_TYPE_MATCH_HPP
#include <cstdint>
#include <type_traits>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat2x2.hpp>
#include <glm/mat2x3.hpp>
#include <glm/mat3x2.hpp>
#include <glm/mat2x4.hpp>
#include <glm/mat4x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat3x4.hpp>
#include <glm/mat4x3.hpp>
#include <glm/mat4x4.hpp>
#include <gct/numeric_types.hpp>

namespace gct {
template< typename T, typename Enable = void >
struct numeric_type_match {
  [[nodiscard]] bool operator()( const numeric_type_t& ) const {
    return false;
  };
};
template< typename T >
struct numeric_type_match< T, std::enable_if_t< std::is_same_v< std::remove_cvref_t< T >, bool > > > {
  [[nodiscard]] bool operator()( const numeric_type_t &v ) const {
    return
      v.component == numeric_component_type_t::bool_ &&
      v.composite == numeric_composite_type_t::scalar &&
      sizeof( std::remove_cvref_t< T > ) * 8u == v.depth;
  }
};
template< typename T >
struct numeric_type_match< T, std::enable_if_t< std::is_integral_v< std::remove_cvref_t< T > > && !std::is_same_v< std::remove_cvref_t< T >, bool > > > {
  [[nodiscard]] bool operator()( const numeric_type_t &v ) const {
    return
      v.component == numeric_component_type_t::int_ &&
      v.composite == numeric_composite_type_t::scalar &&
      v.sign == std::is_signed_v< std::remove_cvref_t< T > > &&
      sizeof( std::remove_cvref_t< T > ) * 8u == v.depth;
  }
};

template< typename T >
struct numeric_type_match< T, std::enable_if_t< std::is_floating_point_v< std::remove_cvref_t< T > > > > {
  [[nodiscard]] bool operator()( const numeric_type_t &v ) const {
    return
      v.component == numeric_component_type_t::float_ &&
      v.composite == numeric_composite_type_t::scalar &&
      sizeof( std::remove_cvref_t< T > ) * 8u == v.depth;
  }
};

template< typename T >
struct numeric_type_match< T, std::enable_if_t< std::is_void_v< std::remove_cvref_t< T > > > > {
  [[nodiscard]] bool operator()( const numeric_type_t &v ) const {
    return
      v.component == numeric_component_type_t::void_ &&
      v.composite == numeric_composite_type_t::scalar;
  }
};

namespace detail {

template< typename T >
struct is_glm_vec : public std::false_type {};
template< glm::length_t L, typename T, glm::qualifier Q >
struct is_glm_vec< glm::vec< L, T, Q > > : public std::true_type {};
template< typename T >
constexpr bool is_glm_vec_v = is_glm_vec< T >::value;
template< typename T >
struct glm_vec_length {};
template< glm::length_t L, typename T, glm::qualifier Q >
struct glm_vec_length< glm::vec< L, T, Q > > {
  constexpr static std::size_t value = L;
};
template< typename T >
constexpr std::size_t glm_vec_length_v = glm_vec_length< T >::value;
template< typename T >
struct glm_vec_component_type {};
template< glm::length_t L, typename T, glm::qualifier Q >
struct glm_vec_component_type< glm::vec< L, T, Q > > {
  using type = T;
};
template< typename T >
using glm_vec_component_type_t = typename glm_vec_component_type< T >::type;

template< typename T >
struct is_glm_mat : public std::false_type {};
template< glm::length_t C, glm::length_t R, typename T, glm::qualifier Q >
struct is_glm_mat< glm::mat< C, R, T, Q > > : public std::true_type {};
template< typename T >
constexpr bool is_glm_mat_v = is_glm_mat< T >::value;
template< typename T >
struct glm_mat_column_length {};
template< glm::length_t C, glm::length_t R, typename T, glm::qualifier Q >
struct glm_mat_column_length< glm::mat< C, R, T, Q > > {
  constexpr static std::size_t value = C;
};
template< typename T >
constexpr std::size_t glm_mat_column_length_v = glm_mat_column_length< T >::value;
template< typename T >
struct glm_mat_row_length {};
template< glm::length_t C, glm::length_t R, typename T, glm::qualifier Q >
struct glm_mat_row_length< glm::mat< C, R, T, Q > > {
  constexpr static std::size_t value = R;
};
template< typename T >
constexpr std::size_t glm_mat_row_length_v = glm_mat_row_length< T >::value;
template< typename T >
struct glm_mat_component_type {};
template< glm::length_t C, glm::length_t R, typename T, glm::qualifier Q >
struct glm_mat_component_type< glm::mat< C, R, T, Q > > {
  using type = T;
};
template< typename T >
using glm_mat_component_type_t = typename glm_mat_component_type< T >::type;

}

template< typename T >
struct numeric_type_match< T, std::enable_if_t< detail::is_glm_vec_v< std::remove_cvref_t< T > > > > {
  [[nodiscard]] bool operator()( const numeric_type_t &v ) const {
    if( v.composite != numeric_composite_type_t::vector ) {
      return false;
    }
    if constexpr ( std::is_same_v< detail::glm_vec_component_type_t< std::remove_cvref_t< T > >, bool > ) {
      if( v.component != numeric_component_type_t::bool_ ) return false;
    }
    else if constexpr ( std::is_integral_v< detail::glm_vec_component_type_t< std::remove_cvref_t< T > > > ) {
      if( v.component != numeric_component_type_t::int_ ) return false;
      if( v.sign != std::is_signed_v< detail::glm_vec_component_type_t< std::remove_cvref_t< T > > > ) return false;
    }
    else if constexpr ( std::is_floating_point_v< detail::glm_vec_component_type_t< std::remove_cvref_t< T > > > ) {
      if( v.component != numeric_component_type_t::float_ ) return false;
    }
    else {
      return false;
    }
    if( detail::glm_vec_length_v< std::remove_cvref_t< T > > != v.rows ) return false;
    return true;
  }
};

template< typename T >
struct numeric_type_match< T, std::enable_if_t< detail::is_glm_mat_v< std::remove_cvref_t< T > > > > {
  [[nodiscard]] bool operator()( const numeric_type_t &v ) const {
    if( v.composite != numeric_composite_type_t::matrix ) {
      return false;
    }
    if constexpr ( std::is_same_v< detail::glm_mat_component_type_t< std::remove_cvref_t< T > >, bool > ) {
      if( v.component != numeric_component_type_t::bool_ ) return false;
    }
    else if constexpr ( std::is_integral_v< detail::glm_mat_component_type_t< std::remove_cvref_t< T > > > ) {
      if( v.component != numeric_component_type_t::int_ ) return false;
      if( v.sign != std::is_signed_v< detail::glm_mat_component_type_t< std::remove_cvref_t< T > > > ) return false;
    }
    else if constexpr ( std::is_floating_point_v< detail::glm_mat_component_type_t< std::remove_cvref_t< T > > > ) {
      if( v.component != numeric_component_type_t::float_ ) return false;
    }
    else {
      return false;
    }
    if( detail::glm_mat_column_length_v< std::remove_cvref_t< T > > != v.cols ) return false;
    if( detail::glm_mat_row_length_v< std::remove_cvref_t< T > > != v.rows ) return false;
    return true;
  }
};

}
#endif

