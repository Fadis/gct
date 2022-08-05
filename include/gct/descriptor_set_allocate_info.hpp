#ifndef GCT_DESCRIPTOR_SET_ALLOCATE_INFO_HPP
#define GCT_DESCRIPTOR_SET_ALLOCATE_INFO_HPP
#include <memory>
#include <vulkan/vulkan.hpp>
#include <nlohmann/json_fwd.hpp>
#include <gct/extension.hpp>

namespace gct {
  class descriptor_set_layout_t;
  class descriptor_set_allocate_info_t : public chained_t {
  public:
    using self_type = descriptor_set_allocate_info_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
    LIBGCT_BASIC_SETTER( vk::DescriptorSetAllocateInfo )
#ifdef VK_VERSION_1_2
    LIBGCT_EXTENSION_SETTER( vk::DescriptorSetVariableDescriptorCountAllocateInfo , variable_descriptor_count )
#elif defined(VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::DescriptorSetVariableDescriptorCountAllocateInfoEXT , variable_descriptor_count )
#endif
  private:
    std::vector< std::shared_ptr< descriptor_set_layout_t > > layout;
    std::vector< vk::DescriptorSetLayout > raw_layout;
    std::vector< std::uint32_t > counts;
  public:
    descriptor_set_allocate_info_t &add_layout( const std::shared_ptr< descriptor_set_layout_t >& );
    descriptor_set_allocate_info_t &add_layout( const std::shared_ptr< descriptor_set_layout_t >&, std::uint32_t );
    descriptor_set_allocate_info_t &clear_layout();
    const std::vector< std::shared_ptr< descriptor_set_layout_t > > &get_layout() const { return layout; }
  };
  void to_json( nlohmann::json &root, const descriptor_set_allocate_info_t &v );
  void from_json( const nlohmann::json &root, descriptor_set_allocate_info_t &v );
}

#endif

