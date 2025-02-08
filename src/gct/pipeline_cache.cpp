#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <gct/device.hpp>
#include <gct/graphics_pipeline.hpp>
#include <gct/compute_pipeline.hpp>
#include <gct/descriptor_set_layout.hpp>
#include <gct/shader_module_reflection.hpp>
#include <gct/get_device.hpp>
#include <gct/shader_module.hpp>
#ifdef VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME
#include <gct/ray_tracing_pipeline.hpp>
#endif
#include <gct/pipeline_cache.hpp>

namespace gct {
  pipeline_cache_t::pipeline_cache_t(
    const std::shared_ptr< device_t > &device,
    const pipeline_cache_create_info_t &create_info
  ) :
    property_type( create_info ),
    created_from< device_t >( device ) {
    props.rebuild_chain();
    handle = (*device)->createPipelineCacheUnique(
      props.get_basic()
    );
  }
  std::shared_ptr< compute_pipeline_t > pipeline_cache_t::get_pipeline(
    const compute_pipeline_create_info_t &create_info
  ) {
    return std::shared_ptr< compute_pipeline_t >(
      new compute_pipeline_t(
        shared_from_this(),
        create_info
      )
    );
  }
  std::shared_ptr< graphics_pipeline_t > pipeline_cache_t::get_pipeline(
    const graphics_pipeline_create_info_t &create_info
  ) {
    return std::shared_ptr< graphics_pipeline_t >(
      new graphics_pipeline_t(
        shared_from_this(),
        create_info
      )
    );
  }
#ifdef VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME
  std::shared_ptr< ray_tracing_pipeline_t > pipeline_cache_t::get_pipeline(
    const ray_tracing_pipeline_create_info_t &create_info
  ) {
    return std::shared_ptr< ray_tracing_pipeline_t >(
      new ray_tracing_pipeline_t(
        shared_from_this(),
        create_info
      )
    );
  }
#endif
  std::pair<
    std::shared_ptr< descriptor_set_layout_t >,
    std::shared_ptr< compute_pipeline_t >
  >
  pipeline_cache_t::get_pipeline(
    const std::string &path
  ) {
    auto &device = get_device( *this );
    auto shader = device.get_shader_module( path );
    std::unordered_map< unsigned int, std::shared_ptr< descriptor_set_layout_t > > set_layouts;
    auto descriptor_set_layout = device.get_descriptor_set_layout(
      shader->get_props().get_reflection(), 0u
    );
    auto pipeline = get_pipeline(
      gct::compute_pipeline_create_info_t()
        .set_stage( shader )
        .set_layout( descriptor_set_layout, shader )
    ); 
    return std::make_pair( descriptor_set_layout, pipeline );
  }
  std::pair<
    std::unordered_map< unsigned int, std::shared_ptr< descriptor_set_layout_t > >,
    std::shared_ptr< compute_pipeline_t >
  >
  pipeline_cache_t::get_pipeline2(
    const std::string &path,
    std::vector< std::shared_ptr< descriptor_set_layout_t > > &external_descriptor_set_layout
  ) {
    auto &device = get_device( *this );
    auto shader = device.get_shader_module( path );
    std::unordered_map< unsigned int, std::shared_ptr< descriptor_set_layout_t > > set_layouts;
    for( const auto id: shader->get_props().get_reflection().get_descriptor_set_id() ) {
      auto descriptor_set_layout = ( external_descriptor_set_layout.size() <= id ) ? device.get_descriptor_set_layout(
        shader->get_props().get_reflection(), id
      ) : external_descriptor_set_layout[ id ];
      set_layouts.insert( std::make_pair( id, descriptor_set_layout ) );
    }
    auto pipeline = get_pipeline(
      gct::compute_pipeline_create_info_t()
        .set_stage( shader )
        .set_layout( set_layouts, shader )
    ); 
    return std::make_pair( set_layouts, pipeline );
  }
  std::pair<
    std::unordered_map< unsigned int, std::shared_ptr< descriptor_set_layout_t > >,
    std::shared_ptr< compute_pipeline_t >
  >
  pipeline_cache_t::get_pipeline2(
    const std::string &path,
    std::vector< std::shared_ptr< descriptor_set_layout_t > > &external_descriptor_set_layout,
    const specialization_map &specs,
    const glm::ivec3 &dim
  ) {
    auto &device = get_device( *this );
    auto shader = device.get_shader_module( path );
    std::unordered_map< unsigned int, std::shared_ptr< descriptor_set_layout_t > > set_layouts;
    for( const auto id: shader->get_props().get_reflection().get_descriptor_set_id() ) {
      auto descriptor_set_layout = ( external_descriptor_set_layout.size() <= id ) ? device.get_descriptor_set_layout(
        shader->get_props().get_reflection(), id
      ) : external_descriptor_set_layout[ id ];
      set_layouts.insert( std::make_pair( id, descriptor_set_layout ) );
    }
    auto pipeline = get_pipeline(
      gct::compute_pipeline_create_info_t()
        .set_dim( dim )
        .set_stage( shader, specs )
        .set_layout( set_layouts, shader )
    ); 
    return std::make_pair( set_layouts, pipeline );
  }
  void pipeline_cache_t::dump( const std::filesystem::path &filename ) const {
    auto &device = get_device( *this );
    auto data = device->getPipelineCacheData( *handle );
    std::fstream fd( filename.c_str(), std::ios::out|std::ios::binary );
    fd.write( reinterpret_cast< char* >( data.data() ), data.size() );
  }
  void to_json( nlohmann::json &dest, const pipeline_cache_t &src ) {
  }
}
