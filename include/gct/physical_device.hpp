#ifndef LIBGCT_INCLUDE_PHYSICAL_DEVICE_HPP
#define LIBGCT_INCLUDE_PHYSICAL_DEVICE_HPP

#include <memory>
#include <vector>
#include <unordered_set>
#include <boost/container/static_vector.hpp>
#include <nlohmann/json_fwd.hpp>
#include <vulkan/vulkan.hpp>
#include <gct/physical_device_properties.hpp>
#include <gct/physical_device_features.hpp>
#include <gct/queue_requirement.hpp>
#include <gct/created_from.hpp>
#include <gct/format_properties.hpp>

namespace gct {
  class instance_t;
#if defined(VK_KHR_SURFACE_EXTENSION_NAME) && defined(VK_KHR_DISPLAY_EXTENSION_NAME)
  class display_surface_create_info_t;
#endif
  class physical_device_t : public created_from< instance_t > {
  public:
    physical_device_t(
      const std::shared_ptr< instance_t > &instance_,
      vk::PhysicalDevice handle_,
      const std::vector< const char* > &device_layers
    );
    physical_device_t(
      const std::shared_ptr< instance_t > &instance_,
      vk::PhysicalDevice handle_,
      const std::vector< const char* > &device_layers,
      const std::vector< const char* > &device_ext
    );
    [[nodiscard]] vk::PhysicalDevice &operator*() { return handle; } 
    [[nodiscard]] const vk::PhysicalDevice &operator*() const { return handle; } 
    [[nodiscard]] vk::PhysicalDevice *operator->() { return &handle; } 
    [[nodiscard]] const vk::PhysicalDevice *operator->() const { return &handle; }
    [[nodiscard]] const physical_device_properties_t &get_props() const { return props; }
    [[nodiscard]] const physical_device_features_t &get_features() const { return features; }
    [[nodiscard]] physical_device_features_t &get_features() { return features; }
    [[nodiscard]] physical_device_t with_extensions(
      const std::vector< const char* > &exts
    ) const;
    [[nodiscard]] physical_device_t with_features( const physical_device_features_t& ) const;
    [[nodiscard]] const std::unordered_set< vk::Format > &get_vertex_buffer_formats() const {
      return vertex_buffer_formats;
    }
#if defined(VK_KHR_SURFACE_EXTENSION_NAME) && defined(VK_KHR_DISPLAY_EXTENSION_NAME)
    [[nodiscard]] std::shared_ptr< surface_t > get_surface(
      const display_surface_create_info_t &
    );
#endif
    [[nodiscard]] format_properties get_format_properties( vk::Format ) const;
  private:
    void detect_vertex_buffer_formats();
    mutable vk::PhysicalDevice handle;
    physical_device_properties_t props;
    physical_device_features_t features;
    std::unordered_set< vk::Format > vertex_buffer_formats;
  };
  using physical_devices_t =
    boost::container::static_vector<
      std::shared_ptr< physical_device_t >,
      VK_MAX_DEVICE_GROUP_SIZE
    >;
  class device_t;
  class device_create_info_t;
  struct device_group_t {
  public:
    [[nodiscard]] device_group_t with_extensions(
      const std::vector< const char* > &exts
    ) const;
    [[nodiscard]] device_group_t with_features( const physical_device_features_t& ) const;
    std::shared_ptr< device_t > create_device(
      const std::vector< queue_requirement_t > &queue_requirements,
      const device_create_info_t &create_info
    );
    [[nodiscard]] format_properties get_format_properties( vk::Format ) const;
    physical_devices_t devices;
    bool subset_allocation = false;
  };
  using device_groups_t =
    std::vector< device_group_t >;
  
  void to_json( nlohmann::json&, const physical_device_t &v );
  void to_json( nlohmann::json&, const physical_devices_t &v );
  void to_json( nlohmann::json&, const device_group_t &v );
}

#endif

