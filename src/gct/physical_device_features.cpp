#include <nlohmann/json.hpp>
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
#ifdef VK_EXT_DEVICE_ADDRESS_BINDING_REPORT_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceAddressBindingReportFeaturesEXT.hpp>
#endif
#ifdef VK_SEC_AMIGO_PROFILING_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceAmigoProfilingFeaturesSEC.hpp>
#endif
#ifdef VK_EXT_ATTACHMENT_FEEDBACK_LOOP_DYNAMIC_STATE_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceAttachmentFeedbackLoopDynamicStateFeaturesEXT.hpp>
#endif
#ifdef VK_EXT_ATTACHMENT_FEEDBACK_LOOP_LAYOUT_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceAttachmentFeedbackLoopLayoutFeaturesEXT.hpp>
#endif
#ifdef VK_EXT_BLEND_OPERATION_ADVANCED_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceBlendOperationAdvancedFeaturesEXT.hpp>
#endif
#ifdef VK_NV_CLUSTER_ACCELERATION_STRUCTURE_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceClusterAccelerationStructureFeaturesNV.hpp>
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
#ifdef VK_HUAWEI_CLUSTER_CULLING_SHADER_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceClusterCullingShaderFeaturesHUAWEI.hpp>
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
#ifdef VK_NV_COOPERATIVE_MATRIX_2_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceCooperativeMatrix2FeaturesNV.hpp>
#endif
#ifdef VK_KHR_COOPERATIVE_MATRIX_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceCooperativeMatrixFeaturesKHR.hpp>
#endif
#ifdef VK_NV_COOPERATIVE_MATRIX_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceCooperativeMatrixFeaturesNV.hpp>
#endif
#ifdef VK_NV_COOPERATIVE_VECTOR_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceCooperativeVectorFeaturesNV.hpp>
#endif
#ifdef VK_NV_COPY_MEMORY_INDIRECT_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceCopyMemoryIndirectFeaturesNV.hpp>
#endif
#ifdef VK_NV_CORNER_SAMPLED_IMAGE_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceCornerSampledImageFeaturesNV.hpp>
#endif
#ifdef VK_NV_COVERAGE_REDUCTION_MODE_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceCoverageReductionModeFeaturesNV.hpp>
#endif
#ifdef VK_QCOM_FILTER_CUBIC_CLAMP_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceCubicClampFeaturesQCOM.hpp>
#endif
#ifdef VK_QCOM_FILTER_CUBIC_WEIGHTS_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceCubicWeightsFeaturesQCOM.hpp>
#endif
#ifdef VK_NV_CUDA_KERNEL_LAUNCH_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceCudaKernelLaunchFeaturesNV.hpp>
#endif
#ifdef VK_EXT_CUSTOM_BORDER_COLOR_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceCustomBorderColorFeaturesEXT.hpp>
#endif
#ifdef VK_NV_DEDICATED_ALLOCATION_IMAGE_ALIASING_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceDedicatedAllocationImageAliasingFeaturesNV.hpp>
#endif
#ifdef VK_EXT_DEPTH_BIAS_CONTROL_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceDepthBiasControlFeaturesEXT.hpp>
#endif
#ifdef VK_EXT_DEPTH_CLAMP_ZERO_ONE_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceDepthClampZeroOneFeaturesEXT.hpp>
#endif
#ifdef VK_EXT_DEPTH_CLIP_CONTROL_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceDepthClipControlFeaturesEXT.hpp>
#endif
#ifdef VK_EXT_DEPTH_CLIP_ENABLE_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceDepthClipEnableFeaturesEXT.hpp>
#endif
#ifdef VK_EXT_DESCRIPTOR_BUFFER_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceDescriptorBufferFeaturesEXT.hpp>
#endif
#if defined(VK_EXT_DESCRIPTOR_BUFFER_EXTENSION_NAME) && defined(VK_ARM_TENSORS_EXTENSION_NAME)
#include <vulkan2json/PhysicalDeviceDescriptorBufferTensorFeaturesARM.hpp>
#endif
#ifdef VK_VERSION_1_2
#include <vulkan2json/PhysicalDeviceDescriptorIndexingFeatures.hpp>
#elif defined(VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME)
#include <vulkan2json/PhysicalDeviceDescriptorIndexingFeaturesEXT.hpp>
#endif
#ifdef VK_NV_DESCRIPTOR_POOL_OVERALLOCATION_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceDescriptorPoolOverallocationFeaturesNV.hpp>
#endif
#ifdef VK_NV_DEVICE_GENERATED_COMMANDS_COMPUTE_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceDeviceGeneratedCommandsComputeFeaturesNV.hpp>
#endif
#ifdef VK_EXT_DEVICE_GENERATED_COMMANDS_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceDeviceGeneratedCommandsFeaturesEXT.hpp>
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
#ifdef VK_NV_DISPLACEMENT_MICROMAP_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceDisplacementMicromapFeaturesNV.hpp>
#endif
#ifdef VK_VERSION_1_3
#include <vulkan2json/PhysicalDeviceDynamicRenderingFeatures.hpp>
#elif defined(VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME)
#include <vulkan2json/PhysicalDeviceDynamicRenderingFeaturesKHR.hpp>
#endif
#ifdef VK_VERSION_1_4
#include <vulkan2json/PhysicalDeviceDynamicRenderingLocalReadFeatures.hpp>
#elif defined(VK_KHR_DYNAMIC_RENDERING_LOCAL_READ_EXTENSION_NAME)
#include <vulkan2json/PhysicalDeviceDynamicRenderingLocalReadFeaturesKHR.hpp>
#endif
#ifdef VK_EXT_DYNAMIC_RENDERING_UNUSED_ATTACHMENTS_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceDynamicRenderingUnusedAttachmentsFeaturesEXT.hpp>
#endif
#ifdef VK_NV_SCISSOR_EXCLUSIVE_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceExclusiveScissorFeaturesNV.hpp>
#endif
#ifdef VK_EXT_EXTENDED_DYNAMIC_STATE_2_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceExtendedDynamicState2FeaturesEXT.hpp>
#endif
#ifdef VK_EXT_EXTENDED_DYNAMIC_STATE_3_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceExtendedDynamicState3FeaturesEXT.hpp>
#endif
#ifdef VK_EXT_EXTENDED_DYNAMIC_STATE_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceExtendedDynamicStateFeaturesEXT.hpp>
#endif
#ifdef VK_NV_EXTENDED_SPARSE_ADDRESS_SPACE_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceExtendedSparseAddressSpaceFeaturesNV.hpp>
#endif
#ifdef VK_ANDROID_EXTERNAL_FORMAT_RESOLVE_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceExternalFormatResolveFeaturesANDROID.hpp>
#endif
#ifdef VK_NV_EXTERNAL_MEMORY_RDMA_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceExternalMemoryRDMAFeaturesNV.hpp>
#endif
#ifdef VK_QNX_EXTERNAL_MEMORY_SCREEN_BUFFER_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceExternalMemoryScreenBufferFeaturesQNX.hpp>
#endif
#ifdef VK_EXT_DEVICE_FAULT_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceFaultFeaturesEXT.hpp>
#endif
#ifdef VK_EXT_FRAGMENT_DENSITY_MAP_2_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceFragmentDensityMap2FeaturesEXT.hpp>
#endif
#ifdef VK_EXT_FRAGMENT_DENSITY_MAP_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceFragmentDensityMapFeaturesEXT.hpp>
#endif
#ifdef VK_VALVE_FRAGMENT_DENSITY_MAP_LAYERED_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceFragmentDensityMapLayeredFeaturesVALVE.hpp>
#endif
#ifdef VK_EXT_FRAGMENT_DENSITY_MAP_OFFSET_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceFragmentDensityMapOffsetFeaturesEXT.hpp>
#elif defined(VK_QCOM_FRAGMENT_DENSITY_MAP_OFFSET_EXTENSION_NAME)
#include <vulkan2json/PhysicalDeviceFragmentDensityMapOffsetFeaturesQCOM.hpp>
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
#ifdef VK_EXT_GRAPHICS_PIPELINE_LIBRARY_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceGraphicsPipelineLibraryFeaturesEXT.hpp>
#endif
#ifdef VK_VERSION_1_4
#include <vulkan2json/PhysicalDeviceHostImageCopyFeatures.hpp>
#elif defined(VK_EXT_HOST_IMAGE_COPY_EXTENSION_NAME)
#include <vulkan2json/PhysicalDeviceHostImageCopyFeaturesEXT.hpp>
#endif
#ifdef VK_EXT_FRAME_BOUNDARY_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceFrameBoundaryFeaturesEXT.hpp>
#endif
#ifdef VK_VERSION_1_4
#include <vulkan2json/PhysicalDeviceGlobalPriorityQueryFeatures.hpp>
#elif defined(VK_KHR_GLOBAL_PRIORITY_EXTENSION_NAME)
#include <vulkan2json/PhysicalDeviceGlobalPriorityQueryFeaturesKHR.hpp>
#elif defined(VK_EXT_GLOBAL_PRIORITY_QUERY_EXTENSION_NAME)
#include <vulkan2json/PhysicalDeviceGlobalPriorityQueryFeaturesEXT.hpp>
#endif
#ifdef VK_VERSION_1_2
#include <vulkan2json/PhysicalDeviceHostQueryResetFeatures.hpp>
#elif defined(VK_EXT_HOST_QUERY_RESET_EXTENSION_NAME)
#include <vulkan2json/PhysicalDeviceHostQueryResetFeaturesEXT.hpp>
#endif
#ifdef VK_VERSION_1_3
#include <vulkan2json/PhysicalDeviceImageRobustnessFeatures.hpp>
#elif defined(VK_EXT_IMAGE_ROBUSTNESS_EXTENSION_NAME)
#include <vulkan2json/PhysicalDeviceImageRobustnessFeaturesEXT.hpp>
#endif
#ifdef VK_EXT_IMAGE_SLICED_VIEW_OF_3D_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceImageSlicedViewOf3DFeaturesEXT.hpp>
#endif
#ifdef VK_EXT_IMAGE_VIEW_MIN_LOD_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceImageViewMinLodFeaturesEXT.hpp>
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
#ifdef VK_MESA_IMAGE_ALIGNMENT_CONTROL_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceImageAlignmentControlFeaturesMESA.hpp>
#endif
#ifdef VK_QCOM_IMAGE_PROCESSING_2_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceImageProcessing2FeaturesQCOM.hpp>
#endif
#ifdef VK_QCOM_IMAGE_PROCESSING_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceImageProcessingFeaturesQCOM.hpp>
#endif
#ifdef VK_VERSION_1_3
#include <vulkan2json/PhysicalDeviceInlineUniformBlockFeatures.hpp>
#elif defined(VK_EXT_INLINE_UNIFORM_BLOCK_EXTENSION_NAME)
#include <vulkan2json/PhysicalDeviceInlineUniformBlockFeaturesEXT.hpp>
#endif
#ifdef VK_HUAWEI_INVOCATION_MASK_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceInvocationMaskFeaturesHUAWEI.hpp>
#endif
#ifdef VK_EXT_LEGACY_DITHERING_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceLegacyDitheringFeaturesEXT.hpp>
#endif
#ifdef VK_EXT_LEGACY_VERTEX_ATTRIBUTES_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceLegacyVertexAttributesFeaturesEXT.hpp>
#endif
#ifdef VK_VERSION_1_4
#include <vulkan2json/PhysicalDeviceLineRasterizationFeatures.hpp>
#elif defined(VK_KHR_LINE_RASTERIZATION_EXTENSION_NAME)
#include <vulkan2json/PhysicalDeviceLineRasterizationFeaturesKHR.hpp>
#elif defined(VK_EXT_LINE_RASTERIZATION_EXTENSION_NAME)
#include <vulkan2json/PhysicalDeviceLineRasterizationFeaturesEXT.hpp>
#endif
#ifdef VK_NV_LINEAR_COLOR_ATTACHMENT_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceLinearColorAttachmentFeaturesNV.hpp>
#endif
#ifdef VK_VERSION_1_3
#include <vulkan2json/PhysicalDeviceMaintenance4Features.hpp>
#elif defined(VK_KHR_MAINTENANCE_4_EXTENSION_NAME)
#include <vulkan2json/PhysicalDeviceMaintenance4FeaturesKHR.hpp>
#endif
#ifdef VK_VERSION_1_4
#include <vulkan2json/PhysicalDeviceMaintenance5Features.hpp>
#elif defined(VK_KHR_MAINTENANCE_5_EXTENSION_NAME)
#include <vulkan2json/PhysicalDeviceMaintenance5FeaturesKHR.hpp>
#endif
#ifdef VK_VERSION_1_4
#include <vulkan2json/PhysicalDeviceMaintenance6Features.hpp>
#elif defined(VK_KHR_MAINTENANCE_6_EXTENSION_NAME)
#include <vulkan2json/PhysicalDeviceMaintenance6FeaturesKHR.hpp>
#endif
#ifdef VK_KHR_MAINTENANCE_7_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceMaintenance7FeaturesKHR.hpp>
#endif
#ifdef VK_KHR_MAINTENANCE_8_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceMaintenance8FeaturesKHR.hpp>
#endif
#ifdef VK_KHR_MAINTENANCE_9_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceMaintenance9FeaturesKHR.hpp>
#endif
#ifdef VK_EXT_MAP_MEMORY_PLACED_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceMapMemoryPlacedFeaturesEXT.hpp>
#endif
#ifdef VK_NV_MEMORY_DECOMPRESSION_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceMemoryDecompressionFeaturesNV.hpp>
#endif
#ifdef VK_EXT_MEMORY_PRIORITY_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceMemoryPriorityFeaturesEXT.hpp>
#endif
#ifdef VK_EXT_MESH_SHADER_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceMeshShaderFeaturesEXT.hpp>
#endif
#ifdef VK_NV_MESH_SHADER_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceMeshShaderFeaturesNV.hpp>
#endif
#ifdef VK_EXT_MULTI_DRAW_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceMultiDrawFeaturesEXT.hpp>
#endif
#ifdef VK_EXT_MULTISAMPLED_RENDER_TO_SINGLE_SAMPLED_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceMultisampledRenderToSingleSampledFeaturesEXT.hpp>
#endif

#ifdef VK_VERSION_1_1
#include <vulkan2json/PhysicalDeviceMultiviewFeatures.hpp>
#elif defined(VK_KHR_MULTIVIEW_EXTENSION_NAME)
#include <vulkan2json/PhysicalDeviceMultiviewFeaturesKHR.hpp>
#endif
#ifdef VK_QCOM_MULTIVIEW_PER_VIEW_RENDER_AREAS_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceMultiviewPerViewRenderAreasFeaturesQCOM.hpp>
#endif
#ifdef VK_QCOM_MULTIVIEW_PER_VIEW_VIEWPORTS_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceMultiviewPerViewViewportsFeaturesQCOM.hpp>
#endif
#ifdef VK_VALVE_MUTABLE_DESCRIPTOR_TYPE_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceMutableDescriptorTypeFeaturesEXT.hpp>
#elif defined(VK_VALVE_MUTABLE_DESCRIPTOR_TYPE_EXTENSION_NAME)
#include <vulkan2json/PhysicalDeviceMutableDescriptorTypeFeaturesVALVE.hpp>
#endif
#ifdef VK_EXT_NESTED_COMMAND_BUFFER_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceNestedCommandBufferFeaturesEXT.hpp>
#endif
#ifdef VK_EXT_NON_SEAMLESS_CUBE_MAP_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceNonSeamlessCubeMapFeaturesEXT.hpp>
#endif
#ifdef VK_EXT_OPACITY_MICROMAP_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceOpacityMicromapFeaturesEXT.hpp>
#endif
#ifdef VK_NV_OPTICAL_FLOW_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceOpticalFlowFeaturesNV.hpp>
#endif
#ifdef VK_EXT_PAGEABLE_DEVICE_LOCAL_MEMORY_EXTENSION_NAME
#include <vulkan2json/PhysicalDevicePageableDeviceLocalMemoryFeaturesEXT.hpp>
#endif
#ifdef VK_NV_PER_STAGE_DESCRIPTOR_SET_EXTENSION_NAME
#include <vulkan2json/PhysicalDevicePerStageDescriptorSetFeaturesNV.hpp>
#endif
#ifdef VK_KHR_PERFORMANCE_QUERY_EXTENSION_NAME
#include <vulkan2json/PhysicalDevicePerformanceQueryFeaturesKHR.hpp>
#endif
#ifdef VK_KHR_PIPELINE_BINARY_EXTENSION_NAME
#include <vulkan2json/PhysicalDevicePipelineBinaryFeaturesKHR.hpp>
#endif
#ifdef VK_VERSION_1_4
#include <vulkan2json/PhysicalDevicePipelineRobustnessFeatures.hpp>
#elif defined(VK_EXT_PIPELINE_ROBUSTNESS_EXTENSION_NAME)
#include <vulkan2json/PhysicalDevicePipelineRobustnessFeaturesEXT.hpp>
#endif
#ifdef VK_NV_PARTITIONED_ACCELERATION_STRUCTURE_EXTENSION_NAME
#include <vulkan2json/PhysicalDevicePartitionedAccelerationStructureFeaturesNV.hpp>
#endif
#ifdef VK_VERSION_1_3
#include <vulkan2json/PhysicalDevicePipelineCreationCacheControlFeatures.hpp>
#elif defined(VK_EXT_PIPELINE_CREATION_CACHE_CONTROL_EXTENSION_NAME)
#include <vulkan2json/PhysicalDevicePipelineCreationCacheControlFeaturesEXT.hpp>
#endif
#ifdef VK_KHR_PIPELINE_EXECUTABLE_PROPERTIES_EXTENSION_NAME
#include <vulkan2json/PhysicalDevicePipelineExecutablePropertiesFeaturesKHR.hpp>
#endif
#ifdef VK_EXT_PIPELINE_LIBRARY_GROUP_HANDLES_EXTENSION_NAME
#include <vulkan2json/PhysicalDevicePipelineLibraryGroupHandlesFeaturesEXT.hpp>
#endif
#ifdef VK_EXT_PIPELINE_PROPERTIES_EXTENSION_NAME
#include <vulkan2json/PhysicalDevicePipelinePropertiesFeaturesEXT.hpp>
#endif
#ifdef VK_VERSION_1_4
#include <vulkan2json/PhysicalDevicePipelineProtectedAccessFeatures.hpp>
#elif defined(VK_EXT_PIPELINE_PROTECTED_ACCESS_EXTENSION_NAME)
#include <vulkan2json/PhysicalDevicePipelineProtectedAccessFeaturesEXT.hpp>
#endif
#ifdef VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME
#include <vulkan2json/PhysicalDevicePortabilitySubsetFeaturesKHR.hpp>
#endif
#ifdef VK_NV_PRESENT_BARRIER_EXTENSION_NAME
#include <vulkan2json/PhysicalDevicePresentBarrierFeaturesNV.hpp>
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
#ifdef VK_EXT_PRIMITIVES_GENERATED_QUERY_EXTENSION_NAME
#include <vulkan2json/PhysicalDevicePrimitivesGeneratedQueryFeaturesEXT.hpp>
#endif
#ifdef VK_VERSION_1_3
#include <vulkan2json/PhysicalDevicePrivateDataFeatures.hpp>
#elif defined(VK_EXT_PRIVATE_DATA_EXTENSION_NAME)
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
#ifdef VK_EXT_RASTERIZATION_ORDER_ATTACHMENT_ACCESS_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceRasterizationOrderAttachmentAccessFeaturesEXT.hpp>
#endif
#ifdef VK_KHR_RAY_QUERY_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceRayQueryFeaturesKHR.hpp>
#endif
#ifdef VK_NV_RAY_TRACING_INVOCATION_REORDER_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceRayTracingInvocationReorderFeaturesNV.hpp>
#endif
#ifdef VK_KHR_RAY_TRACING_MAINTENANCE_1_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceRayTracingMaintenance1FeaturesKHR.hpp>
#endif
#ifdef VK_NV_RAY_TRACING_MOTION_BLUR_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceRayTracingMotionBlurFeaturesNV.hpp>
#endif
#ifdef VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceRayTracingPipelineFeaturesKHR.hpp>
#endif
#ifdef VK_KHR_RAY_TRACING_POSITION_FETCH_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceRayTracingPositionFetchFeaturesKHR.hpp>
#endif
#ifdef VK_IMG_RELAXED_LINE_RASTERIZATION_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceRelaxedLineRasterizationFeaturesIMG.hpp>
#endif
#ifdef VK_ARM_RENDER_PASS_STRIPED_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceRenderPassStripedFeaturesARM.hpp>
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
#ifdef VK_ARM_SCHEDULING_CONTROLS_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceSchedulingControlsFeaturesARM.hpp>
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
#ifdef VK_ARM_SHADER_CORE_BUILTINS_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceShaderCoreBuiltinsFeaturesARM.hpp>
#endif
#ifdef VK_VERSION_1_3
#include <vulkan2json/PhysicalDeviceShaderDemoteToHelperInvocationFeatures.hpp>
#elif defined(VK_EXT_SHADER_DEMOTE_TO_HELPER_INVOCATION_EXTENSION_NAME)
#include <vulkan2json/PhysicalDeviceShaderDemoteToHelperInvocationFeaturesEXT.hpp>
#endif
#ifdef VK_VERSION_1_1
#include <vulkan2json/PhysicalDeviceShaderDrawParametersFeatures.hpp>
#endif
#ifdef VK_AMD_SHADER_EARLY_AND_LATE_FRAGMENT_TESTS_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceShaderEarlyAndLateFragmentTestsFeaturesAMD.hpp>
#endif
#ifdef VK_AMDX_SHADER_ENQUEUE_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceShaderEnqueueFeaturesAMDX.hpp>
#endif
#ifdef VK_VERSION_1_4
#include <vulkan2json/PhysicalDeviceShaderExpectAssumeFeatures.hpp>
#elif defined(VK_KHR_SHADER_EXPECT_ASSUME_EXTENSION_NAME)
#include <vulkan2json/PhysicalDeviceShaderExpectAssumeFeaturesKHR.hpp>
#endif
#ifdef VK_VERSION_1_2
#include <vulkan2json/PhysicalDeviceShaderFloat16Int8Features.hpp>
#elif defined(VK_KHR_SHADER_FLOAT16_INT8_EXTENSION_NAME)
#include <vulkan2json/PhysicalDeviceShaderFloat16Int8FeaturesKHR.hpp>
#endif
#ifdef VK_VERSION_1_4
#include <vulkan2json/PhysicalDeviceShaderFloatControls2Features.hpp>
#elif defined(VK_KHR_SHADER_FLOAT_CONTROLS_2_EXTENSION_NAME)
#include <vulkan2json/PhysicalDeviceShaderFloatControls2FeaturesKHR.hpp>
#endif
#ifdef VK_EXT_SHADER_IMAGE_ATOMIC_INT64_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceShaderImageAtomicInt64FeaturesEXT.hpp>
#endif
#ifdef VK_NV_SHADER_IMAGE_FOOTPRINT_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceShaderImageFootprintFeaturesNV.hpp>
#endif
#ifdef VK_VERSION_1_3
#include <vulkan2json/PhysicalDeviceShaderIntegerDotProductFeatures.hpp>
#elif defined(VK_KHR_SHADER_INTEGER_DOT_PRODUCT_EXTENSION_NAME)
#include <vulkan2json/PhysicalDeviceShaderIntegerDotProductFeaturesKHR.hpp>
#endif

#ifdef VK_INTEL_SHADER_INTEGER_FUNCTIONS_2_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceShaderIntegerFunctions2FeaturesINTEL.hpp>
#endif
#ifdef VK_KHR_SHADER_MAXIMAL_RECONVERGENCE_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceShaderMaximalReconvergenceFeaturesKHR.hpp>
#endif
#ifdef VK_EXT_SHADER_MODULE_IDENTIFIER_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceShaderModuleIdentifierFeaturesEXT.hpp>
#endif
#ifdef VK_EXT_SHADER_OBJECT_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceShaderObjectFeaturesEXT.hpp>
#endif
#ifdef VK_KHR_SHADER_QUAD_CONTROL_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceShaderQuadControlFeaturesKHR.hpp>
#endif
#ifdef VK_NV_SHADER_SM_BUILTINS_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceShaderSMBuiltinsFeaturesNV.hpp>
#endif
#ifdef VK_VERSION_1_2
#include <vulkan2json/PhysicalDeviceShaderSubgroupExtendedTypesFeatures.hpp>
#elif defined(VK_KHR_SHADER_SUBGROUP_EXTENDED_TYPES_EXTENSION_NAME)
#include <vulkan2json/PhysicalDeviceShaderSubgroupExtendedTypesFeaturesKHR.hpp>
#endif
#ifdef VK_VERSION_1_4
#include <vulkan2json/PhysicalDeviceShaderSubgroupRotateFeatures.hpp>
#elif defined(VK_KHR_SHADER_SUBGROUP_ROTATE_EXTENSION_NAME)
#include <vulkan2json/PhysicalDeviceShaderSubgroupRotateFeaturesKHR.hpp>
#endif
#ifdef VK_KHR_SHADER_SUBGROUP_UNIFORM_CONTROL_FLOW_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceShaderSubgroupUniformControlFlowFeaturesKHR.hpp>
#endif
#ifdef VK_VERSION_1_3
#include <vulkan2json/PhysicalDeviceShaderTerminateInvocationFeatures.hpp>
#elif defined(VK_KHR_SHADER_TERMINATE_INVOCATION_EXTENSION_NAME)
#include <vulkan2json/PhysicalDeviceShaderTerminateInvocationFeaturesKHR.hpp>
#endif
#ifdef VK_EXT_SHADER_TILE_IMAGE_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceShaderTileImageFeaturesEXT.hpp>
#endif
#ifdef VK_NV_SHADING_RATE_IMAGE_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceShadingRateImageFeaturesNV.hpp>
#endif
#ifdef VK_VERSION_1_3
#include <vulkan2json/PhysicalDeviceSubgroupSizeControlFeatures.hpp>
#elif defined(VK_EXT_SUBGROUP_SIZE_CONTROL_EXTENSION_NAME)
#include <vulkan2json/PhysicalDeviceSubgroupSizeControlFeaturesEXT.hpp>
#endif
#ifdef VK_EXT_SUBPASS_MERGE_FEEDBACK_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceSubpassMergeFeedbackFeaturesEXT.hpp>
#endif
#ifdef VK_HUAWEI_SUBPASS_SHADING_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceSubpassShadingFeaturesHUAWEI.hpp>
#endif
#ifdef VK_EXT_SWAPCHAIN_MAINTENANCE_1_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceSwapchainMaintenance1FeaturesEXT.hpp>
#endif
#ifdef VK_VERSION_1_3
#include <vulkan2json/PhysicalDeviceSynchronization2Features.hpp>
#elif defined(VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME)
#include <vulkan2json/PhysicalDeviceSynchronization2FeaturesKHR.hpp>
#endif
#ifdef VK_EXT_TEXEL_BUFFER_ALIGNMENT_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceTexelBufferAlignmentFeaturesEXT.hpp>
#endif
#ifdef VK_QCOM_TILE_MEMORY_HEAP_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceTileMemoryHeapFeaturesQCOM.hpp>
#endif
#ifdef VK_QCOM_TILE_SHADING_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceTileShadingFeaturesQCOM.hpp>
#endif
#ifdef VK_VERSION_1_3
#include <vulkan2json/PhysicalDeviceTextureCompressionASTCHDRFeatures.hpp>
#elif defined(VK_EXT_TEXTURE_COMPRESSION_ASTC_HDR_EXTENSION_NAME)
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
#ifdef VK_VERSION_1_4
#include <vulkan2json/PhysicalDeviceVertexAttributeDivisorFeatures.hpp>
#elif defined(VK_KHR_VERTEX_ATTRIBUTE_DIVISOR_EXTENSION_NAME)
#include <vulkan2json/PhysicalDeviceVertexAttributeDivisorFeaturesKHR.hpp>
#elif defined(VK_EXT_VERTEX_ATTRIBUTE_DIVISOR_EXTENSION_NAME)
#include <vulkan2json/PhysicalDeviceVertexAttributeDivisorFeaturesEXT.hpp>
#endif
#ifdef VK_EXT_VERTEX_INPUT_DYNAMIC_STATE_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceVertexInputDynamicStateFeaturesEXT.hpp>
#endif
#ifdef VK_KHR_VIDEO_MAINTENANCE_1_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceVideoMaintenance1FeaturesKHR.hpp>
#endif
#ifdef VK_VERSION_1_2
#include <vulkan2json/PhysicalDeviceVulkanMemoryModelFeatures.hpp>
#elif VK_KHR_VULKAN_MEMORY_MODEL_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceVulkanMemoryModelFeaturesKHR.hpp>
#endif
#ifdef VK_KHR_WORKGROUP_MEMORY_EXPLICIT_LAYOUT_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceWorkgroupMemoryExplicitLayoutFeaturesKHR.hpp>
#endif
#ifdef VK_QCOM_YCBCR_DEGAMMA_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceYcbcrDegammaFeaturesQCOM.hpp>
#endif
#ifdef VK_EXT_YCBCR_2PLANE_444_FORMATS_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceYcbcr2Plane444FormatsFeaturesEXT.hpp>
#endif
#ifdef VK_EXT_YCBCR_IMAGE_ARRAYS_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceYcbcrImageArraysFeaturesEXT.hpp>
#endif
#ifdef VK_VERSION_1_3
#include <vulkan2json/PhysicalDeviceZeroInitializeWorkgroupMemoryFeatures.hpp>
#elif defined(VK_KHR_ZERO_INITIALIZE_WORKGROUP_MEMORY_EXTENSION_NAME)
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
#ifdef VK_EXT_DEVICE_ADDRESS_BINDING_REPORT_EXTENSION_NAME
      LIBGCT_EXTENSION_TO_JSON( address_binding_report )
#endif
#ifdef VK_SEC_AMIGO_PROFILING_EXTENSION_NAME
      LIBGCT_EXTENSION_TO_JSON( amigo_profiling )
#endif
#ifdef VK_EXT_ATTACHMENT_FEEDBACK_LOOP_DYNAMIC_STATE_EXTENSION_NAME
      LIBGCT_EXTENSION_TO_JSON( attachment_feedback_loop_dynamic_state )
#endif
#ifdef VK_EXT_ATTACHMENT_FEEDBACK_LOOP_LAYOUT_EXTENSION_NAME
      LIBGCT_EXTENSION_TO_JSON( attachment_feedback_loop_layout )
#endif
#ifdef VK_EXT_DEVICE_ADDRESS_BINDING_REPORT_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( address_binding_report )
#endif
#ifdef VK_SEC_AMIGO_PROFILING_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( amigo_profiling )
#endif
#ifdef VK_EXT_ATTACHMENT_FEEDBACK_LOOP_DYNAMIC_STATE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( attachment_feedback_loop_dynamic_state )
#endif
#ifdef VK_EXT_ATTACHMENT_FEEDBACK_LOOP_LAYOUT_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( attachment_feedback_loop_layout )
#endif
#ifdef VK_EXT_BLEND_OPERATION_ADVANCED_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( blend_operation_advanced )
#endif
#ifdef VK_NV_CLUSTER_ACCELERATION_STRUCTURE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( cluster_acceleration_structure )
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
#ifdef VK_HUAWEI_CLUSTER_CULLING_SHADER_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( cluster_culling_shader )
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
#ifdef VK_NV_COOPERATIVE_MATRIX_2_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( cooperative_matrix2 )
#endif
#ifdef VK_KHR_COOPERATIVE_MATRIX_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( cooperative_matrix )
#endif
#ifdef VK_NV_COOPERATIVE_VECTOR_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( cooperative_vector )
#endif
#ifdef VK_NV_COOPERATIVE_MATRIX_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( cooperative_matrix_nv )
#endif
#ifdef VK_NV_COPY_MEMORY_INDIRECT_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( copy_memory_indirect )
#endif
#ifdef VK_NV_CORNER_SAMPLED_IMAGE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( corner_sampled_image )
#endif
#ifdef VK_NV_COVERAGE_REDUCTION_MODE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( coverage_reduction_mode )
#endif
#ifdef VK_QCOM_FILTER_CUBIC_CLAMP_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( cubic_clamp )
#endif
#ifdef VK_QCOM_FILTER_CUBIC_WEIGHTS_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( cubic_weight )
#endif
#ifdef VK_NV_CUDA_KERNEL_LAUNCH_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( cuda_kernel_launch )
#endif
#ifdef VK_EXT_CUSTOM_BORDER_COLOR_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( custom_border_color )
#endif
#ifdef VK_NV_DEDICATED_ALLOCATION_IMAGE_ALIASING_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( dedicated_allocation_image_aliasing )
#endif
#ifdef VK_EXT_DEPTH_BIAS_CONTROL_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( depth_bias_control )
#endif
#ifdef VK_EXT_DEPTH_CLAMP_ZERO_ONE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( depth_clamp_zero_one )
#endif
#ifdef VK_EXT_DEPTH_CLIP_CONTROL_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( depth_clip_control )
#endif
#ifdef VK_EXT_DEPTH_CLIP_ENABLE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( depth_clip_enable )
#endif
#ifdef VK_EXT_DESCRIPTOR_BUFFER_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( descriptor_buffer )
#endif
#if defined(VK_EXT_DESCRIPTOR_BUFFER_EXTENSION_NAME) && defined(VK_ARM_TENSORS_EXTENSION_NAME)
   LIBGCT_EXTENSION_TO_JSON( descriptor_buffer_tensor )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( descriptor_indexing )
#endif
#ifdef VK_NV_DESCRIPTOR_POOL_OVERALLOCATION_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( descriptor_pool_overallocation )
#endif
#ifdef VK_NV_DEVICE_GENERATED_COMMANDS_COMPUTE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( device_generated_commands_compute )
#endif
#ifdef VK_EXT_DEVICE_GENERATED_COMMANDS_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( device_generated_commands )
#endif
#ifdef VK_NV_DEVICE_GENERATED_COMMANDS_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( device_generated_commands_nv )
#endif
#ifdef VK_EXT_DEVICE_MEMORY_REPORT_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( device_memory_report )
#endif
#ifdef VK_NV_DEVICE_DIAGNOSTICS_CONFIG_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( diagnostic_config )
#endif
#ifdef VK_NV_DISPLACEMENT_MICROMAP_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( displacement_micromap )
#endif
#if defined(VK_VERSION_1_3) || defined(VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( dynamic_rendering )
#endif
#if defined(VK_VERSION_1_4) || defined(VK_KHR_DYNAMIC_RENDERING_LOCAL_READ_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( dynamic_rendering_local_read )
#endif
#ifdef VK_EXT_DYNAMIC_RENDERING_UNUSED_ATTACHMENTS_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( dynamic_rendering_unused_attachments )
#endif
#ifdef VK_NV_SCISSOR_EXCLUSIVE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( exclusive_scissor )
#endif
#ifdef VK_EXT_EXTENDED_DYNAMIC_STATE_2_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( extended_dynamic_state2 )
#endif
#ifdef VK_EXT_EXTENDED_DYNAMIC_STATE_3_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( extended_dynamic_state3 )
#endif
#ifdef VK_EXT_EXTENDED_DYNAMIC_STATE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( extended_dynamic_state )
#endif
#ifdef VK_NV_EXTENDED_SPARSE_ADDRESS_SPACE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( extended_sparse_address_space )
#endif
#ifdef VK_ANDROID_EXTERNAL_FORMAT_RESOLVE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( external_format_resolve )
#endif
#ifdef VK_NV_EXTERNAL_MEMORY_RDMA_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( external_memory_rdma )
#endif
#ifdef VK_QNX_EXTERNAL_MEMORY_SCREEN_BUFFER_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( external_memory_screen_buffer )
#endif
#ifdef VK_EXT_DEVICE_FAULT_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( fault )
#endif
#ifdef VK_EXT_FRAGMENT_DENSITY_MAP_2_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( fragment_density_map2 )
#endif
#ifdef VK_EXT_FRAGMENT_DENSITY_MAP_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( fragment_density_map )
#endif
#ifdef VK_VALVE_FRAGMENT_DENSITY_MAP_LAYERED_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( fragment_density_map_layered )
#endif
#if defined(VK_EXT_FRAGMENT_DENSITY_MAP_OFFSET_EXTENSION_NAME) || defined(VK_QCOM_FRAGMENT_DENSITY_MAP_OFFSET_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( fragment_density_map_offset )
#endif
#ifdef VK_QCOM_FRAGMENT_DENSITY_MAP_OFFSET_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( fragment_density_map_offset )
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
#ifdef VK_EXT_GRAPHICS_PIPELINE_LIBRARY_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( graphics_pipeline_library )
#endif
#ifdef VK_VERSION_1_4
    LIBGCT_EXTENSION_TO_JSON( host_image_copy )
#elif defined(VK_EXT_HOST_IMAGE_COPY_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( host_image_copy )
#endif
#ifdef VK_EXT_FRAME_BOUNDARY_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( frame_boundary )
#endif
#if defined(VK_VERSION_1_4) || defined(VK_KHR_GLOBAL_PRIORITY_EXTENSION_NAME) || defined(VK_EXT_GLOBAL_PRIORITY_QUERY_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( global_priority_query )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_EXT_HOST_QUERY_RESET_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( host_query_reset )
#endif
#if defined(VK_VERSION_1_3) || defined(VK_EXT_IMAGE_ROBUSTNESS_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( image_robustness )
#endif
#ifdef VK_EXT_IMAGE_SLICED_VIEW_OF_3D_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( image_sliced_view_of_3d )
#endif
#ifdef VK_EXT_IMAGE_VIEW_MIN_LOD_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( image_view_min_lod )
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
#ifdef VK_MESA_IMAGE_ALIGNMENT_CONTROL_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( image_alignment_control )
#endif
#ifdef VK_QCOM_IMAGE_PROCESSING_2_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( image_processing2 )
#endif
#ifdef VK_QCOM_IMAGE_PROCESSING_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( image_processing )
#endif
#if defined(VK_VERSION_1_3) || defined(VK_EXT_INLINE_UNIFORM_BLOCK_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( inline_uniform_block )
#endif
#ifdef VK_HUAWEI_INVOCATION_MASK_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( invocation_mask )
#endif
#ifdef VK_EXT_LEGACY_DITHERING_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( legacy_dithering )
#endif
#ifdef VK_EXT_LEGACY_VERTEX_ATTRIBUTES_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( legacy_vertex_attribute )
#endif
#if defined(VK_VERSION_1_4) || defined(VK_EXT_LINE_RASTERIZATION_EXTENSION_NAME) || defined(VK_EXT_LINE_RASTERIZATION_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( line_rasterization )
#endif
#ifdef VK_NV_LINEAR_COLOR_ATTACHMENT_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( linear_color_attachment )
#endif
#if defined(VK_VERSION_1_3) || defined(VK_KHR_MAINTENANCE_4_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( maintenance4 )
#endif
#if defined(VK_VERSION_1_4) || defined(VK_KHR_MAINTENANCE_5_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( maintenance5 )
#endif
#if defined(VK_VERSION_1_4) || defined(VK_KHR_MAINTENANCE_6_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( maintenance6 )
#endif
#ifdef VK_KHR_MAINTENANCE_7_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( maintenance7 )
#endif
#ifdef VK_KHR_MAINTENANCE_8_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( maintenance8 )
#endif
#ifdef VK_KHR_MAINTENANCE_9_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( maintenance9 )
#endif
#ifdef VK_EXT_MAP_MEMORY_PLACED_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( map_memory_placed )
#endif
#ifdef VK_NV_MEMORY_DECOMPRESSION_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( memory_decompression )
#endif
#ifdef VK_EXT_MEMORY_PRIORITY_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( memory_priority )
#endif
#ifdef VK_EXT_MESH_SHADER_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( mesh_shader )
#endif
#ifdef VK_NV_MESH_SHADER_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( mesh_shader_nv )
#endif
#ifdef VK_EXT_MULTI_DRAW_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( multi_draw )
#endif
#ifdef VK_EXT_MULTISAMPLED_RENDER_TO_SINGLE_SAMPLED_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( multisampled_render_to_single_sampled )
#endif
#if defined(VK_VERSION_1_1) || defined(VK_KHR_MULTIVIEW_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( multiview )
#endif
#ifdef VK_QCOM_MULTIVIEW_PER_VIEW_RENDER_AREAS_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( multiview_per_view_render_areas )
#endif
#ifdef VK_QCOM_MULTIVIEW_PER_VIEW_VIEWPORTS_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( multiview_per_view_viewports )
#endif
#if defined(VK_EXT_MUTABLE_DESCRIPTOR_TYPE_EXTENSION_NAME) || defined(VK_VALVE_MUTABLE_DESCRIPTOR_TYPE_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( mutable_descriptor_type )
#endif
#ifdef VK_EXT_NESTED_COMMAND_BUFFER_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( nested_command_buffer )
#endif
#ifdef VK_EXT_NON_SEAMLESS_CUBE_MAP_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( non_seamless_cube_map )
#endif
#ifdef VK_EXT_OPACITY_MICROMAP_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( opacity_micromap )
#endif
#ifdef VK_NV_OPTICAL_FLOW_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( optical_flow )
#endif
#ifdef VK_EXT_PAGEABLE_DEVICE_LOCAL_MEMORY_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( pageable_device_local_memory )
#endif
#ifdef VK_NV_PER_STAGE_DESCRIPTOR_SET_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( per_stage_descriptor_set )
#endif
#ifdef VK_KHR_PERFORMANCE_QUERY_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( performance_query )
#endif
#ifdef VK_KHR_PIPELINE_BINARY_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( pipeline_binary )
#endif
#if defined(VK_VERSION_1_4) || defined(VK_EXT_PIPELINE_ROBUSTNESS_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( pipeline_robustness )
#endif
#ifdef VK_NV_PARTITIONED_ACCELERATION_STRUCTURE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( partitioned_acceleration_structure )
#endif
#if defined(VK_VERSION_1_3) || defined(VK_EXT_PIPELINE_CREATION_CACHE_CONTROL_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( pipeline_creation_cache_control )
#endif
#ifdef VK_KHR_PIPELINE_EXECUTABLE_PROPERTIES_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( pipeline_executable_properties )
#endif
#ifdef VK_EXT_PIPELINE_LIBRARY_GROUP_HANDLES_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( pipeline_library_group_handles )
#endif
#ifdef VK_EXT_PIPELINE_PROPERTIES_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( pipeline_properties )
#endif
#if defined(VK_VERSION_1_4) || defined(VK_EXT_PIPELINE_PROTECTED_ACCESS_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( pipeline_protected_access )
#endif
#ifdef VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( portability_subset )
#endif
#ifdef VK_NV_PRESENT_BARRIER_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( present_barrier )
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
#ifdef VK_EXT_PRIMITIVES_GENERATED_QUERY_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( primitives_generated_query )
#endif
#if defined(VK_VERSION_1_3) || defined(VK_EXT_PRIVATE_DATA_EXTENSION_NAME)
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
#ifdef VK_NV_RAY_TRACING_INVOCATION_REORDER_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( ray_tracing_invocation_reorder )
#endif
#ifdef VK_KHR_RAY_TRACING_MAINTENANCE_1_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( ray_tracing_maintenance1 )
#endif
#ifdef VK_EXT_RASTERIZATION_ORDER_ATTACHMENT_ACCESS_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( rasterization_order_attachment_access )
#endif
#ifdef VK_NV_RAY_TRACING_MOTION_BLUR_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( ray_tracing_motion_blur )
#endif
#ifdef VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( ray_tracing_pipeline )
#endif
#ifdef VK_KHR_RAY_TRACING_POSITION_FETCH_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( ray_tracing_position_fetch )
#endif
#ifdef VK_IMG_RELAXED_LINE_RASTERIZATION_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( relaxed_line_rasterization )
#endif
#ifdef VK_ARM_RENDER_PASS_STRIPED_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( render_pass_striped )
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
#ifdef VK_ARM_SCHEDULING_CONTROLS_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( scheduling_controls )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_SEPARATE_DEPTH_STENCIL_LAYOUTS_EXTENSION_NAME)
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
#ifdef VK_ARM_SHADER_CORE_BUILTINS_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( shader_core_builtins )
#endif
#if defined(VK_VERSION_1_3) || defined(VK_EXT_SHADER_DEMOTE_TO_HELPER_INVOCATION_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( shader_demote_to_helper_invocation )
#endif
#ifdef VK_VERSION_1_1
    LIBGCT_EXTENSION_TO_JSON( shader_draw_parameters )
#endif
#ifdef VK_AMD_SHADER_EARLY_AND_LATE_FRAGMENT_TESTS_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( shader_early_and_late_fragment_tests )
#endif
#ifdef VK_AMDX_SHADER_ENQUEUE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( shader_enqueue )
#endif
#if defined(VK_VERSION_1_4) || defined(VK_KHR_SHADER_EXPECT_ASSUME_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( shader_expect_assume )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_SHADER_FLOAT16_INT8_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( shader_float16_int8 )
#endif
#if defined(VK_VERSION_1_4) || defined(VK_KHR_SHADER_FLOAT_CONTROLS_2_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( shader_float_controls2 )
#endif
#ifdef VK_EXT_SHADER_IMAGE_ATOMIC_INT64_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( shader_image_atomic_int64 )
#endif
#ifdef VK_NV_SHADER_IMAGE_FOOTPRINT_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( shader_image_footprint )
#endif
#if defined(VK_VERSION_1_3) || defined(VK_KHR_SHADER_INTEGER_DOT_PRODUCT_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( shader_integer_dot_product )
#endif
#ifdef VK_INTEL_SHADER_INTEGER_FUNCTIONS_2_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( shader_integer_functions2 )
#endif
#ifdef VK_KHR_SHADER_MAXIMAL_RECONVERGENCE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( shader_maximal_reconvergence )
#endif
#ifdef VK_EXT_SHADER_MODULE_IDENTIFIER_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( shader_module_identifier )
#endif
#ifdef VK_EXT_SHADER_OBJECT_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( shader_object )
#endif
#ifdef VK_KHR_SHADER_QUAD_CONTROL_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( shader_quad_control )
#endif
#ifdef VK_NV_SHADER_SM_BUILTINS_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( shader_smbuiltins )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_SHADER_SUBGROUP_EXTENDED_TYPES_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( shader_subgroup_extended_types )
#endif
#if defined(VK_VERSION_1_4) || defined(VK_KHR_SHADER_SUBGROUP_ROTATE_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( shader_subgroup_rotate )
#endif
#ifdef VK_KHR_SHADER_SUBGROUP_UNIFORM_CONTROL_FLOW_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( shader_subgroup_uniform_control_flow )
#endif
#if defined(VK_VERSION_1_3) || defined(VK_KHR_SHADER_TERMINATE_INVOCATION_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( shader_terminate_invocation )
#endif
#ifdef VK_EXT_SHADER_TILE_IMAGE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( shader_tile_image )
#endif
#ifdef VK_NV_SHADING_RATE_IMAGE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( shading_rate_image )
#endif
#if defined(VK_VERSION_1_3) || defined(VK_EXT_SUBGROUP_SIZE_CONTROL_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( subgroup_size_control )
#endif
#ifdef VK_EXT_SUBPASS_MERGE_FEEDBACK_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( subpass_merge_feedback )
#endif
#ifdef VK_HUAWEI_SUBPASS_SHADING_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( subpass_shading )
#endif
#ifdef VK_EXT_SWAPCHAIN_MAINTENANCE_1_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( swapchain_maintenance1 )
#endif
#if defined(VK_VERSION_1_3) || defined(VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( synchronization2 )
#endif
#ifdef VK_EXT_TEXEL_BUFFER_ALIGNMENT_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( texel_buffer_alignment )
#endif
#ifdef VK_QCOM_TILE_MEMORY_HEAP_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( tile_memory_heap )
#endif
#ifdef VK_QCOM_TILE_SHADING_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( tile_shading )
#endif
#if defined(VK_VERSION_1_3) || defined(VK_EXT_TEXTURE_COMPRESSION_ASTC_HDR_EXTENSION_NAME)
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
#if defined(VK_VERSION_1_4) || defined(VK_KHR_VERTEX_ATTRIBUTE_DIISOR_EXTENSION_NAME) || defined(VK_EXT_VERTEX_ATTRIBUTE_DIVISOR_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( vertex_attribute_divisor )
#endif
#ifdef VK_EXT_VERTEX_INPUT_DYNAMIC_STATE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( vertex_input_dynamic_state )
#endif
#ifdef VK_KHR_VIDEO_MAINTENANCE_1_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( video_maintenance1 )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_VULKAN_MEMORY_MODEL_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( vulkan_memory_model )
#endif
#ifdef VK_KHR_WORKGROUP_MEMORY_EXPLICIT_LAYOUT_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( workgroup_memory_explicit_layout )
#endif
#ifdef VK_QCOM_YCBCR_DEGAMMA_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( ycbcr_degamma )
#endif
#ifdef VK_EXT_YCBCR_2PLANE_444_FORMATS_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( ycbcr_2plane_444_formats )
#endif
#ifdef VK_EXT_YCBCR_IMAGE_ARRAYS_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( ycbcr_image_array )
#endif
#if defined(VK_VERSION_1_3) || defined(VK_KHR_ZERO_INITIALIZE_WORKGROUP_MEMORY_EXTENSION_NAME)
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
#ifdef VK_EXT_DEVICE_ADDRESS_BINDING_REPORT_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( address_binding_report, "VK_EXT_device_address_binding_report" )
#endif
#ifdef VK_SEC_AMIGO_PROFILING_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( amigo_profiling, "VK_SEC_amigo_profiling" )
#endif
#ifdef VK_EXT_ATTACHMENT_FEEDBACK_LOOP_DYNAMIC_STATE_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( attachment_feedback_loop_dynamic_state , "VK_EXT_attachment_feedback_loop_dynamic_state" )
#endif
#ifdef VK_EXT_ATTACHMENT_FEEDBACK_LOOP_LAYOUT_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( attachment_feedback_loop_layout, "VK_EXT_attachment_feedback_loop_layout" )
#endif
#ifdef VK_EXT_BLEND_OPERATION_ADVANCED_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( blend_operation_advanced, "VK_EXT_blend_operation_advanced" )
#endif
#ifdef VK_NV_CLUSTER_ACCELERATION_STRUCTURE_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( cluster_acceleration_structure, VK_NV_CLUSTER_ACCELERATION_STRUCTURE_EXTENSION_NAME )
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
#ifdef VK_HUAWEI_CLUSTER_CULLING_SHADER_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( cluster_culling_shader, "VK_HUAWEI_cluster_culling_shader" )
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
#ifdef VK_NV_COOPERATIVE_MATRIX_2_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( cooperative_matrix2, VK_NV_COOPERATIVE_MATRIX_2_EXTENSION_NAME )
#endif
#ifdef VK_KHR_COOPERATIVE_MATRIX_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( cooperative_matrix, "VK_KHR_cooperative_matrix" )
#endif
#ifdef VK_NV_COOPERATIVE_MATRIX_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( cooperative_matrix_nv, "VK_NV_cooperative_matrix" )
#endif
#ifdef VK_NV_COOPERATIVE_VECTOR_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( cooperative_vector, VK_NV_COOPERATIVE_VECTOR_EXTENSION_NAME )
#endif
#ifdef VK_NV_COPY_MEMORY_INDIRECT_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( copy_memory_indirect, "VK_NV_copy_memory_indrect" )
#endif
#ifdef VK_NV_CORNER_SAMPLED_IMAGE_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( corner_sampled_image, "VK_NV_corner_sampled_image" )
#endif
#ifdef VK_NV_COVERAGE_REDUCTION_MODE_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( coverage_reduction_mode, "VK_NV_coverage_reduction_mode" )
#endif
#ifdef VK_QCOM_FILTER_CUBIC_CLAMP_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( cubic_clamp, "VK_QCOM_filter_cubic_clamp" )
#endif
#ifdef VK_QCOM_FILTER_CUBIC_WEIGHTS_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( cubic_weight, "VK_QCOM_filter_cubic_weights" )
#endif
#ifdef VK_NV_CUDA_KERNEL_LAUNCH_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( cuda_kernel_launch, "VK_NV_cuda_kernel_launch" )
#endif
#ifdef VK_EXT_CUSTOM_BORDER_COLOR_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( custom_border_color, "VK_EXT_custom_border_color" )
#endif
#ifdef VK_NV_DEDICATED_ALLOCATION_IMAGE_ALIASING_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( dedicated_allocation_image_aliasing, "VK_NV_dedicated_allocation_image_aliasing" )
#endif
#ifdef VK_EXT_DEPTH_BIAS_CONTROL_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( depth_bias_control, "Vk_EXT_depth_bias_control" )
#endif
#ifdef VK_EXT_DEPTH_CLAMP_ZERO_ONE_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( depth_clamp_zero_one, "VK_EXT_depth_clamp_zero_one" )
#endif
#ifdef VK_EXT_DEPTH_CLIP_CONTROL_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( depth_clip_control, "VK_EXT_depth_clip_control" )
#endif
#ifdef VK_EXT_DEPTH_CLIP_ENABLE_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( depth_clip_enable, "VK_EXT_depth_clip_enable" )
#endif
#ifdef VK_EXT_DESCRIPTOR_BUFFER_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( descriptor_buffer, "VK_EXT_descriptor_buffer" )
#endif
#if defined(VK_EXT_DESCRIPTOR_BUFFER_EXTENSION_NAME) && defined(VK_ARM_TENSORS_EXTENSION_NAME)
      LIBGCT_EXTENSION_CREATE_IF_EXT( descriptor_buffer_tensor, VK_ARM_TENSORS_EXTENSION_NAME )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME)
      LIBGCT_EXTENSION_CREATE_IF_VERSION_OR_EXT( descriptor_indexing, 1, 2, 0, "VK_EXT_descriptor_indexing" )
#endif
#ifdef VK_NV_DESCRIPTOR_POOL_OVERALLOCATION_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( descriptor_pool_overallocation, "VK_NV_descriptor_pool_overallocation" )
#endif
#ifdef VK_NV_DEVICE_GENERATED_COMMANDS_COMPUTE_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( device_generated_commands_compute, "VK_NV_device_generated_commands_compute" )
#endif
#ifdef VK_NV_DEVICE_GENERATED_COMMANDS_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( device_generated_commands_nv, "VK_NV_device_generated_commands" )
#endif
#ifdef VK_EXT_DEVICE_GENERATED_COMMANDS_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( device_generated_commands, VK_EXT_DEVICE_GENERATED_COMMANDS_EXTENSION_NAME )
#endif
#ifdef VK_EXT_DEVICE_MEMORY_REPORT_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( device_memory_report, "VK_EXT_device_memory_report" )
#endif
#ifdef VK_NV_DEVICE_DIAGNOSTICS_CONFIG_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( diagnostic_config, "VK_NV_device_diagnostics_config" )
#endif
#ifdef VK_NV_DISPLACEMENT_MICROMAP_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( displacement_micromap, "VK_NV_displacement_micromap" )
#endif
#if defined(VK_VERSION_1_3) || defined(VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME)
      LIBGCT_EXTENSION_CREATE_IF_VERSION_OR_EXT( dynamic_rendering, 1, 3, 0,  "VK_KHR_dynamic_rendering" )
#endif
#if defined(VK_VERSION_1_4) || defined(VK_KHR_DYNAMIC_RENDERING_LOCAL_READ_EXTENSION_NAME)
      LIBGCT_EXTENSION_CREATE_IF_VERSION_OR_EXT( dynamic_rendering_local_read, 1, 4, 0, "VK_KHR_dynamic_rendering_local_read" )
#endif
#ifdef VK_EXT_DYNAMIC_RENDERING_UNUSED_ATTACHMENTS_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( dynamic_rendering_unused_attachments, "VK_EXT_dynamic_rendering_unused_attachments" )
#endif
#ifdef VK_NV_SCISSOR_EXCLUSIVE_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( exclusive_scissor, "VK_NV_scissor_exclusive" )
#endif
#ifdef VK_EXT_EXTENDED_DYNAMIC_STATE_2_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( extended_dynamic_state2, "VK_EXT_extended_dynamic_state2" )
#endif
#ifdef VK_EXT_EXTENDED_DYNAMIC_STATE_3_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( extended_dynamic_state3, "VK_EXT_extended_dynamic_state3" )
#endif
#ifdef VK_EXT_EXTENDED_DYNAMIC_STATE_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( extended_dynamic_state, "VK_EXT_extended_dynamic_state" )
#endif
#ifdef VK_NV_EXTENDED_SPARSE_ADDRESS_SPACE_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( extended_sparse_address_space, "VK_NV_extended_sparse_address_space" )
#endif
#ifdef VK_ANDROID_EXTERNAL_FORMAT_RESOLVE_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( external_format_resolve, "VK_ANDROID_external_format_resolve" )
#endif
#ifdef VK_NV_EXTERNAL_MEMORY_RDMA_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( external_memory_rdma, "VK_NV_external_memory_rdma" )
#endif
#ifdef VK_QNX_EXTERNAL_MEMORY_SCREEN_BUFFER_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( external_memory_screen_buffer, "VK_QNX_external_memory_screen_buffer" )
#endif
#ifdef VK_EXT_DEVICE_FAULT_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( fault, "VK_EXT_device_fault" )
#endif
#ifdef VK_EXT_FRAGMENT_DENSITY_MAP_2_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( fragment_density_map2, "VK_EXT_fragment_density_map2" )
#endif
#ifdef VK_EXT_FRAGMENT_DENSITY_MAP_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( fragment_density_map, "VK_EXT_fragment_density_map" )
#endif
#ifdef VK_QCOM_FRAGMENT_DENSITY_MAP_OFFSET_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( fragment_density_map_offset, VK_QCOM_FRAGMENT_DENSITY_MAP_OFFSET_EXTENSION_NAME )
#endif
#ifdef VK_VALVE_FRAGMENT_DENSITY_MAP_LAYERED_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( fragment_density_map_layered, VK_VALVE_FRAGMENT_DENSITY_MAP_LAYERED_EXTENSION_NAME )
#endif
#ifdef VK_EXT_FRAGMENT_DENSITY_MAP_OFFSET_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( fragment_density_map_offset, VK_EXT_FRAGMENT_DENSITY_MAP_OFFSET_EXTENSION_NAME )
#elif defined(VK_QCOM_FRAGMENT_DENSITY_MAP_OFFSET_EXTENSION_NAME)
      LIBGCT_EXTENSION_CREATE_IF_EXT( fragment_density_map_offset, VK_QCOM_FRAGMENT_DENSITY_MAP_OFFSET_EXTENSION_NAME )
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
#ifdef VK_EXT_GRAPHICS_PIPELINE_LIBRARY_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( graphics_pipeline_library, VK_EXT_GRAPHICS_PIPELINE_LIBRARY_EXTENSION_NAME )
#endif
#if defined(VK_VERSION_1_4) || defined(VK_EXT_HOST_IMAGE_COPY_EXTENSION_NAME)
      LIBGCT_EXTENSION_CREATE_IF_VERSION_OR_EXT( host_image_copy, 1, 4, 0, VK_EXT_HOST_IMAGE_COPY_EXTENSION_NAME )
#endif
#ifdef VK_EXT_FRAME_BOUNDARY_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( frame_boundary, "VK_EXT_frame_boundary" )
#endif
#if defined(VK_VERSION_1_4) || defined(VK_KHR_GLOBAL_PRIORITY_EXTENSION_NAME)
      LIBGCT_EXTENSION_CREATE_IF_VERSION_OR_EXT( global_priority_query, 1, 4, 0, "VK_KHR_global_priority" )
#elif defined(VK_EXT_GLOBAL_PRIORITY_QUERY_EXTENSION_NAME)
      LIBGCT_EXTENSION_CREATE_IF_EXT( global_priority_query, "VK_EXT_global_priority_query" )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_EXT_HOST_QUERY_RESET_EXTENSION_NAME)
      LIBGCT_EXTENSION_CREATE_IF_VERSION_OR_EXT( host_query_reset, 1, 2, 0, "VK_EXT_host_query_reset" )
#endif
#if defined(VK_VERSION_1_3) || defined(VK_EXT_IMAGE_ROBUSTNESS_EXTENSION_NAME)
      LIBGCT_EXTENSION_CREATE_IF_VERSION_OR_EXT( image_robustness, 1, 3, 0, "VK_EXT_image_robustness" )
#endif
#ifdef VK_EXT_IMAGE_SLICED_VIEW_OF_3D_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( image_sliced_view_of_3d, "VK_EXT_image_sliced_view_of_3d" )
#endif
#ifdef VK_EXT_IMAGE_VIEW_MIN_LOD_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( image_view_min_lod, "VK_EXT_image_view_min_lod" )
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
#ifdef VK_MESA_IMAGE_ALIGNMENT_CONTROL_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( image_alignment_control, VK_MESA_IMAGE_ALIGNMENT_CONTROL_EXTENSION_NAME )
#endif
#ifdef VK_QCOM_IMAGE_PROCESSING_2_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( image_processing2, VK_QCOM_IMAGE_PROCESSING_2_EXTENSION_NAME )
#endif
#ifdef VK_QCOM_IMAGE_PROCESSING_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( image_processing, VK_QCOM_IMAGE_PROCESSING_EXTENSION_NAME )
#endif
#if defined(VK_VERSION_1_3) || defined(VK_EXT_INLINE_UNIFORM_BLOCK_EXTENSION_NAME)
      LIBGCT_EXTENSION_CREATE_IF_VERSION_OR_EXT( inline_uniform_block, 1, 3, 0, "VK_EXT_inline_uniform_block" )
#endif
#ifdef VK_HUAWEI_INVOCATION_MASK_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( invocation_mask, "VK_HUAWEI_invocation_mask" )
#endif
#ifdef VK_EXT_LEGACY_DITHERING_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( legacy_dithering, "VK_EXT_legacy_dithering" )
#endif
#ifdef VK_EXT_LEGACY_VERTEX_ATTRIBUTES_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( legacy_vertex_attribute, VK_EXT_LEGACY_VERTEX_ATTRIBUTES_EXTENSION_NAME )
#endif
#if defined(VK_VERSION_1_4) || defined(VK_EXT_LINE_RASTERIZATION_EXTENSION_NAME)
      LIBGCT_EXTENSION_CREATE_IF_VERSION_OR_EXT( line_rasterization, 1, 4, 0, "VK_KHR_line_rasterization" )
#elif defined(VK_EXT_LINE_RASTERIZATION_EXTENSION_NAME)
      LIBGCT_EXTENSION_CREATE_IF_EXT( line_rasterization, "VK_EXT_line_rasterization" )
#endif
#ifdef VK_NV_LINEAR_COLOR_ATTACHMENT_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( linear_color_attachment, "VK_NV_linear_color_attachment" )
#endif
#if defined(VK_VERSION_1_3) || defined(VK_KHR_MAINTENANCE_4_EXTENSION_NAME)
      LIBGCT_EXTENSION_CREATE_IF_VERSION_OR_EXT( maintenance4, 1, 3, 0, "VK_KHR_maintenance4" )
#endif
#if defined(VK_VERSION_1_4) || defined(VK_KHR_MAINTENANCE_5_EXTENSION_NAME)
      LIBGCT_EXTENSION_CREATE_IF_VERSION_OR_EXT( maintenance5, 1, 4, 0, "VK_KHR_maintenance5" )
#endif
#if defined(VK_VERSION_1_4) || defined(VK_KHR_MAINTENANCE_6_EXTENSION_NAME)
      LIBGCT_EXTENSION_CREATE_IF_VERSION_OR_EXT( maintenance6, 1, 4, 0, "VK_KHR_maintenance6" )
#endif
#ifdef VK_KHR_MAINTENANCE_7_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( maintenance7, "VK_KHR_maintenance7" )
#endif
#ifdef VK_KHR_MAINTENANCE_8_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( maintenance8, "VK_KHR_maintenance8" )
#endif
#ifdef VK_KHR_MAINTENANCE_9_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( maintenance9, "VK_KHR_maintenance9" )
#endif
#ifdef VK_EXT_MAP_MEMORY_PLACED_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( map_memory_placed, VK_EXT_MAP_MEMORY_PLACED_EXTENSION_NAME )
#endif
#ifdef VK_NV_MEMORY_DECOMPRESSION_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( memory_decompression, "VK_NV_memory_decompression" )
#endif
#ifdef VK_EXT_MEMORY_PRIORITY_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( memory_priority, "VK_EXT_memory_priority" )
#endif
#ifdef VK_EXT_MESH_SHADER_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( mesh_shader, "VK_EXT_mesh_shader" )
#endif
#ifdef VK_NV_MESH_SHADER_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( mesh_shader_nv, "VK_NV_mesh_shader" )
#endif
#ifdef VK_EXT_MULTI_DRAW_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( multi_draw, "VK_EXT_multi_draw" )
#endif
#ifdef VK_EXT_MULTISAMPLED_RENDER_TO_SINGLE_SAMPLED_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( multisampled_render_to_single_sampled , "VK_EXT_multisampled_render_to_single_sampled" )
#endif
#if defined(VK_VERSION_1_1) || defined(VK_KHR_MULTIVIEW_EXTENSION_NAME)
      LIBGCT_EXTENSION_CREATE_IF_EXT( multiview, "VK_KHR_multiview" )
#endif
#ifdef VK_QCOM_MULTIVIEW_PER_VIEW_RENDER_AREAS_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( multiview_per_view_render_areas, "VK_QCOM_multiview_per_view_render_areas" )
#endif
#ifdef VK_QCOM_MULTIVIEW_PER_VIEW_VIEWPORTS_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( multiview_per_view_viewports, "VK_QCOM_multiview_per_view_viewports" )
#endif
#ifdef VK_EXT_MUTABLE_DESCRIPTOR_TYPE_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( mutable_descriptor_type, "VK_EXT_mutable_descriptor_type" )
#elif defined(VK_VALVE_MUTABLE_DESCRIPTOR_TYPE_EXTENSION_NAME)
      LIBGCT_EXTENSION_CREATE_IF_EXT( mutable_descriptor_type, "VK_VALVE_mutable_descriptor_type" )
#endif
#ifdef VK_EXT_NESTED_COMMAND_BUFFER_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( nested_command_buffer, VK_EXT_NESTED_COMMAND_BUFFER_EXTENSION_NAME )
#endif
#ifdef VK_EXT_NON_SEAMLESS_CUBE_MAP_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( non_seamless_cube_map, "VK_EXT_non_seamless_cube_map" )
#endif
#ifdef VK_EXT_OPACITY_MICROMAP_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( opacity_micromap, VK_EXT_OPACITY_MICROMAP_EXTENSION_NAME )
#endif
#ifdef VK_NV_OPTICAL_FLOW_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( optical_flow, "VK_NV_optical_flow" )
#endif
#ifdef VK_EXT_PAGEABLE_DEVICE_LOCAL_MEMORY_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( pageable_device_local_memory, "VK_EXT_pageable_device_local_memory" )
#endif
#ifdef VK_NV_PER_STAGE_DESCRIPTOR_SET_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( per_stage_descriptor_set, "VK_NV_per_stage_descriptor_set" )
#endif
#ifdef VK_EXT_NESTED_COMMAND_BUFFER_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( nested_command_buffer, VK_EXT_NESTED_COMMAND_BUFFER_EXTENSION_NAME )
#endif
#ifdef VK_EXT_OPACITY_MICROMAP_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( opacity_micromap, VK_EXT_OPACITY_MICROMAP_EXTENSION_NAME )
#endif
#ifdef VK_KHR_PERFORMANCE_QUERY_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( performance_query, "VK_KHR_performance_query" )
#endif
#ifdef VK_KHR_PIPELINE_BINARY_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( pipeline_binary, VK_KHR_PIPELINE_BINARY_EXTENSION_NAME )
#endif
#if defined(VK_VERSION_1_4) || defined(VK_EXT_PIPELINE_ROBUSTNESS_EXTENSION_NAME)
      LIBGCT_EXTENSION_CREATE_IF_VERSION_OR_EXT( pipeline_robustness, 1, 4, 0, VK_EXT_PIPELINE_ROBUSTNESS_EXTENSION_NAME )
#endif
#ifdef VK_NV_PARTITIONED_ACCELERATION_STRUCTURE_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( partitioned_acceleration_structure, VK_NV_PARTITIONED_ACCELERATION_STRUCTURE_EXTENSION_NAME )
#endif
#if defined(VK_VERSION_1_3) || defined(VK_EXT_PIPELINE_CREATION_CACHE_CONTROL_EXTENSION_NAME)
      LIBGCT_EXTENSION_CREATE_IF_VERSION_OR_EXT( pipeline_creation_cache_control, 1, 3, 0, "VK_EXT_pipeline_creation_cache_control" )
#endif
#ifdef VK_KHR_PIPELINE_EXECUTABLE_PROPERTIES_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( pipeline_executable_properties, "VK_KHR_pipeline_executable_properties" )
#endif
#ifdef VK_EXT_PIPELINE_LIBRARY_GROUP_HANDLES_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( pipeline_library_group_handles, "VK_EXT_pipeline_library_group_handles" )
#endif
#ifdef VK_EXT_PIPELINE_PROPERTIES_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( pipeline_properties, "VK_EXT_pipeline_properties" )
#endif
#if defined(VK_VERSION_1_4) || defined(VK_EXT_PIPELINE_PROTECTED_ACCESS_EXTENSION_NAME)
      LIBGCT_EXTENSION_CREATE_IF_VERSION_OR_EXT( pipeline_protected_access, 1, 4, 0, "VK_EXT_pipeline_protected_access" )
#endif
#ifdef VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( portability_subset, "VK_KHR_portability_subset" )
#endif
#ifdef VK_NV_PRESENT_BARRIER_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( present_barrier, "VK_NV_present_barrier" )
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
#ifdef VK_EXT_PRIMITIVES_GENERATED_QUERY_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( primitives_generated_query, "VK_EXT_primitives_generated_query" )
#endif
#if defined(VK_VERSION_1_3) || defined(VK_EXT_PRIVATE_DATA_EXTENSION_NAME)
      LIBGCT_EXTENSION_CREATE_IF_VERSION_OR_EXT( private_data, 1, 3, 0, "VK_EXT_private_data" )
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
#ifdef VK_EXT_RASTERIZATION_ORDER_ATTACHMENT_ACCESS_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( rasterization_order_attachment_access, "VK_EXT_rasterization_order_attachment_access" )
#endif
#ifdef VK_KHR_RAY_QUERY_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( ray_query, "VK_KHR_ray_query" )
#endif
#ifdef VK_NV_RAY_TRACING_INVOCATION_REORDER_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( ray_tracing_invocation_reorder, "VK_NV_ray_tracing_invocation_reorder" )
#endif
#ifdef VK_KHR_RAY_TRACING_MAINTENANCE_1_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( ray_tracing_maintenance1, "VK_KHR_ray_tracing_maintenance1" )
#endif
#ifdef VK_NV_RAY_TRACING_MOTION_BLUR_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( ray_tracing_motion_blur, "VK_NV_ray_tracing_motion_blur" )
#endif
#ifdef VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( ray_tracing_pipeline, "VK_KHR_ray_tracing_pipeline" )
#endif
#ifdef VK_KHR_RAY_TRACING_POSITION_FETCH_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( ray_tracing_position_fetch, "VK_KHR_ray_tracing_position_fetch" )
#endif
#ifdef VK_IMG_RELAXED_LINE_RASTERIZATION_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( relaxed_line_rasterization, "VK_IMG_relaxed_line_rasterization" )
#endif
#ifdef VK_ARM_RENDER_PASS_STRIPED_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( render_pass_striped, "VK_ARM_render_pass_striped" )
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
#ifdef VK_ARM_SCHEDULING_CONTROLS_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( scheduling_controls, "VK_ARM_scheduling_controls" )
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
#ifdef VK_ARM_SHADER_CORE_BUILTINS_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( shader_core_builtins, "VK_ARM_shader_core_builtins" )
#endif
#if defined(VK_VERSION_1_3) || defined(VK_EXT_SHADER_DEMOTE_TO_HELPER_INVOCATION_EXTENSION_NAME)
      LIBGCT_EXTENSION_CREATE_IF_VERSION_OR_EXT( shader_demote_to_helper_invocation, 1, 3, 0, "VK_EXT_shader_demote_to_helper_invocation" )
#endif
#ifdef VK_VERSION_1_1
      LIBGCT_EXTENSION_CREATE_IF_VERSION( shader_draw_parameters, 1, 1, 0 )
#endif
#ifdef VK_AMD_SHADER_EARLY_AND_LATE_FRAGMENT_TESTS_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( shader_early_and_late_fragment_tests, "VK_AMD_shader_early_and_late_fragment_tests" )
#endif
#ifdef VK_AMDX_SHADER_ENQUEUE_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( shader_enqueue, "VK_AMDX_shader_enqueue" )
#endif
#if defined(VK_VERSION_1_4) || defined(VK_KHR_SHADER_EXPECT_ASSUME_EXTENSION_NAME)
      LIBGCT_EXTENSION_CREATE_IF_VERSION_OR_EXT( shader_expect_assume, 1, 4, 0, "VK_KHR_shader_expect_assume" )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_SHADER_FLOAT16_INT8_EXTENSION_NAME)
      LIBGCT_EXTENSION_CREATE_IF_VERSION_OR_EXT( shader_float16_int8, 1, 2, 0, "VK_KHR_shader_float16_int8" )
#endif
#if defined(VK_VERSION_1_4) || defined(VK_KHR_SHADER_FLOAT_CONTROLS_2_EXTENSION_NAME)
      LIBGCT_EXTENSION_CREATE_IF_VERSION_OR_EXT( shader_float_controls2, 1, 4, 0, "VK_KHR_shader_float_controls2" )
#endif
#ifdef VK_EXT_SHADER_IMAGE_ATOMIC_INT64_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( shader_image_atomic_int64, "VK_EXT_shader_image_atomic_int64" )
#endif
#ifdef VK_NV_SHADER_IMAGE_FOOTPRINT_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( shader_image_footprint, "VK_NV_shader_image_footprint" )
#endif
#if defined(VK_VERSION_1_3) || defined(VK_KHR_SHADER_INTEGER_DOT_PRODUCT_EXTENSION_NAME)
      LIBGCT_EXTENSION_CREATE_IF_VERSION_OR_EXT( shader_integer_dot_product, 1, 3, 0, "VK_KHR_shader_integer_dot_product" )
#endif
#ifdef VK_INTEL_SHADER_INTEGER_FUNCTIONS_2_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( shader_integer_functions2, "VK_INTEL_shader_integer_functions2" )
#endif
#ifdef VK_KHR_SHADER_MAXIMAL_RECONVERGENCE_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( shader_maximal_reconvergence, "VK_KHR_shader_maximal_reconvergence" )
#endif
#ifdef VK_EXT_SHADER_MODULE_IDENTIFIER_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( shader_module_identifier, "VK_EXT_shader_module_identifier" )
#endif
#ifdef VK_EXT_SHADER_OBJECT_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( shader_object, "VK_EXT_shader_object" )
#endif
#ifdef VK_KHR_SHADER_QUAD_CONTROL_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( shader_quad_control, "VK_KHR_shader_quad_control" )
#endif
#ifdef VK_NV_SHADER_SM_BUILTINS_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( shader_smbuiltins, "VK_NV_shader_sm_builtins" )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_SHADER_SUBGROUP_EXTENDED_TYPES_EXTENSION_NAME)
      LIBGCT_EXTENSION_CREATE_IF_VERSION_OR_EXT( shader_subgroup_extended_types, 1, 2, 0, "VK_KHR_shader_subgroup_extended_types" )
#endif
#if defined(VK_VERSION_1_4) || defined(VK_KHR_SHADER_SUBGROUP_ROTATE_EXTENSION_NAME)
      LIBGCT_EXTENSION_CREATE_IF_VERSION_OR_EXT( shader_subgroup_rotate, 1, 4, 0, "VK_KHR_shader_subgroup_rotate" )
#endif
#ifdef VK_KHR_SHADER_SUBGROUP_UNIFORM_CONTROL_FLOW_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( shader_subgroup_uniform_control_flow, "VK_KHR_shader_subgroup_uniform_control_flow" )
#endif
#if defined(VK_VERSION_1_3) || defined(VK_KHR_SHADER_TERMINATE_INVOCATION_EXTENSION_NAME)
      LIBGCT_EXTENSION_CREATE_IF_VERSION_OR_EXT( shader_terminate_invocation, 1, 3, 0, "VK_KHR_shader_terminate_invocation" )
#endif
#ifdef VK_EXT_SHADER_TILE_IMAGE_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( shader_tile_image, "VK_EXT_shader_tile_image" )
#endif
#ifdef VK_NV_SHADING_RATE_IMAGE_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( shading_rate_image, "VK_NV_shading_rate_image" )
#endif
#if defined(VK_VERSION_1_3) || defined(VK_EXT_SUBGROUP_SIZE_CONTROL_EXTENSION_NAME)
      LIBGCT_EXTENSION_CREATE_IF_VERSION_OR_EXT( subgroup_size_control, 1, 3, 0, "VK_EXT_subgroup_size_control" )
#endif
#ifdef VK_EXT_SUBPASS_MERGE_FEEDBACK_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( subpass_merge_feedback, "VK_EXT_subpass_merge_feedback" )
#endif
#ifdef VK_HUAWEI_SUBPASS_SHADING_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( subpass_shading, "VK_HUAWEI_subpass_shading" )
#endif
#ifdef VK_EXT_SWAPCHAIN_MAINTENANCE_1_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( swapchain_maintenance1, "VK_EXT_swapchain_maintenance1" )
#endif
#if defined(VK_VERSION_1_3) || defined(VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME)
      LIBGCT_EXTENSION_CREATE_IF_VERSION_OR_EXT( synchronization2, 1, 3, 0, "VK_KHR_synchronization2" )
#endif
#ifdef VK_EXT_TEXEL_BUFFER_ALIGNMENT_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( texel_buffer_alignment, "VK_EXT_texel_buffer_alignment" )
#endif
#ifdef VK_QCOM_TILE_MEMORY_HEAP_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( tile_memory_heap, VK_QCOM_TILE_MEMORY_HEAP_EXTENSION_NAME )
#endif
#ifdef VK_QCOM_TILE_SHADING_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( tile_shading, VK_QCOM_TILE_SHADING_EXTENSION_NAME )
#endif
#if defined(VK_VERSION_1_3) || defined(VK_EXT_TEXTURE_COMPRESSION_ASTC_HDR_EXTENSION_NAME)
      LIBGCT_EXTENSION_CREATE_IF_VERSION_OR_EXT( texture_compression_astc_hdr, 1, 3, 0, "VK_EXT_texture_compression_astc_hdr" )
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
#if defined(VK_VERSION_1_4) || defined(VK_KHR_VERTEX_ATTRIBUTE_DIVISOR_EXTENSION_NAME)
      LIBGCT_EXTENSION_CREATE_IF_EXT( vertex_attribute_divisor, "VK_KHR_vertex_attribute_divisor" )
#elif defined(VK_EXT_VERTEX_ATTRIBUTE_DIVISOR_EXTENSION_NAME)
      LIBGCT_EXTENSION_CREATE_IF_EXT( vertex_attribute_divisor, "VK_EXT_vertex_attribute_divisor" )
#endif
#ifdef VK_EXT_VERTEX_INPUT_DYNAMIC_STATE_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( vertex_input_dynamic_state, "VK_EXT_vertex_input_dynamic_state" )
#endif
#ifdef VK_KHR_VIDEO_MAINTENANCE_1_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( video_maintenance1, "VK_KHR_video_maintenance1" )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_VULKAN_MEMORY_MODEL_EXTENSION_NAME)
      LIBGCT_EXTENSION_CREATE_IF_EXT( vulkan_memory_model, "VK_KHR_vulkan_memory_model" )
#endif
#ifdef VK_KHR_WORKGROUP_MEMORY_EXPLICIT_LAYOUT_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( workgroup_memory_explicit_layout, "VK_KHR_workgroup_memory_explicit_layout" )
#endif
#ifdef VK_QCOM_YCBCR_DEGAMMA_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( ycbcr_degamma, "VK_QCOM_ycbcr_degamma" )
#endif
#ifdef VK_EXT_YCBCR_2PLANE_444_FORMATS_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( ycbcr_2plane_444_formats, "VK_EXT_ycbcr_2plane_444_formats" )
#endif
#ifdef VK_EXT_YCBCR_IMAGE_ARRAYS_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( ycbcr_image_array, "VK_EXT_ycbcr_image_arrays" )
#endif
#if defined(VK_VERSION_1_3) || defined(VK_KHR_ZERO_INITIALIZE_WORKGROUP_MEMORY_EXTENSION_NAME)
      LIBGCT_EXTENSION_CREATE_IF_VERSION_OR_EXT( zero_initialize_workgroup_memory, 1, 3, 0, "VK_KHR_zero_initialize_workgroup_memory" )
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
#ifdef VK_EXT_DEVICE_ADDRESS_BINDING_REPORT_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( address_binding_report )
#endif
#ifdef VK_SEC_AMIGO_PROFILING_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( amigo_profiling )
#endif
#ifdef VK_EXT_ATTACHMENT_FEEDBACK_LOOP_DYNAMIC_STATE_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( attachment_feedback_loop_dynamic_state )
#endif
#ifdef VK_EXT_ATTACHMENT_FEEDBACK_LOOP_LAYOUT_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( attachment_feedback_loop_layout )
#endif
#ifdef VK_EXT_BLEND_OPERATION_ADVANCED_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( blend_operation_advanced )
#endif
#ifdef VK_NV_CLUSTER_ACCELERATION_STRUCTURE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( cluster_acceleration_structure )
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
#ifdef VK_HUAWEI_CLUSTER_CULLING_SHADER_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( cluster_culling_shader )
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
#ifdef VK_NV_COOPERATIVE_MATRIX_2_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( cooperative_matrix2 )
#endif
#ifdef VK_KHR_COOPERATIVE_MATRIX_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( cooperative_matrix )
#endif
#ifdef VK_NV_COOPERATIVE_MATRIX_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( cooperative_matrix_nv )
#endif
#ifdef VK_NV_COOPERATIVE_VECTOR_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( cooperative_vector )
#endif
#ifdef VK_NV_COPY_MEMORY_INDIRECT_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( copy_memory_indirect )
#endif
#ifdef VK_NV_CORNER_SAMPLED_IMAGE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( corner_sampled_image )
#endif
#ifdef VK_NV_COVERAGE_REDUCTION_MODE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( coverage_reduction_mode )
#endif
#ifdef VK_QCOM_FILTER_CUBIC_CLAMP_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( cubic_clamp )
#endif
#ifdef VK_QCOM_FILTER_CUBIC_WEIGHTS_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( cubic_weight )
#endif
#ifdef VK_NV_CUDA_KERNEL_LAUNCH_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( cuda_kernel_launch )
#endif
#ifdef VK_EXT_CUSTOM_BORDER_COLOR_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( custom_border_color )
#endif
#ifdef VK_NV_DEDICATED_ALLOCATION_IMAGE_ALIASING_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( dedicated_allocation_image_aliasing )
#endif
#ifdef VK_EXT_DEPTH_BIAS_CONTROL_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( depth_bias_control )
#endif
#ifdef VK_EXT_DEPTH_CLAMP_ZERO_ONE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( depth_clamp_zero_one )
#endif
#ifdef VK_EXT_DEPTH_CLIP_CONTROL_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( depth_clip_control )
#endif
#ifdef VK_EXT_DEPTH_CLIP_ENABLE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( depth_clip_enable )
#endif
#ifdef VK_EXT_DESCRIPTOR_BUFFER_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( descriptor_buffer )
#endif
#if defined(VK_EXT_DESCRIPTOR_BUFFER_EXTENSION_NAME) && defined(VK_ARM_TENSORS_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( descriptor_buffer_tensor )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( descriptor_indexing )
#endif
#ifdef VK_NV_DESCRIPTOR_POOL_OVERALLOCATION_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( descriptor_pool_overallocation )
#endif
#ifdef VK_NV_DEVICE_GENERATED_COMMANDS_COMPUTE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( device_generated_commands_compute )
#endif
#ifdef VK_EXT_DEVICE_GENERATED_COMMANDS_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( device_generated_commands )
#endif
#ifdef VK_NV_DEVICE_GENERATED_COMMANDS_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( device_generated_commands_nv )
#endif
#ifdef VK_EXT_DEVICE_MEMORY_REPORT_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( device_memory_report )
#endif
#ifdef VK_NV_DEVICE_DIAGNOSTICS_CONFIG_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( diagnostic_config )
#endif
#ifdef VK_NV_DISPLACEMENT_MICROMAP_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( displacement_micromap )
#endif
#if defined(VK_VERSION_1_3) || defined(VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( dynamic_rendering )
#endif
#if defined(VK_VERSION_1_4) || defined(VK_KHR_DYNAMIC_RENDERING_LOCAL_READ_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( dynamic_rendering_local_read )
#endif
#ifdef VK_EXT_DYNAMIC_RENDERING_UNUSED_ATTACHMENTS_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( dynamic_rendering_unused_attachments )
#endif
#ifdef VK_NV_SCISSOR_EXCLUSIVE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( exclusive_scissor )
#endif
#ifdef VK_EXT_EXTENDED_DYNAMIC_STATE_2_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( extended_dynamic_state2 )
#endif
#ifdef VK_EXT_EXTENDED_DYNAMIC_STATE_3_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( extended_dynamic_state3 )
#endif
#ifdef VK_EXT_EXTENDED_DYNAMIC_STATE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( extended_dynamic_state )
#endif
#ifdef VK_NV_EXTENDED_SPARSE_ADDRESS_SPACE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( extended_sparse_address_space )
#endif
#ifdef VK_ANDROID_EXTERNAL_FORMAT_RESOLVE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( external_format_resolve )
#endif
#ifdef VK_NV_EXTERNAL_MEMORY_RDMA_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( external_memory_rdma )
#endif
#ifdef VK_QNX_EXTERNAL_MEMORY_SCREEN_BUFFER_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( external_memory_screen_buffer )
#endif
#ifdef VK_EXT_DEVICE_FAULT_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( fault )
#endif
#ifdef VK_EXT_FRAGMENT_DENSITY_MAP_2_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( fragment_density_map2 )
#endif
#ifdef VK_EXT_FRAGMENT_DENSITY_MAP_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( fragment_density_map )
#endif
#ifdef VK_VALVE_FRAGMENT_DENSITY_MAP_LAYERED_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( fragment_density_map_layered )
#endif
#if defined(VK_EXT_FRAGMENT_DENSITY_MAP_OFFSET_EXTENSION_NAME) || defined(VK_QCOM_FRAGMENT_DENSITY_MAP_OFFSET_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( fragment_density_map_offset )
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
#ifdef VK_EXT_GRAPHICS_PIPELINE_LIBRARY_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( graphics_pipeline_library )
#endif
#ifdef VK_VERSION_1_4
    LIBGCT_EXTENSION_REBUILD_CHAIN( host_image_copy )
#elif defined(VK_EXT_HOST_IMAGE_COPY_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( host_image_copy )
#endif
#ifdef VK_EXT_FRAME_BOUNDARY_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( frame_boundary )
#endif
#if defined(VK_VERSION_1_4) || defined(VK_KHR_GLOBAL_PRIORITY_EXTENSION_NAME) || defined(VK_EXT_GLOBAL_PRIORITY_QUERY_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( global_priority_query )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_EXT_HOST_QUERY_RESET_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( host_query_reset )
#endif
#if defined(VK_VERSION_1_3) || defined(VK_EXT_IMAGE_ROBUSTNESS_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( image_robustness )
#endif
#ifdef VK_EXT_IMAGE_SLICED_VIEW_OF_3D_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( image_sliced_view_of_3d )
#endif
#ifdef VK_EXT_IMAGE_VIEW_MIN_LOD_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( image_view_min_lod )
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
#ifdef VK_MESA_IMAGE_ALIGNMENT_CONTROL_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( image_alignment_control )
#endif
#ifdef VK_QCOM_IMAGE_PROCESSING_2_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( image_processing2 )
#endif
#ifdef VK_QCOM_IMAGE_PROCESSING_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( image_processing )
#endif
#if defined(VK_VERSION_1_3) || defined(VK_EXT_INLINE_UNIFORM_BLOCK_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( inline_uniform_block )
#endif
#ifdef VK_HUAWEI_INVOCATION_MASK_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( invocation_mask )
#endif
#ifdef VK_EXT_LEGACY_DITHERING_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( legacy_dithering )
#endif
#ifdef VK_EXT_LEGACY_VERTEX_ATTRIBUTES_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( legacy_vertex_attribute )
#endif
#if defined(VK_VERSION_1_4) || defined(VK_KHR_LINE_RASTERIZATION_EXTENSION_NAME) || defined(VK_EXT_LINE_RASTERIZATION_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( line_rasterization )
#endif
#ifdef VK_NV_LINEAR_COLOR_ATTACHMENT_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( linear_color_attachment )
#endif
#if defined(VK_VERSION_1_3) || defined(VK_KHR_MAINTENANCE_4_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( maintenance4 )
#endif
#if defined(VK_VERSION_1_4) || defined(VK_KHR_MAINTENANCE_5_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( maintenance5 )
#endif
#if defined(VK_VERSION_1_4) || defined(VK_KHR_MAINTENANCE_6_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( maintenance6 )
#endif
#ifdef VK_KHR_MAINTENANCE_7_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( maintenance7 )
#endif
#ifdef VK_KHR_MAINTENANCE_8_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( maintenance8 )
#endif
#ifdef VK_KHR_MAINTENANCE_9_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( maintenance9 )
#endif
#ifdef VK_EXT_MAP_MEMORY_PLACED_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( map_memory_placed )
#endif
#ifdef VK_NV_MEMORY_DECOMPRESSION_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( memory_decompression )
#endif
#ifdef VK_EXT_MEMORY_PRIORITY_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( memory_priority )
#endif
#ifdef VK_EXT_MESH_SHADER_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( mesh_shader )
#endif
#ifdef VK_NV_MESH_SHADER_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( mesh_shader_nv )
#endif
#ifdef VK_EXT_MULTI_DRAW_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( multi_draw )
#endif
#ifdef VK_EXT_MULTISAMPLED_RENDER_TO_SINGLE_SAMPLED_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( multisampled_render_to_single_sampled )
#endif
#if defined(VK_VERSION_1_1) || defined(VK_KHR_MULTIVIEW_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( multiview )
#endif
#ifdef VK_QCOM_MULTIVIEW_PER_VIEW_RENDER_AREAS_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( multiview_per_view_render_areas )
#endif
#ifdef VK_QCOM_MULTIVIEW_PER_VIEW_VIEWPORTS_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( multiview_per_view_viewports )
#endif
#if defined(VK_VALVE_MUTABLE_DESCRIPTOR_TYPE_EXTENSION_NAME) || defined(VK_EXT_MUTABLE_DESCRIPTOR_TYPE_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( mutable_descriptor_type )
#endif
#ifdef VK_EXT_NON_SEAMLESS_CUBE_MAP_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( non_seamless_cube_map )
#endif
#ifdef VK_NV_OPTICAL_FLOW_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( optical_flow )
#endif
#ifdef VK_EXT_PAGEABLE_DEVICE_LOCAL_MEMORY_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( pageable_device_local_memory )
#endif
#ifdef VK_NV_PER_STAGE_DESCRIPTOR_SET_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( per_stage_descriptor_set )
#endif
#ifdef VK_EXT_NESTED_COMMAND_BUFFER_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( nested_command_buffer )
#endif
#ifdef VK_EXT_OPACITY_MICROMAP_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( opacity_micromap )
#endif
#ifdef VK_KHR_PERFORMANCE_QUERY_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( performance_query )
#endif
#ifdef VK_KHR_PIPELINE_BINARY_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( pipeline_binary )
#endif
#if defined(VK_VERSION_1_4) || defined(VK_EXT_PIPELINE_ROBUSTNESS_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( pipeline_robustness )
#endif
#ifdef VK_NV_PARTITIONED_ACCELERATION_STRUCTURE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( partitioned_acceleration_structure )
#endif
#if defined(VK_VERSION_1_3) || defined(VK_EXT_PIPELINE_CREATION_CACHE_CONTROL_EXTENSION_NAME)
      LIBGCT_EXTENSION_REBUILD_CHAIN( pipeline_creation_cache_control )
#endif
#ifdef VK_KHR_PIPELINE_EXECUTABLE_PROPERTIES_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( pipeline_executable_properties )
#endif
#ifdef VK_EXT_PIPELINE_LIBRARY_GROUP_HANDLES_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( pipeline_library_group_handles )
#endif
#ifdef VK_EXT_PIPELINE_PROPERTIES_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( pipeline_properties )
#endif
#if defined(VK_VERSION_1_4) || defined(VK_EXT_PIPELINE_PROTECTED_ACCESS_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( pipeline_protected_access )
#endif
#ifdef VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( portability_subset )
#endif
#ifdef VK_NV_PRESENT_BARRIER_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( present_barrier )
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
#ifdef VK_EXT_PRIMITIVES_GENERATED_QUERY_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( primitives_generated_query )
#endif
#if defined(VK_VERSION_1_3) || defined(VK_EXT_PRIVATE_DATA_EXTENSION_NAME)
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
#ifdef VK_EXT_RASTERIZATION_ORDER_ATTACHMENT_ACCESS_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( rasterization_order_attachment_access )
#endif
#ifdef VK_KHR_RAY_QUERY_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( ray_query )
#endif
#ifdef VK_NV_RAY_TRACING_INVOCATION_REORDER_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( ray_tracing_invocation_reorder )
#endif
#ifdef VK_KHR_RAY_TRACING_MAINTENANCE_1_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( ray_tracing_maintenance1 )
#endif
#ifdef VK_NV_RAY_TRACING_MOTION_BLUR_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( ray_tracing_motion_blur )
#endif
#ifdef VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( ray_tracing_pipeline )
#endif
#ifdef VK_KHR_RAY_TRACING_POSITION_FETCH_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( ray_tracing_position_fetch )
#endif
#ifdef VK_IMG_RELAXED_LINE_RASTERIZATION_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( relaxed_line_rasterization )
#endif
#ifdef VK_ARM_RENDER_PASS_STRIPED_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( render_pass_striped )
#endif
#ifdef VK_NV_REPRESENTATIVE_FRAGMENT_TEST_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( representative_fragment_test )
#endif
#ifdef VK_EXT_ROBUSTNESS_2_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( robustness2 )
#endif
#ifdef VK_QCOM_YCBCR_DEGAMMA_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( ycbcr_degamma )
#endif
#if defined(VK_VERSION_1_1) || defined(VK_KHR_SAMPLER_YCBCR_CONVERSION_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( sampler_ycbcr_conversion )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_EXT_SCALAR_BLOCK_LAYOUT_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( scalar_block_layout )
#endif
#ifdef VK_ARM_SCHEDULING_CONTROLS_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( scheduling_controls )
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
#ifdef VK_ARM_SHADER_CORE_BUILTINS_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( shader_core_builtins )
#endif
#if defined(VK_VERSION_1_3) || defined(VK_EXT_SHADER_DEMOTE_TO_HELPER_INVOCATION_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( shader_demote_to_helper_invocation )
#endif
#ifdef VK_VERSION_1_1
    LIBGCT_EXTENSION_REBUILD_CHAIN( shader_draw_parameters )
#endif
#ifdef VK_AMD_SHADER_EARLY_AND_LATE_FRAGMENT_TESTS_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( shader_early_and_late_fragment_tests )
#endif
#ifdef VK_AMDX_SHADER_ENQUEUE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( shader_enqueue )
#endif
#if defined(VK_VERSION_1_4) || defined(VK_KHR_SHADER_EXPECT_ASSUME_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( shader_expect_assume )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_SHADER_FLOAT16_INT8_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( shader_float16_int8 )
#endif
#if defined(VK_VERSION_1_4) || defined(VK_KHR_SHADER_FLOAT_CONTROLS_2_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( shader_float_controls2 )
#endif
#ifdef VK_EXT_SHADER_IMAGE_ATOMIC_INT64_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( shader_image_atomic_int64 )
#endif
#ifdef VK_NV_SHADER_IMAGE_FOOTPRINT_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( shader_image_footprint )
#endif
#if defined(VK_VERSION_1_3) || defined(VK_KHR_SHADER_INTEGER_DOT_PRODUCT_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( shader_integer_dot_product )
#endif
#ifdef VK_INTEL_SHADER_INTEGER_FUNCTIONS_2_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( shader_integer_functions2 )
#endif
#ifdef VK_KHR_SHADER_MAXIMAL_RECONVERGENCE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( shader_maximal_reconvergence )
#endif
#ifdef VK_EXT_SHADER_MODULE_IDENTIFIER_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( shader_module_identifier )
#endif
#ifdef VK_EXT_SHADER_OBJECT_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( shader_object )
#endif
#ifdef VK_KHR_SHADER_QUAD_CONTROL_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( shader_quad_control )
#endif
#ifdef VK_NV_SHADER_SM_BUILTINS_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( shader_smbuiltins )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_SHADER_SUBGROUP_EXTENDED_TYPES_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( shader_subgroup_extended_types )
#endif
#if defined(VK_VERSION_1_4) || defined(VK_KHR_SHADER_SUBGROUP_ROTATE_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( shader_subgroup_rotate )
#endif
#ifdef VK_KHR_SHADER_SUBGROUP_UNIFORM_CONTROL_FLOW_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( shader_subgroup_uniform_control_flow )
#endif
#if defined(VK_VERSION_1_3) || defined(VK_KHR_SHADER_TERMINATE_INVOCATION_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( shader_terminate_invocation )
#endif
#ifdef VK_EXT_SHADER_TILE_IMAGE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( shader_tile_image )
#endif
#ifdef VK_NV_SHADING_RATE_IMAGE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( shading_rate_image )
#endif
#if defined(VK_VERSION_1_3) || defined(VK_EXT_SUBGROUP_SIZE_CONTROL_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( subgroup_size_control )
#endif
#ifdef VK_EXT_SUBPASS_MERGE_FEEDBACK_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( subpass_merge_feedback )
#endif
#ifdef VK_HUAWEI_SUBPASS_SHADING_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( subpass_shading )
#endif
#ifdef VK_EXT_SWAPCHAIN_MAINTENANCE_1_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( swapchain_maintenance1 )
#endif
#if defined(VK_VERSION_1_3) || defined(VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( synchronization2 )
#endif
#ifdef VK_EXT_TEXEL_BUFFER_ALIGNMENT_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( texel_buffer_alignment )
#endif
#ifdef VK_QCOM_TILE_MEMORY_HEAP_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( tile_memory_heap )
#endif
#ifdef VK_QCOM_TILE_SHADING_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( tile_shading )
#endif
#if defined(VK_VERSION_1_3) || defined(VK_EXT_TEXTURE_COMPRESSION_ASTC_HDR_EXTENSION_NAME)
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
#if defined(VK_VERSION_1_4) || defined(VK_KHR_VERTEX_ATTRIBUTE_DIVISOR_EXTENSION_NAME) || defined(VK_EXT_VERTEX_ATTRIBUTE_DIVISOR_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( vertex_attribute_divisor )
#endif
#ifdef VK_EXT_VERTEX_INPUT_DYNAMIC_STATE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( vertex_input_dynamic_state )
#endif
#ifdef VK_KHR_VIDEO_MAINTENANCE_1_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( video_maintenance1 )
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
#if defined(VK_VERSION_1_3) || defined(VK_KHR_ZERO_INITIALIZE_WORKGROUP_MEMORY_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( zero_initialize_workgroup_memory )
#endif
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
  void physical_device_features_t::check_subset( const physical_device_features_t &sub ) const {
#if defined(VK_VERSION_1_1) || defined(VK_KHR_16BIT_STORAGE_EXTENSION_NAME)
    LIBGCT_EXTENSION_CHECK_SUBSET( storage_16bit )
#endif
#ifdef VK_EXT_4444_FORMATS_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( formats_4444 )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_8BIT_STORAGE_EXTENSION_NAME)
    LIBGCT_EXTENSION_CHECK_SUBSET( storage_8bit )
#endif
#ifdef VK_EXT_ASTC_DECODE_MODE_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( astc_decode )
#endif
#ifdef VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( acceleration_strucutre )
#endif
#ifdef VK_EXT_DEVICE_ADDRESS_BINDING_REPORT_EXTENSION_NAME
      LIBGCT_EXTENSION_CHECK_SUBSET( address_binding_report )
#endif
#ifdef VK_SEC_AMIGO_PROFILING_EXTENSION_NAME
      LIBGCT_EXTENSION_CHECK_SUBSET( amigo_profiling )
#endif
#ifdef VK_EXT_ATTACHMENT_FEEDBACK_LOOP_DYNAMIC_STATE_EXTENSION_NAME
      LIBGCT_EXTENSION_CHECK_SUBSET( attachment_feedback_loop_dynamic_state )
#endif
#ifdef VK_EXT_ATTACHMENT_FEEDBACK_LOOP_LAYOUT_EXTENSION_NAME
      LIBGCT_EXTENSION_CHECK_SUBSET( attachment_feedback_loop_layout )
#endif
#ifdef VK_EXT_BLEND_OPERATION_ADVANCED_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( blend_operation_advanced )
#endif
#ifdef VK_NV_CLUSTER_ACCELERATION_STRUCTURE_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( cluster_acceleration_structure )
#endif
#ifdef VK_EXT_BORDER_COLOR_SWIZZLE_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( border_color_swizzle )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME)
    LIBGCT_EXTENSION_CHECK_SUBSET( buffer_device_address )
#endif
#ifdef VK_EXT_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( buffer_device_address_ext )
#endif
#ifdef VK_HUAWEI_CLUSTER_CULLING_SHADER_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( cluster_culling_shader )
#endif
#ifdef VK_AMD_DEVICE_COHERENT_MEMORY_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( coherent_memory )
#endif
#ifdef VK_EXT_COLOR_WRITE_ENABLE_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( color_write_enable )
#endif
#ifdef VK_NV_COMPUTE_SHADER_DERIVATIVES_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( compute_shader_derivatives )
#endif
#ifdef VK_EXT_CONDITIONAL_RENDERING_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( conditional_rendering )
#endif
#ifdef VK_NV_COOPERATIVE_MATRIX_2_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( cooperative_matrix2 )
#endif
#ifdef VK_KHR_COOPERATIVE_MATRIX_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( cooperative_matrix )
#endif
#ifdef VK_NV_COOPERATIVE_MATRIX_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( cooperative_matrix_nv )
#endif
#ifdef VK_NV_COOPERATIVE_VECTOR_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( cooperative_vector )
#endif
#ifdef VK_NV_COPY_MEMORY_INDIRECT_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( copy_memory_indirect )
#endif
#ifdef VK_NV_CORNER_SAMPLED_IMAGE_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( corner_sampled_image )
#endif
#ifdef VK_NV_COVERAGE_REDUCTION_MODE_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( coverage_reduction_mode )
#endif
#ifdef VK_QCOM_FILTER_CUBIC_CLAMP_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( cubic_clamp )
#endif
#ifdef VK_QCOM_FILTER_CUBIC_WEIGHTS_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( cubic_weight )
#endif
#ifdef VK_NV_CUDA_KERNEL_LAUNCH_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( cuda_kernel_launch )
#endif
#ifdef VK_EXT_CUSTOM_BORDER_COLOR_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( custom_border_color )
#endif
#ifdef VK_NV_DEDICATED_ALLOCATION_IMAGE_ALIASING_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( dedicated_allocation_image_aliasing )
#endif
#ifdef VK_EXT_DEPTH_BIAS_CONTROL_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( depth_bias_control )
#endif
#ifdef VK_EXT_DEPTH_CLAMP_ZERO_ONE_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( depth_clamp_zero_one )
#endif
#ifdef VK_EXT_DEPTH_CLIP_CONTROL_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( depth_clip_control )
#endif
#ifdef VK_EXT_DEPTH_CLIP_ENABLE_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( depth_clip_enable )
#endif
#ifdef VK_EXT_DESCRIPTOR_BUFFER_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( descriptor_buffer )
#endif
#if defined(VK_EXT_DESCRIPTOR_BUFFER_EXTENSION_NAME) && defined(VK_ARM_TENSORS_EXTENSION_NAME)
    LIBGCT_EXTENSION_CHECK_SUBSET( descriptor_buffer_tensor )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME)
    LIBGCT_EXTENSION_CHECK_SUBSET( descriptor_indexing )
#endif
#ifdef VK_NV_DESCRIPTOR_POOL_OVERALLOCATION_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( descriptor_pool_overallocation )
#endif
#ifdef VK_NV_DEVICE_GENERATED_COMMANDS_COMPUTE_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( device_generated_commands_compute )
#endif
#ifdef VK_EXT_DEVICE_GENERATED_COMMANDS_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( device_generated_commands )
#endif
#ifdef VK_NV_DEVICE_GENERATED_COMMANDS_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( device_generated_commands_nv )
#endif
#ifdef VK_EXT_DEVICE_MEMORY_REPORT_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( device_memory_report )
#endif
#ifdef VK_NV_DEVICE_DIAGNOSTICS_CONFIG_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( diagnostic_config )
#endif
#ifdef VK_NV_DISPLACEMENT_MICROMAP_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( displacement_micromap )
#endif
#if defined(VK_VERSION_1_3) || defined(VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME)
    LIBGCT_EXTENSION_CHECK_SUBSET( dynamic_rendering )
#endif
#if defined(VK_VERSION_1_4) || defined(VK_KHR_DYNAMIC_RENDERING_LOCAL_READ_EXTENSION_NAME)
    LIBGCT_EXTENSION_CHECK_SUBSET( dynamic_rendering_local_read )
#endif
#ifdef VK_EXT_DYNAMIC_RENDERING_UNUSED_ATTACHMENTS_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( dynamic_rendering_unused_attachments )
#endif
#ifdef VK_NV_SCISSOR_EXCLUSIVE_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( exclusive_scissor )
#endif
#ifdef VK_EXT_EXTENDED_DYNAMIC_STATE_2_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( extended_dynamic_state2 )
#endif
#ifdef VK_EXT_EXTENDED_DYNAMIC_STATE_3_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( extended_dynamic_state3 )
#endif
#ifdef VK_EXT_EXTENDED_DYNAMIC_STATE_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( extended_dynamic_state )
#endif
#ifdef VK_NV_EXTENDED_SPARSE_ADDRESS_SPACE_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( extended_sparse_address_space )
#endif
#ifdef VK_ANDROID_EXTERNAL_FORMAT_RESOLVE_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( external_format_resolve )
#endif
#ifdef VK_NV_EXTERNAL_MEMORY_RDMA_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( external_memory_rdma )
#endif
#ifdef VK_QNX_EXTERNAL_MEMORY_SCREEN_BUFFER_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( external_memory_screen_buffer )
#endif
#ifdef VK_EXT_DEVICE_FAULT_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( fault )
#endif
#ifdef VK_EXT_FRAGMENT_DENSITY_MAP_2_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( fragment_density_map2 )
#endif
#ifdef VK_EXT_FRAGMENT_DENSITY_MAP_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( fragment_density_map )
#endif
#ifdef VK_VALVE_FRAGMENT_DENSITY_MAP_LAYERED_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( fragment_density_map_layered )
#endif
#if defined(VK_EXT_FRAGMENT_DENSITY_MAP_OFFSET_EXTENSION_NAME) || defined(VK_QCOM_FRAGMENT_DENSITY_MAP_OFFSET_EXTENSION_NAME)
    LIBGCT_EXTENSION_CHECK_SUBSET( fragment_density_map_offset )
#endif
#ifdef VK_NV_FRAGMENT_SHADER_BARYCENTRIC_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( fragment_shader_barycentric )
#endif
#ifdef VK_EXT_FRAGMENT_SHADER_INTERLOCK_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( fragment_shader_interlock )
#endif
#ifdef VK_NV_FRAGMENT_SHADING_RATE_ENUMS_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( fragment_shading_rate_enums )
#endif
#ifdef VK_KHR_FRAGMENT_SHADING_RATE_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( fragment_shading_rate )
#endif
#ifdef VK_EXT_GRAPHICS_PIPELINE_LIBRARY_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( graphics_pipeline_library )
#endif
#if defined(VK_VERSION_1_4) || defined(VK_EXT_HOST_IMAGE_COPY_EXTENSION_NAME)
    LIBGCT_EXTENSION_CHECK_SUBSET( host_image_copy )
#endif
#ifdef VK_EXT_FRAME_BOUNDARY_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( frame_boundary )
#endif
#if defined(VK_VERSION_1_4) || defined(VK_KHR_GLOBAL_PRIORITY_EXTENSION_NAME) || defined(VK_EXT_GLOBAL_PRIORITY_QUERY_EXTENSION_NAME)
    LIBGCT_EXTENSION_CHECK_SUBSET( global_priority_query )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_EXT_HOST_QUERY_RESET_EXTENSION_NAME)
    LIBGCT_EXTENSION_CHECK_SUBSET( host_query_reset )
#endif
#if defined(VK_VERSION_1_3) || defined(VK_EXT_IMAGE_ROBUSTNESS_EXTENSION_NAME)
    LIBGCT_EXTENSION_CHECK_SUBSET( image_robustness )
#endif
#ifdef VK_EXT_IMAGE_SLICED_VIEW_OF_3D_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( image_sliced_view_of_3d )
#endif
#ifdef VK_EXT_IMAGE_VIEW_MIN_LOD_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( image_view_min_lod )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_IMAGELESS_FRAMEBUFFER_EXTENSION_NAME)
    LIBGCT_EXTENSION_CHECK_SUBSET( image_less_framebuffer )
#endif
#ifdef VK_EXT_INDEX_TYPE_UINT8_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( index_type_unit8 )
#endif
#ifdef VK_NV_INHERITED_VIEWPORT_SCISSOR_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( inherited_viewport_scissor )
#endif
#ifdef VK_MESA_IMAGE_ALIGNMENT_CONTROL_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( image_alignment_control )
#endif
#ifdef VK_QCOM_IMAGE_PROCESSING_2_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( image_processing2 )
#endif
#ifdef VK_QCOM_IMAGE_PROCESSING_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( image_processing )
#endif
#if defined(VK_VERSION_1_3) || defined(VK_EXT_INLINE_UNIFORM_BLOCK_EXTENSION_NAME)
    LIBGCT_EXTENSION_CHECK_SUBSET( inline_uniform_block )
#endif
#ifdef VK_HUAWEI_INVOCATION_MASK_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( invocation_mask )
#endif
#ifdef VK_EXT_LEGACY_DITHERING_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( legacy_dithering )
#endif
#ifdef VK_EXT_LEGACY_VERTEX_ATTRIBUTES_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( legacy_vertex_attribute )
#endif
#if defined(VK_VERSION_1_4) || defined(VK_KHR_LINE_RASTERIZATION_EXTENSION_NAME) || defined(VK_EXT_LINE_RASTERIZATION_EXTENSION_NAME)
    LIBGCT_EXTENSION_CHECK_SUBSET( line_rasterization )
#endif
#ifdef VK_NV_LINEAR_COLOR_ATTACHMENT_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( linear_color_attachment )
#endif
#if defined(VK_VERSION_1_3) || defined(VK_KHR_MAINTENANCE_4_EXTENSION_NAME)
    LIBGCT_EXTENSION_CHECK_SUBSET( maintenance4 )
#endif
#if defined(VK_VERSION_1_4) || defined(VK_KHR_MAINTENANCE_5_EXTENSION_NAME)
    LIBGCT_EXTENSION_CHECK_SUBSET( maintenance5 )
#endif
#if defined(VK_VERSION_1_4) || defined(VK_KHR_MAINTENANCE_6_EXTENSION_NAME)
    LIBGCT_EXTENSION_CHECK_SUBSET( maintenance6 )
#endif
#ifdef VK_KHR_MAINTENANCE_7_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( maintenance7 )
#endif
#ifdef VK_KHR_MAINTENANCE_8_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( maintenance8 )
#endif
#ifdef VK_KHR_MAINTENANCE_9_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( maintenance9 )
#endif
#ifdef VK_EXT_MAP_MEMORY_PLACED_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( map_memory_placed )
#endif
#ifdef VK_NV_MEMORY_DECOMPRESSION_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( memory_decompression )
#endif
#ifdef VK_EXT_MEMORY_PRIORITY_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( memory_priority )
#endif
#ifdef VK_EXT_MESH_SHADER_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( mesh_shader )
#endif
#ifdef VK_NV_MESH_SHADER_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( mesh_shader_nv )
#endif
#ifdef VK_EXT_MULTI_DRAW_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( multi_draw )
#endif
#ifdef VK_EXT_MULTISAMPLED_RENDER_TO_SINGLE_SAMPLED_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( multisampled_render_to_single_sampled )
#endif
#if defined(VK_VERSION_1_1) || defined(VK_KHR_MULTIVIEW_EXTENSION_NAME)
    LIBGCT_EXTENSION_CHECK_SUBSET( multiview )
#endif
#ifdef VK_QCOM_MULTIVIEW_PER_VIEW_RENDER_AREAS_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( multiview_per_view_render_areas )
#endif
#ifdef VK_QCOM_MULTIVIEW_PER_VIEW_VIEWPORTS_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( multiview_per_view_viewports )
#endif
#if defined(VK_VALVE_MUTABLE_DESCRIPTOR_TYPE_EXTENSION_NAME) || defined(VK_EXT_MUTABLE_DESCRIPTOR_TYPE_EXTENSION_NAME)
    LIBGCT_EXTENSION_CHECK_SUBSET( mutable_descriptor_type )
#endif
#ifdef VK_EXT_NON_SEAMLESS_CUBE_MAP_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( non_seamless_cube_map )
#endif
#ifdef VK_NV_OPTICAL_FLOW_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( optical_flow )
#endif
#ifdef VK_EXT_PAGEABLE_DEVICE_LOCAL_MEMORY_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( pageable_device_local_memory )
#endif
#ifdef VK_NV_PER_STAGE_DESCRIPTOR_SET_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( per_stage_descriptor_set )
#endif
#ifdef VK_EXT_NESTED_COMMAND_BUFFER_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( nested_command_buffer )
#endif
#ifdef VK_EXT_OPACITY_MICROMAP_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( opacity_micromap )
#endif
#ifdef VK_KHR_PERFORMANCE_QUERY_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( performance_query )
#endif
#ifdef VK_KHR_PIPELINE_BINARY_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( pipeline_binary )
#endif
#if defined(VK_VERSION_1_4) || defined(VK_EXT_PIPELINE_ROBUSTNESS_EXTENSION_NAME)
    LIBGCT_EXTENSION_CHECK_SUBSET( pipeline_robustness )
#endif
#ifdef VK_NV_PARTITIONED_ACCELERATION_STRUCTURE_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( partitioned_acceleration_structure )
#endif
#if defined(VK_VERSION_1_3) || defined(VK_EXT_PIPELINE_CREATION_CACHE_CONTROL_EXTENSION_NAME)
    LIBGCT_EXTENSION_CHECK_SUBSET( pipeline_creation_cache_control )
#endif
#ifdef VK_KHR_PIPELINE_EXECUTABLE_PROPERTIES_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( pipeline_executable_properties )
#endif
#ifdef VK_EXT_PIPELINE_LIBRARY_GROUP_HANDLES_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( pipeline_library_group_handles )
#endif
#ifdef VK_EXT_PIPELINE_PROPERTIES_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( pipeline_properties )
#endif
#if defined(VK_VERSION_1_4) || defined(VK_EXT_PIPELINE_PROTECTED_ACCESS_EXTENSION_NAME)
    LIBGCT_EXTENSION_CHECK_SUBSET( pipeline_protected_access )
#endif
#ifdef VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( portability_subset )
#endif
#ifdef VK_NV_PRESENT_BARRIER_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( present_barrier )
#endif
#ifdef VK_KHR_PRESENT_ID_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( present_id )
#endif
#ifdef VK_KHR_PRESENT_WAIT_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( present_wait )
#endif
#ifdef VK_EXT_PRIMITIVE_TOPOLOGY_LIST_RESTART_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( primitive_topology_list_restart )
#endif
#ifdef VK_EXT_PRIMITIVES_GENERATED_QUERY_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( primitives_generated_query )
#endif
#if defined(VK_VERSION_1_3) || defined(VK_EXT_PRIVATE_DATA_EXTENSION_NAME)
    LIBGCT_EXTENSION_CHECK_SUBSET( private_data )
#endif
#ifdef VK_VERSION_1_1
    LIBGCT_EXTENSION_CHECK_SUBSET( protected_memory )
#endif
#ifdef VK_EXT_PROVOKING_VERTEX_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( provoking_vertex )
#endif
#ifdef VK_EXT_RGBA10X6_FORMATS_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( rgba10x6_formats )
#endif
#ifdef VK_EXT_RASTERIZATION_ORDER_ATTACHMENT_ACCESS_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( rasterization_order_attachment_access )
#endif
#ifdef VK_KHR_RAY_QUERY_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( ray_query )
#endif
#ifdef VK_NV_RAY_TRACING_INVOCATION_REORDER_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( ray_tracing_invocation_reorder )
#endif
#ifdef VK_KHR_RAY_TRACING_MAINTENANCE_1_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( ray_tracing_maintenance1 )
#endif
#ifdef VK_NV_RAY_TRACING_MOTION_BLUR_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( ray_tracing_motion_blur )
#endif
#ifdef VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( ray_tracing_pipeline )
#endif
#ifdef VK_KHR_RAY_TRACING_POSITION_FETCH_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( ray_tracing_position_fetch )
#endif
#ifdef VK_IMG_RELAXED_LINE_RASTERIZATION_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( relaxed_line_rasterization )
#endif
#ifdef VK_ARM_RENDER_PASS_STRIPED_EXTENSION_NAME
      LIBGCT_EXTENSION_CHECK_SUBSET( render_pass_striped )
#endif
#ifdef VK_NV_REPRESENTATIVE_FRAGMENT_TEST_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( representative_fragment_test )
#endif
#ifdef VK_EXT_ROBUSTNESS_2_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( robustness2 )
#endif
#ifdef VK_QCOM_YCBCR_DEGAMMA_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( ycbcr_degamma )
#endif
#if defined(VK_VERSION_1_1) || defined(VK_KHR_SAMPLER_YCBCR_CONVERSION_EXTENSION_NAME)
    LIBGCT_EXTENSION_CHECK_SUBSET( sampler_ycbcr_conversion )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_EXT_SCALAR_BLOCK_LAYOUT_EXTENSION_NAME)
    LIBGCT_EXTENSION_CHECK_SUBSET( scalar_block_layout )
#endif
#ifdef VK_ARM_SCHEDULING_CONTROLS_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( scheduling_controls )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_SEPARATE_DEPTH_STENCIL_LAYOUTS_EXTENSION_NAME)
    LIBGCT_EXTENSION_CHECK_SUBSET( separate_depth_stencil_layouts )
#endif
#ifdef VK_EXT_SHADER_ATOMIC_FLOAT_2_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( shader_atomic_float2 )
#endif
#ifdef VK_EXT_SHADER_ATOMIC_FLOAT_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( shader_atomic_float )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_SHADER_ATOMIC_INT64_EXTENSION_NAME)
    LIBGCT_EXTENSION_CHECK_SUBSET( shader_atomic_int64 )
#endif
#ifdef VK_KHR_SHADER_CLOCK_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( shader_clock )
#endif
#ifdef VK_ARM_SHADER_CORE_BUILTINS_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( shader_core_builtins )
#endif
#if defined(VK_VERSION_1_3) || defined(VK_EXT_SHADER_DEMOTE_TO_HELPER_INVOCATION_EXTENSION_NAME)
    LIBGCT_EXTENSION_CHECK_SUBSET( shader_demote_to_helper_invocation )
#endif
#ifdef VK_VERSION_1_1
    LIBGCT_EXTENSION_CHECK_SUBSET( shader_draw_parameters )
#endif
#ifdef VK_AMD_SHADER_EARLY_AND_LATE_FRAGMENT_TESTS_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( shader_early_and_late_fragment_tests )
#endif
#ifdef VK_AMDX_SHADER_ENQUEUE_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( shader_enqueue )
#endif
#if defined(VK_VERSION_1_4) || defined(VK_KHR_SHADER_EXPECT_ASSUME_EXTENSION_NAME)
    LIBGCT_EXTENSION_CHECK_SUBSET( shader_expect_assume )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_SHADER_FLOAT16_INT8_EXTENSION_NAME)
    LIBGCT_EXTENSION_CHECK_SUBSET( shader_float16_int8 )
#endif
#if defined(VK_VERSION_1_4) || defined(VK_KHR_SHADER_FLOAT_CONTROLS_2_EXTENSION_NAME)
    LIBGCT_EXTENSION_CHECK_SUBSET( shader_float_controls2 )
#endif
#ifdef VK_EXT_SHADER_IMAGE_ATOMIC_INT64_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( shader_image_atomic_int64 )
#endif
#ifdef VK_NV_SHADER_IMAGE_FOOTPRINT_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( shader_image_footprint )
#endif
#if defined(VK_VERSION_1_3) || defined(VK_KHR_SHADER_INTEGER_DOT_PRODUCT_EXTENSION_NAME)
    LIBGCT_EXTENSION_CHECK_SUBSET( shader_integer_dot_product )
#endif
#ifdef VK_INTEL_SHADER_INTEGER_FUNCTIONS_2_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( shader_integer_functions2 )
#endif
#ifdef VK_KHR_SHADER_MAXIMAL_RECONVERGENCE_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( shader_maximal_reconvergence )
#endif
#ifdef VK_EXT_SHADER_MODULE_IDENTIFIER_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( shader_module_identifier )
#endif
#ifdef VK_EXT_SHADER_OBJECT_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( shader_object )
#endif
#ifdef VK_KHR_SHADER_QUAD_CONTROL_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( shader_quad_control )
#endif
#ifdef VK_NV_SHADER_SM_BUILTINS_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( shader_smbuiltins )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_SHADER_SUBGROUP_EXTENDED_TYPES_EXTENSION_NAME)
    LIBGCT_EXTENSION_CHECK_SUBSET( shader_subgroup_extended_types )
#endif
#if defined(VK_VERSION_1_4) || defined(VK_KHR_SHADER_SUBGROUP_ROTATE_EXTENSION_NAME)
    LIBGCT_EXTENSION_CHECK_SUBSET( shader_subgroup_rotate )
#endif
#ifdef VK_KHR_SHADER_SUBGROUP_UNIFORM_CONTROL_FLOW_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( shader_subgroup_uniform_control_flow )
#endif
#if defined(VK_VERSION_1_3) || defined(VK_KHR_SHADER_TERMINATE_INVOCATION_EXTENSION_NAME)
    LIBGCT_EXTENSION_CHECK_SUBSET( shader_terminate_invocation )
#endif
#ifdef VK_EXT_SHADER_TILE_IMAGE_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( shader_tile_image )
#endif
#ifdef VK_NV_SHADING_RATE_IMAGE_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( shading_rate_image )
#endif
#if defined(VK_VERSION_1_3) || defined(VK_EXT_SUBGROUP_SIZE_CONTROL_EXTENSION_NAME)
    LIBGCT_EXTENSION_CHECK_SUBSET( subgroup_size_control )
#endif
#ifdef VK_EXT_SUBPASS_MERGE_FEEDBACK_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( subpass_merge_feedback )
#endif
#ifdef VK_HUAWEI_SUBPASS_SHADING_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( subpass_shading )
#endif
#ifdef VK_EXT_SWAPCHAIN_MAINTENANCE_1_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( swapchain_maintenance1 )
#endif
#if defined(VK_VERSION_1_3) || defined(VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME)
    LIBGCT_EXTENSION_CHECK_SUBSET( synchronization2 )
#endif
#ifdef VK_EXT_TEXEL_BUFFER_ALIGNMENT_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( texel_buffer_alignment )
#endif
#ifdef VK_QCOM_TILE_MEMORY_HEAP_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( tile_memory_heap )
#endif
#ifdef VK_QCOM_TILE_SHADING_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( tile_shading )
#endif
#if defined(VK_VERSION_1_3) || defined(VK_EXT_TEXTURE_COMPRESSION_ASTC_HDR_EXTENSION_NAME)
    LIBGCT_EXTENSION_CHECK_SUBSET( texture_compression_astc_hdr )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME)
    LIBGCT_EXTENSION_CHECK_SUBSET( timeline_semaphore )
#endif
#ifdef VK_EXT_TRANSFORM_FEEDBACK_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( transform_feedback )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_UNIFORM_BUFFER_STANDARD_LAYOUT_EXTENSION_NAME)
    LIBGCT_EXTENSION_CHECK_SUBSET( uniform_buffer_standard_layout )
#endif
#if defined(VK_VERSION_1_1) || defined(VK_KHR_VARIABLE_POINTERS_EXTENSION_NAME)
    LIBGCT_EXTENSION_CHECK_SUBSET( variable_pointers )
#endif
#if defined(VK_VERSION_1_4) || defined(VK_KHR_VERTEX_ATTRIBUTE_DIVISOR_EXTENSION_NAME) || defined(VK_EXT_VERTEX_ATTRIBUTE_DIVISOR_EXTENSION_NAME)
    LIBGCT_EXTENSION_CHECK_SUBSET( vertex_attribute_divisor )
#endif
#ifdef VK_EXT_VERTEX_INPUT_DYNAMIC_STATE_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( vertex_input_dynamic_state )
#endif
#ifdef VK_KHR_VIDEO_MAINTENANCE_1_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( video_maintenance1 )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_VULKAN_MEMORY_MODEL_EXTENSION_NAME)
    LIBGCT_EXTENSION_CHECK_SUBSET( vulkan_memory_model )
#endif
#ifdef VK_KHR_WORKGROUP_MEMORY_EXPLICIT_LAYOUT_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( workgroup_memory_explicit_layout )
#endif
#ifdef VK_EXT_YCBCR_2PLANE_444_FORMATS_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( ycbcr_2plane_444_formats )
#endif
#ifdef VK_EXT_YCBCR_IMAGE_ARRAYS_EXTENSION_NAME
    LIBGCT_EXTENSION_CHECK_SUBSET( ycbcr_image_array )
#endif
#if defined(VK_VERSION_1_3) || defined(VK_KHR_ZERO_INITIALIZE_WORKGROUP_MEMORY_EXTENSION_NAME)
    LIBGCT_EXTENSION_CHECK_SUBSET( zero_initialize_workgroup_memory )
#endif
  }
}
