#include <cstdint>
#include <locale>
#include <gct/format.hpp>
#include <gct/vertex_attributes.hpp>
#include <gct/numeric_types.hpp>
#include <gct/spirv_reflect.h>
#include <vulkan2json/Format.hpp>
namespace gct {
  /*std::pair<
    gct::pipeline_vertex_input_state_create_info_t,
    std::unordered_map< std::uint32_t, vertex_attribute_usage_t >
  >
  get_vertex_attributes(
    const device_t &device,
    const shader_module_reflection_t &reflection
  ) {
    std::unordered_map< std::uint32_t, vertex_attribute_usage_t > locs;
    const auto &vertex_buffer_formats = device.get_vertex_buffer_formats();
    auto vistat = gct::pipeline_vertex_input_state_create_info_t();
    std::uint32_t offset = 0u;
    for( unsigned int i = 0; i != reflection->input_variable_count; ++i ) {
      if(
        reflection->input_variables &&
        reflection->input_variables[ i ] &&
        reflection->input_variables[ i ]->type_description
      ) {
        const auto usage = vertex_attribute_name_to_usage( reflection->input_variables[ i ]->name );
        if( usage ) {
          const auto input_variable = spv2numeric_type( *reflection->input_variables[ i ]->type_description );
          const auto &compatible_formats = get_compatible_format( input_variable );
          vk::Format selected_format = vk::Format::eUndefined;
          for( const auto &f: compatible_formats ) {
            if( vertex_buffer_formats.find( f ) != vertex_buffer_formats.end() ) {
              selected_format = f;
              break;
            }
          }
          if( selected_format == vk::Format::eUndefined ) throw -1;
          vistat
            .add_vertex_input_attribute_description(
              vk::VertexInputAttributeDescription()
                .setLocation( reflection->input_variables[ i ]->location )
                .setFormat( selected_format )
                .setBinding( 0 )
                .setOffset( offset )
            );
          locs.insert( std::make_pair( reflection->input_variables[ i ]->location, *usage ) );
          offset += format_to_size( selected_format );
        }
      }
    }
    vistat
      .add_vertex_input_binding_description(
        vk::VertexInputBindingDescription()
          .setBinding( 0 )
          .setInputRate( vk::VertexInputRate::eVertex )
          .setStride( offset )
      );
    return std::make_pair( std::move( vistat ), std::move( locs ) );
  }*/
  std::pair<
    gct::pipeline_vertex_input_state_create_info_t,
    std::unordered_map< vertex_attribute_usage_t, vertex_input_detail_t >
  >
  get_vertex_attributes(
    const device_t &device,
    const shader_module_reflection_t &reflection
  ) {
    std::unordered_map< vertex_attribute_usage_t, vertex_input_detail_t > locs;
    const auto &vertex_buffer_formats = device.get_vertex_buffer_formats();
    auto vistat = gct::pipeline_vertex_input_state_create_info_t();
    std::uint32_t offset = 0u;
    for( unsigned int i = 0; i != reflection->input_variable_count; ++i ) {
      if(
        reflection->input_variables &&
        reflection->input_variables[ i ] &&
        reflection->input_variables[ i ]->name &&
        reflection->input_variables[ i ]->type_description
      ) {
        const auto usage = vertex_attribute_name_to_usage( reflection->input_variables[ i ]->name );
        if( usage ) {
          const auto input_variable = spv2numeric_type( *reflection->input_variables[ i ]->type_description );
          const auto &compatible_formats = get_compatible_format( input_variable );
          vk::Format selected_format = vk::Format::eUndefined;
          for( const auto &f: compatible_formats ) {
            if( vertex_buffer_formats.find( f ) != vertex_buffer_formats.end() ) {
              selected_format = f;
              break;
            }
          }
          if( selected_format == vk::Format::eUndefined ) throw -1;
          vistat
            .add_vertex_input_attribute_description(
              vk::VertexInputAttributeDescription()
                .setLocation( reflection->input_variables[ i ]->location )
                .setFormat( selected_format )
                .setBinding( 0 )
                .setOffset( offset )
            );
          locs.insert( std::make_pair(
            *usage,
            vertex_input_detail_t{
              reflection->input_variables[ i ]->location,
              offset,
              format2numeric_type( selected_format )
            }
          ) );
          offset += format_to_size( selected_format );
        }
      }
    }
    vistat
      .add_vertex_input_binding_description(
        vk::VertexInputBindingDescription()
          .setBinding( 0 )
          .setInputRate( vk::VertexInputRate::eVertex )
          .setStride( offset )
      )
      .rebuild_chain();
    return std::make_pair( std::move( vistat ), std::move( locs ) );
  }
}
