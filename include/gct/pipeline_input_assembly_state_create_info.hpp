#ifndef GCT_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO_HPP
#define GCT_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO_HPP
#include <memory>
#include <optional>
#include <vulkan/vulkan.hpp>
#include <gct/extension.hpp>

namespace gct {
  class pipeline_input_assembly_state_create_info_t {
  public:
    using self_type = pipeline_input_assembly_state_create_info_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
    LIBGCT_BASIC_SETTER( vk::PipelineInputAssemblyStateCreateInfo )
  };
}

#endif

