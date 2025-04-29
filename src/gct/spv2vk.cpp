#include <gct/spv2vk.hpp>
#include <gct/exception.hpp>
namespace gct {
  vk::ShaderStageFlagBits spv2vk( SpvReflectShaderStageFlagBits v ) {
    if( v == SPV_REFLECT_SHADER_STAGE_VERTEX_BIT )
      return vk::ShaderStageFlagBits::eVertex;
    else if( v == SPV_REFLECT_SHADER_STAGE_TESSELLATION_CONTROL_BIT )
      return vk::ShaderStageFlagBits::eTessellationControl;
    else if( v == SPV_REFLECT_SHADER_STAGE_TESSELLATION_EVALUATION_BIT )
      return vk::ShaderStageFlagBits::eTessellationEvaluation;
    else if( v == SPV_REFLECT_SHADER_STAGE_GEOMETRY_BIT )
      return vk::ShaderStageFlagBits::eGeometry;
    else if( v == SPV_REFLECT_SHADER_STAGE_FRAGMENT_BIT )
      return vk::ShaderStageFlagBits::eFragment;
    else if( v == SPV_REFLECT_SHADER_STAGE_COMPUTE_BIT )
      return vk::ShaderStageFlagBits::eCompute;
    else if( v == SPV_REFLECT_SHADER_STAGE_TASK_BIT_NV )
      return vk::ShaderStageFlagBits::eTaskNV;
    else if( v == SPV_REFLECT_SHADER_STAGE_MESH_BIT_NV )
      return vk::ShaderStageFlagBits::eMeshEXT;
    else if( v == SPV_REFLECT_SHADER_STAGE_TASK_BIT_EXT )
      return vk::ShaderStageFlagBits::eTaskEXT;
    else if( v == SPV_REFLECT_SHADER_STAGE_MESH_BIT_EXT )
      return vk::ShaderStageFlagBits::eMeshNV;
    else if( v == SPV_REFLECT_SHADER_STAGE_RAYGEN_BIT_KHR )
      return vk::ShaderStageFlagBits::eRaygenKHR;
    else if( v == SPV_REFLECT_SHADER_STAGE_ANY_HIT_BIT_KHR )
      return vk::ShaderStageFlagBits::eAnyHitKHR;
    else if( v == SPV_REFLECT_SHADER_STAGE_CLOSEST_HIT_BIT_KHR )
      return vk::ShaderStageFlagBits::eClosestHitKHR;
    else if( v == SPV_REFLECT_SHADER_STAGE_MISS_BIT_KHR )
      return vk::ShaderStageFlagBits::eMissKHR;
    else if( v == SPV_REFLECT_SHADER_STAGE_INTERSECTION_BIT_KHR )
      return vk::ShaderStageFlagBits::eIntersectionKHR;
    else if( v == SPV_REFLECT_SHADER_STAGE_CALLABLE_BIT_KHR )
      return vk::ShaderStageFlagBits::eCallableKHR;
    else
      throw exception::invalid_argument( "unknown SpvReflectShaderStageFlagBits value", __FILE__, __LINE__ );
  }
  vk::DescriptorType spv2vk( SpvReflectDescriptorType v ) {
    if( v == SPV_REFLECT_DESCRIPTOR_TYPE_SAMPLER )
      return vk::DescriptorType::eSampler;
    else if( v == SPV_REFLECT_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER )
      return vk::DescriptorType::eCombinedImageSampler;
    else if( v == SPV_REFLECT_DESCRIPTOR_TYPE_SAMPLED_IMAGE )
      return vk::DescriptorType::eSampledImage;
    else if( v == SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_IMAGE )
      return vk::DescriptorType::eStorageImage;
    else if( v == SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER )
      return vk::DescriptorType::eUniformTexelBuffer;
    else if( v == SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER )
      return vk::DescriptorType::eStorageTexelBuffer;
    else if( v == SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_BUFFER )
      return vk::DescriptorType::eUniformBuffer;
    else if( v == SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_BUFFER )
      return vk::DescriptorType::eStorageBuffer;
    else if( v == SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC )
      return vk::DescriptorType::eUniformBufferDynamic;
    else if( v == SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC )
      return vk::DescriptorType::eStorageBufferDynamic;
    else if( v == SPV_REFLECT_DESCRIPTOR_TYPE_INPUT_ATTACHMENT )
      return vk::DescriptorType::eInputAttachment;
    else if( v == SPV_REFLECT_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR )
      return vk::DescriptorType::eAccelerationStructureKHR;
    else
      throw exception::invalid_argument( "unknown SpvReflectDescriptorType value", __FILE__, __LINE__ );
  }
}

