#include <gct/spv2json.hpp>
#include <nlohmann/json.hpp>
#include <gct/spirv_reflect.h>

void to_json( nlohmann::json &root, const SpvReflectGenerator &v ) {
  if( v == SPV_REFLECT_GENERATOR_KHRONOS_LLVM_SPIRV_TRANSLATOR )
    root = "SPV_REFLECT_GENERATOR_KHRONOS_LLVM_SPIRV_TRANSLATOR";
  else if( v == SPV_REFLECT_GENERATOR_KHRONOS_LLVM_SPIRV_TRANSLATOR )
    root = "SPV_REFLECT_GENERATOR_KHRONOS_LLVM_SPIRV_TRANSLATOR";
  else if( v == SPV_REFLECT_GENERATOR_KHRONOS_GLSLANG_REFERENCE_FRONT_END )
    root = "SPV_REFLECT_GENERATOR_KHRONOS_GLSLANG_REFERENCE_FRONT_END";
  else if( v == SPV_REFLECT_GENERATOR_GOOGLE_SHADERC_OVER_GLSLANG )
    root = "SPV_REFLECT_GENERATOR_GOOGLE_SHADERC_OVER_GLSLANG";
  else if( v == SPV_REFLECT_GENERATOR_GOOGLE_SPIREGG )
    root = "SPV_REFLECT_GENERATOR_GOOGLE_SPIREGG";
  else if( v == SPV_REFLECT_GENERATOR_GOOGLE_RSPIRV )
    root = "SPV_REFLECT_GENERATOR_GOOGLE_RSPIRV";
  else if( v == SPV_REFLECT_GENERATOR_X_LEGEND_MESA_MESAIR_SPIRV_TRANSLATOR )
    root = "SPV_REFLECT_GENERATOR_X_LEGEND_MESA_MESAIR_SPIRV_TRANSLATOR";
  else if( v == SPV_REFLECT_GENERATOR_KHRONOS_SPIRV_TOOLS_LINKER )
    root = "SPV_REFLECT_GENERATOR_KHRONOS_SPIRV_TOOLS_LINKER";
  else if( v == SPV_REFLECT_GENERATOR_WINE_VKD3D_SHADER_COMPILER )
    root = "SPV_REFLECT_GENERATOR_WINE_VKD3D_SHADER_COMPILER";
  else if( v == SPV_REFLECT_GENERATOR_CLAY_CLAY_SHADER_COMPILER )
    root = "SPV_REFLECT_GENERATOR_CLAY_CLAY_SHADER_COMPILER";
  else
    root = "Unknown";
}

void to_json( nlohmann::json &root, const SpvExecutionModel &v ) {
  if( v == SpvExecutionModelVertex )
    root = "Vertex";
  else if( v == SpvExecutionModelTessellationControl )
    root = "TessellationControl";
  else if( v == SpvExecutionModelTessellationEvaluation )
    root = "TessellationEvaluation";
  else if( v == SpvExecutionModelGeometry )
    root = "Geometry";
  else if( v == SpvExecutionModelFragment )
    root = "Fragment";
  else if( v == SpvExecutionModelGLCompute )
    root = "GLCompute";
  else if( v == SpvExecutionModelKernel )
    root = "Kernel";
  else if( v == SpvExecutionModelTaskNV )
    root = "TaskNV";
  else if( v == SpvExecutionModelMeshNV )
    root = "MeshNV";
  else if( v == SpvExecutionModelRayGenerationKHR )
    root = "RayGenerationKHR";
  else if( v == SpvExecutionModelRayGenerationNV )
    root = "RayGenerationNV";
  else if( v == SpvExecutionModelIntersectionKHR )
    root = "IntersectionKHR";
  else if( v == SpvExecutionModelIntersectionNV )
    root = "IntersectionNV";
  else if( v == SpvExecutionModelAnyHitKHR )
    root = "AnyHitKHR";
  else if( v == SpvExecutionModelAnyHitNV )
    root = "AnyHitNV";
  else if( v == SpvExecutionModelClosestHitKHR )
    root = "ClosestHitKHR";
  else if( v == SpvExecutionModelClosestHitNV )
    root = "ClosestHitNV";
  else if( v == SpvExecutionModelMissKHR )
    root = "MissKHR";
  else if( v == SpvExecutionModelMissNV )
    root = "MissNV";
  else if( v == SpvExecutionModelCallableKHR )
    root = "CallableKHR";
  else if( v == SpvExecutionModelCallableNV )
    root = "CallableNV";
  else
    root = "Unknown";
}
void to_json( nlohmann::json &root, const SpvReflectShaderStageFlagBits &v ) {
  if( v == SPV_REFLECT_SHADER_STAGE_VERTEX_BIT )
    root = "Vertex";
  else if( v == SPV_REFLECT_SHADER_STAGE_TESSELLATION_CONTROL_BIT )
    root = "TesselationControl";
  else if( v == SPV_REFLECT_SHADER_STAGE_TESSELLATION_EVALUATION_BIT )
    root = "TesselationEvaluation";
  else if( v == SPV_REFLECT_SHADER_STAGE_GEOMETRY_BIT )
    root = "Geometry";
  else if( v == SPV_REFLECT_SHADER_STAGE_FRAGMENT_BIT )
    root = "Fragment";
  else if( v == SPV_REFLECT_SHADER_STAGE_COMPUTE_BIT )
    root = "Compute";
  else if( v == SPV_REFLECT_SHADER_STAGE_TASK_BIT_NV )
    root = "TaskNV";
  else if( v == SPV_REFLECT_SHADER_STAGE_MESH_BIT_NV )
    root = "MeshNV";
  else if( v == SPV_REFLECT_SHADER_STAGE_RAYGEN_BIT_KHR )
    root = "RayGenerationKHR";
  else if( v == SPV_REFLECT_SHADER_STAGE_ANY_HIT_BIT_KHR )
    root = "AnyHitKHR";
  else if( v == SPV_REFLECT_SHADER_STAGE_CLOSEST_HIT_BIT_KHR )
    root = "ClosestHitKHR";
  else if( v == SPV_REFLECT_SHADER_STAGE_MISS_BIT_KHR )
    root = "MissKHR";
  else if( v == SPV_REFLECT_SHADER_STAGE_INTERSECTION_BIT_KHR )
    root = "IntersectionKHR";
  else if( v == SPV_REFLECT_SHADER_STAGE_CALLABLE_BIT_KHR )
    root = "CallableKHR";
  else
    root = "Unknown";
}

void to_json( nlohmann::json &root, const SpvStorageClass &v ) {
  if( v == SpvStorageClassUniformConstant )
    root = "UniformConstant";
  else if( v == SpvStorageClassInput )
    root = "Input";
  else if( v == SpvStorageClassUniform )
    root = "Uniform";
  else if( v == SpvStorageClassOutput )
    root = "Output";
  else if( v == SpvStorageClassWorkgroup )
    root = "Workgroup";
  else if( v == SpvStorageClassCrossWorkgroup )
    root = "CrossWorkgroup";
  else if( v == SpvStorageClassPrivate )
    root = "Private";
  else if( v == SpvStorageClassFunction )
    root = "Function";
  else if( v == SpvStorageClassGeneric )
    root = "Generic";
  else if( v == SpvStorageClassPushConstant )
    root = "PushConstant";
  else if( v == SpvStorageClassAtomicCounter )
    root = "AtomicCounter";
  else if( v == SpvStorageClassImage )
    root = "Image";
  else if( v == SpvStorageClassStorageBuffer )
    root = "StorageBuffer";
  else if( v == SpvStorageClassCallableDataKHR )
    root = "CallableDataKHR";
  else if( v == SpvStorageClassCallableDataNV )
    root = "CallableDataNV";
  else if( v == SpvStorageClassIncomingCallableDataKHR )
    root = "IncomingCallableDataKHR";
  else if( v == SpvStorageClassIncomingCallableDataNV )
    root = "IncomingCallableDataNV";
  else if( v == SpvStorageClassRayPayloadKHR )
    root = "RayPayloadKHR";
  else if( v == SpvStorageClassRayPayloadNV )
    root = "RayPayloadNV";
  else if( v == SpvStorageClassHitAttributeKHR )
    root = "HitAttributeKHR";
  else if( v == SpvStorageClassHitAttributeNV )
    root = "HitAttributeNV";
  else if( v == SpvStorageClassIncomingRayPayloadKHR )
    root = "IncomingRayPayloadKHR";
  else if( v == SpvStorageClassIncomingRayPayloadNV )
    root = "IncomingRayPayloadNV";
  else if( v == SpvStorageClassShaderRecordBufferKHR )
    root = "ShaderRecordBufferKHR";
  else if( v == SpvStorageClassShaderRecordBufferNV )
    root = "ShaderRecordBufferNV";
  else if( v == SpvStorageClassPhysicalStorageBuffer )
    root = "PhysicalStorageBuffer";
  else if( v == SpvStorageClassPhysicalStorageBufferEXT )
    root = "PhysicalStorageBufferEXT";
  else if( v == SpvStorageClassCodeSectionINTEL )
    root = "CodeSectionINTEL";
  else
    root = "Unknown";
}

void to_json( nlohmann::json &root, const SpvBuiltIn &v ) {
  if( v == SpvBuiltInPosition )
    root = "Position";
  else if( v == SpvBuiltInPointSize )
    root = "PointSize";
  else if( v == SpvBuiltInClipDistance )
    root = "ClipDistance";
  else if( v == SpvBuiltInCullDistance )
    root = "CullDistance";
  else if( v == SpvBuiltInVertexId )
    root = "VertexId";
  else if( v == SpvBuiltInInstanceId )
    root = "InstanceId";
  else if( v == SpvBuiltInPrimitiveId )
    root = "PrimitiveId";
  else if( v == SpvBuiltInInvocationId )
    root = "InvocationId";
  else if( v == SpvBuiltInLayer )
    root = "Layer";
  else if( v == SpvBuiltInViewportIndex )
    root = "ViewportIndex";
  else if( v == SpvBuiltInTessLevelOuter )
    root = "TessLevelOuter";
  else if( v == SpvBuiltInTessLevelInner )
    root = "TessLevelInner";
  else if( v == SpvBuiltInTessCoord )
    root = "TessCoord";
  else if( v == SpvBuiltInPatchVertices )
    root = "PatchVertices";
  else if( v == SpvBuiltInFragCoord )
    root = "FragCoord";
  else if( v == SpvBuiltInPointCoord )
    root = "PointCoord";
  else if( v == SpvBuiltInFrontFacing )
    root = "FrontFacing";
  else if( v == SpvBuiltInSampleId )
    root = "SampleId";
  else if( v == SpvBuiltInSamplePosition )
    root = "SamplePosition";
  else if( v == SpvBuiltInSampleMask )
    root = "SampleMask";
  else if( v == SpvBuiltInFragDepth )
    root = "FragDepth";
  else if( v == SpvBuiltInHelperInvocation )
    root = "HelperInvocation";
  else if( v == SpvBuiltInNumWorkgroups )
    root = "NumWorkgroups";
  else if( v == SpvBuiltInWorkgroupSize )
    root = "WorkgroupSize";
  else if( v == SpvBuiltInWorkgroupId )
    root = "WorkgroupId";
  else if( v == SpvBuiltInLocalInvocationId )
    root = "LocalInvocationId";
  else if( v == SpvBuiltInGlobalInvocationId )
    root = "GlobalInvocationId";
  else if( v == SpvBuiltInLocalInvocationIndex )
    root = "LocalInvocationIndex";
  else if( v == SpvBuiltInWorkDim )
    root = "WorkDim";
  else if( v == SpvBuiltInGlobalSize )
    root = "GlobalSize";
  else if( v == SpvBuiltInEnqueuedWorkgroupSize )
    root = "EnqueuedWorkgroupSize";
  else if( v == SpvBuiltInGlobalOffset )
    root = "GlobalOffset";
  else if( v == SpvBuiltInGlobalLinearId )
    root = "GlobalLinearId";
  else if( v == SpvBuiltInSubgroupSize )
    root = "SubgroupSize";
  else if( v == SpvBuiltInSubgroupMaxSize )
    root = "SubgroupMaxSize";
  else if( v == SpvBuiltInNumSubgroups )
    root = "NumSubgroups";
  else if( v == SpvBuiltInNumEnqueuedSubgroups )
    root = "NumEnqueuedSubgroups";
  else if( v == SpvBuiltInSubgroupId )
    root = "SubgroupId";
  else if( v == SpvBuiltInSubgroupLocalInvocationId )
    root = "SubgroupLocalInvocationId";
  else if( v == SpvBuiltInVertexIndex )
    root = "VertexIndex";
  else if( v == SpvBuiltInInstanceIndex )
    root = "InstanceIndex";
  else if( v == SpvBuiltInSubgroupEqMask )
    root = "SubgroupEqMask";
  else if( v == SpvBuiltInSubgroupEqMaskKHR )
    root = "SubgroupEqMaskKHR";
  else if( v == SpvBuiltInSubgroupGeMask )
    root = "SubgroupGeMask";
  else if( v == SpvBuiltInSubgroupGeMaskKHR )
    root = "SubgroupGeMaskKHR";
  else if( v == SpvBuiltInSubgroupGtMask )
    root = "SubgroupGtMask";
  else if( v == SpvBuiltInSubgroupGtMaskKHR )
    root = "SubgroupGtMaskKHR";
  else if( v == SpvBuiltInSubgroupLeMask )
    root = "SubgroupLeMask";
  else if( v == SpvBuiltInSubgroupLeMaskKHR )
    root = "SubgroupLeMaskKHR";
  else if( v == SpvBuiltInSubgroupLtMask )
    root = "SubgroupLtMask";
  else if( v == SpvBuiltInSubgroupLtMaskKHR )
    root = "SubgroupLtMaskKHR";
  else if( v == SpvBuiltInBaseVertex )
    root = "BaseVertex";
  else if( v == SpvBuiltInBaseInstance )
    root = "BaseInstance";
  else if( v == SpvBuiltInDrawIndex )
    root = "DrawIndex";
  else if( v == SpvBuiltInPrimitiveShadingRateKHR )
    root = "PrimitiveShadingRateKHR";
  else if( v == SpvBuiltInDeviceIndex )
    root = "DeviceIndex";
  else if( v == SpvBuiltInViewIndex )
    root = "ViewIndex";
  else if( v == SpvBuiltInShadingRateKHR )
    root = "ShadingRateKHR";
  else if( v == SpvBuiltInBaryCoordNoPerspAMD )
    root = "BaryCoordNoPerspAMD";
  else if( v == SpvBuiltInBaryCoordNoPerspCentroidAMD )
    root = "BaryCoordNoPerspCentroidAMD";
  else if( v == SpvBuiltInBaryCoordNoPerspSampleAMD )
    root = "BaryCoordNoPerspSampleAMD";
  else if( v == SpvBuiltInBaryCoordSmoothAMD )
    root = "BaryCoordSmoothAMD";
  else if( v == SpvBuiltInBaryCoordSmoothCentroidAMD )
    root = "BaryCoordSmoothCentroidAMD";
  else if( v == SpvBuiltInBaryCoordSmoothSampleAMD )
    root = "BaryCoordSmoothSampleAMD";
  else if( v == SpvBuiltInBaryCoordPullModelAMD )
    root = "BaryCoordPullModelAMD";
  else if( v == SpvBuiltInFragStencilRefEXT )
    root = "FragStencilRefEXT";
  else if( v == SpvBuiltInViewportMaskNV )
    root = "ViewportMaskNV";
  else if( v == SpvBuiltInSecondaryPositionNV )
    root = "SecondaryPositionNV";
  else if( v == SpvBuiltInSecondaryViewportMaskNV )
    root = "SecondaryViewportMaskNV";
  else if( v == SpvBuiltInPositionPerViewNV )
    root = "PositionPerViewNV";
  else if( v == SpvBuiltInViewportMaskPerViewNV )
    root = "ViewportMaskPerViewNV";
  else if( v == SpvBuiltInFullyCoveredEXT )
    root = "FullyCoveredEXT";
  else if( v == SpvBuiltInTaskCountNV )
    root = "TaskCountNV";
  else if( v == SpvBuiltInPrimitiveCountNV )
    root = "PrimitiveCountNV";
  else if( v == SpvBuiltInPrimitiveIndicesNV )
    root = "PrimitiveIndicesNV";
  else if( v == SpvBuiltInClipDistancePerViewNV )
    root = "ClipDistancePerViewNV";
  else if( v == SpvBuiltInCullDistancePerViewNV )
    root = "CullDistancePerViewNV";
  else if( v == SpvBuiltInLayerPerViewNV )
    root = "LayerPerViewNV";
  else if( v == SpvBuiltInMeshViewCountNV )
    root = "MeshViewCountNV";
  else if( v == SpvBuiltInMeshViewIndicesNV )
    root = "MeshViewIndicesNV";
  else if( v == SpvBuiltInBaryCoordNV )
    root = "BaryCoordNV";
  else if( v == SpvBuiltInBaryCoordNoPerspNV )
    root = "BaryCoordNoPerspNV";
  else if( v == SpvBuiltInFragSizeEXT )
    root = "FragSizeEXT";
  else if( v == SpvBuiltInFragmentSizeNV )
    root = "FragmentSizeNV";
  else if( v == SpvBuiltInFragInvocationCountEXT )
    root = "FragInvocationCountEXT";
  else if( v == SpvBuiltInInvocationsPerPixelNV )
    root = "InvocationsPerPixelNV";
  else if( v == SpvBuiltInLaunchIdKHR )
    root = "LaunchIdKHR";
  else if( v == SpvBuiltInLaunchIdNV )
    root = "LaunchIdNV";
  else if( v == SpvBuiltInLaunchSizeKHR )
    root = "LaunchSizeKHR";
  else if( v == SpvBuiltInLaunchSizeNV )
    root = "LaunchSizeNV";
  else if( v == SpvBuiltInWorldRayOriginKHR )
    root = "WorldRayOriginKHR";
  else if( v == SpvBuiltInWorldRayOriginNV )
    root = "WorldRayOriginNV";
  else if( v == SpvBuiltInWorldRayDirectionKHR )
    root = "WorldRayDirectionKHR";
  else if( v == SpvBuiltInWorldRayDirectionNV )
    root = "WorldRayDirectionNV";
  else if( v == SpvBuiltInObjectRayOriginKHR )
    root = "ObjectRayOriginKHR";
  else if( v == SpvBuiltInObjectRayOriginNV )
    root = "ObjectRayOriginNV";
  else if( v == SpvBuiltInObjectRayDirectionKHR )
    root = "ObjectRayDirectionKHR";
  else if( v == SpvBuiltInObjectRayDirectionNV )
    root = "ObjectRayDirectionNV";
  else if( v == SpvBuiltInRayTminKHR )
    root = "RayTminKHR";
  else if( v == SpvBuiltInRayTminNV )
    root = "RayTminNV";
  else if( v == SpvBuiltInRayTmaxKHR )
    root = "RayTmaxKHR";
  else if( v == SpvBuiltInRayTmaxNV )
    root = "RayTmaxNV";
  else if( v == SpvBuiltInInstanceCustomIndexKHR )
    root = "InstanceCustomIndexKHR";
  else if( v == SpvBuiltInInstanceCustomIndexNV )
    root = "InstanceCustomIndexNV";
  else if( v == SpvBuiltInObjectToWorldKHR )
    root = "ObjectToWorldKHR";
  else if( v == SpvBuiltInObjectToWorldNV )
    root = "ObjectToWorldNV";
  else if( v == SpvBuiltInWorldToObjectKHR )
    root = "WorldToObjectKHR";
  else if( v == SpvBuiltInWorldToObjectNV )
    root = "WorldToObjectNV";
  else if( v == SpvBuiltInHitTNV )
    root = "HitTNV";
  else if( v == SpvBuiltInHitKindKHR )
    root = "HitKindKHR";
  else if( v == SpvBuiltInHitKindNV )
    root = "HitKindNV";
  else if( v == SpvBuiltInIncomingRayFlagsKHR )
    root = "IncomingRayFlagsKHR";
  else if( v == SpvBuiltInIncomingRayFlagsNV )
    root = "IncomingRayFlagsNV";
  else if( v == SpvBuiltInRayGeometryIndexKHR )
    root = "RayGeometryIndexKHR";
  else if( v == SpvBuiltInWarpsPerSMNV )
    root = "WarpsPerSMNV";
  else if( v == SpvBuiltInSMCountNV )
    root = "SMCountNV";
  else if( v == SpvBuiltInWarpIDNV )
    root = "WarpIDNV";
  else if( v == SpvBuiltInSMIDNV )
    root = "SMIDNV";
  else
    root = "Unknown";
}

void to_json( nlohmann::json &root, const SpvReflectNumericTraits &v )  {
  root = nlohmann::json::object();
  root[ "scalar" ] = nlohmann::json::object();
  root[ "scalar" ][ "width" ] = v.scalar.width;
  root[ "scalar" ][ "signedness" ] = v.scalar.signedness;
  root[ "vector" ] = nlohmann::json::object();
  root[ "vector"][ "component_count" ] = v.vector.component_count;
  root[ "matrix" ] = nlohmann::json::object();
  root[ "matrix" ][ "column_count" ] = v.matrix.column_count;
  root[ "matrix" ][ "row_count" ] = v.matrix.row_count;
  root[ "matrix" ][ "stride" ] = v.matrix.stride;
}

void to_json( nlohmann::json &root, const SpvReflectArrayTraits &v ) {
  root = nlohmann::json::object();
  root[ "dims_count" ] = v.dims_count;
  root[ "dims" ] = nlohmann::json::array();
  for( unsigned int i = 0u; i != SPV_REFLECT_MAX_ARRAY_DIMS; ++i ) {
    if( !v.dims[ i ] ) break;
    root[ "dims" ].push_back( nlohmann::json( v.dims[ i ] ) );
  }
  root[ "stride" ] = v.stride;
}

void to_json( nlohmann::json &root, const SpvReflectFormat &v ) {
  if( v == SPV_REFLECT_FORMAT_UNDEFINED )
    root = "UNDEFINED";
  else if( v == SPV_REFLECT_FORMAT_R32_UINT )
    root = "R32_UINT";
  else if( v == SPV_REFLECT_FORMAT_R32_SINT )
    root = "R32_SINT";
  else if( v == SPV_REFLECT_FORMAT_R32_SFLOAT )
    root = "R32_SFLOAT";
  else if( v == SPV_REFLECT_FORMAT_R32G32_UINT )
    root = "R32G32_UINT";
  else if( v == SPV_REFLECT_FORMAT_R32G32_SINT )
    root = "R32G32_SINT";
  else if( v == SPV_REFLECT_FORMAT_R32G32_SFLOAT )
    root = "R32G32_SFLOAT";
  else if( v == SPV_REFLECT_FORMAT_R32G32B32_UINT )
    root = "R32G32B32_UINT";
  else if( v == SPV_REFLECT_FORMAT_R32G32B32_SINT )
    root = "R32G32B32_SINT";
  else if( v == SPV_REFLECT_FORMAT_R32G32B32_SFLOAT )
    root = "R32G32B32_SFLOAT";
  else if( v == SPV_REFLECT_FORMAT_R32G32B32A32_UINT )
    root = "R32G32B32A32_UINT";
  else if( v == SPV_REFLECT_FORMAT_R32G32B32A32_SINT )
    root = "R32G32B32A32_SINT";
  else if( v == SPV_REFLECT_FORMAT_R32G32B32A32_SFLOAT )
    root = "R32G32B32A32_SFLOAT";
  else if( v == SPV_REFLECT_FORMAT_R64_UINT )
    root = "R64_UINT";
  else if( v == SPV_REFLECT_FORMAT_R64_SINT )
    root = "R64_SINT";
  else if( v == SPV_REFLECT_FORMAT_R64_SFLOAT )
    root = "R64_SFLOAT";
  else if( v == SPV_REFLECT_FORMAT_R64G64_UINT )
    root = "R64G64_UINT";
  else if( v == SPV_REFLECT_FORMAT_R64G64_SINT )
    root = "R64G64_SINT";
  else if( v == SPV_REFLECT_FORMAT_R64G64_SFLOAT )
    root = "R64G64_SFLOAT";
  else if( v == SPV_REFLECT_FORMAT_R64G64B64_UINT )
    root = "R64G64B64_UINT";
  else if( v == SPV_REFLECT_FORMAT_R64G64B64_SINT )
    root = "R64G64B64_SINT";
  else if( v == SPV_REFLECT_FORMAT_R64G64B64_SFLOAT )
    root = "R64G64B64_SFLOAT";
  else if( v == SPV_REFLECT_FORMAT_R64G64B64A64_UINT )
    root = "R64G64B64A64_UINT";
  else if( v == SPV_REFLECT_FORMAT_R64G64B64A64_SINT )
    root = "R64G64B64A64_SINT";
  else
    root = "Unknown";
}

void to_json( nlohmann::json &root, const SpvReflectInterfaceVariable &v ) {
  root = nlohmann::json::object();
  root[ "spirv_id" ] = v.spirv_id;
  if( v.name )
    root[ "name" ] = v.name;
  root[ "location" ] = v.location;
  root[ "storage_class" ] = v.storage_class;
  if( v.semantic )
    root[ "semantic" ] = v.semantic;
  root[ "decoration_flags" ] = v.decoration_flags;
  root[ "numeric" ] = v.numeric;
  root[ "array" ] = v.array;
  root[ "member_count" ] = v.member_count;
  root[ "members" ] = nlohmann::json::array();
  for( unsigned int i = 0u; i != v.member_count; ++i )
    root[ "members" ].push_back( nlohmann::json( v.members[ i ] ) );
  root[ "format" ] = v.format;
  if( v.type_description )
    root[ "type_description" ] = *v.type_description;
  root[ "word_offset" ] = nlohmann::json::object();
  root[ "word_offset" ][ "location" ] = v.word_offset.location;
}

void to_json( nlohmann::json &root, const SpvReflectDescriptorSet &v ) {
  root = nlohmann::json::object();
  root[ "set" ] = v.set;
  root[ "binding_count" ] = v.binding_count;
  root[ "bindings" ] = nlohmann::json::array();
  for( unsigned int i = 0u; i != v.binding_count; ++i )
    root[ "bindings" ].push_back( nlohmann::json( *v.bindings[ i ] ) );
}

void to_json( nlohmann::json &root, const SpvReflectEntryPoint &v ) {
  root = nlohmann::json::object();
  root[ "name" ] = v.name;
  root[ "id" ] = v.id;
  root[ "spirv_execution_model" ] = v.spirv_execution_model;
  root[ "shader_stage" ] = v.shader_stage;
  root[ "input_variable_count" ] = v.input_variable_count;
  root[ "input_variables" ] = nlohmann::json::array();
  for( unsigned int i = 0u; i != v.input_variable_count; ++i )
    root[ "input_variables" ].push_back( nlohmann::json( *v.input_variables[ i ] ) );
  root[ "output_variable_count" ] = v.output_variable_count;
  root[ "output_variables" ] = nlohmann::json::array();
  for( unsigned int i = 0u; i != v.output_variable_count; ++i )
    root[ "output_variables" ].push_back( nlohmann::json( *v.output_variables[ i ] ) );
  root[ "interface_variable_count" ] = v.interface_variable_count;
  root[ "interface_variables" ] = nlohmann::json::array();
  for( unsigned int i = 0u; i != v.interface_variable_count; ++i )
    root[ "interface_variables" ].push_back( nlohmann::json( v.interface_variables[ i ] ) );
  root[ "descriptor_set_count" ] = v.descriptor_set_count;
  root[ "descriptor_sets" ] = nlohmann::json::array();
  for( unsigned int i = 0u; i != v.descriptor_set_count; ++i )
    root[ "descriptor_sets" ].push_back( nlohmann::json( v.descriptor_sets[ i ] ) );
  root[ "used_uniform_count" ] = v.used_uniform_count;
  root[ "used_uniforms" ] = nlohmann::json::array();
  for( unsigned int i = 0u; i != v.used_uniform_count; ++i )
    root[ "used_uniforms" ].push_back( nlohmann::json( v.used_uniforms[ i ] ) );
  root[ "used_push_constant_count" ] = v.used_push_constant_count;
  root[ "used_push_constants" ] = nlohmann::json::array();
  for( unsigned int i = 0u; i != v.used_push_constant_count; ++i )
    root[ "used_push_constants" ].push_back( nlohmann::json( v.used_push_constants[ i ] ) );
  root[ "local_size" ] = nlohmann::json::object();
  root[ "local_size" ][ "x" ] = v.local_size.x;
  root[ "local_size" ][ "y" ] = v.local_size.y;
  root[ "local_size" ][ "z" ] = v.local_size.z;
}

void to_json( nlohmann::json &root, const SpvSourceLanguage &v ) {
  if( v == SpvSourceLanguageUnknown )
    root = "Unknown";
  else if( v == SpvSourceLanguageESSL )
    root = "ESSL";
  else if( v == SpvSourceLanguageGLSL )
    root = "GLSL";
  else if( v == SpvSourceLanguageOpenCL_C )
    root = "OpenCL_C";
  else if( v == SpvSourceLanguageOpenCL_CPP )
    root = "OpenCL_CPP";
  else if( v == SpvSourceLanguageHLSL )
    root = "HLSL";
  else if( v == SpvSourceLanguageSYCL )
    root = "SYCL";
  else
    root = "Unknown";
}

void to_json( nlohmann::json &root, const SpvReflectDescriptorType &v ) {
  if( v == SPV_REFLECT_DESCRIPTOR_TYPE_SAMPLER )
    root = "SAMPLER";
  else if( v == SPV_REFLECT_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER )
    root = "COMBINED_IMAGE_SAMPLER";
  else if( v == SPV_REFLECT_DESCRIPTOR_TYPE_SAMPLED_IMAGE )
    root = "SAMPLED_IMAGE";
  else if( v == SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_IMAGE )
    root = "STORAGE_IMAGE";
  else if( v == SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER )
    root = "UNIFORM_TEXEL_BUFFER";
  else if( v == SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER )
    root = "STORAGE_TEXEL_BUFFER";
  else if( v == SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_BUFFER )
    root = "UNIFORM_BUFFER";
  else if( v == SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_BUFFER )
    root = "STORAGE_BUFFER";
  else if( v == SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC )
    root = "UNIFORM_BUFFER_DYNAMIC";
  else if( v == SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC )
    root = "STORAGE_BUFFER_DYNAMIC";
  else if( v == SPV_REFLECT_DESCRIPTOR_TYPE_INPUT_ATTACHMENT )
    root = "INPUT_ATTACHMENT";
  else if( v == SPV_REFLECT_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR )
    root = "ACCELERATION_STRUCTURE_KHR";
  else
    root = "Unknown";
}

void to_json( nlohmann::json &root, const SpvReflectResourceType &v ) {
  if( v == SPV_REFLECT_RESOURCE_FLAG_UNDEFINED )
    root = "UNDEFINED";
  else if( v == SPV_REFLECT_RESOURCE_FLAG_SAMPLER )
    root = "SAMPLER";
  else if( v == SPV_REFLECT_RESOURCE_FLAG_CBV )
    root = "CBV";
  else if( v == SPV_REFLECT_RESOURCE_FLAG_SRV )
    root = "SRV";
  else if( v == SPV_REFLECT_RESOURCE_FLAG_UAV )
    root = "UAV";
  else
    root = "Unknown";
}

void to_json( nlohmann::json &root, const SpvImageFormat &v ) {
  if( v == SpvImageFormatUnknown )
 root = "SpvImageFormatUnknown";
  else if( v == SpvImageFormatRgba32f )
 root = "SpvImageFormatRgba32f";
  else if( v == SpvImageFormatRgba16f )
 root = "SpvImageFormatRgba16f";
  else if( v == SpvImageFormatR32f )
 root = "SpvImageFormatR32f";
  else if( v == SpvImageFormatRgba8 )
 root = "SpvImageFormatRgba8";
  else if( v == SpvImageFormatRgba8Snorm )
 root = "SpvImageFormatRgba8Snorm";
  else if( v == SpvImageFormatRg32f )
 root = "SpvImageFormatRg32f";
  else if( v == SpvImageFormatRg16f )
 root = "SpvImageFormatRg16f";
  else if( v == SpvImageFormatR11fG11fB10f )
 root = "SpvImageFormatR11fG11fB10f";
  else if( v == SpvImageFormatR16f )
 root = "SpvImageFormatR16f";
  else if( v == SpvImageFormatRgba16 )
 root = "SpvImageFormatRgba16";
  else if( v == SpvImageFormatRgb10A2 )
 root = "SpvImageFormatRgb10A2";
  else if( v == SpvImageFormatRg16 )
 root = "SpvImageFormatRg16";
  else if( v == SpvImageFormatRg8 )
 root = "SpvImageFormatRg8";
  else if( v == SpvImageFormatR16 )
 root = "SpvImageFormatR16";
  else if( v == SpvImageFormatR8 )
 root = "SpvImageFormatR8";
  else if( v == SpvImageFormatRgba16Snorm )
 root = "SpvImageFormatRgba16Snorm";
  else if( v == SpvImageFormatRg16Snorm )
 root = "SpvImageFormatRg16Snorm";
  else if( v == SpvImageFormatRg8Snorm )
 root = "SpvImageFormatRg8Snorm";
  else if( v == SpvImageFormatR16Snorm )
 root = "SpvImageFormatR16Snorm";
  else if( v == SpvImageFormatR8Snorm )
 root = "SpvImageFormatR8Snorm";
  else if( v == SpvImageFormatRgba32i )
 root = "SpvImageFormatRgba32i";
  else if( v == SpvImageFormatRgba16i )
 root = "SpvImageFormatRgba16i";
  else if( v == SpvImageFormatRgba8i )
 root = "SpvImageFormatRgba8i";
  else if( v == SpvImageFormatR32i )
 root = "SpvImageFormatR32i";
  else if( v == SpvImageFormatRg32i )
 root = "SpvImageFormatRg32i";
  else if( v == SpvImageFormatRg16i )
 root = "SpvImageFormatRg16i";
  else if( v == SpvImageFormatRg8i )
 root = "SpvImageFormatRg8i";
  else if( v == SpvImageFormatR16i )
 root = "SpvImageFormatR16i";
  else if( v == SpvImageFormatR8i )
 root = "SpvImageFormatR8i";
  else if( v == SpvImageFormatRgba32ui )
 root = "SpvImageFormatRgba32ui";
  else if( v == SpvImageFormatRgba16ui )
 root = "SpvImageFormatRgba16ui";
  else if( v == SpvImageFormatRgba8ui )
 root = "SpvImageFormatRgba8ui";
  else if( v == SpvImageFormatR32ui )
 root = "SpvImageFormatR32ui";
  else if( v == SpvImageFormatRgb10a2ui )
 root = "SpvImageFormatRgb10a2ui";
  else if( v == SpvImageFormatRg32ui )
 root = "SpvImageFormatRg32ui";
  else if( v == SpvImageFormatRg16ui )
 root = "SpvImageFormatRg16ui";
  else if( v == SpvImageFormatRg8ui )
 root = "SpvImageFormatRg8ui";
  else if( v == SpvImageFormatR16ui )
 root = "SpvImageFormatR16ui";
  else if( v == SpvImageFormatR8ui )
 root = "SpvImageFormatR8ui";
  else if( v == SpvImageFormatR64ui )
 root = "SpvImageFormatR64ui";
  else if( v == SpvImageFormatR64i )
 root = "SpvImageFormatR64i";
  else
    root = "Unknown";
}

void to_json( nlohmann::json &root, const SpvReflectImageTraits &v ) {
  root = nlohmann::json::object();
  root[ "dim" ] = v.dim;
  root[ "depth" ] = v.depth;
  root[ "arrayed" ] = v.arrayed;
  root[ "ms" ] = v.ms;
  root[ "sampled" ] = v.sampled;
  root[ "image_format" ] = v.image_format;
}

void to_json( nlohmann::json &root, const SpvOp &v ) {
  if( v == SpvOpNop )
    root = "SpvOpNop";
  else if( v == SpvOpUndef )
    root = "SpvOpUndef";
  else if( v == SpvOpSourceContinued )
    root = "SpvOpSourceContinued";
  else if( v == SpvOpSource )
    root = "SpvOpSource";
  else if( v == SpvOpSourceExtension )
    root = "SpvOpSourceExtension";
  else if( v == SpvOpName )
    root = "SpvOpName";
  else if( v == SpvOpMemberName )
    root = "SpvOpMemberName";
  else if( v == SpvOpString )
    root = "SpvOpString";
  else if( v == SpvOpLine )
    root = "SpvOpLine";
  else if( v == SpvOpExtension )
    root = "SpvOpExtension";
  else if( v == SpvOpExtInstImport )
    root = "SpvOpExtInstImport";
  else if( v == SpvOpExtInst )
    root = "SpvOpExtInst";
  else if( v == SpvOpMemoryModel )
    root = "SpvOpMemoryModel";
  else if( v == SpvOpEntryPoint )
    root = "SpvOpEntryPoint";
  else if( v == SpvOpExecutionMode )
    root = "SpvOpExecutionMode";
  else if( v == SpvOpCapability )
    root = "SpvOpCapability";
  else if( v == SpvOpTypeVoid )
    root = "SpvOpTypeVoid";
  else if( v == SpvOpTypeBool )
    root = "SpvOpTypeBool";
  else if( v == SpvOpTypeInt )
    root = "SpvOpTypeInt";
  else if( v == SpvOpTypeFloat )
    root = "SpvOpTypeFloat";
  else if( v == SpvOpTypeVector )
    root = "SpvOpTypeVector";
  else if( v == SpvOpTypeMatrix )
    root = "SpvOpTypeMatrix";
  else if( v == SpvOpTypeImage )
    root = "SpvOpTypeImage";
  else if( v == SpvOpTypeSampler )
    root = "SpvOpTypeSampler";
  else if( v == SpvOpTypeSampledImage )
    root = "SpvOpTypeSampledImage";
  else if( v == SpvOpTypeArray )
    root = "SpvOpTypeArray";
  else if( v == SpvOpTypeRuntimeArray )
    root = "SpvOpTypeRuntimeArray";
  else if( v == SpvOpTypeStruct )
    root = "SpvOpTypeStruct";
  else if( v == SpvOpTypeOpaque )
    root = "SpvOpTypeOpaque";
  else if( v == SpvOpTypePointer )
    root = "SpvOpTypePointer";
  else if( v == SpvOpTypeFunction )
    root = "SpvOpTypeFunction";
  else if( v == SpvOpTypeEvent )
    root = "SpvOpTypeEvent";
  else if( v == SpvOpTypeDeviceEvent )
    root = "SpvOpTypeDeviceEvent";
  else if( v == SpvOpTypeReserveId )
    root = "SpvOpTypeReserveId";
  else if( v == SpvOpTypeQueue )
    root = "SpvOpTypeQueue";
  else if( v == SpvOpTypePipe )
    root = "SpvOpTypePipe";
  else if( v == SpvOpTypeForwardPointer )
    root = "SpvOpTypeForwardPointer";
  else if( v == SpvOpConstantTrue )
    root = "SpvOpConstantTrue";
  else if( v == SpvOpConstantFalse )
    root = "SpvOpConstantFalse";
  else if( v == SpvOpConstant )
    root = "SpvOpConstant";
  else if( v == SpvOpConstantComposite )
    root = "SpvOpConstantComposite";
  else if( v == SpvOpConstantSampler )
    root = "SpvOpConstantSampler";
  else if( v == SpvOpConstantNull )
    root = "SpvOpConstantNull";
  else if( v == SpvOpSpecConstantTrue )
    root = "SpvOpSpecConstantTrue";
  else if( v == SpvOpSpecConstantFalse )
    root = "SpvOpSpecConstantFalse";
  else if( v == SpvOpSpecConstant )
    root = "SpvOpSpecConstant";
  else if( v == SpvOpSpecConstantComposite )
    root = "SpvOpSpecConstantComposite";
  else if( v == SpvOpSpecConstantOp )
    root = "SpvOpSpecConstantOp";
  else if( v == SpvOpFunction )
    root = "SpvOpFunction";
  else if( v == SpvOpFunctionParameter )
    root = "SpvOpFunctionParameter";
  else if( v == SpvOpFunctionEnd )
    root = "SpvOpFunctionEnd";
  else if( v == SpvOpFunctionCall )
    root = "SpvOpFunctionCall";
  else if( v == SpvOpVariable )
    root = "SpvOpVariable";
  else if( v == SpvOpImageTexelPointer )
    root = "SpvOpImageTexelPointer";
  else if( v == SpvOpLoad )
    root = "SpvOpLoad";
  else if( v == SpvOpStore )
    root = "SpvOpStore";
  else if( v == SpvOpCopyMemory )
    root = "SpvOpCopyMemory";
  else if( v == SpvOpCopyMemorySized )
    root = "SpvOpCopyMemorySized";
  else if( v == SpvOpAccessChain )
    root = "SpvOpAccessChain";
  else if( v == SpvOpInBoundsAccessChain )
    root = "SpvOpInBoundsAccessChain";
  else if( v == SpvOpPtrAccessChain )
    root = "SpvOpPtrAccessChain";
  else if( v == SpvOpArrayLength )
    root = "SpvOpArrayLength";
  else if( v == SpvOpGenericPtrMemSemantics )
    root = "SpvOpGenericPtrMemSemantics";
  else if( v == SpvOpInBoundsPtrAccessChain )
    root = "SpvOpInBoundsPtrAccessChain";
  else if( v == SpvOpDecorate )
    root = "SpvOpDecorate";
  else if( v == SpvOpMemberDecorate )
    root = "SpvOpMemberDecorate";
  else if( v == SpvOpDecorationGroup )
    root = "SpvOpDecorationGroup";
  else if( v == SpvOpGroupDecorate )
    root = "SpvOpGroupDecorate";
  else if( v == SpvOpGroupMemberDecorate )
    root = "SpvOpGroupMemberDecorate";
  else if( v == SpvOpVectorExtractDynamic )
    root = "SpvOpVectorExtractDynamic";
  else if( v == SpvOpVectorInsertDynamic )
    root = "SpvOpVectorInsertDynamic";
  else if( v == SpvOpVectorShuffle )
    root = "SpvOpVectorShuffle";
  else if( v == SpvOpCompositeConstruct )
    root = "SpvOpCompositeConstruct";
  else if( v == SpvOpCompositeExtract )
    root = "SpvOpCompositeExtract";
  else if( v == SpvOpCompositeInsert )
    root = "SpvOpCompositeInsert";
  else if( v == SpvOpCopyObject )
    root = "SpvOpCopyObject";
  else if( v == SpvOpTranspose )
    root = "SpvOpTranspose";
  else if( v == SpvOpSampledImage )
    root = "SpvOpSampledImage";
  else if( v == SpvOpImageSampleImplicitLod )
    root = "SpvOpImageSampleImplicitLod";
  else if( v == SpvOpImageSampleExplicitLod )
    root = "SpvOpImageSampleExplicitLod";
  else if( v == SpvOpImageSampleDrefImplicitLod )
    root = "SpvOpImageSampleDrefImplicitLod";
  else if( v == SpvOpImageSampleDrefExplicitLod )
    root = "SpvOpImageSampleDrefExplicitLod";
  else if( v == SpvOpImageSampleProjImplicitLod )
    root = "SpvOpImageSampleProjImplicitLod";
  else if( v == SpvOpImageSampleProjExplicitLod )
    root = "SpvOpImageSampleProjExplicitLod";
  else if( v == SpvOpImageSampleProjDrefImplicitLod )
    root = "SpvOpImageSampleProjDrefImplicitLod";
  else if( v == SpvOpImageSampleProjDrefExplicitLod )
    root = "SpvOpImageSampleProjDrefExplicitLod";
  else if( v == SpvOpImageFetch )
    root = "SpvOpImageFetch";
  else if( v == SpvOpImageGather )
    root = "SpvOpImageGather";
  else if( v == SpvOpImageDrefGather )
    root = "SpvOpImageDrefGather";
  else if( v == SpvOpImageRead )
    root = "SpvOpImageRead";
  else if( v == SpvOpImageWrite )
    root = "SpvOpImageWrite";
  else if( v == SpvOpImage )
    root = "SpvOpImage";
  else if( v == SpvOpImageQueryFormat )
    root = "SpvOpImageQueryFormat";
  else if( v == SpvOpImageQueryOrder )
    root = "SpvOpImageQueryOrder";
  else if( v == SpvOpImageQuerySizeLod )
    root = "SpvOpImageQuerySizeLod";
  else if( v == SpvOpImageQuerySize )
    root = "SpvOpImageQuerySize";
  else if( v == SpvOpImageQueryLod )
    root = "SpvOpImageQueryLod";
  else if( v == SpvOpImageQueryLevels )
    root = "SpvOpImageQueryLevels";
  else if( v == SpvOpImageQuerySamples )
    root = "SpvOpImageQuerySamples";
  else if( v == SpvOpConvertFToU )
    root = "SpvOpConvertFToU";
  else if( v == SpvOpConvertFToS )
    root = "SpvOpConvertFToS";
  else if( v == SpvOpConvertSToF )
    root = "SpvOpConvertSToF";
  else if( v == SpvOpConvertUToF )
    root = "SpvOpConvertUToF";
  else if( v == SpvOpUConvert )
    root = "SpvOpUConvert";
  else if( v == SpvOpSConvert )
    root = "SpvOpSConvert";
  else if( v == SpvOpFConvert )
    root = "SpvOpFConvert";
  else if( v == SpvOpQuantizeToF16 )
    root = "SpvOpQuantizeToF16";
  else if( v == SpvOpConvertPtrToU )
    root = "SpvOpConvertPtrToU";
  else if( v == SpvOpSatConvertSToU )
    root = "SpvOpSatConvertSToU";
  else if( v == SpvOpSatConvertUToS )
    root = "SpvOpSatConvertUToS";
  else if( v == SpvOpConvertUToPtr )
    root = "SpvOpConvertUToPtr";
  else if( v == SpvOpPtrCastToGeneric )
    root = "SpvOpPtrCastToGeneric";
  else if( v == SpvOpGenericCastToPtr )
    root = "SpvOpGenericCastToPtr";
  else if( v == SpvOpGenericCastToPtrExplicit )
    root = "SpvOpGenericCastToPtrExplicit";
  else if( v == SpvOpBitcast )
    root = "SpvOpBitcast";
  else if( v == SpvOpSNegate )
    root = "SpvOpSNegate";
  else if( v == SpvOpFNegate )
    root = "SpvOpFNegate";
  else if( v == SpvOpIAdd )
    root = "SpvOpIAdd";
  else if( v == SpvOpFAdd )
    root = "SpvOpFAdd";
  else if( v == SpvOpISub )
    root = "SpvOpISub";
  else if( v == SpvOpFSub )
    root = "SpvOpFSub";
  else if( v == SpvOpIMul )
    root = "SpvOpIMul";
  else if( v == SpvOpFMul )
    root = "SpvOpFMul";
  else if( v == SpvOpUDiv )
    root = "SpvOpUDiv";
  else if( v == SpvOpSDiv )
    root = "SpvOpSDiv";
  else if( v == SpvOpFDiv )
    root = "SpvOpFDiv";
  else if( v == SpvOpUMod )
    root = "SpvOpUMod";
  else if( v == SpvOpSRem )
    root = "SpvOpSRem";
  else if( v == SpvOpSMod )
    root = "SpvOpSMod";
  else if( v == SpvOpFRem )
    root = "SpvOpFRem";
  else if( v == SpvOpFMod )
    root = "SpvOpFMod";
  else if( v == SpvOpVectorTimesScalar )
    root = "SpvOpVectorTimesScalar";
  else if( v == SpvOpMatrixTimesScalar )
    root = "SpvOpMatrixTimesScalar";
  else if( v == SpvOpVectorTimesMatrix )
    root = "SpvOpVectorTimesMatrix";
  else if( v == SpvOpMatrixTimesVector )
    root = "SpvOpMatrixTimesVector";
  else if( v == SpvOpMatrixTimesMatrix )
    root = "SpvOpMatrixTimesMatrix";
  else if( v == SpvOpOuterProduct )
    root = "SpvOpOuterProduct";
  else if( v == SpvOpDot )
    root = "SpvOpDot";
  else if( v == SpvOpIAddCarry )
    root = "SpvOpIAddCarry";
  else if( v == SpvOpISubBorrow )
    root = "SpvOpISubBorrow";
  else if( v == SpvOpUMulExtended )
    root = "SpvOpUMulExtended";
  else if( v == SpvOpSMulExtended )
    root = "SpvOpSMulExtended";
  else if( v == SpvOpAny )
    root = "SpvOpAny";
  else if( v == SpvOpAll )
    root = "SpvOpAll";
  else if( v == SpvOpIsNan )
    root = "SpvOpIsNan";
  else if( v == SpvOpIsInf )
    root = "SpvOpIsInf";
  else if( v == SpvOpIsFinite )
    root = "SpvOpIsFinite";
  else if( v == SpvOpIsNormal )
    root = "SpvOpIsNormal";
  else if( v == SpvOpSignBitSet )
    root = "SpvOpSignBitSet";
  else if( v == SpvOpLessOrGreater )
    root = "SpvOpLessOrGreater";
  else if( v == SpvOpOrdered )
    root = "SpvOpOrdered";
  else if( v == SpvOpUnordered )
    root = "SpvOpUnordered";
  else if( v == SpvOpLogicalEqual )
    root = "SpvOpLogicalEqual";
  else if( v == SpvOpLogicalNotEqual )
    root = "SpvOpLogicalNotEqual";
  else if( v == SpvOpLogicalOr )
    root = "SpvOpLogicalOr";
  else if( v == SpvOpLogicalAnd )
    root = "SpvOpLogicalAnd";
  else if( v == SpvOpLogicalNot )
    root = "SpvOpLogicalNot";
  else if( v == SpvOpSelect )
    root = "SpvOpSelect";
  else if( v == SpvOpIEqual )
    root = "SpvOpIEqual";
  else if( v == SpvOpINotEqual )
    root = "SpvOpINotEqual";
  else if( v == SpvOpUGreaterThan )
    root = "SpvOpUGreaterThan";
  else if( v == SpvOpSGreaterThan )
    root = "SpvOpSGreaterThan";
  else if( v == SpvOpUGreaterThanEqual )
    root = "SpvOpUGreaterThanEqual";
  else if( v == SpvOpSGreaterThanEqual )
    root = "SpvOpSGreaterThanEqual";
  else if( v == SpvOpULessThan )
    root = "SpvOpULessThan";
  else if( v == SpvOpSLessThan )
    root = "SpvOpSLessThan";
  else if( v == SpvOpULessThanEqual )
    root = "SpvOpULessThanEqual";
  else if( v == SpvOpSLessThanEqual )
    root = "SpvOpSLessThanEqual";
  else if( v == SpvOpFOrdEqual )
    root = "SpvOpFOrdEqual";
  else if( v == SpvOpFUnordEqual )
    root = "SpvOpFUnordEqual";
  else if( v == SpvOpFOrdNotEqual )
    root = "SpvOpFOrdNotEqual";
  else if( v == SpvOpFUnordNotEqual )
    root = "SpvOpFUnordNotEqual";
  else if( v == SpvOpFOrdLessThan )
    root = "SpvOpFOrdLessThan";
  else if( v == SpvOpFUnordLessThan )
    root = "SpvOpFUnordLessThan";
  else if( v == SpvOpFOrdGreaterThan )
    root = "SpvOpFOrdGreaterThan";
  else if( v == SpvOpFUnordGreaterThan )
    root = "SpvOpFUnordGreaterThan";
  else if( v == SpvOpFOrdLessThanEqual )
    root = "SpvOpFOrdLessThanEqual";
  else if( v == SpvOpFUnordLessThanEqual )
    root = "SpvOpFUnordLessThanEqual";
  else if( v == SpvOpFOrdGreaterThanEqual )
    root = "SpvOpFOrdGreaterThanEqual";
  else if( v == SpvOpFUnordGreaterThanEqual )
    root = "SpvOpFUnordGreaterThanEqual";
  else if( v == SpvOpShiftRightLogical )
    root = "SpvOpShiftRightLogical";
  else if( v == SpvOpShiftRightArithmetic )
    root = "SpvOpShiftRightArithmetic";
  else if( v == SpvOpShiftLeftLogical )
    root = "SpvOpShiftLeftLogical";
  else if( v == SpvOpBitwiseOr )
    root = "SpvOpBitwiseOr";
  else if( v == SpvOpBitwiseXor )
    root = "SpvOpBitwiseXor";
  else if( v == SpvOpBitwiseAnd )
    root = "SpvOpBitwiseAnd";
  else if( v == SpvOpNot )
    root = "SpvOpNot";
  else if( v == SpvOpBitFieldInsert )
    root = "SpvOpBitFieldInsert";
  else if( v == SpvOpBitFieldSExtract )
    root = "SpvOpBitFieldSExtract";
  else if( v == SpvOpBitFieldUExtract )
    root = "SpvOpBitFieldUExtract";
  else if( v == SpvOpBitReverse )
    root = "SpvOpBitReverse";
  else if( v == SpvOpBitCount )
    root = "SpvOpBitCount";
  else if( v == SpvOpDPdx )
    root = "SpvOpDPdx";
  else if( v == SpvOpDPdy )
    root = "SpvOpDPdy";
  else if( v == SpvOpFwidth )
    root = "SpvOpFwidth";
  else if( v == SpvOpDPdxFine )
    root = "SpvOpDPdxFine";
  else if( v == SpvOpDPdyFine )
    root = "SpvOpDPdyFine";
  else if( v == SpvOpFwidthFine )
    root = "SpvOpFwidthFine";
  else if( v == SpvOpDPdxCoarse )
    root = "SpvOpDPdxCoarse";
  else if( v == SpvOpDPdyCoarse )
    root = "SpvOpDPdyCoarse";
  else if( v == SpvOpFwidthCoarse )
    root = "SpvOpFwidthCoarse";
  else if( v == SpvOpEmitVertex )
    root = "SpvOpEmitVertex";
  else if( v == SpvOpEndPrimitive )
    root = "SpvOpEndPrimitive";
  else if( v == SpvOpEmitStreamVertex )
    root = "SpvOpEmitStreamVertex";
  else if( v == SpvOpEndStreamPrimitive )
    root = "SpvOpEndStreamPrimitive";
  else if( v == SpvOpControlBarrier )
    root = "SpvOpControlBarrier";
  else if( v == SpvOpMemoryBarrier )
    root = "SpvOpMemoryBarrier";
  else if( v == SpvOpAtomicLoad )
    root = "SpvOpAtomicLoad";
  else if( v == SpvOpAtomicStore )
    root = "SpvOpAtomicStore";
  else if( v == SpvOpAtomicExchange )
    root = "SpvOpAtomicExchange";
  else if( v == SpvOpAtomicCompareExchange )
    root = "SpvOpAtomicCompareExchange";
  else if( v == SpvOpAtomicCompareExchangeWeak )
    root = "SpvOpAtomicCompareExchangeWeak";
  else if( v == SpvOpAtomicIIncrement )
    root = "SpvOpAtomicIIncrement";
  else if( v == SpvOpAtomicIDecrement )
    root = "SpvOpAtomicIDecrement";
  else if( v == SpvOpAtomicIAdd )
    root = "SpvOpAtomicIAdd";
  else if( v == SpvOpAtomicISub )
    root = "SpvOpAtomicISub";
  else if( v == SpvOpAtomicSMin )
    root = "SpvOpAtomicSMin";
  else if( v == SpvOpAtomicUMin )
    root = "SpvOpAtomicUMin";
  else if( v == SpvOpAtomicSMax )
    root = "SpvOpAtomicSMax";
  else if( v == SpvOpAtomicUMax )
    root = "SpvOpAtomicUMax";
  else if( v == SpvOpAtomicAnd )
    root = "SpvOpAtomicAnd";
  else if( v == SpvOpAtomicOr )
    root = "SpvOpAtomicOr";
  else if( v == SpvOpAtomicXor )
    root = "SpvOpAtomicXor";
  else if( v == SpvOpPhi )
    root = "SpvOpPhi";
  else if( v == SpvOpLoopMerge )
    root = "SpvOpLoopMerge";
  else if( v == SpvOpSelectionMerge )
    root = "SpvOpSelectionMerge";
  else if( v == SpvOpLabel )
    root = "SpvOpLabel";
  else if( v == SpvOpBranch )
    root = "SpvOpBranch";
  else if( v == SpvOpBranchConditional )
    root = "SpvOpBranchConditional";
  else if( v == SpvOpSwitch )
    root = "SpvOpSwitch";
  else if( v == SpvOpKill )
    root = "SpvOpKill";
  else if( v == SpvOpReturn )
    root = "SpvOpReturn";
  else if( v == SpvOpReturnValue )
    root = "SpvOpReturnValue";
  else if( v == SpvOpUnreachable )
    root = "SpvOpUnreachable";
  else if( v == SpvOpLifetimeStart )
    root = "SpvOpLifetimeStart";
  else if( v == SpvOpLifetimeStop )
    root = "SpvOpLifetimeStop";
  else if( v == SpvOpGroupAsyncCopy )
    root = "SpvOpGroupAsyncCopy";
  else if( v == SpvOpGroupWaitEvents )
    root = "SpvOpGroupWaitEvents";
  else if( v == SpvOpGroupAll )
    root = "SpvOpGroupAll";
  else if( v == SpvOpGroupAny )
    root = "SpvOpGroupAny";
  else if( v == SpvOpGroupBroadcast )
    root = "SpvOpGroupBroadcast";
  else if( v == SpvOpGroupIAdd )
    root = "SpvOpGroupIAdd";
  else if( v == SpvOpGroupFAdd )
    root = "SpvOpGroupFAdd";
  else if( v == SpvOpGroupFMin )
    root = "SpvOpGroupFMin";
  else if( v == SpvOpGroupUMin )
    root = "SpvOpGroupUMin";
  else if( v == SpvOpGroupSMin )
    root = "SpvOpGroupSMin";
  else if( v == SpvOpGroupFMax )
    root = "SpvOpGroupFMax";
  else if( v == SpvOpGroupUMax )
    root = "SpvOpGroupUMax";
  else if( v == SpvOpGroupSMax )
    root = "SpvOpGroupSMax";
  else if( v == SpvOpReadPipe )
    root = "SpvOpReadPipe";
  else if( v == SpvOpWritePipe )
    root = "SpvOpWritePipe";
  else if( v == SpvOpReservedReadPipe )
    root = "SpvOpReservedReadPipe";
  else if( v == SpvOpReservedWritePipe )
    root = "SpvOpReservedWritePipe";
  else if( v == SpvOpReserveReadPipePackets )
    root = "SpvOpReserveReadPipePackets";
  else if( v == SpvOpReserveWritePipePackets )
    root = "SpvOpReserveWritePipePackets";
  else if( v == SpvOpCommitReadPipe )
    root = "SpvOpCommitReadPipe";
  else if( v == SpvOpCommitWritePipe )
    root = "SpvOpCommitWritePipe";
  else if( v == SpvOpIsValidReserveId )
    root = "SpvOpIsValidReserveId";
  else if( v == SpvOpGetNumPipePackets )
    root = "SpvOpGetNumPipePackets";
  else if( v == SpvOpGetMaxPipePackets )
    root = "SpvOpGetMaxPipePackets";
  else if( v == SpvOpGroupReserveReadPipePackets )
    root = "SpvOpGroupReserveReadPipePackets";
  else if( v == SpvOpGroupReserveWritePipePackets )
    root = "SpvOpGroupReserveWritePipePackets";
  else if( v == SpvOpGroupCommitReadPipe )
    root = "SpvOpGroupCommitReadPipe";
  else if( v == SpvOpGroupCommitWritePipe )
    root = "SpvOpGroupCommitWritePipe";
  else if( v == SpvOpEnqueueMarker )
    root = "SpvOpEnqueueMarker";
  else if( v == SpvOpEnqueueKernel )
    root = "SpvOpEnqueueKernel";
  else if( v == SpvOpGetKernelNDrangeSubGroupCount )
    root = "SpvOpGetKernelNDrangeSubGroupCount";
  else if( v == SpvOpGetKernelNDrangeMaxSubGroupSize )
    root = "SpvOpGetKernelNDrangeMaxSubGroupSize";
  else if( v == SpvOpGetKernelWorkGroupSize )
    root = "SpvOpGetKernelWorkGroupSize";
  else if( v == SpvOpGetKernelPreferredWorkGroupSizeMultiple )
    root = "SpvOpGetKernelPreferredWorkGroupSizeMultiple";
  else if( v == SpvOpRetainEvent )
    root = "SpvOpRetainEvent";
  else if( v == SpvOpReleaseEvent )
    root = "SpvOpReleaseEvent";
  else if( v == SpvOpCreateUserEvent )
    root = "SpvOpCreateUserEvent";
  else if( v == SpvOpIsValidEvent )
    root = "SpvOpIsValidEvent";
  else if( v == SpvOpSetUserEventStatus )
    root = "SpvOpSetUserEventStatus";
  else if( v == SpvOpCaptureEventProfilingInfo )
    root = "SpvOpCaptureEventProfilingInfo";
  else if( v == SpvOpGetDefaultQueue )
    root = "SpvOpGetDefaultQueue";
  else if( v == SpvOpBuildNDRange )
    root = "SpvOpBuildNDRange";
  else if( v == SpvOpImageSparseSampleImplicitLod )
    root = "SpvOpImageSparseSampleImplicitLod";
  else if( v == SpvOpImageSparseSampleExplicitLod )
    root = "SpvOpImageSparseSampleExplicitLod";
  else if( v == SpvOpImageSparseSampleDrefImplicitLod )
    root = "SpvOpImageSparseSampleDrefImplicitLod";
  else if( v == SpvOpImageSparseSampleDrefExplicitLod )
    root = "SpvOpImageSparseSampleDrefExplicitLod";
  else if( v == SpvOpImageSparseSampleProjImplicitLod )
    root = "SpvOpImageSparseSampleProjImplicitLod";
  else if( v == SpvOpImageSparseSampleProjExplicitLod )
    root = "SpvOpImageSparseSampleProjExplicitLod";
  else if( v == SpvOpImageSparseSampleProjDrefImplicitLod )
    root = "SpvOpImageSparseSampleProjDrefImplicitLod";
  else if( v == SpvOpImageSparseSampleProjDrefExplicitLod )
    root = "SpvOpImageSparseSampleProjDrefExplicitLod";
  else if( v == SpvOpImageSparseFetch )
    root = "SpvOpImageSparseFetch";
  else if( v == SpvOpImageSparseGather )
    root = "SpvOpImageSparseGather";
  else if( v == SpvOpImageSparseDrefGather )
    root = "SpvOpImageSparseDrefGather";
  else if( v == SpvOpImageSparseTexelsResident )
    root = "SpvOpImageSparseTexelsResident";
  else if( v == SpvOpNoLine )
    root = "SpvOpNoLine";
  else if( v == SpvOpAtomicFlagTestAndSet )
    root = "SpvOpAtomicFlagTestAndSet";
  else if( v == SpvOpAtomicFlagClear )
    root = "SpvOpAtomicFlagClear";
  else if( v == SpvOpImageSparseRead )
    root = "SpvOpImageSparseRead";
  else if( v == SpvOpSizeOf )
    root = "SpvOpSizeOf";
  else if( v == SpvOpTypePipeStorage )
    root = "SpvOpTypePipeStorage";
  else if( v == SpvOpConstantPipeStorage )
    root = "SpvOpConstantPipeStorage";
  else if( v == SpvOpCreatePipeFromPipeStorage )
    root = "SpvOpCreatePipeFromPipeStorage";
  else if( v == SpvOpGetKernelLocalSizeForSubgroupCount )
    root = "SpvOpGetKernelLocalSizeForSubgroupCount";
  else if( v == SpvOpGetKernelMaxNumSubgroups )
    root = "SpvOpGetKernelMaxNumSubgroups";
  else if( v == SpvOpTypeNamedBarrier )
    root = "SpvOpTypeNamedBarrier";
  else if( v == SpvOpNamedBarrierInitialize )
    root = "SpvOpNamedBarrierInitialize";
  else if( v == SpvOpMemoryNamedBarrier )
    root = "SpvOpMemoryNamedBarrier";
  else if( v == SpvOpModuleProcessed )
    root = "SpvOpModuleProcessed";
  else if( v == SpvOpExecutionModeId )
    root = "SpvOpExecutionModeId";
  else if( v == SpvOpDecorateId )
    root = "SpvOpDecorateId";
  else if( v == SpvOpGroupNonUniformElect )
    root = "SpvOpGroupNonUniformElect";
  else if( v == SpvOpGroupNonUniformAll )
    root = "SpvOpGroupNonUniformAll";
  else if( v == SpvOpGroupNonUniformAny )
    root = "SpvOpGroupNonUniformAny";
  else if( v == SpvOpGroupNonUniformAllEqual )
    root = "SpvOpGroupNonUniformAllEqual";
  else if( v == SpvOpGroupNonUniformBroadcast )
    root = "SpvOpGroupNonUniformBroadcast";
  else if( v == SpvOpGroupNonUniformBroadcastFirst )
    root = "SpvOpGroupNonUniformBroadcastFirst";
  else if( v == SpvOpGroupNonUniformBallot )
    root = "SpvOpGroupNonUniformBallot";
  else if( v == SpvOpGroupNonUniformInverseBallot )
    root = "SpvOpGroupNonUniformInverseBallot";
  else if( v == SpvOpGroupNonUniformBallotBitExtract )
    root = "SpvOpGroupNonUniformBallotBitExtract";
  else if( v == SpvOpGroupNonUniformBallotBitCount )
    root = "SpvOpGroupNonUniformBallotBitCount";
  else if( v == SpvOpGroupNonUniformBallotFindLSB )
    root = "SpvOpGroupNonUniformBallotFindLSB";
  else if( v == SpvOpGroupNonUniformBallotFindMSB )
    root = "SpvOpGroupNonUniformBallotFindMSB";
  else if( v == SpvOpGroupNonUniformShuffle )
    root = "SpvOpGroupNonUniformShuffle";
  else if( v == SpvOpGroupNonUniformShuffleXor )
    root = "SpvOpGroupNonUniformShuffleXor";
  else if( v == SpvOpGroupNonUniformShuffleUp )
    root = "SpvOpGroupNonUniformShuffleUp";
  else if( v == SpvOpGroupNonUniformShuffleDown )
    root = "SpvOpGroupNonUniformShuffleDown";
  else if( v == SpvOpGroupNonUniformIAdd )
    root = "SpvOpGroupNonUniformIAdd";
  else if( v == SpvOpGroupNonUniformFAdd )
    root = "SpvOpGroupNonUniformFAdd";
  else if( v == SpvOpGroupNonUniformIMul )
    root = "SpvOpGroupNonUniformIMul";
  else if( v == SpvOpGroupNonUniformFMul )
    root = "SpvOpGroupNonUniformFMul";
  else if( v == SpvOpGroupNonUniformSMin )
    root = "SpvOpGroupNonUniformSMin";
  else if( v == SpvOpGroupNonUniformUMin )
    root = "SpvOpGroupNonUniformUMin";
  else if( v == SpvOpGroupNonUniformFMin )
    root = "SpvOpGroupNonUniformFMin";
  else if( v == SpvOpGroupNonUniformSMax )
    root = "SpvOpGroupNonUniformSMax";
  else if( v == SpvOpGroupNonUniformUMax )
    root = "SpvOpGroupNonUniformUMax";
  else if( v == SpvOpGroupNonUniformFMax )
    root = "SpvOpGroupNonUniformFMax";
  else if( v == SpvOpGroupNonUniformBitwiseAnd )
    root = "SpvOpGroupNonUniformBitwiseAnd";
  else if( v == SpvOpGroupNonUniformBitwiseOr )
    root = "SpvOpGroupNonUniformBitwiseOr";
  else if( v == SpvOpGroupNonUniformBitwiseXor )
    root = "SpvOpGroupNonUniformBitwiseXor";
  else if( v == SpvOpGroupNonUniformLogicalAnd )
    root = "SpvOpGroupNonUniformLogicalAnd";
  else if( v == SpvOpGroupNonUniformLogicalOr )
    root = "SpvOpGroupNonUniformLogicalOr";
  else if( v == SpvOpGroupNonUniformLogicalXor )
    root = "SpvOpGroupNonUniformLogicalXor";
  else if( v == SpvOpGroupNonUniformQuadBroadcast )
    root = "SpvOpGroupNonUniformQuadBroadcast";
  else if( v == SpvOpGroupNonUniformQuadSwap )
    root = "SpvOpGroupNonUniformQuadSwap";
  else if( v == SpvOpCopyLogical )
    root = "SpvOpCopyLogical";
  else if( v == SpvOpPtrEqual )
    root = "SpvOpPtrEqual";
  else if( v == SpvOpPtrNotEqual )
    root = "SpvOpPtrNotEqual";
  else if( v == SpvOpPtrDiff )
    root = "SpvOpPtrDiff";
  else if( v == SpvOpTerminateInvocation )
    root = "SpvOpTerminateInvocation";
  else if( v == SpvOpSubgroupBallotKHR )
    root = "SpvOpSubgroupBallotKHR";
  else if( v == SpvOpSubgroupFirstInvocationKHR )
    root = "SpvOpSubgroupFirstInvocationKHR";
  else if( v == SpvOpSubgroupAllKHR )
    root = "SpvOpSubgroupAllKHR";
  else if( v == SpvOpSubgroupAnyKHR )
    root = "SpvOpSubgroupAnyKHR";
  else if( v == SpvOpSubgroupAllEqualKHR )
    root = "SpvOpSubgroupAllEqualKHR";
  else if( v == SpvOpGroupNonUniformRotateKHR )
    root = "SpvOpGroupNonUniformRotateKHR";
  else if( v == SpvOpSubgroupReadInvocationKHR )
    root = "SpvOpSubgroupReadInvocationKHR";
  else if( v == SpvOpTraceRayKHR )
    root = "SpvOpTraceRayKHR";
  else if( v == SpvOpExecuteCallableKHR )
    root = "SpvOpExecuteCallableKHR";
  else if( v == SpvOpConvertUToAccelerationStructureKHR )
    root = "SpvOpConvertUToAccelerationStructureKHR";
  else if( v == SpvOpIgnoreIntersectionKHR )
    root = "SpvOpIgnoreIntersectionKHR";
  else if( v == SpvOpTerminateRayKHR )
    root = "SpvOpTerminateRayKHR";
  else if( v == SpvOpSDot )
    root = "SpvOpSDot";
  else if( v == SpvOpSDotKHR )
    root = "SpvOpSDotKHR";
  else if( v == SpvOpUDot )
    root = "SpvOpUDot";
  else if( v == SpvOpUDotKHR )
    root = "SpvOpUDotKHR";
  else if( v == SpvOpSUDot )
    root = "SpvOpSUDot";
  else if( v == SpvOpSUDotKHR )
    root = "SpvOpSUDotKHR";
  else if( v == SpvOpSDotAccSat )
    root = "SpvOpSDotAccSat";
  else if( v == SpvOpSDotAccSatKHR )
    root = "SpvOpSDotAccSatKHR";
  else if( v == SpvOpUDotAccSat )
    root = "SpvOpUDotAccSat";
  else if( v == SpvOpUDotAccSatKHR )
    root = "SpvOpUDotAccSatKHR";
  else if( v == SpvOpSUDotAccSat )
    root = "SpvOpSUDotAccSat";
  else if( v == SpvOpSUDotAccSatKHR )
    root = "SpvOpSUDotAccSatKHR";
  else if( v == SpvOpTypeRayQueryKHR )
    root = "SpvOpTypeRayQueryKHR";
  else if( v == SpvOpRayQueryInitializeKHR )
    root = "SpvOpRayQueryInitializeKHR";
  else if( v == SpvOpRayQueryTerminateKHR )
    root = "SpvOpRayQueryTerminateKHR";
  else if( v == SpvOpRayQueryGenerateIntersectionKHR )
    root = "SpvOpRayQueryGenerateIntersectionKHR";
  else if( v == SpvOpRayQueryConfirmIntersectionKHR )
    root = "SpvOpRayQueryConfirmIntersectionKHR";
  else if( v == SpvOpRayQueryProceedKHR )
    root = "SpvOpRayQueryProceedKHR";
  else if( v == SpvOpRayQueryGetIntersectionTypeKHR )
    root = "SpvOpRayQueryGetIntersectionTypeKHR";
  else if( v == SpvOpGroupIAddNonUniformAMD )
    root = "SpvOpGroupIAddNonUniformAMD";
  else if( v == SpvOpGroupFAddNonUniformAMD )
    root = "SpvOpGroupFAddNonUniformAMD";
  else if( v == SpvOpGroupFMinNonUniformAMD )
    root = "SpvOpGroupFMinNonUniformAMD";
  else if( v == SpvOpGroupUMinNonUniformAMD )
    root = "SpvOpGroupUMinNonUniformAMD";
  else if( v == SpvOpGroupSMinNonUniformAMD )
    root = "SpvOpGroupSMinNonUniformAMD";
  else if( v == SpvOpGroupFMaxNonUniformAMD )
    root = "SpvOpGroupFMaxNonUniformAMD";
  else if( v == SpvOpGroupUMaxNonUniformAMD )
    root = "SpvOpGroupUMaxNonUniformAMD";
  else if( v == SpvOpGroupSMaxNonUniformAMD )
    root = "SpvOpGroupSMaxNonUniformAMD";
  else if( v == SpvOpFragmentMaskFetchAMD )
    root = "SpvOpFragmentMaskFetchAMD";
  else if( v == SpvOpFragmentFetchAMD )
    root = "SpvOpFragmentFetchAMD";
  else if( v == SpvOpReadClockKHR )
    root = "SpvOpReadClockKHR";
  else if( v == SpvOpImageSampleFootprintNV )
    root = "SpvOpImageSampleFootprintNV";
  else if( v == SpvOpGroupNonUniformPartitionNV )
    root = "SpvOpGroupNonUniformPartitionNV";
  else if( v == SpvOpWritePackedPrimitiveIndices4x8NV )
    root = "SpvOpWritePackedPrimitiveIndices4x8NV";
  else if( v == SpvOpReportIntersectionKHR )
    root = "SpvOpReportIntersectionKHR";
  else if( v == SpvOpReportIntersectionNV )
    root = "SpvOpReportIntersectionNV";
  else if( v == SpvOpIgnoreIntersectionNV )
    root = "SpvOpIgnoreIntersectionNV";
  else if( v == SpvOpTerminateRayNV )
    root = "SpvOpTerminateRayNV";
  else if( v == SpvOpTraceNV )
    root = "SpvOpTraceNV";
  else if( v == SpvOpTraceMotionNV )
    root = "SpvOpTraceMotionNV";
  else if( v == SpvOpTraceRayMotionNV )
    root = "SpvOpTraceRayMotionNV";
  else if( v == SpvOpTypeAccelerationStructureKHR )
    root = "SpvOpTypeAccelerationStructureKHR";
  else if( v == SpvOpTypeAccelerationStructureNV )
    root = "SpvOpTypeAccelerationStructureNV";
  else if( v == SpvOpExecuteCallableNV )
    root = "SpvOpExecuteCallableNV";
  else if( v == SpvOpTypeCooperativeMatrixNV )
    root = "SpvOpTypeCooperativeMatrixNV";
  else if( v == SpvOpCooperativeMatrixLoadNV )
    root = "SpvOpCooperativeMatrixLoadNV";
  else if( v == SpvOpCooperativeMatrixStoreNV )
    root = "SpvOpCooperativeMatrixStoreNV";
  else if( v == SpvOpCooperativeMatrixMulAddNV )
    root = "SpvOpCooperativeMatrixMulAddNV";
  else if( v == SpvOpCooperativeMatrixLengthNV )
    root = "SpvOpCooperativeMatrixLengthNV";
  else if( v == SpvOpBeginInvocationInterlockEXT )
    root = "SpvOpBeginInvocationInterlockEXT";
  else if( v == SpvOpEndInvocationInterlockEXT )
    root = "SpvOpEndInvocationInterlockEXT";
  else if( v == SpvOpDemoteToHelperInvocation )
    root = "SpvOpDemoteToHelperInvocation";
  else if( v == SpvOpDemoteToHelperInvocationEXT )
    root = "SpvOpDemoteToHelperInvocationEXT";
  else if( v == SpvOpIsHelperInvocationEXT )
    root = "SpvOpIsHelperInvocationEXT";
  else if( v == SpvOpConvertUToImageNV )
    root = "SpvOpConvertUToImageNV";
  else if( v == SpvOpConvertUToSamplerNV )
    root = "SpvOpConvertUToSamplerNV";
  else if( v == SpvOpConvertImageToUNV )
    root = "SpvOpConvertImageToUNV";
  else if( v == SpvOpConvertSamplerToUNV )
    root = "SpvOpConvertSamplerToUNV";
  else if( v == SpvOpConvertUToSampledImageNV )
    root = "SpvOpConvertUToSampledImageNV";
  else if( v == SpvOpConvertSampledImageToUNV )
    root = "SpvOpConvertSampledImageToUNV";
  else if( v == SpvOpSamplerImageAddressingModeNV )
    root = "SpvOpSamplerImageAddressingModeNV";
  else if( v == SpvOpSubgroupShuffleINTEL )
    root = "SpvOpSubgroupShuffleINTEL";
  else if( v == SpvOpSubgroupShuffleDownINTEL )
    root = "SpvOpSubgroupShuffleDownINTEL";
  else if( v == SpvOpSubgroupShuffleUpINTEL )
    root = "SpvOpSubgroupShuffleUpINTEL";
  else if( v == SpvOpSubgroupShuffleXorINTEL )
    root = "SpvOpSubgroupShuffleXorINTEL";
  else if( v == SpvOpSubgroupBlockReadINTEL )
    root = "SpvOpSubgroupBlockReadINTEL";
  else if( v == SpvOpSubgroupBlockWriteINTEL )
    root = "SpvOpSubgroupBlockWriteINTEL";
  else if( v == SpvOpSubgroupImageBlockReadINTEL )
    root = "SpvOpSubgroupImageBlockReadINTEL";
  else if( v == SpvOpSubgroupImageBlockWriteINTEL )
    root = "SpvOpSubgroupImageBlockWriteINTEL";
  else if( v == SpvOpSubgroupImageMediaBlockReadINTEL )
    root = "SpvOpSubgroupImageMediaBlockReadINTEL";
  else if( v == SpvOpSubgroupImageMediaBlockWriteINTEL )
    root = "SpvOpSubgroupImageMediaBlockWriteINTEL";
  else if( v == SpvOpUCountLeadingZerosINTEL )
    root = "SpvOpUCountLeadingZerosINTEL";
  else if( v == SpvOpUCountTrailingZerosINTEL )
    root = "SpvOpUCountTrailingZerosINTEL";
  else if( v == SpvOpAbsISubINTEL )
    root = "SpvOpAbsISubINTEL";
  else if( v == SpvOpAbsUSubINTEL )
    root = "SpvOpAbsUSubINTEL";
  else if( v == SpvOpIAddSatINTEL )
    root = "SpvOpIAddSatINTEL";
  else if( v == SpvOpUAddSatINTEL )
    root = "SpvOpUAddSatINTEL";
  else if( v == SpvOpIAverageINTEL )
    root = "SpvOpIAverageINTEL";
  else if( v == SpvOpUAverageINTEL )
    root = "SpvOpUAverageINTEL";
  else if( v == SpvOpIAverageRoundedINTEL )
    root = "SpvOpIAverageRoundedINTEL";
  else if( v == SpvOpUAverageRoundedINTEL )
    root = "SpvOpUAverageRoundedINTEL";
  else if( v == SpvOpISubSatINTEL )
    root = "SpvOpISubSatINTEL";
  else if( v == SpvOpUSubSatINTEL )
    root = "SpvOpUSubSatINTEL";
  else if( v == SpvOpIMul32x16INTEL )
    root = "SpvOpIMul32x16INTEL";
  else if( v == SpvOpUMul32x16INTEL )
    root = "SpvOpUMul32x16INTEL";
  else if( v == SpvOpConstantFunctionPointerINTEL )
    root = "SpvOpConstantFunctionPointerINTEL";
  else if( v == SpvOpFunctionPointerCallINTEL )
    root = "SpvOpFunctionPointerCallINTEL";
  else if( v == SpvOpAsmTargetINTEL )
    root = "SpvOpAsmTargetINTEL";
  else if( v == SpvOpAsmINTEL )
    root = "SpvOpAsmINTEL";
  else if( v == SpvOpAsmCallINTEL )
    root = "SpvOpAsmCallINTEL";
  else if( v == SpvOpAtomicFMinEXT )
    root = "SpvOpAtomicFMinEXT";
  else if( v == SpvOpAtomicFMaxEXT )
    root = "SpvOpAtomicFMaxEXT";
  else if( v == SpvOpAssumeTrueKHR )
    root = "SpvOpAssumeTrueKHR";
  else if( v == SpvOpExpectKHR )
    root = "SpvOpExpectKHR";
  else if( v == SpvOpDecorateString )
    root = "SpvOpDecorateString";
  else if( v == SpvOpDecorateStringGOOGLE )
    root = "SpvOpDecorateStringGOOGLE";
  else if( v == SpvOpMemberDecorateString )
    root = "SpvOpMemberDecorateString";
  else if( v == SpvOpMemberDecorateStringGOOGLE )
    root = "SpvOpMemberDecorateStringGOOGLE";
  else if( v == SpvOpVmeImageINTEL )
    root = "SpvOpVmeImageINTEL";
  else if( v == SpvOpTypeVmeImageINTEL )
    root = "SpvOpTypeVmeImageINTEL";
  else if( v == SpvOpTypeAvcImePayloadINTEL )
    root = "SpvOpTypeAvcImePayloadINTEL";
  else if( v == SpvOpTypeAvcRefPayloadINTEL )
    root = "SpvOpTypeAvcRefPayloadINTEL";
  else if( v == SpvOpTypeAvcSicPayloadINTEL )
    root = "SpvOpTypeAvcSicPayloadINTEL";
  else if( v == SpvOpTypeAvcMcePayloadINTEL )
    root = "SpvOpTypeAvcMcePayloadINTEL";
  else if( v == SpvOpTypeAvcMceResultINTEL )
    root = "SpvOpTypeAvcMceResultINTEL";
  else if( v == SpvOpTypeAvcImeResultINTEL )
    root = "SpvOpTypeAvcImeResultINTEL";
  else if( v == SpvOpTypeAvcImeResultSingleReferenceStreamoutINTEL )
    root = "SpvOpTypeAvcImeResultSingleReferenceStreamoutINTEL";
  else if( v == SpvOpTypeAvcImeResultDualReferenceStreamoutINTEL )
    root = "SpvOpTypeAvcImeResultDualReferenceStreamoutINTEL";
  else if( v == SpvOpTypeAvcImeSingleReferenceStreaminINTEL )
    root = "SpvOpTypeAvcImeSingleReferenceStreaminINTEL";
  else if( v == SpvOpTypeAvcImeDualReferenceStreaminINTEL )
    root = "SpvOpTypeAvcImeDualReferenceStreaminINTEL";
  else if( v == SpvOpTypeAvcRefResultINTEL )
    root = "SpvOpTypeAvcRefResultINTEL";
  else if( v == SpvOpTypeAvcSicResultINTEL )
    root = "SpvOpTypeAvcSicResultINTEL";
  else if( v == SpvOpSubgroupAvcMceGetDefaultInterBaseMultiReferencePenaltyINTEL )
    root = "SpvOpSubgroupAvcMceGetDefaultInterBaseMultiReferencePenaltyINTEL";
  else if( v == SpvOpSubgroupAvcMceSetInterBaseMultiReferencePenaltyINTEL )
    root = "SpvOpSubgroupAvcMceSetInterBaseMultiReferencePenaltyINTEL";
  else if( v == SpvOpSubgroupAvcMceGetDefaultInterShapePenaltyINTEL )
    root = "SpvOpSubgroupAvcMceGetDefaultInterShapePenaltyINTEL";
  else if( v == SpvOpSubgroupAvcMceSetInterShapePenaltyINTEL )
    root = "SpvOpSubgroupAvcMceSetInterShapePenaltyINTEL";
  else if( v == SpvOpSubgroupAvcMceGetDefaultInterDirectionPenaltyINTEL )
    root = "SpvOpSubgroupAvcMceGetDefaultInterDirectionPenaltyINTEL";
  else if( v == SpvOpSubgroupAvcMceSetInterDirectionPenaltyINTEL )
    root = "SpvOpSubgroupAvcMceSetInterDirectionPenaltyINTEL";
  else if( v == SpvOpSubgroupAvcMceGetDefaultIntraLumaShapePenaltyINTEL )
    root = "SpvOpSubgroupAvcMceGetDefaultIntraLumaShapePenaltyINTEL";
  else if( v == SpvOpSubgroupAvcMceGetDefaultInterMotionVectorCostTableINTEL )
    root = "SpvOpSubgroupAvcMceGetDefaultInterMotionVectorCostTableINTEL";
  else if( v == SpvOpSubgroupAvcMceGetDefaultHighPenaltyCostTableINTEL )
    root = "SpvOpSubgroupAvcMceGetDefaultHighPenaltyCostTableINTEL";
  else if( v == SpvOpSubgroupAvcMceGetDefaultMediumPenaltyCostTableINTEL )
    root = "SpvOpSubgroupAvcMceGetDefaultMediumPenaltyCostTableINTEL";
  else if( v == SpvOpSubgroupAvcMceGetDefaultLowPenaltyCostTableINTEL )
    root = "SpvOpSubgroupAvcMceGetDefaultLowPenaltyCostTableINTEL";
  else if( v == SpvOpSubgroupAvcMceSetMotionVectorCostFunctionINTEL )
    root = "SpvOpSubgroupAvcMceSetMotionVectorCostFunctionINTEL";
  else if( v == SpvOpSubgroupAvcMceGetDefaultIntraLumaModePenaltyINTEL )
    root = "SpvOpSubgroupAvcMceGetDefaultIntraLumaModePenaltyINTEL";
  else if( v == SpvOpSubgroupAvcMceGetDefaultNonDcLumaIntraPenaltyINTEL )
    root = "SpvOpSubgroupAvcMceGetDefaultNonDcLumaIntraPenaltyINTEL";
  else if( v == SpvOpSubgroupAvcMceGetDefaultIntraChromaModeBasePenaltyINTEL )
    root = "SpvOpSubgroupAvcMceGetDefaultIntraChromaModeBasePenaltyINTEL";
  else if( v == SpvOpSubgroupAvcMceSetAcOnlyHaarINTEL )
    root = "SpvOpSubgroupAvcMceSetAcOnlyHaarINTEL";
  else if( v == SpvOpSubgroupAvcMceSetSourceInterlacedFieldPolarityINTEL )
    root = "SpvOpSubgroupAvcMceSetSourceInterlacedFieldPolarityINTEL";
  else if( v == SpvOpSubgroupAvcMceSetSingleReferenceInterlacedFieldPolarityINTEL )
    root = "SpvOpSubgroupAvcMceSetSingleReferenceInterlacedFieldPolarityINTEL";
  else if( v == SpvOpSubgroupAvcMceSetDualReferenceInterlacedFieldPolaritiesINTEL )
    root = "SpvOpSubgroupAvcMceSetDualReferenceInterlacedFieldPolaritiesINTEL";
  else if( v == SpvOpSubgroupAvcMceConvertToImePayloadINTEL )
    root = "SpvOpSubgroupAvcMceConvertToImePayloadINTEL";
  else if( v == SpvOpSubgroupAvcMceConvertToImeResultINTEL )
    root = "SpvOpSubgroupAvcMceConvertToImeResultINTEL";
  else if( v == SpvOpSubgroupAvcMceConvertToRefPayloadINTEL )
    root = "SpvOpSubgroupAvcMceConvertToRefPayloadINTEL";
  else if( v == SpvOpSubgroupAvcMceConvertToRefResultINTEL )
    root = "SpvOpSubgroupAvcMceConvertToRefResultINTEL";
  else if( v == SpvOpSubgroupAvcMceConvertToSicPayloadINTEL )
    root = "SpvOpSubgroupAvcMceConvertToSicPayloadINTEL";
  else if( v == SpvOpSubgroupAvcMceConvertToSicResultINTEL )
    root = "SpvOpSubgroupAvcMceConvertToSicResultINTEL";
  else if( v == SpvOpSubgroupAvcMceGetMotionVectorsINTEL )
    root = "SpvOpSubgroupAvcMceGetMotionVectorsINTEL";
  else if( v == SpvOpSubgroupAvcMceGetInterDistortionsINTEL )
    root = "SpvOpSubgroupAvcMceGetInterDistortionsINTEL";
  else if( v == SpvOpSubgroupAvcMceGetBestInterDistortionsINTEL )
    root = "SpvOpSubgroupAvcMceGetBestInterDistortionsINTEL";
  else if( v == SpvOpSubgroupAvcMceGetInterMajorShapeINTEL )
    root = "SpvOpSubgroupAvcMceGetInterMajorShapeINTEL";
  else if( v == SpvOpSubgroupAvcMceGetInterMinorShapeINTEL )
    root = "SpvOpSubgroupAvcMceGetInterMinorShapeINTEL";
  else if( v == SpvOpSubgroupAvcMceGetInterDirectionsINTEL )
    root = "SpvOpSubgroupAvcMceGetInterDirectionsINTEL";
  else if( v == SpvOpSubgroupAvcMceGetInterMotionVectorCountINTEL )
    root = "SpvOpSubgroupAvcMceGetInterMotionVectorCountINTEL";
  else if( v == SpvOpSubgroupAvcMceGetInterReferenceIdsINTEL )
    root = "SpvOpSubgroupAvcMceGetInterReferenceIdsINTEL";
  else if( v == SpvOpSubgroupAvcMceGetInterReferenceInterlacedFieldPolaritiesINTEL )
    root = "SpvOpSubgroupAvcMceGetInterReferenceInterlacedFieldPolaritiesINTEL";
  else if( v == SpvOpSubgroupAvcImeInitializeINTEL )
    root = "SpvOpSubgroupAvcImeInitializeINTEL";
  else if( v == SpvOpSubgroupAvcImeSetSingleReferenceINTEL )
    root = "SpvOpSubgroupAvcImeSetSingleReferenceINTEL";
  else if( v == SpvOpSubgroupAvcImeSetDualReferenceINTEL )
    root = "SpvOpSubgroupAvcImeSetDualReferenceINTEL";
  else if( v == SpvOpSubgroupAvcImeRefWindowSizeINTEL )
    root = "SpvOpSubgroupAvcImeRefWindowSizeINTEL";
  else if( v == SpvOpSubgroupAvcImeAdjustRefOffsetINTEL )
    root = "SpvOpSubgroupAvcImeAdjustRefOffsetINTEL";
  else if( v == SpvOpSubgroupAvcImeConvertToMcePayloadINTEL )
    root = "SpvOpSubgroupAvcImeConvertToMcePayloadINTEL";
  else if( v == SpvOpSubgroupAvcImeSetMaxMotionVectorCountINTEL )
    root = "SpvOpSubgroupAvcImeSetMaxMotionVectorCountINTEL";
  else if( v == SpvOpSubgroupAvcImeSetUnidirectionalMixDisableINTEL )
    root = "SpvOpSubgroupAvcImeSetUnidirectionalMixDisableINTEL";
  else if( v == SpvOpSubgroupAvcImeSetEarlySearchTerminationThresholdINTEL )
    root = "SpvOpSubgroupAvcImeSetEarlySearchTerminationThresholdINTEL";
  else if( v == SpvOpSubgroupAvcImeSetWeightedSadINTEL )
    root = "SpvOpSubgroupAvcImeSetWeightedSadINTEL";
  else if( v == SpvOpSubgroupAvcImeEvaluateWithSingleReferenceINTEL )
    root = "SpvOpSubgroupAvcImeEvaluateWithSingleReferenceINTEL";
  else if( v == SpvOpSubgroupAvcImeEvaluateWithDualReferenceINTEL )
    root = "SpvOpSubgroupAvcImeEvaluateWithDualReferenceINTEL";
  else if( v == SpvOpSubgroupAvcImeEvaluateWithSingleReferenceStreaminINTEL )
    root = "SpvOpSubgroupAvcImeEvaluateWithSingleReferenceStreaminINTEL";
  else if( v == SpvOpSubgroupAvcImeEvaluateWithDualReferenceStreaminINTEL )
    root = "SpvOpSubgroupAvcImeEvaluateWithDualReferenceStreaminINTEL";
  else if( v == SpvOpSubgroupAvcImeEvaluateWithSingleReferenceStreamoutINTEL )
    root = "SpvOpSubgroupAvcImeEvaluateWithSingleReferenceStreamoutINTEL";
  else if( v == SpvOpSubgroupAvcImeEvaluateWithDualReferenceStreamoutINTEL )
    root = "SpvOpSubgroupAvcImeEvaluateWithDualReferenceStreamoutINTEL";
  else if( v == SpvOpSubgroupAvcImeEvaluateWithSingleReferenceStreaminoutINTEL )
    root = "SpvOpSubgroupAvcImeEvaluateWithSingleReferenceStreaminoutINTEL";
  else if( v == SpvOpSubgroupAvcImeEvaluateWithDualReferenceStreaminoutINTEL )
    root = "SpvOpSubgroupAvcImeEvaluateWithDualReferenceStreaminoutINTEL";
  else if( v == SpvOpSubgroupAvcImeConvertToMceResultINTEL )
    root = "SpvOpSubgroupAvcImeConvertToMceResultINTEL";
  else if( v == SpvOpSubgroupAvcImeGetSingleReferenceStreaminINTEL )
    root = "SpvOpSubgroupAvcImeGetSingleReferenceStreaminINTEL";
  else if( v == SpvOpSubgroupAvcImeGetDualReferenceStreaminINTEL )
    root = "SpvOpSubgroupAvcImeGetDualReferenceStreaminINTEL";
  else if( v == SpvOpSubgroupAvcImeStripSingleReferenceStreamoutINTEL )
    root = "SpvOpSubgroupAvcImeStripSingleReferenceStreamoutINTEL";
  else if( v == SpvOpSubgroupAvcImeStripDualReferenceStreamoutINTEL )
    root = "SpvOpSubgroupAvcImeStripDualReferenceStreamoutINTEL";
  else if( v == SpvOpSubgroupAvcImeGetStreamoutSingleReferenceMajorShapeMotionVectorsINTEL )
    root = "SpvOpSubgroupAvcImeGetStreamoutSingleReferenceMajorShapeMotionVectorsINTEL";
  else if( v == SpvOpSubgroupAvcImeGetStreamoutSingleReferenceMajorShapeDistortionsINTEL )
    root = "SpvOpSubgroupAvcImeGetStreamoutSingleReferenceMajorShapeDistortionsINTEL";
  else if( v == SpvOpSubgroupAvcImeGetStreamoutSingleReferenceMajorShapeReferenceIdsINTEL )
    root = "SpvOpSubgroupAvcImeGetStreamoutSingleReferenceMajorShapeReferenceIdsINTEL";
  else if( v == SpvOpSubgroupAvcImeGetStreamoutDualReferenceMajorShapeMotionVectorsINTEL )
    root = "SpvOpSubgroupAvcImeGetStreamoutDualReferenceMajorShapeMotionVectorsINTEL";
  else if( v == SpvOpSubgroupAvcImeGetStreamoutDualReferenceMajorShapeDistortionsINTEL )
    root = "SpvOpSubgroupAvcImeGetStreamoutDualReferenceMajorShapeDistortionsINTEL";
  else if( v == SpvOpSubgroupAvcImeGetStreamoutDualReferenceMajorShapeReferenceIdsINTEL )
    root = "SpvOpSubgroupAvcImeGetStreamoutDualReferenceMajorShapeReferenceIdsINTEL";
  else if( v == SpvOpSubgroupAvcImeGetBorderReachedINTEL )
    root = "SpvOpSubgroupAvcImeGetBorderReachedINTEL";
  else if( v == SpvOpSubgroupAvcImeGetTruncatedSearchIndicationINTEL )
    root = "SpvOpSubgroupAvcImeGetTruncatedSearchIndicationINTEL";
  else if( v == SpvOpSubgroupAvcImeGetUnidirectionalEarlySearchTerminationINTEL )
    root = "SpvOpSubgroupAvcImeGetUnidirectionalEarlySearchTerminationINTEL";
  else if( v == SpvOpSubgroupAvcImeGetWeightingPatternMinimumMotionVectorINTEL )
    root = "SpvOpSubgroupAvcImeGetWeightingPatternMinimumMotionVectorINTEL";
  else if( v == SpvOpSubgroupAvcImeGetWeightingPatternMinimumDistortionINTEL )
    root = "SpvOpSubgroupAvcImeGetWeightingPatternMinimumDistortionINTEL";
  else if( v == SpvOpSubgroupAvcFmeInitializeINTEL )
    root = "SpvOpSubgroupAvcFmeInitializeINTEL";
  else if( v == SpvOpSubgroupAvcBmeInitializeINTEL )
    root = "SpvOpSubgroupAvcBmeInitializeINTEL";
  else if( v == SpvOpSubgroupAvcRefConvertToMcePayloadINTEL )
    root = "SpvOpSubgroupAvcRefConvertToMcePayloadINTEL";
  else if( v == SpvOpSubgroupAvcRefSetBidirectionalMixDisableINTEL )
    root = "SpvOpSubgroupAvcRefSetBidirectionalMixDisableINTEL";
  else if( v == SpvOpSubgroupAvcRefSetBilinearFilterEnableINTEL )
    root = "SpvOpSubgroupAvcRefSetBilinearFilterEnableINTEL";
  else if( v == SpvOpSubgroupAvcRefEvaluateWithSingleReferenceINTEL )
    root = "SpvOpSubgroupAvcRefEvaluateWithSingleReferenceINTEL";
  else if( v == SpvOpSubgroupAvcRefEvaluateWithDualReferenceINTEL )
    root = "SpvOpSubgroupAvcRefEvaluateWithDualReferenceINTEL";
  else if( v == SpvOpSubgroupAvcRefEvaluateWithMultiReferenceINTEL )
    root = "SpvOpSubgroupAvcRefEvaluateWithMultiReferenceINTEL";
  else if( v == SpvOpSubgroupAvcRefEvaluateWithMultiReferenceInterlacedINTEL )
    root = "SpvOpSubgroupAvcRefEvaluateWithMultiReferenceInterlacedINTEL";
  else if( v == SpvOpSubgroupAvcRefConvertToMceResultINTEL )
    root = "SpvOpSubgroupAvcRefConvertToMceResultINTEL";
  else if( v == SpvOpSubgroupAvcSicInitializeINTEL )
    root = "SpvOpSubgroupAvcSicInitializeINTEL";
  else if( v == SpvOpSubgroupAvcSicConfigureSkcINTEL )
    root = "SpvOpSubgroupAvcSicConfigureSkcINTEL";
  else if( v == SpvOpSubgroupAvcSicConfigureIpeLumaINTEL )
    root = "SpvOpSubgroupAvcSicConfigureIpeLumaINTEL";
  else if( v == SpvOpSubgroupAvcSicConfigureIpeLumaChromaINTEL )
    root = "SpvOpSubgroupAvcSicConfigureIpeLumaChromaINTEL";
  else if( v == SpvOpSubgroupAvcSicGetMotionVectorMaskINTEL )
    root = "SpvOpSubgroupAvcSicGetMotionVectorMaskINTEL";
  else if( v == SpvOpSubgroupAvcSicConvertToMcePayloadINTEL )
    root = "SpvOpSubgroupAvcSicConvertToMcePayloadINTEL";
  else if( v == SpvOpSubgroupAvcSicSetIntraLumaShapePenaltyINTEL )
    root = "SpvOpSubgroupAvcSicSetIntraLumaShapePenaltyINTEL";
  else if( v == SpvOpSubgroupAvcSicSetIntraLumaModeCostFunctionINTEL )
    root = "SpvOpSubgroupAvcSicSetIntraLumaModeCostFunctionINTEL";
  else if( v == SpvOpSubgroupAvcSicSetIntraChromaModeCostFunctionINTEL )
    root = "SpvOpSubgroupAvcSicSetIntraChromaModeCostFunctionINTEL";
  else if( v == SpvOpSubgroupAvcSicSetBilinearFilterEnableINTEL )
    root = "SpvOpSubgroupAvcSicSetBilinearFilterEnableINTEL";
  else if( v == SpvOpSubgroupAvcSicSetSkcForwardTransformEnableINTEL )
    root = "SpvOpSubgroupAvcSicSetSkcForwardTransformEnableINTEL";
  else if( v == SpvOpSubgroupAvcSicSetBlockBasedRawSkipSadINTEL )
    root = "SpvOpSubgroupAvcSicSetBlockBasedRawSkipSadINTEL";
  else if( v == SpvOpSubgroupAvcSicEvaluateIpeINTEL )
    root = "SpvOpSubgroupAvcSicEvaluateIpeINTEL";
  else if( v == SpvOpSubgroupAvcSicEvaluateWithSingleReferenceINTEL )
    root = "SpvOpSubgroupAvcSicEvaluateWithSingleReferenceINTEL";
  else if( v == SpvOpSubgroupAvcSicEvaluateWithDualReferenceINTEL )
    root = "SpvOpSubgroupAvcSicEvaluateWithDualReferenceINTEL";
  else if( v == SpvOpSubgroupAvcSicEvaluateWithMultiReferenceINTEL )
    root = "SpvOpSubgroupAvcSicEvaluateWithMultiReferenceINTEL";
  else if( v == SpvOpSubgroupAvcSicEvaluateWithMultiReferenceInterlacedINTEL )
    root = "SpvOpSubgroupAvcSicEvaluateWithMultiReferenceInterlacedINTEL";
  else if( v == SpvOpSubgroupAvcSicConvertToMceResultINTEL )
    root = "SpvOpSubgroupAvcSicConvertToMceResultINTEL";
  else if( v == SpvOpSubgroupAvcSicGetIpeLumaShapeINTEL )
    root = "SpvOpSubgroupAvcSicGetIpeLumaShapeINTEL";
  else if( v == SpvOpSubgroupAvcSicGetBestIpeLumaDistortionINTEL )
    root = "SpvOpSubgroupAvcSicGetBestIpeLumaDistortionINTEL";
  else if( v == SpvOpSubgroupAvcSicGetBestIpeChromaDistortionINTEL )
    root = "SpvOpSubgroupAvcSicGetBestIpeChromaDistortionINTEL";
  else if( v == SpvOpSubgroupAvcSicGetPackedIpeLumaModesINTEL )
    root = "SpvOpSubgroupAvcSicGetPackedIpeLumaModesINTEL";
  else if( v == SpvOpSubgroupAvcSicGetIpeChromaModeINTEL )
    root = "SpvOpSubgroupAvcSicGetIpeChromaModeINTEL";
  else if( v == SpvOpSubgroupAvcSicGetPackedSkcLumaCountThresholdINTEL )
    root = "SpvOpSubgroupAvcSicGetPackedSkcLumaCountThresholdINTEL";
  else if( v == SpvOpSubgroupAvcSicGetPackedSkcLumaSumThresholdINTEL )
    root = "SpvOpSubgroupAvcSicGetPackedSkcLumaSumThresholdINTEL";
  else if( v == SpvOpSubgroupAvcSicGetInterRawSadsINTEL )
    root = "SpvOpSubgroupAvcSicGetInterRawSadsINTEL";
  else if( v == SpvOpVariableLengthArrayINTEL )
    root = "SpvOpVariableLengthArrayINTEL";
  else if( v == SpvOpSaveMemoryINTEL )
    root = "SpvOpSaveMemoryINTEL";
  else if( v == SpvOpRestoreMemoryINTEL )
    root = "SpvOpRestoreMemoryINTEL";
  else if( v == SpvOpArbitraryFloatSinCosPiINTEL )
    root = "SpvOpArbitraryFloatSinCosPiINTEL";
  else if( v == SpvOpArbitraryFloatCastINTEL )
    root = "SpvOpArbitraryFloatCastINTEL";
  else if( v == SpvOpArbitraryFloatCastFromIntINTEL )
    root = "SpvOpArbitraryFloatCastFromIntINTEL";
  else if( v == SpvOpArbitraryFloatCastToIntINTEL )
    root = "SpvOpArbitraryFloatCastToIntINTEL";
  else if( v == SpvOpArbitraryFloatAddINTEL )
    root = "SpvOpArbitraryFloatAddINTEL";
  else if( v == SpvOpArbitraryFloatSubINTEL )
    root = "SpvOpArbitraryFloatSubINTEL";
  else if( v == SpvOpArbitraryFloatMulINTEL )
    root = "SpvOpArbitraryFloatMulINTEL";
  else if( v == SpvOpArbitraryFloatDivINTEL )
    root = "SpvOpArbitraryFloatDivINTEL";
  else if( v == SpvOpArbitraryFloatGTINTEL )
    root = "SpvOpArbitraryFloatGTINTEL";
  else if( v == SpvOpArbitraryFloatGEINTEL )
    root = "SpvOpArbitraryFloatGEINTEL";
  else if( v == SpvOpArbitraryFloatLTINTEL )
    root = "SpvOpArbitraryFloatLTINTEL";
  else if( v == SpvOpArbitraryFloatLEINTEL )
    root = "SpvOpArbitraryFloatLEINTEL";
  else if( v == SpvOpArbitraryFloatEQINTEL )
    root = "SpvOpArbitraryFloatEQINTEL";
  else if( v == SpvOpArbitraryFloatRecipINTEL )
    root = "SpvOpArbitraryFloatRecipINTEL";
  else if( v == SpvOpArbitraryFloatRSqrtINTEL )
    root = "SpvOpArbitraryFloatRSqrtINTEL";
  else if( v == SpvOpArbitraryFloatCbrtINTEL )
    root = "SpvOpArbitraryFloatCbrtINTEL";
  else if( v == SpvOpArbitraryFloatHypotINTEL )
    root = "SpvOpArbitraryFloatHypotINTEL";
  else if( v == SpvOpArbitraryFloatSqrtINTEL )
    root = "SpvOpArbitraryFloatSqrtINTEL";
  else if( v == SpvOpArbitraryFloatLogINTEL )
    root = "SpvOpArbitraryFloatLogINTEL";
  else if( v == SpvOpArbitraryFloatLog2INTEL )
    root = "SpvOpArbitraryFloatLog2INTEL";
  else if( v == SpvOpArbitraryFloatLog10INTEL )
    root = "SpvOpArbitraryFloatLog10INTEL";
  else if( v == SpvOpArbitraryFloatLog1pINTEL )
    root = "SpvOpArbitraryFloatLog1pINTEL";
  else if( v == SpvOpArbitraryFloatExpINTEL )
    root = "SpvOpArbitraryFloatExpINTEL";
  else if( v == SpvOpArbitraryFloatExp2INTEL )
    root = "SpvOpArbitraryFloatExp2INTEL";
  else if( v == SpvOpArbitraryFloatExp10INTEL )
    root = "SpvOpArbitraryFloatExp10INTEL";
  else if( v == SpvOpArbitraryFloatExpm1INTEL )
    root = "SpvOpArbitraryFloatExpm1INTEL";
  else if( v == SpvOpArbitraryFloatSinINTEL )
    root = "SpvOpArbitraryFloatSinINTEL";
  else if( v == SpvOpArbitraryFloatCosINTEL )
    root = "SpvOpArbitraryFloatCosINTEL";
  else if( v == SpvOpArbitraryFloatSinCosINTEL )
    root = "SpvOpArbitraryFloatSinCosINTEL";
  else if( v == SpvOpArbitraryFloatSinPiINTEL )
    root = "SpvOpArbitraryFloatSinPiINTEL";
  else if( v == SpvOpArbitraryFloatCosPiINTEL )
    root = "SpvOpArbitraryFloatCosPiINTEL";
  else if( v == SpvOpArbitraryFloatASinINTEL )
    root = "SpvOpArbitraryFloatASinINTEL";
  else if( v == SpvOpArbitraryFloatASinPiINTEL )
    root = "SpvOpArbitraryFloatASinPiINTEL";
  else if( v == SpvOpArbitraryFloatACosINTEL )
    root = "SpvOpArbitraryFloatACosINTEL";
  else if( v == SpvOpArbitraryFloatACosPiINTEL )
    root = "SpvOpArbitraryFloatACosPiINTEL";
  else if( v == SpvOpArbitraryFloatATanINTEL )
    root = "SpvOpArbitraryFloatATanINTEL";
  else if( v == SpvOpArbitraryFloatATanPiINTEL )
    root = "SpvOpArbitraryFloatATanPiINTEL";
  else if( v == SpvOpArbitraryFloatATan2INTEL )
    root = "SpvOpArbitraryFloatATan2INTEL";
  else if( v == SpvOpArbitraryFloatPowINTEL )
    root = "SpvOpArbitraryFloatPowINTEL";
  else if( v == SpvOpArbitraryFloatPowRINTEL )
    root = "SpvOpArbitraryFloatPowRINTEL";
  else if( v == SpvOpArbitraryFloatPowNINTEL )
    root = "SpvOpArbitraryFloatPowNINTEL";
  else if( v == SpvOpLoopControlINTEL )
    root = "SpvOpLoopControlINTEL";
  else if( v == SpvOpAliasDomainDeclINTEL )
    root = "SpvOpAliasDomainDeclINTEL";
  else if( v == SpvOpAliasScopeDeclINTEL )
    root = "SpvOpAliasScopeDeclINTEL";
  else if( v == SpvOpAliasScopeListDeclINTEL )
    root = "SpvOpAliasScopeListDeclINTEL";
  else if( v == SpvOpFixedSqrtINTEL )
    root = "SpvOpFixedSqrtINTEL";
  else if( v == SpvOpFixedRecipINTEL )
    root = "SpvOpFixedRecipINTEL";
  else if( v == SpvOpFixedRsqrtINTEL )
    root = "SpvOpFixedRsqrtINTEL";
  else if( v == SpvOpFixedSinINTEL )
    root = "SpvOpFixedSinINTEL";
  else if( v == SpvOpFixedCosINTEL )
    root = "SpvOpFixedCosINTEL";
  else if( v == SpvOpFixedSinCosINTEL )
    root = "SpvOpFixedSinCosINTEL";
  else if( v == SpvOpFixedSinPiINTEL )
    root = "SpvOpFixedSinPiINTEL";
  else if( v == SpvOpFixedCosPiINTEL )
    root = "SpvOpFixedCosPiINTEL";
  else if( v == SpvOpFixedSinCosPiINTEL )
    root = "SpvOpFixedSinCosPiINTEL";
  else if( v == SpvOpFixedLogINTEL )
    root = "SpvOpFixedLogINTEL";
  else if( v == SpvOpFixedExpINTEL )
    root = "SpvOpFixedExpINTEL";
  else if( v == SpvOpPtrCastToCrossWorkgroupINTEL )
    root = "SpvOpPtrCastToCrossWorkgroupINTEL";
  else if( v == SpvOpCrossWorkgroupCastToPtrINTEL )
    root = "SpvOpCrossWorkgroupCastToPtrINTEL";
  else if( v == SpvOpReadPipeBlockingINTEL )
    root = "SpvOpReadPipeBlockingINTEL";
  else if( v == SpvOpWritePipeBlockingINTEL )
    root = "SpvOpWritePipeBlockingINTEL";
  else if( v == SpvOpFPGARegINTEL )
    root = "SpvOpFPGARegINTEL";
  else if( v == SpvOpRayQueryGetRayTMinKHR )
    root = "SpvOpRayQueryGetRayTMinKHR";
  else if( v == SpvOpRayQueryGetRayFlagsKHR )
    root = "SpvOpRayQueryGetRayFlagsKHR";
  else if( v == SpvOpRayQueryGetIntersectionTKHR )
    root = "SpvOpRayQueryGetIntersectionTKHR";
  else if( v == SpvOpRayQueryGetIntersectionInstanceCustomIndexKHR )
    root = "SpvOpRayQueryGetIntersectionInstanceCustomIndexKHR";
  else if( v == SpvOpRayQueryGetIntersectionInstanceIdKHR )
    root = "SpvOpRayQueryGetIntersectionInstanceIdKHR";
  else if( v == SpvOpRayQueryGetIntersectionInstanceShaderBindingTableRecordOffsetKHR )
    root = "SpvOpRayQueryGetIntersectionInstanceShaderBindingTableRecordOffsetKHR";
  else if( v == SpvOpRayQueryGetIntersectionGeometryIndexKHR )
    root = "SpvOpRayQueryGetIntersectionGeometryIndexKHR";
  else if( v == SpvOpRayQueryGetIntersectionPrimitiveIndexKHR )
    root = "SpvOpRayQueryGetIntersectionPrimitiveIndexKHR";
  else if( v == SpvOpRayQueryGetIntersectionBarycentricsKHR )
    root = "SpvOpRayQueryGetIntersectionBarycentricsKHR";
  else if( v == SpvOpRayQueryGetIntersectionFrontFaceKHR )
    root = "SpvOpRayQueryGetIntersectionFrontFaceKHR";
  else if( v == SpvOpRayQueryGetIntersectionCandidateAABBOpaqueKHR )
    root = "SpvOpRayQueryGetIntersectionCandidateAABBOpaqueKHR";
  else if( v == SpvOpRayQueryGetIntersectionObjectRayDirectionKHR )
    root = "SpvOpRayQueryGetIntersectionObjectRayDirectionKHR";
  else if( v == SpvOpRayQueryGetIntersectionObjectRayOriginKHR )
    root = "SpvOpRayQueryGetIntersectionObjectRayOriginKHR";
  else if( v == SpvOpRayQueryGetWorldRayDirectionKHR )
    root = "SpvOpRayQueryGetWorldRayDirectionKHR";
  else if( v == SpvOpRayQueryGetWorldRayOriginKHR )
    root = "SpvOpRayQueryGetWorldRayOriginKHR";
  else if( v == SpvOpRayQueryGetIntersectionObjectToWorldKHR )
    root = "SpvOpRayQueryGetIntersectionObjectToWorldKHR";
  else if( v == SpvOpRayQueryGetIntersectionWorldToObjectKHR )
    root = "SpvOpRayQueryGetIntersectionWorldToObjectKHR";
  else if( v == SpvOpAtomicFAddEXT )
    root = "SpvOpAtomicFAddEXT";
  else if( v == SpvOpTypeBufferSurfaceINTEL )
    root = "SpvOpTypeBufferSurfaceINTEL";
  else if( v == SpvOpTypeStructContinuedINTEL )
    root = "SpvOpTypeStructContinuedINTEL";
  else if( v == SpvOpConstantCompositeContinuedINTEL )
    root = "SpvOpConstantCompositeContinuedINTEL";
  else if( v == SpvOpSpecConstantCompositeContinuedINTEL )
    root = "SpvOpSpecConstantCompositeContinuedINTEL";
  else if( v == SpvOpControlBarrierArriveINTEL )
    root = "SpvOpControlBarrierArriveINTEL";
  else if( v == SpvOpControlBarrierWaitINTEL )
    root = "SpvOpControlBarrierWaitINTEL";
  else if( v == SpvOpGroupIMulKHR )
    root = "SpvOpGroupIMulKHR";
  else if( v == SpvOpGroupFMulKHR )
    root = "SpvOpGroupFMulKHR";
  else if( v == SpvOpGroupBitwiseAndKHR )
    root = "SpvOpGroupBitwiseAndKHR";
  else if( v == SpvOpGroupBitwiseOrKHR )
    root = "SpvOpGroupBitwiseOrKHR";
  else if( v == SpvOpGroupBitwiseXorKHR )
    root = "SpvOpGroupBitwiseXorKHR";
  else if( v == SpvOpGroupLogicalAndKHR )
    root = "SpvOpGroupLogicalAndKHR";
  else if( v == SpvOpGroupLogicalOrKHR )
    root = "SpvOpGroupLogicalOrKHR";
  else if( v == SpvOpGroupLogicalXorKHR )
    root = "SpvOpGroupLogicalXorKHR";
  else
    root = "Unknown";
}

void to_json( nlohmann::json &root, const SpvReflectTypeDescription &v ) {
  root = nlohmann::json::object();
  root[ "id" ] = v.id;
  root[ "op" ] = v.op;
  if( v.type_name )
    root[ "type_name" ] = v.type_name;
  if( v.struct_member_name )
    root[ "struct_member_name" ] = v.struct_member_name;
  root[ "storage_class" ] = v.storage_class;
  root[ "type_flags" ] = v.type_flags;
  root[ "decoration_flags" ] = v.decoration_flags;
  root[ "traits" ] = nlohmann::json::object();
  root[ "traits" ][ "numeric" ] = v.traits.numeric;
  root[ "traits" ][ "image" ] = v.traits.image;
  root[ "traits" ][ "array" ] = v.traits.array;
  root[ "member_count" ] = v.member_count;
  root[ "members" ] = nlohmann::json::array();
  for( unsigned int i = 0u; i != v.member_count; ++i )
    root[ "members" ].push_back( nlohmann::json( v.members[ i ] ) );
}

void to_json( nlohmann::json &root, const SpvReflectBlockVariable &v ) {
  root = nlohmann::json::object();
  root[ "spirv_id" ] = v.spirv_id;
  if( v.name )
    root[ "name" ] = v.name;
  root[ "offset" ] = v.offset;
  root[ "absolute_offset" ] = v.absolute_offset;
  root[ "size" ] = v.size;
  root[ "padded_size" ] = v.padded_size;
  root[ "decoration_flags" ] = v.decoration_flags;
  root[ "numeric" ] = v.numeric;
  root[ "array" ] = v.array;
  root[ "flags" ] = v.flags;
  root[ "member_count" ] = v.member_count;
  root[ "members" ] = nlohmann::json::array();
  for( unsigned int i = 0u; i != v.member_count; ++i )
    root[ "members" ].push_back( nlohmann::json( v.members[ i ] ) );
  if( v.type_description )
    root[ "type_description" ] = *v.type_description;
}

void to_json( nlohmann::json &root, const SpvReflectDescriptorBinding &v ) {
  root = nlohmann::json::object();
  root[ "spirv_id" ] = v.spirv_id;
  if( v.name )
    root[ "name" ] = v.name;
  root[ "binding" ] = v.binding;
  root[ "input_attachment_index" ] = v.input_attachment_index;
  root[ "set" ] = v.set;
  root[ "descriptor_type" ] = v.descriptor_type;
  root[ "resource_type" ] = v.resource_type;
  root[ "image" ] = v.image;

}

void to_json( nlohmann::json &root, const SpvReflectShaderModule &v ) {
  root = nlohmann::json::object();
  root[ "generator" ] = v.generator;
  if( v.entry_point_name )
    root[ "entry_point_name" ] = v.entry_point_name;
  root[ "entry_point_id" ] = v.entry_point_id;
  root[ "entry_point_count" ] = v.entry_point_count;
  root[ "entry_points" ] = nlohmann::json::array();
  for( unsigned int i = 0u; i != v.entry_point_count; ++i )
    root[ "entry_points" ].push_back( nlohmann::json( v.entry_points[ i ] ) );
  root[ "source_language" ] = v.source_language;
  root[ "source_language_version" ] = v.source_language_version;
  if( v.source_file )
    root[ "source_file" ] = v.source_file;
  //root[ "source_source" ] = v.source_source;
  root[ "spirv_execution_model" ] = v.spirv_execution_model;
  root[ "shader_stage" ] = v.shader_stage;
  root[ "descriptor_binding_count" ] = v.descriptor_binding_count;
  root[ "descriptor_bindings" ] = nlohmann::json::array();
  for( unsigned int i = 0u; i != v.descriptor_binding_count; ++i )
    root[ "descriptor_bindings" ].push_back( nlohmann::json( v.descriptor_bindings[ i ] ) );
  root[ "descriptor_set_count" ] = v.descriptor_set_count;
  root[ "descriptor_sets" ] = nlohmann::json::array();
  for( unsigned int i = 0u; i != v.descriptor_set_count; ++i )
    root[ "descriptor_sets" ].push_back( nlohmann::json( v.descriptor_sets[ i ] ) );
  root[ "input_variable_count" ] = v.input_variable_count;
  root[ "input_variables" ] = nlohmann::json::array();
  for( unsigned int i = 0u; i != v.input_variable_count; ++i )
    root[ "input_variables" ].push_back( nlohmann::json( *v.input_variables[ i ] ) );
  root[ "output_variable_count" ] = v.output_variable_count;
  root[ "output_variables" ] = nlohmann::json::array();
  for( unsigned int i = 0u; i != v.output_variable_count; ++i )
    root[ "output_variables" ].push_back( nlohmann::json( *v.output_variables[ i ] ) );
  root[ "interface_variable_count" ] = v.interface_variable_count;
  root[ "interface_variables" ] = nlohmann::json::array();
  for( unsigned int i = 0u; i != v.interface_variable_count; ++i )
    root[ "interface_variables" ].push_back( nlohmann::json( v.interface_variables[ i ] ) );
  root[ "push_constant_block_count" ] = v.push_constant_block_count;
  root[ "push_constant_blocks" ] = nlohmann::json::array();
  for( unsigned int i = 0u; i != v.push_constant_block_count; ++i )
    root[ "push_constant_blocks" ].push_back( nlohmann::json( v.push_constant_blocks[ i ] ) );
}

