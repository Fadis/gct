#include <nlohmann/json.hpp>
#include <gct/queue.hpp>
#include <gct/device.hpp>
#include <gct/physical_device.hpp>
#include <gct/surface.hpp>
#include <gct/instance.hpp>
#include <gct/format.hpp>
#include <gct/display_surface_create_info.hpp>
namespace gct {
  void to_json( nlohmann::json &root, const physical_device_t &v ) {
    root = nlohmann::json::object();
    root[ "props" ] = v.get_props();
    root[ "features" ] = v.get_features();
  }
  
  void to_json( nlohmann::json &root, const physical_devices_t &v ) {
    root = nlohmann::json::array();
    for( const auto &e: v ) {
      if( e )
        root.push_back( nlohmann::json( *e ) );
      else
        root.push_back( nullptr );
    }
  }

  void to_json( nlohmann::json &root, const device_group_t &v ) {
    root = nlohmann::json::object();
    root[ "devices" ] = v.devices;
    root[ "subset_allocation" ] = v.subset_allocation;
  }

  physical_device_t::physical_device_t(
    const std::shared_ptr< instance_t > &instance_,
    vk::PhysicalDevice handle_,
    const std::vector< const char* > &device_layers
  ) :
    created_from< instance_t >( instance_ ),
    handle( handle_ ),
    props(
      *instance_,
      handle_,
      device_layers
    ),
    features(
      *instance_,
      handle_,
      props
    ) {
      detect_vertex_buffer_formats();
    }
  physical_device_t::physical_device_t(
    const std::shared_ptr< instance_t > &instance_,
    vk::PhysicalDevice handle_,
    const std::vector< const char* > &device_layers,
    const std::vector< const char* > &device_exts
  ) :
    created_from< instance_t >( instance_ ),
    handle( handle_ ),
    props(
      *instance_,
      handle_,
      device_layers,
      device_exts
    ),
    features(
      *instance_,
      handle_,
      props
    ) {
      detect_vertex_buffer_formats();
    }
  void physical_device_t::detect_vertex_buffer_formats() {
    for( const auto &format: get_all_formats( props.get_basic().apiVersion, props.get_available_extensions() ) ) {
      const auto props = handle.getFormatProperties( format );
      if( props.bufferFeatures & vk::FormatFeatureFlagBits::eVertexBuffer ) {
        vertex_buffer_formats.insert( format );
      }
    }
  }
  physical_device_t physical_device_t::with_extensions(
    const std::vector< const char* > &exts
  ) {
    return physical_device_t(
      get_factory(),
      handle,
      props.get_activated_layer_names(),
      exts
    );
  }
  device_group_t device_group_t::with_extensions(
    const std::vector< const char* > &exts
  ) {
    device_group_t group;
    group.subset_allocation = subset_allocation;
    std::transform(
      devices.begin(),
      devices.end(),
      std::back_inserter( group.devices ),
      [&]( const auto &d ) {
        return std::shared_ptr< physical_device_t >(
          new physical_device_t(
            d->with_extensions( exts )
          )
        );
      }
    );
    return group;
  }
  std::shared_ptr< device_t > device_group_t::create_device(
    const std::vector< queue_requirement_t > &queue_requirements,
    const device_create_info_t &create_info
  ) {
    auto [activated_queue_mappings,activated_queue_family_counts] =
      gct::generate_queue_map(
        **devices[ 0 ],
        devices[ 0 ]->get_props(),
        queue_requirements
      );
    return std::shared_ptr< device_t >(
      new device_t(
        *this,
        activated_queue_mappings,
        activated_queue_family_counts,
        create_info
      )
    );
  }
#if defined(VK_KHR_SURFACE_EXTENSION_NAME) && defined(VK_KHR_DISPLAY_EXTENSION_NAME)
  std::shared_ptr< surface_t > physical_device_t::get_surface(
    const display_surface_create_info_t &create_info
  ) {
    auto ci = create_info;
    ci.rebuild_chain();
    auto raw = (*get_factory())->createDisplayPlaneSurfaceKHRUnique(
      ci.get_basic()
    );
    return std::make_shared< surface_t >(
      *this,
      std::move( raw )
    );
  }
#endif
}

