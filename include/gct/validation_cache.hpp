#ifndef GCT_VALIDATION_CACHE_HPP
#define GCT_VALIDATION_CACHE_HPP
#include <memory>
#include <vulkan/vulkan.hpp>
#include <nlohmann/json_fwd.hpp>
#include <gct/created_from.hpp>
#include <gct/validation_cache_create_info.hpp>
#include <vulkan/vulkan_handles.hpp>
#include <gct/vulkan_handle.hpp>
#include <gct/property.hpp>

namespace gct {
  struct allocator_t;
  class device_t;
  class validation_cache_t :
    public vulkan_handle< vk::ValidationCacheEXT >,
    public property< validation_cache_create_info_t >,
    public created_from< device_t >,
    public std::enable_shared_from_this< validation_cache_t > {
  public:
    validation_cache_t(
      const std::shared_ptr< device_t >&,
      const validation_cache_create_info_t&
    );
    validation_cache_t( const validation_cache_t& ) = delete;
    validation_cache_t( validation_cache_t&& ) = default;
    validation_cache_t &operator=( const validation_cache_t& ) = delete;
    validation_cache_t &operator=( validation_cache_t&& ) = default;
  };
  void to_json( nlohmann::json &root, const validation_cache_t &v );
}

#endif

