#include <cstdint>
#include <locale>
#include <gct/vertex_attributes.hpp>
#include <gct/primitive.hpp>
#include <gct/spirv_reflect.h>

namespace gct::primitive {
  std::tuple<
    pipeline_input_assembly_state_create_info_t,
    std::vector< std::uint8_t >,
    std::uint32_t
  > create_cube(
    const std::unordered_map< vertex_attribute_usage_t, vertex_input_detail_t > &detail,
    std::uint32_t stride
  ) {
    std::vector< std::uint8_t > data( stride * 36u, 0u );
    const auto position = detail.find( vertex_attribute_usage_t::POSITION );
    const auto normal = detail.find( vertex_attribute_usage_t::NORMAL );
    const auto tangent = detail.find( vertex_attribute_usage_t::TANGENT );
    const auto texcoord = detail.find( vertex_attribute_usage_t::TEXCOORD );
    const auto color = detail.find( vertex_attribute_usage_t::COLOR );
    const auto weight = detail.find( vertex_attribute_usage_t::WEIGHT );
    const auto joint = detail.find( vertex_attribute_usage_t::JOINT );
    if( position != detail.end() ) {
      static const std::vector< std::int8_t > values{
        1, 0, 1,  1, 0, 0,  1, 1, 1,  1, 1, 1,  1, 0, 0,  1, 1, 0,
        0, 1, 1,  1, 1, 1,  0, 1, 0,  0, 1, 0,  1, 1, 1,  1, 1, 0,
        0, 0, 1,  1, 0, 1,  0, 1, 1,  0, 1, 1,  1, 0, 1,  1, 1, 1,
        0, 0, 0,  0, 0, 1,  0, 1, 0,  0, 1, 0,  0, 0, 1,  0, 1, 1,
        0, 0, 0,  1, 0, 0,  0, 0, 1,  0, 0, 1,  1, 0, 0,  1, 0, 1,
        1, 0, 0,  0, 0, 0,  1, 1, 0,  1, 1, 0,  0, 0, 0,  0, 1, 0,
      };
      for( unsigned int i = 0u; i != 36u; ++i ) {
        set_vertex_buffer_values(
          data.data(), i, position->second, stride,
          values[ i * 3u ], values[ i * 3u + 1u ], values[ i * 3u + 2 ], 1.f
        );
      }
    }
    if( normal != detail.end() ) {
      static const std::vector< std::int8_t > values{
        1, 0, 0,
        0, 1, 0,
        0, 0, 1,
        -1, 0, 0,
        0, -1, 0,
        0, 0, -1,
      };
      for( unsigned int f = 0u; f != 6u; ++f ) {
        for( unsigned int v = 0u; v != 6u; ++v ) {
          set_vertex_buffer_values(
            data.data(), f * 6u + v, normal->second, stride,
            values[ f * 3u ], values[ f * 3u + 1u ] , values[ f * 3u + 2u ], 1.f
          );
        }
      }
    }
    if( tangent != detail.end() ) {
      static const std::vector< std::int8_t > values{
        0, -1, 0,
        1, 0, 0,
        1, 0, 0,
        0, 0, 1,
        0, 0, 1,
        0, -1, 0,
      };
      for( unsigned int f = 0u; f != 6u; ++f ) {
        for( unsigned int v = 0u; v != 6u; ++v ) {
          set_vertex_buffer_values(
            data.data(), f * 6u + v, tangent->second, stride,
            values[ f * 3u ], values[ f * 3u + 1u ] , values[ f * 3u + 2u ], 1.f
          );
        }
      }
    }
    if( texcoord != detail.end() ) {
      static const std::vector< std::int8_t > values{
        1, 2, 2, 2, 1, 3,  1, 3, 2, 2, 2, 3,
        0, 2, 1, 2, 0, 3,  0, 3, 1, 2, 1, 3,
        0, 1, 1, 1, 0, 2,  0, 2, 1, 1, 1, 2,
        2, 1, 3, 1, 2, 2,  2, 2, 3, 1, 3, 2,
        2, 1, 2, 0, 3, 1,  3, 1, 2, 0, 3, 0,
        2, 0, 2, 1, 1, 0,  1, 0, 2, 1, 1, 1,
      };
      for( unsigned int i = 0u; i != 36u; ++i ) {
        set_vertex_buffer_values(
          data.data(), i, texcoord->second, stride,
          values[ i * 2u ], values[ i * 2u + 1u ], 0.f, 1.f
        );
      }
    }
    if( color != detail.end() ) {
      static const std::vector< std::int8_t > values{
        1, 0, 0,
        0, 1, 0,
        0, 0, 1,
        1, 0, 0,
        0, 1, 0,
        0, 0, 1,
      };
      for( unsigned int f = 0u; f != 6u; ++f ) {
        for( unsigned int v = 0u; v != 6u; ++v ) {
          set_vertex_buffer_values(
            data.data(), f * 6u + v, color->second, stride,
            values[ f * 3u ], values[ f * 3u + 1u ] , values[ f * 3u + 2u ], 1.f
          );
        }
      }
    }
    if( weight != detail.end() ) {
      for( unsigned int i = 0u; i != 36u; ++i ) {
        set_vertex_buffer_values(
          data.data(), i, weight->second, stride,
          1.f
        );
      }
    }
    return std::make_tuple(
      pipeline_input_assembly_state_create_info_t()
        .set_basic(
          vk::PipelineInputAssemblyStateCreateInfo()
            .setTopology( vk::PrimitiveTopology::eTriangleList )
        ),
      data,
      36u
    );
  }
}

