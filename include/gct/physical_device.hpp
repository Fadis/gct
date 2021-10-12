#ifndef LIBGCT_INCLUDE_PHYSICAL_DEVICE_HPP
#define LIBGCT_INCLUDE_PHYSICAL_DEVICE_HPP

#include <memory>
#include <vector>
#include <boost/container/static_vector.hpp>
#include <nlohmann/json.hpp>
#include <vulkan/vulkan.hpp>
#include <gct/physical_device_properties.hpp>
#include <gct/physical_device_features.hpp>
#include <gct/queue_requirement.hpp>
#include <gct/created_from.hpp>

namespace gct {
  class instance_t;

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
    vk::PhysicalDevice &operator*() { return handle; } 
    const vk::PhysicalDevice &operator*() const { return handle; } 
    vk::PhysicalDevice *operator->() { return &handle; } 
    const vk::PhysicalDevice *operator->() const { return &handle; }
    const physical_device_properties_t &get_props() const { return props; }
    const physical_device_features_t &get_features() const { return features; }
    physical_device_features_t &get_features() { return features; }
    physical_device_t with_extensions(
      const std::vector< const char* > &exts
    );
  private:
    vk::PhysicalDevice handle;
    physical_device_properties_t props;
    physical_device_features_t features;
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
    device_group_t with_extensions(
      const std::vector< const char* > &exts
    );
    std::shared_ptr< device_t > create_device(
      const std::vector< queue_requirement_t > &queue_requirements,
      const device_create_info_t &create_info
    );
    physical_devices_t devices;
    bool subset_allocation;
  };
  using device_groups_t =
    std::vector< device_group_t >;
  
  nlohmann::json to_json( const physical_device_t &v );
  nlohmann::json to_json( const physical_devices_t &v );
  nlohmann::json to_json( const device_group_t &v );
  nlohmann::json to_json( const device_groups_t &v );
}

#endif

