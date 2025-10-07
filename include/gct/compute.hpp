#ifndef GCT_COMPUTE_HPP
#define GCT_COMPUTE_HPP
#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include <nlohmann/json_fwd.hpp>
#include <gct/exception.hpp>
#include <gct/compute_create_info.hpp>
#include <gct/property.hpp>
#include <gct/spv_member_pointer.hpp>
#include <gct/push_constant_storage.hpp>

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
    public std::enable_shared_from_this< compute >,
    public push_constant_storage {
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
    template< typename T >
    compute &set_push_constant(
      const std::string &name,
      const T &value
    ) {
      push_constant_storage::set_push_constant< T >( name, value );
      return *this;
    }
  private:
    std::unordered_map< unsigned int, std::shared_ptr< descriptor_set_layout_t > > descriptor_set_layout;
    std::vector< std::vector< std::shared_ptr< descriptor_set_t > > > descriptor_set;
    std::shared_ptr< compute_pipeline_t > pipeline;
  };
  void to_json( nlohmann::json&, const compute& );
}

#endif

