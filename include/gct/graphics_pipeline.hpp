#ifndef GCT_GRAPHICS_PIPELINE_HPP
#define GCT_GRAPHICS_PIPELINE_HPP
#include <memory>
#include <vulkan/vulkan.hpp>
#include <gct/graphics_pipeline_create_info.hpp>
#include <gct/pipeline.hpp>
#include <gct/property.hpp>

namespace gct {
  class device_t;
  class graphics_pipeline_t :
    public property< graphics_pipeline_create_info_t >,
    public pipeline_t,
    public std::enable_shared_from_this< graphics_pipeline_t > {
  public:
    graphics_pipeline_t(
      const std::shared_ptr< pipeline_cache_t >&,
      const graphics_pipeline_create_info_t&
    );
    graphics_pipeline_t( const graphics_pipeline_t& ) = delete;
    graphics_pipeline_t( graphics_pipeline_t&& ) = default;
    graphics_pipeline_t &operator=( const graphics_pipeline_t& ) = delete;
    graphics_pipeline_t &operator=( graphics_pipeline_t&& ) = default;
    const pipeline_t &get_pipeline() const {
      return *this;
    }
  };
  void to_json( nlohmann::json &root, const graphics_pipeline_t &v );
}

#endif

