#ifndef GCT_DESCRIPTOR_IMAGE_INFO_HPP
#define GCT_DESCRIPTOR_IMAGE_INFO_HPP
#include <memory>
#include <optional>
#include <vulkan/vulkan.hpp>
#include <gct/extension.hpp>

namespace gct {
  class image_view_t;
  class sampler_t;
  class descriptor_image_info_t : public chained_t {
  public:
    using self_type = descriptor_image_info_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
    LIBGCT_BASIC_SETTER( vk::DescriptorImageInfo )
  private:
    std::shared_ptr< image_view_t > image_view;
    std::shared_ptr< sampler_t > sampler;
  public:
    descriptor_image_info_t &set_image_view( const std::shared_ptr< image_view_t > & );
    descriptor_image_info_t &clear_image_view();
    descriptor_image_info_t &set_sampler( const std::shared_ptr< sampler_t > &v );
    descriptor_image_info_t &clear_sampler();
  };
}
#endif

