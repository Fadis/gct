#ifndef GCT_FORMAT_TO_ASPECT_HPP
#define GCT_FORMAT_TO_ASPECT_HPP
#include <vulkan/vulkan.hpp>

namespace gct {
  vk::Flags<vk::ImageAspectFlagBits> format_to_aspect( vk::Format format );
}

#endif

