#ifndef LIBGCT_INCLUDE_GCT_PHYSICAL_DEVICE_FEATURES_HPP
#define LIBGCT_INCLUDE_GCT_PHYSICAL_DEVICE_FEATURES_HPP
#include <nlohmann/json_fwd.hpp>
#include <vulkan/vulkan.hpp>
#include <gct/extension.hpp>
#include <vulkan/vulkan_structs.hpp>

namespace gct {
  nlohmann::json to_json( const vk::PhysicalDeviceFeatures &v );
  struct instance_t;
  struct physical_device_properties_t;
  class physical_device_features_t : public chained_t {
  public:
    physical_device_features_t(
      instance_t &instance,
      vk::PhysicalDevice &pdev,
      const physical_device_properties_t &props
    );
    void check_subset( const physical_device_features_t& ) const;
    using self_type = physical_device_features_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
#if defined(VK_VERSION_1_1) || defined(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME)
private:
    using basic_t = vk::PhysicalDeviceFeatures;
#ifdef VK_VERSION_1_1
    using basic2_t = vk::PhysicalDeviceFeatures2;
#else
    using basic2_t = vk::PhysicalDeviceFeatures2KHR;
#endif
    basic2_t basic; 
public: 
    LIBGCT_BASIC_SETTER_INDIRECT( features )
#else
    LIBGCT_BASIC_SETTER( vk::PhysicalDeviceFeatures )
#endif
#ifdef VK_VERSION_1_1
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDevice16BitStorageFeatures , storage_16bit )
#elif defined(VK_KHR_16BIT_STORAGE_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDevice16BitStorageFeaturesKHR , storage_16bit )
#endif
#ifdef VK_EXT_4444_FORMATS_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDevice4444FormatsFeaturesEXT , formats_4444 ) 
#endif
#ifdef VK_VERSION_1_2
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDevice8BitStorageFeatures , storage_8bit )
#elif defined(VK_KHR_8BIT_STORAGE_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDevice8BitStorageFeaturesKHR , storage_8bit )
#endif
#ifdef VK_EXT_ASTC_DECODE_MODE_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceASTCDecodeFeaturesEXT , astc_decode )
#endif
#ifdef VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceAccelerationStructureFeaturesKHR , acceleration_strucutre )
#endif
#ifdef VK_EXT_DEVICE_ADDRESS_BINDING_REPORT_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceAddressBindingReportFeaturesEXT , address_binding_report )
#endif
#ifdef VK_SEC_AMIGO_PROFILING_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceAmigoProfilingFeaturesSEC , amigo_profiling )
#endif
#ifdef VK_EXT_ATTACHMENT_FEEDBACK_LOOP_DYNAMIC_STATE_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceAttachmentFeedbackLoopDynamicStateFeaturesEXT , attachment_feedback_loop_dynamic_state )
#endif
#ifdef VK_EXT_ATTACHMENT_FEEDBACK_LOOP_LAYOUT_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceAttachmentFeedbackLoopLayoutFeaturesEXT , attachment_feedback_loop_layout )
#endif
#ifdef VK_EXT_BLEND_OPERATION_ADVANCED_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceBlendOperationAdvancedFeaturesEXT , blend_operation_advanced )
#endif
#ifdef VK_EXT_BORDER_COLOR_SWIZZLE_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceBorderColorSwizzleFeaturesEXT , border_color_swizzle )
#endif
#ifdef VK_VERSION_1_2
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceBufferDeviceAddressFeatures , buffer_device_address )
#elif defined(VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceBufferDeviceAddressFeaturesKHR , buffer_device_address )
#endif
#ifdef VK_EXT_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceBufferDeviceAddressFeaturesEXT , buffer_device_address_ext )
#endif
#ifdef VK_HUAWEI_CLUSTER_CULLING_SHADER_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceClusterCullingShaderFeaturesHUAWEI , cluster_culling_shader )
#endif
#ifdef VK_AMD_DEVICE_COHERENT_MEMORY_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceCoherentMemoryFeaturesAMD , coherent_memory )
#endif
#ifdef VK_EXT_COLOR_WRITE_ENABLE_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceColorWriteEnableFeaturesEXT , color_write_enable )
#endif
#ifdef VK_NV_COMPUTE_SHADER_DERIVATIVES_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceComputeShaderDerivativesFeaturesNV , compute_shader_derivatives )
#endif
#ifdef VK_EXT_CONDITIONAL_RENDERING_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceConditionalRenderingFeaturesEXT , conditional_rendering )
#endif
#ifdef VK_KHR_COOPERATIVE_MATRIX_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceCooperativeMatrixFeaturesKHR , cooperative_matrix )
#endif
#ifdef VK_NV_COOPERATIVE_MATRIX_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceCooperativeMatrixFeaturesNV , cooperative_matrix_nv )
#endif
#ifdef VK_NV_COPY_MEMORY_INDIRECT_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceCopyMemoryIndirectFeaturesNV , copy_memory_indirect )
#endif
#ifdef VK_NV_CORNER_SAMPLED_IMAGE_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceCornerSampledImageFeaturesNV , corner_sampled_image )
#endif
#ifdef VK_NV_COVERAGE_REDUCTION_MODE_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceCoverageReductionModeFeaturesNV , coverage_reduction_mode )
#endif
#ifdef VK_QCOM_FILTER_CUBIC_CLAMP_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceCubicClampFeaturesQCOM , cubic_clamp )
#endif
#ifdef VK_QCOM_FILTER_CUBIC_WEIGHTS_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceCubicWeightsFeaturesQCOM , cubic_weight )
#endif
#ifdef VK_NV_CUDA_KERNEL_LAUNCH_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceCudaKernelLaunchFeaturesNV , cuda_kernel_launch )
#endif
#ifdef VK_EXT_CUSTOM_BORDER_COLOR_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceCustomBorderColorFeaturesEXT , custom_border_color )
#endif
#ifdef VK_NV_DEDICATED_ALLOCATION_IMAGE_ALIASING_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceDedicatedAllocationImageAliasingFeaturesNV , dedicated_allocation_image_aliasing )
#endif
#ifdef VK_EXT_DEPTH_BIAS_CONTROL_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceDepthBiasControlFeaturesEXT , depth_bias_control )
#endif
#ifdef VK_EXT_DEPTH_CLAMP_ZERO_ONE_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceDepthClampZeroOneFeaturesEXT , depth_clamp_zero_one )
#endif
#ifdef VK_EXT_DEPTH_CLIP_CONTROL_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceDepthClipControlFeaturesEXT , depth_clip_control )
#endif
#ifdef VK_EXT_DEPTH_CLIP_ENABLE_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceDepthClipEnableFeaturesEXT , depth_clip_enable )
#endif
#ifdef VK_EXT_DESCRIPTOR_BUFFER_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceDescriptorBufferFeaturesEXT , descriptor_buffer )
#endif
#ifdef VK_VERSION_1_2
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceDescriptorIndexingFeatures , descriptor_indexing )
#elif defined(VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceDescriptorIndexingFeaturesEXT , descriptor_indexing )
#endif
#ifdef VK_NV_DESCRIPTOR_POOL_OVERALLOCATION_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceDescriptorPoolOverallocationFeaturesNV , descriptor_pool_overallocation )
#endif
#ifdef VK_NV_DEVICE_GENERATED_COMMANDS_COMPUTE_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceDeviceGeneratedCommandsComputeFeaturesNV , device_generated_commands_compute )
#endif
#ifdef VK_NV_DEVICE_GENERATED_COMMANDS_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceDeviceGeneratedCommandsFeaturesNV , device_generated_commands )
#endif
#ifdef VK_EXT_DEVICE_MEMORY_REPORT_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceDeviceMemoryReportFeaturesEXT , device_memory_report )
#endif
#ifdef VK_NV_DEVICE_DIAGNOSTICS_CONFIG_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceDiagnosticsConfigFeaturesNV , diagnostic_config )
#endif
#ifdef VK_NV_DISPLACEMENT_MICROMAP_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceDisplacementMicromapFeaturesNV, displacement_micromap )
#endif
#ifdef VK_VERSION_1_3
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceDynamicRenderingFeatures , dynamic_rendering )
#elif defined(VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceDynamicRenderingFeaturesKHR , dynamic_rendering )
#endif
#ifdef VK_KHR_DYNAMIC_RENDERING_LOCAL_READ_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceDynamicRenderingLocalReadFeaturesKHR , dynamic_rendering_local_read )
#endif
#ifdef VK_EXT_DYNAMIC_RENDERING_UNUSED_ATTACHMENTS_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceDynamicRenderingUnusedAttachmentsFeaturesEXT , dynamic_rendering_unused_attachments )
#endif
#ifdef VK_NV_SCISSOR_EXCLUSIVE_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceExclusiveScissorFeaturesNV , exclusive_scissor )
#endif
#ifdef VK_EXT_EXTENDED_DYNAMIC_STATE_2_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceExtendedDynamicState2FeaturesEXT , extended_dynamic_state2 )
#endif
#ifdef VK_EXT_EXTENDED_DYNAMIC_STATE_3_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceExtendedDynamicState3FeaturesEXT , extended_dynamic_state3 )
#endif
#ifdef VK_EXT_EXTENDED_DYNAMIC_STATE_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT , extended_dynamic_state )
#endif
#ifdef VK_NV_EXTENDED_SPARSE_ADDRESS_SPACE_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceExtendedSparseAddressSpaceFeaturesNV , extended_sparse_address_space )
#endif
#ifdef VK_ANDROID_EXTERNAL_FORMAT_RESOLVE_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceExternalFormatResolveFeaturesANDROID , external_format_resolve )
#endif
#ifdef VK_NV_EXTERNAL_MEMORY_RDMA_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceExternalMemoryRDMAFeaturesNV , external_memory_rdma )
#endif
#ifdef VK_QNX_EXTERNAL_MEMORY_SCREEN_BUFFER_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceExternalMemoryScreenBufferFeaturesQNX , external_memory_screen_buffer )
#endif
#ifdef VK_EXT_DEVICE_FAULT_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceFaultFeaturesEXT , fault )
#endif
#ifdef VK_EXT_FRAGMENT_DENSITY_MAP_2_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceFragmentDensityMap2FeaturesEXT , fragment_density_map2 )
#endif
#ifdef VK_EXT_FRAGMENT_DENSITY_MAP_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceFragmentDensityMapFeaturesEXT , fragment_density_map )
#endif
#ifdef VK_QCOM_FRAGMENT_DENSITY_MAP_OFFSET_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceFragmentDensityMapOffsetFeaturesQCOM , fragment_density_map_offset )
#endif
#ifdef VK_NV_FRAGMENT_SHADER_BARYCENTRIC_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceFragmentShaderBarycentricFeaturesNV , fragment_shader_barycentric )
#endif
#ifdef VK_EXT_FRAGMENT_SHADER_INTERLOCK_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceFragmentShaderInterlockFeaturesEXT , fragment_shader_interlock )
#endif
#ifdef VK_NV_FRAGMENT_SHADING_RATE_ENUMS_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceFragmentShadingRateEnumsFeaturesNV , fragment_shading_rate_enums )
#endif
#ifdef VK_KHR_FRAGMENT_SHADING_RATE_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceFragmentShadingRateFeaturesKHR , fragment_shading_rate )
#endif
#ifdef VK_EXT_FRAME_BOUNDARY_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceFrameBoundaryFeaturesEXT , frame_boundary )
#endif
#ifdef VK_KHR_GLOBAL_PRIORITY_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceGlobalPriorityQueryFeaturesKHR , global_priority_query )
#elif defined(VK_EXT_GLOBAL_PRIORITY_QUERY_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceGlobalPriorityQueryFeaturesEXT , global_priority_query )
#endif
#ifdef VK_EXT_GRAPHICS_PIPELINE_LIBRARY_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceGraphicsPipelineLibraryFeaturesEXT , graphics_pipeline_library )
#endif
#ifdef VK_EXT_HOST_IMAGE_COPY_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceHostImageCopyFeaturesEXT, host_image_copy )
#endif

#ifdef VK_VERSION_1_2
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceHostQueryResetFeatures , host_query_reset )
#elif defined(VK_EXT_HOST_QUERY_RESET_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceHostQueryResetFeaturesEXT , host_query_reset )
#endif
#ifdef VK_EXT_IMAGE_ROBUSTNESS_EXTENSION_NAME 
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceImageRobustnessFeaturesEXT , image_robustness )
#endif
#ifdef VK_EXT_IMAGE_SLICED_VIEW_OF_3D_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceImageSlicedViewOf3DFeaturesEXT , image_sliced_view_of_3d )
#endif
#ifdef VK_EXT_IMAGE_VIEW_MIN_LOD_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceImageViewMinLodFeaturesEXT , image_view_min_lod )
#endif
#ifdef VK_VERSION_1_2
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceImagelessFramebufferFeatures , image_less_framebuffer )
#elif defined(VK_KHR_IMAGELESS_FRAMEBUFFER_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceImagelessFramebufferFeaturesKHR , image_less_framebuffer )
#endif
#ifdef VK_EXT_INDEX_TYPE_UINT8_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceIndexTypeUint8FeaturesEXT , index_type_unit8 )
#endif
#ifdef VK_NV_INHERITED_VIEWPORT_SCISSOR_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceInheritedViewportScissorFeaturesNV , inherited_viewport_scissor )
#endif
#ifdef VK_EXT_INLINE_UNIFORM_BLOCK_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceInlineUniformBlockFeaturesEXT , inline_uniform_block )
#endif
#ifdef VK_HUAWEI_INVOCATION_MASK_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceInvocationMaskFeaturesHUAWEI , invocation_mask )
#endif
#ifdef VK_EXT_LEGACY_DITHERING_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceLegacyDitheringFeaturesEXT , legacy_dithering )
#endif
#ifdef VK_EXT_LINE_RASTERIZATION_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceLineRasterizationFeaturesEXT , line_rasterization )
#endif
#ifdef VK_NV_LINEAR_COLOR_ATTACHMENT_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceLinearColorAttachmentFeaturesNV , linear_color_attachment )
#endif
#ifdef VK_KHR_MAINTENANCE_4_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceMaintenance4FeaturesKHR , maintenance4 )
#endif
#ifdef VK_KHR_MAINTENANCE_5_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceMaintenance5FeaturesKHR , maintenance5 )
#endif
#ifdef VK_KHR_MAINTENANCE_6_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceMaintenance6FeaturesKHR , maintenance6 )
#endif
#ifdef VK_NV_MEMORY_DECOMPRESSION_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceMemoryDecompressionFeaturesNV , memory_decompression )
#endif
#ifdef VK_EXT_MEMORY_PRIORITY_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceMemoryPriorityFeaturesEXT , memory_priority )
#endif
#ifdef VK_EXT_MESH_SHADER_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceMeshShaderFeaturesEXT , mesh_shader )
#endif
#ifdef VK_NV_MESH_SHADER_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceMeshShaderFeaturesNV , mesh_shader_nv )
#endif
#ifdef VK_EXT_MULTI_DRAW_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceMultiDrawFeaturesEXT , multi_draw )
#endif
#ifdef VK_EXT_MULTISAMPLED_RENDER_TO_SINGLE_SAMPLED_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceMultisampledRenderToSingleSampledFeaturesEXT , multisampled_render_to_single_sampled )
#endif
#ifdef VK_VERSION_1_1
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceMultiviewFeatures , multiview )
#elif defined(VK_KHR_MULTIVIEW_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceMultiviewFeaturesKHR , multiview )
#endif
#ifdef VK_QCOM_MULTIVIEW_PER_VIEW_RENDER_AREAS_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceMultiviewPerViewRenderAreasFeaturesQCOM , multiview_per_view_render_areas )
#endif
#ifdef VK_QCOM_MULTIVIEW_PER_VIEW_VIEWPORTS_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceMultiviewPerViewViewportsFeaturesQCOM , multiview_per_view_viewports )
#endif
#ifdef VK_EXT_MUTABLE_DESCRIPTOR_TYPE_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceMutableDescriptorTypeFeaturesEXT , mutable_descriptor_type )
#elif defined(VK_VALVE_MUTABLE_DESCRIPTOR_TYPE_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceMutableDescriptorTypeFeaturesVALVE , mutable_descriptor_type )
#endif
#ifdef VK_EXT_NESTED_COMMAND_BUFFER_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceNestedCommandBufferFeaturesEXT , nexted_command_buffer )
#endif
#ifdef VK_EXT_NON_SEAMLESS_CUBE_MAP_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceNonSeamlessCubeMapFeaturesEXT , non_seamless_cube_map )
#endif
#ifdef VK_EXT_OPACITY_MICROMAP_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceOpacityMicromapFeaturesEXT , opacity_micromap )
#endif
#ifdef VK_NV_OPTICAL_FLOW_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceOpticalFlowFeaturesNV , optical_flow )
#endif
#ifdef VK_EXT_PAGEABLE_DEVICE_LOCAL_MEMORY_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDevicePageableDeviceLocalMemoryFeaturesEXT , pageable_device_local_memory )
#endif
#ifdef VK_NV_PER_STAGE_DESCRIPTOR_SET_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDevicePerStageDescriptorSetFeaturesNV , per_stage_descriptor_set )
#endif
#ifdef VK_KHR_PERFORMANCE_QUERY_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDevicePerformanceQueryFeaturesKHR , performance_query )
#endif
#ifdef VK_EXT_PIPELINE_CREATION_CACHE_CONTROL_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDevicePipelineCreationCacheControlFeaturesEXT , pipeline_creation_cache_control )
#endif
#ifdef VK_KHR_PIPELINE_EXECUTABLE_PROPERTIES_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDevicePipelineExecutablePropertiesFeaturesKHR , pipeline_executable_properties )
#endif
#ifdef VK_EXT_PIPELINE_LIBRARY_GROUP_HANDLES_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDevicePipelineLibraryGroupHandlesFeaturesEXT , pipeline_library_group_handles )
#endif
#ifdef VK_EXT_PIPELINE_PROPERTIES_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDevicePipelinePropertiesFeaturesEXT , pipeline_properties )
#endif
#ifdef VK_EXT_PIPELINE_PROTECTED_ACCESS_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDevicePipelineProtectedAccessFeaturesEXT , pipeline_protected_access )
#endif
#ifdef VK_EXT_PIPELINE_ROBUSTNESS_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDevicePipelineRobustnessFeaturesEXT , pipeline_robustness )
#endif
#ifdef VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDevicePortabilitySubsetFeaturesKHR , portability_subset )
#endif
#ifdef VK_NV_PRESENT_BARRIER_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDevicePresentBarrierFeaturesNV , present_barrier )
#endif
#ifdef VK_KHR_PRESENT_ID_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDevicePresentIdFeaturesKHR , present_id )
#endif
#ifdef VK_KHR_PRESENT_WAIT_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDevicePresentWaitFeaturesKHR , present_wait )
#endif
#ifdef VK_EXT_PRIMITIVE_TOPOLOGY_LIST_RESTART_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDevicePrimitiveTopologyListRestartFeaturesEXT , primitive_topology_list_restart )
#endif
#ifdef VK_EXT_PRIMITIVES_GENERATED_QUERY_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDevicePrimitivesGeneratedQueryFeaturesEXT , primitives_generated_query )
#endif
#ifdef VK_EXT_PRIVATE_DATA_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDevicePrivateDataFeaturesEXT , private_data )
#endif
#ifdef VK_VERSION_1_1
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceProtectedMemoryFeatures , protected_memory )
#endif
#ifdef VK_EXT_PROVOKING_VERTEX_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceProvokingVertexFeaturesEXT , provoking_vertex )
#endif
#ifdef VK_EXT_RGBA10X6_FORMATS_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceRGBA10X6FormatsFeaturesEXT , rgba10x6_formats )
#endif
#ifdef VK_EXT_RASTERIZATION_ORDER_ATTACHMENT_ACCESS_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceRasterizationOrderAttachmentAccessFeaturesEXT , rasterization_order_attachment_access )
#endif
#ifdef VK_KHR_RAY_QUERY_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceRayQueryFeaturesKHR , ray_query )
#endif
#ifdef VK_NV_RAY_TRACING_INVOCATION_REORDER_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceRayTracingInvocationReorderFeaturesNV , ray_tracing_invocation_reorder )
#endif
#ifdef VK_KHR_RAY_TRACING_MAINTENANCE_1_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceRayTracingMaintenance1FeaturesKHR , ray_tracing_maintenance1 )
#endif
#ifdef VK_NV_RAY_TRACING_MOTION_BLUR_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceRayTracingMotionBlurFeaturesNV , ray_tracing_motion_blur )
#endif
#ifdef VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceRayTracingPipelineFeaturesKHR , ray_tracing_pipeline )
#endif
#ifdef VK_KHR_RAY_TRACING_POSITION_FETCH_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceRayTracingPositionFetchFeaturesKHR , ray_tracing_position_fetch )
#endif
#ifdef VK_IMG_RELAXED_LINE_RASTERIZATION_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceRelaxedLineRasterizationFeaturesIMG , relaxed_line_rasterization )
#endif
#ifdef VK_ARM_RENDER_PASS_STRIPED_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceRenderPassStripedFeaturesARM , render_pass_striped )
#endif
#ifdef VK_NV_REPRESENTATIVE_FRAGMENT_TEST_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceRepresentativeFragmentTestFeaturesNV , representative_fragment_test )
#endif
#ifdef VK_EXT_ROBUSTNESS_2_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceRobustness2FeaturesEXT , robustness2 )
#endif
#ifdef VK_VERSION_1_1
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceSamplerYcbcrConversionFeatures , sampler_ycbcr_conversion )
#elif defined(VK_KHR_SAMPLER_YCBCR_CONVERSION_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceSamplerYcbcrConversionFeaturesKHR , sampler_ycbcr_conversion )
#endif
#ifdef VK_VERSION_1_2
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceScalarBlockLayoutFeatures , scalar_block_layout )
#elif defined(VK_EXT_SCALAR_BLOCK_LAYOUT_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceScalarBlockLayoutFeaturesEXT , scalar_block_layout )
#endif
#ifdef VK_ARM_SCHEDULING_CONTROLS_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceSchedulingControlsFeaturesARM , scheduling_controls )
#endif
#ifdef VK_VERSION_1_2
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceSeparateDepthStencilLayoutsFeatures , separate_depth_stencil_layouts )
#elif defined(VK_KHR_SEPARATE_DEPTH_STENCIL_LAYOUTS_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceSeparateDepthStencilLayoutsFeaturesKHR , separate_depth_stencil_layouts )
#endif
#ifdef VK_EXT_SHADER_ATOMIC_FLOAT_2_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceShaderAtomicFloat2FeaturesEXT , shader_atomic_float2 )
#endif
#ifdef VK_EXT_SHADER_ATOMIC_FLOAT_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceShaderAtomicFloatFeaturesEXT , shader_atomic_float )
#endif
#ifdef VK_VERSION_1_2
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceShaderAtomicInt64Features , shader_atomic_int64 )
#elif defined(VK_KHR_SHADER_ATOMIC_INT64_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceShaderAtomicInt64FeaturesKHR , shader_atomic_int64 )
#endif
#ifdef VK_KHR_SHADER_CLOCK_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceShaderClockFeaturesKHR , shader_clock )
#endif
#ifdef VK_ARM_SHADER_CORE_BUILTINS_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceShaderCoreBuiltinsFeaturesARM , shader_core_builtins )
#endif
#ifdef VK_EXT_SHADER_DEMOTE_TO_HELPER_INVOCATION_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceShaderDemoteToHelperInvocationFeaturesEXT , shader_demote_to_helper_invocation )
#endif
#ifdef VK_VERSION_1_1
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceShaderDrawParametersFeatures , shader_draw_parameters )
#endif
#ifdef VK_AMD_SHADER_EARLY_AND_LATE_FRAGMENT_TESTS_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceShaderEarlyAndLateFragmentTestsFeaturesAMD , shader_early_and_late_fragment_tests )
#endif
#ifdef VK_AMDX_SHADER_ENQUEUE_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceShaderEnqueueFeaturesAMDX , shader_enqueue )
#endif
#ifdef VK_KHR_SHADER_EXPECT_ASSUME_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceShaderExpectAssumeFeaturesKHR , shader_expect_assume )
#endif
#ifdef VK_VERSION_1_2
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceShaderFloat16Int8Features , shader_float16_int8 )
#elif defined(VK_KHR_SHADER_FLOAT16_INT8_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceShaderFloat16Int8FeaturesKHR , shader_float16_int8 )
#endif
#ifdef VK_KHR_SHADER_FLOAT_CONTROLS_2_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceShaderFloatControls2FeaturesKHR , shader_float_controls2 )
#endif
#ifdef VK_EXT_SHADER_IMAGE_ATOMIC_INT64_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceShaderImageAtomicInt64FeaturesEXT , shader_image_atomic_int64 )
#endif
#ifdef VK_NV_SHADER_IMAGE_FOOTPRINT_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceShaderImageFootprintFeaturesNV , shader_image_footprint )
#endif
#ifdef VK_KHR_SHADER_INTEGER_DOT_PRODUCT_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceShaderIntegerDotProductFeaturesKHR , shader_integer_dot_product )
#endif
#ifdef VK_INTEL_SHADER_INTEGER_FUNCTIONS_2_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceShaderIntegerFunctions2FeaturesINTEL , shader_integer_functions2 )
#endif
#ifdef VK_KHR_SHADER_MAXIMAL_RECONVERGENCE_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceShaderMaximalReconvergenceFeaturesKHR , shader_maximal_reconvergence )
#endif
#ifdef VK_EXT_SHADER_MODULE_IDENTIFIER_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceShaderModuleIdentifierFeaturesEXT , shader_module_identifier )
#endif
#ifdef VK_EXT_SHADER_OBJECT_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceShaderObjectFeaturesEXT , shader_object )
#endif
#ifdef VK_KHR_SHADER_QUAD_CONTROL_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceShaderQuadControlFeaturesKHR , shader_quad_control )
#endif
#ifdef VK_NV_SHADER_SM_BUILTINS_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceShaderSMBuiltinsFeaturesNV , shader_smbuiltins )
#endif
#ifdef VK_VERSION_1_2
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceShaderSubgroupExtendedTypesFeatures , shader_subgroup_extended_types )
#elif defined(VK_KHR_SHADER_SUBGROUP_EXTENDED_TYPES_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceShaderSubgroupExtendedTypesFeaturesKHR , shader_subgroup_extended_types )
#endif
#ifdef VK_KHR_SHADER_SUBGROUP_ROTATE_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceShaderSubgroupRotateFeaturesKHR , shader_subgroup_rotate )
#endif
#ifdef VK_KHR_SHADER_SUBGROUP_UNIFORM_CONTROL_FLOW_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceShaderSubgroupUniformControlFlowFeaturesKHR , shader_subgroup_uniform_control_flow )
#endif
#ifdef VK_KHR_SHADER_TERMINATE_INVOCATION_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceShaderTerminateInvocationFeaturesKHR , shader_terminate_invocation )
#endif
#ifdef VK_EXT_SHADER_TILE_IMAGE_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceShaderTileImageFeaturesEXT , shader_tile_image )
#endif
#ifdef VK_NV_SHADING_RATE_IMAGE_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceShadingRateImageFeaturesNV , shading_rate_image )
#endif
#ifdef VK_EXT_SUBGROUP_SIZE_CONTROL_EXTENSION_NAME 
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceSubgroupSizeControlFeaturesEXT , subgroup_size_control )
#endif
#ifdef VK_EXT_SUBPASS_MERGE_FEEDBACK_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceSubpassMergeFeedbackFeaturesEXT , subpass_merge_feedback )
#endif
#ifdef VK_HUAWEI_SUBPASS_SHADING_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceSubpassShadingFeaturesHUAWEI , subpass_shading )
#endif
#ifdef VK_EXT_SWAPCHAIN_MAINTENANCE_1_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceSwapchainMaintenance1FeaturesEXT ,  swapchain_maintenance1 )
#endif
#ifdef VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceSynchronization2FeaturesKHR , synchronization2 )
#endif
#ifdef VK_EXT_TEXEL_BUFFER_ALIGNMENT_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceTexelBufferAlignmentFeaturesEXT , texel_buffer_alignment )
#endif
#ifdef VK_EXT_TEXTURE_COMPRESSION_ASTC_HDR_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceTextureCompressionASTCHDRFeaturesEXT , texture_compression_astc_hdr )
#endif
#ifdef VK_VERSION_1_2
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceTimelineSemaphoreFeatures , timeline_semaphore )
#elif defined(VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceTimelineSemaphoreFeaturesKHR , timeline_semaphore )
#endif
#ifdef VK_EXT_TRANSFORM_FEEDBACK_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceTransformFeedbackFeaturesEXT , transform_feedback )
#endif
#ifdef VK_VERSION_1_2
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceUniformBufferStandardLayoutFeatures , uniform_buffer_standard_layout )
#elif defined(VK_KHR_UNIFORM_BUFFER_STANDARD_LAYOUT_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceUniformBufferStandardLayoutFeaturesKHR , uniform_buffer_standard_layout )
#endif
#ifdef VK_VERSION_1_1
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceVariablePointersFeatures , variable_pointers )
#elif defined(VK_KHR_VARIABLE_POINTERS_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceVariablePointersFeaturesKHR , variable_pointers )
#endif
#ifdef VK_EXT_VERTEX_ATTRIBUTE_DIVISOR_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceVertexAttributeDivisorFeaturesEXT , vertex_attribute_divisor )
#endif
#ifdef VK_EXT_VERTEX_INPUT_DYNAMIC_STATE_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceVertexInputDynamicStateFeaturesEXT , vertex_input_dynamic_state )
#endif
#ifdef VK_KHR_VIDEO_MAINTENANCE_1_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceVideoMaintenance1FeaturesKHR , video_maintenance1 )
#endif
#ifdef VK_VERSION_1_2
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceVulkanMemoryModelFeatures , vulkan_memory_model )
#elif VK_KHR_VULKAN_MEMORY_MODEL_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceVulkanMemoryModelFeaturesKHR , vulkan_memory_model )
#endif
#ifdef VK_KHR_WORKGROUP_MEMORY_EXPLICIT_LAYOUT_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceWorkgroupMemoryExplicitLayoutFeaturesKHR , workgroup_memory_explicit_layout )
#endif
#ifdef VK_EXT_YCBCR_2PLANE_444_FORMATS_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceYcbcr2Plane444FormatsFeaturesEXT , ycbcr_2plane_444_formats )
#endif
#ifdef VK_QCOM_YCBCR_DEGAMMA_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceYcbcrDegammaFeaturesQCOM , ycbcr_degamma )
#endif
#ifdef VK_EXT_YCBCR_IMAGE_ARRAYS_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceYcbcrImageArraysFeaturesEXT , ycbcr_image_array )
#endif
#ifdef VK_KHR_ZERO_INITIALIZE_WORKGROUP_MEMORY_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceZeroInitializeWorkgroupMemoryFeaturesKHR , zero_initialize_workgroup_memory )
#endif
  };
  void to_json( nlohmann::json&, const physical_device_features_t& );
}

#endif

