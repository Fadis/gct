#ifndef GCT_CUBEMAP_GRAPHICS_HPP
#define GCT_CUBEMAP_GRAPHICS_HPP
#include <memory>
#include <vector>
#include <filesystem>
#include <gct/graphics_create_info.hpp>
#include <gct/vertex_attributes.hpp>

namespace gct {
  class allocator_t;
  class descriptor_pool_t;
  class pipeline_cache_t;
  class descriptor_set_layout_t;
  class descriptor_set_t;
  class graphic_pipeline_t;
  class command_buffer_recorder_t;
  class graphics : std::enable_shared_from_this< graphics > {
  public:
    graphics(
      const graphics_create_info &ci
    );
    void bind(
      command_buffer_recorder_t &rec,
      unsigned int image_index
    ) const;
    const graphics_create_info &get_props() const {
      return props;
    }
    const std::shared_ptr< graphics_pipeline_t > &get_pipeline() const {
      return pipeline;
    }
    const std::unordered_map< vertex_attribute_usage_t, vertex_input_detail_t > get_vamap() const {
      return vamap;
    }
    std::uint32_t get_stride() const {
      return stride;
    }
  private:
    graphics_create_info props;
    std::shared_ptr< descriptor_set_layout_t > descriptor_set_layout;
    std::vector< std::shared_ptr< descriptor_set_t > > descriptor_set;
    std::shared_ptr< pipeline_layout_t > pipeline_layout;
    std::shared_ptr< graphics_pipeline_t > pipeline;
    gct::pipeline_vertex_input_state_create_info_t vistat;
    std::unordered_map< vertex_attribute_usage_t, vertex_input_detail_t > vamap;
    std::uint32_t stride;
  };
}

#endif


