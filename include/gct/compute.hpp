#ifndef GCT_COMPUTE_HPP
#define GCT_COMPUTE_HPP
#include <memory>
#include <vector>
#include <unordered_map>
#include <nlohmann/json_fwd.hpp>
#include <gct/compute_create_info.hpp>
#include <gct/compute.hpp>
#include <gct/property.hpp>
#include <gct/spv_member_pointer.hpp>

namespace gct {
  class allocator_t;
  class descriptor_pool_t;
  class pipeline_cache_t;
  class descriptor_set_layout_t;
  class descriptor_set_t;
  class compute_pipeline_t;
  class command_buffer_recorder_t;
  class shader_module_reflection_t;
  class compute :
    public property< compute_create_info >,
    public std::enable_shared_from_this< compute > {
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
    [[nodiscard]] const std::shared_ptr< compute_pipeline_t > &get_pipeline() const {
      return pipeline;
    }
    [[nodiscard]] const shader_module_reflection_t &get_reflection() const;
    [[nodiscard]] const std::optional< spv_member_pointer > &get_push_constant_member_pointer() const {
      return push_constant_mp;
    }
    [[nodiscard]] std::vector< std::uint8_t > &get_push_constant() const;
  private:
    std::unordered_map< unsigned int, std::shared_ptr< descriptor_set_layout_t > > descriptor_set_layout;
    std::vector< std::vector< std::shared_ptr< descriptor_set_t > > > descriptor_set;
    std::shared_ptr< compute_pipeline_t > pipeline;
    mutable bool use_internal_push_constant = false;
    mutable std::vector< std::uint8_t > push_constant;
    std::optional< spv_member_pointer > push_constant_mp;
  };
  void to_json( nlohmann::json&, const compute& );
}

#endif

