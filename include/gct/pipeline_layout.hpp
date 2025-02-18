#ifndef GCT_PIPELINE_LAYOUT_HPP
#define GCT_PIPELINE_LAYOUT_HPP
#include <memory>
#include <nlohmann/json_fwd.hpp>
#include <vulkan/vulkan.hpp>
#include <gct/created_from.hpp>
#include <gct/pipeline_layout_create_info.hpp>
#include <gct/vulkan_handle.hpp>
#include <gct/property.hpp>

namespace gct {
  struct device_t;
  class pipeline_layout_t :
    public vulkan_handle< vk::PipelineLayout >,
    public property< pipeline_layout_create_info_t >,
    public created_from< device_t >,
    public std::enable_shared_from_this< pipeline_layout_t > {
  public:
    pipeline_layout_t(
      const std::shared_ptr< device_t >&,
      const pipeline_layout_create_info_t&
    );
    pipeline_layout_t( const pipeline_layout_t& ) = delete;
    pipeline_layout_t( pipeline_layout_t&& ) = default;
    pipeline_layout_t &operator=( const pipeline_layout_t& ) = delete;
    pipeline_layout_t &operator=( pipeline_layout_t&& ) = default;
  };

  void to_json( nlohmann::json&, const pipeline_layout_t& );
}

#endif

