#include <nlohmann/json.hpp>
#include <gct/mappable_buffer.hpp>

namespace gct {
  mappable_buffer_t::mappable_buffer_t(
    const std::shared_ptr< allocator_t > &allocator,
    const buffer_create_info_t &ci,
    VmaAllocationCreateFlags flags
  ) : device( new buffer_t(
    allocator,
    buffer_create_info_t( ci )
      .set_basic(
        vk::BufferCreateInfo( ci.get_basic() )
          .setUsage(
            ci.get_basic().usage |
            vk::BufferUsageFlagBits::eTransferSrc |
            vk::BufferUsageFlagBits::eTransferDst
          )
      ),
    VMA_MEMORY_USAGE_GPU_ONLY,
    flags
  ) ),
  staging( new buffer_t(
    allocator,
    buffer_create_info_t( ci )
      .set_basic(
        vk::BufferCreateInfo( ci.get_basic() )
          .setUsage(
            vk::BufferUsageFlagBits::eTransferSrc |
            vk::BufferUsageFlagBits::eTransferDst
          )
      ),
    VMA_MEMORY_USAGE_CPU_TO_GPU,
    VmaAllocationCreateFlags( 0 )
  ) ) {
  }
  void to_json( nlohmann::json &dest, const mappable_buffer_t &src ) {
    dest = nlohmann::json::object();
    dest[ "device" ] = *src.get_buffer();
    dest[ "staging" ] = *src.get_staging_buffer();
  }
}

