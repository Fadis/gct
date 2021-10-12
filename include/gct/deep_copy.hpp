#ifndef GCT_BUFFER_DEEP_COPY_HPP
#define GCT_BUFFER_DEEP_COPY_HPP

#include <type_traits>
#include <utility>
#include <memory>

namespace gct {
  template< typename T >
  struct deep_copy_t : public T {
    using value_type = std::remove_cvref_t< decltype( *std::declval< T >() ) >;
    using T::T;
    deep_copy_t( const deep_copy_t &v ) {
      if( v )
        T::reset( new value_type( *v ) );
    }
    deep_copy_t &operator=( const deep_copy_t &v ) {
      if( v )
        T::reset( new value_type( *v ) );
      return *this;
    }
    deep_copy_t( deep_copy_t &&v ) : T( std::move( static_cast< T&& >( v ) ) ) {}
    deep_copy_t &operator=( deep_copy_t &&v ) {
      static_cast< T& >( *this ) = std::move( static_cast< T&& >( v ) );
    }
    explicit operator bool() const {
      return bool( static_cast< const T& >( *this ) );
    }
  };
  template< typename ...T >
  using deep_copy_unique_ptr = deep_copy_t< std::unique_ptr< T... > >;

}

#endif

