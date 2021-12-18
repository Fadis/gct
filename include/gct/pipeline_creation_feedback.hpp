#ifndef GCT_PIPELINE_CREATION_FEEDBACK_HPP
#define GCT_PIPELINE_CREATION_FEEDBACK_HPP
#include <vulkan/vulkan.hpp>
#ifdef VK_EXT_PIPELINE_CREATION_FEEDBACK_EXTENSION_NAME
#include <unordered_map>
namespace gct {
  using pipeline_creation_feedback_t = std::unordered_map< vk::ShaderStageFlagBits, vk::PipelineCreationFeedbackEXT >;
}
#endif
#endif

