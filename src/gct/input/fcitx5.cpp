#include <cstdint>
#include <exception>
#include <memory>
#include <vector>
#include <iostream>
#include <utility>
#include <gct/input/libinput.hpp>
#include <gct/input/key_state.hpp>
#include <gct/future.hpp>
#include <gct/sched/sleep.hpp>
#include <gct/dbus/dbus.hpp>
#include <gct/sched/thread_pool.hpp>
#include <gct/sched/wait.hpp>
#include <gct/input/fcitx5.hpp>

namespace gct::input {

namespace {
#ifdef GCT_SDBUSCPP_STRONG_TYPE
  using error_type = std::optional< sdbus::Error >;
#else
  using error_type = sdbus::Error*;
#endif
}

fcitx5::fcitx5_internal::fcitx5_internal(
  const std::shared_ptr< sched::thread_pool_t > &tp
) : thread_pool( tp ) {
 conn = sdbus::createSessionBusConnection();
#ifdef GCT_SDBUSCPP_STRONG_TYPE
 input_method = sdbus::createProxy( *conn, sdbus::ServiceName( "org.fcitx.Fcitx5" ), sdbus::ObjectPath( "/org/freedesktop/portal/inputmethod" ) );
#else
 input_method = sdbus::createProxy( *conn, "org.fcitx.Fcitx5", "/org/freedesktop/portal/inputmethod" );
#endif
 conn->enterEventLoopAsync();
}
void fcitx5::fcitx5_internal::run() {
  thread_pool->add_co(
    [self=shared_from_this()]() {
      self->poll();
    }
  );
}
void fcitx5::fcitx5_internal::poll() {
  auto [version] = sched::wait(
    gct::dbus::call< error_type, std::uint32_t >(
      input_method->callMethodAsync( "Version" )
        .onInterface( "org.fcitx.Fcitx.InputMethod1" )
        .withArguments()
    )
  );
  std::cout << "fcitx5 version : " << version << std::endl;
  std::vector< sdbus::Struct< std::string, std::string > > args{
    sdbus::make_struct( std::string( "program" ), std::string( "hoge" ) ),
    sdbus::make_struct( std::string( "display" ), std::string( "x11:" ) )
  };
  auto [path,wtf] = sched::wait(
    gct::dbus::call< error_type, sdbus::ObjectPath, std::vector< std::uint8_t > >(
      input_method->callMethodAsync( "CreateInputContext" )
        .onInterface( "org.fcitx.Fcitx.InputMethod1" )
        .withArguments( args )
    )
  );
#ifdef GCT_SDBUSCPP_STRONG_TYPE
  input_context = sdbus::createProxy( *conn, sdbus::ServiceName( "org.fcitx.Fcitx5" ), sdbus::ObjectPath( path ) );
#else
  input_context = sdbus::createProxy( *conn, "org.fcitx.Fcitx5", path );
#endif
  input_context->uponSignal( "CommitString" )
    .onInterface( "org.fcitx.Fcitx.InputContext1" )
    .call( [self=shared_from_this()]( const std::string &s ) {
      std::optional< promise< std::string > > p;
      {
        std::scoped_lock< std::mutex > lock( self->guard );
        if( self->commit_p ) {
          p = std::move( *self->commit_p );
          self->commit_p = std::nullopt;
        }
        else {
          self->commit_v += s;
        }
      }
      if( p ) {
        p->set_value( s );
      }
    } );
  input_context->uponSignal( "CurrentIM" )
    .onInterface( "org.fcitx.Fcitx.InputContext1" )
    .call( [self=shared_from_this()]( const std::string &a, const std::string &b, const std::string &c ) {
      std::optional< promise< std::tuple< std::string, std::string, std::string > > > p;
      {
        std::scoped_lock< std::mutex > lock( self->guard );
        if( self->current_im_p ) {
          p = std::move( *self->current_im_p );
          self->current_im_p = std::nullopt;
        }
        else {
          self->current_im_v = std::make_tuple( a, b, c );
        }
      }
      if( p ) {
        p->set_value( std::make_tuple( a, b, c ) ) ;
      }
    } );
  input_context->uponSignal( "DeleteSurroundingText" )
    .onInterface( "org.fcitx.Fcitx.InputContext1" )
    .call( [self=shared_from_this()]( std::int32_t a, const std::string &b ) {
      std::cout << "DeleteSurroundingText : " << a << " " << b << std::endl;
    } );
  input_context->uponSignal( "ForwardKey" )
    .onInterface( "org.fcitx.Fcitx.InputContext1" )
    .call( [self=shared_from_this()]( std::uint32_t a, std::uint32_t b, bool c ) {
      std::optional< promise< std::vector< std::uint32_t > > > p;
      {
        std::scoped_lock< std::mutex > lock( self->guard );
        if( self->forward_p ) {
          p = std::move( *self->forward_p );
          self->forward_p = std::nullopt;
        }
        else {
          self->forward_v.push_back( a );
        }
      }
      if( p ) {
        p->set_value( std::vector< std::uint32_t >{ a } );
      }
    } );
  input_context->uponSignal( "UpdateClientSideUI" )
    .onInterface( "org.fcitx.Fcitx.InputContext1" )
    .call( [self=shared_from_this()](
      const std::vector< sdbus::Struct< std::string, std::int32_t > > &a,
      std::int32_t b,
      const std::vector< sdbus::Struct< std::string, std::int32_t > > &c,
      const std::vector< sdbus::Struct< std::string, std::int32_t > > &d,
      const std::vector< sdbus::Struct< std::string, std::string > > &e,
      std::int32_t f,
      std::int32_t g,
      bool h,
      bool i
    ) {
      fcitx5_client_ui v;
      v.message = c.empty() ? std::string() : c[ 0 ].get< 0 >();
      v.selected = f;
      v.page = h;
      for( auto &x: e ) {
        v.cands.push_back( x.get< 0 >() );
      }
      std::optional< promise< fcitx5_client_ui > > p;
      {
        std::scoped_lock< std::mutex > lock( self->guard );
        if( self->client_ui_p ) {
          p = std::move( *self->client_ui_p );
          self->client_ui_p = std::nullopt;
        }
        else {
          self->client_ui_v = v;
        }
      }
      if( p ) {
        p->set_value( v );
      }
    } );
  input_context->uponSignal( "UpdateFormattedPreedit" )
    .onInterface( "org.fcitx.Fcitx.InputContext1" )
    .call( [self=shared_from_this()](
      const std::vector< sdbus::Struct< std::string, std::int32_t > > &a,
      std::int32_t b
    ) {
      fcitx5_preedit v;
      v.selected = b;
      for( auto &x: a ) {
        v.cands.push_back( x.get< 0 >() );
      }
      std::optional< promise< fcitx5_preedit > > p;
      {
        std::scoped_lock< std::mutex > lock( self->guard );
        if( self->preedit_p ) {
          p = std::move( *self->preedit_p );
          self->preedit_p = std::nullopt;
        }
        else {
          self->preedit_v = v;
        }
      }
      if( p ) {
        p->set_value( v );
      }
    } );
#ifndef GCT_SDBUSCPP_STRONG_TYPE
  input_context->finishRegistration();
#endif
  sched::wait(
    gct::dbus::call< error_type >(
      input_context->callMethodAsync( "SetCursorRect" )
        .onInterface( "org.fcitx.Fcitx.InputContext1" )
        .withArguments( 128, 128, 8, 17 )
    )
  );
  sched::wait(
    gct::dbus::call< error_type >(
      input_context->callMethodAsync( "SetCapability" )
        .onInterface( "org.fcitx.Fcitx.InputContext1" )
        .withArguments( 962081063026u )
    )
  );
  sched::wait(
    gct::dbus::call< error_type >(
      input_context->callMethodAsync( "FocusIn" )
        .onInterface( "org.fcitx.Fcitx.InputContext1" )
    )
  );

}
void fcitx5::fcitx5_internal::key_event(
  const xkb_key_event &e
) {
  std::uint32_t key_state = 0u;
  {
    std::scoped_lock< std::mutex > lock( guard );
    if( e.state == key_state::released ) {
      state.erase( e.sym );
    }
    else {
      state.insert( e.sym );
    }
    key_state = get_state();
  }
  thread_pool->add_co(
    [self=shared_from_this(),e=e,key_state=key_state]() {
      if( self->input_context ) {
        auto [result] = sched::wait(
          gct::dbus::call< error_type, bool >(
            self->input_context->callMethodAsync( "ProcessKeyEvent" )
              .onInterface( "org.fcitx.Fcitx.InputContext1" )
              .withArguments( std::uint32_t( e.sym ), std::uint32_t( e.code ), std::uint32_t( key_state ), bool( e.state == key_state::released ), std::uint32_t( e.relative_time.count() ) )
          )
        );
        if( !result ) {
          std::cout << "ProcessKeyEvent failed : " << result << std::endl;
        }
      }
    }
  );
}

future< std::tuple< std::string, std::string, std::string > > fcitx5::fcitx5_internal::get_current_im() {
  future< std::tuple< std::string, std::string, std::string > > f;
  std::optional< promise< std::tuple< std::string, std::string, std::string > > > old_p;
  {
    std::scoped_lock< std::mutex > lock( guard );
    if( current_im_p ) old_p = std::move( current_im_p );
    current_im_p = promise< std::tuple< std::string, std::string, std::string > >();
    f = current_im_p->get_future();
    if( current_im_v ) {
      current_im_p->set_value( std::move( *current_im_v ) );
      current_im_p = std::nullopt;
      current_im_v = std::nullopt;
    }
  }
  if( old_p ) {
    try {
      throw std::system_error( std::make_error_code( std::errc::operation_canceled ), "fcitx5::get_current_im : canceled." );
    }
    catch( ... ) {
      old_p->set_exception( std::current_exception() );
    }
  }
  f.set_cancel( [self=shared_from_this()]() {
    std::scoped_lock< std::mutex > lock( self->guard );
    self->current_im_p = std::nullopt;
  } );
  return f;
}
future< std::vector< std::uint32_t > > fcitx5::fcitx5_internal::get_forwarded() {
  future< std::vector< std::uint32_t > > f;
  std::optional< promise< std::vector< std::uint32_t > > > old_p;
  {
    std::scoped_lock< std::mutex > lock( guard );
    if( forward_p ) old_p = std::move( forward_p );
    forward_p = promise< std::vector< std::uint32_t > >();
    f = forward_p->get_future();
    if( !forward_v.empty() ) {
      forward_p->set_value( std::move( forward_v ) );
      forward_p = std::nullopt;
      forward_v.clear();
    }
  }
  if( old_p ) {
    try {
      throw std::system_error( std::make_error_code( std::errc::operation_canceled ), "fcitx5::get_current_im : canceled." );
    }
    catch( ... ) {
      old_p->set_exception( std::current_exception() );
    }
  }
  f.set_cancel( [self=shared_from_this()]() {
    std::scoped_lock< std::mutex > lock( self->guard );
    self->forward_p = std::nullopt;
  } );
  return f;
}
future< std::string > fcitx5::fcitx5_internal::get_commited() {
  future< std::string > f;
  std::optional< promise< std::string > > old_p;
  {
    std::scoped_lock< std::mutex > lock( guard );
    if( commit_p ) old_p = std::move( commit_p );
    commit_p = promise< std::string >();
    f = commit_p->get_future();
    if( !commit_v.empty() ) {
      commit_p->set_value( std::move( commit_v ) );
      commit_p = std::nullopt;
      commit_v.clear();
    }
  }
  if( old_p ) {
    try {
      throw std::system_error( std::make_error_code( std::errc::operation_canceled ), "fcitx5::get_current_im : canceled." );
    }
    catch( ... ) {
      old_p->set_exception( std::current_exception() );
    }
  }
  f.set_cancel( [self=shared_from_this()]() {
    std::scoped_lock< std::mutex > lock( self->guard );
    self->commit_p = std::nullopt;
  } );
  return f;
}
future< fcitx5_preedit > fcitx5::fcitx5_internal::get_preedit() {
  future< fcitx5_preedit > f;
  std::optional< promise< fcitx5_preedit > > old_p;
  {
    std::scoped_lock< std::mutex > lock( guard );
    if( preedit_p ) old_p = std::move( preedit_p );
    preedit_p = promise< fcitx5_preedit >();
    f = preedit_p->get_future();
    if( preedit_v ) {
      preedit_p->set_value( std::move( *preedit_v ) );
      preedit_p = std::nullopt;
      preedit_v = std::nullopt;
    }
  }
  if( old_p ) {
    try {
      throw std::system_error( std::make_error_code( std::errc::operation_canceled ), "fcitx5::get_current_im : canceled." );
    }
    catch( ... ) {
      old_p->set_exception( std::current_exception() );
    }
  }
  f.set_cancel( [self=shared_from_this()]() {
    std::scoped_lock< std::mutex > lock( self->guard );
    self->preedit_p = std::nullopt;
  } );
  return f;
}
future< fcitx5_client_ui > fcitx5::fcitx5_internal::get_client_ui() {
  future< fcitx5_client_ui > f;
  std::optional< promise< fcitx5_client_ui > > old_p;
  {
    std::scoped_lock< std::mutex > lock( guard );
    if( client_ui_p ) old_p = std::move( client_ui_p );
    client_ui_p = promise< fcitx5_client_ui >();
    f = client_ui_p->get_future();
    if( client_ui_v ) {
      client_ui_p->set_value( std::move( *client_ui_v ) );
      client_ui_p = std::nullopt;
      client_ui_v = std::nullopt;
    }
  }
  if( old_p ) {
    try {
      throw std::system_error( std::make_error_code( std::errc::operation_canceled ), "fcitx5::get_current_im : canceled." );
    }
    catch( ... ) {
      old_p->set_exception( std::current_exception() );
    }
  }
  f.set_cancel( [self=shared_from_this()]() {
    std::scoped_lock< std::mutex > lock( self->guard );
    self->client_ui_p = std::nullopt;
  } );
  return f;
}
future< std::tuple< std::string, std::string, std::string > > fcitx5::get_current_im() {
  return internal->get_current_im();
}
future< std::vector< std::uint32_t > > fcitx5::get_forwarded() {
  return internal->get_forwarded();
}
future< std::string > fcitx5::get_commited() {
  return internal->get_commited();
}
future< fcitx5_preedit > fcitx5::get_preedit() {
  return internal->get_preedit();
}
future< fcitx5_client_ui > fcitx5::get_client_ui() {
  return internal->get_client_ui();
}

fcitx5::fcitx5(
  const std::shared_ptr< sched::thread_pool_t > &tp
) : internal( new fcitx5_internal( tp ) ) {}
void fcitx5::run() {
  internal->run();
}
void fcitx5::key_event(
  const xkb_key_event &e
) {
  internal->key_event( e );
}

std::uint32_t fcitx5::fcitx5_internal::get_state() {
  std::uint32_t value = 0u;
  for( auto v: state ) {
    if( v == XKB_KEY_Shift_L ) {
      value |= ( 1 << 0 );
    }
    else if( v == XKB_KEY_Shift_R ) {
      value |= ( 1 << 0 );
    }
    else if( v == XKB_KEY_Caps_Lock ) {
      value |= ( 1 << 1 );
    }
    else if( v == XKB_KEY_Control_L ) {
      value |= ( 1 << 2 );
    }
    else if( v == XKB_KEY_Control_R ) {
      value |= ( 1 << 2 );
    }
    else if( v == XKB_KEY_Alt_L ) {
      value |= ( 1 << 3 );
    }
    else if( v == XKB_KEY_Alt_R ) {
      value |= ( 1 << 3 );
    }
    else if( v == XKB_KEY_Num_Lock ) {
      value |= ( 1 << 4 );
    }
    else if( v == XKB_KEY_Hyper_L ) {
      value |= ( 1 << 5 );
    }
    else if( v == XKB_KEY_Hyper_R ) {
      value |= ( 1 << 5 );
    }
    else if( v == XKB_KEY_Super_L ) {
      value |= ( 1 << 6 );
    }
    else if( v == XKB_KEY_Super_R ) {
      value |= ( 1 << 6 );
    }
    else if( v == XKB_KEY_Meta_L ) {
      value |= ( 1 << 28 );
    }
    else if( v == XKB_KEY_Meta_R ) {
      value |= ( 1 << 28 );
    }
  }
  return value;
}

}

