#ifndef GCT_SIMPLIFY_BUFFER_COPY_HPP
#define GCT_SIMPLIFY_BUFFER_COPY_HPP

#include <vector>
#include <vulkan/vulkan.hpp>

namespace gct {

bool simplify_buffer_copy(
  std::vector< vk::BufferCopy > &copy
);

}

#endif

