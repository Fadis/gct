#ifndef GCT_GET_DEVICE_HPP
#define GCT_GET_DEVICE_HPP
#include <memory>
#include <type_traits>
#include <gct/voider.hpp>
namespace gct {
  class device_t;
  template< typename T, typename Enable = void >
  struct has_get_factory : public std::false_type {};
  template< typename T >
  struct has_get_factory< T, void_t<
    std::remove_cvref< decltype( *std::declval< T >().get_factory() ) >
  > > : public std::true_type {};
  template< typename T >
  constexpr bool has_get_factory_v = has_get_factory< T >::value;
  template< typename T, typename Enable = void >
  struct has_get_device : public std::false_type {};
  template< typename T >
  struct has_get_device< T, void_t<
    std::remove_cvref< decltype( *std::declval< T >().get_device() ) >
  > > : public std::true_type {};
  template< typename T >
  constexpr bool has_get_device_v = has_get_device< T >::value;

  namespace detail {
    template< typename T, typename Enable = void >
    struct get_device {};
    template< typename T >
    struct get_device<
      T,
      std::enable_if_t<
        has_get_factory_v< std::remove_cvref_t< T > > &&
        !std::is_same_v< std::remove_cvref_t< T >, device_t >
      >
    > {
      decltype(auto) operator()( T &v ) const {
        return get_device< decltype( *std::declval< T >().get_factory() ) >()( *v.get_factory() );
      }
    };
    template< typename T >
    struct get_device<
      T,
      std::enable_if_t<
        has_get_device_v< std::remove_cvref_t< T > > &&
        !std::is_same_v< std::remove_cvref_t< T >, device_t >
      >
    > {
      decltype(auto) operator()( T &v ) const {
        return get_device< decltype( *std::declval< T >().get_device() ) >()( *v.get_device() );
      }
    };
    template< typename T >
    struct get_device<
      T,
      std::enable_if_t<
        std::is_same_v< std::remove_cvref_t< T >, device_t >
      >
    > {
      decltype(auto) operator()( T &v ) const {
        return v;
      }
    };
  }
  template< typename T >
  decltype(auto) get_device( T &v ) {
    return detail::get_device< T >()( v );
  }
}

#endif

