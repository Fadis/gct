#ifndef GCT_INPUT_INPUT_BUFFER_HPP
#define GCT_INPUT_INPUT_BUFFER_HPP

#include <exception>
#include <memory>
#include <vector>
#include <utility>
#include <optional>
#include <gct/input/libinput.hpp>
#include <gct/non_copyable_function.hpp>
#include <gct/future.hpp>

namespace gct::input {


template< libinput_event_category cat, typename T >
class input_buffer final {
class input_buffer_internal final : public std::enable_shared_from_this< input_buffer_internal > {
public:
  input_buffer_internal(
    const std::shared_ptr< gct::input::libinput_t > &i,
    non_copyable_function< void( std::vector< T > &, libinput_event_type_t< cat >* ) > filter_
  ) : libinput( i ), filter( std::move( filter_ ) ), end( false ), current_id( 0u ) {}
  void on_event( libinput_event_type_t< cat > *event, std::exception_ptr err ) {
    std::vector< T > temp;
    std::optional< promise< std::vector< T > > > p_temp;
    if( !err && event ) {
      {
        std::scoped_lock< std::mutex > lock( guard );
        filter( buffer, event );
        if( !buffer.empty() && p ) {
          p_temp = std::move( p );
          temp = std::move( buffer );
          p = std::nullopt;
        }
      }
      if( p_temp ) {
        p_temp->set_value( std::move( temp ) );
      }
    }
    else if( err ) {
      {
        std::scoped_lock< std::mutex > lock( guard );
        if( p ) {
          p_temp = std::move( p );
          p = std::nullopt;
        }
      }
      try {
        std::rethrow_exception( err );
      }
      catch( const std::system_error &e ) {
        if( std::errc( e.code().value() ) == std::errc::operation_canceled ) {
          close();
        }
        if( p_temp ) {
          p_temp->set_exception( std::current_exception() );
        }
      }
      catch( ... ) {
        if( p_temp ) {
          p_temp->set_exception( std::current_exception() );
        }
      }
    }
    else {
      assert( event != nullptr );
    }
  }
  future< std::vector< T > > get_future() {
    future< std::vector< T > > f;
    std::optional< promise< std::vector< T > > > old_p;
    std::uint64_t id = 0u;
    {
      std::scoped_lock< std::mutex > lock( guard );
      if( p ) old_p = std::move( p );
      p = promise< std::vector< T > >();
      id = ++current_id;
      f = p->get_future();
      if( !buffer.empty() ) {
        p->set_value( std::move( buffer ) );
        p = std::nullopt;
      }
    }
    if( old_p ) {
      try {
        throw std::system_error( std::make_error_code( std::errc::operation_canceled ), "input_buffer::get_future : canceled." );
      }
      catch( ... ) {
        old_p->set_exception( std::current_exception() );
      }
    }
    f.set_cancel( [self=std::enable_shared_from_this< input_buffer_internal >::shared_from_this(),id=id]() {
      self->close( id );
    } );
    return f;
  }
  void close( std::uint64_t id ) {
    std::optional< promise< std::vector< T > > > old_p;
    {
      std::scoped_lock< std::mutex > lock( guard );
      if( id == current_id ) {
        if( p ) {
          old_p = std::move( p );
          p = std::nullopt;
        }
      }
    }
    if( old_p ) {
      try {
        throw std::system_error( std::make_error_code( std::errc::operation_canceled ), "input_buffer::get_future : canceled." );
      }
      catch( ... ) {
        old_p->set_exception( std::current_exception() );
      }
    }
  }
  void close() {
    std::optional< promise< std::vector< T > > > old_p;
    {
      std::scoped_lock< std::mutex > lock( guard );
      if( p ) {
        old_p = std::move( p );
        p = std::nullopt;
      }
    }
    if( old_p ) {
      try {
        throw std::system_error( std::make_error_code( std::errc::operation_canceled ), "input_buffer::get_future : canceled." );
      }
      catch( ... ) {
        old_p->set_exception( std::current_exception() );
      }
    }
  }
private:
  std::shared_ptr< gct::input::libinput_t > libinput;
  std::mutex guard;
  non_copyable_function< void( std::vector< T > &, libinput_event_type_t< cat >* ) > filter;
  std::atomic< bool > end;
  std::optional< promise< std::vector< T > > > p;
  std::vector< T > buffer;
  std::uint64_t current_id;
};
public:
  input_buffer(
    const std::shared_ptr< gct::input::libinput_t > &libinput,
    non_copyable_function< void( std::vector< T > &, libinput_event_type_t< cat >* ) > &&filter_
  ) : internal( new input_buffer_internal( libinput, std::move( filter_ ) ) ) {
    conn = libinput->add< cat >( [self=internal]( libinput_event_type_t< cat > *event, auto err ){ self->on_event( event, err ); } );
  }
  future< std::vector< T > > get_future() {
    return internal->get_future();
  }
  input_buffer( const input_buffer& ) = delete;
  input_buffer( input_buffer&& ) = delete;
  input_buffer &operator=( const input_buffer& ) = delete;
  input_buffer &operator=( input_buffer && ) = delete;
  ~input_buffer() {
    internal->close();
  }
private:
  std::shared_ptr< input_buffer_internal > internal;
  std::shared_ptr< void > conn;
};
}

#endif

