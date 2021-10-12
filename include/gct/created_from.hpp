#ifndef LIBGCT_INCLUDE_GCT_CREATED_FROM_HPP
#define LIBGCT_INCLUDE_GCT_CREATED_FROM_HPP

#include <memory>

namespace gct {
  template< typename T >
  struct created_from {
  public:
    created_from( const std::shared_ptr< T > &v_ ) : v( v_ ) {}
    const std::shared_ptr< T > &get_factory() const {
      return v;
    }
  private:
    std::shared_ptr< T > v;
  };
}
#endif

