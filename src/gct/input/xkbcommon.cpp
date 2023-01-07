#include <exception>
#include <memory>
#include <vector>
#include <iostream>
#include <utility>
#include <gct/input/libinput.hpp>
#include <gct/input/key_state.hpp>
#include <gct/input/xkbcommon.hpp>
#include <gct/future.hpp>

namespace gct::input {


void xkb_context_deleter::operator()( xkb_context *p ) const {
  if( p ) xkb_context_unref( p );
}
void xkb_keymap_deleter::operator()( xkb_keymap *p ) const {
  if( p ) xkb_keymap_unref( p );
}
void xkb_state_deleter::operator()( ::xkb_state *p ) const {
  if( p ) xkb_state_unref( p );
}


xkb_key_direction gct_key_state_to_xkb_key_state( key_state s ) {
  if( s == key_state::released ) return xkb_key_direction::XKB_KEY_UP;
  else if( s == key_state::pressed ) return xkb_key_direction::XKB_KEY_DOWN;
  assert( s == key_state::released || s == key_state::pressed );
  return xkb_key_direction::XKB_KEY_UP;
}

xkb_state::xkb_state_internal::xkb_state_internal(
  const xkb_rule_names &names
) : current_id( 0u ) {
  auto raw_ctx = xkb_context_new( XKB_CONTEXT_NO_FLAGS );
  if( !raw_ctx ) {
    throw std::system_error( std::make_error_code( std::errc::not_supported ), "xkbcommon_t::xkbcommon_t : xkb_context_new failed." );
  }
  ctx.reset( raw_ctx );
  auto raw_keymap = xkb_keymap_new_from_names( ctx.get(), &names, XKB_KEYMAP_COMPILE_NO_FLAGS);
  if( !raw_keymap ) {
    throw std::system_error( std::make_error_code( std::errc::not_supported ), "xkbcommon_t::xkbcommon_t : xkb_keymap_new_from_names failed." );
  }
  keymap.reset( raw_keymap );
  auto raw_state = xkb_state_new( keymap.get() );
  if( !raw_state ) {
    throw std::system_error( std::make_error_code( std::errc::not_supported ), "xkbcommon_t::xkbcommon_t : xkb_state_new failed." );
  }
  state.reset( raw_state );
}
void xkb_state::xkb_state_internal::key_event( const xkb_key_event &e ) {
  std::vector< xkb_key_event > syms_temp;
  std::optional< promise< std::vector< xkb_key_event > > > p_temp;
  {
    std::scoped_lock< std::mutex > lock( guard );
    xkb_state_update_key(
      state.get(),
      e.code,
      gct_key_state_to_xkb_key_state( e.state )
    );
    const auto keysym = xkb_state_key_get_one_sym( state.get(), e.code );
    syms.push_back( xkb_key_event{ keysym, e.code, e.state, e.relative_time } );
    if( p ) {
      p_temp = std::move( p );
      p = std::nullopt;
      syms_temp = std::move( syms );
      syms.clear();
    }
  }
  if( p_temp ) {
    p_temp->set_value( std::move( syms_temp ) );
  }
}
future< std::vector< xkb_key_event > > xkb_state::xkb_state_internal::get_future() {
  future< std::vector< xkb_key_event > > f;
  std::optional< promise< std::vector< xkb_key_event > > > old_p;
  std::uint64_t id = 0u;
  {
    std::scoped_lock< std::mutex > lock( guard );
    if( p ) old_p = std::move( p );
    p = promise< std::vector< xkb_key_event > >();
    id = ++current_id;
    f = p->get_future();
    if( !syms.empty() ) {
      p->set_value( std::move( syms ) );
      p = std::nullopt;
    }
  }
  f.set_cancel( [self=shared_from_this()]() { self->close(); } );
  if( old_p ) {
    try {
      throw std::system_error( std::make_error_code( std::errc::operation_canceled ), "xkb_state::get_future : canceled." );
    }
    catch( ... ) {
      old_p->set_exception( std::current_exception() );
    }
  }
  f.set_cancel( [self=shared_from_this(),id=id]() {
    self->close( id );
  } );
  return f;
}
void xkb_state::xkb_state_internal::close( std::uint64_t id ) {
  std::optional< promise< std::vector< xkb_key_event > > > old_p;
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
      throw std::system_error( std::make_error_code( std::errc::operation_canceled ), "xkb_state::get_future : canceled." );
    }
    catch( ... ) {
      old_p->set_exception( std::current_exception() );
    }
  }
}
void xkb_state::xkb_state_internal::close() {
  std::optional< promise< std::vector< xkb_key_event > > > old_p;
  {
    std::scoped_lock< std::mutex > lock( guard );
    if( p ) {
      old_p = std::move( p );
      p = std::nullopt;
    }
  }
  if( old_p ) {
    try {
      throw std::system_error( std::make_error_code( std::errc::operation_canceled ), "xkb_state::get_future : canceled." );
    }
    catch( ... ) {
      old_p->set_exception( std::current_exception() );
    }
  }
}
xkb_state::xkb_state(
  const xkb_rule_names &names
) : internal( new xkb_state_internal( names ) ) {}
void xkb_state::key_event( const xkb_key_event &e ) {
  internal->key_event( e );
}
future< std::vector< xkb_key_event > > xkb_state::get_future() {
  return internal->get_future();
}
xkb_state::~xkb_state() {
  internal->close();
}
}

