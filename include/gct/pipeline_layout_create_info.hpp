#ifndef GCT_PIPELINE_LAYOUT_CREATE_INFO_HPP
#define GCT_PIPELINE_LAYOUT_CREATE_INFO_HPP
#include <memory>
#include <vector>
#include <nlohmann/json_fwd.hpp>
#include <vulkan/vulkan.hpp>
#include <gct/extension.hpp>

namespace gct {
  class descriptor_set_layout_t;
  class shader_module_t;
  class pipeline_layout_create_info_t : public chained_t {
  public:
    using self_type = pipeline_layout_create_info_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
    LIBGCT_BASIC_SETTER( vk::PipelineLayoutCreateInfo )
  private:
    std::vector< std::shared_ptr< descriptor_set_layout_t > > descriptor_set_layout;
    std::vector< vk::DescriptorSetLayout > raw_descriptor_set_layout;
    std::vector< vk::PushConstantRange > push_constant_range;
  public:
    pipeline_layout_create_info_t &add_descriptor_set_layout( const std::shared_ptr< descriptor_set_layout_t >& );
    pipeline_layout_create_info_t &clear_descriptor_set_layout();
    pipeline_layout_create_info_t &add_push_constant_range( const vk::PushConstantRange& );
    pipeline_layout_create_info_t &add_push_constant_range(
      const std::shared_ptr< shader_module_t > &v
    );
    pipeline_layout_create_info_t &clear_push_constant_range();
    const std::vector< vk::PushConstantRange > &get_push_constant_range() const {
      return push_constant_range;
    }
  };
  void to_json( nlohmann::json &root, const pipeline_layout_create_info_t &v );
  void from_json( const nlohmann::json &root, pipeline_layout_create_info_t &v );
}

#endif

