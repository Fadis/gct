#include <nlohmann/json.hpp>
#include <gct/to_json.hpp>
#include <gct/instance.hpp>
#include <gct/physical_device.hpp>
#include <gct/physical_device_properties.hpp>
#include <gct/queue_family_properties.hpp>
#include <vulkan2json/PhysicalDeviceLimits.hpp>
#include <vulkan2json/PhysicalDeviceSparseProperties.hpp>
#include <vulkan2json/PhysicalDeviceProperties.hpp>
#include <vulkan2json/LayerProperties.hpp>
#ifdef VK_KHR_DISPLAY_EXTENSION_NAME
#include <vulkan2json/DisplayPropertiesKHR.hpp>
#include <vulkan2json/DisplayModeParametersKHR.hpp>
#include <vulkan2json/DisplayModePropertiesKHR.hpp>
#include <vulkan2json/DisplayPlaneCapabilitiesKHR.hpp>
#endif
#ifdef VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceAccelerationStructurePropertiesKHR.hpp>
#endif

#ifdef VK_EXT_BLEND_OPERATION_ADVANCED_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceBlendOperationAdvancedPropertiesEXT.hpp>
#endif
#ifdef VK_EXT_CONSERVATIVE_RASTERIZATION_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceConservativeRasterizationPropertiesEXT.hpp>
#endif
#ifdef VK_NV_COOPERATIVE_MATRIX_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceCooperativeMatrixPropertiesNV.hpp>
#endif
#ifdef VK_EXT_CUSTOM_BORDER_COLOR_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceCustomBorderColorPropertiesEXT.hpp>
#endif
#ifdef VK_VERSION_1_2
#include <vulkan2json/PhysicalDeviceDepthStencilResolveProperties.hpp>
#elif defined(VK_KHR_DEPTH_STENCIL_RESOLVE_EXTENSION_NAME)
#include <vulkan2json/PhysicalDeviceDepthStencilResolvePropertiesKHR.hpp>
#endif
#ifdef VK_VERSION_1_2
#include <vulkan2json/PhysicalDeviceDescriptorIndexingProperties.hpp>
#elif defined(VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME)
#include <vulkan2json/PhysicalDeviceDescriptorIndexingPropertiesEXT.hpp>
#endif
#ifdef VK_NV_DEVICE_GENERATED_COMMANDS_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceDeviceGeneratedCommandsPropertiesNV.hpp>
#endif
#ifdef VK_EXT_DISCARD_RECTANGLES_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceDiscardRectanglePropertiesEXT.hpp>
#endif
#ifdef VK_VERSION_1_2
#include <vulkan2json/PhysicalDeviceDriverProperties.hpp>
#elif defined(VK_KHR_DRIVER_PROPERTIES_EXTENSION_NAME)
#include <vulkan2json/PhysicalDeviceDriverPropertiesKHR.hpp>
#endif
#ifdef VK_EXT_PHYSICAL_DEVICE_DRM_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceDrmPropertiesEXT.hpp>
#endif
#ifdef VK_EXT_EXTERNAL_MEMORY_HOST_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceExternalMemoryHostPropertiesEXT.hpp>
#endif
#ifdef VK_VERSION_1_2
#include <vulkan2json/PhysicalDeviceFloatControlsProperties.hpp>
#elif defined(VK_KHR_SHADER_FLOAT_CONTROLS_EXTENSION_NAME)
#include <vulkan2json/PhysicalDeviceFloatControlsPropertiesKHR.hpp>
#endif
#ifdef VK_EXT_FRAGMENT_DENSITY_MAP_2_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceFragmentDensityMap2PropertiesEXT.hpp>
#endif
#ifdef VK_EXT_FRAGMENT_DENSITY_MAP_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceFragmentDensityMapPropertiesEXT.hpp>
#endif
#ifdef VK_NV_FRAGMENT_SHADING_RATE_ENUMS_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceFragmentShadingRateEnumsPropertiesNV.hpp>
#endif
#ifdef VK_KHR_FRAGMENT_SHADING_RATE_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceFragmentShadingRatePropertiesKHR.hpp>
#endif


#ifdef VK_VERSION_1_1
#include <vulkan2json/PhysicalDeviceIDProperties.hpp>
#elif defined(VK_KHR_external_memory_capabilities) || defined(VK_KHR_EXTERNAL_SEMAPHORE_CAPABILITIES_EXTENSION_NAME) || defined(VK_KHR_EXTERNAL_FENCE_CAPABILITIES_EXTENSION_NAME)
#include <vulkan2json/PhysicalDeviceIDPropertiesKHR.hpp>
#endif
#ifdef VK_EXT_INLINE_UNIFORM_BLOCK_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceInlineUniformBlockPropertiesEXT.hpp>
#endif
#ifdef VK_EXT_LINE_RASTERIZATION_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceLineRasterizationPropertiesEXT.hpp>
#endif
#ifdef VK_VERSION_1_1
#include <vulkan2json/PhysicalDeviceMaintenance3Properties.hpp>
#elif defined(VK_KHR_MAINTENANCE3_EXTENSION_NAME)
#include <vulkan2json/PhysicalDeviceMaintenance3PropertiesKHR.hpp>
#endif
#ifdef VK_NV_MESH_SHADER_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceMeshShaderPropertiesNV.hpp>
#endif
#ifdef VK_EXT_MULTI_DRAW_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceMultiDrawPropertiesEXT.hpp>
#endif
#ifdef VK_NVX_MULTIVIEW_PER_VIEW_ATTRIBUTES_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceMultiviewPerViewAttributesPropertiesNVX.hpp>
#endif
#ifdef VK_VERSION_1_1
#include <vulkan2json/PhysicalDeviceMultiviewProperties.hpp>
#elif defined(VK_KHR_MULTIVIEW_EXTENSION_NAME)
#include <vulkan2json/PhysicalDeviceMultiviewPropertiesKHR.hpp>
#endif
#ifdef VK_EXT_PCI_BUS_INFO_EXTENSION_NAME
#include <vulkan2json/PhysicalDevicePCIBusInfoPropertiesEXT.hpp>
#endif
#ifdef VK_KHR_PERFORMANCE_QUERY_EXTENSION_NAME
#include <vulkan2json/PhysicalDevicePerformanceQueryPropertiesKHR.hpp>
#endif
#ifdef VK_VERSION_1_1
#include <vulkan2json/PhysicalDevicePointClippingProperties.hpp>
#elif defined(VK_KHR_MAINTENANCE2_EXTENSION_NAME)
#include <vulkan2json/PhysicalDevicePointClippingPropertiesKHR.hpp>
#endif
#ifdef VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME
#include <vulkan2json/PhysicalDevicePortabilitySubsetPropertiesKHR.hpp>
#endif
#ifdef VK_VERSION_1_1
#include <vulkan2json/PhysicalDeviceProtectedMemoryProperties.hpp>
#endif
#ifdef VK_EXT_PROVOKING_VERTEX_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceProvokingVertexPropertiesEXT.hpp>
#endif
#ifdef VK_KHR_PUSH_DESCRIPTOR_EXTENSION_NAME
#include <vulkan2json/PhysicalDevicePushDescriptorPropertiesKHR.hpp>
#endif

#ifdef VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceRayTracingPipelinePropertiesKHR.hpp>
#endif
#ifdef VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceRayTracingPropertiesNV.hpp>
#endif
#ifdef VK_EXT_ROBUSTNESS_2_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceRobustness2PropertiesEXT.hpp>
#endif
#ifdef VK_EXT_SAMPLE_LOCATIONS_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceSampleLocationsPropertiesEXT.hpp>
#endif
#ifdef VK_VERSION_1_2
#include <vulkan2json/PhysicalDeviceSamplerFilterMinmaxProperties.hpp>
#elif defined(VK_EXT_SAMPLER_FILTER_MINMAX_EXTENSION_NAME)
#include <vulkan2json/PhysicalDeviceSamplerFilterMinmaxPropertiesEXT.hpp>
#endif
#ifdef VK_AMD_SHADER_CORE_PROPERTIES_2_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceShaderCoreProperties2AMD.hpp>
#endif
#ifdef VK_AMD_SHADER_CORE_PROPERTIES_2_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceShaderCorePropertiesAMD.hpp>
#endif
#ifdef VK_KHR_SHADER_INTEGER_DOT_PRODUCT_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceShaderIntegerDotProductPropertiesKHR.hpp>
#endif
#ifdef VK_NV_SHADER_SM_BUILTINS_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceShaderSMBuiltinsPropertiesNV.hpp>
#endif
#ifdef VK_NV_SHADING_RATE_IMAGE_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceShadingRateImagePropertiesNV.hpp>
#endif
#ifdef VK_VERSION_1_1
#include <vulkan2json/PhysicalDeviceSubgroupProperties.hpp>
#endif
#ifdef VK_EXT_SUBGROUP_SIZE_CONTROL_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceSubgroupSizeControlPropertiesEXT.hpp>
#endif
#ifdef VK_HUAWEI_SUBPASS_SHADING_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceSubpassShadingPropertiesHUAWEI.hpp>
#endif
#ifdef VK_EXT_TEXEL_BUFFER_ALIGNMENT_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceTexelBufferAlignmentPropertiesEXT.hpp>
#endif
#ifdef VK_VERSION_1_2
#include <vulkan2json/PhysicalDeviceTimelineSemaphoreProperties.hpp>
#elif defined(VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME)
#include <vulkan2json/PhysicalDeviceTimelineSemaphorePropertiesKHR.hpp>
#endif
#ifdef VK_EXT_TRANSFORM_FEEDBACK_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceTransformFeedbackPropertiesEXT.hpp>
#endif
#ifdef VK_EXT_VERTEX_ATTRIBUTE_DIVISOR_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceVertexAttributeDivisorPropertiesEXT.hpp>
#endif

namespace gct {
  void to_json( nlohmann::json &root, const layer_map_t &v ) {
    root = nlohmann::json::object();
    for( const auto &[key,value]: v )
      root[ key ] = value;
  }
  void to_json( nlohmann::json &root, const extension_map_t &v ) {
    root = nlohmann::json::object();
    for( const auto &[key,value]: v ) {
      root[ key ] = nlohmann::json::object();
      root[ key ][ "specVersion" ] = value;
    }
  }

#ifdef VK_KHR_DISPLAY_EXTENSION_NAME
  void to_json( nlohmann::json &root, const display_plane_t &v ) {
    root = nlohmann::json::object();
    root[ "basic" ] = v.basic;
  }
  void to_json( nlohmann::json &root, const display_mode_properties_t &v ) {
    root = nlohmann::json::object();
    root[ "basic" ] = v.basic;
    root[ "capabilities" ] = v.capabilities;
  }
  void to_json( nlohmann::json &root, const display_properties_t &v ) {
    root = nlohmann::json::object();
    root[ "basic" ] = v.basic;
    root[ "modes" ] = v.modes;
  }
#endif

  void to_json( nlohmann::json &root, const physical_device_properties_t &v ) {
    root = nlohmann::json::object();
    root[ "basic" ] = v.get_basic();
    root[ "available_extensions" ] = v.get_available_extensions();
    root[ "available_layers" ] = v.get_available_layers();
    root[ "activated_layers" ] = v.get_activated_layers();
    LIBGCT_EXTENSION_TO_JSON( memory_props )
#ifdef VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( acceleration_structure )
#endif
#ifdef VK_EXT_BLEND_OPERATION_ADVANCED_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( blend_operation )
#endif
#ifdef VK_EXT_CONSERVATIVE_RASTERIZATION_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( conservative_rasterization )
#endif
#ifdef VK_NV_COOPERATIVE_MATRIX_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( cooperative_matrix )
#endif
#ifdef VK_EXT_CUSTOM_BORDER_COLOR_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( custom_border_color )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_DEPTH_STENCIL_RESOLVE_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( depth_stencil_resolve )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( device_descriptor_indexing )
#endif
#ifdef VK_NV_DEVICE_GENERATED_COMMANDS_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( device_generated_commands )
#endif
#ifdef VK_EXT_DISCARD_RECTANGLES_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( discard_rectangle )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_DRIVER_PROPERTIES_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( driver )
#endif
#ifdef VK_EXT_PHYSICAL_DEVICE_DRM_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( drm )
#endif
#ifdef VK_EXT_EXTERNAL_MEMORY_HOST_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( external_memory_host )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_SHADER_FLOAT_CONTROLS_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( float_controls )
#endif
#ifdef VK_EXT_FRAGMENT_DENSITY_MAP_2_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( fragment_density_map2 )
#endif
#ifdef VK_EXT_FRAGMENT_DENSITY_MAP_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( fragment_density_map )
#endif
#ifdef VK_NV_FRAGMENT_SHADING_RATE_ENUMS_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( fragment_shading_rate_enums )
#endif
#ifdef VK_KHR_FRAGMENT_SHADING_RATE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( fragment_shading_rate )
#endif
#if defined(VK_VERSION_1_1) || defined(VK_KHR_external_memory_capabilities) || defined(VK_KHR_EXTERNAL_SEMAPHORE_CAPABILITIES_EXTENSION_NAME) || defined(VK_KHR_EXTERNAL_FENCE_CAPABILITIES_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( id )
#endif
#ifdef VK_EXT_INLINE_UNIFORM_BLOCK_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( inline_uniform_block )
#endif
#ifdef VK_EXT_LINE_RASTERIZATION_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( line_rasterization )
#endif
#if defined(VK_VERSION_1_1) || defined(VK_KHR_MAINTENANCE3_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( maintenance3 )
#endif
#ifdef VK_NV_MESH_SHADER_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( mesh_shader )
#endif
#ifdef VK_EXT_MULTI_DRAW_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( multi_draw )
#endif
#ifdef VK_NVX_MULTIVIEW_PER_VIEW_ATTRIBUTES_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( multiview_per_view_attributes )
#endif
#if defined(VK_VERSION_1_1) || defined(VK_KHR_MULTIVIEW_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( multiview )
#endif
#ifdef VK_EXT_PCI_BUS_INFO_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( pci_bus_info )
#endif
#ifdef VK_KHR_PERFORMANCE_QUERY_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( performance_query )
#endif
#if defined(VK_VERSION_1_1) || defined(VK_KHR_MAINTENANCE2_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( point_clipping )
#endif
#ifdef VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( portability_subset )
#endif
#if VK_VERSION_1_1
    LIBGCT_EXTENSION_TO_JSON( protected_memory )
#endif
#ifdef VK_EXT_PROVOKING_VERTEX_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( provoking_vertex )
#endif
#ifdef VK_KHR_PUSH_DESCRIPTOR_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( push_descriptor )
#endif
#ifdef VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( ray_tracing_pipeline )
#endif
#ifdef VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( ray_tracing )
#endif
#ifdef VK_EXT_ROBUSTNESS_2_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( robustness2 )
#endif
#ifdef VK_EXT_SAMPLE_LOCATIONS_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( sample_locations )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_EXT_SAMPLER_FILTER_MINMAX_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( sampler_filter_minmax )
#endif
#ifdef VK_AMD_SHADER_CORE_PROPERTIES_2_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( shader_core2 )
#endif
#ifdef VK_AMD_SHADER_CORE_PROPERTIES_2_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( shader_core )
#endif
#ifdef VK_KHR_SHADER_INTEGER_DOT_PRODUCT_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( shader_integer_dot_product )
#endif
#ifdef VK_NV_SHADER_SM_BUILTINS_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( shader_sm_builtins )
#endif
#ifdef VK_NV_SHADING_RATE_IMAGE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( shading_rate_image )
#endif
#ifdef VK_VERSION_1_1
    LIBGCT_EXTENSION_TO_JSON( subgroup )
#endif
#ifdef VK_EXT_SUBGROUP_SIZE_CONTROL_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( subgroup_size_control )
#endif
#ifdef VK_HUAWEI_SUBPASS_SHADING_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( subpass_shading )
#endif
#ifdef VK_EXT_TEXEL_BUFFER_ALIGNMENT_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( texel_buffer_alignment )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( timeline_semaphore )
#endif
#ifdef VK_EXT_TRANSFORM_FEEDBACK_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( transform_feedback )
#endif
#ifdef VK_EXT_VERTEX_ATTRIBUTE_DIVISOR_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( vertex_attribute_divisor )
#endif



    root[ "queue_family" ] = nlohmann::json::array();
    for( auto &q: v.get_queue_family() )
      root[ "queue_family" ].push_back( queue_family_properties_t( q ) );
#ifdef VK_KHR_DISPLAY_EXTENSION_NAME
    root[ "display" ] = v.get_display();
#endif
  }
  
  physical_device_properties_t::physical_device_properties_t(
    instance_t &instance,
    vk::PhysicalDevice &pdev,
    const std::vector< const char* > &layers
  ) : api_version( instance.get_api_version() ) {
    available_layers = get_device_layers( pdev );
    activated_layer_names = layers;
    for( const auto &l: layers ) {
      auto found = available_layers.find( l );
      if( found != available_layers.end() ) {
        activated_layers.insert( *found );
        activated_layer_names.push_back( l );
      }
    }
    available_extensions = get_device_extensions( pdev, activated_layer_names );
    activated_extensions = available_extensions;
    std::transform(
      activated_extensions.begin(),
      activated_extensions.end(),
      std::back_inserter( activated_extension_names ),
      []( const auto &v ) {
        return v.first.data();
      }
    );
    load( instance, pdev, layers );
  }

  physical_device_properties_t::physical_device_properties_t(
    instance_t &instance,
    vk::PhysicalDevice &pdev,
    const std::vector< const char* > &layers,
    const std::vector< const char* > &exts
  ) {
    available_layers = get_device_layers( pdev );
    activated_layer_names = layers;
    activated_extension_names = exts;
    for( const auto &l: layers ) {
      auto found = available_layers.find( l );
      if( found != available_layers.end() ) {
        activated_layers.insert( *found );
      }
    }
    available_extensions = get_device_extensions( pdev, activated_layer_names );
    for( const auto &e: exts ) {
      const auto available = available_extensions.find( e );
      if( available == available_extensions.end() )
        throw -1;
      activated_extensions.insert( *available );
    }
    load( instance, pdev, layers );
  }

  void physical_device_properties_t::load(
    instance_t &instance,
    vk::PhysicalDevice &pdev,
    const std::vector< const char* > &layers
  ) {
#if defined(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME) || defined(VK_VERSION_1_1)
    if(
      instance.get_api_version() >= VK_MAKE_VERSION( 1, 1, 0 ) ||
      instance.get_activated_extensions().find( "VK_KHR_get_physical_device_properties2" ) != instance.get_activated_extensions().end()
    ) {
      const auto &ext = activated_extensions;
#ifdef VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( acceleration_structure, "VK_KHR_acceleration_structure" )
#endif
#ifdef VK_EXT_BLEND_OPERATION_ADVANCED_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( blend_operation, "VK_EXT_blend_operation_advanced" )
#endif
#ifdef VK_EXT_CONSERVATIVE_RASTERIZATION_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( conservative_rasterization, "VK_EXT_conservative_rasterization" )
#endif
#ifdef VK_NV_COOPERATIVE_MATRIX_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( cooperative_matrix, "VK_NV_cooperative_matrix" )
#endif
#ifdef VK_EXT_CUSTOM_BORDER_COLOR_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( custom_border_color, "VK_EXT_custom_border_color" )
#endif
#if defined(VK_KHR_DEPTH_STENCIL_RESOLVE_EXTENSION_NAME) || defined(VK_VERSION_1_2)
      LIBGCT_EXTENSION_CREATE_IF_VERSION_OR_EXT( depth_stencil_resolve, 1, 2, 0, "VK_KHR_depth_stencil_resolve" )
#endif
#if defined(VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME) || defined(VK_VERSION_1_2)
      LIBGCT_EXTENSION_CREATE_IF_VERSION_OR_EXT( device_descriptor_indexing, 1, 2, 0, "VK_EXT_descriptor_indexing" )
#endif
#ifdef VK_NV_DEVICE_GENERATED_COMMANDS_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( device_generated_commands, "VK_NV_device_generated_commands" )
#endif
#ifdef VK_EXT_DISCARD_RECTANGLES_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( discard_rectangle, "VK_EXT_discard_rectangles" )
#endif
#if defined(VK_KHR_DRIVER_PROPERTIES_EXTENSION_NAME) || defined(VK_VERSION_1_2)
      LIBGCT_EXTENSION_CREATE_IF_VERSION_OR_EXT( driver, 1, 2, 0, "VK_KHR_driver_properties" )
#endif
#ifdef VK_EXT_PHYSICAL_DEVICE_DRM_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( drm, "VK_EXT_physical_device_drm" )
#endif
#ifdef VK_EXT_EXTERNAL_MEMORY_HOST_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( external_memory_host, "VK_EXT_external_memory_host" )
#endif
#if defined(VK_KHR_SHADER_FLOAT_CONTROLS_EXTENSION_NAME) || defined(VK_VERSION_1_2)
      LIBGCT_EXTENSION_CREATE_IF_VERSION_OR_EXT( float_controls, 1, 2, 0, "VK_KHR_shader_float_controls" )
#endif
#ifdef VK_EXT_FRAGMENT_DENSITY_MAP_2_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( fragment_density_map2, "VK_EXT_fragment_density_map2" )
#endif
#ifdef VK_EXT_FRAGMENT_DENSITY_MAP_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( fragment_density_map, "VK_EXT_fragment_density_map" )
#endif
#ifdef VK_NV_FRAGMENT_SHADING_RATE_ENUMS_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( fragment_shading_rate_enums, "VK_NV_fragment_shading_rate_enums" )
#endif
#ifdef VK_KHR_FRAGMENT_SHADING_RATE_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( fragment_shading_rate, "VK_KHR_fragment_shading_rate" )
#endif
#if defined(VK_KHR_external_memory_capabilities) || defined(VK_VERSION_1_1) || defined(VK_KHR_EXTERNAL_SEMAPHORE_CAPABILITIES_EXTENSION_NAME) || defined(VK_KHR_EXTERNAL_FENCE_CAPABILITIES_EXTENSION_NAME)
      LIBGCT_EXTENSION_CREATE_IF_VERSION_OR_EXT( id, 1, 1, 0, "VK_KHR_external_memory_capabilities" )
      else
      LIBGCT_EXTENSION_CREATE_IF_EXT( id, "VK_KHR_external_semaphore_capabilities" )
      else
      LIBGCT_EXTENSION_CREATE_IF_EXT( id, "VK_KHR_external_fence_capabilities" )
#endif
#ifdef VK_EXT_INLINE_UNIFORM_BLOCK_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( inline_uniform_block, "VK_EXT_inline_uniform_block" )
#endif
#ifdef VK_EXT_LINE_RASTERIZATION_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( line_rasterization, "VK_EXT_line_rasterization" )
#endif
#if defined(VK_KHR_MAINTENANCE3_EXTENSION_NAME) || defined(VK_VERSION_1_1)
      LIBGCT_EXTENSION_CREATE_IF_VERSION_OR_EXT( maintenance3, 1, 1, 0, "VK_KHR_maintenance3" )
#endif
#ifdef VK_NV_MESH_SHADER_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( mesh_shader, "VK_NV_mesh_shader" )
#endif
#ifdef VK_EXT_MULTI_DRAW_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( multi_draw, "VK_EXT_multi_draw" )
#endif
#ifdef VK_NVX_MULTIVIEW_PER_VIEW_ATTRIBUTES_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( multiview_per_view_attributes, "VK_NVX_multiview_per_view_attributes" )
#endif
#if defined(VK_KHR_MULTIVIEW_EXTENSION_NAME) || defined(VK_VERSION_1_1)
      LIBGCT_EXTENSION_CREATE_IF_VERSION_OR_EXT( multiview, 1, 1, 0, "VK_KHR_multiview" )
#endif
#ifdef VK_EXT_PCI_BUS_INFO_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( pci_bus_info, "VK_EXT_pci_bus_info" )
#endif
#ifdef VK_KHR_PERFORMANCE_QUERY_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( performance_query, "VK_KHR_performance_query" )
#endif
#if defined(VK_KHR_MAINTENANCE2_EXTENSION_NAME) || defined(VK_VERSION_1_1)
      LIBGCT_EXTENSION_CREATE_IF_VERSION_OR_EXT( point_clipping, 1, 1, 0, "VK_KHR_maintenance2" )
#endif
#ifdef VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( portability_subset, "VK_KHR_portability_subset" )
#endif
#ifdef VK_VERSION_1_1
      LIBGCT_EXTENSION_CREATE_IF_VERSION( protected_memory, 1, 1, 0 )
#endif
#ifdef VK_EXT_PROVOKING_VERTEX_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( provoking_vertex, "VK_EXT_provoking_vertex" )
#endif
#ifdef VK_KHR_PUSH_DESCRIPTOR_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( push_descriptor, "VK_KHR_push_descriptor" )
#endif
#ifdef VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( ray_tracing_pipeline, "VK_KHR_ray_tracing_pipeline" )
#endif
#ifdef VK_NV_RAY_TRACING_MOTION_BLUR_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( ray_tracing, "VK_NV_ray_tracing" )
#endif
#ifdef VK_EXT_ROBUSTNESS_2_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( robustness2, "VK_EXT_robustness2" )
#endif
#ifdef VK_EXT_SAMPLE_LOCATIONS_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( sample_locations, "VK_EXT_sample_locations" )
#endif
#if defined(VK_EXT_SAMPLER_FILTER_MINMAX_EXTENSION_NAME) || defined(VK_VERSION_1_2)
      LIBGCT_EXTENSION_CREATE_IF_VERSION_OR_EXT( sampler_filter_minmax, 1, 2, 0, "VK_EXT_sampler_filter_minmax" )
#endif
#ifdef VK_AMD_SHADER_CORE_PROPERTIES_2_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( shader_core2, "VK_AMD_shader_core_properties2" )
#endif
#ifdef VK_KHR_SHADER_INTEGER_DOT_PRODUCT_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( shader_integer_dot_product, "VK_KHR_shader_integer_dot_product" )
#endif
#ifdef VK_NV_SHADER_SM_BUILTINS_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( shader_sm_builtins, "VK_NV_shader_sm_builtins" )
#endif
#ifdef VK_NV_SHADING_RATE_IMAGE_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( shading_rate_image, "VK_NV_shading_rate_image" )
#endif
#ifdef VK_VERSION_1_1
      LIBGCT_EXTENSION_CREATE_IF_VERSION( subgroup, 1, 1, 0 )
#endif
#ifdef VK_EXT_SUBGROUP_SIZE_CONTROL_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( subgroup_size_control, "VK_EXT_subgroup_size_control" )
#endif
#ifdef VK_HUAWEI_SUBPASS_SHADING_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( subpass_shading, "VK_HUAWEI_subpass_shading" )
#endif
#ifdef VK_EXT_TEXEL_BUFFER_ALIGNMENT_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( texel_buffer_alignment, "VK_EXT_texel_buffer_alignment" )
#endif
#if defined(VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME) || defined(VK_VERSION_1_2)
      LIBGCT_EXTENSION_CREATE_IF_VERSION_OR_EXT( timeline_semaphore, 1, 2, 0, "VK_KHR_timeline_semaphore" )
#endif
#ifdef VK_EXT_TRANSFORM_FEEDBACK_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( transform_feedback, "VK_EXT_transform_feedback" )
#endif
#ifdef VK_EXT_VERTEX_ATTRIBUTE_DIVISOR_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( vertex_attribute_divisor, "VK_EXT_vertex_attribute_divisor" )
#endif
      rebuild_chain();
#ifdef VK_VERSION_1_1
      pdev.getProperties2(
        &basic
      );
#elif defined(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME)
      pdev.getProperties2KHR(
        &basic
      );
#endif
      {
        std::uint32_t queue_family_count = 0u;
        pdev.getQueueFamilyProperties2( &queue_family_count, nullptr );
        queue_family.resize( queue_family_count );
        for( auto &q : queue_family )
          q.activate( activated_extensions ).rebuild_chain();
        std::vector< vk::QueueFamilyProperties2 > root;
        root.reserve( queue_family_count );
        std::transform(
          queue_family.begin(),
          queue_family.end(),
          std::back_inserter( root ),
          []( const auto &v ) { return v.get_basic2(); }
        );
        pdev.getQueueFamilyProperties2( &queue_family_count, root.data() );
        std::copy(
          root.begin(),
          root.end(),
          queue_family.begin()
        );
      }
    }
    else {
      pdev.getProperties(
        &basic.properties
      );
      {
        std::uint32_t queue_family_count = 0u;
        pdev.getQueueFamilyProperties2( &queue_family_count, nullptr );
        queue_family.resize( queue_family_count );
        std::vector< vk::QueueFamilyProperties > root( queue_family_count );
        pdev.getQueueFamilyProperties( &queue_family_count, root.data() );
        std::copy(
          root.begin(),
          root.end(),
          queue_family.begin()
        );
      }
    }
#else
    {
      pdev.getProperties(
        &basic
      );
      {
        std::uint32_t queue_family_count = 0u;
        pdev.getQueueFamilyProperties2( &queue_family_count, nullptr );
        queue_family.resize( queue_family_count );
        std::vector< vk::QueueFamilyProperties > root( queue_family_count );
        pdev.getQueueFamilyProperties( &queue_family_count, root.data() );
        std::copy(
          root.begin(),
          root.end(),
          queue_family.begin()
        );
      }
    }
#endif
#ifdef VK_KHR_DISPLAY_EXTENSION_NAME
    if( instance.get_activated_extensions().find( "VK_KHR_display" ) != instance.get_activated_extensions().end() ) {
      for( const auto &d: pdev.getDisplayPropertiesKHR() ) {
        display.push_back(
          display_properties_t()
        );
        display.back().basic = d;
        for( const auto &m: pdev.getDisplayModePropertiesKHR( d.display ) ) {
          display.back().modes.push_back(
            display_mode_properties_t()
          );
          display.back().modes.back().basic = m;
        }
      }
      std::uint32_t plane_index = 0u;
      for( const auto &p: pdev.getDisplayPlanePropertiesKHR() ) {
        for( auto &d: display ) {
          if( p.currentDisplay == d.basic.display ) {
            d.planes.push_back( plane_index );
          }
        }
        ++plane_index;
      }
      for( auto &d: display ) {
        for( auto &m: d.modes ) {
          for( const auto &p: d.planes ) {
            auto iter = m.capabilities.insert(
              std::make_pair(
                p,
                display_plane_t()
              )
            ).first;
            iter->second.basic = pdev.getDisplayPlaneCapabilitiesKHR( m.basic.displayMode, p );
          }
        }
      }
    }
#endif
    memory_props.reset( new physical_device_memory_properties_t(
      instance,
      pdev,
      activated_extensions
    ) );
  }
  
  physical_device_properties_t &physical_device_properties_t::rebuild_chain() {
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
#ifdef VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( acceleration_structure )
#endif
#ifdef VK_EXT_BLEND_OPERATION_ADVANCED_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( blend_operation )
#endif
#ifdef VK_EXT_CONSERVATIVE_RASTERIZATION_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( conservative_rasterization )
#endif
#ifdef VK_NV_COOPERATIVE_MATRIX_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( cooperative_matrix )
#endif
#ifdef VK_EXT_CUSTOM_BORDER_COLOR_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( custom_border_color )
#endif
#if defined(VK_KHR_DEPTH_STENCIL_RESOLVE_EXTENSION_NAME) || defined(VK_VERSION_1_2)
      LIBGCT_EXTENSION_REBUILD_CHAIN( depth_stencil_resolve )
#endif
#if defined(VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME) || defined(VK_VERSION_1_2)
      LIBGCT_EXTENSION_REBUILD_CHAIN( device_descriptor_indexing )
#endif
#ifdef VK_NV_DEVICE_GENERATED_COMMANDS_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( device_generated_commands )
#endif
#ifdef VK_EXT_DISCARD_RECTANGLES_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( discard_rectangle )
#endif
#if defined(VK_KHR_DRIVER_PROPERTIES_EXTENSION_NAME) || defined(VK_VERSION_1_2)
      LIBGCT_EXTENSION_REBUILD_CHAIN( driver )
#endif
#ifdef VK_EXT_PHYSICAL_DEVICE_DRM_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( drm )
#endif
#ifdef VK_EXT_EXTERNAL_MEMORY_HOST_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( external_memory_host )
#endif
#if defined(VK_KHR_SHADER_FLOAT_CONTROLS_EXTENSION_NAME) || defined(VK_VERSION_1_2)
      LIBGCT_EXTENSION_REBUILD_CHAIN( float_controls )
#endif
#ifdef VK_EXT_FRAGMENT_DENSITY_MAP_2_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( fragment_density_map2 )
#endif
#ifdef VK_EXT_FRAGMENT_DENSITY_MAP_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( fragment_density_map )
#endif
#ifdef VK_NV_FRAGMENT_SHADING_RATE_ENUMS_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( fragment_shading_rate_enums )
#endif
#ifdef VK_KHR_FRAGMENT_SHADING_RATE_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( fragment_shading_rate )
#endif
#if defined(VK_KHR_external_memory_capabilities) || defined(VK_VERSION_1_1) || defined(VK_KHR_EXTERNAL_SEMAPHORE_CAPABILITIES_EXTENSION_NAME) || defined(VK_KHR_EXTERNAL_FENCE_CAPABILITIES_EXTENSION_NAME)
      LIBGCT_EXTENSION_REBUILD_CHAIN( id )
#endif
#ifdef VK_EXT_INLINE_UNIFORM_BLOCK_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( inline_uniform_block )
#endif
#ifdef VK_EXT_LINE_RASTERIZATION_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( line_rasterization )
#endif
#if defined(VK_KHR_MAINTENANCE3_EXTENSION_NAME) || defined(VK_VERSION_1_1)
      LIBGCT_EXTENSION_REBUILD_CHAIN( maintenance3 )
#endif
#ifdef VK_NV_MESH_SHADER_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( mesh_shader )
#endif
#ifdef VK_EXT_MULTI_DRAW_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( multi_draw )
#endif
#ifdef VK_NVX_MULTIVIEW_PER_VIEW_ATTRIBUTES_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( multiview_per_view_attributes )
#endif
#if defined(VK_KHR_MULTIVIEW_EXTENSION_NAME) || defined(VK_VERSION_1_1)
      LIBGCT_EXTENSION_REBUILD_CHAIN( multiview )
#endif
#ifdef VK_EXT_PCI_BUS_INFO_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( pci_bus_info )
#endif
#ifdef VK_KHR_PERFORMANCE_QUERY_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( performance_query )
#endif
#if defined(VK_KHR_MAINTENANCE2_EXTENSION_NAME) || defined(VK_VERSION_1_1)
      LIBGCT_EXTENSION_REBUILD_CHAIN( point_clipping )
#endif
#ifdef VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( portability_subset )
#endif
#ifdef VK_VERSION_1_1
      LIBGCT_EXTENSION_REBUILD_CHAIN( protected_memory )
#endif
#ifdef VK_EXT_PROVOKING_VERTEX_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( provoking_vertex )
#endif
#ifdef VK_KHR_PUSH_DESCRIPTOR_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( push_descriptor )
#endif
#ifdef VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( ray_tracing_pipeline )
#endif
#ifdef VK_NV_RAY_TRACING_MOTION_BLUR_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( ray_tracing )
#endif
#ifdef VK_EXT_ROBUSTNESS_2_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( robustness2 )
#endif
#ifdef VK_EXT_SAMPLE_LOCATIONS_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( sample_locations )
#endif
#if defined(VK_EXT_SAMPLER_FILTER_MINMAX_EXTENSION_NAME) || defined(VK_VERSION_1_2)
      LIBGCT_EXTENSION_REBUILD_CHAIN( sampler_filter_minmax )
#endif
#ifdef VK_AMD_SHADER_CORE_PROPERTIES_2_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( shader_core2 )
#endif
#ifdef VK_KHR_SHADER_INTEGER_DOT_PRODUCT_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( shader_integer_dot_product )
#endif
#ifdef VK_NV_SHADER_SM_BUILTINS_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( shader_sm_builtins )
#endif
#ifdef VK_NV_SHADING_RATE_IMAGE_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( shading_rate_image )
#endif
#ifdef VK_VERSION_1_1
      LIBGCT_EXTENSION_REBUILD_CHAIN( subgroup )
#endif
#ifdef VK_EXT_SUBGROUP_SIZE_CONTROL_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( subgroup_size_control )
#endif
#ifdef VK_HUAWEI_SUBPASS_SHADING_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( subpass_shading )
#endif
#ifdef VK_EXT_TEXEL_BUFFER_ALIGNMENT_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( texel_buffer_alignment )
#endif
#if defined(VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME) || defined(VK_VERSION_1_2)
      LIBGCT_EXTENSION_REBUILD_CHAIN( timeline_semaphore )
#endif
#ifdef VK_EXT_TRANSFORM_FEEDBACK_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( transform_feedback )
#endif
#ifdef VK_EXT_VERTEX_ATTRIBUTE_DIVISOR_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( vertex_attribute_divisor )
#endif
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }

}

