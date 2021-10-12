#include <gct/instance.hpp>
#include <gct/physical_device_properties.hpp>
#include <gct/physical_device_features.hpp>

namespace gct {
  nlohmann::json to_json( const vk::PhysicalDeviceFeatures &v ) {
    auto root = nlohmann::json::object();
    root[ "robustBufferAccess" ] = bool( v.robustBufferAccess );
    root[ "fullDrawIndexUint32" ] = bool( v.fullDrawIndexUint32 );
    root[ "imageCubeArray" ] = bool( v.imageCubeArray );
    root[ "independentBlend" ] = bool( v.independentBlend );
    root[ "geometryShader" ] = bool( v.geometryShader );
    root[ "tessellationShader" ] = bool( v.tessellationShader );
    root[ "sampleRateShading" ] = bool( v.sampleRateShading );
    root[ "dualSrcBlend" ] = bool( v.dualSrcBlend );
    root[ "logicOp" ] = bool( v.logicOp );
    root[ "multiDrawIndirect" ] = bool( v.multiDrawIndirect );
    root[ "drawIndirectFirstInstance" ] = bool( v.drawIndirectFirstInstance );
    root[ "depthClamp" ] = bool( v.depthClamp );
    root[ "depthBiasClamp" ] = bool( v.depthBiasClamp );
    root[ "fillModeNonSolid" ] = bool( v.fillModeNonSolid );
    root[ "depthBounds" ] = bool( v.depthBounds );
    root[ "wideLines" ] = bool( v.wideLines );
    root[ "largePoints" ] = bool( v.largePoints );
    root[ "alphaToOne" ] = bool( v.alphaToOne );
    root[ "multiViewport" ] = bool( v.multiViewport );
    root[ "samplerAnisotropy" ] = bool( v.samplerAnisotropy );
    root[ "textureCompressionETC2" ] = bool( v.textureCompressionETC2 );
    root[ "textureCompressionASTC_LDR" ] = bool( v.textureCompressionASTC_LDR );
    root[ "textureCompressionBC" ] = bool( v.textureCompressionBC );
    root[ "occlusionQueryPrecise" ] = bool( v.occlusionQueryPrecise );
    root[ "pipelineStatisticsQuery" ] = bool( v.pipelineStatisticsQuery );
    root[ "vertexPipelineStoresAndAtomics" ] = bool( v.vertexPipelineStoresAndAtomics );
    root[ "fragmentStoresAndAtomics" ] = bool( v.fragmentStoresAndAtomics );
    root[ "shaderTessellationAndGeometryPointSize" ] = bool( v.shaderTessellationAndGeometryPointSize );
    root[ "shaderImageGatherExtended" ] = bool( v.shaderImageGatherExtended );
    root[ "shaderStorageImageExtendedFormats" ] = bool( v.shaderStorageImageExtendedFormats );
    root[ "shaderStorageImageMultisample" ] = bool( v.shaderStorageImageMultisample );
    root[ "shaderStorageImageReadWithoutFormat" ] = bool( v.shaderStorageImageReadWithoutFormat );
    root[ "shaderStorageImageWriteWithoutFormat" ] = bool( v.shaderStorageImageWriteWithoutFormat );
    root[ "shaderUniformBufferArrayDynamicIndexing" ] = bool( v.shaderUniformBufferArrayDynamicIndexing );
    root[ "shaderSampledImageArrayDynamicIndexing" ] = bool( v.shaderSampledImageArrayDynamicIndexing );
    root[ "shaderStorageBufferArrayDynamicIndexing" ] = bool( v.shaderStorageBufferArrayDynamicIndexing );
    root[ "shaderStorageImageArrayDynamicIndexing" ] = bool( v.shaderStorageImageArrayDynamicIndexing );
    root[ "shaderClipDistance" ] = bool( v.shaderClipDistance );
    root[ "shaderCullDistance" ] = bool( v.shaderCullDistance );
    root[ "shaderFloat64" ] = bool( v.shaderFloat64 );
    root[ "shaderInt64" ] = bool( v.shaderInt64 );
    root[ "shaderInt16" ] = bool( v.shaderInt16 );
    root[ "shaderResourceResidency" ] = bool( v.shaderResourceResidency );
    root[ "shaderResourceMinLod" ] = bool( v.shaderResourceMinLod );
    root[ "sparseBinding" ] = bool( v.sparseBinding );
    root[ "sparseResidencyBuffer" ] = bool( v.sparseResidencyBuffer );
    root[ "sparseResidencyImage2D" ] = bool( v.sparseResidencyImage2D );
    root[ "sparseResidencyImage3D" ] = bool( v.sparseResidencyImage3D );
    root[ "sparseResidency2Samples" ] = bool( v.sparseResidency2Samples );
    root[ "sparseResidency4Samples" ] = bool( v.sparseResidency4Samples );
    root[ "sparseResidency8Samples" ] = bool( v.sparseResidency8Samples );
    root[ "sparseResidency16Samples" ] = bool( v.sparseResidency16Samples );
    root[ "sparseResidencyAliased" ] = bool( v.sparseResidencyAliased );
    root[ "variableMultisampleRate" ] = bool( v.variableMultisampleRate );
    root[ "inheritedQueries" ] = bool( v.inheritedQueries );
    return root; 
  }
#if defined(VK_VERSION_1_1) || defined(VK_KHR_16BIT_STORAGE_EXTENSION_NAME)
#ifdef VK_VERSION_1_1
  nlohmann::json to_json( const vk::PhysicalDevice16BitStorageFeatures &v ) {
#elif defined(VK_KHR_16BIT_STORAGE_EXTENSION_NAME)
  nlohmann::json to_json( const vk::PhysicalDevice16BitStorageFeaturesKHR &v ) {
#endif
    auto root = nlohmann::json::object();
    root[ "storageBuffer16BitAccess" ] = bool( v.storageBuffer16BitAccess );
    root[ "uniformAndStorageBuffer16BitAccess" ] = bool( v.uniformAndStorageBuffer16BitAccess );
    root[ "storagePushConstant16" ] = bool( v.storagePushConstant16 );
    root[ "storageInputOutput16" ] = bool( v.storageInputOutput16 );
    return root;
  }
#endif
#ifdef VK_EXT_4444_FORMATS_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDevice4444FormatsFeaturesEXT &v ) {
    auto root = nlohmann::json::object();
    root[ "formatA4R4G4B4" ] = bool( v.formatA4R4G4B4 );
    root[ "formatA4B4G4R4" ] = bool( v.formatA4B4G4R4 );
    return root;
  }
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_8BIT_STORAGE_EXTENSION_NAME)
#ifdef VK_VERSION_1_2
  nlohmann::json to_json( const vk::PhysicalDevice8BitStorageFeatures &v ) {
#elif defined(VK_KHR_8BIT_STORAGE_EXTENSION_NAME)
  nlohmann::json to_json( const vk::VkPhysicalDevice8BitStorageFeaturesKHR &v ) {
#endif
    auto root = nlohmann::json::object();
    root[ "storageBuffer8BitAccess" ] = bool( v.storageBuffer8BitAccess );
    root[ "uniformAndStorageBuffer8BitAccess" ] = bool( v.uniformAndStorageBuffer8BitAccess );
    root[ "storagePushConstant8" ] = bool( v.storagePushConstant8 );
    return root;
  }
#endif
#ifdef VK_EXT_ASTC_DECODE_MODE_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceASTCDecodeFeaturesEXT &v ) {
    auto root = nlohmann::json::object();
    root[ "decodeModeSharedExponent" ] = bool( v.decodeModeSharedExponent );
    return root;
  }
#endif
#ifdef VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceAccelerationStructureFeaturesKHR &v ) {
    auto root = nlohmann::json::object();
    root[ "accelerationStructure" ] = bool( v.accelerationStructure );
    root[ "accelerationStructureCaptureReplay" ] = bool( v.accelerationStructureCaptureReplay );
    root[ "accelerationStructureIndirectBuild" ] = bool( v.accelerationStructureIndirectBuild );
    root[ "accelerationStructureHostCommands" ] = bool( v.accelerationStructureHostCommands );
    root[ "descriptorBindingAccelerationStructureUpdateAfterBind" ] = bool( v.descriptorBindingAccelerationStructureUpdateAfterBind );
    return root;
  }
#endif
#ifdef VK_EXT_BLEND_OPERATION_ADVANCED_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceBlendOperationAdvancedFeaturesEXT &v ) {
    auto root = nlohmann::json::object();
    root[ "advancedBlendCoherentOperations" ] = bool( v.advancedBlendCoherentOperations );
    return root;
  }
#endif
#ifdef VK_EXT_BORDER_COLOR_SWIZZLE_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceBorderColorSwizzleFeaturesEXT &v ) {
    auto root = nlohmann::json::object();
    root[ "borderColorSwizzle" ] = bool( v.borderColorSwizzle );
    root[ "borderColorSwizzleFromImage" ] = bool( v.borderColorSwizzleFromImage );
    return root;
  }
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME)
#ifdef VK_VERSION_1_2
  nlohmann::json to_json( const vk::PhysicalDeviceBufferDeviceAddressFeatures &v ) {
#elif defined(VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME)
  nlohmann::json to_json( const vk::PhysicalDeviceBufferDeviceAddressFeaturesKHR &v ) {
#endif
    auto root = nlohmann::json::object();
    root[ "bufferDeviceAddress" ] = bool( v.bufferDeviceAddress );
    root[ "bufferDeviceAddressCaptureReplay" ] = bool( v.bufferDeviceAddressCaptureReplay );
    root[ "bufferDeviceAddressMultiDevice" ] = bool( v.bufferDeviceAddressMultiDevice );
    return root;
  }
#endif
#ifdef VK_EXT_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceBufferDeviceAddressFeaturesEXT &v ) {
    auto root = nlohmann::json::object();
    root[ "bufferDeviceAddress" ] = bool( v.bufferDeviceAddress );
    root[ "bufferDeviceAddressCaptureReplay" ] = bool( v.bufferDeviceAddressCaptureReplay );
    root[ "bufferDeviceAddressMultiDevice" ] = bool( v.bufferDeviceAddressMultiDevice );
    return root;
  }
#endif
#ifdef VK_AMD_DEVICE_COHERENT_MEMORY_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceCoherentMemoryFeaturesAMD &v ) {
    auto root = nlohmann::json::object();
    root[ "deviceCoherentMemory" ] = bool( v.deviceCoherentMemory );
    return root;
  }
#endif
#ifdef VK_EXT_COLOR_WRITE_ENABLE_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceColorWriteEnableFeaturesEXT &v ) {
    auto root = nlohmann::json::object();
    root[ "colorWriteEnable" ] = bool( v.colorWriteEnable );
    return root;
  }
#endif
#ifdef VK_NV_COMPUTE_SHADER_DERIVATIVES_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceComputeShaderDerivativesFeaturesNV &v ) {
    auto root = nlohmann::json::object();
    root[ "computeDerivativeGroupQuads" ] = bool( v.computeDerivativeGroupQuads );
    root[ "computeDerivativeGroupLinear" ] = bool( v.computeDerivativeGroupLinear );
    return root;
  }
#endif
#ifdef VK_EXT_CONDITIONAL_RENDERING_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceConditionalRenderingFeaturesEXT &v ) {
    auto root = nlohmann::json::object();
    root[ "conditionalRendering" ] = bool( v.conditionalRendering );
    root[ "inheritedConditionalRendering" ] = bool( v.inheritedConditionalRendering );
    return root;
  }
#endif
#ifdef VK_NV_COOPERATIVE_MATRIX_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceCooperativeMatrixFeaturesNV &v ) {
    auto root = nlohmann::json::object();
    root[ "cooperativeMatrix" ] = bool( v.cooperativeMatrix );
    root[ "cooperativeMatrixRobustBufferAccess" ] = bool( v.cooperativeMatrixRobustBufferAccess );
    return root;
  }
#endif
#ifdef VK_NV_CORNER_SAMPLED_IMAGE_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceCornerSampledImageFeaturesNV &v ) {
    auto root = nlohmann::json::object();
    root[ "cornerSampledImage" ] = bool( v.cornerSampledImage );
    return root;
  }
#endif
#ifdef VK_NV_COVERAGE_REDUCTION_MODE_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceCoverageReductionModeFeaturesNV &v ) {
    auto root = nlohmann::json::object();
    root[ "coverageReductionMode" ] = bool( v.coverageReductionMode );
    return root;
  }
#endif
#ifdef VK_EXT_CUSTOM_BORDER_COLOR_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceCustomBorderColorFeaturesEXT &v ) {
    auto root = nlohmann::json::object();
    root[ "customBorderColors" ] = bool( v.customBorderColors );
    root[ "customBorderColorWithoutFormat" ] = bool( v.customBorderColorWithoutFormat );
    return root;
  }
#endif
#ifdef VK_NV_DEDICATED_ALLOCATION_IMAGE_ALIASING_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceDedicatedAllocationImageAliasingFeaturesNV &v ) {
    auto root = nlohmann::json::object();
    root[ "dedicatedAllocationImageAliasing" ] = bool( v.dedicatedAllocationImageAliasing );
    return root;
  }
#endif
#ifdef VK_EXT_DEPTH_CLIP_ENABLE_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceDepthClipEnableFeaturesEXT &v ) {
    auto root = nlohmann::json::object();
    root[ "depthClipEnable" ] = bool( v.depthClipEnable );
    return root;
  }
#endif
#if defined(VK_VERSION_1_2) || defined(VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME)
#ifdef VK_VERSION_1_2
  nlohmann::json to_json( const vk::PhysicalDeviceDescriptorIndexingFeatures &v ) {
#elif defined(VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME)
  nlohmann::json to_json( const vk::PhysicalDeviceDescriptorIndexingFeaturesEXT &v ) {
#endif
    auto root = nlohmann::json::object();
    root[ "shaderInputAttachmentArrayDynamicIndexing" ] = bool( v.shaderInputAttachmentArrayDynamicIndexing );
    root[ "shaderUniformTexelBufferArrayDynamicIndexing" ] = bool( v.shaderUniformTexelBufferArrayDynamicIndexing );
    root[ "shaderStorageTexelBufferArrayDynamicIndexing" ] = bool( v.shaderStorageTexelBufferArrayDynamicIndexing );
    root[ "shaderUniformBufferArrayNonUniformIndexing" ] = bool( v.shaderUniformBufferArrayNonUniformIndexing );
    root[ "shaderSampledImageArrayNonUniformIndexing" ] = bool( v.shaderSampledImageArrayNonUniformIndexing );
    root[ "shaderStorageBufferArrayNonUniformIndexing" ] = bool( v.shaderStorageBufferArrayNonUniformIndexing );
    root[ "shaderStorageImageArrayNonUniformIndexing" ] = bool( v.shaderStorageImageArrayNonUniformIndexing );
    root[ "shaderInputAttachmentArrayNonUniformIndexing" ] = bool( v.shaderInputAttachmentArrayNonUniformIndexing );
    root[ "shaderUniformTexelBufferArrayNonUniformIndexing" ] = bool( v.shaderUniformTexelBufferArrayNonUniformIndexing );
    root[ "shaderStorageTexelBufferArrayNonUniformIndexing" ] = bool( v.shaderStorageTexelBufferArrayNonUniformIndexing );
    root[ "descriptorBindingUniformBufferUpdateAfterBind" ] = bool( v.descriptorBindingUniformBufferUpdateAfterBind );
    root[ "descriptorBindingSampledImageUpdateAfterBind" ] = bool( v.descriptorBindingSampledImageUpdateAfterBind );
    root[ "descriptorBindingStorageImageUpdateAfterBind" ] = bool( v.descriptorBindingStorageImageUpdateAfterBind );
    root[ "descriptorBindingStorageBufferUpdateAfterBind" ] = bool( v.descriptorBindingStorageBufferUpdateAfterBind );
    root[ "descriptorBindingUniformTexelBufferUpdateAfterBind" ] = bool( v.descriptorBindingUniformTexelBufferUpdateAfterBind );
    root[ "descriptorBindingStorageTexelBufferUpdateAfterBind" ] = bool( v.descriptorBindingStorageTexelBufferUpdateAfterBind );
    root[ "descriptorBindingUpdateUnusedWhilePending" ] = bool( v.descriptorBindingUpdateUnusedWhilePending );
    root[ "descriptorBindingPartiallyBound" ] = bool( v.descriptorBindingPartiallyBound );
    root[ "descriptorBindingVariableDescriptorCount" ] = bool( v.descriptorBindingVariableDescriptorCount );
    root[ "runtimeDescriptorArray" ] = bool( v.runtimeDescriptorArray );
    return root;
  }
#endif
#ifdef VK_NV_DEVICE_GENERATED_COMMANDS_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceDeviceGeneratedCommandsFeaturesNV &v ) {
    auto root = nlohmann::json::object();
    root[ "deviceGeneratedCommands" ] = bool( v.deviceGeneratedCommands );
    return root;
  }
#endif
#ifdef VK_EXT_DEVICE_MEMORY_REPORT_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceDeviceMemoryReportFeaturesEXT &v ) {
    auto root = nlohmann::json::object();
    root[ "deviceMemoryReport" ] = bool( v.deviceMemoryReport );
    return root;
  }
#endif
#ifdef VK_NV_DEVICE_DIAGNOSTICS_CONFIG_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceDiagnosticsConfigFeaturesNV &v ) {
    auto root = nlohmann::json::object();
    root[ "diagnosticsConfig" ] = bool( v.diagnosticsConfig );
    return root;
  }
#endif
#ifdef VK_NV_SCISSOR_EXCLUSIVE_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceExclusiveScissorFeaturesNV &v ) {
    auto root = nlohmann::json::object();
    root[ "exclusiveScissor" ] = bool( v.exclusiveScissor );
    return root;
  }
#endif
#ifdef VK_EXT_EXTENDED_DYNAMIC_STATE_2_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceExtendedDynamicState2FeaturesEXT &v ) {
    auto root = nlohmann::json::object();
    root[ "extendedDynamicState2" ] = bool( v.extendedDynamicState2 );
    root[ "extendedDynamicState2LogicOp" ] = bool( v.extendedDynamicState2LogicOp );
    root[ "extendedDynamicState2PatchControlPoints" ] = bool( v.extendedDynamicState2PatchControlPoints );
    return root;
  }
#endif
#ifdef VK_EXT_EXTENDED_DYNAMIC_STATE_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT &v ) {
    auto root = nlohmann::json::object();
    root[ "extendedDynamicState" ] = bool( v.extendedDynamicState );
    return root;
  }
#endif
#ifdef VK_NV_EXTERNAL_MEMORY_RDMA_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceExternalMemoryRDMAFeaturesNV &v ) {
    auto root = nlohmann::json::object();
    root[ "externalMemoryRDMA" ] = bool( v.externalMemoryRDMA );
    return root;
  }
#endif
#ifdef VK_EXT_FRAGMENT_DENSITY_MAP_2_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceFragmentDensityMap2FeaturesEXT &v ) {
    auto root = nlohmann::json::object();
    root[ "fragmentDensityMapDeferred" ] = bool( v.fragmentDensityMapDeferred );
    return root;
  }
#endif
#ifdef VK_EXT_FRAGMENT_DENSITY_MAP_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceFragmentDensityMapFeaturesEXT &v ) {
    auto root = nlohmann::json::object();
    root[ "fragmentDensityMap" ] = bool( v.fragmentDensityMap );
    root[ "fragmentDensityMapDynamic" ] = bool( v.fragmentDensityMapDynamic );
    root[ "fragmentDensityMapNonSubsampledImages" ] = bool( v.fragmentDensityMapNonSubsampledImages );
    return root;
  }
#endif
#ifdef VK_NV_FRAGMENT_SHADER_BARYCENTRIC_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceFragmentShaderBarycentricFeaturesNV &v ) {
    auto root = nlohmann::json::object();
    root[ "fragmentShaderBarycentric" ] = bool( v.fragmentShaderBarycentric );
    return root;
  }
#endif
#ifdef VK_EXT_FRAGMENT_SHADER_INTERLOCK_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceFragmentShaderInterlockFeaturesEXT &v ) {
    auto root = nlohmann::json::object();
    root[ "fragmentShaderSampleInterlock" ] = bool( v.fragmentShaderSampleInterlock );
    root[ "fragmentShaderPixelInterlock" ] = bool( v.fragmentShaderPixelInterlock );
    root[ "fragmentShaderShadingRateInterlock" ] = bool( v.fragmentShaderShadingRateInterlock );
    return root;
  }
#endif
#ifdef VK_NV_FRAGMENT_SHADING_RATE_ENUMS_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceFragmentShadingRateEnumsFeaturesNV &v ) {
    auto root = nlohmann::json::object();
    root[ "fragmentShadingRateEnums" ] = bool( v.fragmentShadingRateEnums );
    root[ "supersampleFragmentShadingRates" ] = bool( v.supersampleFragmentShadingRates );
    root[ "noInvocationFragmentShadingRates" ] = bool( v.noInvocationFragmentShadingRates );
    return root;
  }
#endif
#ifdef VK_KHR_FRAGMENT_SHADING_RATE_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceFragmentShadingRateFeaturesKHR &v ) {
    auto root = nlohmann::json::object();
    root[ "pipelineFragmentShadingRate" ] = bool( v.pipelineFragmentShadingRate );
    root[ "primitiveFragmentShadingRate" ] = bool( v.primitiveFragmentShadingRate );
    root[ "attachmentFragmentShadingRate" ] = bool( v.attachmentFragmentShadingRate );
    return root;
  }
#endif
#ifdef VK_EXT_GLOBAL_PRIORITY_QUERY_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceGlobalPriorityQueryFeaturesEXT &v ) {
    auto root = nlohmann::json::object();
    root[ "globalPriorityQuery" ] = bool( v.globalPriorityQuery );
    return root;
  }
#endif
#if defined(VK_VERSION_1_2) || defined(VK_EXT_HOST_QUERY_RESET_EXTENSION_NAME)
#ifdef VK_VERSION_1_2
  nlohmann::json to_json( const vk::PhysicalDeviceHostQueryResetFeatures &v ) {
#elif defined(VK_EXT_HOST_QUERY_RESET_EXTENSION_NAME)
  nlohmann::json to_json( const vk::PhysicalDeviceHostQueryResetFeaturesEXT &v ) {
#endif
    auto root = nlohmann::json::object();
    root[ "hostQueryReset" ] = bool( v.hostQueryReset );
    return root;
  }
#endif
#ifdef VK_EXT_IMAGE_ROBUSTNESS_EXTENSION_NAME 
  nlohmann::json to_json( const vk::PhysicalDeviceImageRobustnessFeaturesEXT &v ) {
    auto root = nlohmann::json::object();
    root[ "robustImageAccess" ] = bool( v.robustImageAccess );
    return root;
  }
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_IMAGELESS_FRAMEBUFFER_EXTENSION_NAME)
#ifdef VK_VERSION_1_2
  nlohmann::json to_json( const vk::PhysicalDeviceImagelessFramebufferFeatures &v ) {
#elif defined(VK_KHR_IMAGELESS_FRAMEBUFFER_EXTENSION_NAME)
  nlohmann::json to_json( const vk::PhysicalDeviceImagelessFramebufferFeaturesKHR &v ) {
#endif
    auto root = nlohmann::json::object();
    root[ "imagelessFramebuffer" ] = bool( v.imagelessFramebuffer );
    return root;
  }
#endif
#ifdef VK_EXT_INDEX_TYPE_UINT8_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceIndexTypeUint8FeaturesEXT &v ) {
    auto root = nlohmann::json::object();
    root[ "indexTypeUint8" ] = bool( v.indexTypeUint8 );
    return root;
  }
#endif
#ifdef VK_NV_INHERITED_VIEWPORT_SCISSOR_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceInheritedViewportScissorFeaturesNV &v ) {
    auto root = nlohmann::json::object();
    root[ "inheritedViewportScissor2D" ] = bool( v.inheritedViewportScissor2D );
    return root;
  }
#endif
#ifdef VK_EXT_INLINE_UNIFORM_BLOCK_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceInlineUniformBlockFeaturesEXT &v ) {
    auto root = nlohmann::json::object();
    root[ "inlineUniformBlock" ] = bool( v.inlineUniformBlock );
    root[ "descriptorBindingInlineUniformBlockUpdateAfterBind" ] = bool( v.descriptorBindingInlineUniformBlockUpdateAfterBind );
    return root;
  }
#endif
#ifdef VK_HUAWEI_INVOCATION_MASK_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceInvocationMaskFeaturesHUAWEI &v ) {
    auto root = nlohmann::json::object();
    root[ "invocationMask" ] = bool( v.invocationMask );
    return root;
  }
#endif
#ifdef VK_EXT_LINE_RASTERIZATION_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceLineRasterizationFeaturesEXT &v ) {
    auto root = nlohmann::json::object();
    root[ "rectangularLines" ] = bool( v.rectangularLines );
    root[ "bresenhamLines" ] = bool( v.bresenhamLines );
    root[ "smoothLines" ] = bool( v.smoothLines );
    root[ "stippledRectangularLines" ] = bool( v.stippledRectangularLines );
    root[ "stippledBresenhamLines" ] = bool( v.stippledBresenhamLines );
    root[ "stippledSmoothLines" ] = bool( v.stippledSmoothLines );
    return root;
  }
#endif
#ifdef VK_KHR_MAINTENANCE_4_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceMaintenance4FeaturesKHR &v ) {
    auto root = nlohmann::json::object();
    root[ "maintenance4" ] = bool( v.maintenance4 );
    return root;
  }
#endif
#ifdef VK_EXT_MEMORY_PRIORITY_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceMemoryPriorityFeaturesEXT &v ) {
    auto root = nlohmann::json::object();
    root[ "memoryPriority" ] = bool( v.memoryPriority );
    return root;
  }
#endif
#ifdef VK_NV_MESH_SHADER_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceMeshShaderFeaturesNV &v ) {
    auto root = nlohmann::json::object();
    root[ "taskShader" ] = bool( v.taskShader );
    root[ "meshShader" ] = bool( v.meshShader );
    return root;
  }
#endif
#ifdef VK_EXT_MULTI_DRAW_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceMultiDrawFeaturesEXT &v ) {
    auto root = nlohmann::json::object();
    root[ "multiDraw" ] = bool( v.multiDraw );
    return root;
  }
#endif
#if defined(VK_VERSION_1_1) || defined(VK_KHR_MULTIVIEW_EXTENSION_NAME)
#ifdef VK_VERSION_1_1
  nlohmann::json to_json( const vk::PhysicalDeviceMultiviewFeatures &v ) {
#elif defined(VK_KHR_MULTIVIEW_EXTENSION_NAME)
  nlohmann::json to_json( const vk::PhysicalDeviceMultiviewFeaturesKHR &v ) {
#endif
    auto root = nlohmann::json::object();
    root[ "multiview" ] = bool( v.multiview );
    root[ "multiviewGeometryShader" ] = bool( v.multiviewGeometryShader );
    root[ "multiviewTessellationShader" ] = bool( v.multiviewTessellationShader );
    return root;
  }
#endif
#ifdef VK_VALVE_MUTABLE_DESCRIPTOR_TYPE_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceMutableDescriptorTypeFeaturesVALVE &v ) {
    auto root = nlohmann::json::object();
    root[ "mutableDescriptorType" ] = bool( v.mutableDescriptorType );
    return root;
  }
#endif
#ifdef VK_EXT_PAGEABLE_DEVICE_LOCAL_MEMORY_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDevicePageableDeviceLocalMemoryFeaturesEXT &v ) {
    auto root = nlohmann::json::object();
    root[ "pageableDeviceLocalMemory" ] = bool( v.pageableDeviceLocalMemory );
    return root;
  }
#endif
#ifdef VK_KHR_PERFORMANCE_QUERY_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDevicePerformanceQueryFeaturesKHR &v ) {
    auto root = nlohmann::json::object();
    root[ "performanceCounterQueryPools" ] = bool( v.performanceCounterQueryPools );
    root[ "performanceCounterMultipleQueryPools" ] = bool( v.performanceCounterMultipleQueryPools );
    return root;
  }
#endif
#ifdef VK_EXT_PIPELINE_CREATION_CACHE_CONTROL_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDevicePipelineCreationCacheControlFeaturesEXT &v ) {
    auto root = nlohmann::json::object();
    root[ "pipelineCreationCacheControl" ] = bool( v.pipelineCreationCacheControl );
    return root;
  }
#endif
#ifdef VK_KHR_PIPELINE_EXECUTABLE_PROPERTIES_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDevicePipelineExecutablePropertiesFeaturesKHR &v ) {
    auto root = nlohmann::json::object();
    root[ "pipelineExecutableInfo" ] = bool( v.pipelineExecutableInfo );
    return root;
  }
#endif
#ifdef VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDevicePortabilitySubsetFeaturesKHR &v ) {
    auto root = nlohmann::json::object();
    root[ "constantAlphaColorBlendFactors" ] = bool( v.constantAlphaColorBlendFactors );
    root[ "events" ] = bool( v.events );
    root[ "imageViewFormatReinterpretation" ] = bool( v.imageViewFormatReinterpretation );
    root[ "imageViewFormatSwizzle" ] = bool( v.imageViewFormatSwizzle );
    root[ "imageView2DOn3DImage" ] = bool( v.imageView2DOn3DImage );
    root[ "multisampleArrayImage" ] = bool( v.multisampleArrayImage );
    root[ "mutableComparisonSamplers" ] = bool( v.mutableComparisonSamplers );
    root[ "pointPolygons" ] = bool( v.pointPolygons );
    root[ "samplerMipLodBias" ] = bool( v.samplerMipLodBias );
    root[ "separateStencilMaskRef" ] = bool( v.separateStencilMaskRef );
    root[ "shaderSampleRateInterpolationFunctions" ] = bool( v.shaderSampleRateInterpolationFunctions );
    root[ "tessellationIsolines" ] = bool( v.tessellationIsolines );
    root[ "tessellationPointMode" ] = bool( v.tessellationPointMode );
    root[ "triangleFans" ] = bool( v.triangleFans );
    root[ "vertexAttributeAccessBeyondStride" ] = bool( v.vertexAttributeAccessBeyondStride );
    return root;
  }
#endif
#ifdef VK_KHR_PRESENT_ID_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDevicePresentIdFeaturesKHR &v ) {
    auto root = nlohmann::json::object();
    root[ "presentId" ] = bool( v.presentId );
    return root;
  }
#endif
#ifdef VK_KHR_PRESENT_WAIT_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDevicePresentWaitFeaturesKHR &v ) {
    auto root = nlohmann::json::object();
    root[ "presentWait" ] = bool( v.presentWait );
    return root;
  }
#endif
#ifdef VK_EXT_PRIMITIVE_TOPOLOGY_LIST_RESTART_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDevicePrimitiveTopologyListRestartFeaturesEXT &v ) {
    auto root = nlohmann::json::object();
    root[ "primitiveTopologyListRestart" ] = bool( v.primitiveTopologyListRestart );
    root[ "primitiveTopologyPatchListRestart" ] = bool( v.primitiveTopologyPatchListRestart );
    return root;
  }
#endif
#ifdef VK_EXT_PRIVATE_DATA_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDevicePrivateDataFeaturesEXT &v ) {
    auto root = nlohmann::json::object();
    root[ "privateData" ] = bool( v.privateData );
    return root;
  }
#endif
#ifdef VK_VERSION_1_1
  nlohmann::json to_json( const vk::PhysicalDeviceProtectedMemoryFeatures &v ) {
    auto root = nlohmann::json::object();
    root[ "protectedMemory" ] = bool( v.protectedMemory );
    return root;
  }
#endif
#ifdef VK_EXT_PROVOKING_VERTEX_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceProvokingVertexFeaturesEXT &v ) {
    auto root = nlohmann::json::object();
    root[ "provokingVertexLast" ] = bool( v.provokingVertexLast );
    root[ "transformFeedbackPreservesProvokingVertex" ] = bool( v.transformFeedbackPreservesProvokingVertex );
    return root;
  }
#endif
#ifdef VK_EXT_RGBA10X6_FORMATS_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceRGBA10X6FormatsFeaturesEXT &v ) {
    auto root = nlohmann::json::object();
    root[ "formatRgba10x6WithoutYCbCrSampler" ] = bool( v.formatRgba10x6WithoutYCbCrSampler );
    return root;
  }
#endif
#ifdef VK_KHR_RAY_QUERY_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceRayQueryFeaturesKHR &v ) {
    auto root = nlohmann::json::object();
    root[ "rayQuery" ] = bool( v.rayQuery );
    return root;
  }
#endif
#ifdef VK_NV_RAY_TRACING_MOTION_BLUR_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceRayTracingMotionBlurFeaturesNV &v ) {
    auto root = nlohmann::json::object();
    root[ "rayTracingMotionBlur" ] = bool( v.rayTracingMotionBlur );
    root[ "rayTracingMotionBlurPipelineTraceRaysIndirect" ] = bool( v.rayTracingMotionBlurPipelineTraceRaysIndirect );
    return root;
  }
#endif
#ifdef VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceRayTracingPipelineFeaturesKHR &v ) {
    auto root = nlohmann::json::object();
    root[ "rayTracingPipeline" ] = bool( v.rayTracingPipeline );
    root[ "rayTracingPipelineShaderGroupHandleCaptureReplay" ] = bool( v.rayTracingPipelineShaderGroupHandleCaptureReplay );
    root[ "rayTracingPipelineShaderGroupHandleCaptureReplayMixed" ] = bool( v.rayTracingPipelineShaderGroupHandleCaptureReplayMixed );
    root[ "rayTracingPipelineTraceRaysIndirect" ] = bool( v.rayTracingPipelineTraceRaysIndirect );
    root[ "rayTraversalPrimitiveCulling" ] = bool( v.rayTraversalPrimitiveCulling );
    return root;
  }
#endif
#ifdef VK_NV_REPRESENTATIVE_FRAGMENT_TEST_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceRepresentativeFragmentTestFeaturesNV &v ) {
    auto root = nlohmann::json::object();
    root[ "representativeFragmentTest" ] = bool( v.representativeFragmentTest );
    return root;
  }
#endif
#ifdef VK_EXT_ROBUSTNESS_2_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceRobustness2FeaturesEXT &v ) {
    auto root = nlohmann::json::object();
    root[ "robustBufferAccess2" ] = bool( v.robustBufferAccess2 );
    root[ "robustImageAccess2" ] = bool( v.robustImageAccess2 );
    root[ "nullDescriptor" ] = bool( v.nullDescriptor );
    return root;
  }
#endif
#if defined(VK_VERSION_1_1) || defined(VK_KHR_SAMPLER_YCBCR_CONVERSION_EXTENSION_NAME)
#ifdef VK_VERSION_1_1
  nlohmann::json to_json( const vk::PhysicalDeviceSamplerYcbcrConversionFeatures &v ) {
#elif defined(VK_KHR_SAMPLER_YCBCR_CONVERSION_EXTENSION_NAME)
  nlohmann::json to_json( const vk::PhysicalDeviceSamplerYcbcrConversionFeaturesKHR &v ) {
#endif
    auto root = nlohmann::json::object();
    root[ "samplerYcbcrConversion" ] = bool( v.samplerYcbcrConversion );
    return root;
  }
#endif
#if defined(VK_VERSION_1_2) || defined(VK_EXT_SCALAR_BLOCK_LAYOUT_EXTENSION_NAME)
#ifdef VK_VERSION_1_2
  nlohmann::json to_json( const vk::PhysicalDeviceScalarBlockLayoutFeatures &v ) {
#elif defined(VK_EXT_SCALAR_BLOCK_LAYOUT_EXTENSION_NAME)
  nlohmann::json to_json( const vk::PhysicalDeviceScalarBlockLayoutFeaturesEXT &v ) {
#endif
    auto root = nlohmann::json::object();
    root[ "scalarBlockLayout" ] = bool( v.scalarBlockLayout );
    return root;
  }
#endif
#if defined(scalar_block_layout) || defined(VK_KHR_SEPARATE_DEPTH_STENCIL_LAYOUTS_EXTENSION_NAME)
#ifdef VK_VERSION_1_2
  nlohmann::json to_json( const vk::PhysicalDeviceSeparateDepthStencilLayoutsFeatures &v ) {
#elif defined(VK_KHR_SEPARATE_DEPTH_STENCIL_LAYOUTS_EXTENSION_NAME)
  nlohmann::json to_json( const vk::PhysicalDeviceSeparateDepthStencilLayoutsFeaturesKHR &v ) {
#endif
    auto root = nlohmann::json::object();
    root[ "separateDepthStencilLayouts" ] = bool( v.separateDepthStencilLayouts );
    return root;
  }
#endif
#ifdef VK_EXT_SHADER_ATOMIC_FLOAT_2_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceSeparateDepthStencilLayoutsFeatures &v ) {
    auto root = nlohmann::json::object();
    root[ "shaderBufferFloat16Atomics" ] = bool( v.shaderBufferFloat16Atomics );
    root[ "shaderBufferFloat16AtomicAdd" ] = bool( v.shaderBufferFloat16AtomicAdd );
    root[ "shaderBufferFloat16AtomicMinMax" ] = bool( v.shaderBufferFloat16AtomicMinMax );
    root[ "shaderBufferFloat32AtomicMinMax" ] = bool( v.shaderBufferFloat32AtomicMinMax );
    root[ "shaderBufferFloat64AtomicMinMax" ] = bool( v.shaderBufferFloat64AtomicMinMax );
    root[ "shaderSharedFloat16Atomics" ] = bool( v.shaderSharedFloat16Atomics );
    root[ "shaderSharedFloat16AtomicAdd" ] = bool( v.shaderSharedFloat16AtomicAdd );
    root[ "shaderSharedFloat16AtomicMinMax" ] = bool( v.shaderSharedFloat16AtomicMinMax );
    root[ "shaderSharedFloat32AtomicMinMax" ] = bool( v.shaderSharedFloat32AtomicMinMax );
    root[ "shaderSharedFloat64AtomicMinMax" ] = bool( v.shaderSharedFloat64AtomicMinMax );
    root[ "shaderImageFloat32AtomicMinMax" ] = bool( v.shaderImageFloat32AtomicMinMax );
    root[ "sparseImageFloat32AtomicMinMax" ] = bool( v.sparseImageFloat32AtomicMinMax );
    return root;
  }
#endif
#ifdef VK_EXT_SHADER_ATOMIC_FLOAT_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceShaderAtomicFloatFeaturesEXT &v ) {
    auto root = nlohmann::json::object();
    root[ "shaderBufferFloat32Atomics" ] = bool( v.shaderBufferFloat32Atomics );
    root[ "shaderBufferFloat32AtomicAdd" ] = bool( v.shaderBufferFloat32AtomicAdd );
    root[ "shaderBufferFloat64Atomics" ] = bool( v.shaderBufferFloat64Atomics );
    root[ "shaderBufferFloat64AtomicAdd" ] = bool( v.shaderBufferFloat64AtomicAdd );
    root[ "shaderSharedFloat32Atomics" ] = bool( v.shaderSharedFloat32Atomics );
    root[ "shaderSharedFloat32AtomicAdd" ] = bool( v.shaderSharedFloat32AtomicAdd );
    root[ "shaderSharedFloat64Atomics" ] = bool( v.shaderSharedFloat64Atomics );
    root[ "shaderSharedFloat64AtomicAdd" ] = bool( v.shaderSharedFloat64AtomicAdd );
    root[ "shaderImageFloat32Atomics" ] = bool( v.shaderImageFloat32Atomics );
    root[ "shaderImageFloat32AtomicAdd" ] = bool( v.shaderImageFloat32AtomicAdd );
    root[ "sparseImageFloat32Atomics" ] = bool( v.sparseImageFloat32Atomics );
    root[ "sparseImageFloat32AtomicAdd" ] = bool( v.sparseImageFloat32AtomicAdd );
    return root;
  }
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_SHADER_ATOMIC_INT64_EXTENSION_NAME)
#ifdef VK_VERSION_1_2
  nlohmann::json to_json( const vk::PhysicalDeviceShaderAtomicInt64Features &v ) {
#elif defined(VK_KHR_SHADER_ATOMIC_INT64_EXTENSION_NAME)
  nlohmann::json to_json( const vk::PhysicalDeviceShaderAtomicInt64FeaturesKHR &v ) {
#endif
    auto root = nlohmann::json::object();
    root[ "shaderBufferInt64Atomics" ] = bool( v.shaderBufferInt64Atomics );
    root[ "shaderSharedInt64Atomics" ] = bool( v.shaderSharedInt64Atomics );
    return root;
  }
#endif
#ifdef VK_KHR_SHADER_CLOCK_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceShaderClockFeaturesKHR &v ) {
    auto root = nlohmann::json::object();
    root[ "shaderSubgroupClock" ] = bool( v.shaderSubgroupClock );
    root[ "shaderDeviceClock" ] = bool( v.shaderDeviceClock );
    return root;
  }
#endif
#ifdef VK_EXT_SHADER_DEMOTE_TO_HELPER_INVOCATION_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceShaderDemoteToHelperInvocationFeaturesEXT &v ) {
    auto root = nlohmann::json::object();
    root[ "shaderDemoteToHelperInvocation" ] = bool( v.shaderDemoteToHelperInvocation );
    return root;
  }
#endif
#ifdef VK_VERSION_1_1
  nlohmann::json to_json( const vk::PhysicalDeviceShaderDrawParametersFeatures &v ) {
    auto root = nlohmann::json::object();
    root[ "shaderDrawParameters" ] = bool( v.shaderDrawParameters );
    return root;
  }
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_SHADER_FLOAT16_INT8_EXTENSION_NAME)
#ifdef VK_VERSION_1_2
  nlohmann::json to_json( const vk::PhysicalDeviceShaderFloat16Int8Features &v ) {
#elif defined(VK_KHR_SHADER_FLOAT16_INT8_EXTENSION_NAME)
  nlohmann::json to_json( const vk::PhysicalDeviceShaderFloat16Int8Features &v ) {
#endif
    auto root = nlohmann::json::object();
    root[ "shaderFloat16" ] = bool( v.shaderFloat16 );
    root[ "shaderInt8" ] = bool( v.shaderInt8 );
    return root;
  }
#endif
#ifdef VK_EXT_SHADER_IMAGE_ATOMIC_INT64_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceShaderImageAtomicInt64FeaturesEXT &v ) {
    auto root = nlohmann::json::object();
    root[ "shaderImageInt64Atomics" ] = bool( v.shaderImageInt64Atomics );
    root[ "sparseImageInt64Atomics" ] = bool( v.sparseImageInt64Atomics );
    return root;
  }
#endif
#ifdef VK_NV_SHADER_IMAGE_FOOTPRINT_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceShaderImageFootprintFeaturesNV &v ) {
    auto root = nlohmann::json::object();
    root[ "imageFootprint" ] = bool( v.imageFootprint );
    return root;
  }
#endif
#ifdef VK_KHR_SHADER_INTEGER_DOT_PRODUCT_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceShaderIntegerDotProductFeaturesKHR &v ) {
    auto root = nlohmann::json::object();
    root[ "shaderIntegerDotProduct" ] = bool( v.shaderIntegerDotProduct );
    return root;
  }
#endif
#ifdef VK_INTEL_SHADER_INTEGER_FUNCTIONS_2_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceShaderIntegerFunctions2FeaturesINTEL &v ) {
    auto root = nlohmann::json::object();
    root[ "shaderIntegerFunctions2" ] = bool( v.shaderIntegerFunctions2 );
    return root;
  }
#endif
#ifdef VK_NV_SHADER_SM_BUILTINS_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceShaderSMBuiltinsFeaturesNV &v ) {
    auto root = nlohmann::json::object();
    root[ "shaderSMBuiltins" ] = bool( v.shaderSMBuiltins );
    return root;
  }
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_SHADER_SUBGROUP_EXTENDED_TYPES_EXTENSION_NAME)
#ifdef VK_VERSION_1_2
  nlohmann::json to_json( const vk::PhysicalDeviceShaderSubgroupExtendedTypesFeatures &v ) {
#elif defined(VK_KHR_SHADER_SUBGROUP_EXTENDED_TYPES_EXTENSION_NAME)
  nlohmann::json to_json( const vk::PhysicalDeviceShaderSubgroupExtendedTypesFeaturesKHR &v ) {
#endif
    auto root = nlohmann::json::object();
    root[ "shaderSubgroupExtendedTypes" ] = bool( v.shaderSubgroupExtendedTypes );
    return root;
  }
#endif
#ifdef VK_KHR_SHADER_SUBGROUP_UNIFORM_CONTROL_FLOW_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceShaderSubgroupUniformControlFlowFeaturesKHR &v ) {
    auto root = nlohmann::json::object();
    root[ "shaderSubgroupUniformControlFlow" ] = bool( v.shaderSubgroupUniformControlFlow );
    return root;
  }
#endif
#ifdef VK_KHR_SHADER_TERMINATE_INVOCATION_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceShaderTerminateInvocationFeaturesKHR &v ) {
    auto root = nlohmann::json::object();
    root[ "shaderTerminateInvocation" ] = bool( v.shaderTerminateInvocation );
    return root;
  }
#endif
#ifdef VK_NV_SHADING_RATE_IMAGE_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceShadingRateImageFeaturesNV &v ) {
    auto root = nlohmann::json::object();
    root[ "shadingRateImage" ] = bool( v.shadingRateImage );
    root[ "shadingRateCoarseSampleOrder" ] = bool( v.shadingRateCoarseSampleOrder );
    return root;
  }
#endif
#ifdef VK_EXT_SUBGROUP_SIZE_CONTROL_EXTENSION_NAME 
  nlohmann::json to_json( const vk::PhysicalDeviceSubgroupSizeControlFeaturesEXT &v ) {
    auto root = nlohmann::json::object();
    root[ "subgroupSizeControl" ] = bool( v.subgroupSizeControl );
    root[ "computeFullSubgroups" ] = bool( v.computeFullSubgroups );
    return root;
  }
#endif
#ifdef VK_HUAWEI_SUBPASS_SHADING_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceSubpassShadingFeaturesHUAWEI &v ) {
    auto root = nlohmann::json::object();
    root[ "subpassShading" ] = bool( v.subpassShading );
    return root;
  }
#endif
#ifdef VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceSynchronization2FeaturesKHR &v ) {
    auto root = nlohmann::json::object();
    root[ "synchronization2" ] = bool( v.synchronization2 );
    return root;
  }
#endif
#ifdef VK_EXT_TEXEL_BUFFER_ALIGNMENT_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceTexelBufferAlignmentFeaturesEXT &v ) {
    auto root = nlohmann::json::object();
    root[ "texelBufferAlignment" ] = bool( v.texelBufferAlignment );
    return root;
  }
#endif
#ifdef VK_EXT_TEXTURE_COMPRESSION_ASTC_HDR_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceTextureCompressionASTCHDRFeaturesEXT &v ) {
    auto root = nlohmann::json::object();
    root[ "textureCompressionASTC_HDR" ] = bool( v.textureCompressionASTC_HDR );
    return root;
  }
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME)
#ifdef VK_VERSION_1_2
  nlohmann::json to_json( const vk::PhysicalDeviceTimelineSemaphoreFeatures &v ) {
#elif defined(VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME)
  nlohmann::json to_json( const vk::PhysicalDeviceTimelineSemaphoreFeaturesKHR &v ) {
#endif
    auto root = nlohmann::json::object();
    root[ "timelineSemaphore" ] = bool( v.timelineSemaphore );
    return root;
  }
#endif
#ifdef VK_EXT_TRANSFORM_FEEDBACK_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceTransformFeedbackFeaturesEXT &v ) {
    auto root = nlohmann::json::object();
    root[ "transformFeedback" ] = bool( v.transformFeedback );
    root[ "geometryStreams" ] = bool( v.geometryStreams );
    return root;
  }
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_UNIFORM_BUFFER_STANDARD_LAYOUT_EXTENSION_NAME)
#ifdef VK_VERSION_1_2
  nlohmann::json to_json( const vk::PhysicalDeviceUniformBufferStandardLayoutFeatures &v ) {
#elif defined(VK_KHR_UNIFORM_BUFFER_STANDARD_LAYOUT_EXTENSION_NAME)
  nlohmann::json to_json( const vk::PhysicalDeviceUniformBufferStandardLayoutFeaturesKHR &v ) {
#endif
    auto root = nlohmann::json::object();
    root[ "uniformBufferStandardLayout" ] = bool( v.uniformBufferStandardLayout );
    return root;
  }
#endif
#if defined(VK_VERSION_1_1) || defined(VK_KHR_VARIABLE_POINTERS_EXTENSION_NAME)
#ifdef VK_VERSION_1_1
  nlohmann::json to_json( const vk::PhysicalDeviceVariablePointersFeatures &v ) {
#elif defined(VK_KHR_VARIABLE_POINTERS_EXTENSION_NAME)
  nlohmann::json to_json( const vk::PhysicalDeviceVariablePointersFeaturesKHR &v ) {
#endif
    auto root = nlohmann::json::object();
    root[ "variablePointersStorageBuffer" ] = bool( v.variablePointersStorageBuffer );
    root[ "variablePointers" ] = bool( v.variablePointers );
    return root;
  }
#endif
#ifdef VK_EXT_VERTEX_ATTRIBUTE_DIVISOR_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceVertexAttributeDivisorFeaturesEXT &v ) {
    auto root = nlohmann::json::object();
    root[ "vertexAttributeInstanceRateDivisor" ] = bool( v.vertexAttributeInstanceRateDivisor );
    root[ "vertexAttributeInstanceRateZeroDivisor" ] = bool( v.vertexAttributeInstanceRateZeroDivisor );
    return root;
  }
#endif
#ifdef VK_EXT_VERTEX_INPUT_DYNAMIC_STATE_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceVertexInputDynamicStateFeaturesEXT &v ) {
    auto root = nlohmann::json::object();
    root[ "vertexInputDynamicState" ] = bool( v.vertexInputDynamicState );
    return root;
  }
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_VULKAN_MEMORY_MODEL_EXTENSION_NAME)
#ifdef VK_VERSION_1_2
  nlohmann::json to_json( const vk::PhysicalDeviceVulkanMemoryModelFeatures &v ) {
#elif VK_KHR_VULKAN_MEMORY_MODEL_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceVulkanMemoryModelFeaturesKHR &v ) {
#endif
    auto root = nlohmann::json::object();
    root[ "vulkanMemoryModel" ] = bool( v.vulkanMemoryModel );
    root[ "vulkanMemoryModelDeviceScope" ] = bool( v.vulkanMemoryModelDeviceScope );
    root[ "vulkanMemoryModelAvailabilityVisibilityChains" ] = bool( v.vulkanMemoryModelAvailabilityVisibilityChains );
    return root;
  }
#endif
#ifdef VK_KHR_WORKGROUP_MEMORY_EXPLICIT_LAYOUT_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceWorkgroupMemoryExplicitLayoutFeaturesKHR &v ) {
    auto root = nlohmann::json::object();
    root[ "workgroupMemoryExplicitLayout" ] = bool( v.workgroupMemoryExplicitLayout );
    root[ "workgroupMemoryExplicitLayoutScalarBlockLayout" ] = bool( v.workgroupMemoryExplicitLayoutScalarBlockLayout );
    root[ "workgroupMemoryExplicitLayout8BitAccess" ] = bool( v.workgroupMemoryExplicitLayout8BitAccess );
    root[ "workgroupMemoryExplicitLayout16BitAccess" ] = bool( v.workgroupMemoryExplicitLayout16BitAccess );
    return root;
  }
#endif
#ifdef VK_EXT_YCBCR_2PLANE_444_FORMATS_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceYcbcr2Plane444FormatsFeaturesEXT &v ) {
    auto root = nlohmann::json::object();
    root[ "ycbcr2plane444Formats" ] = bool( v.ycbcr2plane444Formats );
    return root;
  }
#endif
#ifdef VK_EXT_YCBCR_IMAGE_ARRAYS_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceYcbcrImageArraysFeaturesEXT &v ) {
    auto root = nlohmann::json::object();
    root[ "ycbcrImageArrays" ] = bool( v.ycbcrImageArrays );
    return root;
  }
#endif
#ifdef VK_KHR_ZERO_INITIALIZE_WORKGROUP_MEMORY_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceZeroInitializeWorkgroupMemoryFeaturesKHR &v ) {
    auto root = nlohmann::json::object();
    root[ "shaderZeroInitializeWorkgroupMemory" ] = bool( v.shaderZeroInitializeWorkgroupMemory );
    return root;
  }
#endif
  nlohmann::json to_json( const physical_device_features_t &v ) {
    auto root = nlohmann::json::object();
    root[ "basic" ] = to_json( v.get_basic() );
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
    return root;
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
