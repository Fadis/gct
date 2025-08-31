#ifndef LIBGCT_INCLUDE_GCT_PHYSICAL_DEVICE_PROPERTIES_HPP
#define LIBGCT_INCLUDE_GCT_PHYSICAL_DEVICE_PROPERTIES_HPP
#include <unordered_map>
#include <nlohmann/json_fwd.hpp>
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
  struct display_mode_properties_t {
    vk::DisplayModePropertiesKHR basic;
    std::unordered_map< std::uint32_t, display_plane_t > capabilities;
  };
  struct display_properties_t {
    vk::DisplayPropertiesKHR basic;
    std::vector< display_mode_properties_t > modes;
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
    [[nodiscard]] std::uint32_t get_api_version() const { return api_version; }
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
    [[nodiscard]] const extension_map_t &get_available_extensions() const {
      return available_extensions;
    }
    [[nodiscard]] const extension_map_t &get_activated_extensions() const {
      return activated_extensions;
    }
    [[nodiscard]] const std::vector< const char* > &get_activated_extension_names() const {
      return activated_extension_names;
    }
    [[nodiscard]] const layer_map_t &get_available_layers() const {
      return available_layers;
    }
    [[nodiscard]] const layer_map_t &get_activated_layers() const {
      return activated_layers;
    }
    [[nodiscard]] const std::vector< const char* > &get_activated_layer_names() const {
      return activated_layer_names;
    }
    [[nodiscard]] const std::vector< queue_family_properties_t > &get_queue_family() const {
      return queue_family;
    }
    bool has_memory_props() const {
      return memory_props.get();
    }
    [[nodiscard]] const physical_device_memory_properties_t &get_memory_props() const {
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
#ifdef VK_NV_CLUSTER_ACCELERATION_STRUCTURE_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceClusterAccelerationStructurePropertiesNV, cluster_acceleration_structure )
#endif
#ifdef VK_HUAWEI_CLUSTER_CULLING_SHADER_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceClusterCullingShaderPropertiesHUAWEI , cluster_culling_shader )
#endif
#ifdef VK_KHR_COMPUTE_SHADER_DERIVATIVES_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceComputeShaderDerivativesPropertiesKHR , compute_shader_derivative )
#endif
#ifdef VK_EXT_CONSERVATIVE_RASTERIZATION_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceConservativeRasterizationPropertiesEXT , conservative_rasterization )
#endif
#ifdef VK_NV_COOPERATIVE_MATRIX_2_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceCooperativeMatrix2PropertiesNV, cooperative_matrix2 )
#endif
#ifdef VK_KHR_COOPERATIVE_MATRIX_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceCooperativeMatrixPropertiesKHR , cooperative_matrix )
#endif
#ifdef VK_NV_COOPERATIVE_MATRIX_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceCooperativeMatrixPropertiesNV , cooperative_matrix_nv )
#endif
#ifdef VK_NV_COOPERATIVE_VECTOR_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceCooperativeVectorPropertiesNV, cooperative_vector )
#endif
#ifdef VK_NV_COPY_MEMORY_INDIRECT_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceCopyMemoryIndirectPropertiesNV , copy_memory_indirect )
#endif
#ifdef VK_NV_CUDA_KERNEL_LAUNCH_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceCudaKernelLaunchPropertiesNV , cuda_kernel_launch )
#endif
#ifdef VK_EXT_CUSTOM_BORDER_COLOR_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceCustomBorderColorPropertiesEXT , custom_border_color )
#endif
#ifdef VK_VERSION_1_2
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceDepthStencilResolveProperties , depth_stencil_resolve )
#elif defined(VK_KHR_DEPTH_STENCIL_RESOLVE_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceDepthStencilResolvePropertiesKHR , depth_stencil_resolve )
#endif
#ifdef VK_EXT_DESCRIPTOR_BUFFER_EXTENSION_NAME
   LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceDescriptorBufferDensityMapPropertiesEXT , descriptor_buffer_density_map )
   LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceDescriptorBufferPropertiesEXT , descriptor_buffer )
#endif
#if defined(VK_EXT_DESCRIPTOR_BUFFER_EXTENSION_NAME) && defined(VK_ARM_TENSORS_EXTENSION_NAME)
   LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceDescriptorBufferTensorPropertiesARM, descriptor_buffer_tensor )
#endif
#ifdef VK_VERSION_1_2
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceDescriptorIndexingProperties , descriptor_indexing )
#elif defined(VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceDescriptorIndexingPropertiesEXT , descriptor_indexing )
#endif
#ifdef VK_EXT_DEVICE_GENERATED_COMMANDS_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceDeviceGeneratedCommandsPropertiesEXT , device_generated_commands )
#endif
#ifdef VK_NV_DEVICE_GENERATED_COMMANDS_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceDeviceGeneratedCommandsPropertiesNV , device_generated_commands_nv )
#endif
#ifdef VK_EXT_DISCARD_RECTANGLES_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceDiscardRectanglePropertiesEXT , discard_rectangle )
#endif
#ifdef VK_NV_DISPLACEMENT_MICROMAP_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceDisplacementMicromapPropertiesNV , displacement_micromap )
#endif
#ifdef VK_VERSION_1_2
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceDriverProperties , driver )
#elif defined(VK_KHR_DRIVER_PROPERTIES_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceDriverPropertiesKHR , driver )
#endif
#ifdef VK_EXT_PHYSICAL_DEVICE_DRM_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceDrmPropertiesEXT , drm )
#endif
#ifdef VK_EXT_EXTENDED_DYNAMIC_STATE_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceExtendedDynamicState3PropertiesEXT , extended_dynamic_state3 )
#endif
#ifdef VK_NV_EXTENDED_SPARSE_ADDRESS_SPACE_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceExtendedSparseAddressSpacePropertiesNV , extended_sparse_address_space )
#endif
#ifdef VK_NV_EXTERNAL_COMPUTE_QUEUE_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceExternalComputeQueuePropertiesNV, external_compute_queue )
#endif
#ifdef VK_ANDROID_EXTERNAL_FORMAT_RESOLVE_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceExternalFormatResolvePropertiesANDROID , external_format_resolve )
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
#ifdef VK_VALVE_FRAGMENT_DENSITY_MAP_LAYERED_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceFragmentDensityMapLayeredPropertiesVALVE , fragment_density_map_layered )
#endif
#ifdef VK_EXT_FRAGMENT_DENSITY_MAP_OFFSET_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceFragmentDensityMapOffsetPropertiesEXT , fragment_density_map_offset )
#endif
#ifdef VK_EXT_FRAGMENT_DENSITY_MAP_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceFragmentDensityMapPropertiesEXT , fragment_density_map )
#endif
#ifdef VK_KHR_FRAGMENT_SHADER_BARYCENTRIC_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceFragmentShaderBarycentricPropertiesKHR , fragment_shader_barycentric )
#endif
#ifdef VK_NV_FRAGMENT_SHADING_RATE_ENUMS_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceFragmentShadingRateEnumsPropertiesNV , fragment_shading_rate_enums )
#endif
#ifdef VK_KHR_FRAGMENT_SHADING_RATE_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceFragmentShadingRatePropertiesKHR , fragment_shading_rate )
#endif ///////////
#ifdef VK_EXT_GRAPHICS_PIPELINE_LIBRARY_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceGraphicsPipelineLibraryPropertiesEXT , graphics_pipeline_library )
#endif
#ifdef VK_VERSION_1_4
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceHostImageCopyProperties , host_image_copy )
#elif defined(VK_EXT_HOST_IMAGE_COPY_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceHostImageCopyPropertiesEXT , host_image_copy )
#endif
#ifdef VK_VERSION_1_1
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceIDProperties , id )
#elif defined(VK_KHR_external_memory_capabilities) || defined(VK_KHR_EXTERNAL_SEMAPHORE_CAPABILITIES_EXTENSION_NAME) || defined(VK_KHR_EXTERNAL_FENCE_CAPABILITIES_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceIDPropertiesKHR , id )
#endif
#ifdef VK_MESA_IMAGE_ALIGNMENT_CONTROL_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceImageAlignmentControlPropertiesMESA , image_alignment_control )
#endif
#ifdef VK_QCOM_IMAGE_PROCESSING_2_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceImageProcessing2PropertiesQCOM , image_processing2 )
#endif
#ifdef VK_QCOM_IMAGE_PROCESSING_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceImageProcessingPropertiesQCOM , image_processing )
#endif
#ifdef VK_VERSION_1_3
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceInlineUniformBlockProperties , inline_uniform_block )
#elif defined(VK_EXT_INLINE_UNIFORM_BLOCK_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceInlineUniformBlockPropertiesEXT , inline_uniform_block )
#endif
#ifdef VK_KHR_MAINTENANCE_7_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceLayeredApiPropertiesListKHR , layered_api )
#endif
#ifdef VK_MSFT_LAYERED_DRIVER_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceLayeredDriverPropertiesMSFT , layered_driver )
#endif
#ifdef VK_EXT_LEGACY_VERTEX_ATTRIBUTES_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceLegacyVertexAttributesPropertiesEXT , legacy_vertex_attribute )
#endif
#ifdef VK_VERSION_1_4
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceLineRasterizationProperties , line_rasterization )
#elif defined(VK_KHR_LINE_RASTERIZATION_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceLineRasterizationPropertiesKHR , line_rasterization )
#elif defined(VK_EXT_LINE_RASTERIZATION_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceLineRasterizationPropertiesEXT , line_rasterization )
#endif
#ifdef VK_VERSION_1_1
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceMaintenance3Properties , maintenance3 )
#elif defined(VK_KHR_MAINTENANCE_3_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceMaintenance3PropertiesKHR , maintenance3 )
#endif
#ifdef VK_VERSION_1_3
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceMaintenance4Properties , maintenance4 )
#elif defined(VK_KHR_MAINTENANCE_4_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceMaintenance4PropertiesKHR , maintenance4 )
#endif
#ifdef VK_VERSION_1_4
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceMaintenance5Properties , maintenance5 )
#elif defined(VK_KHR_MAINTENANCE_5_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceMaintenance5PropertiesKHR , maintenance5 )
#endif
#ifdef VK_VERSION_1_4
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceMaintenance6Properties , maintenance6 )
#elif defined(VK_KHR_MAINTENANCE_6_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceMaintenance6PropertiesKHR , maintenance6 )
#endif
#ifdef VK_KHR_MAINTENANCE_7_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceMaintenance7PropertiesKHR , maintenance7 )
#endif
#ifdef VK_KHR_MAINTENANCE_9_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceMaintenance9PropertiesKHR , maintenance9 )
#endif
#ifdef VK_EXT_MAP_MEMORY_PLACED_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceMapMemoryPlacedPropertiesEXT , map_memory_placed )
#endif
#ifdef VK_NV_MEMORY_DECOMPRESSION_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceMemoryDecompressionPropertiesNV , memory_decompression )
#endif
#ifdef VK_NV_MESH_SHADER_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceMeshShaderPropertiesNV , mesh_shader_nv )
#endif
#ifdef VK_EXT_MESH_SHADER_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceMeshShaderPropertiesEXT , mesh_shader )
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
#ifdef VK_EXT_NESTED_COMMAND_BUFFER_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceNestedCommandBufferPropertiesEXT , nested_command_buffer )
#endif
#ifdef VK_EXT_OPACITY_MICROMAP_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceOpacityMicromapPropertiesEXT , opacity_micromap )
#endif
#ifdef VK_EXT_PCI_BUS_INFO_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDevicePCIBusInfoPropertiesEXT , pci_bus_info )
#endif
#ifdef VK_NV_PARTITIONED_ACCELERATION_STRUCTURE_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDevicePartitionedAccelerationStructurePropertiesNV , partitioned_acceleration_structure )
#endif
#ifdef VK_KHR_PERFORMANCE_QUERY_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDevicePerformanceQueryPropertiesKHR , performance_query )
#endif
#ifdef VK_KHR_PIPELINE_BINARY_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDevicePipelineBinaryPropertiesKHR , pipeline_binary )
#endif
#ifdef VK_VERSION_1_4
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDevicePipelineRobustnessProperties , pipeline_robustness )
#elif defined(VK_EXT_PIPELINE_ROBUSTNESS_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDevicePipelineRobustnessPropertiesEXT , pipeline_robustness )
#endif
#ifdef VK_VERSION_1_1
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDevicePointClippingProperties , point_clipping )
#elif defined(VK_KHR_MAINTENANCE_2_EXTENSION_NAME)
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
#ifdef VK_VERSION_1_4
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDevicePushDescriptorProperties , push_descriptor )
#elif defined(VK_KHR_PUSH_DESCRIPTOR_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDevicePushDescriptorPropertiesKHR , push_descriptor )
#endif
#ifdef VK_NV_RAY_TRACING_INVOCATION_REORDER_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceRayTracingInvocationReorderPropertiesNV , ray_tracing_invocation_reorder )
#endif
#ifdef VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceRayTracingPipelinePropertiesKHR , ray_tracing_pipeline )
#endif
#ifdef VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceRayTracingPropertiesNV , ray_tracing )
#endif
#ifdef VK_ARM_RENDER_PASS_STRIPED_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceRenderPassStripedPropertiesARM, render_pass_striped )
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
#ifdef VK_ARM_SCHEDULING_CONTROLS_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceSchedulingControlsPropertiesARM , scheduling_control )
#endif
#ifdef VK_ARM_SHADER_CORE_BUILTINS_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceShaderCoreBuiltinsPropertiesARM , shader_core_builtin )
#endif
#ifdef VK_AMD_SHADER_CORE_PROPERTIES_2_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceShaderCoreProperties2AMD , shader_core2 )
#endif
#ifdef VK_AMD_SHADER_CORE_PROPERTIES_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceShaderCorePropertiesAMD , shader_core )
#endif
#ifdef VK_ARM_SHADER_CORE_PROPERTIES_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceShaderCorePropertiesARM , shader_core_arm )
#endif
#ifdef VK_VERSION_1_3
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceShaderIntegerDotProductProperties , shader_integer_dot_product )
#elif defined(VK_KHR_SHADER_INTEGER_DOT_PRODUCT_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceShaderIntegerDotProductPropertiesKHR , shader_integer_dot_product )
#endif
#ifdef VK_EXT_SHADER_MODULE_IDENTIFIER_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceShaderModuleIdentifierPropertiesEXT , shader_module_identifier )
#endif
#ifdef VK_EXT_SHADER_OBJECT_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceShaderObjectPropertiesEXT , shader_object )
#endif
#ifdef VK_NV_SHADER_SM_BUILTINS_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceShaderSMBuiltinsPropertiesNV , shader_sm_builtins )
#endif
#ifdef VK_EXT_SHADER_TILE_IMAGE_EXTENSION_NAME 
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceShaderTileImagePropertiesEXT , shader_tile_image )
#endif
#ifdef VK_NV_SHADING_RATE_IMAGE_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceShadingRateImagePropertiesNV , shading_rate_image )
#endif
#ifdef VK_VERSION_1_1
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceSubgroupProperties , subgroup )
#endif
#ifdef VK_VERSION_1_3
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceSubgroupSizeControlProperties , subgroup_size_control )
#elif defined(VK_EXT_SUBGROUP_SIZE_CONTROL_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceSubgroupSizeControlPropertiesEXT , subgroup_size_control )
#endif
#ifdef VK_HUAWEI_SUBPASS_SHADING_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceSubpassShadingPropertiesHUAWEI , subpass_shading )
#endif
#ifdef VK_VERSION_1_3
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceTexelBufferAlignmentProperties , texel_buffer_alignment )
#elif defined(VK_EXT_TEXEL_BUFFER_ALIGNMENT_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceTexelBufferAlignmentPropertiesEXT , texel_buffer_alignment )
#endif
#ifdef VK_QCOM_TILE_MEMORY_HEAP_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceTileMemoryHeapPropertiesQCOM , tile_memory_heap )
#endif
#ifdef VK_QCOM_TILE_SHADING_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceTileShadingPropertiesQCOM , tile_shading )
#endif
#ifdef VK_VERSION_1_2
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceTimelineSemaphoreProperties , timeline_semaphore )
#elif defined(VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceTimelineSemaphorePropertiesKHR , timeline_semaphore )
#endif
#ifdef VK_EXT_TRANSFORM_FEEDBACK_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceTransformFeedbackPropertiesEXT , transform_feedback )
#endif
#ifdef VK_VERSION_1_4
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceVertexAttributeDivisorProperties , vertex_attribute_divisor )
#elif defined(VK_EXT_VERTEX_ATTRIBUTE_DIVISOR_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceVertexAttributeDivisorPropertiesKHR , vertex_attribute_divisor )
#elif defined(VK_EXT_VERTEX_ATTRIBUTE_DIVISOR_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceVertexAttributeDivisorPropertiesEXT , vertex_attribute_divisor )
#endif
#ifdef VK_KHR_DISPLAY_EXTENSION_NAME
private:
    std::vector< display_properties_t > display;
public:
    [[nodiscard]] const std::vector< display_properties_t > &get_display() const {
      return display;
    }
#endif
  };
  void to_json( nlohmann::json&, const physical_device_properties_t& );
}

#endif

