#ifndef GCT_DESCRIPTOR_BUFFER_INFO_HPP
#define GCT_DESCRIPTOR_BUFFER_INFO_HPP
#include <memory>
#include <nlohmann/json_fwd.hpp>
#include <vulkan/vulkan.hpp>
#include <gct/extension.hpp>

namespace gct {
  class buffer_t;
  class descriptor_buffer_info_t : public chained_t {
  public:
    using self_type = descriptor_buffer_info_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
    LIBGCT_BASIC_SETTER( vk::DescriptorBufferInfo )
  private:
    std::shared_ptr< buffer_t > buffer;
  public:
    [[nodiscard]] const std::shared_ptr< buffer_t > &get_buffer() const {
      return buffer;
    }
    descriptor_buffer_info_t &set_buffer( const std::shared_ptr< buffer_t > & );
    descriptor_buffer_info_t &clear_buffer();
  };

void to_json( nlohmann::json&, const descriptor_buffer_info_t& );

}
#endif

