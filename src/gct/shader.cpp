#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_enums.hpp>
#if defined(VK_VERSION_1_3) || defined(VK_EXT_SHADER_OBJECT_EXTENSION_NAME)
#include <gct/device.hpp>
#include <gct/exception.hpp>
#include <gct/shader.hpp>
#include <gct/shader_module_reflection.hpp>
#include <gct/spv2vk.hpp>
#include <nlohmann/json.hpp>

namespace gct {
  shader_t::shader_t(
    const std::shared_ptr< device_t > &device,
    const shader_create_info_t &create_info
  ) :
    property_type( create_info ),
    created_from< device_t >( device ) {
    props.rebuild_chain();
    auto handle_maybe = (*device)->createShaderEXTUnique( props.get_basic() );
    if( handle_maybe.result != vk::Result::eSuccess ) {
#if VK_HEADER_VERSION >= 256
      vk::detail::throwResultException( vk::Result( handle_maybe.result ), "createShaderExtUnique failed." );
#else
      vk::throwResultException( vk::Result( handle_maybe.result ), "createShaderExtUnique failed." );
#endif
    }
    handle = std::move( handle_maybe.value );
  }
  void to_json( nlohmann::json &root, const shader_t &v ) {
    root = nlohmann::json::object();
    root[ "props" ] = v.get_props();
  }
  vk::ShaderStageFlagBits shader_t::get_stage() const {
    if( !get_props().has_reflection() ) {
      throw exception::runtime_error( "shader_t::get_stage : reflection is not available.", __FILE__, __LINE__ );
    }
    return spv2vk( get_props().get_reflection()->shader_stage );
  }
  bool shader_t::is_compute() const {
    return get_stage() == vk::ShaderStageFlagBits::eCompute;
  }
  bool shader_t::is_graphics() const {
    const auto stage = get_stage();
    if( stage == vk::ShaderStageFlagBits::eVertex ) return true;
    if( stage == vk::ShaderStageFlagBits::eFragment ) return true;
    if( stage == vk::ShaderStageFlagBits::eGeometry ) return true;
 #ifdef VK_EXT_MESH_SHADER_EXTENSION_NAME 
    if( stage == vk::ShaderStageFlagBits::eMeshEXT ) return true;
    if( stage == vk::ShaderStageFlagBits::eTaskEXT ) return true;
 #endif
 #ifdef VK_NV_MESH_SHADER_EXTENSION_NAME
    if( stage == vk::ShaderStageFlagBits::eMeshNV ) return true;
    if( stage == vk::ShaderStageFlagBits::eTaskNV ) return true;
#endif
    if( stage == vk::ShaderStageFlagBits::eTessellationEvaluation ) return true;
    if( stage == vk::ShaderStageFlagBits::eTessellationControl ) return true;
    return false;
  }
  bool shader_t::is_ray_trace() const {
    const auto stage = get_stage();
 #ifdef VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME 
    if( stage == vk::ShaderStageFlagBits::eMissKHR ) return true;
    if( stage == vk::ShaderStageFlagBits::eRaygenKHR ) return true;
    if( stage == vk::ShaderStageFlagBits::eAnyHitKHR ) return true;
    if( stage == vk::ShaderStageFlagBits::eClosestHitKHR ) return true;
#endif
    return false;
  }
}
#endif

