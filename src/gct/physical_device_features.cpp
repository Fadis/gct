#include <gct/instance.hpp>
#include <gct/physical_device_properties.hpp>
#include <gct/physical_device_features.hpp>
#include <vulkan2json/PhysicalDeviceFeatures.hpp>

#ifdef VK_VERSION_1_1
#include <vulkan2json/PhysicalDevice16BitStorageFeatures.hpp>
#elif defined(VK_KHR_16BIT_STORAGE_EXTENSION_NAME)
#include <vulkan2json/PhysicalDevice16BitStorageFeaturesKHR.hpp>
#endif
#ifdef VK_EXT_4444_FORMATS_EXTENSION_NAME
#include <vulkan2json/PhysicalDevice4444FormatsFeaturesEXT.hpp>
#endif
#ifdef VK_VERSION_1_2
#include <vulkan2json/PhysicalDevice8BitStorageFeatures.hpp>
#elif defined(VK_KHR_8BIT_STORAGE_EXTENSION_NAME)
#include <vulkan2json/PhysicalDevice8BitStorageFeaturesKHR.hpp>
#endif
#ifdef VK_EXT_ASTC_DECODE_MODE_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceASTCDecodeFeaturesEXT.hpp>
#endif
#ifdef VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceAccelerationStructureFeaturesKHR.hpp>
#endif
#ifdef VK_EXT_BLEND_OPERATION_ADVANCED_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceBlendOperationAdvancedFeaturesEXT.hpp>
#endif
#ifdef VK_EXT_BORDER_COLOR_SWIZZLE_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceBorderColorSwizzleFeaturesEXT.hpp>
#endif
#ifdef VK_VERSION_1_2
#include <vulkan2json/PhysicalDeviceBufferDeviceAddressFeatures.hpp>
#elif defined(VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME)
#include <vulkan2json/PhysicalDeviceBufferDeviceAddressFeaturesKHR.hpp>
#endif
#ifdef VK_EXT_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceBufferDeviceAddressFeaturesEXT.hpp>
#endif
#ifdef VK_AMD_DEVICE_COHERENT_MEMORY_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceCoherentMemoryFeaturesAMD.hpp>
#endif
#ifdef VK_EXT_COLOR_WRITE_ENABLE_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceColorWriteEnableFeaturesEXT.hpp>
#endif
#ifdef VK_NV_COMPUTE_SHADER_DERIVATIVES_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceComputeShaderDerivativesFeaturesNV.hpp>
#endif
#ifdef VK_EXT_CONDITIONAL_RENDERING_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceConditionalRenderingFeaturesEXT.hpp>
#endif

#ifdef VK_NV_COOPERATIVE_MATRIX_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceCooperativeMatrixFeaturesNV.hpp>
#endif
#ifdef VK_NV_CORNER_SAMPLED_IMAGE_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceCornerSampledImageFeaturesNV.hpp>
#endif
#ifdef VK_NV_COVERAGE_REDUCTION_MODE_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceCoverageReductionModeFeaturesNV.hpp>
#endif
#ifdef VK_EXT_CUSTOM_BORDER_COLOR_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceCustomBorderColorFeaturesEXT.hpp>
#endif
#ifdef VK_NV_DEDICATED_ALLOCATION_IMAGE_ALIASING_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceDedicatedAllocationImageAliasingFeaturesNV.hpp>
#endif
#ifdef VK_EXT_DEPTH_CLIP_ENABLE_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceDepthClipEnableFeaturesEXT.hpp>
#endif
#ifdef VK_VERSION_1_2
#include <vulkan2json/PhysicalDeviceDescriptorIndexingFeatures.hpp>
#elif defined(VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME)
#include <vulkan2json/PhysicalDeviceDescriptorIndexingFeaturesEXT.hpp>
#endif
#ifdef VK_NV_DEVICE_GENERATED_COMMANDS_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceDeviceGeneratedCommandsFeaturesNV.hpp>
#endif
#ifdef VK_EXT_DEVICE_MEMORY_REPORT_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceDeviceMemoryReportFeaturesEXT.hpp>
#endif
#ifdef VK_NV_DEVICE_DIAGNOSTICS_CONFIG_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceDiagnosticsConfigFeaturesNV.hpp>
#endif
#ifdef VK_NV_SCISSOR_EXCLUSIVE_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceExclusiveScissorFeaturesNV.hpp>
#endif
#ifdef VK_EXT_EXTENDED_DYNAMIC_STATE_2_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceExtendedDynamicState2FeaturesEXT.hpp>
#endif
#ifdef VK_EXT_EXTENDED_DYNAMIC_STATE_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceExtendedDynamicStateFeaturesEXT.hpp>
#endif
#ifdef VK_NV_EXTERNAL_MEMORY_RDMA_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceExternalMemoryRDMAFeaturesNV.hpp>
#endif
#ifdef VK_EXT_FRAGMENT_DENSITY_MAP_2_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceFragmentDensityMap2FeaturesEXT.hpp>
#endif
#ifdef VK_EXT_FRAGMENT_DENSITY_MAP_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceFragmentDensityMapFeaturesEXT.hpp>
#endif
#ifdef VK_NV_FRAGMENT_SHADER_BARYCENTRIC_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceFragmentShaderBarycentricFeaturesNV.hpp>
#endif

#ifdef VK_EXT_FRAGMENT_SHADER_INTERLOCK_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceFragmentShaderInterlockFeaturesEXT.hpp>
#endif
#ifdef VK_NV_FRAGMENT_SHADING_RATE_ENUMS_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceFragmentShadingRateEnumsFeaturesNV.hpp>
#endif
#ifdef VK_KHR_FRAGMENT_SHADING_RATE_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceFragmentShadingRateFeaturesKHR.hpp>
#endif
#ifdef VK_EXT_GLOBAL_PRIORITY_QUERY_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceGlobalPriorityQueryFeaturesEXT.hpp>
#endif
#ifdef VK_VERSION_1_2
#include <vulkan2json/PhysicalDeviceHostQueryResetFeatures.hpp>
#elif defined(VK_EXT_HOST_QUERY_RESET_EXTENSION_NAME)
#include <vulkan2json/PhysicalDeviceHostQueryResetFeaturesEXT.hpp>
#endif
#ifdef VK_EXT_IMAGE_ROBUSTNESS_EXTENSION_NAME 
#include <vulkan2json/PhysicalDeviceImageRobustnessFeaturesEXT.hpp>
#endif
#ifdef VK_VERSION_1_2
#include <vulkan2json/PhysicalDeviceImagelessFramebufferFeatures.hpp>
#elif defined(VK_KHR_IMAGELESS_FRAMEBUFFER_EXTENSION_NAME)
#include <vulkan2json/PhysicalDeviceImagelessFramebufferFeaturesKHR.hpp>
#endif
#ifdef VK_EXT_INDEX_TYPE_UINT8_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceIndexTypeUint8FeaturesEXT.hpp>
#endif
#ifdef VK_NV_INHERITED_VIEWPORT_SCISSOR_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceInheritedViewportScissorFeaturesNV.hpp>
#endif
#ifdef VK_EXT_INLINE_UNIFORM_BLOCK_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceInlineUniformBlockFeaturesEXT.hpp>
#endif
#ifdef VK_HUAWEI_INVOCATION_MASK_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceInvocationMaskFeaturesHUAWEI.hpp>
#endif
#ifdef VK_EXT_LINE_RASTERIZATION_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceLineRasterizationFeaturesEXT.hpp>
#endif
#ifdef VK_KHR_MAINTENANCE_4_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceMaintenance4FeaturesKHR.hpp>
#endif
#ifdef VK_EXT_MEMORY_PRIORITY_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceMemoryPriorityFeaturesEXT.hpp>
#endif
#ifdef VK_NV_MESH_SHADER_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceMeshShaderFeaturesNV.hpp>
#endif
#ifdef VK_EXT_MULTI_DRAW_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceMultiDrawFeaturesEXT.hpp>
#endif

#ifdef VK_VERSION_1_1
#include <vulkan2json/PhysicalDeviceMultiviewFeatures.hpp>
#elif defined(VK_KHR_MULTIVIEW_EXTENSION_NAME)
#include <vulkan2json/PhysicalDeviceMultiviewFeaturesKHR.hpp>
#endif
#ifdef VK_VALVE_MUTABLE_DESCRIPTOR_TYPE_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceMutableDescriptorTypeFeaturesVALVE.hpp>
#endif
#ifdef VK_EXT_PAGEABLE_DEVICE_LOCAL_MEMORY_EXTENSION_NAME
#include <vulkan2json/PhysicalDevicePageableDeviceLocalMemoryFeaturesEXT.hpp>
#endif
#ifdef VK_KHR_PERFORMANCE_QUERY_EXTENSION_NAME
#include <vulkan2json/PhysicalDevicePerformanceQueryFeaturesKHR.hpp>
#endif
#ifdef VK_EXT_PIPELINE_CREATION_CACHE_CONTROL_EXTENSION_NAME
#include <vulkan2json/PhysicalDevicePipelineCreationCacheControlFeaturesEXT.hpp>
#endif
#ifdef VK_KHR_PIPELINE_EXECUTABLE_PROPERTIES_EXTENSION_NAME
#include <vulkan2json/PhysicalDevicePipelineExecutablePropertiesFeaturesKHR.hpp>
#endif
#ifdef VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME
#include <vulkan2json/PhysicalDevicePortabilitySubsetFeaturesKHR.hpp>
#endif
#ifdef VK_KHR_PRESENT_ID_EXTENSION_NAME
#include <vulkan2json/PhysicalDevicePresentIdFeaturesKHR.hpp>
#endif
#ifdef VK_KHR_PRESENT_WAIT_EXTENSION_NAME
#include <vulkan2json/PhysicalDevicePresentWaitFeaturesKHR.hpp>
#endif
#ifdef VK_EXT_PRIMITIVE_TOPOLOGY_LIST_RESTART_EXTENSION_NAME
#include <vulkan2json/PhysicalDevicePrimitiveTopologyListRestartFeaturesEXT.hpp>
#endif
#ifdef VK_EXT_PRIVATE_DATA_EXTENSION_NAME
#include <vulkan2json/PhysicalDevicePrivateDataFeaturesEXT.hpp>
#endif
#ifdef VK_VERSION_1_1
#include <vulkan2json/PhysicalDeviceProtectedMemoryFeatures.hpp>
#endif
#ifdef VK_EXT_PROVOKING_VERTEX_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceProvokingVertexFeaturesEXT.hpp>
#endif
#ifdef VK_EXT_RGBA10X6_FORMATS_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceRGBA10X6FormatsFeaturesEXT.hpp>
#endif
#ifdef VK_KHR_RAY_QUERY_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceRayQueryFeaturesKHR.hpp>
#endif
#ifdef VK_NV_RAY_TRACING_MOTION_BLUR_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceRayTracingMotionBlurFeaturesNV.hpp>
#endif
#ifdef VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceRayTracingPipelineFeaturesKHR.hpp>
#endif

#ifdef VK_NV_REPRESENTATIVE_FRAGMENT_TEST_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceRepresentativeFragmentTestFeaturesNV.hpp>
#endif
#ifdef VK_EXT_ROBUSTNESS_2_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceRobustness2FeaturesEXT.hpp>
#endif
#ifdef VK_VERSION_1_1
#include <vulkan2json/PhysicalDeviceSamplerYcbcrConversionFeatures.hpp>
#elif defined(VK_KHR_SAMPLER_YCBCR_CONVERSION_EXTENSION_NAME)
#include <vulkan2json/PhysicalDeviceSamplerYcbcrConversionFeaturesKHR.hpp>
#endif
#ifdef VK_VERSION_1_2
#include <vulkan2json/PhysicalDeviceScalarBlockLayoutFeatures.hpp>
#elif defined(VK_EXT_SCALAR_BLOCK_LAYOUT_EXTENSION_NAME)
#include <vulkan2json/PhysicalDeviceScalarBlockLayoutFeaturesEXT.hpp>
#endif
#ifdef VK_VERSION_1_2
#include <vulkan2json/PhysicalDeviceSeparateDepthStencilLayoutsFeatures.hpp>
#elif defined(VK_KHR_SEPARATE_DEPTH_STENCIL_LAYOUTS_EXTENSION_NAME)
#include <vulkan2json/PhysicalDeviceSeparateDepthStencilLayoutsFeaturesKHR.hpp>
#endif
#ifdef VK_EXT_SHADER_ATOMIC_FLOAT_2_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceShaderAtomicFloat2FeaturesEXT.hpp>
#endif
#ifdef VK_EXT_SHADER_ATOMIC_FLOAT_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceShaderAtomicFloatFeaturesEXT.hpp>
#endif
#ifdef VK_VERSION_1_2
#include <vulkan2json/PhysicalDeviceShaderAtomicInt64Features.hpp>
#elif defined(VK_KHR_SHADER_ATOMIC_INT64_EXTENSION_NAME)
#include <vulkan2json/PhysicalDeviceShaderAtomicInt64FeaturesKHR.hpp>
#endif
#ifdef VK_KHR_SHADER_CLOCK_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceShaderClockFeaturesKHR.hpp>
#endif
#ifdef VK_EXT_SHADER_DEMOTE_TO_HELPER_INVOCATION_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceShaderDemoteToHelperInvocationFeaturesEXT.hpp>
#endif
#ifdef VK_VERSION_1_1
#include <vulkan2json/PhysicalDeviceShaderDrawParametersFeatures.hpp>
#endif
#ifdef VK_VERSION_1_2
#include <vulkan2json/PhysicalDeviceShaderFloat16Int8Features.hpp>
#elif defined(VK_KHR_SHADER_FLOAT16_INT8_EXTENSION_NAME)
#include <vulkan2json/PhysicalDeviceShaderFloat16Int8FeaturesKHR.hpp>
#endif
#ifdef VK_EXT_SHADER_IMAGE_ATOMIC_INT64_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceShaderImageAtomicInt64FeaturesEXT.hpp>
#endif
#ifdef VK_NV_SHADER_IMAGE_FOOTPRINT_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceShaderImageFootprintFeaturesNV.hpp>
#endif
#ifdef VK_KHR_SHADER_INTEGER_DOT_PRODUCT_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceShaderIntegerDotProductFeaturesKHR.hpp>
#endif

#ifdef VK_INTEL_SHADER_INTEGER_FUNCTIONS_2_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceShaderIntegerFunctions2FeaturesINTEL.hpp>
#endif
#ifdef VK_NV_SHADER_SM_BUILTINS_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceShaderSMBuiltinsFeaturesNV.hpp>
#endif
#ifdef VK_VERSION_1_2
#include <vulkan2json/PhysicalDeviceShaderSubgroupExtendedTypesFeatures.hpp>
#elif defined(VK_KHR_SHADER_SUBGROUP_EXTENDED_TYPES_EXTENSION_NAME)
#include <vulkan2json/PhysicalDeviceShaderSubgroupExtendedTypesFeaturesKHR.hpp>
#endif
#ifdef VK_KHR_SHADER_SUBGROUP_UNIFORM_CONTROL_FLOW_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceShaderSubgroupUniformControlFlowFeaturesKHR.hpp>
#endif
#ifdef VK_KHR_SHADER_TERMINATE_INVOCATION_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceShaderTerminateInvocationFeaturesKHR.hpp>
#endif
#ifdef VK_NV_SHADING_RATE_IMAGE_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceShadingRateImageFeaturesNV.hpp>
#endif
#ifdef VK_EXT_SUBGROUP_SIZE_CONTROL_EXTENSION_NAME 
#include <vulkan2json/PhysicalDeviceSubgroupSizeControlFeaturesEXT.hpp>
#endif
#ifdef VK_HUAWEI_SUBPASS_SHADING_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceSubpassShadingFeaturesHUAWEI.hpp>
#endif
#ifdef VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceSynchronization2FeaturesKHR.hpp>
#endif
#ifdef VK_EXT_TEXEL_BUFFER_ALIGNMENT_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceTexelBufferAlignmentFeaturesEXT.hpp>
#endif
#ifdef VK_EXT_TEXTURE_COMPRESSION_ASTC_HDR_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceTextureCompressionASTCHDRFeaturesEXT.hpp>
#endif
#ifdef VK_VERSION_1_2
#include <vulkan2json/PhysicalDeviceTimelineSemaphoreFeatures.hpp>
#elif defined(VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME)
#include <vulkan2json/PhysicalDeviceTimelineSemaphoreFeaturesKHR.hpp>
#endif
#ifdef VK_EXT_TRANSFORM_FEEDBACK_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceTransformFeedbackFeaturesEXT.hpp>
#endif
#ifdef VK_VERSION_1_2
#include <vulkan2json/PhysicalDeviceUniformBufferStandardLayoutFeatures.hpp>
#elif defined(VK_KHR_UNIFORM_BUFFER_STANDARD_LAYOUT_EXTENSION_NAME)
#include <vulkan2json/PhysicalDeviceUniformBufferStandardLayoutFeaturesKHR.hpp>
#endif
#ifdef VK_VERSION_1_1
#include <vulkan2json/PhysicalDeviceVariablePointersFeatures.hpp>
#elif defined(VK_KHR_VARIABLE_POINTERS_EXTENSION_NAME)
#include <vulkan2json/PhysicalDeviceVariablePointersFeaturesKHR.hpp>
#endif

#ifdef VK_EXT_VERTEX_ATTRIBUTE_DIVISOR_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceVertexAttributeDivisorFeaturesEXT.hpp>
#endif
#ifdef VK_EXT_VERTEX_INPUT_DYNAMIC_STATE_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceVertexInputDynamicStateFeaturesEXT.hpp>
#endif
#ifdef VK_VERSION_1_2
#include <vulkan2json/PhysicalDeviceVulkanMemoryModelFeatures.hpp>
#elif VK_KHR_VULKAN_MEMORY_MODEL_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceVulkanMemoryModelFeaturesKHR.hpp>
#endif
#ifdef VK_KHR_WORKGROUP_MEMORY_EXPLICIT_LAYOUT_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceWorkgroupMemoryExplicitLayoutFeaturesKHR.hpp>
#endif
#ifdef VK_EXT_YCBCR_2PLANE_444_FORMATS_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceYcbcr2Plane444FormatsFeaturesEXT.hpp>
#endif
#ifdef VK_EXT_YCBCR_IMAGE_ARRAYS_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceYcbcrImageArraysFeaturesEXT.hpp>
#endif
#ifdef VK_KHR_ZERO_INITIALIZE_WORKGROUP_MEMORY_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceZeroInitializeWorkgroupMemoryFeaturesKHR.hpp>
#endif

namespace gct {
  void to_json( nlohmann::json &root, const physical_device_features_t &v ) {
    root = nlohmann::json::object();
    root[ "basic" ] = v.get_basic();
#if defined(VK_VERSION_1_1) || defined(VK_KHR_16BIT_STORAGE_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( storage_16bit )
#endif
#ifdef VK_EXT_4444_FORMATS_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( formats_4444 )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_8BIT_STORAGE_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( storage_8bit )
#endif
#ifdef VK_EXT_ASTC_DECODE_MODE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( astc_decode )
#endif
#ifdef VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( acceleration_strucutre )
#endif
#ifdef VK_EXT_BLEND_OPERATION_ADVANCED_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( blend_operation_advanced )
#endif
#ifdef VK_EXT_BORDER_COLOR_SWIZZLE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( border_color_swizzle )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( buffer_device_address )
#endif
#ifdef VK_EXT_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( buffer_device_address_ext )
#endif
#ifdef VK_AMD_DEVICE_COHERENT_MEMORY_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( coherent_memory )
#endif
#ifdef VK_EXT_COLOR_WRITE_ENABLE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( color_write_enable )
#endif
#ifdef VK_NV_COMPUTE_SHADER_DERIVATIVES_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( compute_shader_derivatives )
#endif
#ifdef VK_EXT_CONDITIONAL_RENDERING_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( conditional_rendering )
#endif
#ifdef VK_NV_COOPERATIVE_MATRIX_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( cooperative_matrix )
#endif
#ifdef VK_NV_CORNER_SAMPLED_IMAGE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( corner_sampled_image )
#endif
#ifdef VK_NV_COVERAGE_REDUCTION_MODE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( coverage_reduction_mode )
#endif
#ifdef VK_EXT_CUSTOM_BORDER_COLOR_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( custom_border_color )
#endif
#ifdef VK_NV_DEDICATED_ALLOCATION_IMAGE_ALIASING_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( dedicated_allocation_image_aliasing )
#endif
#ifdef VK_EXT_DEPTH_CLIP_ENABLE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( depth_clip_enable )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( descriptor_indexing )
#endif
#ifdef VK_NV_DEVICE_GENERATED_COMMANDS_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( device_generated_commands )
#endif
#ifdef VK_EXT_DEVICE_MEMORY_REPORT_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( device_memory_report )
#endif
#ifdef VK_NV_DEVICE_DIAGNOSTICS_CONFIG_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( diagnostic_config )
#endif
#ifdef VK_EXT_EXTENDED_DYNAMIC_STATE_2_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( extended_dynamic_state2 )
#endif
#ifdef VK_NV_SCISSOR_EXCLUSIVE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( exclusive_scissor )
#endif
#ifdef VK_EXT_EXTENDED_DYNAMIC_STATE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( extended_dynamic_state )
#endif
#ifdef VK_NV_EXTERNAL_MEMORY_RDMA_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( external_memory_rdma )
#endif
#ifdef VK_EXT_FRAGMENT_DENSITY_MAP_2_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( fragment_density_map2 )
#endif
#ifdef VK_EXT_FRAGMENT_DENSITY_MAP_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( fragment_density_map )
#endif
#ifdef VK_NV_FRAGMENT_SHADER_BARYCENTRIC_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( fragment_shader_barycentric )
#endif
#ifdef VK_EXT_FRAGMENT_SHADER_INTERLOCK_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( fragment_shader_interlock )
#endif
#ifdef VK_NV_FRAGMENT_SHADING_RATE_ENUMS_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( fragment_shading_rate_enums )
#endif
#ifdef VK_KHR_FRAGMENT_SHADING_RATE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( fragment_shading_rate )
#endif
#ifdef VK_EXT_GLOBAL_PRIORITY_QUERY_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( global_priority_query )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_EXT_HOST_QUERY_RESET_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( host_query_reset )
#endif
#ifdef VK_EXT_IMAGE_ROBUSTNESS_EXTENSION_NAME 
    LIBGCT_EXTENSION_TO_JSON( image_robustness )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_IMAGELESS_FRAMEBUFFER_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( image_less_framebuffer )
#endif
#ifdef VK_EXT_INDEX_TYPE_UINT8_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( index_type_unit8 )
#endif
#ifdef VK_NV_INHERITED_VIEWPORT_SCISSOR_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( inherited_viewport_scissor )
#endif
#ifdef VK_EXT_INLINE_UNIFORM_BLOCK_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( inline_uniform_block )
#endif
#ifdef VK_HUAWEI_INVOCATION_MASK_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( invocation_mask )
#endif
#ifdef VK_EXT_LINE_RASTERIZATION_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( line_rasterization )
#endif
#ifdef VK_KHR_MAINTENANCE_4_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( maintenance4 )
#endif
#ifdef VK_EXT_MEMORY_PRIORITY_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( memory_priority )
#endif
#ifdef VK_NV_MESH_SHADER_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( mesh_shader )
#endif
#ifdef VK_EXT_MULTI_DRAW_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( multi_draw )
#endif
#if defined(VK_VERSION_1_1) || defined(VK_KHR_MULTIVIEW_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( multiview )
#endif
#ifdef VK_VALVE_MUTABLE_DESCRIPTOR_TYPE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( mutable_descriptor_type )
#endif
#ifdef VK_EXT_PAGEABLE_DEVICE_LOCAL_MEMORY_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( pageable_device_local_memory )
#endif
#ifdef VK_KHR_PERFORMANCE_QUERY_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( performance_query )
#endif
#ifdef VK_EXT_PIPELINE_CREATION_CACHE_CONTROL_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( pipeline_creation_cache_control )
#endif
#ifdef VK_KHR_PIPELINE_EXECUTABLE_PROPERTIES_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( pipeline_executable_properties )
#endif
#ifdef VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( portability_subset )
#endif
#ifdef VK_KHR_PRESENT_ID_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( present_id )
#endif
#ifdef VK_KHR_PRESENT_WAIT_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( present_wait )
#endif
#ifdef VK_EXT_PRIMITIVE_TOPOLOGY_LIST_RESTART_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( primitive_topology_list_restart )
#endif
#ifdef VK_EXT_PRIVATE_DATA_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( private_data )
#endif
#ifdef VK_VERSION_1_1
    LIBGCT_EXTENSION_TO_JSON( protected_memory )
#endif
#ifdef VK_EXT_PROVOKING_VERTEX_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( provoking_vertex )
#endif
#ifdef VK_EXT_RGBA10X6_FORMATS_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( rgba10x6_formats )
#endif
#ifdef VK_KHR_RAY_QUERY_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( ray_query )
#endif
#ifdef VK_NV_RAY_TRACING_MOTION_BLUR_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( ray_tracing_motion_blur )
#endif
#ifdef VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( ray_tracing_pipeline )
#endif
#ifdef VK_NV_REPRESENTATIVE_FRAGMENT_TEST_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( representative_fragment_test )
#endif
#ifdef VK_EXT_ROBUSTNESS_2_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( robustness2 )
#endif
#if defined(VK_VERSION_1_1) || defined(VK_KHR_SAMPLER_YCBCR_CONVERSION_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( sampler_ycbcr_conversion )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_EXT_SCALAR_BLOCK_LAYOUT_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( scalar_block_layout )
#endif
#if defined(scalar_block_layout) || defined(VK_KHR_SEPARATE_DEPTH_STENCIL_LAYOUTS_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( separate_depth_stencil_layouts )
#endif
#ifdef VK_EXT_SHADER_ATOMIC_FLOAT_2_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( shader_atomic_float2 )
#endif
#ifdef VK_EXT_SHADER_ATOMIC_FLOAT_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( shader_atomic_float )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_SHADER_ATOMIC_INT64_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( shader_atomic_float )
#endif
#ifdef VK_KHR_SHADER_CLOCK_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( shader_clock )
#endif
#ifdef VK_EXT_SHADER_DEMOTE_TO_HELPER_INVOCATION_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( shader_demote_to_helper_invocation )
#endif
#ifdef VK_VERSION_1_1
    LIBGCT_EXTENSION_TO_JSON( shader_draw_parameters )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_SHADER_FLOAT16_INT8_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( shader_float16_int8 )
#endif
#ifdef VK_EXT_SHADER_IMAGE_ATOMIC_INT64_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( shader_image_atomic_int64 )
#endif
#ifdef VK_NV_SHADER_IMAGE_FOOTPRINT_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( shader_image_footprint )
#endif
#ifdef VK_KHR_SHADER_INTEGER_DOT_PRODUCT_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( shader_integer_dot_product )
#endif
#ifdef VK_INTEL_SHADER_INTEGER_FUNCTIONS_2_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( shader_integer_functions2 )
#endif
#ifdef VK_NV_SHADER_SM_BUILTINS_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( shader_smbuiltins )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_SHADER_SUBGROUP_EXTENDED_TYPES_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( shader_subgroup_extended_types )
#endif
#ifdef VK_KHR_SHADER_SUBGROUP_UNIFORM_CONTROL_FLOW_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( shader_subgroup_uniform_control_flow )
#endif
#ifdef VK_KHR_SHADER_TERMINATE_INVOCATION_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( shader_terminate_invocation )
#endif
#ifdef VK_NV_SHADING_RATE_IMAGE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( shading_rate_image )
#endif
#ifdef VK_EXT_SUBGROUP_SIZE_CONTROL_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( subgroup_size_control )
#endif
#ifdef VK_HUAWEI_SUBPASS_SHADING_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( subpass_shading )
#endif
#ifdef VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( synchronization2 )
#endif
#ifdef VK_EXT_TEXEL_BUFFER_ALIGNMENT_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( texel_buffer_alignment )
#endif
#ifdef VK_EXT_TEXTURE_COMPRESSION_ASTC_HDR_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( texture_compression_astc_hdr )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( timeline_semaphore )
#endif
#ifdef VK_EXT_TRANSFORM_FEEDBACK_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( transform_feedback )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_UNIFORM_BUFFER_STANDARD_LAYOUT_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( uniform_buffer_standard_layout )
#endif
#if defined(VK_VERSION_1_1) || defined(VK_KHR_VARIABLE_POINTERS_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( variable_pointers )
#endif
#ifdef VK_EXT_VERTEX_ATTRIBUTE_DIVISOR_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( vertex_attribute_divisor )
#endif
#ifdef VK_EXT_VERTEX_INPUT_DYNAMIC_STATE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( vertex_input_dynamic_state )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_VULKAN_MEMORY_MODEL_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( vulkan_memory_model )
#endif
#ifdef VK_KHR_WORKGROUP_MEMORY_EXPLICIT_LAYOUT_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( workgroup_memory_explicit_layout )
#endif
#ifdef VK_EXT_YCBCR_2PLANE_444_FORMATS_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( ycbcr_2plane_444_formats )
#endif
#ifdef VK_EXT_YCBCR_IMAGE_ARRAYS_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( ycbcr_image_array )
#endif
#ifdef VK_KHR_ZERO_INITIALIZE_WORKGROUP_MEMORY_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( zero_initialize_workgroup_memory )
#endif
  }

  physical_device_features_t::physical_device_features_t(
    instance_t &instance,
    vk::PhysicalDevice &pdev,
    const physical_device_properties_t &props
  ) {
    const auto &ext = props.get_activated_extensions();
#if defined(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME) || defined(VK_VERSION_1_1)
    if(
      instance.get_api_version() >= VK_MAKE_VERSION( 1, 1, 0 ) ||
      instance.get_activated_extensions().find( "VK_KHR_get_physical_device_properties2" ) != instance.get_activated_extensions().end()
    ) {
#if defined(VK_VERSION_1_1) || defined(VK_KHR_16BIT_STORAGE_EXTENSION_NAME)
      LIBGCT_EXTENSION_CREATE_IF_VERSION_OR_EXT( storage_16bit, 1, 1, 0, "VK_KHR_16bit_storage" )
#endif
#ifdef VK_EXT_4444_FORMATS_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( formats_4444, "VK_EXT_4444_formats" )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_8BIT_STORAGE_EXTENSION_NAME)
      LIBGCT_EXTENSION_CREATE_IF_VERSION_OR_EXT( storage_8bit, 1, 2, 0, "VK_KHR_8bit_storage" )
#endif
#ifdef VK_EXT_ASTC_DECODE_MODE_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( astc_decode, "VK_EXT_astc_decode_mode" )
#endif
#ifdef VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( acceleration_strucutre, "VK_KHR_acceleration_structure" )
#endif
#ifdef VK_EXT_BLEND_OPERATION_ADVANCED_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( blend_operation_advanced, "VK_EXT_blend_operation_advanced" )
#endif
#ifdef VK_EXT_BORDER_COLOR_SWIZZLE_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( border_color_swizzle, "VK_EXT_border_color_swizzle" )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME)
      LIBGCT_EXTENSION_CREATE_IF_VERSION_OR_EXT( buffer_device_address, 1, 2, 0, "VK_KHR_buffer_device_address" )
#endif
#ifdef VK_EXT_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( buffer_device_address_ext, "VK_EXT_buffer_device_address" )
#endif
#ifdef VK_AMD_DEVICE_COHERENT_MEMORY_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( coherent_memory, "VK_AMD_device_coherent_memory" )
#endif
#ifdef VK_EXT_COLOR_WRITE_ENABLE_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( color_write_enable, "VK_EXT_color_write_enable" )
#endif
#ifdef VK_NV_COMPUTE_SHADER_DERIVATIVES_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( compute_shader_derivatives, "VK_NV_compute_shader_derivatives" )
#endif
#ifdef VK_EXT_CONDITIONAL_RENDERING_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( conditional_rendering, "VK_EXT_conditional_rendering" )
#endif
#ifdef VK_NV_COOPERATIVE_MATRIX_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( cooperative_matrix, "VK_NV_cooperative_matrix" )
#endif
#ifdef VK_NV_CORNER_SAMPLED_IMAGE_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( corner_sampled_image, "VK_NV_corner_sampled_image" )
#endif
#ifdef VK_NV_COVERAGE_REDUCTION_MODE_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( coverage_reduction_mode, "VK_NV_coverage_reduction_mode" )
#endif
#ifdef VK_EXT_CUSTOM_BORDER_COLOR_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( custom_border_color, "VK_EXT_custom_border_color" )
#endif
#ifdef VK_NV_DEDICATED_ALLOCATION_IMAGE_ALIASING_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( dedicated_allocation_image_aliasing, "VK_NV_dedicated_allocation_image_aliasing" )
#endif
#ifdef VK_EXT_DEPTH_CLIP_ENABLE_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( depth_clip_enable, "VK_EXT_depth_clip_enable" )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME)
      LIBGCT_EXTENSION_CREATE_IF_VERSION_OR_EXT( descriptor_indexing, 1, 2, 0, "VK_EXT_descriptor_indexing" )
#endif
#ifdef VK_NV_DEVICE_GENERATED_COMMANDS_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( device_generated_commands, "VK_NV_device_generated_commands" )
#endif
#ifdef VK_EXT_DEVICE_MEMORY_REPORT_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( device_memory_report, "VK_EXT_device_memory_report" )
#endif
#ifdef VK_NV_DEVICE_DIAGNOSTICS_CONFIG_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( diagnostic_config, "VK_NV_device_diagnostics_config" )
#endif
#ifdef VK_NV_SCISSOR_EXCLUSIVE_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( exclusive_scissor, "VK_NV_scissor_exclusive" )
#endif
#ifdef VK_EXT_EXTENDED_DYNAMIC_STATE_2_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( extended_dynamic_state2, "VK_EXT_extended_dynamic_state2" )
#endif
#ifdef VK_EXT_EXTENDED_DYNAMIC_STATE_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( extended_dynamic_state, "VK_EXT_extended_dynamic_state" )
#endif
#ifdef VK_NV_EXTERNAL_MEMORY_RDMA_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( external_memory_rdma, "VK_NV_external_memory_rdma" )
#endif
#ifdef VK_EXT_FRAGMENT_DENSITY_MAP_2_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( fragment_density_map2, "VK_EXT_fragment_density_map2" )
#endif
#ifdef VK_EXT_FRAGMENT_DENSITY_MAP_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( fragment_density_map, "VK_EXT_fragment_density_map" )
#endif
#ifdef VK_NV_FRAGMENT_SHADER_BARYCENTRIC_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( fragment_shader_barycentric, "VK_NV_fragment_shader_barycentric" )
#endif
#ifdef VK_EXT_FRAGMENT_SHADER_INTERLOCK_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( fragment_shader_interlock, "VK_EXT_fragment_shader_interlock" )
#endif
#ifdef VK_NV_FRAGMENT_SHADING_RATE_ENUMS_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( fragment_shading_rate_enums, "VK_NV_fragment_shading_rate_enums" )
#endif
#ifdef VK_KHR_FRAGMENT_SHADING_RATE_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( fragment_shading_rate, "VK_KHR_fragment_shading_rate" )
#endif
#ifdef VK_EXT_GLOBAL_PRIORITY_QUERY_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( global_priority_query, "VK_EXT_global_priority_query" )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_EXT_HOST_QUERY_RESET_EXTENSION_NAME)
      LIBGCT_EXTENSION_CREATE_IF_VERSION_OR_EXT( host_query_reset, 1, 2, 0, "VK_EXT_host_query_reset" )
#endif
#ifdef VK_EXT_IMAGE_ROBUSTNESS_EXTENSION_NAME 
      LIBGCT_EXTENSION_CREATE_IF_EXT( image_robustness, "VK_EXT_image_robustness" )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_IMAGELESS_FRAMEBUFFER_EXTENSION_NAME)
      LIBGCT_EXTENSION_CREATE_IF_VERSION_OR_EXT( image_less_framebuffer, 1, 2, 0, "VK_KHR_imageless_framebuffer" )
#endif
#ifdef VK_EXT_INDEX_TYPE_UINT8_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( index_type_unit8, "VK_EXT_index_type_uint8" )
#endif
#ifdef VK_NV_INHERITED_VIEWPORT_SCISSOR_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( inherited_viewport_scissor, "VK_NV_inherited_viewport_scissor" )
#endif
#ifdef VK_EXT_INLINE_UNIFORM_BLOCK_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( inline_uniform_block, "VK_EXT_inline_uniform_block" )
#endif
#ifdef VK_HUAWEI_INVOCATION_MASK_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( invocation_mask, "VK_HUAWEI_invocation_mask" )
#endif
#ifdef VK_EXT_LINE_RASTERIZATION_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( line_rasterization, "VK_EXT_line_rasterization" )
#endif
#ifdef VK_KHR_MAINTENANCE_4_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( maintenance4, "VK_KHR_maintenance4" )
#endif
#ifdef VK_EXT_MEMORY_PRIORITY_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( memory_priority, "VK_EXT_memory_priority" )
#endif
#ifdef VK_NV_MESH_SHADER_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( mesh_shader, "VK_NV_mesh_shader" )
#endif
#ifdef VK_EXT_MULTI_DRAW_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( multi_draw, "VK_EXT_multi_draw" )
#endif
#if defined(VK_VERSION_1_1) || defined(VK_KHR_MULTIVIEW_EXTENSION_NAME)
      LIBGCT_EXTENSION_CREATE_IF_EXT( multiview, "VK_KHR_multiview" )
#endif
#ifdef VK_VALVE_MUTABLE_DESCRIPTOR_TYPE_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( mutable_descriptor_type, "VK_VALVE_mutable_descriptor_type" )
#endif
#ifdef VK_EXT_PAGEABLE_DEVICE_LOCAL_MEMORY_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( pageable_device_local_memory, "VK_EXT_pageable_device_local_memory" )
#endif
#ifdef VK_KHR_PERFORMANCE_QUERY_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( performance_query, "VK_KHR_performance_query" )
#endif
#ifdef VK_EXT_PIPELINE_CREATION_CACHE_CONTROL_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( pipeline_creation_cache_control, "VK_EXT_pipeline_creation_cache_control" )
#endif
#ifdef VK_KHR_PIPELINE_EXECUTABLE_PROPERTIES_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( pipeline_executable_properties, "VK_KHR_pipeline_executable_properties" )
#endif
#ifdef VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( portability_subset, "VK_KHR_portability_subset" )
#endif
#ifdef VK_KHR_PRESENT_ID_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( present_id, "VK_KHR_present_id" )
#endif
#ifdef VK_KHR_PRESENT_WAIT_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( present_wait, "VK_KHR_present_wait" )
#endif
#ifdef VK_EXT_PRIMITIVE_TOPOLOGY_LIST_RESTART_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( primitive_topology_list_restart, "VK_EXT_primitive_topology_list_restart" )
#endif
#ifdef VK_EXT_PRIVATE_DATA_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( private_data, "VK_EXT_private_data" )
#endif
#ifdef VK_VERSION_1_1
      LIBGCT_EXTENSION_CREATE_IF_VERSION( protected_memory, 1, 1, 0 )
#endif
#ifdef VK_EXT_PROVOKING_VERTEX_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( provoking_vertex, "VK_EXT_provoking_vertex" )
#endif
#ifdef VK_EXT_RGBA10X6_FORMATS_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( rgba10x6_formats, "VK_EXT_rgba10x6_formats" )
#endif
#ifdef VK_KHR_RAY_QUERY_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( ray_query, "VK_KHR_ray_query" )
#endif
#ifdef VK_NV_RAY_TRACING_MOTION_BLUR_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( ray_tracing_motion_blur, "VK_NV_ray_tracing_motion_blur" )
#endif
#ifdef VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( ray_tracing_pipeline, "VK_KHR_ray_tracing_pipeline" )
#endif
#ifdef VK_NV_REPRESENTATIVE_FRAGMENT_TEST_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( representative_fragment_test, "VK_NV_representative_fragment_test" )
#endif
#ifdef VK_EXT_ROBUSTNESS_2_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( robustness2, "VK_EXT_robustness2" )
#endif
#if defined(VK_VERSION_1_1) || defined(VK_KHR_SAMPLER_YCBCR_CONVERSION_EXTENSION_NAME)
      LIBGCT_EXTENSION_CREATE_IF_VERSION_OR_EXT( sampler_ycbcr_conversion, 1, 1, 0, "VK_KHR_sampler_ycbcr_conversion" )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_EXT_SCALAR_BLOCK_LAYOUT_EXTENSION_NAME)
      LIBGCT_EXTENSION_CREATE_IF_VERSION_OR_EXT( scalar_block_layout, 1, 2, 0, "VK_EXT_scalar_block_layout" )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_SEPARATE_DEPTH_STENCIL_LAYOUTS_EXTENSION_NAME)
      LIBGCT_EXTENSION_CREATE_IF_VERSION_OR_EXT( separate_depth_stencil_layouts, 1, 2, 0, "VK_KHR_separate_depth_stencil_layouts" )
#endif
#ifdef VK_EXT_SHADER_ATOMIC_FLOAT_2_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( shader_atomic_float2, "VK_EXT_shader_atomic_float2" )
#endif
#ifdef VK_EXT_SHADER_ATOMIC_FLOAT_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( shader_atomic_float, "VK_EXT_shader_atomic_float" )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_SHADER_ATOMIC_INT64_EXTENSION_NAME)
      LIBGCT_EXTENSION_CREATE_IF_EXT( shader_atomic_int64, "VK_KHR_shader_atomic_int64" )
#endif
#ifdef VK_KHR_SHADER_CLOCK_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( shader_clock, "VK_KHR_shader_clock" )
#endif
#ifdef VK_EXT_SHADER_DEMOTE_TO_HELPER_INVOCATION_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( shader_demote_to_helper_invocation, "VK_EXT_shader_demote_to_helper_invocation" )
#endif
#ifdef VK_VERSION_1_1
      LIBGCT_EXTENSION_CREATE_IF_VERSION( shader_draw_parameters, 1, 1, 0 )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_SHADER_FLOAT16_INT8_EXTENSION_NAME)
      LIBGCT_EXTENSION_CREATE_IF_VERSION_OR_EXT( shader_float16_int8, 1, 2, 0, "VK_KHR_shader_float16_int8" )
#endif
#ifdef VK_EXT_SHADER_IMAGE_ATOMIC_INT64_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( shader_image_atomic_int64, "VK_EXT_shader_image_atomic_int64" )
#endif
#ifdef VK_NV_SHADER_IMAGE_FOOTPRINT_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( shader_image_footprint, "VK_NV_shader_image_footprint" )
#endif
#ifdef VK_KHR_SHADER_INTEGER_DOT_PRODUCT_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( shader_integer_dot_product, "VK_KHR_shader_integer_dot_product" )
#endif
#ifdef VK_INTEL_SHADER_INTEGER_FUNCTIONS_2_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( shader_integer_functions2, "VK_INTEL_shader_integer_functions2" )
#endif
#ifdef VK_NV_SHADER_SM_BUILTINS_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( shader_smbuiltins, "VK_NV_shader_sm_builtins" )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_SHADER_SUBGROUP_EXTENDED_TYPES_EXTENSION_NAME)
      LIBGCT_EXTENSION_CREATE_IF_VERSION_OR_EXT( shader_subgroup_extended_types, 1, 2, 0, "VK_KHR_shader_subgroup_extended_types" )
#endif
#ifdef VK_KHR_SHADER_SUBGROUP_UNIFORM_CONTROL_FLOW_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( shader_subgroup_uniform_control_flow, "VK_KHR_shader_subgroup_uniform_control_flow" )
#endif
#ifdef VK_KHR_SHADER_TERMINATE_INVOCATION_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( shader_terminate_invocation, "VK_KHR_shader_terminate_invocation" )
#endif
#ifdef VK_NV_SHADING_RATE_IMAGE_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( shading_rate_image, "VK_NV_shading_rate_image" )
#endif
#ifdef VK_EXT_SUBGROUP_SIZE_CONTROL_EXTENSION_NAME 
      LIBGCT_EXTENSION_CREATE_IF_EXT( subgroup_size_control, "VK_EXT_subgroup_size_control" )
#endif
#ifdef VK_HUAWEI_SUBPASS_SHADING_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( subpass_shading, "VK_HUAWEI_subpass_shading" )
#endif
#ifdef VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( synchronization2, "VK_KHR_synchronization2" )
#endif
#ifdef VK_EXT_TEXEL_BUFFER_ALIGNMENT_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( texel_buffer_alignment, "VK_EXT_texel_buffer_alignment" )
#endif
#ifdef VK_EXT_TEXTURE_COMPRESSION_ASTC_HDR_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( texture_compression_astc_hdr, "VK_EXT_texture_compression_astc_hdr" )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME)
      LIBGCT_EXTENSION_CREATE_IF_VERSION_OR_EXT( timeline_semaphore, 1, 2, 0, "VK_KHR_timeline_semaphore" )
#endif
#ifdef VK_EXT_TRANSFORM_FEEDBACK_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( transform_feedback, "VK_EXT_transform_feedback" )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_UNIFORM_BUFFER_STANDARD_LAYOUT_EXTENSION_NAME)
      LIBGCT_EXTENSION_CREATE_IF_VERSION_OR_EXT( uniform_buffer_standard_layout, 1, 2, 0, "VK_KHR_uniform_buffer_standard_layout" )
#endif
#if defined(VK_VERSION_1_1) || defined(VK_KHR_VARIABLE_POINTERS_EXTENSION_NAME)
      LIBGCT_EXTENSION_CREATE_IF_VERSION_OR_EXT( variable_pointers, 1, 1, 0, "VK_KHR_variable_pointers" )
#endif
#ifdef VK_EXT_VERTEX_ATTRIBUTE_DIVISOR_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( vertex_attribute_divisor, "VK_EXT_vertex_attribute_divisor" )
#endif
#ifdef VK_EXT_VERTEX_INPUT_DYNAMIC_STATE_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( vertex_input_dynamic_state, "VK_EXT_vertex_input_dynamic_state" )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_VULKAN_MEMORY_MODEL_EXTENSION_NAME)
      LIBGCT_EXTENSION_CREATE_IF_EXT( vulkan_memory_model, "VK_KHR_vulkan_memory_model" )
#endif
#ifdef VK_KHR_WORKGROUP_MEMORY_EXPLICIT_LAYOUT_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( workgroup_memory_explicit_layout, "VK_KHR_workgroup_memory_explicit_layout" )
#endif
#ifdef VK_EXT_YCBCR_2PLANE_444_FORMATS_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( ycbcr_2plane_444_formats, "VK_EXT_ycbcr_2plane_444_formats" )
#endif
#ifdef VK_EXT_YCBCR_IMAGE_ARRAYS_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( ycbcr_image_array, "VK_EXT_ycbcr_image_arrays" )
#endif
#ifdef VK_KHR_ZERO_INITIALIZE_WORKGROUP_MEMORY_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( zero_initialize_workgroup_memory, "VK_KHR_zero_initialize_workgroup_memory" )
#endif
     rebuild_chain();
#ifdef VK_VERSION_1_1
      pdev.getFeatures2(
        &basic
      );
#elif defined(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME)
      pdev.getFeatures2KHR(
        &basic
      );
#endif
    }
    else {
      pdev.getFeatures(
        &basic.features
      );
    }
#else
    {
      pdev.getFeatures(
        &basic
      );
    }
#endif
  }
  physical_device_features_t &physical_device_features_t::rebuild_chain() {
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
#if defined(VK_VERSION_1_1) || defined(VK_KHR_16BIT_STORAGE_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( storage_16bit )
#endif
#ifdef VK_EXT_4444_FORMATS_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( formats_4444 )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_8BIT_STORAGE_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( storage_8bit )
#endif
#ifdef VK_EXT_ASTC_DECODE_MODE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( astc_decode )
#endif
#ifdef VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( acceleration_strucutre )
#endif
#ifdef VK_EXT_BLEND_OPERATION_ADVANCED_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( blend_operation_advanced )
#endif
#ifdef VK_EXT_BORDER_COLOR_SWIZZLE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( border_color_swizzle )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( buffer_device_address )
#endif
#ifdef VK_EXT_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( buffer_device_address_ext )
#endif
#ifdef VK_AMD_DEVICE_COHERENT_MEMORY_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( coherent_memory )
#endif
#ifdef VK_EXT_COLOR_WRITE_ENABLE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( color_write_enable )
#endif
#ifdef VK_NV_COMPUTE_SHADER_DERIVATIVES_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( compute_shader_derivatives )
#endif
#ifdef VK_EXT_CONDITIONAL_RENDERING_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( conditional_rendering )
#endif
#ifdef VK_NV_COOPERATIVE_MATRIX_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( cooperative_matrix )
#endif
#ifdef VK_NV_CORNER_SAMPLED_IMAGE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( corner_sampled_image )
#endif
#ifdef VK_NV_COVERAGE_REDUCTION_MODE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( coverage_reduction_mode )
#endif
#ifdef VK_EXT_CUSTOM_BORDER_COLOR_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( custom_border_color )
#endif
#ifdef VK_NV_DEDICATED_ALLOCATION_IMAGE_ALIASING_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( dedicated_allocation_image_aliasing )
#endif
#ifdef VK_EXT_DEPTH_CLIP_ENABLE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( depth_clip_enable )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( descriptor_indexing )
#endif
#ifdef VK_NV_DEVICE_GENERATED_COMMANDS_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( device_generated_commands )
#endif
#ifdef VK_EXT_DEVICE_MEMORY_REPORT_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( device_memory_report )
#endif
#ifdef VK_NV_DEVICE_DIAGNOSTICS_CONFIG_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( diagnostic_config )
#endif
#ifdef VK_NV_SCISSOR_EXCLUSIVE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( exclusive_scissor )
#endif
#ifdef VK_EXT_EXTENDED_DYNAMIC_STATE_2_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( extended_dynamic_state2 )
#endif
#ifdef VK_EXT_EXTENDED_DYNAMIC_STATE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( extended_dynamic_state )
#endif
#ifdef VK_NV_EXTERNAL_MEMORY_RDMA_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( external_memory_rdma )
#endif
#ifdef VK_EXT_FRAGMENT_DENSITY_MAP_2_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( fragment_density_map2 )
#endif
#ifdef VK_EXT_FRAGMENT_DENSITY_MAP_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( fragment_density_map )
#endif
#ifdef VK_NV_FRAGMENT_SHADER_BARYCENTRIC_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( fragment_shader_barycentric )
#endif
#ifdef VK_EXT_FRAGMENT_SHADER_INTERLOCK_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( fragment_shader_interlock )
#endif
#ifdef VK_NV_FRAGMENT_SHADING_RATE_ENUMS_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( fragment_shading_rate_enums )
#endif
#ifdef VK_KHR_FRAGMENT_SHADING_RATE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( fragment_shading_rate )
#endif
#ifdef VK_EXT_GLOBAL_PRIORITY_QUERY_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( global_priority_query )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_EXT_HOST_QUERY_RESET_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( host_query_reset )
#endif
#ifdef VK_EXT_IMAGE_ROBUSTNESS_EXTENSION_NAME 
    LIBGCT_EXTENSION_REBUILD_CHAIN( image_robustness )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_IMAGELESS_FRAMEBUFFER_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( image_less_framebuffer )
#endif
#ifdef VK_EXT_INDEX_TYPE_UINT8_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( index_type_unit8 )
#endif
#ifdef VK_NV_INHERITED_VIEWPORT_SCISSOR_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( inherited_viewport_scissor )
#endif
#ifdef VK_EXT_INLINE_UNIFORM_BLOCK_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( inline_uniform_block )
#endif
#ifdef VK_HUAWEI_INVOCATION_MASK_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( invocation_mask )
#endif
#ifdef VK_EXT_LINE_RASTERIZATION_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( line_rasterization )
#endif
#ifdef VK_KHR_MAINTENANCE_4_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( maintenance4 )
#endif
#ifdef VK_EXT_MEMORY_PRIORITY_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( memory_priority )
#endif
#ifdef VK_NV_MESH_SHADER_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( mesh_shader )
#endif
#ifdef VK_EXT_MULTI_DRAW_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( multi_draw )
#endif
#if defined(VK_VERSION_1_1) || defined(VK_KHR_MULTIVIEW_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( multiview )
#endif
#ifdef VK_VALVE_MUTABLE_DESCRIPTOR_TYPE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( mutable_descriptor_type )
#endif
#ifdef VK_EXT_PAGEABLE_DEVICE_LOCAL_MEMORY_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( pageable_device_local_memory )
#endif
#ifdef VK_KHR_PERFORMANCE_QUERY_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( performance_query )
#endif
#ifdef VK_EXT_PIPELINE_CREATION_CACHE_CONTROL_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( pipeline_creation_cache_control )
#endif
#ifdef VK_KHR_PIPELINE_EXECUTABLE_PROPERTIES_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( pipeline_executable_properties )
#endif
#ifdef VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( portability_subset )
#endif
#ifdef VK_KHR_PRESENT_ID_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( present_id )
#endif
#ifdef VK_KHR_PRESENT_WAIT_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( present_wait )
#endif
#ifdef VK_EXT_PRIMITIVE_TOPOLOGY_LIST_RESTART_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( primitive_topology_list_restart )
#endif
#ifdef VK_EXT_PRIVATE_DATA_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( private_data )
#endif
#ifdef VK_VERSION_1_1
    LIBGCT_EXTENSION_REBUILD_CHAIN( protected_memory )
#endif
#ifdef VK_EXT_PROVOKING_VERTEX_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( provoking_vertex )
#endif
#ifdef VK_EXT_RGBA10X6_FORMATS_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( rgba10x6_formats )
#endif
#ifdef VK_KHR_RAY_QUERY_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( ray_query )
#endif
#ifdef VK_NV_RAY_TRACING_MOTION_BLUR_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( ray_tracing_motion_blur )
#endif
#ifdef VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( ray_tracing_pipeline )
#endif
#ifdef VK_NV_REPRESENTATIVE_FRAGMENT_TEST_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( representative_fragment_test )
#endif
#ifdef VK_EXT_ROBUSTNESS_2_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( robustness2 )
#endif
#if defined(VK_VERSION_1_1) || defined(VK_KHR_SAMPLER_YCBCR_CONVERSION_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( sampler_ycbcr_conversion )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_EXT_SCALAR_BLOCK_LAYOUT_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( scalar_block_layout )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_SEPARATE_DEPTH_STENCIL_LAYOUTS_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( separate_depth_stencil_layouts )
#endif
#ifdef VK_EXT_SHADER_ATOMIC_FLOAT_2_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( shader_atomic_float2 )
#endif
#ifdef VK_EXT_SHADER_ATOMIC_FLOAT_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( shader_atomic_float )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_SHADER_ATOMIC_INT64_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( shader_atomic_int64 )
#endif
#ifdef VK_KHR_SHADER_CLOCK_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( shader_clock )
#endif
#ifdef VK_EXT_SHADER_DEMOTE_TO_HELPER_INVOCATION_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( shader_demote_to_helper_invocation )
#endif
#ifdef VK_VERSION_1_1
    LIBGCT_EXTENSION_REBUILD_CHAIN( shader_draw_parameters )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_SHADER_FLOAT16_INT8_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( shader_float16_int8 )
#endif
#ifdef VK_EXT_SHADER_IMAGE_ATOMIC_INT64_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( shader_image_atomic_int64 )
#endif
#ifdef VK_NV_SHADER_IMAGE_FOOTPRINT_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( shader_image_footprint )
#endif
#ifdef VK_KHR_SHADER_INTEGER_DOT_PRODUCT_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( shader_integer_dot_product )
#endif
#ifdef VK_INTEL_SHADER_INTEGER_FUNCTIONS_2_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( shader_integer_functions2 )
#endif
#ifdef VK_NV_SHADER_SM_BUILTINS_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( shader_smbuiltins )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_SHADER_SUBGROUP_EXTENDED_TYPES_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( shader_subgroup_extended_types )
#endif
#ifdef VK_KHR_SHADER_SUBGROUP_UNIFORM_CONTROL_FLOW_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( shader_subgroup_uniform_control_flow )
#endif
#ifdef VK_KHR_SHADER_TERMINATE_INVOCATION_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( shader_terminate_invocation )
#endif
#ifdef VK_NV_SHADING_RATE_IMAGE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( shading_rate_image )
#endif
#ifdef VK_EXT_SUBGROUP_SIZE_CONTROL_EXTENSION_NAME 
    LIBGCT_EXTENSION_REBUILD_CHAIN( subgroup_size_control )
#endif
#ifdef VK_HUAWEI_SUBPASS_SHADING_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( subpass_shading )
#endif
#ifdef VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( synchronization2 )
#endif
#ifdef VK_EXT_TEXEL_BUFFER_ALIGNMENT_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( texel_buffer_alignment )
#endif
#ifdef VK_EXT_TEXTURE_COMPRESSION_ASTC_HDR_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( texture_compression_astc_hdr )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( timeline_semaphore )
#endif
#ifdef VK_EXT_TRANSFORM_FEEDBACK_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( transform_feedback )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_UNIFORM_BUFFER_STANDARD_LAYOUT_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( uniform_buffer_standard_layout )
#endif
#if defined(VK_VERSION_1_1) || defined(VK_KHR_VARIABLE_POINTERS_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( variable_pointers )
#endif
#ifdef VK_EXT_VERTEX_ATTRIBUTE_DIVISOR_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( vertex_attribute_divisor )
#endif
#ifdef VK_EXT_VERTEX_INPUT_DYNAMIC_STATE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( vertex_input_dynamic_state )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_VULKAN_MEMORY_MODEL_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( vulkan_memory_model )
#endif
#ifdef VK_KHR_WORKGROUP_MEMORY_EXPLICIT_LAYOUT_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( workgroup_memory_explicit_layout )
#endif
#ifdef VK_EXT_YCBCR_2PLANE_444_FORMATS_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( ycbcr_2plane_444_formats )
#endif
#ifdef VK_EXT_YCBCR_IMAGE_ARRAYS_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( ycbcr_image_array )
#endif
#ifdef VK_KHR_ZERO_INITIALIZE_WORKGROUP_MEMORY_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( zero_initialize_workgroup_memory )
#endif
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
}
