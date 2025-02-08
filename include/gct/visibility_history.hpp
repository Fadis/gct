#ifndef GCT_VISIBILITY_HISTORY_HPP
#define GCT_VISIBILITY_HISTORY_HPP
#include <vector>
#include <unordered_map>
#include <gct/instance_list.hpp>

namespace gct::scene_graph {

enum class visibility_state {
  visible,
  invisible,
  unknown
};

class visibility_history {
public:
  struct state_type {
    unsigned int frame_count = 0u;
    visibility_state state = visibility_state::unknown;
  };
  visibility_history( const std::vector< resource_pair > &il ) {
    for( const auto &v: il ) {
      state.insert( std::make_pair( *v.inst, state_type() ) );
    }
  }
  void record( const std::unordered_set< unsigned int > &visible ) {
    for( auto &v: state ) {
      if( visible.find( v.first ) != visible.end() ) {
        if( v.second.state == visibility_state::visible ) {
          ++v.second.frame_count;
        }
        else {
          v.second.state = visibility_state::visible;
          v.second.frame_count = 0u;
        }
      }
      else {
        if( v.second.state == visibility_state::invisible ) {
          ++v.second.frame_count;
        }
        else {
          v.second.state = visibility_state::invisible;
          v.second.frame_count = 0u;
        }
      }
    }
  }
  [[nodiscard]] const std::unordered_map< unsigned int, state_type > &get() const {
    return state;
  }
private:
  std::unordered_map< unsigned int, state_type > state;
};

}

#endif

