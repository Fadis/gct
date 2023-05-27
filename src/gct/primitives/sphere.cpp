#include <cstdint>
#include <locale>
#include <cmath>
#include <gct/vertex_attributes.hpp>
#include <gct/primitive.hpp>
#include <gct/spirv_reflect.h>

namespace gct::primitive {
  struct sphere_patch_t {
    float x0 = 0.f;
    float x1 = 0.f;
    float y0 = 0.f;
    float y1 = 0.f;
    float z0 = 0.f;
    float z1 = 0.f;
    float r0 = 0.f;
    float r1 = 0.f;
    std::uint32_t segment_count = 0u;
    std::uint32_t ring_count = 0u;
    std::uint32_t current_segment = 0u;
    std::uint32_t current_ring = 0u;
  };
  sphere_patch_t create_sphere_patch(
    std::uint32_t segment_count,
    std::uint32_t ring_count,
    std::uint32_t current_segment,
    std::uint32_t current_ring
  ) {
    const float vangle0 = float( current_ring ) / float( ring_count ) * M_PI;
    const float vangle1 = float( current_ring + 1u ) / float( ring_count ) * M_PI;
    const auto y0 = -std::cos( vangle0 );
    const auto y1 = -std::cos( vangle1 );
    const auto r0 = std::sin( vangle0 );
    const auto r1 = std::sin( vangle1 );
    const float hangle0 = float( current_segment ) / float( segment_count ) * 2.f * M_PI;
    const float hangle1 = float( current_segment + 1u ) / float( segment_count ) * 2.f * M_PI;
    const auto x0 = std::sin( hangle0 );
    const auto x1 = std::sin( hangle1 );
    const auto z0 = std::cos( hangle0 );
    const auto z1 = std::cos( hangle1 );
    return {
      x0, x1, y0, y1, z0, z1, r0, r1, segment_count, ring_count, current_segment, current_ring
    };
  }

  void create_sphere_triangle_position(
    std::uint8_t *head,
    const sphere_patch_t &patch,
    const vertex_input_detail_t &detail,
    std::uint32_t stride
  ) {
    const std::uint32_t ring_vertex_count = patch.segment_count * 3u;
    if( patch.current_ring != 0u ) {
      const std::uint32_t vertex_offset = ( ( patch.current_ring * 2u - 1u ) * ring_vertex_count ) + patch.current_segment * 3u;
      set_vertex_buffer_values(
        head, vertex_offset, detail, stride,
        patch.x1 * patch.r0, patch.y0, patch.z1 * patch.r0, 1.f
      );
      set_vertex_buffer_values(
        head, vertex_offset + 1u, detail, stride,
        patch.x1 * patch.r1, patch.y1, patch.z1 * patch.r1, 1.f
      );
      set_vertex_buffer_values(
        head, vertex_offset + 2u, detail, stride,
        patch.x0 * patch.r0, patch.y0, patch.z0 * patch.r0, 1.f
      );
    }
    if( patch.current_ring != patch.ring_count - 1u ) {
      const std::uint32_t vertex_offset = ( ( patch.current_ring * 2u ) * ring_vertex_count ) + patch.current_segment * 3u;
      set_vertex_buffer_values(
        head, vertex_offset, detail, stride,
        patch.x0 * patch.r0, patch.y0, patch.z0 * patch.r0, 1.f
      );
      set_vertex_buffer_values(
        head, vertex_offset + 1u, detail, stride,
        patch.x1 * patch.r1, patch.y1, patch.z1 * patch.r1, 1.f
      );
      set_vertex_buffer_values(
        head, vertex_offset + 2u, detail, stride,
        patch.x0 * patch.r1, patch.y1, patch.z0 * patch.r1, 1.f
      );
    }
  }
  void create_sphere_triangle_tangent(
    std::uint8_t *head,
    const sphere_patch_t &patch,
    const vertex_input_detail_t &detail,
    std::uint32_t stride
  ) {
    const std::uint32_t ring_vertex_count = patch.segment_count * 3u;
    const auto cx0 = patch.z0;
    const auto cz0 = -patch.x0;
    const auto cx1 = patch.z1;
    const auto cz1 = -patch.x1;
    if( patch.current_ring != 0u ) {
      const std::uint32_t vertex_offset = ( ( patch.current_ring * 2u - 1u ) * ring_vertex_count ) + patch.current_segment * 3u;
      set_vertex_buffer_values(
        head, vertex_offset, detail, stride,
        cx1, 0.f, cz1, 1.f
      );
      set_vertex_buffer_values(
        head, vertex_offset + 1u, detail, stride,
        cx1, 0.f, cz1, 1.f
      );
      set_vertex_buffer_values(
        head, vertex_offset + 2u, detail, stride,
        cx0, 0.f, cz0, 1.f
      );
    }
    if( patch.current_ring != patch.ring_count - 1u ) {
      const std::uint32_t vertex_offset = ( ( patch.current_ring * 2u ) * ring_vertex_count ) + patch.current_segment * 3u;
      set_vertex_buffer_values(
        head, vertex_offset, detail, stride,
        cx0, 0.f, cz0, 1.f
      );
      set_vertex_buffer_values(
        head, vertex_offset + 1u, detail, stride,
        cx1, 0.f, cz1, 1.f
      );
      set_vertex_buffer_values(
        head, vertex_offset + 2u, detail, stride,
        cx0, 0.f, cz0, 1.f
      );
    }
  }
  void create_sphere_triangle_texcoord(
    std::uint8_t *head,
    const sphere_patch_t &patch,
    const vertex_input_detail_t &detail,
    std::uint32_t stride
  ) {
    const std::uint32_t ring_vertex_count = patch.segment_count * 3u;
    const auto u0 = float( patch.current_segment ) / float( patch.segment_count );
    const auto v0 = float( patch.current_ring ) / float( patch.ring_count );
    const auto u1 = float( patch.current_segment + 1u ) / float( patch.segment_count );
    const auto v1 = float( patch.current_ring + 1u ) / float( patch.ring_count );
    if( patch.current_ring != 0u ) {
      const std::uint32_t vertex_offset = ( ( patch.current_ring * 2u - 1u ) * ring_vertex_count ) + patch.current_segment * 3u;
      set_vertex_buffer_values(
        head, vertex_offset, detail, stride,
        u1, v0, 0.f, 1.f
      );
      set_vertex_buffer_values(
        head, vertex_offset + 1u, detail, stride,
        u1, v1, 0.f, 1.f
      );
      set_vertex_buffer_values(
        head, vertex_offset + 2u, detail, stride,
        u0, v0, 0.f, 1.f
      );
    }
    if( patch.current_ring != patch.ring_count - 1u ) {
      const std::uint32_t vertex_offset = ( ( patch.current_ring * 2u ) * ring_vertex_count ) + patch.current_segment * 3u;
      set_vertex_buffer_values(
        head, vertex_offset, detail, stride,
        u0, v0, 0.f, 1.f
      );
      set_vertex_buffer_values(
        head, vertex_offset + 1u, detail, stride,
        u1, v1, 0.f, 1.f
      );
      set_vertex_buffer_values(
        head, vertex_offset + 2u, detail, stride,
        u0, v1, 0.f, 1.f
      );
    }
  }
  void create_sphere_triangle_color(
    std::uint8_t *head,
    const sphere_patch_t &patch,
    const vertex_input_detail_t &detail,
    std::uint32_t stride
  ) {
    const std::uint32_t ring_vertex_count = patch.segment_count * 3u;
    const auto u0 = float( patch.current_segment ) / float( patch.segment_count );
    const auto v0 = float( patch.current_ring ) / float( patch.ring_count );
    const auto u1 = float( patch.current_segment + 1u ) / float( patch.segment_count );
    const auto v1 = float( patch.current_ring + 1u ) / float( patch.ring_count );
    std::uint32_t color = ( patch.current_segment + patch.current_ring ) & 0x1u;
    if( patch.current_ring != 0u ) {
      const std::uint32_t vertex_offset = ( ( patch.current_ring * 2u - 1u ) * ring_vertex_count ) + patch.current_segment * 3u;
      set_vertex_buffer_values(
        head, vertex_offset, detail, stride,
        1.f, color, color, 1.f
      );
      set_vertex_buffer_values(
        head, vertex_offset + 1u, detail, stride,
        1.f, color, color, 1.f
      );
      set_vertex_buffer_values(
        head, vertex_offset + 2u, detail, stride,
        1.f, color, color, 1.f
      );
    }
    if( patch.current_ring != patch.ring_count - 1u ) {
      const std::uint32_t vertex_offset = ( ( patch.current_ring * 2u ) * ring_vertex_count ) + patch.current_segment * 3u;
      set_vertex_buffer_values(
        head, vertex_offset, detail, stride,
        1.f, color, color, 1.f
      );
      set_vertex_buffer_values(
        head, vertex_offset + 1u, detail, stride,
        1.f, color, color, 1.f
      );
      set_vertex_buffer_values(
        head, vertex_offset + 2u, detail, stride,
        1.f, color, color, 1.f
      );
    }
  }
  void create_sphere_triangle_weight(
    std::uint8_t *head,
    const sphere_patch_t &patch,
    const vertex_input_detail_t &detail,
    std::uint32_t stride
  ) {
    const std::uint32_t ring_vertex_count = patch.segment_count * 3u;
    if( patch.current_ring != 0u ) {
      const std::uint32_t vertex_offset = ( ( patch.current_ring * 2u - 1u ) * ring_vertex_count ) + patch.current_segment * 3u;
      set_vertex_buffer_values(
        head, vertex_offset, detail, stride,
        1.f
      );
      set_vertex_buffer_values(
        head, vertex_offset + 1u, detail, stride,
        1.f
      );
      set_vertex_buffer_values(
        head, vertex_offset + 2u, detail, stride,
        1.f
      );
    }
    if( patch.current_ring != patch.ring_count - 1u ) {
      const std::uint32_t vertex_offset = ( ( patch.current_ring * 2u ) * ring_vertex_count ) + patch.current_segment * 3u;
      set_vertex_buffer_values(
        head, vertex_offset, detail, stride,
        1.f
      );
      set_vertex_buffer_values(
        head, vertex_offset + 1u, detail, stride,
        1.f
      );
      set_vertex_buffer_values(
        head, vertex_offset + 2u, detail, stride,
        1.f
      );
    }
  }
  std::tuple<
    pipeline_input_assembly_state_create_info_t,
    std::vector< std::uint8_t >,
    std::uint32_t
  > create_sphere(
    const std::unordered_map< vertex_attribute_usage_t, vertex_input_detail_t > &detail,
    std::uint32_t stride,
    std::uint32_t segment_count,
    std::uint32_t ring_count
  ) {
    std::vector< std::uint8_t > data( stride * ( ring_count * 2u - 2u ) * segment_count * 3u, 0u );
    const auto position = detail.find( vertex_attribute_usage_t::POSITION );
    const auto normal = detail.find( vertex_attribute_usage_t::NORMAL );
    const auto tangent = detail.find( vertex_attribute_usage_t::TANGENT );
    const auto texcoord = detail.find( vertex_attribute_usage_t::TEXCOORD );
    const auto color = detail.find( vertex_attribute_usage_t::COLOR );
    const auto weight = detail.find( vertex_attribute_usage_t::WEIGHT );
    const auto joint = detail.find( vertex_attribute_usage_t::JOINT );
    for( std::uint32_t current_ring = 0u; current_ring != ring_count; ++current_ring ) {
      for( std::uint32_t current_segment = 0u; current_segment != segment_count; ++current_segment ) {
        const auto patch = create_sphere_patch( segment_count, ring_count, current_segment, current_ring );
        if( position != detail.end() ) {
          create_sphere_triangle_position( data.data(), patch, position->second, stride );
        }
        if( normal != detail.end() ) {
          create_sphere_triangle_position( data.data(), patch, normal->second, stride );
        }
        if( tangent != detail.end() ) {
          create_sphere_triangle_tangent( data.data(), patch, tangent->second, stride );
        }
        if( texcoord != detail.end() ) {
          create_sphere_triangle_texcoord( data.data(), patch, texcoord->second, stride );
        }
        if( color != detail.end() ) {
          create_sphere_triangle_color( data.data(), patch, color->second, stride );
        }
        if( weight != detail.end() ) {
          create_sphere_triangle_weight( data.data(), patch, texcoord->second, stride );
        }
      }
    }
    return std::make_tuple(
      pipeline_input_assembly_state_create_info_t()
        .set_basic(
          vk::PipelineInputAssemblyStateCreateInfo()
            .setTopology( vk::PrimitiveTopology::eTriangleList )
        ),
      data,
      ( ring_count * 2u - 2u ) * segment_count * 3u
    );
  }
}

