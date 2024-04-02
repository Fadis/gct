#ifndef GCT_CUBEMAP_COMPUTE_HPP
#define GCT_CUBEMAP_COMPUTE_HPP
#include <memory>
#include <vector>
#include <filesystem>
#include <nlohmann/json_fwd.hpp>
#include <gct/compute_create_info.hpp>

namespace gct {
  class allocator_t;
  class descriptor_pool_t;
  class pipeline_cache_t;
  class descriptor_set_layout_t;
  class descriptor_set_t;
  class compute_pipeline_t;
  class command_buffer_recorder_t;
  class shader_module_reflection_t;
  class compute : std::enable_shared_from_this< compute > {
  public:
    compute(
      const compute_create_info &ci
    );
    void operator()(
      command_buffer_recorder_t &rec,
      unsigned int image_index,
      unsigned int x,
      unsigned int y,
      unsigned int z
    ) const;
    const compute_create_info &get_props() const {
      return props;
    }
    const std::shared_ptr< compute_pipeline_t > &get_pipeline() const {
      return pipeline;
    }
    const shader_module_reflection_t &get_reflection() const;
  private:
    compute_create_info props;
    std::shared_ptr< descriptor_set_layout_t > descriptor_set_layout;
    std::vector< std::shared_ptr< descriptor_set_t > > descriptor_set;
    std::shared_ptr< compute_pipeline_t > pipeline;
  };
  void to_json( nlohmann::json&, const compute& );
}

#endif

