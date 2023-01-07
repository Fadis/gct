#ifndef GCT_INPUT_XKBCOMMON_HPP
#define GCT_INPUT_XKBCOMMON_HPP

#include <exception>
#include <memory>
#include <vector>
#include <utility>
#include <xkbcommon/xkbcommon.h>
#include <gct/input/key_state.hpp>
#include <gct/future.hpp>

namespace gct::input {


struct xkb_context_deleter {
  void operator()( xkb_context *p ) const;
};
struct xkb_keymap_deleter {
  void operator()( xkb_keymap *p ) const;
};
struct xkb_state_deleter {
  void operator()( ::xkb_state *p ) const;
};

struct xkb_key_event {
  std::uint32_t sym = 0u;
  std::uint32_t code = 0u;
  key_state state = key_state::released;
  std::chrono::microseconds relative_time;
};


xkb_key_direction gct_key_state_to_xkb_key_state( key_state s );

class xkb_state final {
class xkb_state_internal final : public std::enable_shared_from_this< xkb_state_internal > {
public:
  xkb_state_internal(
    const xkb_rule_names &names
  );
  void key_event( const xkb_key_event &e );
  future< std::vector< xkb_key_event > > get_future();
  void close( std::uint64_t id );
  void close();
private:
  std::unique_ptr< xkb_context, xkb_context_deleter > ctx;
  std::unique_ptr< xkb_keymap, xkb_keymap_deleter > keymap;
  std::unique_ptr< ::xkb_state, xkb_state_deleter > state;
  std::atomic< std::uint64_t > currentid;
  std::mutex guard;
  std::optional< promise< std::vector< xkb_key_event > > > p;
  std::vector< xkb_key_event > syms;
  std::uint64_t current_id;
};
public:
  xkb_state(
    const xkb_rule_names &names
  );
  xkb_state( const xkb_state& ) = delete;
  xkb_state( xkb_state&& ) = delete;
  xkb_state &operator=( const xkb_state& ) = delete;
  xkb_state &operator=( xkb_state && ) = delete;
  void key_event( const xkb_key_event &e );
  future< std::vector< xkb_key_event > > get_future();
  ~xkb_state();
private:
  std::shared_ptr< xkb_state_internal > internal;
};
}

#endif

