#ifndef GCT_CUBEMAP_IMAGE_FILTER_HPP
#define GCT_CUBEMAP_IMAGE_FILTER_HPP
#include <memory>
#include <vector>
#include <filesystem>
#include <gct/image_filter_create_info.hpp>
#include <gct/property.hpp>

namespace gct {
  class allocator_t;
  class descriptor_pool_t;
  class pipeline_cache_t;
  class descriptor_set_layout_t;
  class compute_pipeline_t;
  class image_view_t;
  class command_buffer_recorder_t;
  class named_resource;
  class image_filter {
    class image_filter_impl :
      public property< image_filter_create_info >,
      public std::enable_shared_from_this< image_filter_impl > {
    public:
      image_filter_impl(
        const image_filter_create_info &ci
      );
      void operator()(
        command_buffer_recorder_t &rec,
        unsigned int image_index
      ) const;
      [[nodiscard]] const std::vector< std::shared_ptr< image_view_t > > &get_output() const;
      image_filter_impl(
        const std::shared_ptr< image_filter::image_filter_impl > &prev_,
        const image_filter_create_info &ci
      );
      [[nodiscard]] bool is_reused() const {
        return reused;
      }
      [[nodiscard]] const std::shared_ptr< compute_pipeline_t > &get_pipeline() const {
        return pipeline;
      }
    private:
      std::shared_ptr< descriptor_set_layout_t > descriptor_set_layout;
      std::vector< std::shared_ptr< descriptor_set_t > > descriptor_set;
      std::shared_ptr< compute_pipeline_t > pipeline;
      std::shared_ptr< image_filter_impl > prev;
      bool reused = false;
    };
  public:
    image_filter(
      const image_filter_create_info &ci
    );
    void operator()(
      command_buffer_recorder_t &rec,
      unsigned int image_index = 0u
    ) const;
    [[nodiscard]] const std::vector< std::shared_ptr< image_view_t > > &get_output() const {
      return resources->get_output();
    }
    [[nodiscard]] image_filter operator()(
      const image_filter_create_info &ci
    ) const;
    [[nodiscard]] const image_filter_create_info &get_props() const {
      return resources->get_props();
    }
    [[nodiscard]] const std::shared_ptr< compute_pipeline_t > &get_pipeline() const {
      return resources->get_pipeline();
    }
  private:
    image_filter(
      const std::shared_ptr< image_filter_impl > &prev_,
      const image_filter_create_info &ci
    ) : resources( new image_filter_impl(
      prev_,
      ci
    ) ) {}
    std::shared_ptr< image_filter_impl > resources;
  };
}

#endif

