#ifndef GCT_SHADER_CREATE_INFO_HPP
#define GCT_SHADER_CREATE_INFO_HPP
#include <string>
#include <memory>
#include <vector>
#include <nlohmann/json_fwd.hpp>
#include <vulkan/vulkan.hpp>
#include <gct/extension.hpp>
#include <gct/pipeline_layout_create_info.hpp>
#include <gct/array_of.hpp>

#if defined(VK_VERSION_1_3) || defined(VK_EXT_SHADER_OBJECT_EXTENSION_NAME)
namespace gct {
  class shader_module_reflection_t;
  class descriptor_set_layout_t;
  class shader_create_info_t : public chained_t {
  public:
    using self_type = shader_create_info_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
    LIBGCT_BASIC_SETTER( vk::ShaderCreateInfoEXT )
    LIBGCT_ARRAY_OF_WRAPPED_INDIRECT( descriptor_set_layout_t, vk::DescriptorSetLayout, descriptor_set_layout )
    LIBGCT_ARRAY_OF( vk::PushConstantRange, push_constant_range )
#ifdef VK_VERSION_1_3
    LIBGCT_EXTENSION_SETTER( vk::PipelineShaderStageRequiredSubgroupSizeCreateInfo , subgroup_size )
#elif defined(VK_EXT_SUBGROUP_SIZE_CONTROL_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::PipelineShaderStageRequiredSubgroupSizeCreateInfoEXT , subgroup_size )
#endif
  private:
    std::vector< std::uint8_t > code;
  public:
    shader_create_info_t &load( const std::string& );
    shader_create_info_t &load_from_memory( const std::vector< std::uint8_t >& );
    const std::vector< std::uint8_t > &get_code() const { return code; }
    pipeline_layout_create_info_t get_pipeline_layout_create_info() const;
  private:
    std::shared_ptr< shader_module_reflection_t > reflect;
  public:
    bool has_reflection() const { return reflect.get(); }
    const shader_module_reflection_t &get_reflection() const;
  };
  void to_json( nlohmann::json &root, const shader_create_info_t &v );
  void from_json( const nlohmann::json &root, shader_create_info_t &v );
}
#endif

#endif


