#ifndef GCT_TONE_MAPPING_HPP
#define GCT_TONE_MAPPING_HPP

#include <memory>
#include <vector>
#include <cstdint>
#include <gct/hysteresis.hpp>
#include <gct/tone_mapping_create_info.hpp>

namespace gct {

class buffer_t;
class compute;
class tone_mapping {
  struct tone_state_t {
    std::uint32_t max;
    float scale;
  };
public:
  tone_mapping(
    const tone_mapping_create_info &ci
  );
  void set(
    command_buffer_recorder_t &rec,
    unsigned int image_index
  ) const;
  void get(
    command_buffer_recorder_t &rec,
    unsigned int image_index
  ) const;
  const std::vector< std::shared_ptr< buffer_t > > &get_buffer() const {
    return tone;
  }
  const tone_mapping_create_info &get_props() const {
    return props;
  };
private:
  tone_mapping_create_info props;
  std::vector< std::shared_ptr< buffer_t > > tone;
  std::vector< std::shared_ptr< buffer_t > > tone_staging;
  std::shared_ptr< compute > calc_tone;
  mutable hysteresis< float > tone_scale;
};

}

#endif

