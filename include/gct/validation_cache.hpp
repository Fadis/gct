#ifndef GCT_VALIDATION_CACHE_HPP
#define GCT_VALIDATION_CACHE_HPP
#include <memory>
#include <vulkan/vulkan.hpp>
#include <nlohmann/json_fwd.hpp>
#include <gct/created_from.hpp>
#include <gct/validation_cache_create_info.hpp>
#include <vulkan/vulkan_handles.hpp>

namespace gct {
  struct allocator_t;
  class device_t;
  class validation_cache_t : public created_from< device_t >, public std::enable_shared_from_this< validation_cache_t > {
  public:
    validation_cache_t(
      const std::shared_ptr< device_t >&,
      const validation_cache_create_info_t&
    );
    validation_cache_t( const validation_cache_t& ) = delete;
    validation_cache_t( validation_cache_t&& ) = default;
    validation_cache_t &operator=( const validation_cache_t& ) = delete;
    validation_cache_t &operator=( validation_cache_t&& ) = default;
    const validation_cache_create_info_t &get_props() const { return props; }
    vk::ValidationCacheEXT &operator*() {
      return *handle;
    }
    const vk::ValidationCacheEXT &operator*() const {
      return *handle;
    }
    vk::ValidationCacheEXT *operator->() {
      return &handle.get();
    }
    const vk::ValidationCacheEXT *operator->() const {
      return &handle.get();
    }
  private:
    validation_cache_create_info_t props;
    vk::UniqueHandle< vk::ValidationCacheEXT, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE > handle;
  };
  void to_json( nlohmann::json &root, const validation_cache_t &v );
}

#endif

