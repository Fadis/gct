#ifndef GCT_TONE_MAPPING_HPP
#define GCT_TONE_MAPPING_HPP

#include <memory>
#include <vector>
#include <cstdint>
#include <gct/hysteresis.hpp>
#include <gct/tone_mapping_create_info.hpp>
#include <gct/property.hpp>

namespace gct {

class buffer_t;
class compute;
class tone_mapping :
  public property< tone_mapping_create_info > {
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
  [[nodiscard]] const std::vector< std::shared_ptr< buffer_t > > &get_buffer() const {
    return tone;
  }
private:
  std::vector< std::shared_ptr< buffer_t > > tone;
  std::vector< std::shared_ptr< buffer_t > > tone_staging;
  std::shared_ptr< compute > calc_tone;
  mutable hysteresis< float > tone_scale;
};

}

#endif

