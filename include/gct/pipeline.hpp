#ifndef GCT_PIPELINE_HPP
#define GCT_PIPELINE_HPP
#include <memory>
#include <vulkan/vulkan.hpp>
#include <gct/created_from.hpp>
#include <gct/pipeline_creation_feedback.hpp>
#include <gct/vulkan_handle.hpp>

namespace gct {
  class pipeline_cache_t;
  class pipeline_t :
    public vulkan_handle< vk::Pipeline >,
    public created_from< pipeline_cache_t > {
  public:
    pipeline_t(
      const std::shared_ptr< pipeline_cache_t > &cache
    ) : created_from< pipeline_cache_t >( cache ) {}
    pipeline_t( const pipeline_t& ) = delete;
    pipeline_t( pipeline_t&& ) = default;
    pipeline_t &operator=( const pipeline_t& ) = delete;
    pipeline_t &operator=( pipeline_t&& ) = default;
#ifdef VK_EXT_PIPELINE_CREATION_FEEDBACK_EXTENSION_NAME
    [[nodiscard]] const pipeline_creation_feedback_t &get_feedback() const {
      return feedback;
    }
#endif
  protected:
#ifdef VK_EXT_PIPELINE_CREATION_FEEDBACK_EXTENSION_NAME
    pipeline_creation_feedback_t feedback;
#endif
  };
}

#endif

