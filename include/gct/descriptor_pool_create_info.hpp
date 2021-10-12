#ifndef GCT_DESCRIPTOR_POOL_CREATE_INFO_HPP
#define GCT_DESCRIPTOR_POOL_CREATE_INFO_HPP
#include <memory>
#include <unordered_map>
#include <vector>
#include <vulkan/vulkan.hpp>
#include <gct/extension.hpp>

namespace gct {
  class descriptor_pool_create_info_t {
  public:
    using self_type = descriptor_pool_create_info_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
    LIBGCT_BASIC_SETTER( vk::DescriptorPoolCreateInfo )
#ifdef VK_EXT_INLINE_UNIFORM_BLOCK_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::DescriptorPoolInlineUniformBlockCreateInfoEXT , inline_uniform_block )
#endif
#ifdef VK_VALVE_MUTABLE_DESCRIPTOR_TYPE_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::MutableDescriptorTypeCreateInfoVALVE , mutable_descriptor_type )
#endif
  private:
    std::unordered_map< vk::DescriptorType, vk::DescriptorPoolSize > unique_types;
    std::vector< vk::DescriptorPoolSize > sequential;
  public:
    descriptor_pool_create_info_t &set_descriptor_pool_size( const vk::DescriptorPoolSize &v );
    descriptor_pool_create_info_t &set_descriptor_pool_size( vk::DescriptorType type, std::uint32_t count );
    descriptor_pool_create_info_t &clear_descriptor_pool_size();
  };
}

#endif

