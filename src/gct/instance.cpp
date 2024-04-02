#include <iostream>
#include <nlohmann/json.hpp>
#include <vulkan2json/DebugUtilsMessageSeverityFlagsEXT.hpp>
#include <gct/get_extensions.hpp>
#include <gct/instance.hpp>
#include <gct/vulkanhpp.hpp>
#include <gct/io_context.hpp>
#include <gct/exception.hpp>
#include <vulkan/vulkan_enums.hpp>
namespace gct {
  bool is_valid_vulkan_version( std::uint32_t version ) {
    return
      version >= VK_API_VERSION_1_0 &&
      version <= get_instance_version();
  }

  instance_t::instance_t(
    const instance_create_info_t &create_info
  ) : props( create_info ) {
    props.rebuild_chain();
    vulkanhpp::init();
    thread_pool::init();
    if( !is_valid_vulkan_version( get_api_version() ) )
      throw -1;
    auto available_layers = get_instance_layers();
    for( const auto &l: create_info.get_layer() ) {
      auto found = available_layers.find( l );
      if( found == available_layers.end() )
        throw -1;
      else activated_layers.insert( *found );
    }
    auto available_extensions = get_instance_extensions( create_info.get_layer() );
    for( const auto &e: create_info.get_extension() ) {
      auto found = available_extensions.find( e );
      if( found == available_extensions.end() )
        throw -1;
      else activated_extensions.insert( *found );
    }
    handle = vk::createInstanceUnique(
      props.get_basic()
    );
    VULKAN_HPP_DEFAULT_DISPATCHER.init( *handle );
  }
#if defined(VK_EXT_DEBUG_UTILS_EXTENSION_NAME)
  void instance_t::set_debug_callback(
    vk::DebugUtilsMessageSeverityFlagsEXT severity,
    vk::DebugUtilsMessageTypeFlagsEXT type,
    debug_callback_t &&cb
  ) {
    if( activated_extensions.find( VK_EXT_DEBUG_UTILS_EXTENSION_NAME ) != activated_extensions.end() ) {
      debug_callback = cb;
      debug_message = handle->createDebugUtilsMessengerEXTUnique(
        vk::DebugUtilsMessengerCreateInfoEXT()
          .setMessageSeverity( severity )
          .setMessageType( type )
          .setPfnUserCallback( instance_t::call_debug_callback )
          .setPUserData( reinterpret_cast< void* >( this ) )
      );
    }
    else {
      throw exception::runtime_error( "instance_t::set_debug_callback : Required instance extension VK_EXT_debug_utils is not enabled.", __FILE__, __LINE__ );
    }
  }
  VkBool32 instance_t::call_debug_callback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageTypes,
    const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
    void *pUserData
  ) {
    const auto instance = reinterpret_cast< instance_t* >( pUserData );
    if( !instance ) {
      std::cerr << "instance_t::call_debug_callback : invalid user data." << std::endl;
      std::abort();
    }
    if( !pCallbackData ) {
      std::cerr << "instance_t::call_debug_callback : invalid callback data." << std::endl;
      std::abort();
    }
    if( instance->debug_callback ) {
      instance->debug_callback(
        vk::DebugUtilsMessageSeverityFlagBitsEXT( messageSeverity ),
        vk::DebugUtilsMessageTypeFlagsEXT( messageTypes ),
        *reinterpret_cast< const vk::DebugUtilsMessengerCallbackDataEXT* >( pCallbackData )
      );
    }
    return VK_FALSE;
  }
#endif
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
    if( get_api_version() >= VK_MAKE_VERSION( 1, 1, 0 ) ) {
      // this produces invalid validation VUID-VkPhysicalDeviceGroupProperties-sType-sType
      for( auto &group: handle->enumeratePhysicalDeviceGroups() ) {
        device_group_t group_;
        for( std::uint32_t i = 0u; i != group.physicalDeviceCount; ++i ) {
          auto detailed = devices.find( VkPhysicalDevice( group.physicalDevices[ i ] ) );
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
          auto detailed = devices.find( VkPhysicalDevice( group.physicalDevices[ i ] ) );
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
          auto detailed = devices.find( VkPhysicalDevice( group.physicalDevices[ i ] ) );
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
  std::uint32_t instance_t::get_api_version() const {
    return props.get_basic().pApplicationInfo->apiVersion;
  }
  void instance_t::abort_on_validation_failure( bool debug ) {
    if( activated_layers.find( "VK_LAYER_KHRONOS_validation" ) != activated_layers.end() ) {
      if( activated_extensions.find( "VK_EXT_debug_utils" ) != activated_extensions.end() ) {
        set_debug_callback(
          debug ?
          vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose|
          vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo|
          vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning|
          vk::DebugUtilsMessageSeverityFlagBitsEXT::eError :
          vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning|
          vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
          debug ?
          vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation|
          vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral|
          vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance :
          vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation,
          [](
            vk::DebugUtilsMessageSeverityFlagBitsEXT severity,
            vk::DebugUtilsMessageTypeFlagsEXT,
            const vk::DebugUtilsMessengerCallbackDataEXT &data
          ) {
            std::cout << std::string( nlohmann::json( severity ) ) << " : " << ( data.pMessageIdName ? data.pMessageIdName : "(none)" ) << "[" <<  data.messageIdNumber << "]" << ( data.pMessage ? data.pMessage : "(none)" ) << std::endl;
	    if( int( severity ) & int( vk::DebugUtilsMessageSeverityFlagBitsEXT::eError ) ) {
              std::abort();
	    }
          }
        );
      }
    }
  }
}

