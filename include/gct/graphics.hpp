#ifndef GCT_CUBEMAP_GRAPHICS_HPP
#define GCT_CUBEMAP_GRAPHICS_HPP
#include <memory>
#include <vector>
#include <filesystem>
#include <gct/graphics_create_info.hpp>
#include <gct/vertex_attributes.hpp>
#include <gct/property.hpp>

namespace gct {
  class allocator_t;
  class descriptor_pool_t;
  class pipeline_cache_t;
  class descriptor_set_layout_t;
  class descriptor_set_t;
  class graphic_pipeline_t;
  class command_buffer_recorder_t;
  class graphics :
    public property< graphics_create_info >,
    public std::enable_shared_from_this< graphics > {
  public:
    graphics(
      const graphics_create_info &ci
    );
    void bind(
      command_buffer_recorder_t &rec,
      unsigned int image_index
    ) const;
    [[nodiscard]] const std::shared_ptr< graphics_pipeline_t > &get_pipeline() const {
      return pipeline;
    }
    [[nodiscard]] const std::unordered_map< vertex_attribute_usage_t, vertex_input_detail_t > get_vamap() const {
      return vamap;
    }
    [[nodiscard]] std::uint32_t get_stride() const {
      return stride;
    }
    [[nodiscard]] const std::shared_ptr< pipeline_layout_t > get_layout() const {
      return pipeline_layout;
    }
  private:
    std::unordered_map< unsigned int, std::shared_ptr< descriptor_set_layout_t > > descriptor_set_layout;
    std::vector< std::vector< std::shared_ptr< descriptor_set_t > > > descriptor_set;
    std::shared_ptr< pipeline_layout_t > pipeline_layout;
    std::shared_ptr< graphics_pipeline_t > pipeline;
    gct::pipeline_vertex_input_state_create_info_t vistat;
    std::unordered_map< vertex_attribute_usage_t, vertex_input_detail_t > vamap;
    std::uint32_t stride;
  };
}

#endif


