#ifndef GCT_COMMAND_BUFFER_RECORDER_HPP
#define GCT_COMMAND_BUFFER_RECORDER_HPP
#include <string>
#include <cstdint>
#include <memory>
#include <vector>
#include <array>
#include <tuple>
#include <memory>
#include <optional>
#include <filesystem>
#include <tuple>
#include <type_traits>
#include <boost/range/iterator_range.hpp>
#include <vulkan/vulkan.hpp>
#include <gct/created_from.hpp>
#include <gct/command_buffer_begin_info.hpp>
#include <gct/pipeline_vertex_input_state_create_info.hpp>
#include <gct/font.hpp>
#include <gct/numeric_types.hpp>
#include <gct/nnef_data.hpp>
#include <gct/future.hpp>
#include <gct/type_traits/is_lifted.hpp>
#include <gct/type_traits/nth_type.hpp>
#include <gct/property.hpp>

namespace gct {
  struct command_pool_t;
  class buffer_t;
  class mappable_buffer_t;
  class allocator_t;
  class image_t;
  class image_view_t;
  class bound_command_buffer_t;
  class descriptor_set_t;
  class query_pool_t;
  class pipeline_layout_t;
  class compute_pipeline_t;
  class graphics_pipeline_t;
#ifdef VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME
  class ray_tracing_pipeline_t;
  class strided_device_address_region_t;
#endif
#ifdef VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME
  class acceleration_structure_build_geometry_info_t;
  class acceleration_structure_build_region_info_t;
#endif
  class render_pass_begin_info_t;
#if defined(VK_VERSION_1_3) || defined(VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME)
  class rendering_info_t;
#endif
#ifdef VK_EXT_SHADER_OBJECT_EXTENSION_NAME
  class shader_t;
#endif
#ifdef VK_EXT_CONDITIONAL_RENDERING_EXTENSION_NAME
  class conditional_rendering_begin_info_t;
#endif
  [[nodiscard]] std::uint32_t get_pot( std::uint32_t v );
  class command_buffer_recorder_t :
    public property< command_buffer_begin_info_t >,
    public created_from< bound_command_buffer_t > {
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
    [[nodiscard]] std::shared_ptr< buffer_t > load_buffer(
      const std::shared_ptr< allocator_t > &allocator,
      const void * addr,
      std::size_t size,
      vk::BufferUsageFlags usage
    );
    std::shared_ptr< buffer_t > load_buffer(
      const std::shared_ptr< allocator_t > &allocator,
      const std::vector< uint8_t > &data,
      vk::BufferUsageFlags usage
    );
    [[nodiscard]] std::shared_ptr< buffer_t > load_buffer_from_file(
      const std::shared_ptr< allocator_t > &allocator,
      const std::string &filename,
      vk::BufferUsageFlags usage
    );
    void load_buffer(
      const std::shared_ptr< allocator_t > &allocator,
      const void * addr,
      std::size_t size,
      const std::shared_ptr< buffer_t > &dest
    );
    void load_buffer(
      const std::shared_ptr< allocator_t > &allocator,
      const std::vector< uint8_t > &data,
      const std::shared_ptr< buffer_t > &dest
    );
    [[nodiscard]] std::shared_ptr< image_t > load_image(
      const std::shared_ptr< allocator_t > &allocator,
      const std::string &filename,
      vk::ImageUsageFlagBits usage,
      bool mipmap,
      integer_attribute_t attr = integer_attribute_t::srgb,
      unsigned int max_channels_per_layer = 4
    );
    [[nodiscard]] future< std::vector< std::uint8_t > > dump_buffer(
      const std::shared_ptr< allocator_t > &allocator,
      const std::shared_ptr< buffer_t > &buffer
    );
    [[nodiscard]] future< std::vector< std::uint8_t > > dump_buffer(
      const std::shared_ptr< buffer_t > &buffer,
      const std::shared_ptr< buffer_t > &staging_buffer
    );
    [[nodiscard]] future< std::vector< std::uint8_t > > dump_buffer(
      const std::shared_ptr< mappable_buffer_t > &buffer
    );
    template< typename T >
    [[nodiscard]] auto dump_buffer_as(
      const std::shared_ptr< mappable_buffer_t > &buffer
    ) -> std::enable_if_t<
      !type_traits::is_lifted_by_v< T, std::vector >,
      future< T >
    > {
// NOLINTBEGIN(cppcoreguidelines-pro-type-reinterpret-cast,cppcoreguidelines-rvalue-reference-param-not-moved)
      return dump_buffer( buffer ).then(
        []( std::vector< std::uint8_t > &&v ) -> T {
          return *reinterpret_cast< T* >( v.data() );
        }
      );
// NOLINTEND(cppcoreguidelines-pro-type-reinterpret-cast,cppcoreguidelines-rvalue-reference-param-not-moved)
    }
    template< typename T >
    [[nodiscard]] auto dump_buffer_as(
      const std::shared_ptr< mappable_buffer_t > &buffer
    ) -> std::enable_if_t<
      type_traits::is_lifted_by_v< T, std::vector >,
      future< T >
    > {
      return dump_buffer( buffer ).then(
        []( std::vector< std::uint8_t > &&v ) -> T {
// NOLINTBEGIN(cppcoreguidelines-pro-type-reinterpret-cast)
          T temp(
            reinterpret_cast< typename T::value_type* >( v.data() ),
            std::next( reinterpret_cast< typename T::value_type* >( v.data() ), v.size()/sizeof( typename T::value_type ) )
          );
// NOLINTEND(cppcoreguidelines-pro-type-reinterpret-cast)
          return temp;
        }
      );
    }
    [[nodiscard]] nnef_data_t
    load_nnef_data(
      const std::shared_ptr< allocator_t > &allocator,
      const std::filesystem::path &path,
      vk::BufferUsageFlags usage
    );
    void dump_image(
      const std::shared_ptr< allocator_t > &allocator,
      const std::shared_ptr< image_t > &image,
      const std::string &filename,
      unsigned int mipmap,
      unsigned int layer = 0u,
      unsigned int depth = 0u
    );
    void dump_field(
      const std::shared_ptr< allocator_t > &allocator,
      const std::shared_ptr< image_t > &image,
      const std::string &filename,
      unsigned int mipmap,
      unsigned int layer,
      unsigned int depth,
      unsigned int channel,
      const std::optional< double > &clamp_min = std::nullopt,
      const std::optional< double > &clamp_max = std::nullopt
    );
    [[nodiscard]] std::shared_ptr< image_t > load_astc(
      const std::shared_ptr< allocator_t > &allocator,
      const std::vector< std::string > &filename,
      vk::ImageUsageFlagBits usage,
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
// NOLINTBEGIN(cppcoreguidelines-pro-type-reinterpret-cast)
      copy(
        boost::make_iterator_range(
          reinterpret_cast< const std::uint8_t* >( &src ),
          reinterpret_cast< const std::uint8_t* >( &src ) + sizeof( T )
        ),
        staging,
        dest
      );
// NOLINTEND(cppcoreguidelines-pro-type-reinterpret-cast)
    }
    void copy(
      const boost::iterator_range< const std::uint8_t* >&,
      const std::shared_ptr< mappable_buffer_t >&
    );
    template< typename T >
    void copy(
      const T &src,
      const std::shared_ptr< mappable_buffer_t > &dest
    ) {
// NOLINTBEGIN(cppcoreguidelines-pro-type-reinterpret-cast)
      copy(
        boost::make_iterator_range(
          reinterpret_cast< const std::uint8_t* >( &src ),
          reinterpret_cast< const std::uint8_t* >( &src ) + sizeof( T )
        ),
        dest
      );
// NOLINTEND(cppcoreguidelines-pro-type-reinterpret-cast)
    }
  private:
    void copy(
      const std::shared_ptr< buffer_t >&,
      const std::shared_ptr< buffer_t >&,
      const std::vector< vk::BufferCopy >&
    );
  public:
    void copy(
      const std::shared_ptr< buffer_t >&,
      const std::shared_ptr< buffer_t >&,
      const vk::BufferCopy&
    );
    void copy(
      const std::shared_ptr< buffer_t >&,
      const std::shared_ptr< buffer_t >&
    );
    void sync_to_device(
      const std::shared_ptr< mappable_buffer_t > &dest
    );
    void sync_to_host(
      const std::shared_ptr< mappable_buffer_t > &dest
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
    void copy(
      const std::shared_ptr< image_t > &src,
      const std::shared_ptr< buffer_t > &dest,
      const std::vector< vk::BufferImageCopy > &range,
      vk::ImageLayout final_layout
    );
    void copy(
      const std::shared_ptr< image_t > &src,
      const std::shared_ptr< buffer_t > &dest,
      const vk::BufferImageCopy &range,
      vk::ImageLayout final_layout
    );
    void copy(
      const std::shared_ptr< image_t > &src,
      const std::shared_ptr< buffer_t > &dest,
      vk::ImageLayout final_layout
    );
    void copy(
      const std::shared_ptr< image_t > &src,
      const std::shared_ptr< buffer_t > &dest,
      vk::ImageLayout final_layout,
      unsigned int i
    );
    void copy(
      const std::shared_ptr< image_t > &src,
      const std::shared_ptr< buffer_t > &dest,
      const std::vector< vk::BufferImageCopy > &range
    );
    void copy(
      const std::shared_ptr< image_t > &src,
      const std::shared_ptr< buffer_t > &dest,
      const vk::BufferImageCopy &range
    );
    void copy(
      const std::shared_ptr< image_t > &src,
      const std::shared_ptr< buffer_t > &dest
    );
    void copy(
      const std::shared_ptr< image_t > &src,
      const std::shared_ptr< buffer_t > &dest,
      unsigned int i
    );
    void blit(
      const std::shared_ptr< image_t > &src,
      const std::shared_ptr< image_t > &dest,
      const std::vector< vk::ImageBlit > &range,
      vk::Filter filter
    );
    void blit(
      const std::shared_ptr< image_t > &src,
      const std::shared_ptr< image_t > &dest,
      const vk::ImageBlit &range,
      vk::Filter filter
    );
    void blit(
      const std::shared_ptr< image_t > &src,
      const std::shared_ptr< image_t > &dest
    );
    void fill(
      const std::shared_ptr< buffer_t > &,
      std::uint32_t
    );
    void fill(
      const std::shared_ptr< mappable_buffer_t > &,
      std::uint32_t
    );
    void fill(
      const std::shared_ptr< image_t > &dest,
      const std::array< float, 4u > &color
    );
    void fill(
      const std::shared_ptr< image_t > &dest,
      const vk::ClearColorValue &color
    );
    void fill(
      const std::shared_ptr< image_t > &dest,
      const std::array< float, 4u > &color,
      const vk::ImageSubresourceRange &range
    );
    void fill(
      const std::shared_ptr< image_t > &dest,
      const vk::ClearColorValue &color,
      const vk::ImageSubresourceRange &range
    );
    std::vector< vk::ImageMemoryBarrier > barrier(
      vk::AccessFlags,
      vk::AccessFlags,
      vk::PipelineStageFlagBits,
      vk::PipelineStageFlagBits,
      vk::DependencyFlagBits,
      const std::vector< std::shared_ptr< buffer_t > >&,
      const std::vector< std::shared_ptr< image_t > >&
    );
    std::vector< vk::ImageMemoryBarrier > barrier(
      const std::vector< std::shared_ptr< buffer_t > > &buffer,
      const std::vector< std::shared_ptr< image_t > > &image
    );
    std::vector< vk::ImageMemoryBarrier > compute_barrier(
      const std::vector< std::shared_ptr< buffer_t > > &buffer,
      const std::vector< std::shared_ptr< image_t > > &image
    );
    std::vector< vk::ImageMemoryBarrier > compute_write_barrier(
      const std::vector< std::shared_ptr< buffer_t > > &buffer,
      const std::vector< std::shared_ptr< image_t > > &image
    );
    std::vector< vk::ImageMemoryBarrier > transfer_barrier(
      const std::vector< std::shared_ptr< buffer_t > > &buffer,
      const std::vector< std::shared_ptr< image_t > > &image
    );
    std::vector< vk::ImageMemoryBarrier > transfer_to_compute_barrier(
      const std::vector< std::shared_ptr< buffer_t > > &buffer,
      const std::vector< std::shared_ptr< image_t > > &image
    );
    std::vector< vk::ImageMemoryBarrier > compute_to_transfer_barrier(
      const std::vector< std::shared_ptr< buffer_t > > &buffer,
      const std::vector< std::shared_ptr< image_t > > &image
    );
    std::vector< vk::ImageMemoryBarrier > transfer_to_graphics_barrier(
      const std::vector< std::shared_ptr< buffer_t > > &buffer,
      const std::vector< std::shared_ptr< image_t > > &image
    );
    std::vector< vk::ImageMemoryBarrier > graphics_to_transfer_barrier(
      const std::vector< std::shared_ptr< buffer_t > > &buffer,
      const std::vector< std::shared_ptr< image_t > > &image
    );
    std::vector< vk::ImageMemoryBarrier > compute_to_graphics_barrier(
      const std::vector< std::shared_ptr< buffer_t > > &buffer,
      const std::vector< std::shared_ptr< image_t > > &image
    );
    std::vector< vk::ImageMemoryBarrier > graphics_to_compute_barrier(
      const std::vector< std::shared_ptr< buffer_t > > &buffer,
      const std::vector< std::shared_ptr< image_t > > &image
    );
    /*
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
    */
    std::vector< vk::ImageMemoryBarrier > convert_image(
      vk::AccessFlagBits src_access_mask,
      vk::AccessFlagBits dest_access_mask,
      vk::PipelineStageFlagBits src_stage,
      vk::PipelineStageFlagBits dest_stage,
      const std::shared_ptr< image_t > &image,
      uint32_t mip_base,
      uint32_t mip_count,
      uint32_t array_base,
      uint32_t array_count,
      vk::ImageLayout to
    );
    std::vector< vk::ImageMemoryBarrier > convert_image(
      vk::AccessFlagBits src_access_mask,
      vk::AccessFlagBits dest_access_mask,
      vk::PipelineStageFlagBits src_stage,
      vk::PipelineStageFlagBits dest_stage,
      const std::shared_ptr< image_t > &image,
      vk::ImageLayout to
    );
    std::vector< vk::ImageMemoryBarrier > convert_image(
      const std::shared_ptr< image_t > &image,
      uint32_t mip_base,
      uint32_t mip_count,
      uint32_t array_base,
      uint32_t array_count,
      vk::ImageLayout to
    );
    std::vector< vk::ImageMemoryBarrier > convert_image(
      const std::shared_ptr< image_t > &image,
      vk::ImageLayout to
    );
    std::vector< vk::ImageMemoryBarrier > revert_convert_image(
      vk::PipelineStageFlagBits src_stage,
      vk::PipelineStageFlagBits dest_stage,
      const std::shared_ptr< image_t > &image,
      const std::vector< vk::ImageMemoryBarrier > &old
    );
    std::vector< vk::ImageMemoryBarrier > revert_convert_image(
      const std::shared_ptr< image_t > &image,
      const std::vector< vk::ImageMemoryBarrier > &old
    );
    void bind_descriptor_set(
      vk::PipelineBindPoint bind_point,
      std::uint32_t offset,
      std::shared_ptr< pipeline_layout_t > pipeline_layout,
      std::shared_ptr< descriptor_set_t > descriptor_set
    );
    void bind_descriptor_set(
      vk::PipelineBindPoint bind_point,
      std::shared_ptr< pipeline_layout_t > pipeline_layout,
      std::shared_ptr< descriptor_set_t > descriptor_set
    );
    void bind_descriptor_set(
      vk::PipelineBindPoint bind_point,
      std::uint32_t offset,
      std::shared_ptr< pipeline_layout_t > pipeline_layout,
      const std::vector< std::shared_ptr< descriptor_set_t > > &descriptor_set
    );
    void bind_pipeline(
      std::shared_ptr< compute_pipeline_t > pipeline
    );
    void bind_pipeline(
      std::shared_ptr< graphics_pipeline_t > pipeline
    );
#ifdef VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME
    void bind_pipeline(
      std::shared_ptr< ray_tracing_pipeline_t > pipeline
    );
#endif
    void bind(
      std::shared_ptr< compute_pipeline_t > pipeline,
      const std::vector< std::shared_ptr< descriptor_set_t > > &descriptor_set
    );
    void bind(
      std::shared_ptr< graphics_pipeline_t > pipeline,
      const std::vector< std::shared_ptr< descriptor_set_t > > &descriptor_set
    );
#ifdef VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME
    void bind(
      std::shared_ptr< ray_tracing_pipeline_t > pipeline,
      const std::vector< std::shared_ptr< descriptor_set_t > > &descriptor_set
    );
#endif
#ifdef VK_EXT_SHADER_OBJECT_EXTENSION_NAME
  void bind(
     const std::vector< std::shared_ptr< shader_t > > &shader
  );
  void bind(
    const std::vector< std::shared_ptr< shader_t > > &shader,
    const std::shared_ptr< pipeline_layout_t > &pipeline_layout,
    const std::vector< std::shared_ptr< descriptor_set_t > > &descriptor_set
  );
#endif
    void bind_vertex_buffer(
      std::shared_ptr< buffer_t > vertex_buffer
    );
    void bind_vertex_buffer(
      std::shared_ptr< buffer_t >,
      vk::DeviceSize
    );
    void bind_index_buffer(
      std::shared_ptr< buffer_t > index_buffer,
      vk::IndexType type
    );
    void bind_index_buffer(
      std::shared_ptr< buffer_t > index_buffer,
      vk::DeviceSize offset,
      vk::IndexType type
    );
#if defined(VK_VERSION_1_3) || defined(VK_EXT_EXTENDED_DYNAMIC_STATE_EXTENSION_NAME) || defined(VK_EXT_SHADER_OBJECT_EXTENSION_NAME)
    void bind_vertex_buffer(
      std::uint32_t,
      const std::vector< std::shared_ptr< buffer_t > > &,
      const std::vector< vk::DeviceSize > &offset,
      const std::vector< vk::DeviceSize > &size,
      const std::vector< vk::DeviceSize > &stride
    );
    void bind_vertex_buffer(
      std::uint32_t,
      const std::vector< std::shared_ptr< buffer_t > > &,
      const std::vector< vk::DeviceSize > &size,
      const std::vector< vk::DeviceSize > &stride
    );
    void bind_vertex_buffer(
      std::uint32_t,
      const std::vector< std::shared_ptr< buffer_t > > &,
      const std::vector< vk::DeviceSize > &stride
    );
    void bind_vertex_buffer(
      const std::vector< std::shared_ptr< buffer_t > > &,
      const std::vector< vk::DeviceSize > &stride
    );
#endif
    [[nodiscard]] std::shared_ptr< void > begin(
      const std::shared_ptr< query_pool_t > &query_pool,
      std::uint32_t query,
      vk::QueryControlFlags flags
    );
#ifdef VK_EXT_TRANSFORM_FEEDBACK_EXTENSION_NAME
    [[nodiscard]] std::shared_ptr< void > begin(
      const std::shared_ptr< query_pool_t > &query_pool,
      std::uint32_t query,
      vk::QueryControlFlags flags,
      std::uint32_t index
    );
#endif
#ifdef VK_EXT_CONDITIONAL_RENDERING_EXTENSION_NAME
    [[nodiscard]] std::shared_ptr< void > begin(
      const conditional_rendering_begin_info_t&
    );
#endif
#ifdef VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME
    void build_acceleration_structure(
      const std::vector< gct::acceleration_structure_build_geometry_info_t >&,
      const std::vector< std::vector< vk::AccelerationStructureBuildRangeInfoKHR > >&
    );
    void build_acceleration_structure(
      const gct::acceleration_structure_build_geometry_info_t&,
      const std::vector< vk::AccelerationStructureBuildRangeInfoKHR >&
    );
#endif
#ifdef VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME
    void trace_rays(
      const strided_device_address_region_t&,
      const strided_device_address_region_t&,
      const strided_device_address_region_t&,
      const strided_device_address_region_t&,
      std::uint32_t,
      std::uint32_t,
      std::uint32_t
    );
#endif
    [[nodiscard]] std::shared_ptr< void > begin_render_pass(
      const render_pass_begin_info_t &begin_info,
      vk::SubpassContents subpass_contents
    );
#if defined(VK_VERSION_1_3) || defined(VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME)
    [[nodiscard]] std::shared_ptr< void > begin_rendering(
      const rendering_info_t&
    );
#endif
    [[nodiscard]] std::tuple<
      std::shared_ptr< buffer_t >,
      pipeline_vertex_input_state_create_info_t,
      std::uint32_t
    >
    generate_triangle(
      const std::shared_ptr< allocator_t >&
    );

    [[nodiscard]] font load_font(
      std::filesystem::path path,
      const std::shared_ptr< allocator_t > &allocator
    );
    
    void set_image_layout(
      const std::vector< std::shared_ptr< image_view_t > > &views,
      vk::ImageLayout layout
    );
    void set_image_layout(
      const std::vector< std::shared_ptr< image_t > > &images,
      vk::ImageLayout layout
    );
    void set_image_layout(
      const std::shared_ptr< image_view_t > &views,
      vk::ImageLayout layout
    );
    void set_image_layout(
      const std::shared_ptr< image_t > &images,
      vk::ImageLayout layout
    );

    vk::CommandBuffer &operator*();
    const vk::CommandBuffer &operator*() const;
    vk::CommandBuffer *operator->();
    const vk::CommandBuffer *operator->() const;
    void dispatch_threads(
      std::uint32_t x,
      std::uint32_t y,
      std::uint32_t z
    );
    void draw(
      std::uint32_t vertex_count,
      std::uint32_t instnce_count,
      std::uint32_t first_vertex,
      std::uint32_t first_instance
    );
    void draw_indexed(
      std::uint32_t index_count,
      std::uint32_t instnce_count,
      std::uint32_t first_index,
      std::int32_t vertex_offset,
      std::uint32_t first_instance
    );
    void draw_indirect(
      const std::shared_ptr< buffer_t > &buffer,
      vk::DeviceSize offset,
      std::uint32_t raw_count,
      std::uint32_t stride
    );
    void draw_indexed_indirect(
      const std::shared_ptr< buffer_t > &buffer,
      vk::DeviceSize offset,
      std::uint32_t raw_count,
      std::uint32_t stride
    );
    void on_executed(
      const std::function< void( vk::Result ) >&
   );
  private:
    void update_framebuffer_image_layout();
    bool local_size_is_available;
    std::array< std::uint32_t, 3u > local_size;
  };
}

#endif

