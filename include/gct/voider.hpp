#ifndef GCT_VOIDER_HPP
#define GCT_VOIDER_HPP

namespace gct {

template< typename ...Args >
struct voider_t {
  using type = void;
};
template< typename ...Args >
using void_t = typename voider_t< Args... >::type;

}

#endif
