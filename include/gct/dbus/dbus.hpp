#ifndef GCT_DBUS_DBUS_HPP
#define GCT_DBUS_DBUS_HPP
#include <tuple>
#include <memory>
#include <exception>
#include <sdbus-c++/sdbus-c++.h>
#include <gct/future.hpp>

namespace gct::dbus {

template< typename ...T >
future< std::tuple< T... > > call(
  sdbus::AsyncMethodInvoker &async
) {
  std::shared_ptr< promise< std::tuple< T... > > > p( new promise< std::tuple< T... > >() );
  auto f = p->get_future();
  async.uponReplyInvoke(
    [p=std::move(p)]( const sdbus::Error* error, const T & ... v ) {
      if( error ) {
        try {
          throw *error;
        }
        catch( ... ) {
          p->set_exception( std::current_exception() );
        }
      }
      else {
        p->set_value( std::make_tuple( T( v )... ) );
      }
    }
  );
  return f;
}

}

#endif

