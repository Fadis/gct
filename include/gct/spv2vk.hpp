#ifndef GCT_SPV2VK_HPP
#define GCT_SPV2VK_HPP
#include <vulkan/vulkan.hpp>
#include <gct/spirv_reflect.h>

namespace gct {
  vk::ShaderStageFlagBits spv2vk( SpvReflectShaderStageFlagBits v );
  vk::DescriptorType spv2vk( SpvReflectDescriptorType v );
}

#endif

