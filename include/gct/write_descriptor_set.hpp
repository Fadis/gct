#ifndef GCT_WRITE_DESCRIPTOR_SET_HPP
#define GCT_WRITE_DESCRIPTOR_SET_HPP
#include <memory>
#include <optional>
#include <vulkan/vulkan.hpp>
#include <gct/extension.hpp>
#include <gct/descriptor_image_info.hpp>
#include <gct/descriptor_buffer_info.hpp>
#include <gct/deep_copy.hpp>

namespace gct {
  class write_descriptor_set_t {
  public:
    using self_type = write_descriptor_set_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
    LIBGCT_BASIC_SETTER( vk::WriteDescriptorSet )
#ifdef VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::WriteDescriptorSetAccelerationStructureKHR , acceleration_structure )
#endif
#ifdef VK_NV_RAY_TRACING_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::WriteDescriptorSetAccelerationStructureNV , acceleration_structure_nv )
#endif
#ifdef VK_EXT_INLINE_UNIFORM_BLOCK_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::WriteDescriptorSetInlineUniformBlockEXT, inline_uniform_block )
#endif
  private:
    deep_copy_unique_ptr< descriptor_image_info_t > image;
    deep_copy_unique_ptr< descriptor_buffer_info_t > buffer;
  public:
    write_descriptor_set_t &set_image( const descriptor_image_info_t& );
    write_descriptor_set_t &clear_image();
    write_descriptor_set_t &set_buffer( const descriptor_buffer_info_t& );
    write_descriptor_set_t &clear_buffer();
  };
}

#endif

