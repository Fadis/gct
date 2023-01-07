#ifndef GCT_NON_COPYABLE_FUNCTION_HPP
#define GCT_NON_COPYABLE_FUNCTION_HPP

#include <cstddef>
#include <cstdlib>
#include <memory>
#include <utility>
#include <tuple>
#include <type_traits>
#include <boost/type_erasure/any.hpp>
#include <boost/type_erasure/constructible.hpp>
#include <boost/type_erasure/callable.hpp>
#include <boost/type_erasure/typeid_of.hpp>
#include <boost/mpl/greater.hpp>

namespace gct {
  struct bad_function_call : public std::runtime_error {
    bad_function_call() : std::runtime_error( "bad_function_call" ) {}
  };
  
  template< typename >
  class non_copyable_function;
  
  template< std::size_t index, typename Args, typename Enable = void >
  struct non_copyable_function_argument {
    using type = void;
  };

  template< std::size_t index, typename Args >
  struct non_copyable_function_argument<
    index,
    Args,
    std::enable_if_t<
      bool( std::tuple_size< Args >::value > index )
    >
  > {
    using type = typename std::tuple_element< index, Args >::type;
  };
  template< std::size_t index, typename ... Args >
  using non_copyable_function_argument_t = typename non_copyable_function_argument< index, std::tuple< Args... > >::type;


  template< typename R, typename ... Args >
  class non_copyable_function< R( Args... ) > {
    template<class T = boost::type_erasure::_self>
    struct move_constructible : ::boost::mpl::vector<
      boost::type_erasure::constructible< T( T&& ) >, boost::type_erasure::destructible< T > > {};
  public:
    using result_type = R;
    using argument_type = non_copyable_function_argument_t< 0u, Args... >;
    using first_argument_type = non_copyable_function_argument_t< 0u, Args... >;
    using second_argument_type = non_copyable_function_argument_t< 1u, Args... >;
    using function_pointer = R(*)( Args... );
    using concept_ = boost::mpl::vector<
      move_constructible<>,
      boost::type_erasure::callable< R( Args... ) >
    >;
    using type_erased_function =
       boost::type_erasure::any< concept_ >;
    using function_holder =
       std::pair< function_pointer, type_erased_function >;
    non_copyable_function() noexcept {}
    non_copyable_function( std::nullptr_t ) noexcept {}
    non_copyable_function( const non_copyable_function& ) = delete;
    non_copyable_function( non_copyable_function &&src ) noexcept {
      func = std::move( src.func );
    }
    template< typename F >
    non_copyable_function( F &&f ) {
      set_function( std::move( f ) );
    }
    non_copyable_function &operator=( const non_copyable_function& ) = delete;
    non_copyable_function &operator=( non_copyable_function &&f ) noexcept {
      func = std::move( f.func );
      return *this;
    }
    void swap( non_copyable_function &r ) noexcept {
      std::swap( func, r.func );
    }
    template< typename F >
    void assign( F&& f ) {
      set_function( f );
    }
    explicit operator bool() const noexcept {
      return func.get();
    }
    R operator()( Args... args ) const {
      if( !func )
        throw bad_function_call();
      if constexpr ( std::is_same_v< R, void > ) {
        func->second( std::forward< Args >( args )... );
      }
      else {
        return func->second( std::forward< Args >( args )... );
      }
    }
    const std::type_info& target_type() const noexcept {
      if( func && func->first != nullptr )
        return typeid( R( Args... ) );
      else
        return typeid( void );
    }
    function_pointer target() const noexcept {
      if( func )
        return func->first;
      else
        return nullptr;
    }
  private:
    template< typename F >
    void set_function( F &&f ) {
      if constexpr ( std::is_convertible_v< F, function_pointer > ) {
        func.reset( new function_holder( f, type_erased_function( std::forward< F >( f ) ) ) );
      }
      else {
        func.reset( new function_holder( nullptr, type_erased_function( std::forward< F >( f ) ) ) );
      }
    }
    std::unique_ptr< function_holder > func;
  };
}

#endif

