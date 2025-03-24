#ifndef GCT_HANDLER_HPP
#define GCT_HANDLER_HPP

#include <memory>

namespace gct {
template< typename T, typename Tag = void >
class handler {
public:
  using weak_type = std::weak_ptr< T >;
  handler() {}
  template< typename F >
  handler(
    T *v,
    F &&f
  ) : value( v, std::forward< F >( f ) ) {}
  handler( const std::weak_ptr< T > &w ) :
    value( w ) {}
  handler( const handler& ) = default;
  handler( handler&& ) = default;
  handler &operator=( const handler& ) = default;
  handler &operator=( handler&& ) = default;
  const T &operator*() const {
    return *value;
  }
  [[nodiscard]] weak_type get_weak() const {
    return value;
  }
  [[nodiscard]] operator bool() const {
    return bool( value );
  }
private:
  std::shared_ptr< T > value;
};

}

namespace std {

template< typename T, typename Tag >
struct hash< gct::handler< T, Tag > > {
  auto operator()( const gct::handler< T, Tag > &v ) const -> std::size_t {
    return std::hash< T >{}( T( *v ) );
  }
};

}


#endif

