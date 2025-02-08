#ifndef GCT_PARTICLE_BUFFER_HPP
#define GCT_PARTICLE_BUFFER_HPP
#include <memory>
#include <vector>
#include <cstdint>
#include <gct/particle_buffer_create_info.hpp>
#include <gct/aabb.hpp>

namespace gct {

class buffer_t;
class particle_buffer {
public:
  particle_buffer(
    const particle_buffer_create_info &ci
  );
  [[nodiscard]] std::shared_ptr< buffer_t > get_buffer() const {
    return vertex_buffer;
  }
  [[nodiscard]] std::uint32_t get_count() const {
    return particle_count;
  }
  [[nodiscard]] const aabb &get_aabb() const {
    return aabb_;
  }
  void init(
    command_buffer_recorder_t&
  );
private:
  particle_buffer_create_info props;
  std::vector< std::uint8_t > host_vertex_buffer;
  std::shared_ptr< buffer_t > vertex_buffer;
  std::uint32_t particle_count = 0u;
  aabb aabb_;
};

}

#endif

