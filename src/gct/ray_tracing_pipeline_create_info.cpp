#include <nlohmann/json.hpp>
#include <gct/ray_tracing_pipeline_create_info.hpp>
#include <gct/device.hpp>
#include <gct/pipeline_layout.hpp>
#include <gct/shader_module.hpp>
#include <gct/pipeline_shader_stage_create_info.hpp>
#include <gct/ray_tracing_shader_group_create_info.hpp>
#include <gct/pipeline_library_create_info.hpp>
#include <gct/ray_tracing_pipeline_interface_create_info.hpp>
#include <gct/pipeline_dynamic_state_create_info.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/descriptor_set_layout.hpp>
#include <gct/get_device.hpp>
#ifdef VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME
#include <vulkan2json/RayTracingPipelineCreateInfoKHR.hpp>
#ifdef VK_EXT_PIPELINE_CREATION_FEEDBACK_EXTENSION_NAME
#include <vulkan2json/PipelineCreationFeedbackCreateInfoEXT.hpp>
#endif
#include <gct/extension.hpp>
namespace gct {
  void to_json( nlohmann::json &root, const ray_tracing_pipeline_create_info_t &v ) {
     root = nlohmann::json::object();
     root[ "basic" ] = v.get_basic();
#ifdef VK_EXT_PIPELINE_CREATION_FEEDBACK_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( creation_feedback )
#endif
  }
  void from_json( const nlohmann::json &root, ray_tracing_pipeline_create_info_t &v ) {
    if( !root.is_object() ) throw incompatible_json( "The JSON is incompatible to ray_tracing_pipeline_create_info_t", __FILE__, __LINE__ );
    LIBGCT_EXTENSION_FROM_JSON( basic )
#ifdef VK_EXT_PIPELINE_CREATION_FEEDBACK_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( creation_feedback )
#endif
  }
  ray_tracing_pipeline_create_info_t &ray_tracing_pipeline_create_info_t::rebuild_chain() {
    if( chained ) return *this;
    for( auto &s: stage ) s.rebuild_chain();
    raw_stage.clear();
    raw_stage.reserve( stage.size() );
    std::transform(
      stage.begin(),
      stage.end(),
      std::back_inserter( raw_stage ),
      []( const auto &v ) {
        return v.get_basic();
      }
    );
    if( !raw_stage.empty() )
      basic
        .setStageCount( raw_stage.size() )
        .setPStages( raw_stage.data() );
    else
      basic
        .setStageCount( 0u )
        .setPStages( nullptr );

    for( auto &g: group ) g.rebuild_chain();
    raw_group.clear();
    raw_group.reserve( group.size() );
    std::transform(
      group.begin(),
      group.end(),
      std::back_inserter( raw_group ),
      []( const auto &v ) {
        return v.get_basic();
      }
    );
    if( !raw_group.empty() )
      basic
        .setGroupCount( raw_group.size() )
        .setPGroups( raw_group.data() );
    else
      basic
        .setGroupCount( 0u )
        .setPGroups( nullptr );

    if( library ) {
      library->rebuild_chain();
      basic
        .setPLibraryInfo( &library->get_basic() );
    }
    else
      basic.setPLibraryInfo( nullptr );

    if( library_interface ) {
      library_interface->rebuild_chain();
      basic
        .setPLibraryInterface( &library_interface->get_basic() );
    }
    else
      basic.setPLibraryInterface( nullptr );

    if( dynamic ) {
      dynamic->rebuild_chain();
      basic
        .setPDynamicState( &dynamic->get_basic() );
    }
    else
      basic.setPDynamicState( nullptr );
    
    if( layout )
      basic.setLayout( **layout );
    else
      basic.setLayout( nullptr );


    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
#ifdef VK_EXT_PIPELINE_CREATION_FEEDBACK_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( creation_feedback )
#endif
  }
  ray_tracing_pipeline_create_info_t &ray_tracing_pipeline_create_info_t::add_stage( const pipeline_shader_stage_create_info_t &v, vk::RayTracingShaderGroupTypeKHR type ) {
    stage.push_back( v );
    stage.back().rebuild_chain();
    if( stage.back().get_basic().stage == vk::ShaderStageFlagBits::eClosestHitKHR ) {
      group.push_back(
        ray_tracing_shader_group_create_info_t()
          .set_basic(
            vk::RayTracingShaderGroupCreateInfoKHR()
              .setType( type )
              .setGeneralShader( VK_SHADER_UNUSED_KHR )
              .setClosestHitShader( stage.size() - 1u )
              .setAnyHitShader( VK_SHADER_UNUSED_KHR )
              .setIntersectionShader( VK_SHADER_UNUSED_KHR )
          )
      );
    }
    else if( stage.back().get_basic().stage == vk::ShaderStageFlagBits::eAnyHitKHR ) {
      group.push_back(
        ray_tracing_shader_group_create_info_t()
          .set_basic(
            vk::RayTracingShaderGroupCreateInfoKHR()
              .setType( type )
              .setGeneralShader( VK_SHADER_UNUSED_KHR )
              .setClosestHitShader( VK_SHADER_UNUSED_KHR )
              .setAnyHitShader( stage.size() - 1u )
              .setIntersectionShader( VK_SHADER_UNUSED_KHR )
          )
      );
    }
    else if( stage.back().get_basic().stage == vk::ShaderStageFlagBits::eIntersectionKHR ) {
      group.push_back(
        ray_tracing_shader_group_create_info_t()
          .set_basic(
            vk::RayTracingShaderGroupCreateInfoKHR()
              .setType( type )
              .setGeneralShader( VK_SHADER_UNUSED_KHR )
              .setClosestHitShader( VK_SHADER_UNUSED_KHR )
              .setAnyHitShader( VK_SHADER_UNUSED_KHR )
              .setIntersectionShader( stage.size() - 1u )
          )
      );
    }
    else {
      group.push_back(
        ray_tracing_shader_group_create_info_t()
          .set_basic(
            vk::RayTracingShaderGroupCreateInfoKHR()
              .setType( type )
              .setGeneralShader( stage.size() - 1u )
              .setClosestHitShader( VK_SHADER_UNUSED_KHR )
              .setAnyHitShader( VK_SHADER_UNUSED_KHR )
              .setIntersectionShader( VK_SHADER_UNUSED_KHR )
          )
      );
    }
    chained = false;
    return *this;
  }
  ray_tracing_pipeline_create_info_t &ray_tracing_pipeline_create_info_t::add_stage( const pipeline_shader_stage_create_info_t &v ) {
    stage.push_back( v );
    stage.back().rebuild_chain();
    if( stage.back().get_basic().stage == vk::ShaderStageFlagBits::eClosestHitKHR ) {
      group.push_back(
        ray_tracing_shader_group_create_info_t()
          .set_basic(
            vk::RayTracingShaderGroupCreateInfoKHR()
              .setType( vk::RayTracingShaderGroupTypeKHR::eTrianglesHitGroup )
              .setGeneralShader( VK_SHADER_UNUSED_KHR )
              .setClosestHitShader( stage.size() - 1u )
              .setAnyHitShader( VK_SHADER_UNUSED_KHR )
              .setIntersectionShader( VK_SHADER_UNUSED_KHR )
          )
      );
    }
    else if( stage.back().get_basic().stage == vk::ShaderStageFlagBits::eAnyHitKHR ) {
      group.push_back(
        ray_tracing_shader_group_create_info_t()
          .set_basic(
            vk::RayTracingShaderGroupCreateInfoKHR()
              .setType( vk::RayTracingShaderGroupTypeKHR::eTrianglesHitGroup )
              .setGeneralShader( VK_SHADER_UNUSED_KHR )
              .setClosestHitShader( VK_SHADER_UNUSED_KHR )
              .setAnyHitShader( stage.size() - 1u )
              .setIntersectionShader( VK_SHADER_UNUSED_KHR )
          )
      );
    }
    else if( stage.back().get_basic().stage == vk::ShaderStageFlagBits::eIntersectionKHR ) {
      group.push_back(
        ray_tracing_shader_group_create_info_t()
          .set_basic(
            vk::RayTracingShaderGroupCreateInfoKHR()
              .setType( vk::RayTracingShaderGroupTypeKHR::eProceduralHitGroup )
              .setGeneralShader( VK_SHADER_UNUSED_KHR )
              .setClosestHitShader( VK_SHADER_UNUSED_KHR )
              .setAnyHitShader( VK_SHADER_UNUSED_KHR )
              .setIntersectionShader( stage.size() - 1u )
          )
      );
    }
    else {
      group.push_back(
        ray_tracing_shader_group_create_info_t()
          .set_basic(
            vk::RayTracingShaderGroupCreateInfoKHR()
              .setType( vk::RayTracingShaderGroupTypeKHR::eGeneral )
              .setGeneralShader( stage.size() - 1u )
              .setClosestHitShader( VK_SHADER_UNUSED_KHR )
              .setAnyHitShader( VK_SHADER_UNUSED_KHR )
              .setIntersectionShader( VK_SHADER_UNUSED_KHR )
          )
      );
    }
    chained = false;
    return *this;
  }
  ray_tracing_pipeline_create_info_t &ray_tracing_pipeline_create_info_t::add_stage( const std::shared_ptr< shader_module_t > &v, vk::RayTracingShaderGroupTypeKHR type ) {
    if( !v->get_props().has_reflection() )
      throw exception::invalid_argument( "Reflection is required to set shader directly to pipeline", __FILE__, __LINE__ );
    add_stage(
      pipeline_shader_stage_create_info_t()
        .set_shader_module( v ),
      type
    );

    return *this;
  }
  ray_tracing_pipeline_create_info_t &ray_tracing_pipeline_create_info_t::add_stage( const std::shared_ptr< shader_module_t > &v ) {
    if( !v->get_props().has_reflection() )
      throw exception::invalid_argument( "Reflection is required to set shader directly to pipeline", __FILE__, __LINE__ );
    add_stage(
      pipeline_shader_stage_create_info_t()
        .set_shader_module( v )
    );

    return *this;
  }
  ray_tracing_pipeline_create_info_t &ray_tracing_pipeline_create_info_t::clear_stage() {
    group.clear();
    stage.clear();
    chained = false;
    return *this;
  }
  ray_tracing_pipeline_create_info_t &ray_tracing_pipeline_create_info_t::set_library( const pipeline_library_create_info_t &v ) {
    library.reset( new pipeline_library_create_info_t( v ) );
    chained = false;
    return *this;
  }
  ray_tracing_pipeline_create_info_t &ray_tracing_pipeline_create_info_t::clear_library() {
    library.reset();
    chained = false;
    return *this;
  }
  ray_tracing_pipeline_create_info_t &ray_tracing_pipeline_create_info_t::set_library_interface( const ray_tracing_pipeline_interface_create_info_t &v ) {
    library_interface.reset( new ray_tracing_pipeline_interface_create_info_t( v ) );
    chained = false;
    return *this;
  }
  ray_tracing_pipeline_create_info_t &ray_tracing_pipeline_create_info_t::clear_library_interface() {
    library_interface.reset();
    chained = false;
    return *this;
  }
  ray_tracing_pipeline_create_info_t &ray_tracing_pipeline_create_info_t::set_dynamic( const pipeline_dynamic_state_create_info_t &v ) {
    dynamic.reset( new pipeline_dynamic_state_create_info_t( v ) );
    chained = false;
    return *this;
  }
  ray_tracing_pipeline_create_info_t &ray_tracing_pipeline_create_info_t::clear_dynamic() {
    dynamic.reset();
    chained = false;
    return *this;
  }
  ray_tracing_pipeline_create_info_t &ray_tracing_pipeline_create_info_t::set_layout( const std::shared_ptr< pipeline_layout_t > &v ) {
    layout = v;
    chained = false;
    return *this;
  }
  ray_tracing_pipeline_create_info_t &ray_tracing_pipeline_create_info_t::set_layout( const std::shared_ptr< descriptor_set_layout_t > &v ) {
    return set_layout(
      get_device( *v ).get_pipeline_layout(
        gct::pipeline_layout_create_info_t()
          .add_descriptor_set_layout(
            v
          )
      )
    );
  }
  ray_tracing_pipeline_create_info_t &ray_tracing_pipeline_create_info_t::clear_layout() {
    layout.reset();
    chained = false;
    return *this;
  }
}
#endif

