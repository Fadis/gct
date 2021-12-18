#include <iostream>
#include <gct/get_extensions.hpp>
#include <gct/instance.hpp>
#include <gct/vulkanhpp.hpp>
#include <gct/io_context.hpp>
namespace gct {
  bool is_valid_vulkan_version( std::uint32_t version ) {
    return
      version >= VK_API_VERSION_1_0 &&
      version <= get_instance_version();
  }

  instance_t::instance_t(
    const std::string &application_name,
    uint32_t application_version,
    uint32_t api_version_,
    const std::vector< const char* > &iext,
    const std::vector< const char* > &ilayers
  ) : api_version( api_version_ ) {
    vulkanhpp::init();
    thread_pool::init();
    if( !is_valid_vulkan_version( api_version ) )
      throw -1;
    const auto app_info = vk::ApplicationInfo(
      application_name.c_str(),
      application_version,
      "gct",
      VK_MAKE_VERSION(1, 0, 0),
      api_version
    );
    auto available_layers = get_instance_layers();
    for( const auto &l: ilayers ) {
      auto found = available_layers.find( l );
      if( found == available_layers.end() )
        throw -1;
      else activated_layers.insert( *found );
    }
    auto available_extensions = get_instance_extensions( ilayers );
    for( const auto &e: iext ) {
      auto found = available_extensions.find( e );
      if( found == available_extensions.end() )
        throw -1;
      else activated_extensions.insert( *found );
    }
    handle = vk::createInstanceUnique(
      vk::InstanceCreateInfo()
        .setPApplicationInfo( &app_info )
        .setEnabledExtensionCount( iext.size() )
        .setPpEnabledExtensionNames( iext.data() )
        .setEnabledLayerCount( ilayers.size() )
        .setPpEnabledLayerNames( ilayers.data() )
    );
    VULKAN_HPP_DEFAULT_DISPATCHER.init( *handle );
  }
  device_groups_t
  instance_t::get_physical_devices(
    const std::vector< const char* > &device_layers
  ) {
    std::unordered_map< VkPhysicalDevice, std::shared_ptr< physical_device_t > > devices;
    for( auto &device: handle->enumeratePhysicalDevices() ) {
      std::shared_ptr< physical_device_t > detailed(
        new physical_device_t(
          shared_from_this(),
          device,
          device_layers
        )
      );
      devices.insert( 
        std::make_pair(
          static_cast< VkPhysicalDevice >( device ),
          std::move( detailed )
        )
      );
    }
    device_groups_t groups;

#ifdef VK_VERSION_1_1
    if( api_version >= VK_MAKE_VERSION( 1, 1, 0 ) ) {
      // this produces invalid validation VUID-VkPhysicalDeviceGroupProperties-sType-sType
      for( auto &group: handle->enumeratePhysicalDeviceGroups() ) {
        device_group_t group_;
        for( std::uint32_t i = 0u; i != group.physicalDeviceCount; ++i ) {
          auto detailed = devices.find( group.physicalDevices[ i ] );
          if( detailed == devices.end() )
            throw -1;
          group_.devices.push_back( detailed->second );
        }
        group_.subset_allocation = group.subsetAllocation;
        groups.push_back( std::move( group_ ) );
      }
    }
#if defined(VK_KHR_DEVICE_GROUP_CREATION_EXTENSION_NAME)
    else if( activated_extensions.find( "VK_KHR_device_group_creation" ) != activated_extensions.end() ) {
      for( auto &group: handle->enumeratePhysicalDeviceGroups() ) {
        device_group_t group_;
        for( std::uint32_t i = 0u; i != group.physicalDeviceCount; ++i ) {
          auto detailed = devices.find( group.physicalDevices[ i ] );
          if( detailed == devices.end() )
            throw -1;
          group_.devices.push_back( detailed->second );
        }
        group_.subset_allocation = group.subsetAllocation;
        groups.push_back( std::move( group_ ) );
      }
    }
#endif
#elif defined(VK_KHR_DEVICE_GROUP_CREATION_EXTENSION_NAME)
    if( activated_extensions.find( "VK_KHR_device_group_creation" ) != activated_extensions.end() ) {
      for( auto &group: handle->enumeratePhysicalDeviceGroupsKHR() ) {
        device_group_t group_;
        for( std::uint32_t i = 0u; i != group.physicalDeviceCount; ++i ) {
          auto detailed = devices.find( group.physicalDevices[ i ] );
          if( detailed == devices.end() )
            throw -1;
          group_.devices.push_back( detailed->second );
        }
        group_.subset_allocation = group.subsetAllocation;
        groups.push_back( std::move( group_ ) );
      }
    }
#else
#endif
    return groups;
  }
}

