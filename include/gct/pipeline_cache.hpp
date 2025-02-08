#ifndef LIBGCT_INCLUDE_GCT_PIPELINE_CACHE_HPP
#define LIBGCT_INCLUDE_GCT_PIPELINE_CACHE_HPP

#include <memory>
#include <string>
#include <utility>
#include <unordered_map>
#include <filesystem>
#include <glm/vec3.hpp>
#include <vulkan/vulkan.hpp>
#include <nlohmann/json_fwd.hpp>
#include <gct/created_from.hpp>
#include <gct/pipeline_cache_create_info.hpp>
#include <gct/specialization_map.hpp>
#include <gct/vulkan_handle.hpp>
#include <gct/property.hpp>

namespace gct {
  class device_t;
  class compute_pipeline_t;
  class compute_pipeline_create_info_t;
  class graphics_pipeline_t;
  class graphics_pipeline_create_info_t;
  class descriptor_set_layout_t;
#ifdef VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME
  class ray_tracing_pipeline_t;
  class deferred_operation_t;
  class ray_tracing_pipeline_create_info_t;
#endif
  class pipeline_cache_t :
    public vulkan_handle< vk::PipelineCache >,
    public property< pipeline_cache_create_info_t >,
    public created_from< device_t >,
    public std::enable_shared_from_this< pipeline_cache_t > {
  public:
    pipeline_cache_t(
      const std::shared_ptr< device_t >&,
      const pipeline_cache_create_info_t&
    );
    [[nodiscard]] std::shared_ptr< compute_pipeline_t > get_pipeline(
      const compute_pipeline_create_info_t&
    );
    [[nodiscard]] std::shared_ptr< graphics_pipeline_t > get_pipeline(
      const graphics_pipeline_create_info_t&
    );
#ifdef VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME
    std::shared_ptr< ray_tracing_pipeline_t > get_pipeline(
      const ray_tracing_pipeline_create_info_t&
    );
#endif
    [[nodiscard]] std::pair<
      std::shared_ptr< descriptor_set_layout_t >,
      std::shared_ptr< compute_pipeline_t >
    >
    get_pipeline(
      const std::string &path
    );
    [[nodiscard]] std::pair<
      std::unordered_map< unsigned int, std::shared_ptr< descriptor_set_layout_t > >,
      std::shared_ptr< compute_pipeline_t >
    >
    get_pipeline2(
      const std::string &path,
      std::vector< std::shared_ptr< descriptor_set_layout_t > > &external_descriptor_set_layout
    );
    [[nodiscard]] std::pair<
      std::unordered_map< unsigned int, std::shared_ptr< descriptor_set_layout_t > >,
      std::shared_ptr< compute_pipeline_t >
    >
    get_pipeline2(
      const std::string &path,
      std::vector< std::shared_ptr< descriptor_set_layout_t > > &external_descriptor_set_layout,
      const specialization_map &specs,
      const glm::ivec3 &dim
    );
    void dump( const std::filesystem::path &filename ) const;
  };
  void to_json( nlohmann::json &dest, const pipeline_cache_t &src );
}

#endif
