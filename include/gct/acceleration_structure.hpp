#ifndef GCT_ACCELERATION_STRUCTURE_HPP
#define GCT_ACCELERATION_STRUCTURE_HPP
#include <vulkan/vulkan.hpp>
#ifdef VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME
#include <memory>
#include <gct/acceleration_structure_create_info.hpp>
#include <gct/created_from.hpp>
#include <gct/device_address.hpp>

namespace gct {
  class device_t;
  class buffer_t;
#if defined(VK_VERSION_1_2) || defined(VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME)
  class acceleration_structure_device_address_info_t;
#endif
  class acceleration_structure_t : public created_from< buffer_t >, public std::enable_shared_from_this< acceleration_structure_t > {
  public:
    acceleration_structure_t(
      const std::shared_ptr< buffer_t >&,
      const acceleration_structure_create_info_t&
    );
    virtual ~acceleration_structure_t() {}
    const acceleration_structure_create_info_t &get_props() const { return props; }
    virtual vk::AccelerationStructureKHR &operator*() { return *handle; }
    virtual const vk::AccelerationStructureKHR &operator*() const { return *handle; }
    virtual vk::AccelerationStructureKHR *operator->() { return &handle.get(); }
    virtual const vk::AccelerationStructureKHR *operator->() const { return &handle.get(); }
#if defined(VK_VERSION_1_2) || defined(VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME)
    device_address_t get_address( const acceleration_structure_device_address_info_t& );
    device_address_t get_address();
#endif
  private:
    acceleration_structure_create_info_t props;
    vk::UniqueHandle< vk::AccelerationStructureKHR, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE > handle;
  };
  void to_json( nlohmann::json &root, const acceleration_structure_t &v );
}
#endif

#endif

