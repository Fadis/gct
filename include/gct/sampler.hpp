#ifndef GCT_SAMPLER_HPP
#define GCT_SAMPLER_HPP
#include <memory>
#include <vulkan/vulkan.hpp>
#include <nlohmann/json_fwd.hpp>
#include <gct/created_from.hpp>
#include <gct/sampler_create_info.hpp>
#include <gct/vulkan_handle.hpp>
#include <gct/property.hpp>

namespace gct {
  struct allocator_t;
  class device_t;
  class sampler_t :
    public vulkan_handle< vk::Sampler >,
    public property< sampler_create_info_t >,
    public created_from< device_t >,
    public std::enable_shared_from_this< sampler_t > {
  public:
    sampler_t(
      const std::shared_ptr< device_t >&,
      const sampler_create_info_t&
    );
    sampler_t( const sampler_t& ) = delete;
    sampler_t( sampler_t&& ) = default;
    sampler_t &operator=( const sampler_t& ) = delete;
    sampler_t &operator=( sampler_t&& ) = default;
  };
  void to_json( nlohmann::json&, const sampler_t& );
}

#endif

