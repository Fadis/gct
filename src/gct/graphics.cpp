#include <nlohmann/json.hpp>
#include <gct/allocator.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/descriptor_set_layout.hpp>
#include <gct/graphics_pipeline.hpp>
#include <gct/pipeline_layout.hpp>
#include <gct/write_descriptor_set.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/named_resource.hpp>
#include <gct/get_device.hpp>
#include <gct/device.hpp>
#include <gct/spv2vk.hpp>
#include <gct/graphics.hpp>
#include <gct/graphics_create_info.hpp>
#include <gct/shader_module_reflection.hpp>
#include <gct/color_attachment_name.hpp>
#include <gct/numeric_types.hpp>

namespace gct {
  graphics::graphics(
    const graphics_create_info &ci
  ) :
    property_type( ci ) {
    auto &device = get_device( *props.allocator_set.pipeline_cache );
    for( const auto &s: props.shaders ) {
      props.pipeline_create_info.add_stage( device.get_shader_module( s ) );
    }
    for( const auto &s: props.shader_module ) {
      if( s ) {
        props.pipeline_create_info.add_stage( s );
      }
    }
    props.pipeline_create_info.rebuild_chain();
    if( props.external_pipeline_layout ) {
      pipeline_layout = props.external_pipeline_layout;
      std::unordered_map< unsigned int, std::shared_ptr< descriptor_set_layout_t > > set_layouts;
      const auto &dsl = props.external_pipeline_layout->get_props().get_descriptor_set_layout();
      for( unsigned int i = 0u; i != dsl.size(); ++i ) {
        set_layouts.insert( std::make_pair( i, dsl[ i ] ) );
      }
      descriptor_set_layout = set_layouts;
    }
    else {
      std::unordered_map< unsigned int, std::shared_ptr< descriptor_set_layout_t > > set_layouts;
      std::unordered_set< unsigned int > set_ids;
      for( const auto &s: props.pipeline_create_info.get_stage() ) {
        for( const auto id: s.get_shader_module()->get_props().get_reflection().get_descriptor_set_id() ) {
          set_ids.insert( id );
        }
      }
      for( const auto id: set_ids ) {
        if( props.descriptor_set_layout.size() <= id ) {
          gct::descriptor_set_layout_create_info_t dslci;
          for( const auto &s: props.pipeline_create_info.get_stage() ) {
            dslci.add_binding( s.get_shader_module()->get_props().get_reflection(), id );
          }
          auto descriptor_set_layout = device.get_descriptor_set_layout( dslci );
          set_layouts.insert( std::make_pair( id, descriptor_set_layout ) );
        }
        else {
          set_layouts.insert( std::make_pair( id, props.descriptor_set_layout[ id ] ) );
        }
      }
      descriptor_set_layout = set_layouts;
     
      gct::pipeline_layout_create_info_t plci;
      for( const auto &v: descriptor_set_layout ) {
        plci.add_descriptor_set_layout( v.first, v.second );
      }
      for( const auto &s: props.pipeline_create_info.get_stage() ) {
        plci.add_push_constant_range( s.get_shader_module() );
      }
      pipeline_layout = device.get_pipeline_layout( plci );
    }

    const auto vs = std::find_if(
      props.pipeline_create_info.get_stage().begin(),
      props.pipeline_create_info.get_stage().end(),
      []( const auto &s ) -> bool {
        return spv2vk(
          s.get_shader_module()->get_props().get_reflection()->shader_stage
        ) == vk::ShaderStageFlagBits::eVertex;
      }
    );
    if( vs == props.pipeline_create_info.get_stage().end() ) { /////
      const auto ms = std::find_if(
        props.pipeline_create_info.get_stage().begin(),
        props.pipeline_create_info.get_stage().end(),
        []( const auto &s ) -> bool {
          return spv2vk(
            s.get_shader_module()->get_props().get_reflection()->shader_stage
          ) == vk::ShaderStageFlagBits::eMeshEXT;
        }
      );
      if( ms == props.pipeline_create_info.get_stage().end() ) {
        throw -1;
      }
    }
    else {
      std::tie(vistat,vamap,stride) = gct::get_vertex_attributes(
        device,
        vs->get_shader_module()->get_props().get_reflection()
      );
    }

    descriptor_set.resize( props.swapchain_image_count );
    for( auto &d: descriptor_set_layout ) {
      for( unsigned int i = 0u; i != props.swapchain_image_count; ++i ) {
        if( descriptor_set[ i ].size() <= d.first ) {
          descriptor_set[ i ].resize( d.first + 1u );
        }
      }
      const auto ext = props.external_descriptor_set.find( d.first );
      if( ext != props.external_descriptor_set.end() ) {
        for( unsigned int i = 0u; i != props.swapchain_image_count; ++i ) {
          descriptor_set[ i ][ d.first ] = ext->second;
          descriptor_set_layout[ d.first ] = ext->second->get_props().get_layout()[ 0 ];
        }
      }
      else {
        for( unsigned int i = 0u; i != props.swapchain_image_count; ++i ) {
          descriptor_set[ i ][ d.first ] = props.allocator_set.descriptor_pool->allocate(
            d.second
          );
        }
      }
    }
    for( unsigned int i = 0u; i != descriptor_set.size(); ++i ) {
      std::vector< write_descriptor_set_t > temp;
      for( const auto &r: props.resources ) {
        temp.push_back( { r, i } );
      }
      if( props.ignore_unused_descriptor ) {
        for( unsigned int did = 0u; did != descriptor_set[ i ].size(); ++did ) {
          if( descriptor_set[ i ][ did ] ) {
            const auto ext = props.external_descriptor_set.find( did );
            if( ext == props.external_descriptor_set.end() ) {
              descriptor_set[ i ][ did ]->update( temp, props.ignore_unused_descriptor );
            }
          }
        }
      }
      else {
        if( !descriptor_set[ i ].empty() && descriptor_set[ i ][ 0 ] ) {
          const auto ext = props.external_descriptor_set.find( 0u );
          if( ext == props.external_descriptor_set.end() ) {
            descriptor_set[ i ][ 0 ]->update( temp, props.ignore_unused_descriptor );
          }
        }
      }
    }
    if( props.pipeline_create_info.get_color_blend().get_attachment().empty() ) {
      if( props.pipeline_create_info.has_reflection( vk::ShaderStageFlagBits::eFragment ) ) {
        const auto &reflection = props.pipeline_create_info.get_reflection( vk::ShaderStageFlagBits::eFragment );
        pipeline_color_blend_state_create_info_t temp( props.pipeline_create_info.get_color_blend() );
        temp.clear_attachment();
        for( unsigned int i = 0u; i != reflection->output_variable_count; ++i ) {
          temp.add_attachment();
        }
        props.pipeline_create_info
          .set_color_blend( temp );
      }
    }
    pipeline = props.allocator_set.pipeline_cache->get_pipeline(
      props.pipeline_create_info
        .set_vertex_input( vistat )
        .set_layout( pipeline_layout )
        .fill_untouched()
        .rebuild_chain()
    );
    for( const auto &s: pipeline->get_props().get_stage() ) {
      const auto &r = s.get_shader_module()->get_props().get_reflection();
      auto pcmp = r.get_push_constant_member_pointer_maybe( "PushConstants" );
      if( pcmp ) {
        enable_push_constant( *pcmp );
        break;
      }
    }
  }
  void graphics::bind(
    command_buffer_recorder_t &rec,
    unsigned int image_index
  ) const {
    rec.bind(
      pipeline,
      descriptor_set[ image_index ]
    );
  }
  void graphics::operator()(
    command_buffer_recorder_t &rec,
    unsigned int image_index,
    unsigned int x,
    unsigned int y,
    unsigned int z
  ) const {
    bind( rec, image_index );
    push_constant_storage::operator()( rec, pipeline );
    rec.draw_mesh_task( x, y, z );
  }
  bool graphics::has_reflection( vk::ShaderStageFlagBits s ) const {
    return pipeline->get_props().has_reflection( s );
  }
  const shader_module_reflection_t &graphics::get_reflection( vk::ShaderStageFlagBits s ) const {
    return pipeline->get_props().get_reflection( s );
  }
  std::vector< color_attachment_name > graphics::get_color_attachment() const {
    std::vector< color_attachment_name > ca;
    const auto &reflection = graphics::get_reflection( vk::ShaderStageFlagBits::eFragment );
    ca.reserve( reflection->output_variable_count );
    for( unsigned int i = 0u; i != reflection->output_variable_count; ++i ) {
      ca.push_back(
      color_attachment_name()
        .set_location( reflection->output_variables[ i ]->location )
        .set_name( reflection->output_variables[ i ]->name )
        .set_type( spv2numeric_type( *reflection->output_variables[ i ]->type_description ) )
      );
    }
    return ca;
  }
  void to_json( nlohmann::json &dest, const graphics &src ) {
    dest = nlohmann::json::object();
    dest[ "props" ] = src.get_props();
  }
}

