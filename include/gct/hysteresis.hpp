#ifndef GCT_HYSTERESIS_HPP
#define GCT_HYSTERESIS_HPP
#include <cstdint>
#include <memory>
#include <vulkan/vulkan.hpp>
#include <gct/image_create_info.hpp>
#include <gct/image_view.hpp>
#include <gct/image_layout.hpp>

namespace gct {
template< typename T >
class hysteresis {
public:
  hysteresis(
    T value_,
    unsigned int up_samples_,
    unsigned int down_samples_
  ) : value( value_ ), up_samples( up_samples_ ), down_samples( down_samples_ ) {}
  void set( T new_value ) {
    if( new_value > value ) {
      value = ( new_value + value * ( up_samples - 1u ) ) / ( up_samples );
    }
    else {
      value = ( new_value + value * ( down_samples - 1u ) ) / ( down_samples );
    }
  }
  T get() const {
    return value;
  }
private:
  T value;
  unsigned int up_samples;
  unsigned int down_samples;
};
}

#endif


