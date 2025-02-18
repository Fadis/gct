#ifndef GCT_DESCRIPTOR_POOL_CREATE_INFO_HPP
#define GCT_DESCRIPTOR_POOL_CREATE_INFO_HPP
#include <unordered_map>
#include <vector>
#include <vulkan/vulkan.hpp>
#include <nlohmann/json_fwd.hpp>
#include <gct/extension.hpp>

namespace gct {
  class descriptor_pool_create_info_t : public chained_t {
  public:
    using self_type = descriptor_pool_create_info_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
    LIBGCT_BASIC_SETTER( vk::DescriptorPoolCreateInfo )
#ifdef VK_VERSION_1_3
    LIBGCT_EXTENSION_SETTER( vk::DescriptorPoolInlineUniformBlockCreateInfo , inline_uniform_block )
#elif defined(VK_EXT_INLINE_UNIFORM_BLOCK_EXTENSION_NAME)
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
  void to_json( nlohmann::json &root, const descriptor_pool_create_info_t &v );
  void from_json( const nlohmann::json &root, descriptor_pool_create_info_t &v );
}

#endif

