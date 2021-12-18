#include <gct/device.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/compute_pipeline.hpp>
#include <gct/device.hpp>
#include <gct/get_device.hpp>
#include <gct/pipeline_creation_feedback.hpp>
#include <vulkan2json/ShaderStageFlags.hpp>
#include <vulkan2json/PipelineCreationFeedbackEXT.hpp>
namespace gct {
  compute_pipeline_t::compute_pipeline_t(
    const std::shared_ptr< pipeline_cache_t > &cache,
    const compute_pipeline_create_info_t &create_info
  ) :
    pipeline_t( cache ),
    props( create_info ) {
#ifdef VK_EXT_PIPELINE_CREATION_FEEDBACK_EXTENSION_NAME
    const auto &exts = get_device( *this ).get_activated_extensions();
    std::vector< vk::PipelineCreationFeedbackEXT > feedback_;
    const bool use_feedback = exts.find( VK_EXT_PIPELINE_CREATION_FEEDBACK_EXTENSION_NAME ) != exts.end();
    if( use_feedback ) {
      feedback_.resize( 2 );
      props.set_creation_feedback(
        vk::PipelineCreationFeedbackCreateInfoEXT()
          .setPPipelineCreationFeedback( std::next( feedback_.data() ) )
          .setPipelineStageCreationFeedbackCount( 1u )
          .setPPipelineStageCreationFeedbacks( feedback_.data() )
      );
    }
#endif
    props.rebuild_chain();

    auto wrapped = (*cache->get_factory())->createComputePipelinesUnique( **cache, { props.get_basic() } );
    if( wrapped.result != vk::Result::eSuccess )
      vk::throwResultException( wrapped.result, "createComputePipeline failed" );
    handle = std::move( wrapped.value[ 0 ] );
#ifdef VK_EXT_PIPELINE_CREATION_FEEDBACK_EXTENSION_NAME
    if( use_feedback ) {
      feedback.insert(
        std::make_pair(
          vk::ShaderStageFlagBits::eCompute,
          feedback_[ 0 ]
        )
      );
      feedback.insert(
        std::make_pair(
          vk::ShaderStageFlagBits::eAll,
          feedback_[ 1 ]
        )
      );
    }
#endif
  }
  void to_json( nlohmann::json &root, const compute_pipeline_t &v ) {
    root = nlohmann::json::object();
    root[ "props" ] = v.get_props();
#ifdef VK_EXT_PIPELINE_CREATION_FEEDBACK_EXTENSION_NAME
    root[ "feedback" ] = nlohmann::json::object();
    for( const auto &[key,value]: v.get_feedback() ) {
      std::string label = nlohmann::json( key );
      root[ "feedback" ][ label ] = value;
    }
#endif
  }
}

