#ifndef GCT_COMPUTE_PIPELINE_HPP
#define GCT_COMPUTE_PIPELINE_HPP
#include <memory>
#include <vulkan/vulkan.hpp>
#include <gct/compute_pipeline_create_info.hpp>
#include <gct/pipeline.hpp>
#include <gct/property.hpp>

namespace gct {
  class device_t;
  class compute_pipeline_t :
    public property< compute_pipeline_create_info_t >,
    public pipeline_t,
    public std::enable_shared_from_this< compute_pipeline_t > {
  public:
    compute_pipeline_t(
      const std::shared_ptr< pipeline_cache_t >&,
      const compute_pipeline_create_info_t&
    );
    compute_pipeline_t( const compute_pipeline_t& ) = delete;
    compute_pipeline_t( compute_pipeline_t&& ) = default;
    compute_pipeline_t &operator=( const compute_pipeline_t& ) = delete;
    compute_pipeline_t &operator=( compute_pipeline_t&& ) = default;
  };
  void to_json( nlohmann::json &root, const compute_pipeline_t &v );
}

#endif

