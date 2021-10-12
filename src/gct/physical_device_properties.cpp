#include <gct/to_json.hpp>
#include <gct/instance.hpp>
#include <gct/physical_device.hpp>
#include <gct/physical_device_properties.hpp>
#include <gct/queue_family_properties.hpp>

namespace gct {
  nlohmann::json to_json( const vk::PhysicalDeviceLimits &v ) {
    auto root = nlohmann::json::object();
    root[ "maxImageDimension1D" ] = v.maxImageDimension1D;
    root[ "maxImageDimension2D" ] = v.maxImageDimension2D;
    root[ "maxImageDimension3D" ] = v.maxImageDimension3D;
    root[ "maxImageDimensionCube" ] = v.maxImageDimensionCube;
    root[ "maxImageArrayLayers" ] = v.maxImageArrayLayers;
    root[ "maxTexelBufferElements" ] = v.maxTexelBufferElements;
    root[ "maxUniformBufferRange" ] = v.maxUniformBufferRange;
    root[ "maxStorageBufferRange" ] = v.maxStorageBufferRange;
    root[ "maxPushConstantsSize" ] = v.maxPushConstantsSize;
    root[ "maxMemoryAllocationCount" ] = v.maxMemoryAllocationCount;
    root[ "maxSamplerAllocationCount" ] = v.maxSamplerAllocationCount;
    root[ "bufferImageGranularity" ] = v.bufferImageGranularity;
    root[ "sparseAddressSpaceSize" ] = v.sparseAddressSpaceSize;
    root[ "maxBoundDescriptorSets" ] = v.maxBoundDescriptorSets;
    root[ "maxPerStageDescriptorSamplers" ] = v.maxPerStageDescriptorSamplers;
    root[ "maxPerStageDescriptorUniformBuffers" ] = v.maxPerStageDescriptorUniformBuffers;
    root[ "maxPerStageDescriptorStorageBuffers" ] = v.maxPerStageDescriptorStorageBuffers;
    root[ "maxPerStageDescriptorSampledImages" ] = v.maxPerStageDescriptorSampledImages;
    root[ "maxPerStageDescriptorStorageImages" ] = v.maxPerStageDescriptorStorageImages;
    root[ "maxPerStageDescriptorInputAttachments" ] = v.maxPerStageDescriptorInputAttachments;
    root[ "maxPerStageResources" ] = v.maxPerStageResources;
    root[ "maxDescriptorSetSamplers" ] = v.maxDescriptorSetSamplers;
    root[ "maxDescriptorSetUniformBuffers" ] = v.maxDescriptorSetUniformBuffers;
    root[ "maxDescriptorSetUniformBuffersDynamic" ] = v.maxDescriptorSetUniformBuffersDynamic;
    root[ "maxDescriptorSetStorageBuffers" ] = v.maxDescriptorSetStorageBuffers;
    root[ "maxDescriptorSetStorageBuffersDynamic" ] = v.maxDescriptorSetStorageBuffersDynamic;
    root[ "maxDescriptorSetSampledImages" ] = v.maxDescriptorSetSampledImages;
    root[ "maxDescriptorSetStorageImages" ] = v.maxDescriptorSetStorageImages;
    root[ "maxDescriptorSetInputAttachments" ] = v.maxDescriptorSetInputAttachments;
    root[ "maxVertexInputAttributes" ] = v.maxVertexInputAttributes;
    root[ "maxVertexInputBindings" ] = v.maxVertexInputBindings;
    root[ "maxVertexInputAttributeOffset" ] = v.maxVertexInputAttributeOffset;
    root[ "maxVertexInputBindingStride" ] = v.maxVertexInputBindingStride;
    root[ "maxVertexOutputComponents" ] = v.maxVertexOutputComponents;
    root[ "maxTessellationGenerationLevel" ] = v.maxTessellationGenerationLevel;
    root[ "maxTessellationPatchSize" ] = v.maxTessellationPatchSize;
    root[ "maxTessellationControlPerVertexInputComponents" ] = v.maxTessellationControlPerVertexInputComponents;
    root[ "maxTessellationControlPerVertexOutputComponents" ] = v.maxTessellationControlPerVertexOutputComponents;
    root[ "maxTessellationControlPerPatchOutputComponents" ] = v.maxTessellationControlPerPatchOutputComponents;
    root[ "maxTessellationControlTotalOutputComponents" ] = v.maxTessellationControlTotalOutputComponents;
    root[ "maxTessellationEvaluationInputComponents" ] = v.maxTessellationEvaluationInputComponents;
    root[ "maxTessellationEvaluationOutputComponents" ] = v.maxTessellationEvaluationOutputComponents;
    root[ "maxGeometryShaderInvocations" ] = v.maxGeometryShaderInvocations;
    root[ "maxGeometryInputComponents" ] = v.maxGeometryInputComponents;
    root[ "maxGeometryOutputComponents" ] = v.maxGeometryOutputComponents;
    root[ "maxGeometryOutputVertices" ] = v.maxGeometryOutputVertices;
    root[ "maxGeometryTotalOutputComponents" ] = v.maxGeometryTotalOutputComponents;
    root[ "maxFragmentInputComponents" ] = v.maxFragmentInputComponents;
    root[ "maxFragmentOutputAttachments" ] = v.maxFragmentOutputAttachments;
    root[ "maxFragmentDualSrcAttachments" ] = v.maxFragmentDualSrcAttachments;
    root[ "maxFragmentCombinedOutputResources" ] = v.maxFragmentCombinedOutputResources;
    root[ "maxComputeSharedMemorySize" ] = v.maxComputeSharedMemorySize;
    root[ "maxComputeWorkGroupCount" ] = v.maxComputeWorkGroupCount;
    root[ "maxComputeWorkGroupInvocations" ] = v.maxComputeWorkGroupInvocations;
    root[ "maxComputeWorkGroupSize" ] = v.maxComputeWorkGroupSize;
    root[ "subPixelPrecisionBits" ] = v.subPixelPrecisionBits;
    root[ "subTexelPrecisionBits" ] = v.subTexelPrecisionBits;
    root[ "mipmapPrecisionBits" ] = v.mipmapPrecisionBits;
    root[ "maxDrawIndexedIndexValue" ] = v.maxDrawIndexedIndexValue;
    root[ "maxDrawIndirectCount" ] = v.maxDrawIndirectCount;
    root[ "maxSamplerLodBias" ] = v.maxSamplerLodBias;
    root[ "maxSamplerAnisotropy" ] = v.maxSamplerAnisotropy;
    root[ "maxViewports" ] = v.maxViewports;
    root[ "maxViewportDimensions" ] = v.maxViewportDimensions;
    root[ "viewportBoundsRange" ] = v.viewportBoundsRange;
    root[ "viewportSubPixelBits" ] = v.viewportSubPixelBits;
    root[ "minMemoryMapAlignment" ] = v.minMemoryMapAlignment;
    root[ "minTexelBufferOffsetAlignment" ] = v.minTexelBufferOffsetAlignment;
    root[ "minUniformBufferOffsetAlignment" ] = v.minUniformBufferOffsetAlignment;
    root[ "minStorageBufferOffsetAlignment" ] = v.minStorageBufferOffsetAlignment;
    root[ "minTexelOffset" ] = v.minTexelOffset;
    root[ "maxTexelOffset" ] = v.maxTexelOffset;
    root[ "minTexelGatherOffset" ] = v.minTexelGatherOffset;
    root[ "maxTexelGatherOffset" ] = v.maxTexelGatherOffset;
    root[ "minInterpolationOffset" ] = v.minInterpolationOffset;
    root[ "maxInterpolationOffset" ] = v.maxInterpolationOffset;
    root[ "subPixelInterpolationOffsetBits" ] = v.subPixelInterpolationOffsetBits;
    root[ "maxFramebufferWidth" ] = v.maxFramebufferWidth;
    root[ "maxFramebufferHeight" ] = v.maxFramebufferHeight;
    root[ "maxFramebufferLayers" ] = v.maxFramebufferLayers;
    root[ "framebufferColorSampleCounts" ] = std::uint32_t( v.framebufferColorSampleCounts );
    root[ "framebufferDepthSampleCounts" ] = std::uint32_t( v.framebufferDepthSampleCounts );
    root[ "framebufferStencilSampleCounts" ] = std::uint32_t( v.framebufferStencilSampleCounts );
    root[ "framebufferNoAttachmentsSampleCounts" ] = std::uint32_t( v.framebufferNoAttachmentsSampleCounts );
    root[ "maxColorAttachments" ] = v.maxColorAttachments;
    root[ "sampledImageColorSampleCounts" ] = std::uint32_t( v.sampledImageColorSampleCounts );
    root[ "sampledImageIntegerSampleCounts" ] = std::uint32_t( v.sampledImageIntegerSampleCounts );
    root[ "sampledImageDepthSampleCounts" ] = std::uint32_t( v.sampledImageDepthSampleCounts );
    root[ "sampledImageStencilSampleCounts" ] = std::uint32_t( v.sampledImageStencilSampleCounts );
    root[ "storageImageSampleCounts" ] = std::uint32_t( v.storageImageSampleCounts );
    root[ "maxSampleMaskWords" ] = v.maxSampleMaskWords;
    root[ "timestampComputeAndGraphics" ] = v.timestampComputeAndGraphics;
    root[ "timestampPeriod" ] = v.timestampPeriod;
    root[ "maxClipDistances" ] = v.maxClipDistances;
    root[ "maxCullDistances" ] = v.maxCullDistances;
    root[ "maxCombinedClipAndCullDistances" ] = v.maxCombinedClipAndCullDistances;
    root[ "discreteQueuePriorities" ] = v.discreteQueuePriorities;
    root[ "pointSizeRange" ] = v.pointSizeRange;
    root[ "lineWidthRange" ] = v.lineWidthRange;
    root[ "pointSizeGranularity" ] = v.pointSizeGranularity;
    root[ "lineWidthGranularity" ] = v.lineWidthGranularity;
    root[ "strictLines" ] = v.strictLines;
    root[ "standardSampleLocations" ] = v.standardSampleLocations;
    root[ "optimalBufferCopyOffsetAlignment" ] = v.optimalBufferCopyOffsetAlignment;
    root[ "optimalBufferCopyRowPitchAlignment" ] = v.optimalBufferCopyRowPitchAlignment;
    root[ "nonCoherentAtomSize" ] = v.nonCoherentAtomSize;
    return root;
  }
  nlohmann::json to_json( const vk::PhysicalDeviceSparseProperties &v ) {
    auto root = nlohmann::json::object();
    root[ "residencyStandard2DBlockShape" ] = v.residencyStandard2DBlockShape;
    root[ "residencyStandard2DMultisampleBlockShape" ] = v.residencyStandard2DMultisampleBlockShape;
    root[ "residencyStandard3DBlockShape" ] = v.residencyStandard3DBlockShape;
    root[ "residencyAlignedMipSize" ] = v.residencyAlignedMipSize;
    root[ "residencyNonResidentStrict" ] = v.residencyNonResidentStrict;
    return root;
  }
  nlohmann::json to_json( const vk::PhysicalDeviceProperties &v ) {
    auto root = nlohmann::json::object();
    root[ "apiVersion" ] = v.apiVersion;
    root[ "driverVersion" ] = v.driverVersion;
    root[ "vendorID" ] = v.vendorID;
    root[ "deviceID" ] = v.deviceID;
    root[ "deviceType" ] = v.deviceType;
    root[ "deviceName" ] = v.deviceName;
    root[ "pipelineCacheUUID" ] = v.pipelineCacheUUID;
    root[ "limits" ] = to_json( v.limits );
    root[ "sparseProperties" ] = to_json( v.sparseProperties );
    return root; 
  }
  nlohmann::json to_json( const vk::LayerProperties &v ) {
    auto root = nlohmann::json::object();
    root[ "layerName" ] = v.layerName;
    root[ "specVersion" ] = v.specVersion;
    root[ "implementationVersion" ] = v.implementationVersion;
    root[ "description" ] = v.description;
    return root; 
  }
  nlohmann::json to_json( const layer_map_t &v ) {
    auto root = nlohmann::json::object();
    for( const auto &[key,value]: v )
      root[ key ] = to_json( value );
    return root; 
  }
  nlohmann::json to_json( const extension_map_t &v ) {
    auto root = nlohmann::json::object();
    for( const auto &[key,value]: v ) {
      root[ key ] = nlohmann::json::object();
      root[ key ][ "specVersion" ] = value;
    }
    return root; 
  }

#ifdef VK_KHR_DISPLAY_EXTENSION_NAME
  nlohmann::json to_json( const vk::DisplayPropertiesKHR &v ) {
    auto root = nlohmann::json::object();
    root[ "displayName" ] = v.displayName;
    root[ "physicalDimensions" ] = to_json( v.physicalDimensions );
    root[ "physicalResolution" ] = to_json( v.physicalResolution );
    root[ "supportedTransforms" ] = std::uint32_t( v.supportedTransforms );
    root[ "planeReorderPossible" ] = v.planeReorderPossible;
    root[ "persistentContent" ] = v.persistentContent;
    return root;
  }
  nlohmann::json to_json( const vk::DisplayModeParametersKHR &v ) {
    auto root = nlohmann::json::object();
    root[ "visibleRegion" ] = to_json( v.visibleRegion );
    root[ "refreshRate" ] = v.refreshRate;
    return root;
  }
  nlohmann::json to_json( const vk::DisplayModePropertiesKHR &v ) {
    auto root = nlohmann::json::object();
    root[ "parameters" ] = to_json( v.parameters );
    return root;
  }
  nlohmann::json to_json( const std::vector< vk::DisplayModePropertiesKHR > &v ) {
    auto root = nlohmann::json::array();
    for( const auto &e: v )
      root.push_back( to_json( e ) );
    return root;
  }
  nlohmann::json to_json( const vk::DisplayPlaneCapabilitiesKHR &v ) {
    auto root = nlohmann::json::object();
    root[ "supportedAlpha" ] = std::uint32_t( v.supportedAlpha );
    root[ "minSrcPosition" ] = to_json( v.minSrcPosition );
    root[ "maxSrcPosition" ] = to_json( v.maxSrcPosition );
    root[ "minSrcExtent" ] = to_json( v.minSrcExtent );
    root[ "maxSrcExtent" ] = to_json( v.maxSrcExtent );
    root[ "minDstPosition" ] = to_json( v.minDstPosition );
    root[ "maxDstPosition" ] = to_json( v.maxDstPosition );
    root[ "minDstExtent" ] = to_json( v.minDstExtent );
    root[ "maxDstExtent" ] = to_json( v.maxDstExtent );
    return root;
  }
  nlohmann::json to_json( const display_plane_t &v ) {
    auto root = nlohmann::json::object();
    root[ "basic" ] = to_json( v.basic );
    return root; 
  }
  nlohmann::json to_json( const std::unordered_map< std::uint32_t, display_plane_t > &v ) {
    auto root = nlohmann::json::object();
    for( auto &[key,value]: v ) 
      root[ std::to_string( key ) ] = to_json( value );
    return root;
  }
  nlohmann::json to_json( const display_mode_t &v ) {
    auto root = nlohmann::json::object();
    root[ "basic" ] = to_json( v.basic );
    root[ "capabilities" ] = to_json( v.capabilities );
    return root;
  }
  nlohmann::json to_json( const std::vector< display_mode_t > &v ) {
    auto root = nlohmann::json::array();
    for( const auto &e: v )
      root.push_back( to_json( e ) );
    return root;
  }
  nlohmann::json to_json( const display_properties_t &v ) {
    auto root = nlohmann::json::object();
    root[ "basic" ] = to_json( v.basic );
    root[ "modes" ] = to_json( v.modes );
    return root;
  }
  nlohmann::json to_json( const std::vector< display_properties_t > &v ) {
    auto root = nlohmann::json::array();
    for( const auto &e: v )
      root.push_back( to_json( e ) );
    return root;
  }
#endif

#ifdef VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME
    nlohmann::json to_json( const vk::PhysicalDeviceAccelerationStructurePropertiesKHR &v ) {
      auto root = nlohmann::json::object();
      root[ "maxGeometryCount" ] = v.maxGeometryCount;
      root[ "maxInstanceCount" ] = v.maxInstanceCount;
      root[ "maxPrimitiveCount" ] = v.maxPrimitiveCount;
      root[ "maxPerStageDescriptorAccelerationStructures" ] = v.maxPerStageDescriptorAccelerationStructures;
      root[ "maxPerStageDescriptorUpdateAfterBindAccelerationStructures" ] = v.maxPerStageDescriptorUpdateAfterBindAccelerationStructures;
      root[ "maxDescriptorSetAccelerationStructures" ] = v.maxDescriptorSetAccelerationStructures;
      root[ "maxDescriptorSetUpdateAfterBindAccelerationStructures" ] = v.maxDescriptorSetUpdateAfterBindAccelerationStructures;
      root[ "minAccelerationStructureScratchOffsetAlignment" ] = v.minAccelerationStructureScratchOffsetAlignment;
      return root;
    }
#endif
#ifdef VK_EXT_BLEND_OPERATION_ADVANCED_EXTENSION_NAME
    nlohmann::json to_json( const vk::PhysicalDeviceBlendOperationAdvancedPropertiesEXT &v ) {
       auto root = nlohmann::json::object();
       root[ "advancedBlendMaxColorAttachments" ] = v.advancedBlendMaxColorAttachments;
       root[ "advancedBlendIndependentBlend" ] = v.advancedBlendIndependentBlend;
       root[ "advancedBlendNonPremultipliedSrcColor" ] = v.advancedBlendNonPremultipliedSrcColor;
       root[ "advancedBlendNonPremultipliedDstColor" ] = v.advancedBlendNonPremultipliedDstColor;
       root[ "advancedBlendCorrelatedOverlap" ] = v.advancedBlendCorrelatedOverlap;
       root[ "advancedBlendAllOperations" ] = v.advancedBlendAllOperations;
       return root;
    }
#endif
#ifdef VK_EXT_CONSERVATIVE_RASTERIZATION_EXTENSION_NAME
    nlohmann::json to_json( const vk::PhysicalDeviceConservativeRasterizationPropertiesEXT &v ) {
      auto root = nlohmann::json::object();
      root[ "primitiveOverestimationSize" ] = v.primitiveOverestimationSize;
      root[ "maxExtraPrimitiveOverestimationSize" ] = v.maxExtraPrimitiveOverestimationSize;
      root[ "extraPrimitiveOverestimationSizeGranularity" ] = v.extraPrimitiveOverestimationSizeGranularity;
      root[ "primitiveUnderestimation" ] = v.primitiveUnderestimation;
      root[ "conservativePointAndLineRasterization" ] = v.conservativePointAndLineRasterization;
      root[ "degenerateTrianglesRasterized" ] = v.degenerateTrianglesRasterized;
      root[ "degenerateLinesRasterized" ] = v.degenerateLinesRasterized;
      root[ "fullyCoveredFragmentShaderInputVariable" ] = v.fullyCoveredFragmentShaderInputVariable;
      root[ "conservativeRasterizationPostDepthCoverage" ] = v.conservativeRasterizationPostDepthCoverage;
      return root;
    }
#endif
#ifdef VK_NV_COOPERATIVE_MATRIX_EXTENSION_NAME
    nlohmann::json to_json( const vk::PhysicalDeviceCooperativeMatrixPropertiesNV &v ) {
      auto root = nlohmann::json::object();
      root[ "cooperativeMatrixSupportedStages" ] = std::uint32_t( v.cooperativeMatrixSupportedStages );
      return root;
    }
#endif
#ifdef VK_EXT_CUSTOM_BORDER_COLOR_EXTENSION_NAME
    nlohmann::json to_json( const vk::PhysicalDeviceCustomBorderColorPropertiesEXT &v ) {
      auto root = nlohmann::json::object();
      root[ "maxCustomBorderColorSamplers" ] = v.maxCustomBorderColorSamplers;
      return root;
    }
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_DEPTH_STENCIL_RESOLVE_EXTENSION_NAME)
#ifdef VK_VERSION_1_2
    nlohmann::json to_json( const vk::PhysicalDeviceDepthStencilResolveProperties &v ) {
#elif defined(VK_KHR_DEPTH_STENCIL_RESOLVE_EXTENSION_NAME)
    nlohmann::json to_json( const vk::PhysicalDeviceDepthStencilResolvePropertiesKHR &v ) {
#endif
      auto root = nlohmann::json::object();
      root[ "supportedDepthResolveModes" ] = std::uint32_t( v.supportedDepthResolveModes );
      root[ "supportedStencilResolveModes" ] = std::uint32_t( v.supportedStencilResolveModes );
      root[ "independentResolveNone" ] = v.independentResolveNone;
      root[ "independentResolve" ] = v.independentResolve;
      return root;
    }
#endif
#if defined(VK_VERSION_1_2) || defined(VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME)
#ifdef VK_VERSION_1_2
    nlohmann::json to_json( const vk::PhysicalDeviceDescriptorIndexingProperties &v ) {
#elif defined(VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME)
    nlohmann::json to_json( const vk::PhysicalDeviceDescriptorIndexingPropertiesEXT &v ) {
#endif
      auto root = nlohmann::json::object();
      root[ "maxUpdateAfterBindDescriptorsInAllPools" ] = v.maxUpdateAfterBindDescriptorsInAllPools;
      root[ "shaderUniformBufferArrayNonUniformIndexingNative" ] = v.shaderUniformBufferArrayNonUniformIndexingNative;
      root[ "shaderSampledImageArrayNonUniformIndexingNative" ] = v.shaderSampledImageArrayNonUniformIndexingNative;
      root[ "shaderStorageBufferArrayNonUniformIndexingNative" ] = v.shaderStorageBufferArrayNonUniformIndexingNative;
      root[ "shaderStorageImageArrayNonUniformIndexingNative" ] = v.shaderStorageImageArrayNonUniformIndexingNative;
      root[ "shaderInputAttachmentArrayNonUniformIndexingNative" ] = v.shaderInputAttachmentArrayNonUniformIndexingNative;
      root[ "robustBufferAccessUpdateAfterBind" ] = v.robustBufferAccessUpdateAfterBind;
      root[ "quadDivergentImplicitLod" ] = v.quadDivergentImplicitLod;
      root[ "maxPerStageDescriptorUpdateAfterBindSamplers" ] = v.maxPerStageDescriptorUpdateAfterBindSamplers;
      root[ "maxPerStageDescriptorUpdateAfterBindUniformBuffers" ] = v.maxPerStageDescriptorUpdateAfterBindUniformBuffers;
      root[ "maxPerStageDescriptorUpdateAfterBindStorageBuffers" ] = v.maxPerStageDescriptorUpdateAfterBindStorageBuffers;
      root[ "maxPerStageDescriptorUpdateAfterBindSampledImages" ] = v.maxPerStageDescriptorUpdateAfterBindSampledImages;
      root[ "maxPerStageDescriptorUpdateAfterBindStorageImages" ] = v.maxPerStageDescriptorUpdateAfterBindStorageImages;
      root[ "maxPerStageDescriptorUpdateAfterBindInputAttachments" ] = v.maxPerStageDescriptorUpdateAfterBindInputAttachments;
      root[ "maxPerStageUpdateAfterBindResources" ] = v.maxPerStageUpdateAfterBindResources;
      root[ "maxDescriptorSetUpdateAfterBindSamplers" ] = v.maxDescriptorSetUpdateAfterBindSamplers;
      root[ "maxDescriptorSetUpdateAfterBindUniformBuffers" ] = v.maxDescriptorSetUpdateAfterBindUniformBuffers;
      root[ "maxDescriptorSetUpdateAfterBindUniformBuffersDynamic" ] = v.maxDescriptorSetUpdateAfterBindUniformBuffersDynamic;
      root[ "maxDescriptorSetUpdateAfterBindStorageBuffers" ] = v.maxDescriptorSetUpdateAfterBindStorageBuffers;
      root[ "maxDescriptorSetUpdateAfterBindStorageBuffersDynamic" ] = v.maxDescriptorSetUpdateAfterBindStorageBuffersDynamic;
      root[ "maxDescriptorSetUpdateAfterBindSampledImages" ] = v.maxDescriptorSetUpdateAfterBindSampledImages;
      root[ "maxDescriptorSetUpdateAfterBindStorageImages" ] = v.maxDescriptorSetUpdateAfterBindStorageImages;
      root[ "maxDescriptorSetUpdateAfterBindInputAttachments" ] = v.maxDescriptorSetUpdateAfterBindInputAttachments;
      return root;
    }
#endif
#ifdef VK_NV_DEVICE_GENERATED_COMMANDS_EXTENSION_NAME
    nlohmann::json to_json( const vk::PhysicalDeviceDeviceGeneratedCommandsPropertiesNV &v ) {
      auto root = nlohmann::json::object();
      root[ "maxGraphicsShaderGroupCount" ] = v.maxGraphicsShaderGroupCount;
      root[ "maxIndirectSequenceCount" ] = v.maxIndirectSequenceCount;
      root[ "maxIndirectCommandsTokenCount" ] = v.maxIndirectCommandsTokenCount;
      root[ "maxIndirectCommandsStreamCount" ] = v.maxIndirectCommandsStreamCount;
      root[ "maxIndirectCommandsTokenOffset" ] = v.maxIndirectCommandsTokenOffset;
      root[ "maxIndirectCommandsStreamStride" ] = v.maxIndirectCommandsStreamStride;
      root[ "minSequencesCountBufferOffsetAlignment" ] = v.minSequencesCountBufferOffsetAlignment;
      root[ "minSequencesIndexBufferOffsetAlignment" ] = v.minSequencesIndexBufferOffsetAlignment;
      root[ "minIndirectCommandsBufferOffsetAlignment" ] = v.minIndirectCommandsBufferOffsetAlignment;
      return root;
    }
#endif
#ifdef VK_EXT_DISCARD_RECTANGLES_EXTENSION_NAME
    nlohmann::json to_json( const vk::PhysicalDeviceDiscardRectanglePropertiesEXT &v ) {
      auto root = nlohmann::json::object();
      root[ "maxDiscardRectangles" ] = v.maxDiscardRectangles;
      return root;
    }
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_DRIVER_PROPERTIES_EXTENSION_NAME)
#ifdef VK_VERSION_1_2
    nlohmann::json to_json( const vk::ConformanceVersion &v ) {
#elif defined(VK_KHR_DRIVER_PROPERTIES_EXTENSION_NAME)
    nlohmann::json to_json( const vk::ConformanceVersionKHR &v ) {
#endif
      auto root = nlohmann::json::object();
      root[ "major" ] = v.major;
      root[ "minor" ] = v.minor;
      root[ "subminor" ] = v.subminor;
      root[ "patch" ] = v.patch;
      return root;
    }
#ifdef VK_VERSION_1_2
    nlohmann::json to_json( const vk::PhysicalDeviceDriverProperties &v ) {
#elif defined(VK_KHR_DRIVER_PROPERTIES_EXTENSION_NAME)
    nlohmann::json to_json( const vk::PhysicalDeviceDriverPropertiesKHR &v ) {
#endif
      auto root = nlohmann::json::object();
      root[ "driverID" ] = std::uint32_t( v.driverID );
      root[ "driverName" ] = v.driverName;
      root[ "driverInfo" ] = v.driverInfo;
      root[ "conformanceVersion" ] = to_json( v.conformanceVersion );
      return root;
    }
#endif
#ifdef VK_EXT_PHYSICAL_DEVICE_DRM_EXTENSION_NAME
    nlohmann::json to_json( const vk::PhysicalDeviceDrmPropertiesEXT &v ) {
      auto root = nlohmann::json::object();
      root[ "hasPrimary" ] = v.hasPrimary;
      root[ "hasRender" ] = v.hasRender;
      root[ "primaryMajor" ] = v.primaryMajor;
      root[ "primaryMinor" ] = v.primaryMinor;
      root[ "renderMajor" ] = v.renderMajor;
      root[ "renderMinor" ] = v.renderMinor;
      return root;
    }
#endif
#ifdef VK_EXT_EXTERNAL_MEMORY_HOST_EXTENSION_NAME
    nlohmann::json to_json( const vk::PhysicalDeviceExternalMemoryHostPropertiesEXT &v ) {
      auto root = nlohmann::json::object();
      root[ "minImportedHostPointerAlignment" ] = v.minImportedHostPointerAlignment;
      return root;
    }
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_SHADER_FLOAT_CONTROLS_EXTENSION_NAME)
#ifdef VK_VERSION_1_2
    nlohmann::json to_json( const vk::PhysicalDeviceFloatControlsProperties &v ) {
#elif defined(VK_KHR_SHADER_FLOAT_CONTROLS_EXTENSION_NAME)
    nlohmann::json to_json( const vk::PhysicalDeviceFloatControlsPropertiesKHR &v ) {
#endif
      auto root = nlohmann::json::object();
      root[ "denormBehaviorIndependence" ] = std::uint32_t( v.denormBehaviorIndependence );
      root[ "roundingModeIndependence" ] = std::uint32_t( v.roundingModeIndependence );
      root[ "shaderSignedZeroInfNanPreserveFloat16" ] = v.shaderSignedZeroInfNanPreserveFloat16;
      root[ "shaderSignedZeroInfNanPreserveFloat32" ] = v.shaderSignedZeroInfNanPreserveFloat32;
      root[ "shaderSignedZeroInfNanPreserveFloat64" ] = v.shaderSignedZeroInfNanPreserveFloat64;
      root[ "shaderDenormPreserveFloat16" ] = v.shaderDenormPreserveFloat16;
      root[ "shaderDenormPreserveFloat32" ] = v.shaderDenormPreserveFloat32;
      root[ "shaderDenormPreserveFloat64" ] = v.shaderDenormPreserveFloat64;
      root[ "shaderDenormFlushToZeroFloat16" ] = v.shaderDenormFlushToZeroFloat16;
      root[ "shaderDenormFlushToZeroFloat32" ] = v.shaderDenormFlushToZeroFloat32;
      root[ "shaderDenormFlushToZeroFloat64" ] = v.shaderDenormFlushToZeroFloat64;
      root[ "shaderRoundingModeRTEFloat16" ] = v.shaderRoundingModeRTEFloat16;
      root[ "shaderRoundingModeRTEFloat32" ] = v.shaderRoundingModeRTEFloat32;
      root[ "shaderRoundingModeRTEFloat64" ] = v.shaderRoundingModeRTEFloat64;
      root[ "shaderRoundingModeRTZFloat16" ] = v.shaderRoundingModeRTZFloat16;
      root[ "shaderRoundingModeRTZFloat32" ] = v.shaderRoundingModeRTZFloat32;
      root[ "shaderRoundingModeRTZFloat64" ] = v.shaderRoundingModeRTZFloat64;
      return root;
    }
#endif
#ifdef VK_EXT_FRAGMENT_DENSITY_MAP_2_EXTENSION_NAME
    nlohmann::json to_json( const vk::PhysicalDeviceFragmentDensityMap2PropertiesEXT &v ) {
      auto root = nlohmann::json::object();
      root[ "subsampledLoads" ] = v.subsampledLoads;
      root[ "subsampledCoarseReconstructionEarlyAccess" ] = v.subsampledCoarseReconstructionEarlyAccess;
      root[ "maxSubsampledArrayLayers" ] = v.maxSubsampledArrayLayers;
      root[ "maxDescriptorSetSubsampledSamplers" ] = v.maxDescriptorSetSubsampledSamplers;
      return root;
    }
#endif
#ifdef VK_EXT_FRAGMENT_DENSITY_MAP_EXTENSION_NAME
    nlohmann::json to_json( const vk::PhysicalDeviceFragmentDensityMapPropertiesEXT &v ) {
      auto root = nlohmann::json::object();
      root[ "minFragmentDensityTexelSize" ] = to_json( v.minFragmentDensityTexelSize );
      root[ "maxFragmentDensityTexelSize" ] = to_json( v.maxFragmentDensityTexelSize );
      root[ "fragmentDensityInvocations" ] = v.fragmentDensityInvocations;
      return root;
    }
#endif
#ifdef VK_NV_FRAGMENT_SHADING_RATE_ENUMS_EXTENSION_NAME
    nlohmann::json to_json( const vk::PhysicalDeviceFragmentShadingRateEnumsPropertiesNV &v ) {
      auto root = nlohmann::json::object();
      root[ "maxFragmentShadingRateInvocationCount" ] = std::uint32_t( v.maxFragmentShadingRateInvocationCount );
      return root;
    }
#endif
#ifdef VK_KHR_FRAGMENT_SHADING_RATE_EXTENSION_NAME
    nlohmann::json to_json( const vk::PhysicalDeviceFragmentShadingRatePropertiesKHR &v ) {
      auto root = nlohmann::json::object();
      root[ "minFragmentShadingRateAttachmentTexelSize" ] = to_json( v.minFragmentShadingRateAttachmentTexelSize );
      root[ "maxFragmentShadingRateAttachmentTexelSize" ] = to_json( v.maxFragmentShadingRateAttachmentTexelSize );
      root[ "maxFragmentShadingRateAttachmentTexelSizeAspectRatio" ] = v.maxFragmentShadingRateAttachmentTexelSizeAspectRatio;
      root[ "primitiveFragmentShadingRateWithMultipleViewports" ] = v.primitiveFragmentShadingRateWithMultipleViewports;
      root[ "layeredShadingRateAttachments" ] = v.layeredShadingRateAttachments;
      root[ "fragmentShadingRateNonTrivialCombinerOps" ] = v.fragmentShadingRateNonTrivialCombinerOps;
      root[ "maxFragmentSize" ] = to_json( v.maxFragmentSize );
      root[ "maxFragmentSizeAspectRatio" ] = v.maxFragmentSizeAspectRatio;
      root[ "maxFragmentShadingRateCoverageSamples" ] = v.maxFragmentShadingRateCoverageSamples;
      root[ "maxFragmentShadingRateRasterizationSamples" ] = std::uint32_t( v.maxFragmentShadingRateRasterizationSamples );
      root[ "fragmentShadingRateWithShaderDepthStencilWrites" ] = v.fragmentShadingRateWithShaderDepthStencilWrites;
      root[ "fragmentShadingRateWithSampleMask" ] = v.fragmentShadingRateWithSampleMask;
      root[ "fragmentShadingRateWithShaderSampleMask" ] = v.fragmentShadingRateWithShaderSampleMask;
      root[ "fragmentShadingRateWithConservativeRasterization" ] = v.fragmentShadingRateWithConservativeRasterization;
      root[ "fragmentShadingRateWithFragmentShaderInterlock" ] = v.fragmentShadingRateWithFragmentShaderInterlock;
      root[ "fragmentShadingRateWithCustomSampleLocations" ] = v.fragmentShadingRateWithCustomSampleLocations;
      root[ "fragmentShadingRateStrictMultiplyCombiner" ] = v.fragmentShadingRateStrictMultiplyCombiner;
      return root;
    }
#endif
#if defined(VK_VERSION_1_1) || defined(VK_KHR_external_memory_capabilities) || defined(VK_KHR_EXTERNAL_SEMAPHORE_CAPABILITIES_EXTENSION_NAME) || defined(VK_KHR_EXTERNAL_FENCE_CAPABILITIES_EXTENSION_NAME)
#ifdef VK_VERSION_1_1
    nlohmann::json to_json( const vk::PhysicalDeviceIDProperties &v ) {
#elif defined(VK_KHR_external_memory_capabilities) || defined(VK_KHR_EXTERNAL_SEMAPHORE_CAPABILITIES_EXTENSION_NAME) || defined(VK_KHR_EXTERNAL_FENCE_CAPABILITIES_EXTENSION_NAME)
    nlohmann::json to_json( const vk::PhysicalDeviceIDPropertiesKHR &v ) {
#endif
      auto root = nlohmann::json::object();
      root[ "deviceUUID" ] = v.deviceUUID;
      root[ "driverUUID" ] = v.driverUUID;
      root[ "deviceLUID" ] = v.deviceLUID;
      root[ "deviceNodeMask" ] = v.deviceNodeMask;
      root[ "deviceLUIDValid" ] = v.deviceLUIDValid;
      return root;
    }
#endif
#ifdef VK_EXT_INLINE_UNIFORM_BLOCK_EXTENSION_NAME
    nlohmann::json to_json( const vk::PhysicalDeviceInlineUniformBlockPropertiesEXT &v ) {
      auto root = nlohmann::json::object();
      root[ "maxInlineUniformBlockSize" ] = v.maxInlineUniformBlockSize;
      root[ "maxPerStageDescriptorInlineUniformBlocks" ] = v.maxPerStageDescriptorInlineUniformBlocks;
      root[ "maxPerStageDescriptorUpdateAfterBindInlineUniformBlocks" ] = v.maxPerStageDescriptorUpdateAfterBindInlineUniformBlocks;
      root[ "maxDescriptorSetInlineUniformBlocks" ] = v.maxDescriptorSetInlineUniformBlocks;
      root[ "maxDescriptorSetUpdateAfterBindInlineUniformBlocks" ] = v.maxDescriptorSetUpdateAfterBindInlineUniformBlocks;
      return root;
    }
#endif
#ifdef VK_EXT_LINE_RASTERIZATION_EXTENSION_NAME
    nlohmann::json to_json( const vk::PhysicalDeviceLineRasterizationPropertiesEXT &v ) {
      auto root = nlohmann::json::object();
      root[ "lineSubPixelPrecisionBits" ] = v.lineSubPixelPrecisionBits;
      return root;
    }
#endif
#if defined(VK_VERSION_1_1) ||  defined(VK_KHR_MAINTENANCE3_EXTENSION_NAME)
#ifdef VK_VERSION_1_1
    nlohmann::json to_json( const vk::PhysicalDeviceMaintenance3Properties &v ) {
#elif defined(VK_KHR_MAINTENANCE3_EXTENSION_NAME)
    nlohmann::json to_json( const vk::PhysicalDeviceMaintenance3PropertiesKHR &v ) {
#endif
      auto root = nlohmann::json::object();
      root[ "maxPerSetDescriptors" ] = v.maxPerSetDescriptors;
      root[ "maxMemoryAllocationSize" ] = v.maxMemoryAllocationSize;
      return root;
    }
#endif
#ifdef VK_NV_MESH_SHADER_EXTENSION_NAME
    nlohmann::json to_json( const vk::PhysicalDeviceMeshShaderPropertiesNV &v ) {
      auto root = nlohmann::json::object();
      root[ "maxDrawMeshTasksCount" ] = v.maxDrawMeshTasksCount;
      root[ "maxTaskWorkGroupInvocations" ] = v.maxTaskWorkGroupInvocations;
      root[ "maxTaskWorkGroupSize" ] = v.maxTaskWorkGroupSize;
      root[ "maxTaskTotalMemorySize" ] = v.maxTaskTotalMemorySize;
      root[ "maxTaskOutputCount" ] = v.maxTaskOutputCount;
      root[ "maxMeshWorkGroupInvocations" ] = v.maxMeshWorkGroupInvocations;
      root[ "maxMeshWorkGroupSize" ] = v.maxMeshWorkGroupSize;
      root[ "maxMeshTotalMemorySize" ] = v.maxMeshTotalMemorySize;
      root[ "maxMeshOutputVertices" ] = v.maxMeshOutputVertices;
      root[ "maxMeshOutputPrimitives" ] = v.maxMeshOutputPrimitives;
      root[ "maxMeshMultiviewViewCount" ] = v.maxMeshMultiviewViewCount;
      root[ "meshOutputPerVertexGranularity" ] = v.meshOutputPerVertexGranularity;
      root[ "meshOutputPerPrimitiveGranularity" ] = v.meshOutputPerPrimitiveGranularity;
      return root;
    }
#endif
#ifdef VK_EXT_MULTI_DRAW_EXTENSION_NAME
    nlohmann::json to_json( const vk::PhysicalDeviceMultiDrawPropertiesEXT &v ) {
      auto root = nlohmann::json::object();
      root[ "maxMultiDrawCount" ] = v.maxMultiDrawCount;
      return root;
    }
#endif
#ifdef VK_NVX_MULTIVIEW_PER_VIEW_ATTRIBUTES_EXTENSION_NAME
    nlohmann::json to_json( const vk::PhysicalDeviceMultiviewPerViewAttributesPropertiesNVX &v ) {
      auto root = nlohmann::json::object();
      root[ "perViewPositionAllComponents" ] = v.perViewPositionAllComponents;
      return root;
    }
#endif
#if defined(VK_VERSION_1_1) || defined(VK_KHR_MULTIVIEW_EXTENSION_NAME)
#ifdef VK_VERSION_1_1
    nlohmann::json to_json( const vk::PhysicalDeviceMultiviewProperties &v ) {
#elif defined(VK_KHR_MULTIVIEW_EXTENSION_NAME)
    nlohmann::json to_json( const vk::PhysicalDeviceMultiviewPropertiesKHR &v ) {
#endif
      auto root = nlohmann::json::object();
      root[ "maxMultiviewViewCount" ] = v.maxMultiviewViewCount;
      root[ "maxMultiviewInstanceIndex" ] = v.maxMultiviewInstanceIndex;
      return root;
    }
#endif
#ifdef VK_EXT_PCI_BUS_INFO_EXTENSION_NAME
    nlohmann::json to_json( const vk::PhysicalDevicePCIBusInfoPropertiesEXT &v ) {
      auto root = nlohmann::json::object();
      root[ "pciDomain" ] = v.pciDomain;
      root[ "pciBus" ] = v.pciBus;
      root[ "pciDevice" ] = v.pciDevice;
      root[ "pciFunction" ] = v.pciFunction;
      return root;
    }
#endif
#ifdef VK_KHR_PERFORMANCE_QUERY_EXTENSION_NAME
    nlohmann::json to_json( const vk::PhysicalDevicePerformanceQueryPropertiesKHR &v ) {
      auto root = nlohmann::json::object();
      root[ "allowCommandBufferQueryCopies" ] = v.allowCommandBufferQueryCopies;
      return root;
    }
#endif
#if defined(VK_VERSION_1_1) || defined(VK_KHR_MAINTENANCE2_EXTENSION_NAME)
#ifdef VK_VERSION_1_1
    nlohmann::json to_json( const vk::PhysicalDevicePointClippingProperties &v ) {
#elif defined(VK_KHR_MAINTENANCE2_EXTENSION_NAME)
    nlohmann::json to_json( const vk::PhysicalDevicePointClippingPropertiesKHR &v ) {
#endif
      auto root = nlohmann::json::object();
      root[ "pointClippingBehavior" ] = v.pointClippingBehavior;
      return root;
    }
#endif
#ifdef VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME
    nlohmann::json to_json( const vk::PhysicalDevicePortabilitySubsetPropertiesKHR &v ) {
      auto root = nlohmann::json::object();
      root[ "minVertexInputBindingStrideAlignment" ] = v.minVertexInputBindingStrideAlignment;
      return root;
    }
#endif
#ifdef VK_VERSION_1_1
    nlohmann::json to_json( const vk::PhysicalDeviceProtectedMemoryProperties &v ) {
      auto root = nlohmann::json::object();
      root[ "protectedNoFault" ] = v.protectedNoFault;
      return root;
    }
#endif
#ifdef VK_EXT_PROVOKING_VERTEX_EXTENSION_NAME
    nlohmann::json to_json( const vk::PhysicalDeviceProvokingVertexPropertiesEXT &v ) {
      auto root = nlohmann::json::object();
      root[ "provokingVertexModePerPipeline" ] = v.provokingVertexModePerPipeline;
      root[ "transformFeedbackPreservesTriangleFanProvokingVertex" ] = v.transformFeedbackPreservesTriangleFanProvokingVertex;
      return root;
    }
#endif
#ifdef VK_KHR_PUSH_DESCRIPTOR_EXTENSION_NAME
    nlohmann::json to_json( const vk::PhysicalDevicePushDescriptorPropertiesKHR &v ) {
      auto root = nlohmann::json::object();
      root[ "maxPushDescriptors" ] = v.maxPushDescriptors;
      return root;
    }
#endif
#ifdef VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME
    nlohmann::json to_json( const vk::PhysicalDeviceRayTracingPipelinePropertiesKHR &v ) {
      auto root = nlohmann::json::object();
      root[ "shaderGroupHandleSize" ] = v.shaderGroupHandleSize;
      root[ "maxRayRecursionDepth" ] = v.maxRayRecursionDepth;
      root[ "maxShaderGroupStride" ] = v.maxShaderGroupStride;
      root[ "shaderGroupBaseAlignment" ] = v.shaderGroupBaseAlignment;
      root[ "shaderGroupHandleCaptureReplaySize" ] = v.shaderGroupHandleCaptureReplaySize;
      root[ "maxRayDispatchInvocationCount" ] = v.maxRayDispatchInvocationCount;
      root[ "shaderGroupHandleAlignment" ] = v.shaderGroupHandleAlignment;
      root[ "maxRayHitAttributeSize" ] = v.maxRayHitAttributeSize;
      return root;
    }
#endif
#ifdef VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME
    nlohmann::json to_json( const vk::PhysicalDeviceRayTracingPropertiesNV &v ) {
      auto root = nlohmann::json::object();
      root[ "shaderGroupHandleSize" ] = v.shaderGroupHandleSize;
      root[ "maxRecursionDepth" ] = v.maxRecursionDepth;
      root[ "maxShaderGroupStride" ] = v.maxShaderGroupStride;
      root[ "shaderGroupBaseAlignment" ] = v.shaderGroupBaseAlignment;
      root[ "maxGeometryCount" ] = v.maxGeometryCount;
      root[ "maxInstanceCount" ] = v.maxInstanceCount;
      root[ "maxTriangleCount" ] = v.maxTriangleCount;
      root[ "maxDescriptorSetAccelerationStructures" ] = v.maxDescriptorSetAccelerationStructures;
      return root;
    }
#endif
#ifdef VK_EXT_ROBUSTNESS_2_EXTENSION_NAME
    nlohmann::json to_json( const vk::PhysicalDeviceRobustness2PropertiesEXT &v ) {
      auto root = nlohmann::json::object();
      root[ "robustStorageBufferAccessSizeAlignment" ] = v.robustStorageBufferAccessSizeAlignment;
      root[ "robustUniformBufferAccessSizeAlignment" ] = v.robustUniformBufferAccessSizeAlignment;
      return root;
    }
#endif
#ifdef VK_EXT_SAMPLE_LOCATIONS_EXTENSION_NAME
    nlohmann::json to_json( const vk::PhysicalDeviceSampleLocationsPropertiesEXT &v ) {
      auto root = nlohmann::json::object();
      root[ "sampleLocationSampleCounts" ] = std::uint32_t( v.sampleLocationSampleCounts );
      root[ "maxSampleLocationGridSize" ] = to_json( v.maxSampleLocationGridSize );
      root[ "sampleLocationCoordinateRange" ] = v.sampleLocationCoordinateRange;
      root[ "sampleLocationSubPixelBits" ] = v.sampleLocationSubPixelBits;
      root[ "variableSampleLocations" ] = v.variableSampleLocations;
      return root;
    }
#endif
#if defined(VK_VERSION_1_2) || defined(VK_EXT_SAMPLER_FILTER_MINMAX_EXTENSION_NAME)
#ifdef VK_VERSION_1_2
    nlohmann::json to_json( const vk::PhysicalDeviceSamplerFilterMinmaxProperties &v ) {
#elif defined(VK_EXT_SAMPLER_FILTER_MINMAX_EXTENSION_NAME)
    nlohmann::json to_json( const vk::PhysicalDeviceSamplerFilterMinmaxPropertiesEXT &v ) {
#endif
      auto root = nlohmann::json::object();
      root[ "filterMinmaxSingleComponentFormats" ] = v.filterMinmaxSingleComponentFormats;
      root[ "filterMinmaxImageComponentMapping" ] = v.filterMinmaxImageComponentMapping;
      return root;
    }
#endif
#ifdef VK_AMD_SHADER_CORE_PROPERTIES_2_EXTENSION_NAME
    nlohmann::json to_json( const vk::PhysicalDeviceShaderCoreProperties2AMD &v ) {
      auto root = nlohmann::json::object();
      root[ "shaderCoreFeatures" ] = std::uint32_t( v.shaderCoreFeatures );
      root[ "activeComputeUnitCount" ] = v.activeComputeUnitCount;
      return root;
    }
#endif
#ifdef VK_AMD_SHADER_CORE_PROPERTIES_2_EXTENSION_NAME
    nlohmann::json to_json( const vk::PhysicalDeviceShaderCorePropertiesAMD &v ) {
      auto root = nlohmann::json::object();
      root[ "shaderEngineCount" ] = v.shaderEngineCount;
      root[ "shaderArraysPerEngineCount" ] = v.shaderArraysPerEngineCount;
      root[ "computeUnitsPerShaderArray" ] = v.computeUnitsPerShaderArray;
      root[ "simdPerComputeUnit" ] = v.simdPerComputeUnit;
      root[ "wavefrontsPerSimd" ] = v.wavefrontsPerSimd;
      root[ "wavefrontSize" ] = v.wavefrontSize;
      root[ "sgprsPerSimd" ] = v.sgprsPerSimd;
      root[ "minSgprAllocation" ] = v.minSgprAllocation;
      root[ "maxSgprAllocation" ] = v.maxSgprAllocation;
      root[ "sgprAllocationGranularity" ] = v.sgprAllocationGranularity;
      root[ "vgprsPerSimd" ] = v.vgprsPerSimd;
      root[ "minVgprAllocation" ] = v.minVgprAllocation;
      root[ "maxVgprAllocation" ] = v.maxVgprAllocation;
      root[ "vgprAllocationGranularity" ] = v.vgprAllocationGranularity;
      return root;
    }
#endif
#ifdef VK_KHR_SHADER_INTEGER_DOT_PRODUCT_EXTENSION_NAME
    nlohmann::json to_json( const vk::PhysicalDeviceShaderIntegerDotProductPropertiesKHR &v ) {
      auto root = nlohmann::json::object();
      root[ "integerDotProduct8BitUnsignedAccelerated" ] = v.integerDotProduct8BitUnsignedAccelerated;
      root[ "integerDotProduct8BitSignedAccelerated" ] = v.integerDotProduct8BitSignedAccelerated;
      root[ "integerDotProduct8BitMixedSignednessAccelerated" ] = v.integerDotProduct8BitMixedSignednessAccelerated;
      root[ "integerDotProduct4x8BitPackedUnsignedAccelerated" ] = v.integerDotProduct4x8BitPackedUnsignedAccelerated;
      root[ "integerDotProduct4x8BitPackedSignedAccelerated" ] = v.integerDotProduct4x8BitPackedSignedAccelerated;
      root[ "integerDotProduct4x8BitPackedMixedSignednessAccelerated" ] = v.integerDotProduct4x8BitPackedMixedSignednessAccelerated;
      root[ "integerDotProduct16BitUnsignedAccelerated" ] = v.integerDotProduct16BitUnsignedAccelerated;
      root[ "integerDotProduct16BitSignedAccelerated" ] = v.integerDotProduct16BitSignedAccelerated;
      root[ "integerDotProduct16BitMixedSignednessAccelerated" ] = v.integerDotProduct16BitMixedSignednessAccelerated;
      root[ "integerDotProduct32BitUnsignedAccelerated" ] = v.integerDotProduct32BitUnsignedAccelerated;
      root[ "integerDotProduct32BitSignedAccelerated" ] = v.integerDotProduct32BitSignedAccelerated;
      root[ "integerDotProduct32BitMixedSignednessAccelerated" ] = v.integerDotProduct32BitMixedSignednessAccelerated;
      root[ "integerDotProduct64BitUnsignedAccelerated" ] = v.integerDotProduct64BitUnsignedAccelerated;
      root[ "integerDotProduct64BitSignedAccelerated" ] = v.integerDotProduct64BitSignedAccelerated;
      root[ "integerDotProduct64BitMixedSignednessAccelerated" ] = v.integerDotProduct64BitMixedSignednessAccelerated;
      root[ "integerDotProductAccumulatingSaturating8BitUnsignedAccelerated" ] = v.integerDotProductAccumulatingSaturating8BitUnsignedAccelerated;
      root[ "integerDotProductAccumulatingSaturating8BitSignedAccelerated" ] = v.integerDotProductAccumulatingSaturating8BitSignedAccelerated;
      root[ "integerDotProductAccumulatingSaturating8BitMixedSignednessAccelerated" ] = v.integerDotProductAccumulatingSaturating8BitMixedSignednessAccelerated;
      root[ "integerDotProductAccumulatingSaturating4x8BitPackedUnsignedAccelerated" ] = v.integerDotProductAccumulatingSaturating4x8BitPackedUnsignedAccelerated;
      root[ "integerDotProductAccumulatingSaturating4x8BitPackedSignedAccelerated" ] = v.integerDotProductAccumulatingSaturating4x8BitPackedSignedAccelerated;
      root[ "integerDotProductAccumulatingSaturating4x8BitPackedMixedSignednessAccelerated" ] = v.integerDotProductAccumulatingSaturating4x8BitPackedMixedSignednessAccelerated;
      root[ "integerDotProductAccumulatingSaturating16BitUnsignedAccelerated" ] = v.integerDotProductAccumulatingSaturating16BitUnsignedAccelerated;
      root[ "integerDotProductAccumulatingSaturating16BitSignedAccelerated" ] = v.integerDotProductAccumulatingSaturating16BitSignedAccelerated;
      root[ "integerDotProductAccumulatingSaturating16BitMixedSignednessAccelerated" ] = v.integerDotProductAccumulatingSaturating16BitMixedSignednessAccelerated;
      root[ "integerDotProductAccumulatingSaturating32BitUnsignedAccelerated" ] = v.integerDotProductAccumulatingSaturating32BitUnsignedAccelerated;
      root[ "integerDotProductAccumulatingSaturating32BitSignedAccelerated" ] = v.integerDotProductAccumulatingSaturating32BitSignedAccelerated;
      root[ "integerDotProductAccumulatingSaturating32BitMixedSignednessAccelerated" ] = v.integerDotProductAccumulatingSaturating32BitMixedSignednessAccelerated;
      root[ "integerDotProductAccumulatingSaturating64BitUnsignedAccelerated" ] = v.integerDotProductAccumulatingSaturating64BitUnsignedAccelerated;
      root[ "integerDotProductAccumulatingSaturating64BitSignedAccelerated" ] = v.integerDotProductAccumulatingSaturating64BitSignedAccelerated;
      root[ "integerDotProductAccumulatingSaturating64BitMixedSignednessAccelerated" ] = v.integerDotProductAccumulatingSaturating64BitMixedSignednessAccelerated;
      return root;
    }
#endif
#ifdef VK_NV_SHADER_SM_BUILTINS_EXTENSION_NAME
    nlohmann::json to_json( const vk::PhysicalDeviceShaderSMBuiltinsPropertiesNV &v ) {
      auto root = nlohmann::json::object();
      root[ "shaderSMCount" ] = v.shaderSMCount;
      root[ "shaderWarpsPerSM" ] = v.shaderWarpsPerSM;
      return root;
    }
#endif
#ifdef VK_NV_SHADING_RATE_IMAGE_EXTENSION_NAME
    nlohmann::json to_json( const vk::PhysicalDeviceShadingRateImagePropertiesNV &v ) {
      auto root = nlohmann::json::object();
      root[ "shadingRateTexelSize" ] = to_json( v.shadingRateTexelSize );
      root[ "shadingRatePaletteSize" ] = v.shadingRatePaletteSize;
      root[ "shadingRateMaxCoarseSamples" ] = v.shadingRateMaxCoarseSamples;
      return root;
    }
#endif
#ifdef VK_VERSION_1_1
    nlohmann::json to_json( const vk::PhysicalDeviceSubgroupProperties &v ) {
      auto root = nlohmann::json::object();
      root[ "subgroupSize" ] = v.subgroupSize;
      root[ "supportedStages" ] = std::uint32_t( v.supportedStages );
      root[ "supportedOperations" ] = std::uint32_t( v.supportedOperations );
      root[ "quadOperationsInAllStages" ] = v.quadOperationsInAllStages;
      return root;
    }
#endif
#ifdef VK_EXT_SUBGROUP_SIZE_CONTROL_EXTENSION_NAME
    nlohmann::json to_json( const vk::PhysicalDeviceSubgroupSizeControlPropertiesEXT &v ) {
      auto root = nlohmann::json::object();
      root[ "minSubgroupSize" ] = v.minSubgroupSize;
      root[ "maxSubgroupSize" ] = v.maxSubgroupSize;
      root[ "maxComputeWorkgroupSubgroups" ] = v.maxComputeWorkgroupSubgroups;
      root[ "requiredSubgroupSizeStages" ] = std::uint32_t( v.requiredSubgroupSizeStages );
      return root;
    }
#endif
#ifdef VK_HUAWEI_SUBPASS_SHADING_EXTENSION_NAME
    nlohmann::json to_json( const vk::PhysicalDeviceSubpassShadingPropertiesHUAWEI &v ) {
      auto root = nlohmann::json::object();
      root[ "maxSubpassShadingWorkgroupSizeAspectRatio" ] = v.maxSubpassShadingWorkgroupSizeAspectRatio;
      return root;
    }
#endif
#ifdef VK_EXT_TEXEL_BUFFER_ALIGNMENT_EXTENSION_NAME
    nlohmann::json to_json( const vk::PhysicalDeviceTexelBufferAlignmentPropertiesEXT &v ) {
      auto root = nlohmann::json::object();
      root[ "storageTexelBufferOffsetAlignmentBytes" ] = v.storageTexelBufferOffsetAlignmentBytes;
      root[ "storageTexelBufferOffsetSingleTexelAlignment" ] = v.storageTexelBufferOffsetSingleTexelAlignment;
      root[ "uniformTexelBufferOffsetAlignmentBytes" ] = v.uniformTexelBufferOffsetAlignmentBytes;
      root[ "uniformTexelBufferOffsetSingleTexelAlignment" ] = v.uniformTexelBufferOffsetSingleTexelAlignment;
      return root;
    }
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME)
#ifdef VK_VERSION_1_2
    nlohmann::json to_json( const vk::PhysicalDeviceTimelineSemaphoreProperties &v ) {
#elif defined(VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME)
    nlohmann::json to_json( const vk::PhysicalDeviceTimelineSemaphorePropertiesKHR &v ) {
#endif
      auto root = nlohmann::json::object();
      root[ "maxTimelineSemaphoreValueDifference" ] = v.maxTimelineSemaphoreValueDifference;
      return root;
    }
#endif
#ifdef VK_EXT_TRANSFORM_FEEDBACK_EXTENSION_NAME
    nlohmann::json to_json( const vk::PhysicalDeviceTransformFeedbackPropertiesEXT &v ) {
      auto root = nlohmann::json::object();
      root[ "maxTransformFeedbackStreams" ] = v.maxTransformFeedbackStreams;
      root[ "maxTransformFeedbackBuffers" ] = v.maxTransformFeedbackBuffers;
      root[ "maxTransformFeedbackBufferSize" ] = v.maxTransformFeedbackBufferSize;
      root[ "maxTransformFeedbackStreamDataSize" ] = v.maxTransformFeedbackStreamDataSize;
      root[ "maxTransformFeedbackBufferDataSize" ] = v.maxTransformFeedbackBufferDataSize;
      root[ "maxTransformFeedbackBufferDataStride" ] = v.maxTransformFeedbackBufferDataStride;
      root[ "transformFeedbackQueries" ] = v.transformFeedbackQueries;
      root[ "transformFeedbackStreamsLinesTriangles" ] = v.transformFeedbackStreamsLinesTriangles;
      root[ "transformFeedbackRasterizationStreamSelect" ]  = v.transformFeedbackRasterizationStreamSelect;
      root[ "transformFeedbackDraw" ] = v.transformFeedbackDraw;
      return root;
    }
#endif
#ifdef VK_EXT_VERTEX_ATTRIBUTE_DIVISOR_EXTENSION_NAME
    nlohmann::json to_json( const vk::PhysicalDeviceVertexAttributeDivisorPropertiesEXT &v ) {
      auto root = nlohmann::json::object();
      root[ "maxVertexAttribDivisor" ] = v.maxVertexAttribDivisor;
      return root;
    }
#endif

  nlohmann::json to_json( const physical_device_properties_t &v ) {
    auto root = nlohmann::json::object();
    root[ "basic" ] = to_json( v.get_basic() );
    root[ "available_extensions" ] = to_json( v.get_available_extensions() );
    root[ "available_layers" ] = to_json( v.get_available_layers() );
    root[ "activated_layers" ] = to_json( v.get_activated_layers() );
    LIBGCT_EXTENSION_TO_JSON( memory_props )
#ifdef VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( acceleration_structure )
#endif
#ifdef VK_EXT_BLEND_OPERATION_ADVANCED_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( blend_operation )
#endif
#ifdef VK_EXT_CONSERVATIVE_RASTERIZATION_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( conservative_rasterization )
#endif
#ifdef VK_NV_COOPERATIVE_MATRIX_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( cooperative_matrix )
#endif
#ifdef VK_EXT_CUSTOM_BORDER_COLOR_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( custom_border_color )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_DEPTH_STENCIL_RESOLVE_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( depth_stencil_resolve )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( device_descriptor_indexing )
#endif
#ifdef VK_NV_DEVICE_GENERATED_COMMANDS_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( device_generated_commands )
#endif
#ifdef VK_EXT_DISCARD_RECTANGLES_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( discard_rectangle )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_DRIVER_PROPERTIES_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( driver )
#endif
#ifdef VK_EXT_PHYSICAL_DEVICE_DRM_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( drm )
#endif
#ifdef VK_EXT_EXTERNAL_MEMORY_HOST_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( external_memory_host )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_SHADER_FLOAT_CONTROLS_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( float_controls )
#endif
#ifdef VK_EXT_FRAGMENT_DENSITY_MAP_2_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( fragment_density_map2 )
#endif
#ifdef VK_EXT_FRAGMENT_DENSITY_MAP_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( fragment_density_map )
#endif
#ifdef VK_NV_FRAGMENT_SHADING_RATE_ENUMS_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( fragment_shading_rate_enums )
#endif
#ifdef VK_KHR_FRAGMENT_SHADING_RATE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( fragment_shading_rate )
#endif
#if defined(VK_VERSION_1_1) || defined(VK_KHR_external_memory_capabilities) || defined(VK_KHR_EXTERNAL_SEMAPHORE_CAPABILITIES_EXTENSION_NAME) || defined(VK_KHR_EXTERNAL_FENCE_CAPABILITIES_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( id )
#endif
#ifdef VK_EXT_INLINE_UNIFORM_BLOCK_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( inline_uniform_block )
#endif
#ifdef VK_EXT_LINE_RASTERIZATION_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( line_rasterization )
#endif
#if defined(VK_VERSION_1_1) || defined(VK_KHR_MAINTENANCE3_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( maintenance3 )
#endif
#ifdef VK_NV_MESH_SHADER_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( mesh_shader )
#endif
#ifdef VK_EXT_MULTI_DRAW_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( multi_draw )
#endif
#ifdef VK_NVX_MULTIVIEW_PER_VIEW_ATTRIBUTES_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( multiview_per_view_attributes )
#endif
#if defined(VK_VERSION_1_1) || defined(VK_KHR_MULTIVIEW_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( multiview )
#endif
#ifdef VK_EXT_PCI_BUS_INFO_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( pci_bus_info )
#endif
#ifdef VK_KHR_PERFORMANCE_QUERY_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( performance_query )
#endif
#if defined(VK_VERSION_1_1) || defined(VK_KHR_MAINTENANCE2_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( point_clipping )
#endif
#ifdef VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( portability_subset )
#endif
#if VK_VERSION_1_1
    LIBGCT_EXTENSION_TO_JSON( protected_memory )
#endif
#ifdef VK_EXT_PROVOKING_VERTEX_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( provoking_vertex )
#endif
#ifdef VK_KHR_PUSH_DESCRIPTOR_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( push_descriptor )
#endif
#ifdef VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( ray_tracing_pipeline )
#endif
#ifdef VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( ray_tracing )
#endif
#ifdef VK_EXT_ROBUSTNESS_2_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( robustness2 )
#endif
#ifdef VK_EXT_SAMPLE_LOCATIONS_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( sample_locations )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_EXT_SAMPLER_FILTER_MINMAX_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( sampler_filter_minmax )
#endif
#ifdef VK_AMD_SHADER_CORE_PROPERTIES_2_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( shader_core2 )
#endif
#ifdef VK_AMD_SHADER_CORE_PROPERTIES_2_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( shader_core )
#endif
#ifdef VK_KHR_SHADER_INTEGER_DOT_PRODUCT_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( shader_integer_dot_product )
#endif
#ifdef VK_NV_SHADER_SM_BUILTINS_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( shader_sm_builtins )
#endif
#ifdef VK_NV_SHADING_RATE_IMAGE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( shading_rate_image )
#endif
#ifdef VK_VERSION_1_1
    LIBGCT_EXTENSION_TO_JSON( subgroup )
#endif
#ifdef VK_EXT_SUBGROUP_SIZE_CONTROL_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( subgroup_size_control )
#endif
#ifdef VK_HUAWEI_SUBPASS_SHADING_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( subpass_shading )
#endif
#ifdef VK_EXT_TEXEL_BUFFER_ALIGNMENT_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( texel_buffer_alignment )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( timeline_semaphore )
#endif
#ifdef VK_EXT_TRANSFORM_FEEDBACK_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( transform_feedback )
#endif
#ifdef VK_EXT_VERTEX_ATTRIBUTE_DIVISOR_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( vertex_attribute_divisor )
#endif



    root[ "queue_family" ] = nlohmann::json::array();
    for( auto &q: v.get_queue_family() )
      root[ "queue_family" ].push_back( to_json( q ) );
#ifdef VK_KHR_DISPLAY_EXTENSION_NAME
    root[ "display" ] = to_json( v.get_display() );
#endif
    return root;
  }
  
  physical_device_properties_t::physical_device_properties_t(
    instance_t &instance,
    vk::PhysicalDevice &pdev,
    const std::vector< const char* > &layers
  ) {
    available_layers = get_device_layers( pdev );
    activated_layer_names = layers;
    for( const auto &l: layers ) {
      auto found = available_layers.find( l );
      if( found != available_layers.end() ) {
        activated_layers.insert( *found );
        activated_layer_names.push_back( l );
      }
    }
    available_extensions = get_device_extensions( pdev, activated_layer_names );
    activated_extensions = available_extensions;
    std::transform(
      activated_extensions.begin(),
      activated_extensions.end(),
      std::back_inserter( activated_extension_names ),
      []( const auto &v ) {
        return v.first.data();
      }
    );
    load( instance, pdev, layers );
  }

  physical_device_properties_t::physical_device_properties_t(
    instance_t &instance,
    vk::PhysicalDevice &pdev,
    const std::vector< const char* > &layers,
    const std::vector< const char* > &exts
  ) {
    available_layers = get_device_layers( pdev );
    activated_layer_names = layers;
    activated_extension_names = exts;
    for( const auto &l: layers ) {
      auto found = available_layers.find( l );
      if( found != available_layers.end() ) {
        activated_layers.insert( *found );
      }
    }
    available_extensions = get_device_extensions( pdev, activated_layer_names );
    for( const auto &e: exts ) {
      const auto available = available_extensions.find( e );
      if( available == available_extensions.end() )
        throw -1;
      activated_extensions.insert( *available );
    }
    load( instance, pdev, layers );
  }

  void physical_device_properties_t::load(
    instance_t &instance,
    vk::PhysicalDevice &pdev,
    const std::vector< const char* > &layers
  ) {
#if defined(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME) || defined(VK_VERSION_1_1)
    if(
      instance.get_api_version() >= VK_MAKE_VERSION( 1, 1, 0 ) ||
      instance.get_activated_extensions().find( "VK_KHR_get_physical_device_properties2" ) != instance.get_activated_extensions().end()
    ) {
      const auto &ext = activated_extensions;
#ifdef VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( acceleration_structure, "VK_KHR_acceleration_structure" )
#endif
#ifdef VK_EXT_BLEND_OPERATION_ADVANCED_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( blend_operation, "VK_EXT_blend_operation_advanced" )
#endif
#ifdef VK_EXT_CONSERVATIVE_RASTERIZATION_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( conservative_rasterization, "VK_EXT_conservative_rasterization" )
#endif
#ifdef VK_NV_COOPERATIVE_MATRIX_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( cooperative_matrix, "VK_NV_cooperative_matrix" )
#endif
#ifdef VK_EXT_CUSTOM_BORDER_COLOR_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( custom_border_color, "VK_EXT_custom_border_color" )
#endif
#if defined(VK_KHR_DEPTH_STENCIL_RESOLVE_EXTENSION_NAME) || defined(VK_VERSION_1_2)
      LIBGCT_EXTENSION_CREATE_IF_VERSION_OR_EXT( depth_stencil_resolve, 1, 2, 0, "VK_KHR_depth_stencil_resolve" )
#endif
#if defined(VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME) || defined(VK_VERSION_1_2)
      LIBGCT_EXTENSION_CREATE_IF_VERSION_OR_EXT( device_descriptor_indexing, 1, 2, 0, "VK_EXT_descriptor_indexing" )
#endif
#ifdef VK_NV_DEVICE_GENERATED_COMMANDS_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( device_generated_commands, "VK_NV_device_generated_commands" )
#endif
#ifdef VK_EXT_DISCARD_RECTANGLES_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( discard_rectangle, "VK_EXT_discard_rectangles" )
#endif
#if defined(VK_KHR_DRIVER_PROPERTIES_EXTENSION_NAME) || defined(VK_VERSION_1_2)
      LIBGCT_EXTENSION_CREATE_IF_VERSION_OR_EXT( driver, 1, 2, 0, "VK_KHR_driver_properties" )
#endif
#ifdef VK_EXT_PHYSICAL_DEVICE_DRM_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( drm, "VK_EXT_physical_device_drm" )
#endif
#ifdef VK_EXT_EXTERNAL_MEMORY_HOST_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( external_memory_host, "VK_EXT_external_memory_host" )
#endif
#if defined(VK_KHR_SHADER_FLOAT_CONTROLS_EXTENSION_NAME) || defined(VK_VERSION_1_2)
      LIBGCT_EXTENSION_CREATE_IF_VERSION_OR_EXT( float_controls, 1, 2, 0, "VK_KHR_shader_float_controls" )
#endif
#ifdef VK_EXT_FRAGMENT_DENSITY_MAP_2_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( fragment_density_map2, "VK_EXT_fragment_density_map2" )
#endif
#ifdef VK_EXT_FRAGMENT_DENSITY_MAP_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( fragment_density_map, "VK_EXT_fragment_density_map" )
#endif
#ifdef VK_NV_FRAGMENT_SHADING_RATE_ENUMS_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( fragment_shading_rate_enums, "VK_NV_fragment_shading_rate_enums" )
#endif
#ifdef VK_KHR_FRAGMENT_SHADING_RATE_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( fragment_shading_rate, "VK_KHR_fragment_shading_rate" )
#endif
#if defined(VK_KHR_external_memory_capabilities) || defined(VK_VERSION_1_1) || defined(VK_KHR_EXTERNAL_SEMAPHORE_CAPABILITIES_EXTENSION_NAME) || defined(VK_KHR_EXTERNAL_FENCE_CAPABILITIES_EXTENSION_NAME)
      LIBGCT_EXTENSION_CREATE_IF_VERSION_OR_EXT( id, 1, 1, 0, "VK_KHR_external_memory_capabilities" )
      else
      LIBGCT_EXTENSION_CREATE_IF_EXT( id, "VK_KHR_external_semaphore_capabilities" )
      else
      LIBGCT_EXTENSION_CREATE_IF_EXT( id, "VK_KHR_external_fence_capabilities" )
#endif
#ifdef VK_EXT_INLINE_UNIFORM_BLOCK_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( inline_uniform_block, "VK_EXT_inline_uniform_block" )
#endif
#ifdef VK_EXT_LINE_RASTERIZATION_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( line_rasterization, "VK_EXT_line_rasterization" )
#endif
#if defined(VK_KHR_MAINTENANCE3_EXTENSION_NAME) || defined(VK_VERSION_1_1)
      LIBGCT_EXTENSION_CREATE_IF_VERSION_OR_EXT( maintenance3, 1, 1, 0, "VK_KHR_maintenance3" )
#endif
#ifdef VK_NV_MESH_SHADER_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( mesh_shader, "VK_NV_mesh_shader" )
#endif
#ifdef VK_EXT_MULTI_DRAW_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( multi_draw, "VK_EXT_multi_draw" )
#endif
#ifdef VK_NVX_MULTIVIEW_PER_VIEW_ATTRIBUTES_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( multiview_per_view_attributes, "VK_NVX_multiview_per_view_attributes" )
#endif
#if defined(VK_KHR_MULTIVIEW_EXTENSION_NAME) || defined(VK_VERSION_1_1)
      LIBGCT_EXTENSION_CREATE_IF_VERSION_OR_EXT( multiview, 1, 1, 0, "VK_KHR_multiview" )
#endif
#ifdef VK_EXT_PCI_BUS_INFO_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( pci_bus_info, "VK_EXT_pci_bus_info" )
#endif
#ifdef VK_KHR_PERFORMANCE_QUERY_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( performance_query, "VK_KHR_performance_query" )
#endif
#if defined(VK_KHR_MAINTENANCE2_EXTENSION_NAME) || defined(VK_VERSION_1_1)
      LIBGCT_EXTENSION_CREATE_IF_VERSION_OR_EXT( point_clipping, 1, 1, 0, "VK_KHR_maintenance2" )
#endif
#ifdef VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( portability_subset, "VK_KHR_portability_subset" )
#endif
#ifdef VK_VERSION_1_1
      LIBGCT_EXTENSION_CREATE_IF_VERSION( protected_memory, 1, 1, 0 )
#endif
#ifdef VK_EXT_PROVOKING_VERTEX_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( provoking_vertex, "VK_EXT_provoking_vertex" )
#endif
#ifdef VK_KHR_PUSH_DESCRIPTOR_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( push_descriptor, "VK_KHR_push_descriptor" )
#endif
#ifdef VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( ray_tracing_pipeline, "VK_KHR_ray_tracing_pipeline" )
#endif
#ifdef VK_NV_RAY_TRACING_MOTION_BLUR_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( ray_tracing, "VK_NV_ray_tracing" )
#endif
#ifdef VK_EXT_ROBUSTNESS_2_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( robustness2, "VK_EXT_robustness2" )
#endif
#ifdef VK_EXT_SAMPLE_LOCATIONS_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( sample_locations, "VK_EXT_sample_locations" )
#endif
#if defined(VK_EXT_SAMPLER_FILTER_MINMAX_EXTENSION_NAME) || defined(VK_VERSION_1_2)
      LIBGCT_EXTENSION_CREATE_IF_VERSION_OR_EXT( sampler_filter_minmax, 1, 2, 0, "VK_EXT_sampler_filter_minmax" )
#endif
#ifdef VK_AMD_SHADER_CORE_PROPERTIES_2_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( shader_core2, "VK_AMD_shader_core_properties2" )
#endif
#ifdef VK_KHR_SHADER_INTEGER_DOT_PRODUCT_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( shader_integer_dot_product, "VK_KHR_shader_integer_dot_product" )
#endif
#ifdef VK_NV_SHADER_SM_BUILTINS_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( shader_sm_builtins, "VK_NV_shader_sm_builtins" )
#endif
#ifdef VK_NV_SHADING_RATE_IMAGE_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( shading_rate_image, "VK_NV_shading_rate_image" )
#endif
#ifdef VK_VERSION_1_1
      LIBGCT_EXTENSION_CREATE_IF_VERSION( subgroup, 1, 1, 0 )
#endif
#ifdef VK_EXT_SUBGROUP_SIZE_CONTROL_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( subgroup_size_control, "VK_EXT_subgroup_size_control" )
#endif
#ifdef VK_HUAWEI_SUBPASS_SHADING_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( subpass_shading, "VK_HUAWEI_subpass_shading" )
#endif
#ifdef VK_EXT_TEXEL_BUFFER_ALIGNMENT_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( texel_buffer_alignment, "VK_EXT_texel_buffer_alignment" )
#endif
#if defined(VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME) || defined(VK_VERSION_1_2)
      LIBGCT_EXTENSION_CREATE_IF_VERSION_OR_EXT( timeline_semaphore, 1, 2, 0, "VK_KHR_timeline_semaphore" )
#endif
#ifdef VK_EXT_TRANSFORM_FEEDBACK_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( transform_feedback, "VK_EXT_transform_feedback" )
#endif
#ifdef VK_EXT_VERTEX_ATTRIBUTE_DIVISOR_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( vertex_attribute_divisor, "VK_EXT_vertex_attribute_divisor" )
#endif
      rebuild_chain();
#ifdef VK_VERSION_1_1
      pdev.getProperties2(
        &basic
      );
#elif defined(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME)
      pdev.getProperties2KHR(
        &basic
      );
#endif
      {
        std::uint32_t queue_family_count = 0u;
        pdev.getQueueFamilyProperties2( &queue_family_count, nullptr );
        queue_family.resize( queue_family_count );
        for( auto &q : queue_family )
          q.activate( activated_extensions ).rebuild_chain();
        std::vector< vk::QueueFamilyProperties2 > root;
        root.reserve( queue_family_count );
        std::transform(
          queue_family.begin(),
          queue_family.end(),
          std::back_inserter( root ),
          []( const auto &v ) { return v.get_basic2(); }
        );
        pdev.getQueueFamilyProperties2( &queue_family_count, root.data() );
        std::copy(
          root.begin(),
          root.end(),
          queue_family.begin()
        );
      }
    }
    else {
      pdev.getProperties(
        &basic.properties
      );
      {
        std::uint32_t queue_family_count = 0u;
        pdev.getQueueFamilyProperties2( &queue_family_count, nullptr );
        queue_family.resize( queue_family_count );
        std::vector< vk::QueueFamilyProperties > root( queue_family_count );
        pdev.getQueueFamilyProperties( &queue_family_count, root.data() );
        std::copy(
          root.begin(),
          root.end(),
          queue_family.begin()
        );
      }
    }
#else
    {
      pdev.getProperties(
        &basic
      );
      {
        std::uint32_t queue_family_count = 0u;
        pdev.getQueueFamilyProperties2( &queue_family_count, nullptr );
        queue_family.resize( queue_family_count );
        std::vector< vk::QueueFamilyProperties > root( queue_family_count );
        pdev.getQueueFamilyProperties( &queue_family_count, root.data() );
        std::copy(
          root.begin(),
          root.end(),
          queue_family.begin()
        );
      }
    }
#endif
#ifdef VK_KHR_DISPLAY_EXTENSION_NAME
    if( instance.get_activated_extensions().find( "VK_KHR_display" ) != instance.get_activated_extensions().end() ) {
      for( const auto &d: pdev.getDisplayPropertiesKHR() ) {
        display.push_back(
          display_properties_t()
        );
        display.back().basic = d;
        for( const auto &m: pdev.getDisplayModePropertiesKHR( d.display ) ) {
          display.back().modes.push_back(
            display_mode_t()
          );
          display.back().modes.back().basic = m;
        }
      }
      std::uint32_t plane_index = 0u;
      for( const auto &p: pdev.getDisplayPlanePropertiesKHR() ) {
        for( auto &d: display ) {
          if( p.currentDisplay == d.basic.display ) {
            d.planes.push_back( plane_index );
          }
        }
        ++plane_index;
      }
      for( auto &d: display ) {
        for( auto &m: d.modes ) {
          for( const auto &p: d.planes ) {
            auto iter = m.capabilities.insert(
              std::make_pair(
                p,
                display_plane_t()
              )
            ).first;
            iter->second.basic = pdev.getDisplayPlaneCapabilitiesKHR( m.basic.displayMode, p );
          }
        }
      }
    }
#endif
    memory_props.reset( new physical_device_memory_properties_t(
      instance,
      pdev,
      activated_extensions
    ) );
  }
  
  physical_device_properties_t &physical_device_properties_t::rebuild_chain() {
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
#ifdef VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( acceleration_structure )
#endif
#ifdef VK_EXT_BLEND_OPERATION_ADVANCED_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( blend_operation )
#endif
#ifdef VK_EXT_CONSERVATIVE_RASTERIZATION_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( conservative_rasterization )
#endif
#ifdef VK_NV_COOPERATIVE_MATRIX_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( cooperative_matrix )
#endif
#ifdef VK_EXT_CUSTOM_BORDER_COLOR_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( custom_border_color )
#endif
#if defined(VK_KHR_DEPTH_STENCIL_RESOLVE_EXTENSION_NAME) || defined(VK_VERSION_1_2)
      LIBGCT_EXTENSION_REBUILD_CHAIN( depth_stencil_resolve )
#endif
#if defined(VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME) || defined(VK_VERSION_1_2)
      LIBGCT_EXTENSION_REBUILD_CHAIN( device_descriptor_indexing )
#endif
#ifdef VK_NV_DEVICE_GENERATED_COMMANDS_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( device_generated_commands )
#endif
#ifdef VK_EXT_DISCARD_RECTANGLES_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( discard_rectangle )
#endif
#if defined(VK_KHR_DRIVER_PROPERTIES_EXTENSION_NAME) || defined(VK_VERSION_1_2)
      LIBGCT_EXTENSION_REBUILD_CHAIN( driver )
#endif
#ifdef VK_EXT_PHYSICAL_DEVICE_DRM_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( drm )
#endif
#ifdef VK_EXT_EXTERNAL_MEMORY_HOST_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( external_memory_host )
#endif
#if defined(VK_KHR_SHADER_FLOAT_CONTROLS_EXTENSION_NAME) || defined(VK_VERSION_1_2)
      LIBGCT_EXTENSION_REBUILD_CHAIN( float_controls )
#endif
#ifdef VK_EXT_FRAGMENT_DENSITY_MAP_2_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( fragment_density_map2 )
#endif
#ifdef VK_EXT_FRAGMENT_DENSITY_MAP_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( fragment_density_map )
#endif
#ifdef VK_NV_FRAGMENT_SHADING_RATE_ENUMS_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( fragment_shading_rate_enums )
#endif
#ifdef VK_KHR_FRAGMENT_SHADING_RATE_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( fragment_shading_rate )
#endif
#if defined(VK_KHR_external_memory_capabilities) || defined(VK_VERSION_1_1) || defined(VK_KHR_EXTERNAL_SEMAPHORE_CAPABILITIES_EXTENSION_NAME) || defined(VK_KHR_EXTERNAL_FENCE_CAPABILITIES_EXTENSION_NAME)
      LIBGCT_EXTENSION_REBUILD_CHAIN( id )
#endif
#ifdef VK_EXT_INLINE_UNIFORM_BLOCK_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( inline_uniform_block )
#endif
#ifdef VK_EXT_LINE_RASTERIZATION_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( line_rasterization )
#endif
#if defined(VK_KHR_MAINTENANCE3_EXTENSION_NAME) || defined(VK_VERSION_1_1)
      LIBGCT_EXTENSION_REBUILD_CHAIN( maintenance3 )
#endif
#ifdef VK_NV_MESH_SHADER_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( mesh_shader )
#endif
#ifdef VK_EXT_MULTI_DRAW_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( multi_draw )
#endif
#ifdef VK_NVX_MULTIVIEW_PER_VIEW_ATTRIBUTES_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( multiview_per_view_attributes )
#endif
#if defined(VK_KHR_MULTIVIEW_EXTENSION_NAME) || defined(VK_VERSION_1_1)
      LIBGCT_EXTENSION_REBUILD_CHAIN( multiview )
#endif
#ifdef VK_EXT_PCI_BUS_INFO_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( pci_bus_info )
#endif
#ifdef VK_KHR_PERFORMANCE_QUERY_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( performance_query )
#endif
#if defined(VK_KHR_MAINTENANCE2_EXTENSION_NAME) || defined(VK_VERSION_1_1)
      LIBGCT_EXTENSION_REBUILD_CHAIN( point_clipping )
#endif
#ifdef VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( portability_subset )
#endif
#ifdef VK_VERSION_1_1
      LIBGCT_EXTENSION_REBUILD_CHAIN( protected_memory )
#endif
#ifdef VK_EXT_PROVOKING_VERTEX_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( provoking_vertex )
#endif
#ifdef VK_KHR_PUSH_DESCRIPTOR_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( push_descriptor )
#endif
#ifdef VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( ray_tracing_pipeline )
#endif
#ifdef VK_NV_RAY_TRACING_MOTION_BLUR_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( ray_tracing )
#endif
#ifdef VK_EXT_ROBUSTNESS_2_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( robustness2 )
#endif
#ifdef VK_EXT_SAMPLE_LOCATIONS_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( sample_locations )
#endif
#if defined(VK_EXT_SAMPLER_FILTER_MINMAX_EXTENSION_NAME) || defined(VK_VERSION_1_2)
      LIBGCT_EXTENSION_REBUILD_CHAIN( sampler_filter_minmax )
#endif
#ifdef VK_AMD_SHADER_CORE_PROPERTIES_2_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( shader_core2 )
#endif
#ifdef VK_KHR_SHADER_INTEGER_DOT_PRODUCT_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( shader_integer_dot_product )
#endif
#ifdef VK_NV_SHADER_SM_BUILTINS_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( shader_sm_builtins )
#endif
#ifdef VK_NV_SHADING_RATE_IMAGE_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( shading_rate_image )
#endif
#ifdef VK_VERSION_1_1
      LIBGCT_EXTENSION_REBUILD_CHAIN( subgroup )
#endif
#ifdef VK_EXT_SUBGROUP_SIZE_CONTROL_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( subgroup_size_control )
#endif
#ifdef VK_HUAWEI_SUBPASS_SHADING_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( subpass_shading )
#endif
#ifdef VK_EXT_TEXEL_BUFFER_ALIGNMENT_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( texel_buffer_alignment )
#endif
#if defined(VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME) || defined(VK_VERSION_1_2)
      LIBGCT_EXTENSION_REBUILD_CHAIN( timeline_semaphore )
#endif
#ifdef VK_EXT_TRANSFORM_FEEDBACK_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( transform_feedback )
#endif
#ifdef VK_EXT_VERTEX_ATTRIBUTE_DIVISOR_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( vertex_attribute_divisor )
#endif
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }

}

