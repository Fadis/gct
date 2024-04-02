#ifndef GCT_WRITE_DESCRIPTOR_SET_HPP
#define GCT_WRITE_DESCRIPTOR_SET_HPP
#include <memory>
#include <vector>
#include <nlohmann/json_fwd.hpp>
#include <vulkan/vulkan.hpp>
#include <gct/extension.hpp>
#include <gct/descriptor_image_info.hpp>
#include <gct/descriptor_buffer_info.hpp>
#include <gct/array_of.hpp>
#include <vulkan/vulkan_structs.hpp>

namespace gct {
  class buffer_t;
  class mappable_buffer_t;
  class image_view_t;
  class sampler_t;
  class acceleration_structure_t;
  class named_resource;
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
#ifdef VK_VERSION_1_3
    LIBGCT_EXTENSION_SETTER( vk::WriteDescriptorSetInlineUniformBlock, inline_uniform_block )
#elif defined(VK_EXT_INLINE_UNIFORM_BLOCK_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::WriteDescriptorSetInlineUniformBlockEXT, inline_uniform_block )
#endif
    LIBGCT_ARRAY_OF_WRAPPED( descriptor_image_info_t, vk::DescriptorImageInfo, image ) 
    LIBGCT_ARRAY_OF_WRAPPED( descriptor_buffer_info_t, vk::DescriptorBufferInfo, buffer ) 
  private:
#ifdef VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME
    std::vector< std::shared_ptr< gct::acceleration_structure_t > > acceleration_structure_handles;
    std::vector< vk::AccelerationStructureKHR > raw_acceleration_structure;
    std::string name;
    std::uint32_t index = 0;
#endif
  public:
    write_descriptor_set_t() {}
    write_descriptor_set_t(
      const std::string &,
      const std::shared_ptr< image_view_t >&
    );
    write_descriptor_set_t(
      const std::string &,
      const std::shared_ptr< sampler_t > &sampler,
      const std::shared_ptr< image_view_t >&
    );
    write_descriptor_set_t(
      const std::string &,
      const std::shared_ptr< image_view_t >&,
      vk::ImageLayout layout
    );
    write_descriptor_set_t(
      const std::string &,
      const std::shared_ptr< sampler_t > &sampler,
      const std::shared_ptr< image_view_t >&,
      vk::ImageLayout layout
    );
    write_descriptor_set_t(
      const std::string &,
      const std::shared_ptr< buffer_t >&
    );
    write_descriptor_set_t(
      const std::string &,
      const std::shared_ptr< mappable_buffer_t >&
    );
    write_descriptor_set_t(
      const std::string &,
      std::uint32_t,
      const std::shared_ptr< image_view_t >&
    );
    write_descriptor_set_t(
      const std::string &,
      std::uint32_t,
      const std::shared_ptr< sampler_t > &sampler,
      const std::shared_ptr< image_view_t >&
    );
    write_descriptor_set_t(
      const std::string &,
      std::uint32_t,
      const std::shared_ptr< image_view_t >&,
      vk::ImageLayout layout
    );
    write_descriptor_set_t(
      const std::string &,
      std::uint32_t,
      const std::shared_ptr< sampler_t > &sampler,
      const std::shared_ptr< image_view_t >&,
      vk::ImageLayout layout
    );
    write_descriptor_set_t(
      const std::string &,
      std::uint32_t,
      const std::shared_ptr< buffer_t >&
    );
    write_descriptor_set_t(
      const std::string &,
      std::uint32_t,
      const std::shared_ptr< mappable_buffer_t >&
    );
    write_descriptor_set_t(
      const named_resource &r,
      unsigned int index = 0u
    );
    write_descriptor_set_t &add_image( const std::shared_ptr< image_view_t > &v );
    write_descriptor_set_t &add_image(
      const std::shared_ptr< sampler_t > &sampler,
      const std::shared_ptr< image_view_t > &image_view
    );
    write_descriptor_set_t &add_image(
      const std::shared_ptr< image_view_t > &image_view,
      vk::ImageLayout layout
    );
    write_descriptor_set_t &add_image(
      const std::shared_ptr< sampler_t > &sampler,
      const std::shared_ptr< image_view_t > &image_view,
      vk::ImageLayout layout
    );
    write_descriptor_set_t &add_sampler(
      const std::shared_ptr< sampler_t > &sampler
    );
    write_descriptor_set_t &add_buffer( const std::shared_ptr< buffer_t > &v );
    write_descriptor_set_t &add_buffer( const std::shared_ptr< mappable_buffer_t > &v );
    write_descriptor_set_t &add(
      const named_resource &r,
      unsigned int index = 0u
    );
#ifdef VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME
    write_descriptor_set_t &add_acceleration_structure_handle( const std::shared_ptr< gct::acceleration_structure_t >& );
    write_descriptor_set_t &clear_acceleration_structure_handle();
#endif
    write_descriptor_set_t &set_name( const std::string &n ) {
      name = n;
      return *this;
    }
    const std::string &get_name() const {
      return name;
    }
    write_descriptor_set_t &set_index( std::uint32_t i ) {
      index = i;
      return *this;
    }
    std::uint32_t get_index() const {
      return index;
    }
  };
void to_json( nlohmann::json&, const write_descriptor_set_t& );
}

#endif

