#ifndef GCT_GRAPHICS_PIPELINE_CREATE_INFO_HPP
#define GCT_GRAPHICS_PIPELINE_CREATE_INFO_HPP
#include <memory>
#include <vector>
#include <tuple>
#include <optional>
#include <vulkan/vulkan.hpp>
#include <glm/vec3.hpp>
#include <gct/extension.hpp>
#include <gct/pipeline_shader_stage_create_info.hpp>
#include <gct/pipeline_vertex_input_state_create_info.hpp>
#include <gct/pipeline_input_assembly_state_create_info.hpp>
#include <gct/pipeline_tessellation_state_create_info.hpp>
#include <gct/pipeline_viewport_state_create_info.hpp>
#include <gct/pipeline_rasterization_state_create_info.hpp>
#include <gct/pipeline_multisample_state_create_info.hpp>
#include <gct/pipeline_depth_stencil_state_create_info.hpp>
#include <gct/pipeline_color_blend_state_create_info.hpp>
#include <gct/pipeline_dynamic_state_create_info.hpp>
#include <gct/array_of.hpp>
#include <vulkan/vulkan_structs.hpp>

namespace gct {
  class pipeline_layout_t;
  class render_pass_t;
  class descriptor_set_layout_t;
  class gbuffer;
  class shader_module_reflection_t;
  class graphics_pipeline_create_info_t : public chained_t {
    friend void to_json( nlohmann::json &root, const graphics_pipeline_create_info_t &v );
  public:
    using self_type = graphics_pipeline_create_info_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
    LIBGCT_BASIC_SETTER( vk::GraphicsPipelineCreateInfo )
    LIBGCT_ARRAY_OF_WRAPPED( pipeline_shader_stage_create_info_t, vk::PipelineShaderStageCreateInfo, stage )
#ifdef VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::AttachmentSampleCountInfoAMD , attachment_sample_count )
#endif
#ifdef VK_ANDROID_EXTERNAL_MEMORY_ANDROID_HARDWARE_BUFFER_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::ExternalFormatANDROID , external_format )
#endif
#ifdef VK_EXT_GRAPHICS_PIPELINE_LIBRARY_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::GraphicsPipelineLibraryCreateInfoEXT , graphics_pipeline_library )
#endif
#ifdef VK_NV_DEVICE_GENERATED_COMMANDS_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::GraphicsPipelineShaderGroupsCreateInfoNV , shader_group )
#endif
#if defined(VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME) && defined(VK_NVX_MULTIVIEW_PER_VIEW_ATTRIBUTES_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::MultiviewPerViewAttributesInfoNVX , multiview_per_view_attributes )
#endif
#ifdef VK_AMD_PIPELINE_COMPILER_CONTROL_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PipelineCompilerControlCreateInfoAMD , compiler_control )
#endif
#ifdef VK_KHR_MAINTENANCE_5_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PipelineCreateFlags2CreateInfoKHR, create_flag2 )
#endif
#ifdef VK_VERSION_1_3
    LIBGCT_EXTENSION_SETTER( vk::PipelineCreationFeedbackCreateInfo, creation_feedback )
    LIBGCT_ARRAY_OF( vk::PipelineCreationFeedback, stage_creation_feedback )
#elif defined(VK_EXT_PIPELINE_CREATION_FEEDBACK_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::PipelineCreationFeedbackCreateInfoEXT, creation_feedback )
    LIBGCT_ARRAY_OF( vk::PipelineCreationFeedbackEXT, stage_creation_feedback )
#endif
#ifdef VK_EXT_DISCARD_RECTANGLES_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PipelineDiscardRectangleStateCreateInfoEXT , discard_rectangle )
#endif
#ifdef VK_NV_FRAGMENT_SHADING_RATE_ENUMS_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PipelineFragmentShadingRateEnumStateCreateInfoNV , fragment_shading_rate_nv )
#endif
#ifdef VK_KHR_FRAGMENT_SHADING_RATE_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PipelineFragmentShadingRateStateCreateInfoKHR, fragment_shading_rate )
#endif
#ifdef VK_KHR_PIPELINE_LIBRARY_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PipelineLibraryCreateInfoKHR , library )
#endif
#ifdef VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PipelineRenderingCreateInfoKHR, rendering )
#endif
#ifdef VK_NV_REPRESENTATIVE_FRAGMENT_TEST_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PipelineRepresentativeFragmentTestStateCreateInfoNV, representative_fragment_test )
#endif
#ifdef VK_EXT_PIPELINE_ROBUSTNESS_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PipelineRobustnessCreateInfoEXT , robustness )
#endif
#ifdef VK_KHR_DYNAMIC_RENDERING_LOCAL_READ_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::RenderingAttachmentLocationInfoKHR , rendering_attachment_location )
    LIBGCT_EXTENSION_SETTER( vk::RenderingInputAttachmentIndexInfoKHR , rendering_input_attachment_index )
#endif
  private:
    deep_copy_unique_ptr< pipeline_vertex_input_state_create_info_t > vertex_input;
    deep_copy_unique_ptr< pipeline_input_assembly_state_create_info_t > input_assembly;
    deep_copy_unique_ptr< pipeline_tessellation_state_create_info_t > tessellation;
    deep_copy_unique_ptr< pipeline_viewport_state_create_info_t > viewport;
    deep_copy_unique_ptr< pipeline_rasterization_state_create_info_t > rasterization;
    deep_copy_unique_ptr< pipeline_multisample_state_create_info_t > multisample;
    deep_copy_unique_ptr< pipeline_depth_stencil_state_create_info_t > depth_stencil;
    deep_copy_unique_ptr< pipeline_color_blend_state_create_info_t > color_blend;
    deep_copy_unique_ptr< pipeline_dynamic_state_create_info_t > dynamic;
    std::shared_ptr< pipeline_layout_t > layout;
    std::shared_ptr< render_pass_t > render_pass;
    glm::ivec3 dim = glm::ivec3( 0, 0, 0 );
    std::uint32_t subpass = 0u;
  public:
    graphics_pipeline_create_info_t &add_stage( const std::shared_ptr< shader_module_t >& );
    graphics_pipeline_create_info_t &add_stage( const std::vector< std::shared_ptr< shader_module_t > >& );
    graphics_pipeline_create_info_t &set_vertex_input( const pipeline_vertex_input_state_create_info_t& );
    graphics_pipeline_create_info_t &set_vertex_input();
    graphics_pipeline_create_info_t &clear_vertex_input();
    graphics_pipeline_create_info_t &set_input_assembly( const pipeline_input_assembly_state_create_info_t& );
    graphics_pipeline_create_info_t &set_input_assembly();
    graphics_pipeline_create_info_t &clear_input_assembly();
    graphics_pipeline_create_info_t &set_tessellation( const pipeline_tessellation_state_create_info_t& );
    graphics_pipeline_create_info_t &set_tessellation();
    graphics_pipeline_create_info_t &clear_tessellation();
    graphics_pipeline_create_info_t &set_viewport( const pipeline_viewport_state_create_info_t& );
    graphics_pipeline_create_info_t &set_viewport();
    graphics_pipeline_create_info_t &clear_viewport();
    [[nodiscard]] const pipeline_viewport_state_create_info_t &get_viewport() const;
    graphics_pipeline_create_info_t &set_rasterization( const pipeline_rasterization_state_create_info_t& );
    graphics_pipeline_create_info_t &set_rasterization();
    graphics_pipeline_create_info_t &clear_rasterization();
    graphics_pipeline_create_info_t &set_multisample( const pipeline_multisample_state_create_info_t& );
    graphics_pipeline_create_info_t &set_multisample();
    graphics_pipeline_create_info_t &clear_multisample();
    graphics_pipeline_create_info_t &set_depth_stencil( const pipeline_depth_stencil_state_create_info_t& );
    graphics_pipeline_create_info_t &set_depth_stencil();
    graphics_pipeline_create_info_t &clear_depth_stencil();
    graphics_pipeline_create_info_t &enable_depth_test();
    graphics_pipeline_create_info_t &disable_depth_test();
    graphics_pipeline_create_info_t &disable_depth_write();
    graphics_pipeline_create_info_t &set_color_blend( const pipeline_color_blend_state_create_info_t& );
    graphics_pipeline_create_info_t &set_color_blend( common_color_blend_mode mode );
    [[nodiscard]] const pipeline_color_blend_state_create_info_t &get_color_blend() const;
    graphics_pipeline_create_info_t &clear_color_blend();
    graphics_pipeline_create_info_t &set_dynamic( const pipeline_dynamic_state_create_info_t& );
    graphics_pipeline_create_info_t &set_dynamic();
    graphics_pipeline_create_info_t &clear_dynamic();
    [[nodiscard]] const pipeline_dynamic_state_create_info_t &get_dynamic() const;
    graphics_pipeline_create_info_t &set_layout( const std::shared_ptr< pipeline_layout_t >& );
    graphics_pipeline_create_info_t &set_layout( const std::shared_ptr< descriptor_set_layout_t >& );
    graphics_pipeline_create_info_t &clear_layout();
    [[nodiscard]] const std::shared_ptr< pipeline_layout_t > &get_layout() const {
      return layout;
    }
    graphics_pipeline_create_info_t &set_dim( const glm::vec3 &v ) {
      dim = v;
      return *this;
    }
    graphics_pipeline_create_info_t &set_dim(
      std::int32_t x,
      std::int32_t y,
      std::int32_t z
    ) {
      dim = glm::ivec3( x, y, z );
      return *this;
    }
    [[nodiscard]] const glm::ivec3 &get_dim() const {
      return dim;
    }
    graphics_pipeline_create_info_t &set_render_pass( const std::shared_ptr< render_pass_t >&, std::uint32_t );
    graphics_pipeline_create_info_t &clear_render_pass();
    graphics_pipeline_create_info_t &fill_untouched();
    graphics_pipeline_create_info_t &set_gbuffer( const gbuffer& );
    graphics_pipeline_create_info_t &use_dynamic_rendering(
      const std::vector< vk::Format > &color_attachment_format,
      vk::Format depth_attachment_format,
      vk::Format stencil_attachment_format
    );
    graphics_pipeline_create_info_t &use_dynamic_rendering(
      vk::Format color_attachment_format,
      vk::Format depth_attachment_format,
      vk::Format stencil_attachment_format
    );
    graphics_pipeline_create_info_t &use_color_blend( common_color_blend_mode );
    graphics_pipeline_create_info_t &set_line_width( float w );
    [[nodiscard]] bool has_reflection( vk::ShaderStageFlagBits ) const;
    [[nodiscard]] const shader_module_reflection_t &get_reflection( vk::ShaderStageFlagBits ) const;
    void to_json( nlohmann::json &root );
  private:
    std::optional< std::tuple< vk::Format, vk::Format, vk::Format > > dynamic_rendering_format;
    std::vector< vk::Format > color_attachment_format_list;
  };
  void to_json( nlohmann::json &root, const graphics_pipeline_create_info_t &v );
}

#endif

