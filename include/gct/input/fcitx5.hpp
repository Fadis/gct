#ifndef GCT_INPUT_FCITX5_HPP
#define GCT_INPUT_FCITX5_HPP
#include <string>
#include <tuple>
#include <exception>
#include <memory>
#include <vector>
#include <utility>
#include <unordered_set>
#include <mutex>
#include <optional>
#include <gct/input/key_state.hpp>
#include <gct/input/xkbcommon.hpp>
#include <gct/dbus/dbus.hpp>
#include <gct/sched/thread_pool.hpp>
#include <gct/future.hpp>

namespace gct::input {

struct fcitx5_client_ui {
  std::string message;
  std::vector< std::string > cands;
  std::int32_t page = 0u;
  std::int32_t selected = 0u;
};
struct fcitx5_preedit {
  std::vector< std::string > cands;
  std::int32_t selected = 0u;
};

class fcitx5 final {
class fcitx5_internal final : public std::enable_shared_from_this< fcitx5_internal > {
public:
  fcitx5_internal(
    const std::shared_ptr< sched::thread_pool_t > &tp
  );
  void run();
  void key_event(
    const xkb_key_event &e
  );
  future< std::tuple< std::string, std::string, std::string > > get_current_im();
  future< std::vector< std::uint32_t > > get_forwarded();
  future< std::string > get_commited();
  future< fcitx5_preedit > get_preedit();
  future< fcitx5_client_ui > get_client_ui();
private:
  void poll();
  std::uint32_t get_state();
  std::shared_ptr< sched::thread_pool_t > thread_pool;
  std::unique_ptr< sdbus::IConnection > conn;
  std::unique_ptr< sdbus::IConnection > conn2;
  std::unique_ptr<sdbus::IProxy> input_method;
  std::unique_ptr<sdbus::IProxy> input_context;
  std::mutex guard;
  std::unordered_set< unsigned int > state;
  std::optional< promise< std::tuple< std::string, std::string, std::string > > > current_im_p;
  std::optional< std::tuple< std::string, std::string, std::string > > current_im_v;
  std::optional< promise< std::vector< std::uint32_t > > > forward_p;
  std::vector< std::uint32_t > forward_v;
  std::optional< promise< std::string > > commit_p;
  std::string commit_v;
  std::optional< promise< fcitx5_preedit > > preedit_p;
  std::optional< fcitx5_preedit > preedit_v;
  std::optional< promise< fcitx5_client_ui > > client_ui_p;
  std::optional< fcitx5_client_ui > client_ui_v;
};
public:
  fcitx5(
    const std::shared_ptr< sched::thread_pool_t > &tp
  );
  fcitx5( const fcitx5& ) = delete;
  fcitx5( fcitx5&& ) = delete;
  fcitx5 &operator=( const fcitx5& ) = delete;
  fcitx5 &operator=( fcitx5&& ) = delete;
  void run();
  void key_event(
    const xkb_key_event &e
  );
  future< std::tuple< std::string, std::string, std::string > > get_current_im();
  future< std::vector< std::uint32_t > > get_forwarded();
  future< std::string > get_commited();
  future< fcitx5_preedit > get_preedit();
  future< fcitx5_client_ui > get_client_ui();
private:
  std::shared_ptr< fcitx5_internal > internal;
};

}

#endif

