#include <cstdint>
#include <locale>
#include <gct/format.hpp>
#include <gct/vertex_attributes.hpp>
#include <gct/primitive.hpp>
#include <gct/spirv_reflect.h>

namespace gct::primitive {
  std::tuple<
    pipeline_input_assembly_state_create_info_t,
    std::vector< std::uint8_t >,
    std::uint32_t
  > create_cursor(
    const std::unordered_map< vertex_attribute_usage_t, vertex_input_detail_t > &detail,
    std::uint32_t stride,
    bool positive
  ) {
    std::vector< std::uint8_t > data( stride * 15u, 0u );
    const auto position = detail.find( vertex_attribute_usage_t::POSITION );
    const auto normal = detail.find( vertex_attribute_usage_t::NORMAL );
    const auto tangent = detail.find( vertex_attribute_usage_t::TANGENT );
    const auto texcoord = detail.find( vertex_attribute_usage_t::TEXCOORD );
    const auto color = detail.find( vertex_attribute_usage_t::COLOR );
    const auto weight = detail.find( vertex_attribute_usage_t::WEIGHT );
    const auto joint = detail.find( vertex_attribute_usage_t::JOINT );
    if( position != detail.end() ) {
      set_vertex_buffer_values(
        data.data(), 0u, position->second, stride,
        0.f, 1.f-0.7f, 0.f, 1.f
      );
      set_vertex_buffer_values(
        data.data(), 1u, position->second, stride,
        0.1258f, 1.f-0.6612f, 0.f, 1.f
      );
      set_vertex_buffer_values(
        data.data(), 2u, position->second, stride,
        0.f, 1.f, 0.f, 1.f
      );
      set_vertex_buffer_values(
        data.data(), 3u, position->second, stride,
        0.1258f, 1.f-0.6612f, 0.f, 1.f
      );
      set_vertex_buffer_values(
        data.data(), 4u, position->second, stride,
        0.3283f, 1.f-0.5857f, 0.f, 1.f
      );
      set_vertex_buffer_values(
        data.data(), 5u, position->second, stride,
        0.f, 1.f, 0.f, 1.f
      );
      set_vertex_buffer_values(
        data.data(), 6u, position->second, stride,
        0.3283f, 1.f-0.5857f, 0.f, 1.f
      );
      set_vertex_buffer_values(
        data.data(), 7u, position->second, stride,
        0.4522f, 1.f-0.5403f, 0.f, 1.f
      );
      set_vertex_buffer_values(
        data.data(), 8u, position->second, stride,
        0.f, 1.f, 0.f, 1.f
      );
      set_vertex_buffer_values(
        data.data(), 9u, position->second, stride,
        0.2488f, 1.f-1.f, 0.f, 1.f
      );
      set_vertex_buffer_values(
        data.data(), 10u, position->second, stride,
        0.4520f, 1.f-0.9264f, 0.f, 1.f
      );
      set_vertex_buffer_values(
        data.data(), 11u, position->second, stride,
        0.1258f, 1.f-0.6612f, 0.f, 1.f
      );
      set_vertex_buffer_values(
        data.data(), 12u, position->second, stride,
        0.1258f, 1.f-0.6612f, 0.f, 1.f
      );
      set_vertex_buffer_values(
        data.data(), 13u, position->second, stride,
        0.4520f, 1.f-0.9264f, 0.f, 1.f
      );
      set_vertex_buffer_values(
        data.data(), 14u, position->second, stride,
        0.3283f, 1.f-0.5857f, 0.f, 1.f
      );
    }
    if( normal != detail.end() ) {
      for( unsigned int i = 0u; i != 15u; ++i ) {
        set_vertex_buffer_values(
          data.data(), i, normal->second, stride,
          0.f, 0.f, 1.f, 1.f
        );
      }
    }
    if( tangent != detail.end() ) {
      for( unsigned int i = 0u; i != 15u; ++i ) {
        set_vertex_buffer_values(
          data.data(), i, tangent->second, stride,
          1.f, 0.f, 0.f, 1.f
        );
      }
    }
    if( texcoord != detail.end() ) {
      set_vertex_buffer_values(
        data.data(), 0u, texcoord->second, stride,
        0.f, 1.f-0.7f, 0.f, 1.f
      );
      set_vertex_buffer_values(
        data.data(), 1u, texcoord->second, stride,
        0.1258f, 1.f-0.6612f, 0.f, 1.f
      );
      set_vertex_buffer_values(
        data.data(), 2u, texcoord->second, stride,
        0.f, 1.f, 0.f, 1.f
      );
      set_vertex_buffer_values(
        data.data(), 3u, texcoord->second, stride,
        0.1258f, 1.f-0.6612f, 0.f, 1.f
      );
      set_vertex_buffer_values(
        data.data(), 4u, texcoord->second, stride,
        0.3283f, 1.f-0.5857f, 0.f, 1.f
      );
      set_vertex_buffer_values(
        data.data(), 5u, texcoord->second, stride,
        0.f, 1.f, 0.f, 1.f
      );
      set_vertex_buffer_values(
        data.data(), 6u, texcoord->second, stride,
        0.3283f, 1.f-0.5857f, 0.f, 1.f
      );
      set_vertex_buffer_values(
        data.data(), 7u, texcoord->second, stride,
        0.4522f, 1.f-0.5403f, 0.f, 1.f
      );
      set_vertex_buffer_values(
        data.data(), 8u, texcoord->second, stride,
        0.f, 1.f, 0.f, 1.f
      );
      set_vertex_buffer_values(
        data.data(), 9u, texcoord->second, stride,
        0.2488f, 1.f-1.f, 0.f, 1.f
      );
      set_vertex_buffer_values(
        data.data(), 10u, texcoord->second, stride,
        0.4520f, 1.f-0.9264f, 0.f, 1.f
      );
      set_vertex_buffer_values(
        data.data(), 11u, texcoord->second, stride,
        0.1258f, 1.f-0.6612f, 0.f, 1.f
      );
      set_vertex_buffer_values(
        data.data(), 12u, texcoord->second, stride,
        0.1258f, 1.f-0.6612f, 0.f, 1.f
      );
      set_vertex_buffer_values(
        data.data(), 13u, texcoord->second, stride,
        0.4520f, 1.f-0.9264f, 0.f, 1.f
      );
      set_vertex_buffer_values(
        data.data(), 14u, texcoord->second, stride,
        0.3283f, 1.f-0.5857f, 0.f, 1.f
      );
    }
    if( color != detail.end() ) {
      for( unsigned int i = 0u; i != 15u; ++i ) {
        set_vertex_buffer_values(
          data.data(), i, color->second, stride,
          1.f, 1.f, 1.f, 1.f
        );
      }
    }
    if( weight != detail.end() ) {
      for( unsigned int i = 0u; i != 15u; ++i ) {
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
      15u
    );
  }
}

