#ifndef LIBGCT_INCLUDE_GCT_PHYSICAL_DEVICE_PROPERTIES_HPP
#define LIBGCT_INCLUDE_GCT_PHYSICAL_DEVICE_PROPERTIES_HPP
#include <iostream>
#include <optional>
#include <nlohmann/json.hpp>
#include <vulkan/vulkan.hpp>
#include <gct/get_extensions.hpp>
#include <gct/queue_family_properties.hpp>
#include <gct/physical_device_memory_properties.hpp>

namespace gct {
  class instance_t;
#ifdef VK_KHR_DISPLAY_EXTENSION_NAME
  struct display_plane_t : public chained_t {
    vk::DisplayPlaneCapabilitiesKHR basic;
  };
  struct display_mode_t {
    vk::DisplayModePropertiesKHR basic;
    std::unordered_map< std::uint32_t, display_plane_t > capabilities;
  };
  struct display_properties_t {
    vk::DisplayPropertiesKHR basic;
    std::vector< display_mode_t > modes;
    std::vector< std::uint32_t > planes;
  };
#endif

  class physical_device_properties_t : public chained_t {
public:
    physical_device_properties_t(
      instance_t &instance,
      vk::PhysicalDevice &pdev,
      const std::vector< const char* > &layers
    );
    physical_device_properties_t(
      instance_t &instance,
      vk::PhysicalDevice &pdev,
      const std::vector< const char* > &layers,
      const std::vector< const char* > &exts
    );
    void load(
      instance_t &instance,
      vk::PhysicalDevice &pdev,
      const std::vector< const char* > &layers
    );
    std::uint32_t get_api_version() const { return api_version; }
private:
    std::uint32_t api_version;
    extension_map_t available_extensions;
    extension_map_t activated_extensions;
    std::vector< const char* > activated_extension_names;
    layer_map_t available_layers;
    std::vector< const char* > activated_layer_names;
    layer_map_t activated_layers;
    std::vector< queue_family_properties_t > queue_family;
    deep_copy_unique_ptr< physical_device_memory_properties_t > memory_props;
public:
    const extension_map_t &get_available_extensions() const {
      return available_extensions;
    }
    const extension_map_t &get_activated_extensions() const {
      return activated_extensions;
    }
    const std::vector< const char* > &get_activated_extension_names() const {
      return activated_extension_names;
    }
    const layer_map_t &get_available_layers() const {
      return available_layers;
    }
    const layer_map_t &get_activated_layers() const {
      return activated_layers;
    }
    const std::vector< const char* > &get_activated_layer_names() const {
      return activated_layer_names;
    }
    const std::vector< queue_family_properties_t > &get_queue_family() const {
      return queue_family;
    }
    bool has_memory_props() const {
      return memory_props.get();
    }
    const physical_device_memory_properties_t &get_memory_props() const {
      return *memory_props;
    }

    using self_type = physical_device_properties_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
#if defined(VK_VERSION_1_1) || defined(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME)
private:
    using basic_t = vk::PhysicalDeviceProperties;
#ifdef VK_VERSION_1_1
    using basic2_t = vk::PhysicalDeviceProperties2;
#else
    using basic2_t = vk::PhysicalDeviceProperties2KHR;
#endif
    basic2_t basic; 
public:
    LIBGCT_BASIC_SETTER_INDIRECT( properties )
#else
    LIBGCT_BASIC_SETTER( vk::PhysicalDeviceProperties )
#endif
#ifdef VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceAccelerationStructurePropertiesKHR , acceleration_structure )
#endif
#ifdef VK_EXT_BLEND_OPERATION_ADVANCED_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceBlendOperationAdvancedPropertiesEXT , blend_operation )
#endif
#ifdef VK_EXT_CONSERVATIVE_RASTERIZATION_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceConservativeRasterizationPropertiesEXT , conservative_rasterization )
#endif
#ifdef VK_NV_COOPERATIVE_MATRIX_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceCooperativeMatrixPropertiesNV , cooperative_matrix )
#endif
#ifdef VK_EXT_CUSTOM_BORDER_COLOR_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceCustomBorderColorPropertiesEXT , custom_border_color )
#endif
#ifdef VK_VERSION_1_2
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceDepthStencilResolveProperties , depth_stencil_resolve )
#elif defined(VK_KHR_DEPTH_STENCIL_RESOLVE_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceDepthStencilResolvePropertiesKHR , depth_stencil_resolve )
#endif
#ifdef VK_VERSION_1_2
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceDescriptorIndexingProperties , device_descriptor_indexing )
#elif defined(VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceDescriptorIndexingPropertiesEXT , device_descriptor_indexing )
#endif
#ifdef VK_NV_DEVICE_GENERATED_COMMANDS_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceDeviceGeneratedCommandsPropertiesNV , device_generated_commands )
#endif
#ifdef VK_EXT_DISCARD_RECTANGLES_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceDiscardRectanglePropertiesEXT , discard_rectangle )
#endif
#ifdef VK_VERSION_1_2
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceDriverProperties , driver )
#elif defined(VK_KHR_DRIVER_PROPERTIES_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceDriverPropertiesKHR , driver )
#endif
#ifdef VK_EXT_PHYSICAL_DEVICE_DRM_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceDrmPropertiesEXT , drm )
#endif
#ifdef VK_EXT_EXTERNAL_MEMORY_HOST_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceExternalMemoryHostPropertiesEXT , external_memory_host )
#endif
#ifdef VK_VERSION_1_2
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceFloatControlsProperties , float_controls )
#elif defined(VK_KHR_SHADER_FLOAT_CONTROLS_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceFloatControlsPropertiesKHR , float_controls )
#endif
#ifdef VK_EXT_FRAGMENT_DENSITY_MAP_2_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceFragmentDensityMap2PropertiesEXT , fragment_density_map2 )
#endif
#ifdef VK_EXT_FRAGMENT_DENSITY_MAP_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceFragmentDensityMapPropertiesEXT , fragment_density_map )
#endif
#ifdef VK_NV_FRAGMENT_SHADING_RATE_ENUMS_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceFragmentShadingRateEnumsPropertiesNV , fragment_shading_rate_enums )
#endif
#ifdef VK_KHR_FRAGMENT_SHADING_RATE_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceFragmentShadingRatePropertiesKHR , fragment_shading_rate )
#endif
#ifdef VK_VERSION_1_1
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceIDProperties , id )
#elif defined(VK_KHR_external_memory_capabilities) || defined(VK_KHR_EXTERNAL_SEMAPHORE_CAPABILITIES_EXTENSION_NAME) || defined(VK_KHR_EXTERNAL_FENCE_CAPABILITIES_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceIDPropertiesKHR , id )
#endif
#ifdef VK_EXT_INLINE_UNIFORM_BLOCK_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceInlineUniformBlockPropertiesEXT , inline_uniform_block )
#endif
#ifdef VK_EXT_LINE_RASTERIZATION_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceLineRasterizationPropertiesEXT , line_rasterization )
#endif
#ifdef VK_VERSION_1_1
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceMaintenance3Properties , maintenance3 )
#elif defined(VK_KHR_MAINTENANCE3_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceMaintenance3PropertiesKHR , maintenance3 )
#endif
#ifdef VK_NV_MESH_SHADER_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceMeshShaderPropertiesNV , mesh_shader )
#endif
#ifdef VK_EXT_MULTI_DRAW_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceMultiDrawPropertiesEXT , multi_draw )
#endif
#ifdef VK_NVX_MULTIVIEW_PER_VIEW_ATTRIBUTES_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceMultiviewPerViewAttributesPropertiesNVX , multiview_per_view_attributes )
#endif
#ifdef VK_VERSION_1_1
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceMultiviewProperties , multiview )
#elif defined(VK_KHR_MULTIVIEW_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceMultiviewPropertiesKHR , multiview )
#endif
#ifdef VK_EXT_PCI_BUS_INFO_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDevicePCIBusInfoPropertiesEXT , pci_bus_info )
#endif
#ifdef VK_KHR_PERFORMANCE_QUERY_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDevicePerformanceQueryPropertiesKHR , performance_query )
#endif
#ifdef VK_VERSION_1_1
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDevicePointClippingProperties , point_clipping )
#elif defined(VK_KHR_MAINTENANCE2_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDevicePointClippingPropertiesKHR , point_clipping )
#endif
#ifdef VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDevicePortabilitySubsetPropertiesKHR , portability_subset )
#endif
#ifdef VK_VERSION_1_1
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceProtectedMemoryProperties , protected_memory )
#endif
#ifdef VK_EXT_PROVOKING_VERTEX_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceProvokingVertexPropertiesEXT , provoking_vertex )
#endif
#ifdef VK_KHR_PUSH_DESCRIPTOR_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDevicePushDescriptorPropertiesKHR , push_descriptor )
#endif
#ifdef VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceRayTracingPipelinePropertiesKHR , ray_tracing_pipeline )
#endif
#ifdef VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceRayTracingPropertiesNV , ray_tracing )
#endif
#ifdef VK_EXT_ROBUSTNESS_2_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceRobustness2PropertiesEXT , robustness2 )
#endif
#ifdef VK_EXT_SAMPLE_LOCATIONS_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceSampleLocationsPropertiesEXT , sample_locations )
#endif
#ifdef VK_VERSION_1_2
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceSamplerFilterMinmaxProperties , sampler_filter_minmax )
#elif defined(VK_EXT_SAMPLER_FILTER_MINMAX_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceSamplerFilterMinmaxPropertiesEXT , sampler_filter_minmax )
#endif
#ifdef VK_AMD_SHADER_CORE_PROPERTIES_2_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceShaderCoreProperties2AMD , shader_core2 )
#endif
#ifdef VK_AMD_SHADER_CORE_PROPERTIES_2_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceShaderCorePropertiesAMD , shader_core )
#endif
#ifdef VK_KHR_SHADER_INTEGER_DOT_PRODUCT_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceShaderIntegerDotProductPropertiesKHR , shader_integer_dot_product )
#endif
#ifdef VK_NV_SHADER_SM_BUILTINS_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceShaderSMBuiltinsPropertiesNV , shader_sm_builtins )
#endif
#ifdef VK_NV_SHADING_RATE_IMAGE_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceShadingRateImagePropertiesNV , shading_rate_image )
#endif
#ifdef VK_VERSION_1_1
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceSubgroupProperties , subgroup )
#endif
#ifdef VK_EXT_SUBGROUP_SIZE_CONTROL_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceSubgroupSizeControlPropertiesEXT , subgroup_size_control )
#endif
#ifdef VK_HUAWEI_SUBPASS_SHADING_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceSubpassShadingPropertiesHUAWEI , subpass_shading )
#endif
#ifdef VK_EXT_TEXEL_BUFFER_ALIGNMENT_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceTexelBufferAlignmentPropertiesEXT , texel_buffer_alignment )
#endif
#ifdef VK_VERSION_1_2
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceTimelineSemaphoreProperties , timeline_semaphore )
#elif defined(VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceTimelineSemaphorePropertiesKHR , timeline_semaphore )
#endif
#ifdef VK_EXT_TRANSFORM_FEEDBACK_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceTransformFeedbackPropertiesEXT , transform_feedback )
#endif
#ifdef VK_EXT_VERTEX_ATTRIBUTE_DIVISOR_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceVertexAttributeDivisorPropertiesEXT , vertex_attribute_divisor )
#endif
#ifdef VK_KHR_DISPLAY_EXTENSION_NAME
private:
    std::vector< display_properties_t > display;
public:
    const std::vector< display_properties_t > &get_display() const {
      return display;
    }
#endif
  };
  void to_json( nlohmann::json&, const physical_device_properties_t& );
}

#endif

