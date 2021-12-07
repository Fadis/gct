#ifndef GCT_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO_HPP
#define GCT_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO_HPP
#include <memory>
#include <optional>
#include <nlohmann/json_fwd.hpp>
#include <vulkan/vulkan.hpp>
#include <gct/extension.hpp>

namespace gct {
  class pipeline_multisample_state_create_info_t {
  public:
    using self_type = pipeline_multisample_state_create_info_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
    LIBGCT_BASIC_SETTER( vk::PipelineMultisampleStateCreateInfo )
#ifdef VK_NV_FRAMEBUFFER_MIXED_SAMPLES_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PipelineCoverageModulationStateCreateInfoNV, coverage_modulation )
#endif
#ifdef VK_NV_COVERAGE_REDUCTION_MODE_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PipelineCoverageReductionStateCreateInfoNV, coverage_reduction )
#endif
#ifdef VK_NV_FRAGMENT_COVERAGE_TO_COLOR_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PipelineCoverageToColorStateCreateInfoNV, coverage_to_color )
#endif
#ifdef VK_EXT_SAMPLE_LOCATIONS_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PipelineSampleLocationsStateCreateInfoEXT, sample_locations )
#endif
  };
  void to_json( nlohmann::json &root, const pipeline_multisample_state_create_info_t &v );
  void from_json( const nlohmann::json &root, pipeline_multisample_state_create_info_t &v );
}

#endif

