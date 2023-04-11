#ifndef GCT_WRITE_DESCRIPTOR_SET_HPP
#define GCT_WRITE_DESCRIPTOR_SET_HPP
#include <memory>
#include <optional>
#include <vulkan/vulkan.hpp>
#include <gct/extension.hpp>
#include <gct/descriptor_image_info.hpp>
#include <gct/descriptor_buffer_info.hpp>

namespace gct {
  class buffer_t;
  class image_view_t;
  class sampler_t;
  class acceleration_structure_t;
  class write_descriptor_set_t : public chained_t {
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
    std::vector< descriptor_image_info_t > image;
    std::vector< vk::DescriptorImageInfo > raw_image;
    std::vector< descriptor_buffer_info_t > buffer;
    std::vector< vk::DescriptorBufferInfo > raw_buffer;
#ifdef VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME
    std::vector< std::shared_ptr< gct::acceleration_structure_t > > acceleration_structure_handles;
    std::vector< vk::AccelerationStructureKHR > raw_acceleration_structure;
#endif
  public:
    write_descriptor_set_t &add_image( const descriptor_image_info_t& );
    write_descriptor_set_t &add_image( const std::shared_ptr< image_view_t > &v );
    write_descriptor_set_t &add_image(
      const std::shared_ptr< sampler_t > &sampler,
      const std::shared_ptr< image_view_t > &image_view
    );
    write_descriptor_set_t &clear_image();
    write_descriptor_set_t &add_buffer( const descriptor_buffer_info_t& );
    write_descriptor_set_t &add_buffer( const std::shared_ptr< buffer_t > &v );
    write_descriptor_set_t &clear_buffer();
#ifdef VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME
    write_descriptor_set_t &add_acceleration_structure_handle( const std::shared_ptr< gct::acceleration_structure_t >& );
    write_descriptor_set_t &clear_acceleration_structure_handle();
#endif
  };
}

#endif

