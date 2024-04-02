#include "gct/exception.hpp"
#include <nlohmann/json.hpp>
#include <gct/shader_module.hpp>
#include <gct/pipeline_multisample_state_create_info.hpp>
#include <vulkan2json/PipelineMultisampleStateCreateInfo.hpp>
#ifdef VK_NV_FRAMEBUFFER_MIXED_SAMPLES_EXTENSION_NAME
#include <vulkan2json/PipelineCoverageModulationStateCreateInfoNV.hpp>
#endif
#ifdef VK_NV_COVERAGE_REDUCTION_MODE_EXTENSION_NAME
#include <vulkan2json/PipelineCoverageReductionStateCreateInfoNV.hpp>
#endif
#ifdef VK_NV_FRAGMENT_COVERAGE_TO_COLOR_EXTENSION_NAME
#include <vulkan2json/PipelineCoverageToColorStateCreateInfoNV.hpp>
#endif
#ifdef VK_EXT_SAMPLE_LOCATIONS_EXTENSION_NAME
#include <vulkan2json/PipelineSampleLocationsStateCreateInfoEXT.hpp>
#endif

namespace gct {

  void to_json( nlohmann::json &root, const pipeline_multisample_state_create_info_t &v ) {
    root = nlohmann::json::object();
    root[ "basic" ] = v.get_basic();
    LIBGCT_ARRAY_OF_TO_JSON( basic, pSampleMask, sample_mask )
#ifdef VK_NV_FRAMEBUFFER_MIXED_SAMPLES_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( coverage_modulation )
    LIBGCT_ARRAY_OF_TO_JSON( coverage_modulation, pCoverageModulationTable, coverage_modulation_table )
#endif
#ifdef VK_NV_COVERAGE_REDUCTION_MODE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( coverage_reduction )
#endif
#ifdef VK_NV_FRAGMENT_COVERAGE_TO_COLOR_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( coverage_to_color )
#endif
#ifdef VK_EXT_SAMPLE_LOCATIONS_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( sample_locations )
#endif
  }
  void from_json( const nlohmann::json &root, pipeline_multisample_state_create_info_t &v ) {
    if( !root.is_object() ) throw incompatible_json( "The JSON is incompatible to pipeline_multisample_state_create_info_t", __FILE__, __LINE__ );
    LIBGCT_EXTENSION_FROM_JSON( basic )
    LIBGCT_ARRAY_OF_FROM_JSON( basic, pSampleMask, sample_mask )
#ifdef VK_NV_FRAMEBUFFER_MIXED_SAMPLES_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( coverage_modulation )
    LIBGCT_ARRAY_OF_FROM_JSON( coverage_modulation, pCoverageModulationTable, coverage_modulation_table )
#endif
#ifdef VK_NV_COVERAGE_REDUCTION_MODE_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( coverage_reduction )
#endif
#ifdef VK_NV_FRAGMENT_COVERAGE_TO_COLOR_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( coverage_to_color )
#endif
#ifdef VK_EXT_SAMPLE_LOCATIONS_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( sample_locations )
#endif
  }
  pipeline_multisample_state_create_info_t &pipeline_multisample_state_create_info_t::rebuild_chain() {
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
    if( !sample_mask.empty() ) {
      if( basic.rasterizationSamples == vk::SampleCountFlagBits::e1 ) {
        if( sample_mask.size() != 1u ) {
          throw gct::exception::invalid_argument( "pipeline_multisample_state_create_info_t : rasterizationSamples != sample_mask.size()", __FILE__, __LINE__ );
        }
      }
      else if( basic.rasterizationSamples == vk::SampleCountFlagBits::e2 ) {
        if( sample_mask.size() != 2u ) {
          throw gct::exception::invalid_argument( "pipeline_multisample_state_create_info_t : rasterizationSamples != sample_mask.size()", __FILE__, __LINE__ );
        }
      }
      else if( basic.rasterizationSamples == vk::SampleCountFlagBits::e4 ) {
        if( sample_mask.size() != 4u ) {
          throw gct::exception::invalid_argument( "pipeline_multisample_state_create_info_t : rasterizationSamples != sample_mask.size()", __FILE__, __LINE__ );
        }
      }
      else if( basic.rasterizationSamples == vk::SampleCountFlagBits::e8 ) {
        if( sample_mask.size() != 8u ) {
          throw gct::exception::invalid_argument( "pipeline_multisample_state_create_info_t : rasterizationSamples != sample_mask.size()", __FILE__, __LINE__ );
        }
      }
      else if( basic.rasterizationSamples == vk::SampleCountFlagBits::e16 ) {
        if( sample_mask.size() != 16u ) {
          throw gct::exception::invalid_argument( "pipeline_multisample_state_create_info_t : rasterizationSamples != sample_mask.size()", __FILE__, __LINE__ );
        }
      }
      else if( basic.rasterizationSamples == vk::SampleCountFlagBits::e32 ) {
        if( sample_mask.size() != 32u ) {
          throw gct::exception::invalid_argument( "pipeline_multisample_state_create_info_t : rasterizationSamples != sample_mask.size()", __FILE__, __LINE__ );
        }
      }
      else if( basic.rasterizationSamples == vk::SampleCountFlagBits::e64 ) {
        if( sample_mask.size() != 64u ) {
          throw gct::exception::invalid_argument( "pipeline_multisample_state_create_info_t : rasterizationSamples != sample_mask.size()", __FILE__, __LINE__ );
        }
      }
    }
    LIBGCT_ARRAY_OF_REBUILD_CHAIN_PARTIAL( basic, PSampleMask, sample_mask )
#ifdef VK_NV_FRAMEBUFFER_MIXED_SAMPLES_EXTENSION_NAME
    LIBGCT_ARRAY_OF_REBUILD_CHAIN( coverage_modulation, CoverageModulationTableCount, PCoverageModulationTable, coverage_modulation_table )
    LIBGCT_EXTENSION_REBUILD_CHAIN( coverage_modulation )
#endif
#ifdef VK_NV_COVERAGE_REDUCTION_MODE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( coverage_reduction )
#endif
#ifdef VK_NV_FRAGMENT_COVERAGE_TO_COLOR_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( coverage_to_color )
#endif
#ifdef VK_EXT_SAMPLE_LOCATIONS_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( sample_locations )
#endif
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
  pipeline_multisample_state_create_info_t::pipeline_multisample_state_create_info_t() {
    set_basic(
      vk::PipelineMultisampleStateCreateInfo()
    );
  }
}

