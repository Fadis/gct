#ifndef GCT_HANDLER_HPP
#define GCT_HANDLER_HPP

#include <memory>

namespace gct {
template< typename T >
class handler {
public:
  using weak_type = std::weak_ptr< T >;
  template< typename ... Args >
  handler(
    Args&& ... v
  ) : value( std::forward< Args >( v )... ) {}
  const T &operator*() const {
    return *value;
  }
  weak_type get_weak() const {
    return value;
  }
  operator bool() const {
    return bool( value );
  }
private:
  std::shared_ptr< T > value;
};
}

#endif

