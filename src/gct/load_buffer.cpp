#include <fstream>
#include <nlohmann/json.hpp>
#include <gct/exception.hpp>
#include <gct/buffer.hpp>
#include <gct/mappable_buffer.hpp>
#include <gct/allocator.hpp>
#include <gct/device.hpp>
#include <gct/command_pool.hpp>
#include <gct/command_buffer.hpp>
#include <gct/command_buffer_recorder.hpp>

namespace gct {
  std::shared_ptr< buffer_t > command_buffer_recorder_t::load_buffer(
    const std::shared_ptr< allocator_t > &allocator,
    const void * addr,
    std::size_t size,
    vk::BufferUsageFlags usage
  ) {
    auto final_buffer = allocator->create_buffer(
      size,
      usage | vk::BufferUsageFlagBits::eTransferDst,
      VMA_MEMORY_USAGE_GPU_ONLY
    );
    auto temporary_buffer = allocator->create_buffer(
      size,
      vk::BufferUsageFlagBits::eTransferSrc,
      VMA_MEMORY_USAGE_AUTO_PREFER_HOST,
      VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT
    );
    {
      auto mapped = temporary_buffer->map< std::uint8_t >();
      std::copy(
        reinterpret_cast< const std::uint8_t* >( addr ),
        std::next( reinterpret_cast< const std::uint8_t* >( addr ), size ),
        mapped.begin()
      );
    }
    (*get_factory())->copyBuffer(
      **temporary_buffer,
      **final_buffer,
      {
        vk::BufferCopy()
          .setSrcOffset( 0 )
          .setDstOffset( 0 )
          .setSize( size )
      }
    );
    get_factory()->unbound()->keep.push_back( std::move( temporary_buffer ) );
    return final_buffer;
  }
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
    get_factory()->unbound()->keep.push_back( final_buffer );
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
  future< std::vector< std::uint8_t > > command_buffer_recorder_t::dump_buffer(
    const std::shared_ptr< allocator_t > &allocator,
    const std::shared_ptr< buffer_t > &buffer
  ) {
    promise< std::vector< std::uint8_t > > p;
    auto f = p.get_future();
    const auto staging_buffer = allocator->create_buffer(
      buffer->get_props().get_basic().size,
      vk::BufferUsageFlagBits::eTransferDst,
      VMA_MEMORY_USAGE_GPU_TO_CPU
    );
    copy(
      buffer,
      staging_buffer
    );
    get_factory()->unbound()->keep.push_back( buffer );
    get_factory()->unbound()->keep.push_back( staging_buffer );
    get_factory()->unbound()->cbs.push_back(
      [staging_buffer,p=std::move(p)]( vk::Result result ) mutable {
        std::vector< std::uint8_t > host_buffer(
          staging_buffer->get_props().get_basic().size
        );
        auto mapped = staging_buffer->map< std::uint8_t >();
        std::copy(
          mapped.begin(),
          mapped.end(),
          host_buffer.begin()
        );
        p.set_value( std::move( host_buffer ) );
      }
    );
    return f;
  }
  future< std::vector< std::uint8_t > > command_buffer_recorder_t::dump_buffer(
    const std::shared_ptr< buffer_t > &buffer,
    const std::shared_ptr< buffer_t > &staging_buffer
  ) {
    promise< std::vector< std::uint8_t > > p;
    auto f = p.get_future();
    copy(
      buffer,
      staging_buffer
    );
    get_factory()->unbound()->keep.push_back( buffer );
    get_factory()->unbound()->keep.push_back( staging_buffer );
    get_factory()->unbound()->cbs.push_back(
      [staging_buffer,p=std::move(p)]( vk::Result result ) mutable {
        std::vector< std::uint8_t > host_buffer(
          staging_buffer->get_props().get_basic().size
        );
        auto mapped = staging_buffer->map< std::uint8_t >();
        std::copy(
          mapped.begin(),
          mapped.end(),
          host_buffer.begin()
        );
        p.set_value( std::move( host_buffer ) );
      }
    );
    return f;
  }
  future< std::vector< std::uint8_t > > command_buffer_recorder_t::dump_buffer(
    const std::shared_ptr< mappable_buffer_t > &buffer
  ) {
    return dump_buffer(
      buffer->get_buffer(),
      buffer->get_staging_buffer()
    );
  }
  void command_buffer_recorder_t::load_buffer(
    const std::shared_ptr< allocator_t > &allocator,
    const void * addr,
    std::size_t size,
    const std::shared_ptr< buffer_t > &dest
  ) {
    if( dest->get_props().get_basic().size < size ) {
      throw exception::length_error( "load_buffer : destination buffer is smaller than source length", __FILE__, __LINE__ );
    }
    auto temporary_buffer = allocator->create_buffer(
      size,
      vk::BufferUsageFlagBits::eTransferSrc,
      VMA_MEMORY_USAGE_AUTO_PREFER_HOST,
      VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT
    );
    {
      auto mapped = temporary_buffer->map< std::uint8_t >();
      std::copy(
        reinterpret_cast< const std::uint8_t* >( addr ),
        std::next( reinterpret_cast< const std::uint8_t* >( addr ), size ),
        mapped.begin()
      );
    }
    (*get_factory())->copyBuffer(
      **temporary_buffer,
      **dest,
      {
        vk::BufferCopy()
          .setSrcOffset( 0 )
          .setDstOffset( 0 )
          .setSize( size )
      }
    );
    get_factory()->unbound()->keep.push_back( std::move( temporary_buffer ) );
    get_factory()->unbound()->keep.push_back( std::move( dest ) );
  }
  void command_buffer_recorder_t::load_buffer(
    const std::shared_ptr< allocator_t > &allocator,
    const std::vector< uint8_t > &data,
    const std::shared_ptr< buffer_t > &dest
  ) {
    load_buffer(
      allocator,
      reinterpret_cast< const void* >( data.data() ),
      data.size(),
      dest
    );
  }
}
