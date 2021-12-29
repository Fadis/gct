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
    root = "Unknown";
  else if( v == SpvImageFormatRgba32f )
    root = "Rgba32f";
  else if( v == SpvImageFormatRgba16f )
    root = "Rgba16f";
  else if( v == SpvImageFormatR32f )
    root = "R32f";
  else if( v == SpvImageFormatRgba8 )
    root = "Rgba8";
  else if( v == SpvImageFormatRgba8Snorm )
    root = "Rgba8Snorm";
  else if( v == SpvImageFormatRg32f )
    root = "Rg32f";
  else if( v == SpvImageFormatRg16f )
    root = "Rg16f";
  else if( v == SpvImageFormatR11fG11fB10f )
    root = "R11fG11fB10f";
  else if( v == SpvImageFormatR16f )
    root = "R16f";
  else if( v == SpvImageFormatRgba16 )
    root = "Rgba16";
  else if( v == SpvImageFormatRgb10A2 )
    root = "Rgb10A2";
  else if( v == SpvImageFormatRg16 )
    root = "Rg16";
  else if( v == SpvImageFormatRg8 )
    root = "Rg8";
  else if( v == SpvImageFormatR16 )
    root = "R16";
  else if( v == SpvImageFormatR8 )
    root = "R8";
  else if( v == SpvImageFormatRgba16Snorm )
    root = "Rgba16Snorm";
  else if( v == SpvImageFormatRg16Snorm )
    root = "Rg16Snorm";
  else if( v == SpvImageFormatRg8Snorm )
    root = "Rg8Snorm";
  else if( v == SpvImageFormatR16Snorm )
    root = "R16Snorm";
  else if( v == SpvImageFormatR8Snorm )
    root = "R8Snorm";
  else if( v == SpvImageFormatRgba32i )
    root = "Rgba32i";
  else if( v == SpvImageFormatRgba16i )
    root = "Rgba16i";
  else if( v == SpvImageFormatRgba8i )
    root = "Rgba8i";
  else if( v == SpvImageFormatR32i )
    root = "R32i";
  else if( v == SpvImageFormatRg32i )
    root = "Rg32i";
  else if( v == SpvImageFormatRg16i )
    root = "Rg16i";
  else if( v == SpvImageFormatRg8i )
    root = "Rg8i";
  else if( v == SpvImageFormatR16i )
    root = "R16i";
  else if( v == SpvImageFormatR8i )
    root = "R8i";
  else if( v == SpvImageFormatRgba32ui )
    root = "Rgba32ui";
  else if( v == SpvImageFormatRgba16ui )
    root = "Rgba16ui";
  else if( v == SpvImageFormatRgba8ui )
    root = "Rgba8ui";
  else if( v == SpvImageFormatR32ui )
    root = "R32ui";
  else if( v == SpvImageFormatRgb10a2ui )
    root = "Rgb10a2ui";
  else if( v == SpvImageFormatRg32ui )
    root = "Rg32ui";
  else if( v == SpvImageFormatRg16ui )
    root = "Rg16ui";
  else if( v == SpvImageFormatRg8ui )
    root = "Rg8ui";
  else if( v == SpvImageFormatR16ui )
    root = "R16ui";
  else if( v == SpvImageFormatR8ui )
    root = "R8ui";
  else if( v == SpvImageFormatR64ui )
    root = "R64ui";
  else if( v == SpvImageFormatR64i )
    root = "R64i";
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
    root = "Nop";
  else if( v == SpvOpUndef )
    root = "Undef";
  else if( v == SpvOpSourceContinued )
    root = "SourceContinued";
  else if( v == SpvOpSource )
    root = "Source";
  else if( v == SpvOpSourceExtension )
    root = "SourceExtension";
  else if( v == SpvOpName )
    root = "Name";
  else if( v == SpvOpMemberName )
    root = "MemberName";
  else if( v == SpvOpString )
    root = "String";
  else if( v == SpvOpLine )
    root = "Line";
  else if( v == SpvOpExtension )
    root = "Extension";
  else if( v == SpvOpExtInstImport )
    root = "ExtInstImport";
  else if( v == SpvOpExtInst )
    root = "ExtInst";
  else if( v == SpvOpMemoryModel )
    root = "MemoryModel";
  else if( v == SpvOpEntryPoint )
    root = "EntryPoint";
  else if( v == SpvOpExecutionMode )
    root = "ExecutionMode";
  else if( v == SpvOpCapability )
    root = "Capability";
  else if( v == SpvOpTypeVoid )
    root = "TypeVoid";
  else if( v == SpvOpTypeBool )
    root = "TypeBool";
  else if( v == SpvOpTypeInt )
    root = "TypeInt";
  else if( v == SpvOpTypeFloat )
    root = "TypeFloat";
  else if( v == SpvOpTypeVector )
    root = "TypeVector";
  else if( v == SpvOpTypeMatrix )
    root = "TypeMatrix";
  else if( v == SpvOpTypeImage )
    root = "TypeImage";
  else if( v == SpvOpTypeSampler )
    root = "TypeSampler";
  else if( v == SpvOpTypeSampledImage )
    root = "TypeSampledImage";
  else if( v == SpvOpTypeArray )
    root = "TypeArray";
  else if( v == SpvOpTypeRuntimeArray )
    root = "TypeRuntimeArray";
  else if( v == SpvOpTypeStruct )
    root = "TypeStruct";
  else if( v == SpvOpTypeOpaque )
    root = "TypeOpaque";
  else if( v == SpvOpTypePointer )
    root = "TypePointer";
  else if( v == SpvOpTypeFunction )
    root = "TypeFunction";
  else if( v == SpvOpTypeEvent )
    root = "TypeEvent";
  else if( v == SpvOpTypeDeviceEvent )
    root = "TypeDeviceEvent";
  else if( v == SpvOpTypeReserveId )
    root = "TypeReserveId";
  else if( v == SpvOpTypeQueue )
    root = "TypeQueue";
  else if( v == SpvOpTypePipe )
    root = "TypePipe";
  else if( v == SpvOpTypeForwardPointer )
    root = "TypeForwardPointer";
  else if( v == SpvOpConstantTrue )
    root = "ConstantTrue";
  else if( v == SpvOpConstantFalse )
    root = "ConstantFalse";
  else if( v == SpvOpConstant )
    root = "Constant";
  else if( v == SpvOpConstantComposite )
    root = "ConstantComposite";
  else if( v == SpvOpConstantSampler )
    root = "ConstantSampler";
  else if( v == SpvOpConstantNull )
    root = "ConstantNull";
  else if( v == SpvOpSpecConstantTrue )
    root = "SpecConstantTrue";
  else if( v == SpvOpSpecConstantFalse )
    root = "SpecConstantFalse";
  else if( v == SpvOpSpecConstant )
    root = "SpecConstant";
  else if( v == SpvOpSpecConstantComposite )
    root = "SpecConstantComposite";
  else if( v == SpvOpSpecConstantOp )
    root = "SpecConstantOp";
  else if( v == SpvOpFunction )
    root = "Function";
  else if( v == SpvOpFunctionParameter )
    root = "FunctionParameter";
  else if( v == SpvOpFunctionEnd )
    root = "FunctionEnd";
  else if( v == SpvOpFunctionCall )
    root = "FunctionCall";
  else if( v == SpvOpVariable )
    root = "Variable";
  else if( v == SpvOpImageTexelPointer )
    root = "ImageTexelPointer";
  else if( v == SpvOpLoad )
    root = "Load";
  else if( v == SpvOpStore )
    root = "Store";
  else if( v == SpvOpCopyMemory )
    root = "CopyMemory";
  else if( v == SpvOpCopyMemorySized )
    root = "CopyMemorySized";
  else if( v == SpvOpAccessChain )
    root = "AccessChain";
  else if( v == SpvOpInBoundsAccessChain )
    root = "InBoundsAccessChain";
  else if( v == SpvOpPtrAccessChain )
    root = "PtrAccessChain";
  else if( v == SpvOpArrayLength )
    root = "ArrayLength";
  else if( v == SpvOpGenericPtrMemSemantics )
    root = "GenericPtrMemSemantics";
  else if( v == SpvOpInBoundsPtrAccessChain )
    root = "InBoundsPtrAccessChain";
  else if( v == SpvOpDecorate )
    root = "Decorate";
  else if( v == SpvOpMemberDecorate )
    root = "MemberDecorate";
  else if( v == SpvOpDecorationGroup )
    root = "DecorationGroup";
  else if( v == SpvOpGroupDecorate )
    root = "GroupDecorate";
  else if( v == SpvOpGroupMemberDecorate )
    root = "GroupMemberDecorate";
  else if( v == SpvOpVectorExtractDynamic )
    root = "VectorExtractDynamic";
  else if( v == SpvOpVectorInsertDynamic )
    root = "VectorInsertDynamic";
  else if( v == SpvOpVectorShuffle )
    root = "VectorShuffle";
  else if( v == SpvOpCompositeConstruct )
    root = "CompositeConstruct";
  else if( v == SpvOpCompositeExtract )
    root = "CompositeExtract";
  else if( v == SpvOpCompositeInsert )
    root = "CompositeInsert";
  else if( v == SpvOpCopyObject )
    root = "CopyObject";
  else if( v == SpvOpTranspose )
    root = "Transpose";
  else if( v == SpvOpSampledImage )
    root = "SampledImage";
  else if( v == SpvOpImageSampleImplicitLod )
    root = "ImageSampleImplicitLod";
  else if( v == SpvOpImageSampleExplicitLod )
    root = "ImageSampleExplicitLod";
  else if( v == SpvOpImageSampleDrefImplicitLod )
    root = "ImageSampleDrefImplicitLod";
  else if( v == SpvOpImageSampleDrefExplicitLod )
    root = "ImageSampleDrefExplicitLod";
  else if( v == SpvOpImageSampleProjImplicitLod )
    root = "ImageSampleProjImplicitLod";
  else if( v == SpvOpImageSampleProjExplicitLod )
    root = "ImageSampleProjExplicitLod";
  else if( v == SpvOpImageSampleProjDrefImplicitLod )
    root = "ImageSampleProjDrefImplicitLod";
  else if( v == SpvOpImageSampleProjDrefExplicitLod )
    root = "ImageSampleProjDrefExplicitLod";
  else if( v == SpvOpImageFetch )
    root = "ImageFetch";
  else if( v == SpvOpImageGather )
    root = "ImageGather";
  else if( v == SpvOpImageDrefGather )
    root = "ImageDrefGather";
  else if( v == SpvOpImageRead )
    root = "ImageRead";
  else if( v == SpvOpImageWrite )
    root = "ImageWrite";
  else if( v == SpvOpImage )
    root = "Image";
  else if( v == SpvOpImageQueryFormat )
    root = "ImageQueryFormat";
  else if( v == SpvOpImageQueryOrder )
    root = "ImageQueryOrder";
  else if( v == SpvOpImageQuerySizeLod )
    root = "ImageQuerySizeLod";
  else if( v == SpvOpImageQuerySize )
    root = "ImageQuerySize";
  else if( v == SpvOpImageQueryLod )
    root = "ImageQueryLod";
  else if( v == SpvOpImageQueryLevels )
    root = "ImageQueryLevels";
  else if( v == SpvOpImageQuerySamples )
    root = "ImageQuerySamples";
  else if( v == SpvOpConvertFToU )
    root = "ConvertFToU";
  else if( v == SpvOpConvertFToS )
    root = "ConvertFToS";
  else if( v == SpvOpConvertSToF )
    root = "ConvertSToF";
  else if( v == SpvOpConvertUToF )
    root = "ConvertUToF";
  else if( v == SpvOpUConvert )
    root = "UConvert";
  else if( v == SpvOpSConvert )
    root = "SConvert";
  else if( v == SpvOpFConvert )
    root = "FConvert";
  else if( v == SpvOpQuantizeToF16 )
    root = "QuantizeToF16";
  else if( v == SpvOpConvertPtrToU )
    root = "ConvertPtrToU";
  else if( v == SpvOpSatConvertSToU )
    root = "SatConvertSToU";
  else if( v == SpvOpSatConvertUToS )
    root = "SatConvertUToS";
  else if( v == SpvOpConvertUToPtr )
    root = "ConvertUToPtr";
  else if( v == SpvOpPtrCastToGeneric )
    root = "PtrCastToGeneric";
  else if( v == SpvOpGenericCastToPtr )
    root = "GenericCastToPtr";
  else if( v == SpvOpGenericCastToPtrExplicit )
    root = "GenericCastToPtrExplicit";
  else if( v == SpvOpBitcast )
    root = "Bitcast";
  else if( v == SpvOpSNegate )
    root = "SNegate";
  else if( v == SpvOpFNegate )
    root = "FNegate";
  else if( v == SpvOpIAdd )
    root = "IAdd";
  else if( v == SpvOpFAdd )
    root = "FAdd";
  else if( v == SpvOpISub )
    root = "ISub";
  else if( v == SpvOpFSub )
    root = "FSub";
  else if( v == SpvOpIMul )
    root = "IMul";
  else if( v == SpvOpFMul )
    root = "FMul";
  else if( v == SpvOpUDiv )
    root = "UDiv";
  else if( v == SpvOpSDiv )
    root = "SDiv";
  else if( v == SpvOpFDiv )
    root = "FDiv";
  else if( v == SpvOpUMod )
    root = "UMod";
  else if( v == SpvOpSRem )
    root = "SRem";
  else if( v == SpvOpSMod )
    root = "SMod";
  else if( v == SpvOpFRem )
    root = "FRem";
  else if( v == SpvOpFMod )
    root = "FMod";
  else if( v == SpvOpVectorTimesScalar )
    root = "VectorTimesScalar";
  else if( v == SpvOpMatrixTimesScalar )
    root = "MatrixTimesScalar";
  else if( v == SpvOpVectorTimesMatrix )
    root = "VectorTimesMatrix";
  else if( v == SpvOpMatrixTimesVector )
    root = "MatrixTimesVector";
  else if( v == SpvOpMatrixTimesMatrix )
    root = "MatrixTimesMatrix";
  else if( v == SpvOpOuterProduct )
    root = "OuterProduct";
  else if( v == SpvOpDot )
    root = "Dot";
  else if( v == SpvOpIAddCarry )
    root = "IAddCarry";
  else if( v == SpvOpISubBorrow )
    root = "ISubBorrow";
  else if( v == SpvOpUMulExtended )
    root = "UMulExtended";
  else if( v == SpvOpSMulExtended )
    root = "SMulExtended";
  else if( v == SpvOpAny )
    root = "Any";
  else if( v == SpvOpAll )
    root = "All";
  else if( v == SpvOpIsNan )
    root = "IsNan";
  else if( v == SpvOpIsInf )
    root = "IsInf";
  else if( v == SpvOpIsFinite )
    root = "IsFinite";
  else if( v == SpvOpIsNormal )
    root = "IsNormal";
  else if( v == SpvOpSignBitSet )
    root = "SignBitSet";
  else if( v == SpvOpLessOrGreater )
    root = "LessOrGreater";
  else if( v == SpvOpOrdered )
    root = "Ordered";
  else if( v == SpvOpUnordered )
    root = "Unordered";
  else if( v == SpvOpLogicalEqual )
    root = "LogicalEqual";
  else if( v == SpvOpLogicalNotEqual )
    root = "LogicalNotEqual";
  else if( v == SpvOpLogicalOr )
    root = "LogicalOr";
  else if( v == SpvOpLogicalAnd )
    root = "LogicalAnd";
  else if( v == SpvOpLogicalNot )
    root = "LogicalNot";
  else if( v == SpvOpSelect )
    root = "Select";
  else if( v == SpvOpIEqual )
    root = "IEqual";
  else if( v == SpvOpINotEqual )
    root = "INotEqual";
  else if( v == SpvOpUGreaterThan )
    root = "UGreaterThan";
  else if( v == SpvOpSGreaterThan )
    root = "SGreaterThan";
  else if( v == SpvOpUGreaterThanEqual )
    root = "UGreaterThanEqual";
  else if( v == SpvOpSGreaterThanEqual )
    root = "SGreaterThanEqual";
  else if( v == SpvOpULessThan )
    root = "ULessThan";
  else if( v == SpvOpSLessThan )
    root = "SLessThan";
  else if( v == SpvOpULessThanEqual )
    root = "ULessThanEqual";
  else if( v == SpvOpSLessThanEqual )
    root = "SLessThanEqual";
  else if( v == SpvOpFOrdEqual )
    root = "FOrdEqual";
  else if( v == SpvOpFUnordEqual )
    root = "FUnordEqual";
  else if( v == SpvOpFOrdNotEqual )
    root = "FOrdNotEqual";
  else if( v == SpvOpFUnordNotEqual )
    root = "FUnordNotEqual";
  else if( v == SpvOpFOrdLessThan )
    root = "FOrdLessThan";
  else if( v == SpvOpFUnordLessThan )
    root = "FUnordLessThan";
  else if( v == SpvOpFOrdGreaterThan )
    root = "FOrdGreaterThan";
  else if( v == SpvOpFUnordGreaterThan )
    root = "FUnordGreaterThan";
  else if( v == SpvOpFOrdLessThanEqual )
    root = "FOrdLessThanEqual";
  else if( v == SpvOpFUnordLessThanEqual )
    root = "FUnordLessThanEqual";
  else if( v == SpvOpFOrdGreaterThanEqual )
    root = "FOrdGreaterThanEqual";
  else if( v == SpvOpFUnordGreaterThanEqual )
    root = "FUnordGreaterThanEqual";
  else if( v == SpvOpShiftRightLogical )
    root = "ShiftRightLogical";
  else if( v == SpvOpShiftRightArithmetic )
    root = "ShiftRightArithmetic";
  else if( v == SpvOpShiftLeftLogical )
    root = "ShiftLeftLogical";
  else if( v == SpvOpBitwiseOr )
    root = "BitwiseOr";
  else if( v == SpvOpBitwiseXor )
    root = "BitwiseXor";
  else if( v == SpvOpBitwiseAnd )
    root = "BitwiseAnd";
  else if( v == SpvOpNot )
    root = "Not";
  else if( v == SpvOpBitFieldInsert )
    root = "BitFieldInsert";
  else if( v == SpvOpBitFieldSExtract )
    root = "BitFieldSExtract";
  else if( v == SpvOpBitFieldUExtract )
    root = "BitFieldUExtract";
  else if( v == SpvOpBitReverse )
    root = "BitReverse";
  else if( v == SpvOpBitCount )
    root = "BitCount";
  else if( v == SpvOpDPdx )
    root = "DPdx";
  else if( v == SpvOpDPdy )
    root = "DPdy";
  else if( v == SpvOpFwidth )
    root = "Fwidth";
  else if( v == SpvOpDPdxFine )
    root = "DPdxFine";
  else if( v == SpvOpDPdyFine )
    root = "DPdyFine";
  else if( v == SpvOpFwidthFine )
    root = "FwidthFine";
  else if( v == SpvOpDPdxCoarse )
    root = "DPdxCoarse";
  else if( v == SpvOpDPdyCoarse )
    root = "DPdyCoarse";
  else if( v == SpvOpFwidthCoarse )
    root = "FwidthCoarse";
  else if( v == SpvOpEmitVertex )
    root = "EmitVertex";
  else if( v == SpvOpEndPrimitive )
    root = "EndPrimitive";
  else if( v == SpvOpEmitStreamVertex )
    root = "EmitStreamVertex";
  else if( v == SpvOpEndStreamPrimitive )
    root = "EndStreamPrimitive";
  else if( v == SpvOpControlBarrier )
    root = "ControlBarrier";
  else if( v == SpvOpMemoryBarrier )
    root = "MemoryBarrier";
  else if( v == SpvOpAtomicLoad )
    root = "AtomicLoad";
  else if( v == SpvOpAtomicStore )
    root = "AtomicStore";
  else if( v == SpvOpAtomicExchange )
    root = "AtomicExchange";
  else if( v == SpvOpAtomicCompareExchange )
    root = "AtomicCompareExchange";
  else if( v == SpvOpAtomicCompareExchangeWeak )
    root = "AtomicCompareExchangeWeak";
  else if( v == SpvOpAtomicIIncrement )
    root = "AtomicIIncrement";
  else if( v == SpvOpAtomicIDecrement )
    root = "AtomicIDecrement";
  else if( v == SpvOpAtomicIAdd )
    root = "AtomicIAdd";
  else if( v == SpvOpAtomicISub )
    root = "AtomicISub";
  else if( v == SpvOpAtomicSMin )
    root = "AtomicSMin";
  else if( v == SpvOpAtomicUMin )
    root = "AtomicUMin";
  else if( v == SpvOpAtomicSMax )
    root = "AtomicSMax";
  else if( v == SpvOpAtomicUMax )
    root = "AtomicUMax";
  else if( v == SpvOpAtomicAnd )
    root = "AtomicAnd";
  else if( v == SpvOpAtomicOr )
    root = "AtomicOr";
  else if( v == SpvOpAtomicXor )
    root = "AtomicXor";
  else if( v == SpvOpPhi )
    root = "Phi";
  else if( v == SpvOpLoopMerge )
    root = "LoopMerge";
  else if( v == SpvOpSelectionMerge )
    root = "SelectionMerge";
  else if( v == SpvOpLabel )
    root = "Label";
  else if( v == SpvOpBranch )
    root = "Branch";
  else if( v == SpvOpBranchConditional )
    root = "BranchConditional";
  else if( v == SpvOpSwitch )
    root = "Switch";
  else if( v == SpvOpKill )
    root = "Kill";
  else if( v == SpvOpReturn )
    root = "Return";
  else if( v == SpvOpReturnValue )
    root = "ReturnValue";
  else if( v == SpvOpUnreachable )
    root = "Unreachable";
  else if( v == SpvOpLifetimeStart )
    root = "LifetimeStart";
  else if( v == SpvOpLifetimeStop )
    root = "LifetimeStop";
  else if( v == SpvOpGroupAsyncCopy )
    root = "GroupAsyncCopy";
  else if( v == SpvOpGroupWaitEvents )
    root = "GroupWaitEvents";
  else if( v == SpvOpGroupAll )
    root = "GroupAll";
  else if( v == SpvOpGroupAny )
    root = "GroupAny";
  else if( v == SpvOpGroupBroadcast )
    root = "GroupBroadcast";
  else if( v == SpvOpGroupIAdd )
    root = "GroupIAdd";
  else if( v == SpvOpGroupFAdd )
    root = "GroupFAdd";
  else if( v == SpvOpGroupFMin )
    root = "GroupFMin";
  else if( v == SpvOpGroupUMin )
    root = "GroupUMin";
  else if( v == SpvOpGroupSMin )
    root = "GroupSMin";
  else if( v == SpvOpGroupFMax )
    root = "GroupFMax";
  else if( v == SpvOpGroupUMax )
    root = "GroupUMax";
  else if( v == SpvOpGroupSMax )
    root = "GroupSMax";
  else if( v == SpvOpReadPipe )
    root = "ReadPipe";
  else if( v == SpvOpWritePipe )
    root = "WritePipe";
  else if( v == SpvOpReservedReadPipe )
    root = "ReservedReadPipe";
  else if( v == SpvOpReservedWritePipe )
    root = "ReservedWritePipe";
  else if( v == SpvOpReserveReadPipePackets )
    root = "ReserveReadPipePackets";
  else if( v == SpvOpReserveWritePipePackets )
    root = "ReserveWritePipePackets";
  else if( v == SpvOpCommitReadPipe )
    root = "CommitReadPipe";
  else if( v == SpvOpCommitWritePipe )
    root = "CommitWritePipe";
  else if( v == SpvOpIsValidReserveId )
    root = "IsValidReserveId";
  else if( v == SpvOpGetNumPipePackets )
    root = "GetNumPipePackets";
  else if( v == SpvOpGetMaxPipePackets )
    root = "GetMaxPipePackets";
  else if( v == SpvOpGroupReserveReadPipePackets )
    root = "GroupReserveReadPipePackets";
  else if( v == SpvOpGroupReserveWritePipePackets )
    root = "GroupReserveWritePipePackets";
  else if( v == SpvOpGroupCommitReadPipe )
    root = "GroupCommitReadPipe";
  else if( v == SpvOpGroupCommitWritePipe )
    root = "GroupCommitWritePipe";
  else if( v == SpvOpEnqueueMarker )
    root = "EnqueueMarker";
  else if( v == SpvOpEnqueueKernel )
    root = "EnqueueKernel";
  else if( v == SpvOpGetKernelNDrangeSubGroupCount )
    root = "GetKernelNDrangeSubGroupCount";
  else if( v == SpvOpGetKernelNDrangeMaxSubGroupSize )
    root = "GetKernelNDrangeMaxSubGroupSize";
  else if( v == SpvOpGetKernelWorkGroupSize )
    root = "GetKernelWorkGroupSize";
  else if( v == SpvOpGetKernelPreferredWorkGroupSizeMultiple )
    root = "GetKernelPreferredWorkGroupSizeMultiple";
  else if( v == SpvOpRetainEvent )
    root = "RetainEvent";
  else if( v == SpvOpReleaseEvent )
    root = "ReleaseEvent";
  else if( v == SpvOpCreateUserEvent )
    root = "CreateUserEvent";
  else if( v == SpvOpIsValidEvent )
    root = "IsValidEvent";
  else if( v == SpvOpSetUserEventStatus )
    root = "SetUserEventStatus";
  else if( v == SpvOpCaptureEventProfilingInfo )
    root = "CaptureEventProfilingInfo";
  else if( v == SpvOpGetDefaultQueue )
    root = "GetDefaultQueue";
  else if( v == SpvOpBuildNDRange )
    root = "BuildNDRange";
  else if( v == SpvOpImageSparseSampleImplicitLod )
    root = "ImageSparseSampleImplicitLod";
  else if( v == SpvOpImageSparseSampleExplicitLod )
    root = "ImageSparseSampleExplicitLod";
  else if( v == SpvOpImageSparseSampleDrefImplicitLod )
    root = "ImageSparseSampleDrefImplicitLod";
  else if( v == SpvOpImageSparseSampleDrefExplicitLod )
    root = "ImageSparseSampleDrefExplicitLod";
  else if( v == SpvOpImageSparseSampleProjImplicitLod )
    root = "ImageSparseSampleProjImplicitLod";
  else if( v == SpvOpImageSparseSampleProjExplicitLod )
    root = "ImageSparseSampleProjExplicitLod";
  else if( v == SpvOpImageSparseSampleProjDrefImplicitLod )
    root = "ImageSparseSampleProjDrefImplicitLod";
  else if( v == SpvOpImageSparseSampleProjDrefExplicitLod )
    root = "ImageSparseSampleProjDrefExplicitLod";
  else if( v == SpvOpImageSparseFetch )
    root = "ImageSparseFetch";
  else if( v == SpvOpImageSparseGather )
    root = "ImageSparseGather";
  else if( v == SpvOpImageSparseDrefGather )
    root = "ImageSparseDrefGather";
  else if( v == SpvOpImageSparseTexelsResident )
    root = "ImageSparseTexelsResident";
  else if( v == SpvOpNoLine )
    root = "NoLine";
  else if( v == SpvOpAtomicFlagTestAndSet )
    root = "AtomicFlagTestAndSet";
  else if( v == SpvOpAtomicFlagClear )
    root = "AtomicFlagClear";
  else if( v == SpvOpImageSparseRead )
    root = "ImageSparseRead";
  else if( v == SpvOpSizeOf )
    root = "SizeOf";
  else if( v == SpvOpTypePipeStorage )
    root = "TypePipeStorage";
  else if( v == SpvOpConstantPipeStorage )
    root = "ConstantPipeStorage";
  else if( v == SpvOpCreatePipeFromPipeStorage )
    root = "CreatePipeFromPipeStorage";
  else if( v == SpvOpGetKernelLocalSizeForSubgroupCount )
    root = "GetKernelLocalSizeForSubgroupCount";
  else if( v == SpvOpGetKernelMaxNumSubgroups )
    root = "GetKernelMaxNumSubgroups";
  else if( v == SpvOpTypeNamedBarrier )
    root = "TypeNamedBarrier";
  else if( v == SpvOpNamedBarrierInitialize )
    root = "NamedBarrierInitialize";
  else if( v == SpvOpMemoryNamedBarrier )
    root = "MemoryNamedBarrier";
  else if( v == SpvOpModuleProcessed )
    root = "ModuleProcessed";
  else if( v == SpvOpExecutionModeId )
    root = "ExecutionModeId";
  else if( v == SpvOpDecorateId )
    root = "DecorateId";
  else if( v == SpvOpGroupNonUniformElect )
    root = "GroupNonUniformElect";
  else if( v == SpvOpGroupNonUniformAll )
    root = "GroupNonUniformAll";
  else if( v == SpvOpGroupNonUniformAny )
    root = "GroupNonUniformAny";
  else if( v == SpvOpGroupNonUniformAllEqual )
    root = "GroupNonUniformAllEqual";
  else if( v == SpvOpGroupNonUniformBroadcast )
    root = "GroupNonUniformBroadcast";
  else if( v == SpvOpGroupNonUniformBroadcastFirst )
    root = "GroupNonUniformBroadcastFirst";
  else if( v == SpvOpGroupNonUniformBallot )
    root = "GroupNonUniformBallot";
  else if( v == SpvOpGroupNonUniformInverseBallot )
    root = "GroupNonUniformInverseBallot";
  else if( v == SpvOpGroupNonUniformBallotBitExtract )
    root = "GroupNonUniformBallotBitExtract";
  else if( v == SpvOpGroupNonUniformBallotBitCount )
    root = "GroupNonUniformBallotBitCount";
  else if( v == SpvOpGroupNonUniformBallotFindLSB )
    root = "GroupNonUniformBallotFindLSB";
  else if( v == SpvOpGroupNonUniformBallotFindMSB )
    root = "GroupNonUniformBallotFindMSB";
  else if( v == SpvOpGroupNonUniformShuffle )
    root = "GroupNonUniformShuffle";
  else if( v == SpvOpGroupNonUniformShuffleXor )
    root = "GroupNonUniformShuffleXor";
  else if( v == SpvOpGroupNonUniformShuffleUp )
    root = "GroupNonUniformShuffleUp";
  else if( v == SpvOpGroupNonUniformShuffleDown )
    root = "GroupNonUniformShuffleDown";
  else if( v == SpvOpGroupNonUniformIAdd )
    root = "GroupNonUniformIAdd";
  else if( v == SpvOpGroupNonUniformFAdd )
    root = "GroupNonUniformFAdd";
  else if( v == SpvOpGroupNonUniformIMul )
    root = "GroupNonUniformIMul";
  else if( v == SpvOpGroupNonUniformFMul )
    root = "GroupNonUniformFMul";
  else if( v == SpvOpGroupNonUniformSMin )
    root = "GroupNonUniformSMin";
  else if( v == SpvOpGroupNonUniformUMin )
    root = "GroupNonUniformUMin";
  else if( v == SpvOpGroupNonUniformFMin )
    root = "GroupNonUniformFMin";
  else if( v == SpvOpGroupNonUniformSMax )
    root = "GroupNonUniformSMax";
  else if( v == SpvOpGroupNonUniformUMax )
    root = "GroupNonUniformUMax";
  else if( v == SpvOpGroupNonUniformFMax )
    root = "GroupNonUniformFMax";
  else if( v == SpvOpGroupNonUniformBitwiseAnd )
    root = "GroupNonUniformBitwiseAnd";
  else if( v == SpvOpGroupNonUniformBitwiseOr )
    root = "GroupNonUniformBitwiseOr";
  else if( v == SpvOpGroupNonUniformBitwiseXor )
    root = "GroupNonUniformBitwiseXor";
  else if( v == SpvOpGroupNonUniformLogicalAnd )
    root = "GroupNonUniformLogicalAnd";
  else if( v == SpvOpGroupNonUniformLogicalOr )
    root = "GroupNonUniformLogicalOr";
  else if( v == SpvOpGroupNonUniformLogicalXor )
    root = "GroupNonUniformLogicalXor";
  else if( v == SpvOpGroupNonUniformQuadBroadcast )
    root = "GroupNonUniformQuadBroadcast";
  else if( v == SpvOpGroupNonUniformQuadSwap )
    root = "GroupNonUniformQuadSwap";
  else if( v == SpvOpCopyLogical )
    root = "CopyLogical";
  else if( v == SpvOpPtrEqual )
    root = "PtrEqual";
  else if( v == SpvOpPtrNotEqual )
    root = "PtrNotEqual";
  else if( v == SpvOpPtrDiff )
    root = "PtrDiff";
  else if( v == SpvOpTerminateInvocation )
    root = "TerminateInvocation";
  else if( v == SpvOpSubgroupBallotKHR )
    root = "SubgroupBallotKHR";
  else if( v == SpvOpSubgroupFirstInvocationKHR )
    root = "SubgroupFirstInvocationKHR";
  else if( v == SpvOpSubgroupAllKHR )
    root = "SubgroupAllKHR";
  else if( v == SpvOpSubgroupAnyKHR )
    root = "SubgroupAnyKHR";
  else if( v == SpvOpSubgroupAllEqualKHR )
    root = "SubgroupAllEqualKHR";
  else if( v == SpvOpSubgroupReadInvocationKHR )
    root = "SubgroupReadInvocationKHR";
  else if( v == SpvOpTraceRayKHR )
    root = "TraceRayKHR";
  else if( v == SpvOpExecuteCallableKHR )
    root = "ExecuteCallableKHR";
  else if( v == SpvOpConvertUToAccelerationStructureKHR )
    root = "ConvertUToAccelerationStructureKHR";
  else if( v == SpvOpIgnoreIntersectionKHR )
    root = "IgnoreIntersectionKHR";
  else if( v == SpvOpTerminateRayKHR )
    root = "TerminateRayKHR";
  else if( v == SpvOpTypeRayQueryKHR )
    root = "TypeRayQueryKHR";
  else if( v == SpvOpRayQueryInitializeKHR )
    root = "RayQueryInitializeKHR";
  else if( v == SpvOpRayQueryTerminateKHR )
    root = "RayQueryTerminateKHR";
  else if( v == SpvOpRayQueryGenerateIntersectionKHR )
    root = "RayQueryGenerateIntersectionKHR";
  else if( v == SpvOpRayQueryConfirmIntersectionKHR )
    root = "RayQueryConfirmIntersectionKHR";
  else if( v == SpvOpRayQueryProceedKHR )
    root = "RayQueryProceedKHR";
  else if( v == SpvOpRayQueryGetIntersectionTypeKHR )
    root = "RayQueryGetIntersectionTypeKHR";
  else if( v == SpvOpGroupIAddNonUniformAMD )
    root = "GroupIAddNonUniformAMD";
  else if( v == SpvOpGroupFAddNonUniformAMD )
    root = "GroupFAddNonUniformAMD";
  else if( v == SpvOpGroupFMinNonUniformAMD )
    root = "GroupFMinNonUniformAMD";
  else if( v == SpvOpGroupUMinNonUniformAMD )
    root = "GroupUMinNonUniformAMD";
  else if( v == SpvOpGroupSMinNonUniformAMD )
    root = "GroupSMinNonUniformAMD";
  else if( v == SpvOpGroupFMaxNonUniformAMD )
    root = "GroupFMaxNonUniformAMD";
  else if( v == SpvOpGroupUMaxNonUniformAMD )
    root = "GroupUMaxNonUniformAMD";
  else if( v == SpvOpGroupSMaxNonUniformAMD )
    root = "GroupSMaxNonUniformAMD";
  else if( v == SpvOpFragmentMaskFetchAMD )
    root = "FragmentMaskFetchAMD";
  else if( v == SpvOpFragmentFetchAMD )
    root = "FragmentFetchAMD";
  else if( v == SpvOpReadClockKHR )
    root = "ReadClockKHR";
  else if( v == SpvOpImageSampleFootprintNV )
    root = "ImageSampleFootprintNV";
  else if( v == SpvOpGroupNonUniformPartitionNV )
    root = "GroupNonUniformPartitionNV";
  else if( v == SpvOpWritePackedPrimitiveIndices4x8NV )
    root = "WritePackedPrimitiveIndices4x8NV";
  else if( v == SpvOpReportIntersectionKHR )
    root = "ReportIntersectionKHR";
  else if( v == SpvOpReportIntersectionNV )
    root = "ReportIntersectionNV";
  else if( v == SpvOpIgnoreIntersectionNV )
    root = "IgnoreIntersectionNV";
  else if( v == SpvOpTerminateRayNV )
    root = "TerminateRayNV";
  else if( v == SpvOpTraceNV )
    root = "TraceNV";
  else if( v == SpvOpTypeAccelerationStructureKHR )
    root = "TypeAccelerationStructureKHR";
  else if( v == SpvOpTypeAccelerationStructureNV )
    root = "TypeAccelerationStructureNV";
  else if( v == SpvOpExecuteCallableNV )
    root = "ExecuteCallableNV";
  else if( v == SpvOpTypeCooperativeMatrixNV )
    root = "TypeCooperativeMatrixNV";
  else if( v == SpvOpCooperativeMatrixLoadNV )
    root = "CooperativeMatrixLoadNV";
  else if( v == SpvOpCooperativeMatrixStoreNV )
    root = "CooperativeMatrixStoreNV";
  else if( v == SpvOpCooperativeMatrixMulAddNV )
    root = "CooperativeMatrixMulAddNV";
  else if( v == SpvOpCooperativeMatrixLengthNV )
    root = "CooperativeMatrixLengthNV";
  else if( v == SpvOpBeginInvocationInterlockEXT )
    root = "BeginInvocationInterlockEXT";
  else if( v == SpvOpEndInvocationInterlockEXT )
    root = "EndInvocationInterlockEXT";
  else if( v == SpvOpDemoteToHelperInvocationEXT )
    root = "DemoteToHelperInvocationEXT";
  else if( v == SpvOpIsHelperInvocationEXT )
    root = "IsHelperInvocationEXT";
  else if( v == SpvOpSubgroupShuffleINTEL )
    root = "SubgroupShuffleINTEL";
  else if( v == SpvOpSubgroupShuffleDownINTEL )
    root = "SubgroupShuffleDownINTEL";
  else if( v == SpvOpSubgroupShuffleUpINTEL )
    root = "SubgroupShuffleUpINTEL";
  else if( v == SpvOpSubgroupShuffleXorINTEL )
    root = "SubgroupShuffleXorINTEL";
  else if( v == SpvOpSubgroupBlockReadINTEL )
    root = "SubgroupBlockReadINTEL";
  else if( v == SpvOpSubgroupBlockWriteINTEL )
    root = "SubgroupBlockWriteINTEL";
  else if( v == SpvOpSubgroupImageBlockReadINTEL )
    root = "SubgroupImageBlockReadINTEL";
  else if( v == SpvOpSubgroupImageBlockWriteINTEL )
    root = "SubgroupImageBlockWriteINTEL";
  else if( v == SpvOpSubgroupImageMediaBlockReadINTEL )
    root = "SubgroupImageMediaBlockReadINTEL";
  else if( v == SpvOpSubgroupImageMediaBlockWriteINTEL )
    root = "SubgroupImageMediaBlockWriteINTEL";
  else if( v == SpvOpUCountLeadingZerosINTEL )
    root = "UCountLeadingZerosINTEL";
  else if( v == SpvOpUCountTrailingZerosINTEL )
    root = "UCountTrailingZerosINTEL";
  else if( v == SpvOpAbsISubINTEL )
    root = "AbsISubINTEL";
  else if( v == SpvOpAbsUSubINTEL )
    root = "AbsUSubINTEL";
  else if( v == SpvOpIAddSatINTEL )
    root = "IAddSatINTEL";
  else if( v == SpvOpUAddSatINTEL )
    root = "UAddSatINTEL";
  else if( v == SpvOpIAverageINTEL )
    root = "IAverageINTEL";
  else if( v == SpvOpUAverageINTEL )
    root = "UAverageINTEL";
  else if( v == SpvOpIAverageRoundedINTEL )
    root = "IAverageRoundedINTEL";
  else if( v == SpvOpUAverageRoundedINTEL )
    root = "UAverageRoundedINTEL";
  else if( v == SpvOpISubSatINTEL )
    root = "ISubSatINTEL";
  else if( v == SpvOpUSubSatINTEL )
    root = "USubSatINTEL";
  else if( v == SpvOpIMul32x16INTEL )
    root = "IMul32x16INTEL";
  else if( v == SpvOpUMul32x16INTEL )
    root = "UMul32x16INTEL";
  else if( v == SpvOpFunctionPointerINTEL )
    root = "FunctionPointerINTEL";
  else if( v == SpvOpFunctionPointerCallINTEL )
    root = "FunctionPointerCallINTEL";
  else if( v == SpvOpDecorateString )
    root = "DecorateString";
  else if( v == SpvOpDecorateStringGOOGLE )
    root = "DecorateStringGOOGLE";
  else if( v == SpvOpMemberDecorateString )
    root = "MemberDecorateString";
  else if( v == SpvOpMemberDecorateStringGOOGLE )
    root = "MemberDecorateStringGOOGLE";
  else if( v == SpvOpVmeImageINTEL )
    root = "VmeImageINTEL";
  else if( v == SpvOpTypeVmeImageINTEL )
    root = "TypeVmeImageINTEL";
  else if( v == SpvOpTypeAvcImePayloadINTEL )
    root = "TypeAvcImePayloadINTEL";
  else if( v == SpvOpTypeAvcRefPayloadINTEL )
    root = "TypeAvcRefPayloadINTEL";
  else if( v == SpvOpTypeAvcSicPayloadINTEL )
    root = "TypeAvcSicPayloadINTEL";
  else if( v == SpvOpTypeAvcMcePayloadINTEL )
    root = "TypeAvcMcePayloadINTEL";
  else if( v == SpvOpTypeAvcMceResultINTEL )
    root = "TypeAvcMceResultINTEL";
  else if( v == SpvOpTypeAvcImeResultINTEL )
    root = "TypeAvcImeResultINTEL";
  else if( v == SpvOpTypeAvcImeResultSingleReferenceStreamoutINTEL )
    root = "TypeAvcImeResultSingleReferenceStreamoutINTEL";
  else if( v == SpvOpTypeAvcImeResultDualReferenceStreamoutINTEL )
    root = "TypeAvcImeResultDualReferenceStreamoutINTEL";
  else if( v == SpvOpTypeAvcImeSingleReferenceStreaminINTEL )
    root = "TypeAvcImeSingleReferenceStreaminINTEL";
  else if( v == SpvOpTypeAvcImeDualReferenceStreaminINTEL )
    root = "TypeAvcImeDualReferenceStreaminINTEL";
  else if( v == SpvOpTypeAvcRefResultINTEL )
    root = "TypeAvcRefResultINTEL";
  else if( v == SpvOpTypeAvcSicResultINTEL )
    root = "TypeAvcSicResultINTEL";
  else if( v == SpvOpSubgroupAvcMceGetDefaultInterBaseMultiReferencePenaltyINTEL )
    root = "SubgroupAvcMceGetDefaultInterBaseMultiReferencePenaltyINTEL";
  else if( v == SpvOpSubgroupAvcMceSetInterBaseMultiReferencePenaltyINTEL )
    root = "SubgroupAvcMceSetInterBaseMultiReferencePenaltyINTEL";
  else if( v == SpvOpSubgroupAvcMceGetDefaultInterShapePenaltyINTEL )
    root = "SubgroupAvcMceGetDefaultInterShapePenaltyINTEL";
  else if( v == SpvOpSubgroupAvcMceSetInterShapePenaltyINTEL )
    root = "SubgroupAvcMceSetInterShapePenaltyINTEL";
  else if( v == SpvOpSubgroupAvcMceGetDefaultInterDirectionPenaltyINTEL )
    root = "SubgroupAvcMceGetDefaultInterDirectionPenaltyINTEL";
  else if( v == SpvOpSubgroupAvcMceSetInterDirectionPenaltyINTEL )
    root = "SubgroupAvcMceSetInterDirectionPenaltyINTEL";
  else if( v == SpvOpSubgroupAvcMceGetDefaultIntraLumaShapePenaltyINTEL )
    root = "SubgroupAvcMceGetDefaultIntraLumaShapePenaltyINTEL";
  else if( v == SpvOpSubgroupAvcMceGetDefaultInterMotionVectorCostTableINTEL )
    root = "SubgroupAvcMceGetDefaultInterMotionVectorCostTableINTEL";
  else if( v == SpvOpSubgroupAvcMceGetDefaultHighPenaltyCostTableINTEL )
    root = "SubgroupAvcMceGetDefaultHighPenaltyCostTableINTEL";
  else if( v == SpvOpSubgroupAvcMceGetDefaultMediumPenaltyCostTableINTEL )
    root = "SubgroupAvcMceGetDefaultMediumPenaltyCostTableINTEL";
  else if( v == SpvOpSubgroupAvcMceGetDefaultLowPenaltyCostTableINTEL )
    root = "SubgroupAvcMceGetDefaultLowPenaltyCostTableINTEL";
  else if( v == SpvOpSubgroupAvcMceSetMotionVectorCostFunctionINTEL )
    root = "SubgroupAvcMceSetMotionVectorCostFunctionINTEL";
  else if( v == SpvOpSubgroupAvcMceGetDefaultIntraLumaModePenaltyINTEL )
    root = "SubgroupAvcMceGetDefaultIntraLumaModePenaltyINTEL";
  else if( v == SpvOpSubgroupAvcMceGetDefaultNonDcLumaIntraPenaltyINTEL )
    root = "SubgroupAvcMceGetDefaultNonDcLumaIntraPenaltyINTEL";
  else if( v == SpvOpSubgroupAvcMceGetDefaultIntraChromaModeBasePenaltyINTEL )
    root = "SubgroupAvcMceGetDefaultIntraChromaModeBasePenaltyINTEL";
  else if( v == SpvOpSubgroupAvcMceSetAcOnlyHaarINTEL )
    root = "SubgroupAvcMceSetAcOnlyHaarINTEL";
  else if( v == SpvOpSubgroupAvcMceSetSourceInterlacedFieldPolarityINTEL )
    root = "SubgroupAvcMceSetSourceInterlacedFieldPolarityINTEL";
  else if( v == SpvOpSubgroupAvcMceSetSingleReferenceInterlacedFieldPolarityINTEL )
    root = "SubgroupAvcMceSetSingleReferenceInterlacedFieldPolarityINTEL";
  else if( v == SpvOpSubgroupAvcMceSetDualReferenceInterlacedFieldPolaritiesINTEL )
    root = "SubgroupAvcMceSetDualReferenceInterlacedFieldPolaritiesINTEL";
  else if( v == SpvOpSubgroupAvcMceConvertToImePayloadINTEL )
    root = "SubgroupAvcMceConvertToImePayloadINTEL";
  else if( v == SpvOpSubgroupAvcMceConvertToImeResultINTEL )
    root = "SubgroupAvcMceConvertToImeResultINTEL";
  else if( v == SpvOpSubgroupAvcMceConvertToRefPayloadINTEL )
    root = "SubgroupAvcMceConvertToRefPayloadINTEL";
  else if( v == SpvOpSubgroupAvcMceConvertToRefResultINTEL )
    root = "SubgroupAvcMceConvertToRefResultINTEL";
  else if( v == SpvOpSubgroupAvcMceConvertToSicPayloadINTEL )
    root = "SubgroupAvcMceConvertToSicPayloadINTEL";
  else if( v == SpvOpSubgroupAvcMceConvertToSicResultINTEL )
    root = "SubgroupAvcMceConvertToSicResultINTEL";
  else if( v == SpvOpSubgroupAvcMceGetMotionVectorsINTEL )
    root = "SubgroupAvcMceGetMotionVectorsINTEL";
  else if( v == SpvOpSubgroupAvcMceGetInterDistortionsINTEL )
    root = "SubgroupAvcMceGetInterDistortionsINTEL";
  else if( v == SpvOpSubgroupAvcMceGetBestInterDistortionsINTEL )
    root = "SubgroupAvcMceGetBestInterDistortionsINTEL";
  else if( v == SpvOpSubgroupAvcMceGetInterMajorShapeINTEL )
    root = "SubgroupAvcMceGetInterMajorShapeINTEL";
  else if( v == SpvOpSubgroupAvcMceGetInterMinorShapeINTEL )
    root = "SubgroupAvcMceGetInterMinorShapeINTEL";
  else if( v == SpvOpSubgroupAvcMceGetInterDirectionsINTEL )
    root = "SubgroupAvcMceGetInterDirectionsINTEL";
  else if( v == SpvOpSubgroupAvcMceGetInterMotionVectorCountINTEL )
    root = "SubgroupAvcMceGetInterMotionVectorCountINTEL";
  else if( v == SpvOpSubgroupAvcMceGetInterReferenceIdsINTEL )
    root = "SubgroupAvcMceGetInterReferenceIdsINTEL";
  else if( v == SpvOpSubgroupAvcMceGetInterReferenceInterlacedFieldPolaritiesINTEL )
    root = "SubgroupAvcMceGetInterReferenceInterlacedFieldPolaritiesINTEL";
  else if( v == SpvOpSubgroupAvcImeInitializeINTEL )
    root = "SubgroupAvcImeInitializeINTEL";
  else if( v == SpvOpSubgroupAvcImeSetSingleReferenceINTEL )
    root = "SubgroupAvcImeSetSingleReferenceINTEL";
  else if( v == SpvOpSubgroupAvcImeSetDualReferenceINTEL )
    root = "SubgroupAvcImeSetDualReferenceINTEL";
  else if( v == SpvOpSubgroupAvcImeRefWindowSizeINTEL )
    root = "SubgroupAvcImeRefWindowSizeINTEL";
  else if( v == SpvOpSubgroupAvcImeAdjustRefOffsetINTEL )
    root = "SubgroupAvcImeAdjustRefOffsetINTEL";
  else if( v == SpvOpSubgroupAvcImeConvertToMcePayloadINTEL )
    root = "SubgroupAvcImeConvertToMcePayloadINTEL";
  else if( v == SpvOpSubgroupAvcImeSetMaxMotionVectorCountINTEL )
    root = "SubgroupAvcImeSetMaxMotionVectorCountINTEL";
  else if( v == SpvOpSubgroupAvcImeSetUnidirectionalMixDisableINTEL )
    root = "SubgroupAvcImeSetUnidirectionalMixDisableINTEL";
  else if( v == SpvOpSubgroupAvcImeSetEarlySearchTerminationThresholdINTEL )
    root = "SubgroupAvcImeSetEarlySearchTerminationThresholdINTEL";
  else if( v == SpvOpSubgroupAvcImeSetWeightedSadINTEL )
    root = "SubgroupAvcImeSetWeightedSadINTEL";
  else if( v == SpvOpSubgroupAvcImeEvaluateWithSingleReferenceINTEL )
    root = "SubgroupAvcImeEvaluateWithSingleReferenceINTEL";
  else if( v == SpvOpSubgroupAvcImeEvaluateWithDualReferenceINTEL )
    root = "SubgroupAvcImeEvaluateWithDualReferenceINTEL";
  else if( v == SpvOpSubgroupAvcImeEvaluateWithSingleReferenceStreaminINTEL )
    root = "SubgroupAvcImeEvaluateWithSingleReferenceStreaminINTEL";
  else if( v == SpvOpSubgroupAvcImeEvaluateWithDualReferenceStreaminINTEL )
    root = "SubgroupAvcImeEvaluateWithDualReferenceStreaminINTEL";
  else if( v == SpvOpSubgroupAvcImeEvaluateWithSingleReferenceStreamoutINTEL )
    root = "SubgroupAvcImeEvaluateWithSingleReferenceStreamoutINTEL";
  else if( v == SpvOpSubgroupAvcImeEvaluateWithDualReferenceStreamoutINTEL )
    root = "SubgroupAvcImeEvaluateWithDualReferenceStreamoutINTEL";
  else if( v == SpvOpSubgroupAvcImeEvaluateWithSingleReferenceStreaminoutINTEL )
    root = "SubgroupAvcImeEvaluateWithSingleReferenceStreaminoutINTEL";
  else if( v == SpvOpSubgroupAvcImeEvaluateWithDualReferenceStreaminoutINTEL )
    root = "SubgroupAvcImeEvaluateWithDualReferenceStreaminoutINTEL";
  else if( v == SpvOpSubgroupAvcImeConvertToMceResultINTEL )
    root = "SubgroupAvcImeConvertToMceResultINTEL";
  else if( v == SpvOpSubgroupAvcImeGetSingleReferenceStreaminINTEL )
    root = "SubgroupAvcImeGetSingleReferenceStreaminINTEL";
  else if( v == SpvOpSubgroupAvcImeGetDualReferenceStreaminINTEL )
    root = "SubgroupAvcImeGetDualReferenceStreaminINTEL";
  else if( v == SpvOpSubgroupAvcImeStripSingleReferenceStreamoutINTEL )
    root = "SubgroupAvcImeStripSingleReferenceStreamoutINTEL";
  else if( v == SpvOpSubgroupAvcImeStripDualReferenceStreamoutINTEL )
    root = "SubgroupAvcImeStripDualReferenceStreamoutINTEL";
  else if( v == SpvOpSubgroupAvcImeGetStreamoutSingleReferenceMajorShapeMotionVectorsINTEL )
    root = "SubgroupAvcImeGetStreamoutSingleReferenceMajorShapeMotionVectorsINTEL";
  else if( v == SpvOpSubgroupAvcImeGetStreamoutSingleReferenceMajorShapeDistortionsINTEL )
    root = "SubgroupAvcImeGetStreamoutSingleReferenceMajorShapeDistortionsINTEL";
  else if( v == SpvOpSubgroupAvcImeGetStreamoutSingleReferenceMajorShapeReferenceIdsINTEL )
    root = "SubgroupAvcImeGetStreamoutSingleReferenceMajorShapeReferenceIdsINTEL";
  else if( v == SpvOpSubgroupAvcImeGetStreamoutDualReferenceMajorShapeMotionVectorsINTEL )
    root = "SubgroupAvcImeGetStreamoutDualReferenceMajorShapeMotionVectorsINTEL";
  else if( v == SpvOpSubgroupAvcImeGetStreamoutDualReferenceMajorShapeDistortionsINTEL )
    root = "SubgroupAvcImeGetStreamoutDualReferenceMajorShapeDistortionsINTEL";
  else if( v == SpvOpSubgroupAvcImeGetStreamoutDualReferenceMajorShapeReferenceIdsINTEL )
    root = "SubgroupAvcImeGetStreamoutDualReferenceMajorShapeReferenceIdsINTEL";
  else if( v == SpvOpSubgroupAvcImeGetBorderReachedINTEL )
    root = "SubgroupAvcImeGetBorderReachedINTEL";
  else if( v == SpvOpSubgroupAvcImeGetTruncatedSearchIndicationINTEL )
    root = "SubgroupAvcImeGetTruncatedSearchIndicationINTEL";
  else if( v == SpvOpSubgroupAvcImeGetUnidirectionalEarlySearchTerminationINTEL )
    root = "SubgroupAvcImeGetUnidirectionalEarlySearchTerminationINTEL";
  else if( v == SpvOpSubgroupAvcImeGetWeightingPatternMinimumMotionVectorINTEL )
    root = "SubgroupAvcImeGetWeightingPatternMinimumMotionVectorINTEL";
  else if( v == SpvOpSubgroupAvcImeGetWeightingPatternMinimumDistortionINTEL )
    root = "SubgroupAvcImeGetWeightingPatternMinimumDistortionINTEL";
  else if( v == SpvOpSubgroupAvcFmeInitializeINTEL )
    root = "SubgroupAvcFmeInitializeINTEL";
  else if( v == SpvOpSubgroupAvcBmeInitializeINTEL )
    root = "SubgroupAvcBmeInitializeINTEL";
  else if( v == SpvOpSubgroupAvcRefConvertToMcePayloadINTEL )
    root = "SubgroupAvcRefConvertToMcePayloadINTEL";
  else if( v == SpvOpSubgroupAvcRefSetBidirectionalMixDisableINTEL )
    root = "SubgroupAvcRefSetBidirectionalMixDisableINTEL";
  else if( v == SpvOpSubgroupAvcRefSetBilinearFilterEnableINTEL )
    root = "SubgroupAvcRefSetBilinearFilterEnableINTEL";
  else if( v == SpvOpSubgroupAvcRefEvaluateWithSingleReferenceINTEL )
    root = "SubgroupAvcRefEvaluateWithSingleReferenceINTEL";
  else if( v == SpvOpSubgroupAvcRefEvaluateWithDualReferenceINTEL )
    root = "SubgroupAvcRefEvaluateWithDualReferenceINTEL";
  else if( v == SpvOpSubgroupAvcRefEvaluateWithMultiReferenceINTEL )
    root = "SubgroupAvcRefEvaluateWithMultiReferenceINTEL";
  else if( v == SpvOpSubgroupAvcRefEvaluateWithMultiReferenceInterlacedINTEL )
    root = "SubgroupAvcRefEvaluateWithMultiReferenceInterlacedINTEL";
  else if( v == SpvOpSubgroupAvcRefConvertToMceResultINTEL )
    root = "SubgroupAvcRefConvertToMceResultINTEL";
  else if( v == SpvOpSubgroupAvcSicInitializeINTEL )
    root = "SubgroupAvcSicInitializeINTEL";
  else if( v == SpvOpSubgroupAvcSicConfigureSkcINTEL )
    root = "SubgroupAvcSicConfigureSkcINTEL";
  else if( v == SpvOpSubgroupAvcSicConfigureIpeLumaINTEL )
    root = "SubgroupAvcSicConfigureIpeLumaINTEL";
  else if( v == SpvOpSubgroupAvcSicConfigureIpeLumaChromaINTEL )
    root = "SubgroupAvcSicConfigureIpeLumaChromaINTEL";
  else if( v == SpvOpSubgroupAvcSicGetMotionVectorMaskINTEL )
    root = "SubgroupAvcSicGetMotionVectorMaskINTEL";
  else if( v == SpvOpSubgroupAvcSicConvertToMcePayloadINTEL )
    root = "SubgroupAvcSicConvertToMcePayloadINTEL";
  else if( v == SpvOpSubgroupAvcSicSetIntraLumaShapePenaltyINTEL )
    root = "SubgroupAvcSicSetIntraLumaShapePenaltyINTEL";
  else if( v == SpvOpSubgroupAvcSicSetIntraLumaModeCostFunctionINTEL )
    root = "SubgroupAvcSicSetIntraLumaModeCostFunctionINTEL";
  else if( v == SpvOpSubgroupAvcSicSetIntraChromaModeCostFunctionINTEL )
    root = "SubgroupAvcSicSetIntraChromaModeCostFunctionINTEL";
  else if( v == SpvOpSubgroupAvcSicSetBilinearFilterEnableINTEL )
    root = "SubgroupAvcSicSetBilinearFilterEnableINTEL";
  else if( v == SpvOpSubgroupAvcSicSetSkcForwardTransformEnableINTEL )
    root = "SubgroupAvcSicSetSkcForwardTransformEnableINTEL";
  else if( v == SpvOpSubgroupAvcSicSetBlockBasedRawSkipSadINTEL )
    root = "SubgroupAvcSicSetBlockBasedRawSkipSadINTEL";
  else if( v == SpvOpSubgroupAvcSicEvaluateIpeINTEL )
    root = "SubgroupAvcSicEvaluateIpeINTEL";
  else if( v == SpvOpSubgroupAvcSicEvaluateWithSingleReferenceINTEL )
    root = "SubgroupAvcSicEvaluateWithSingleReferenceINTEL";
  else if( v == SpvOpSubgroupAvcSicEvaluateWithDualReferenceINTEL )
    root = "SubgroupAvcSicEvaluateWithDualReferenceINTEL";
  else if( v == SpvOpSubgroupAvcSicEvaluateWithMultiReferenceINTEL )
    root = "SubgroupAvcSicEvaluateWithMultiReferenceINTEL";
  else if( v == SpvOpSubgroupAvcSicEvaluateWithMultiReferenceInterlacedINTEL )
    root = "SubgroupAvcSicEvaluateWithMultiReferenceInterlacedINTEL";
  else if( v == SpvOpSubgroupAvcSicConvertToMceResultINTEL )
    root = "SubgroupAvcSicConvertToMceResultINTEL";
  else if( v == SpvOpSubgroupAvcSicGetIpeLumaShapeINTEL )
    root = "SubgroupAvcSicGetIpeLumaShapeINTEL";
  else if( v == SpvOpSubgroupAvcSicGetBestIpeLumaDistortionINTEL )
    root = "SubgroupAvcSicGetBestIpeLumaDistortionINTEL";
  else if( v == SpvOpSubgroupAvcSicGetBestIpeChromaDistortionINTEL )
    root = "SubgroupAvcSicGetBestIpeChromaDistortionINTEL";
  else if( v == SpvOpSubgroupAvcSicGetPackedIpeLumaModesINTEL )
    root = "SubgroupAvcSicGetPackedIpeLumaModesINTEL";
  else if( v == SpvOpSubgroupAvcSicGetIpeChromaModeINTEL )
    root = "SubgroupAvcSicGetIpeChromaModeINTEL";
  else if( v == SpvOpSubgroupAvcSicGetPackedSkcLumaCountThresholdINTEL )
    root = "SubgroupAvcSicGetPackedSkcLumaCountThresholdINTEL";
  else if( v == SpvOpSubgroupAvcSicGetPackedSkcLumaSumThresholdINTEL )
    root = "SubgroupAvcSicGetPackedSkcLumaSumThresholdINTEL";
  else if( v == SpvOpSubgroupAvcSicGetInterRawSadsINTEL )
    root = "SubgroupAvcSicGetInterRawSadsINTEL";
  else if( v == SpvOpLoopControlINTEL )
    root = "LoopControlINTEL";
  else if( v == SpvOpReadPipeBlockingINTEL )
    root = "ReadPipeBlockingINTEL";
  else if( v == SpvOpWritePipeBlockingINTEL )
    root = "WritePipeBlockingINTEL";
  else if( v == SpvOpFPGARegINTEL )
    root = "FPGARegINTEL";
  else if( v == SpvOpRayQueryGetRayTMinKHR )
    root = "RayQueryGetRayTMinKHR";
  else if( v == SpvOpRayQueryGetRayFlagsKHR )
    root = "RayQueryGetRayFlagsKHR";
  else if( v == SpvOpRayQueryGetIntersectionTKHR )
    root = "RayQueryGetIntersectionTKHR";
  else if( v == SpvOpRayQueryGetIntersectionInstanceCustomIndexKHR )
    root = "RayQueryGetIntersectionInstanceCustomIndexKHR";
  else if( v == SpvOpRayQueryGetIntersectionInstanceIdKHR )
    root = "RayQueryGetIntersectionInstanceIdKHR";
  else if( v == SpvOpRayQueryGetIntersectionInstanceShaderBindingTableRecordOffsetKHR )
    root = "RayQueryGetIntersectionInstanceShaderBindingTableRecordOffsetKHR";
  else if( v == SpvOpRayQueryGetIntersectionGeometryIndexKHR )
    root = "RayQueryGetIntersectionGeometryIndexKHR";
  else if( v == SpvOpRayQueryGetIntersectionPrimitiveIndexKHR )
    root = "RayQueryGetIntersectionPrimitiveIndexKHR";
  else if( v == SpvOpRayQueryGetIntersectionBarycentricsKHR )
    root = "RayQueryGetIntersectionBarycentricsKHR";
  else if( v == SpvOpRayQueryGetIntersectionFrontFaceKHR )
    root = "RayQueryGetIntersectionFrontFaceKHR";
  else if( v == SpvOpRayQueryGetIntersectionCandidateAABBOpaqueKHR )
    root = "RayQueryGetIntersectionCandidateAABBOpaqueKHR";
  else if( v == SpvOpRayQueryGetIntersectionObjectRayDirectionKHR )
    root = "RayQueryGetIntersectionObjectRayDirectionKHR";
  else if( v == SpvOpRayQueryGetIntersectionObjectRayOriginKHR )
    root = "RayQueryGetIntersectionObjectRayOriginKHR";
  else if( v == SpvOpRayQueryGetWorldRayDirectionKHR )
    root = "RayQueryGetWorldRayDirectionKHR";
  else if( v == SpvOpRayQueryGetWorldRayOriginKHR )
    root = "RayQueryGetWorldRayOriginKHR";
  else if( v == SpvOpRayQueryGetIntersectionObjectToWorldKHR )
    root = "RayQueryGetIntersectionObjectToWorldKHR";
  else if( v == SpvOpRayQueryGetIntersectionWorldToObjectKHR )
    root = "RayQueryGetIntersectionWorldToObjectKHR";
  else if( v == SpvOpAtomicFAddEXT )
    root = "AtomicFAddEXT";
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

