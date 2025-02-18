#ifndef GCT_PIPELINE_LAYOUT_CREATE_INFO_HPP
#define GCT_PIPELINE_LAYOUT_CREATE_INFO_HPP
#include <memory>
#include <nlohmann/json_fwd.hpp>
#include <vulkan/vulkan.hpp>
#include <gct/extension.hpp>
#include <gct/array_of.hpp>

namespace gct {
  class descriptor_set_layout_t;
  class shader_module_t;
  class shader_module_reflection_t;
  class pipeline_layout_create_info_t : public chained_t {
  public:
    using self_type = pipeline_layout_create_info_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
    LIBGCT_BASIC_SETTER( vk::PipelineLayoutCreateInfo )
    LIBGCT_ARRAY_OF_WRAPPED_INDIRECT( descriptor_set_layout_t, vk::DescriptorSetLayout, descriptor_set_layout )
    LIBGCT_ARRAY_OF( vk::PushConstantRange, push_constant_range )
  public:
    pipeline_layout_create_info_t &add_push_constant_range(
      const std::shared_ptr< shader_module_t > &v
    );
    pipeline_layout_create_info_t &add_push_constant_range(
      const shader_module_reflection_t &v
    );
    pipeline_layout_create_info_t &add_descriptor_set_layout(
      unsigned int id,
      const std::shared_ptr< descriptor_set_layout_t > &v
    );
  };
  void to_json( nlohmann::json &root, const pipeline_layout_create_info_t &v );
  void from_json( const nlohmann::json &root, pipeline_layout_create_info_t &v );
}

#endif

