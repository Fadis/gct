#include <fstream>
#include <gct/buffer.hpp>
#include <gct/allocator.hpp>
#include <gct/device.hpp>
#include <gct/command_pool.hpp>
#include <gct/command_buffer.hpp>
#include <gct/command_buffer_recorder.hpp>

namespace gct {
  std::shared_ptr< buffer_t > command_buffer_recorder_t::load_buffer(
    const std::shared_ptr< allocator_t > &allocator,
    const std::vector< uint8_t > &data,
    vk::BufferUsageFlags usage
  ) {
    auto final_buffer = allocator->create_buffer(
      data.size(),
      usage | vk::BufferUsageFlagBits::eTransferDst,
      VMA_MEMORY_USAGE_GPU_ONLY
    );
    auto temporary_buffer = allocator->create_buffer(
      data.size(),
      vk::BufferUsageFlagBits::eTransferSrc,
      VMA_MEMORY_USAGE_CPU_TO_GPU
    );
    {
      auto mapped = temporary_buffer->map< std::uint8_t >();
      std::copy( data.begin(), data.end(), mapped.begin() );
    }
    (*get_factory())->copyBuffer(
      **temporary_buffer,
      **final_buffer,
      {
        vk::BufferCopy()
          .setSrcOffset( 0 )
          .setDstOffset( 0 )
          .setSize( data.size() )
      }
    );
    get_factory()->unbound()->keep.push_back( std::move( temporary_buffer ) );
    return final_buffer;
  }
  std::shared_ptr< buffer_t > command_buffer_recorder_t::load_buffer_from_file(
    const std::shared_ptr< allocator_t > &allocator,
    const std::string &filename,
    vk::BufferUsageFlags usage
  ) {
    std::ifstream file( filename, std::ios::in | std::ios::binary );
    std::vector< std::uint8_t > data(
      ( std::istreambuf_iterator< char >( file ) ),
      std::istreambuf_iterator< char >()
    );
    return load_buffer(
      allocator,
      data,
      usage
    );
  }
}
