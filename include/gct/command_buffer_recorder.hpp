#ifndef GCT_COMMAND_BUFFER_RECORDER_HPP
#define GCT_COMMAND_BUFFER_RECORDER_HPP
#include <memory>
#include <vector>
#include <any>
#include <boost/range/iterator_range.hpp>
#include <vulkan/vulkan.hpp>
#include <gct/created_from.hpp>
#include <gct/command_buffer_begin_info.hpp>

namespace gct {
  struct command_pool_t;
  class buffer_t;
  class allocator_t;
  class image_t;
  class bound_command_buffer_t;
  class descriptor_set_t;
  class pipeline_layout_t;
  class compute_pipeline_t;
  std::uint32_t get_pot( std::uint32_t v );
  bool is_pot( std::uint32_t v );
  class command_buffer_recorder_t : public created_from< bound_command_buffer_t > {
  public:
    command_buffer_recorder_t(
      const std::shared_ptr< bound_command_buffer_t >&,
      const command_buffer_begin_info_t&
    );
    ~command_buffer_recorder_t();
    command_buffer_recorder_t( const command_buffer_recorder_t& ) = delete;
    command_buffer_recorder_t( command_buffer_recorder_t&& ) = default;
    command_buffer_recorder_t &operator=( const command_buffer_recorder_t& ) = delete;
    command_buffer_recorder_t &operator=( command_buffer_recorder_t&& ) = default;
    std::shared_ptr< buffer_t > load_buffer(
      const std::shared_ptr< allocator_t > &allocator,
      const std::vector< uint8_t > &data,
      vk::BufferUsageFlags usage
    );
    std::shared_ptr< buffer_t > load_buffer_from_file(
      const std::shared_ptr< allocator_t > &allocator,
      const std::string &filename,
      vk::BufferUsageFlags usage
    );
    std::shared_ptr< image_t > load_image(
      const std::shared_ptr< allocator_t > &allocator,
      const std::string &filename,
      vk::ImageUsageFlagBits usage,
      bool mipmap,
      bool srgb
    );
    void buffer_to_image(
      bool mipmap,
      const std::shared_ptr< buffer_t > &temporary,
      const std::shared_ptr< image_t > &destination
    );
    void create_mipmap(
      const std::shared_ptr< image_t > &image,
      vk::ImageLayout from,
      vk::ImageLayout to 
    );
    void copy(
      const boost::iterator_range< const std::uint8_t* >&,
      const std::shared_ptr< buffer_t >&,
      const std::shared_ptr< buffer_t >&
    );
    template< typename T >
    void copy(
      const T &src,
      const std::shared_ptr< buffer_t > &staging,
      const std::shared_ptr< buffer_t > &dest
    ) {
      copy(
        boost::make_iterator_range(
          reinterpret_cast< const std::uint8_t* >( &src ),
          reinterpret_cast< const std::uint8_t* >( &src ) + sizeof( T )
        ),
        staging,
        dest
      );
    }
    void copy(
      const std::shared_ptr< buffer_t >&,
      const std::shared_ptr< buffer_t >&,
      const std::vector< vk::BufferCopy >&
    );
    void copy(
      const std::shared_ptr< buffer_t >&,
      const std::shared_ptr< buffer_t >&,
      const vk::BufferCopy&
    );
    void copy(
      const std::shared_ptr< buffer_t >&,
      const std::shared_ptr< buffer_t >&
    );
    void copy(
      const std::shared_ptr< image_t >&,
      const std::shared_ptr< image_t >&,
      const std::vector< vk::ImageCopy >&
    );
    void copy(
      const std::shared_ptr< image_t >&,
      const std::shared_ptr< image_t >&,
      const vk::ImageCopy&
    );
    void copy(
      const std::shared_ptr< image_t >&,
      const std::shared_ptr< image_t >&
    );
    void copy(
      const std::shared_ptr< buffer_t > &src,
      const std::shared_ptr< image_t > &dest,
      const std::vector< vk::BufferImageCopy > &range,
      vk::ImageLayout final_layout
    );
    void copy(
      const std::shared_ptr< buffer_t > &src,
      const std::shared_ptr< image_t > &dest,
      const vk::BufferImageCopy &range,
      vk::ImageLayout final_layout
    );
    void copy(
      const std::shared_ptr< buffer_t > &src,
      const std::shared_ptr< image_t > &dest,
      vk::ImageLayout final_layout
    );
    void barrier(
      vk::AccessFlagBits,
      vk::AccessFlagBits,
      vk::PipelineStageFlagBits,
      vk::PipelineStageFlagBits,
      vk::DependencyFlagBits,
      const std::vector< std::shared_ptr< buffer_t > >&,
      const std::vector< std::shared_ptr< image_t > >&
    );
    void convert_image(
      const std::shared_ptr< image_t > &image,
      std::uint32_t mip_base,
      std::uint32_t mip_count,
      vk::ImageLayout from,
      vk::ImageLayout to
    );
    void convert_image(
      const std::shared_ptr< image_t > &image,
      vk::ImageLayout from,
      vk::ImageLayout to
    );
    void convert_image(
      vk::AccessFlagBits src_access_mask,
      vk::AccessFlagBits dest_access_mask,
      vk::PipelineStageFlagBits src_stage,
      vk::PipelineStageFlagBits dest_stage,
      const std::shared_ptr< image_t > &image,
      uint32_t mip_base,
      uint32_t mip_count,
      vk::ImageLayout from,
      vk::ImageLayout to
    );
    void convert_image(
      vk::AccessFlagBits src_access_mask,
      vk::AccessFlagBits dest_access_mask,
      vk::PipelineStageFlagBits src_stage,
      vk::PipelineStageFlagBits dest_stage,
      const std::shared_ptr< image_t > &image,
      vk::ImageLayout from,
      vk::ImageLayout to
    );
    void bind_descriptor_set(
      vk::PipelineBindPoint bind_point,
      std::shared_ptr< pipeline_layout_t > pipeline_layout,
      std::shared_ptr< descriptor_set_t > descriptor_set
    );
    void bind_pipeline(
      vk::PipelineBindPoint bind_point,
      std::shared_ptr< compute_pipeline_t > pipeline
    );
    const command_buffer_begin_info_t &get_props() const { return props; }
    vk::CommandBuffer &operator*();
    const vk::CommandBuffer &operator*() const;
    vk::CommandBuffer *operator->();
    const vk::CommandBuffer *operator->() const;
  private:
    command_buffer_begin_info_t props;
  };
}

#endif

