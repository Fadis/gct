#ifndef GCT_VOXEL_BUFFER_HPP
#define GCT_VOXEL_BUFFER_HPP
#include <memory>
#include <vector>
#include <vulkan/vulkan.hpp>
#include <glm/glm.hpp>

namespace gct {
class allocator_t;
class buffer_t;
class voxel_buffer {
public:
  voxel_buffer(
    const std::shared_ptr< allocator_t > &allocator,
    unsigned int size,
    unsigned int data_size
  );
  const std::shared_ptr< gct::buffer_t > &get_buffer() const {
    return buf;
  }
private:
  std::shared_ptr< gct::buffer_t > buf;
};

}

#endif

