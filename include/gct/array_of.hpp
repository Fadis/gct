#ifndef GCT_ARRAY_OF_HPP
#define GCT_ARRAY_OF_HPP

#include <vulkan/vulkan.h>
#include <gct/type_traits/has_rebuild_chain.hpp>
#include <gct/type_traits/is_lifted.hpp>
#include <gct/type_traits/is_dereferenceable.hpp>
#include <gct/type_traits/is_vulkan_handle.hpp>
#include <gct/deep_copy.hpp>
#include <gct/voider.hpp>

// NOLINTBEGIN(bugprone-macro-parentheses)

#define LIBGCT_ARRAY_OF_SMALL( type, name ) \
  private: \
    std::vector< type > name ; \
  public: \
    self_type &add_ ## name ( type v ) { \
      name .push_back( v ); \
      chained = false; \
      return *this; \
    } \
    template< typename Iterator > \
    self_type &add_ ## name ( Iterator begin, Iterator end ) { \
      name .insert( name .end(), begin, end ); \
      chained = false; \
      return *this; \
    } \
    self_type &set_ ## name ( const std::vector< type > &v ) { \
      name = v; \
      chained = false; \
      return *this; \
    } \
    self_type &set_ ## name ( std::vector< type > &&v ) { \
      name = std::move( v ); \
      chained = false; \
      return *this; \
    } \
    template< typename Iterator > \
    self_type &set_ ## name ( Iterator begin, Iterator end ) { \
      name .assign( begin, end ); \
      chained = false; \
      return *this; \
    } \
    self_type &clear_ ## name () { \
      name .clear(); \
      chained = false; \
      return *this; \
    } \
    [[nodiscard]] const std::vector< type > &get_ ## name () const { return name ; }

#define LIBGCT_ARRAY_OF( type, name ) \
  private: \
    std::vector< type > name ; \
  public: \
    self_type &add_ ## name ( const type &v ) { \
      name .push_back( v ); \
      chained = false; \
      return *this; \
    } \
    self_type &add_ ## name ( type &&v ) { \
      name .push_back( std::move( v ) ); \
      chained = false; \
      return *this; \
    } \
    template< typename Iterator > \
    self_type &add_ ## name ( Iterator begin, Iterator end ) { \
      name .insert( name .end(), begin, end ); \
      chained = false; \
      return *this; \
    } \
    self_type &set_ ## name ( const std::vector< type > &v ) { \
      name = v; \
      chained = false; \
      return *this; \
    } \
    self_type &set_ ## name ( std::vector< type > &&v ) { \
      name = std::move( v ); \
      chained = false; \
      return *this; \
    } \
    template< typename Iterator > \
    self_type &set_ ## name ( Iterator begin, Iterator end ) { \
      name .assign( begin, end ); \
      chained = false; \
      return *this; \
    } \
    self_type &clear_ ## name () { \
      name .clear(); \
      chained = false; \
      return *this; \
    } \
    [[nodiscard]] const std::vector< type > &get_ ## name () const { return name ; }

#define LIBGCT_ARRAY_OF_WRAPPED_INDIRECT( type, value_type, name ) \
  private: \
    using wrapped_ ## name ## _t = type ; \
    std::vector< std::shared_ptr< type > > name ; \
    std::vector< value_type > raw_ ## name ; \
  public: \
    self_type &add_ ## name ( const std::shared_ptr< type > &v ) { \
      name .push_back( v ); \
      chained = false; \
      return *this; \
    } \
    self_type &add_ ## name ( std::shared_ptr< type > &&v ) { \
      name .push_back( std::move( v ) ); \
      chained = false; \
      return *this; \
    } \
    template< typename Iterator > \
    self_type &add_ ## name ( Iterator begin, Iterator end ) { \
      name .insert( name .end(), begin, end ); \
      chained = false; \
      return *this; \
    } \
    self_type &set_ ## name ( const std::vector< std::shared_ptr< type > > &v ) { \
      name = v; \
      chained = false; \
      return *this; \
    } \
    self_type &set_ ## name ( std::vector< std::shared_ptr< type > > &&v ) { \
      name = std::move( v ); \
      chained = false; \
      return *this; \
    } \
    template< typename Iterator > \
    self_type &set_ ## name ( Iterator begin, Iterator end ) { \
      name .assign( begin, end ); \
      chained = false; \
      return *this; \
    } \
    self_type &clear_ ## name () { \
      name .clear(); \
      chained = false; \
      return *this; \
    } \
    [[nodiscard]] const std::vector< std::shared_ptr< type > > &get_ ## name () const { return name ; }

#define LIBGCT_ARRAY_OF_WRAPPED( type, value_type, name ) \
  private: \
    using wrapped_ ## name ## _t = type ; \
    std::vector< type > name ; \
    std::vector< value_type > raw_ ## name ; \
  public: \
    self_type &add_ ## name ( const type &v ) { \
      name .push_back( v ); \
      chained = false; \
      return *this; \
    } \
    self_type &add_ ## name ( type &&v ) { \
      name .push_back( std::move( v ) ); \
      chained = false; \
      return *this; \
    } \
    template< typename Iterator > \
    self_type &add_ ## name ( Iterator begin, Iterator end ) { \
      name .insert( name .end(), begin, end ); \
      chained = false; \
      return *this; \
    } \
    self_type &set_ ## name ( const std::vector< type > &v ) { \
      name = v; \
      chained = false; \
      return *this; \
    } \
    self_type &set_ ## name ( std::vector< type > &&v ) { \
      name = std::move( v ); \
      chained = false; \
      return *this; \
    } \
    template< typename Iterator > \
    self_type &set_ ## name ( Iterator begin, Iterator end ) { \
      name .assign( begin, end ); \
      chained = false; \
      return *this; \
    } \
    self_type &clear_ ## name () { \
      name .clear(); \
      chained = false; \
      return *this; \
    } \
    [[nodiscard]] const std::vector< type > &get_ ## name () const { return name ; }

#define LIBGCT_ARRAY_OF_TO_JSON( node , json_name , name ) \
    if( root.find( #node ) == root.end() ) { \
      root[ #node ] = nlohmann::json::object(); \
    } \
    if( !v.get_ ## name ().empty() ) { \
      root[ #node ][ #json_name ] = v.get_ ## name (); \
    }

#define LIBGCT_ARRAY_OF_TO_JSON_WRAPPED( node , json_name , name ) \
    if( root.find( #node ) == root.end() ) { \
      root[ #node ] = nlohmann::json::object(); \
    } \
    if( !v.get_ ## name ().empty() ) { \
      root[ #node ][ #json_name ] = nlohmann::json::array(); \
      for( const auto &e: v.get_ ## name () ) { \
        root[ #node ][ #json_name ].push_back( gct::detail::unwrap_pointer< nlohmann::json >( e ) ); \
      } \
    }

#define LIBGCT_ARRAY_OF_FROM_JSON( node , json_name , name ) \
    if( root.find( #node ) != root.end() ) { \
      if( root[ #node ].find( #json_name ) != root[ #node ].end() ) { \
        if( !root[ #node ][ #json_name ].is_array() ) throw incompatible_json( "The JSON is incompatible", __FILE__, __LINE__ ); \
        v.clear_ ## name (); \
        for( auto &e: root[ #node ][ #json_name ] ) { \
          v.add_ ## name ( e ); \
        } \
      } \
    }

#define LIBGCT_ARRAY_OF_FROM_JSON_WRAPPED( node , json_name , name ) \
    if( root.find( #node ) != root.end() ) { \
      if( root[ #node ].find( #json_name ) != root[ #node ].end() ) { \
        if( !root[ #node ][ #json_name ].is_array() ) throw incompatible_json( "The JSON is incompatible", __FILE__, __LINE__ ); \
        v.clear_ ## name (); \
        using wrapped_type = std::remove_cvref_t< decltype( *v.get_ ## name ().begin() ) >; \
        using value_type = std::remove_cvref_t< decltype( gct::detail::unwrap_pointer< nlohmann::json >( *v.get_ ## name ().begin() ) ) >; \
        for( auto &e: root[ #node ][ #json_name ] ) { \
          v.add_ ## name ( gct::detail::wrap_pointer< wrapped_type >( e ) ); \
        } \
      } \
    }

#define LIBGCT_ARRAY_OF_REBUILD_CHAIN( node, count_name, pointer_name, name ) \
    if( ! name .empty() ) { \
      if( ! has_ ## node () ) { \
        set_ ## node (); \
      } \
      auto & node = get_writable_ ## node (); \
      node \
        .set ## count_name ( name .size() ) \
        .set ## pointer_name ( name .data() ); \
    }

#define LIBGCT_ARRAY_OF_REBUILD_CHAIN_PARTIAL( node, pointer_name, name ) \
    if( ! name .empty() ) { \
      if( ! has_ ## node () ) { \
        set_ ## node (); \
      } \
      auto & node = get_writable_ ## node (); \
      node \
        .set ## pointer_name ( name .data() ); \
    }
#if 1
#define LIBGCT_ARRAY_OF_REBUILD_CHAIN_WRAPPED( node, count_name, pointer_name, name ) \
    if( ! name .empty() ) { \
      if( ! has_ ## node () ) { \
        set_ ## node (); \
      } \
      auto & node = get_writable_ ## node (); \
      raw_ ## name .clear(); \
      raw_ ## name .reserve( name .size() ); \
      std::transform( \
        name .begin(), \
        name .end(), \
        std::back_inserter( raw_ ## name ), \
        []( const auto &v ) { \
          if constexpr( std::is_constructible_v< bool, std::remove_cvref_t< decltype( v ) > > ) { \
            if( v ) { \
              decltype(auto) r = gct::detail::unwrap_pointer< wrapped_ ## name ## _t >( v ); \
              gct::detail::rebuild_chain_if_available( r ); \
              return gct::detail::get_raw( r ); \
            } \
            else { \
              using result_type = std::remove_cvref_t< decltype( gct::detail::get_raw( gct::detail::unwrap_pointer< wrapped_ ## name ## _t >( v ) ) ) >; \
              if constexpr( type_traits::is_vulkan_hpp_handle_v< result_type > ) { \
                return result_type( VK_NULL_HANDLE ); \
              } \
              else if constexpr ( std::is_pointer_v< result_type > ) { \
                return result_type( nullptr ); \
              } \
              else { \
                return result_type(); \
              } \
            } \
          } \
          else { \
            decltype(auto) r = gct::detail::unwrap_pointer< wrapped_ ## name ## _t >( v ); \
            gct::detail::rebuild_chain_if_available( r ); \
            return gct::detail::get_raw( r ); \
          } \
        } \
      ); \
      node \
        .set ## count_name ( raw_ ## name .size() ) \
        .set ## pointer_name ( raw_ ## name .data() ); \
    }
#else
#define LIBGCT_ARRAY_OF_REBUILD_CHAIN_WRAPPED( node, count_name, pointer_name, name ) \
    if( ! name .empty() ) { \
      if( ! has_ ## node () ) { \
        set_ ## node (); \
      } \
      auto & node = get_writable_ ## node (); \
      raw_ ## name .clear(); \
      raw_ ## name .reserve( name .size() ); \
      std::transform( \
        name .begin(), \
        name .end(), \
        std::back_inserter( raw_ ## name ), \
        []( const auto &v ) { \
          if( v ) { \
            decltype(auto) r = gct::detail::unwrap_pointer< wrapped_ ## name ## _t >( v ); \
            gct::detail::rebuild_chain_if_available( r ); \
            return gct::detail::get_raw( r ); \
          } \
          else { \
            using result_type = std::remove_cvref_t< decltype( gct::detail::get_raw( gct::detail::unwrap_pointer< wrapped_ ## name ## _t >( v ) ) ) >; \
            if constexpr( type_traits::is_vulkan_hpp_handle_v< result_type > ) { \
              return result_type( VK_NULL_HANDLE ); \
            } \
            else if constexpr ( std::is_pointer_v< result_type > ) { \
              return result_type( nullptr ); \
            } \
            else { \
              return result_type(); \
            } \
          } \
        } \
      ); \
      node \
        .set ## count_name ( raw_ ## name .size() ) \
        .set ## pointer_name ( raw_ ## name .data() ); \
    }
#endif


namespace gct::detail {
  template< typename T, typename Enable = void >
  struct has_get_basic : public std::false_type {};
  template< typename T >
  struct has_get_basic<
    T,
    void_t< decltype( std::declval< T >().get_basic() ) >
  > : public std::true_type {};
  template< typename T >
  constexpr bool has_get_basic_v = has_get_basic< T >::value;
  template< typename T >
  [[nodiscard]] decltype(auto) get_raw( const T &v ) {
    if constexpr( has_get_basic_v< T > ) {
      return v.get_basic();
    }
    else if constexpr( type_traits::is_dereferenceable_v< T > ) {
      if constexpr( type_traits::is_dereferenceable_v< decltype( *std::declval< T >() ) > ) {
        if constexpr( std::is_constructible_v< bool, decltype( std::declval< T >() ) > ) {
          using result_type = std::remove_cvref_t< decltype( get_raw( *std::declval< T >() ) ) >;
// NOLINTBEGIN(bugprone-branch-clone)
          if constexpr( type_traits::is_vulkan_hpp_handle_v< decltype( std::declval< T >() ) > ) {
            if( v ) {
              return get_raw( *v );
            }
            else {
              return VK_NULL_HANDLE;
            }
          }
          else if constexpr( std::is_pointer_v< decltype( std::declval< T >() ) > ) {
            if( v ) {
              return get_raw( *v );
            }
            else {
              return nullptr;
            }
          }
// NOLINTEND(bugprone-branch-clone)
          else if constexpr( std::is_constructible_v< result_type > ) {
            if( v ) {
              return get_raw( *v );
            }
            else {
              throw -1;
            }
          }
          else {
            return get_raw( *v );
          }
        }
        else {
          return get_raw( *v );
        }
      }
      else {
        return v;
      }
    }
    else {
      return v;
    }
  }
  template< typename T >
  void rebuild_chain_if_available( T &v ) {
    if constexpr ( gct::type_traits::has_rebuild_chain< T >::value ) {
      v.rebuild_chain();
    }
  }
  template< typename T, typename U >
  decltype(auto) unwrap_pointer( U &v ) {
    if constexpr( std::is_convertible_v< std::remove_cvref_t< U >, T > ) {
      return v;
    }
    else {
      return unwrap_pointer< T >( *v );
    }
  }
  template< typename T, typename U >
  decltype(auto) wrap_pointer( U &&v ) {
// NOLINTBEGIN(bugprone-branch-clone)
    if constexpr( std::is_convertible_v< std::remove_cvref_t< U >, std::remove_cvref_t< T > > ) {
      return std::forward< U >( v );
    }
    else if constexpr ( type_traits::is_lifted_by_v< T, std::shared_ptr > ) {
      return T( new U( wrap_poiinter( std::forward< U >( v ) ) ) );
    }
    else if constexpr ( type_traits::is_lifted_by_v< U, std::unique_ptr > ) {
      return T( new U( wrap_pointer( std::forward< U >( v ) ) ) );
    }
    else if constexpr ( type_traits::is_lifted_by_v< U, deep_copy_unique_ptr > ) {
      return T( new U( wrap_pointer( std::forward< U >( v ) ) ) );
    }
    else {
      static_assert( "wrap_pointer : unsupported conversion" );
    }
// NOLINTEND(bugprone-branch-clone)
  }
  template< typename T, typename U >
  decltype(auto) wrap_pointer( const U &v ) {
// NOLINTBEGIN(bugprone-branch-clone)
    if constexpr( std::is_convertible_v< std::remove_cvref_t< U >, std::remove_cvref_t< T > > ) {
      return v;
    }
    else if constexpr ( type_traits::is_lifted_by_v< T, std::shared_ptr > ) {
      return T( new U( wrap_poiinter( v ) ) );
    }
    else if constexpr ( type_traits::is_lifted_by_v< U, std::unique_ptr > ) {
      return T( new U( wrap_pointer( v ) ) );
    }
    else if constexpr ( type_traits::is_lifted_by_v< U, deep_copy_unique_ptr > ) {
      return T( new U( wrap_pointer( v ) ) );
    }
    else {
      static_assert( "wrap_pointer : unsupported conversion" );
    }
// NOLINTEND(bugprone-branch-clone)
  }
}

// NOLINTEND(bugprone-macro-parentheses)

#endif

