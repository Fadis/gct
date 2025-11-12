#ifndef GCT_HANDLER_HPP
#define GCT_HANDLER_HPP

#include <memory>
#include <gct/spv_member_pointer.hpp>
#include <gct/exception.hpp>

namespace gct {
template< typename T, typename Tag = void >
class typed_handler {
  struct detail {
    detail(
      spv_member_pointer mp_,
      std::size_t array_size_,
      T id_
    ) : mp( mp_ ), array_size( array_size_ ), id( id_ ) {}
    spv_member_pointer mp;
    std::size_t array_size = 1u;
    T id;
  };
public:
  using weak_type = std::weak_ptr< T >;
  typed_handler() {}
  template< typename F >
  typed_handler(
    spv_member_pointer mp_,
    std::size_t array_size_,
    T id_,
    F &&f
  ) : value( detail{ mp_, array_size_, id_ }, std::forward< F >( f ) ) {}
  typed_handler( const std::weak_ptr< T > &w ) :
    value( w ) {}
  typed_handler( const typed_handler& ) = default;
  typed_handler( typed_handler&& ) = default;
  typed_handler &operator=( const typed_handler& ) = default;
  typed_handler &operator=( typed_handler&& ) = default;
  const T &get_id() const {
    return value->id;
  }
  const spv_member_pointer &get_member_pointer() const {
    return value->mp;
  }
  const std::size_t &size() const {
    return value->array_size;
  }
  const spv_member_pointer &operator[]( int i ) const {
    if( i >= value->array_size ) {
      throw exception::out_of_range( "spv_member_pointer::operator[] index >= array_size", __FILE__, __LINE__ );
    }
    return value->mp[ i ];
  }
  [[nodiscard]] weak_type get_weak() const {
    return value;
  }
  [[nodiscard]] operator bool() const {
    return bool( value );
  }
private:
  std::shared_ptr< detail > value;
};

}

/*namespace std {

template< typename T, typename Tag >
struct hash< gct::typed_handler< T, Tag > > {
  auto operator()( const gct::handler< T, Tag > &v ) const -> std::size_t {
    return std::hash< T >{}( T( *v ) );
  }
};

}
*/

#endif

