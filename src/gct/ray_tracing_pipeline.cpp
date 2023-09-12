#include <nlohmann/json.hpp>
#include <gct/device.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/ray_tracing_pipeline.hpp>
#include <gct/deferred_operation.hpp>
#include <gct/device.hpp>
#include <gct/get_device.hpp>
#include <gct/async.hpp>
#include <vulkan2json/ShaderStageFlags.hpp>
#include <vulkan2json/PipelineCreationFeedbackEXT.hpp>

namespace gct {
  ray_tracing_pipeline_t::ray_tracing_pipeline_t(
    const std::shared_ptr< pipeline_cache_t > &cache,
    const ray_tracing_pipeline_create_info_t &create_info
  ) :
    pipeline_t( cache ),
    props( create_info ) {
    auto deferred_operation = get_device( *this ).get_deferred_operation();
#ifdef VK_EXT_PIPELINE_CREATION_FEEDBACK_EXTENSION_NAME
    const auto &exts = get_device( *this ).get_activated_extensions();
    std::vector< vk::PipelineCreationFeedbackEXT > feedback_;
    const bool use_feedback = exts.find( VK_EXT_PIPELINE_CREATION_FEEDBACK_EXTENSION_NAME ) != exts.end();
    if( use_feedback ) {
      feedback_.resize( props.get_basic().stageCount + 1u );
      props.set_creation_feedback(
        vk::PipelineCreationFeedbackCreateInfoEXT()
          .setPPipelineCreationFeedback( &feedback_.back() )
          .setPipelineStageCreationFeedbackCount( props.get_basic().stageCount )
          .setPPipelineStageCreationFeedbacks( feedback_.data() )
      );
    }
#endif
    props.rebuild_chain();

    auto wrapped = (*cache->get_factory())->createRayTracingPipelinesKHRUnique( **deferred_operation, **cache, { props.get_basic() } );
    if( wrapped.result != vk::Result::eSuccess && wrapped.result != vk::Result::eOperationDeferredKHR ) {
#if VK_HEADER_VERSION >= 256
      vk::detail::throwResultException( wrapped.result, "createRayTracingPipeline failed" );
#else
      vk::throwResultException( wrapped.result, "createRayTracingPipeline failed" );
#endif
    }
    auto future = async( deferred_operation );
    if( future.get() != vk::Result::eSuccess ) {
#if VK_HEADER_VERSION >= 256
      vk::detail::throwResultException( wrapped.result, "createRayTracingPipeline failed" );
#else
      vk::throwResultException( wrapped.result, "createRayTracingPipeline failed" );
#endif
    }
    handle = std::move( wrapped.value[ 0 ] );
#ifdef VK_EXT_PIPELINE_CREATION_FEEDBACK_EXTENSION_NAME
    if( use_feedback ) {
      for( std::uint32_t i = 0u; i != props.get_basic().stageCount; ++i ) {
        feedback.insert(
          std::make_pair(
            props.get_basic().pStages[ i ].stage,
            feedback_[ i ]
          )
        );
      }
      feedback.insert(
        std::make_pair(
          vk::ShaderStageFlagBits::eAll,
          feedback_[ props.get_basic().stageCount ]
        )
      );
    }
#endif
  }
  void to_json( nlohmann::json &root, const ray_tracing_pipeline_t &v ) {
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
