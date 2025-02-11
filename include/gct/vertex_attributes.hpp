#ifndef GCT_VERTEX_ATTRIBUTES_HPP
#define GCT_VERTEX_ATTRIBUTES_HPP

#include <cstdint>
#include <type_traits>
#include <string>
#include <optional>
#include <tuple>
#include <limits>
#include <unordered_map>
#include <gct/shader_module.hpp>
#include <gct/device.hpp>
#include <gct/pipeline_vertex_input_state_create_info.hpp>
#include <gct/numeric_types.hpp>
namespace gct {
  enum class vertex_attribute_usage_t {
    POSITION,
    NORMAL,
    TANGENT,
    TEXCOORD,
    COLOR,
    JOINT,
    WEIGHT
  };
  [[nodiscard]] std::string get_normalized_vertex_attribute_name( const std::string &name );
  [[nodiscard]] std::uint32_t get_normalized_vertex_attribute_local_index( const std::string &name );
  [[nodiscard]] std::optional< vertex_attribute_usage_t > vertex_attribute_name_to_usage( const std::string &name );
  struct vertex_input_detail_t {
    std::uint32_t location = 0u;
    std::uint32_t offset = 0u;
    numeric_type_t format;
  };
  std::string to_string( const vertex_attribute_usage_t &src );
  void to_json( nlohmann::json&, const vertex_attribute_usage_t& );
  void to_json( nlohmann::json&, const vertex_input_detail_t& );
  void to_json( nlohmann::json&, const std::unordered_map< vertex_attribute_usage_t, vertex_input_detail_t >& );
  void from_json( const nlohmann::json&, vertex_attribute_usage_t& );
  void from_json( const nlohmann::json&, vertex_input_detail_t& );
  void from_json( const nlohmann::json&, std::unordered_map< vertex_attribute_usage_t, vertex_input_detail_t >& );
  [[nodiscard]] std::tuple<
    gct::pipeline_vertex_input_state_create_info_t,
    std::unordered_map< vertex_attribute_usage_t, vertex_input_detail_t >,
    std::uint32_t
  >
  get_vertex_attributes(
    const device_t &device,
    const shader_module_reflection_t &reflection
  );
  template<
    std::floating_point T,
    std::floating_point U
  >
  void set_vertex_buffer_value(
    T &dest,
    U src,
    const vertex_input_detail_t&
  ) {
    dest = src;
  }
  template<
    std::floating_point T,
    std::integral U
  >
  void set_vertex_buffer_value(
    T &dest,
    U src,
    const vertex_input_detail_t&
  ) {
    dest = src;
  }
  template<
    std::unsigned_integral T,
    std::floating_point U
  >
  void set_vertex_buffer_value(
    T &dest,
    U src,
    const vertex_input_detail_t &detail
  ) {
    if( detail.format.attr == integer_attribute_t::normalized ) {
      if( src == U( 0 ) ) dest = 0u;
      else {
        if( detail.format.sign ) {
          if( src > U( 0 ) ) {
            dest = T( U( std::numeric_limits< T >::max() >> 1 ) * std::min( U( 1.0 ), src ) );
          }
          else {
            dest = T( U( std::numeric_limits< T >::max() >> 1 ) * std::max( U( -1.0 ), src ) );
          }
        }
        else {
          if( src > U( 0 ) ) {
            dest = T( U( std::numeric_limits< T >::max() ) * std::min( U( 1.0 ), src ) );
          }
          else {
            dest = 0u;
          }
        }
      }
    }
    else if( detail.format.attr == integer_attribute_t::srgb ) {
      throw -1; // not implemented
    }
    else {
      dest = std::int64_t( src );
    }
  }
  template<
    std::unsigned_integral T,
    std::integral U
  >
  void set_vertex_buffer_value(
    T &dest,
    U src,
    const vertex_input_detail_t &detail
  ) {
    if( detail.format.attr == integer_attribute_t::normalized ) {
      if( src == U( 0 ) ) dest = 0u;
      else {
        if( detail.format.sign ) {
          if constexpr ( std::is_signed_v< U > ) {
            if( src > U( 0 ) ) {
              dest = std::numeric_limits< T >::max() >> 1;
            }
            else {
              dest = ( std::numeric_limits< T >::max() >> 1 ) + 1u;
            }
          }
          else {
            dest = std::numeric_limits< T >::max() >> 1;
          }
        }
        else {
          if constexpr ( std::is_signed_v< U > ) {
            if( src > U( 0 ) ) {
              dest = std::numeric_limits< T >::max();
            }
            else {
              dest = 0u;
            }
          }
          else {
            dest = std::numeric_limits< T >::max();
          }
        }
      }
    }
    else if( detail.format.attr == integer_attribute_t::srgb ) {
      throw -1; // not implemented
    }
    else {
      dest = src;
    }
  }
  template< typename T >
  void set_vertex_buffer_value(
    std::uint8_t *head,
    std::uint32_t vertex_index,
    std::uint32_t component_index,
    const vertex_input_detail_t &detail,
    std::uint32_t stride,
    T value
  ) {
    if( detail.format.component == numeric_component_type_t::void_ ) return;
    if( detail.format.composite == numeric_composite_type_t::scalar && component_index >= 1u ) return;
    if( detail.format.composite == numeric_composite_type_t::vector && component_index >= detail.format.rows ) return;
    if( detail.format.composite == numeric_composite_type_t::matrix && component_index >= detail.format.rows * detail.format.cols ) return;
    auto dest = std::next( head, stride * vertex_index + detail.format.depth / 8u * component_index + detail.offset );
    if(
      detail.format.component == numeric_component_type_t::bool_ ||
      detail.format.component == numeric_component_type_t::int_
    ) {
      if( detail.format.depth == 8u ) {
        set_vertex_buffer_value( *dest, value, detail );
      }
      else if( detail.format.depth == 16u ) {
        set_vertex_buffer_value( *reinterpret_cast< std::uint16_t* >( dest ), value, detail );
      }
      else if( detail.format.depth == 32u ) {
        set_vertex_buffer_value( *reinterpret_cast< std::uint32_t* >( dest ), value, detail );
      }
      else if( detail.format.depth == 64u ) {
        set_vertex_buffer_value( *reinterpret_cast< std::uint64_t* >( dest ), value, detail );
      }
    }
    else if( detail.format.component == numeric_component_type_t::float_ ) {
      if( detail.format.depth == 16u ) {
        //set_vertex_buffer_value( *reinterpret_cast< FLOAT16* >( dest ), value, detail );
      }
      else if( detail.format.depth == 32u ) {
        set_vertex_buffer_value( *reinterpret_cast< float* >( dest ), value, detail );
      }
      else if( detail.format.depth == 64u ) {
        set_vertex_buffer_value( *reinterpret_cast< double* >( dest ), value, detail );
      }
    }
  }
  template< std::size_t i >
  void set_vertex_buffer_values_internal(
    std::uint8_t*,
    std::uint32_t,
    const vertex_input_detail_t&,
    std::uint32_t
  ) {}
  template< std::size_t i, typename Head, typename ...Tail >
  void set_vertex_buffer_values_internal(
    std::uint8_t *dest,
    std::uint32_t vertex_index,
    const vertex_input_detail_t &detail,
    std::uint32_t stride,
    Head head,
    Tail ...tail
  ) {
    set_vertex_buffer_value( dest, vertex_index, i, detail, stride, head );
    set_vertex_buffer_values_internal< i + 1u >( dest, vertex_index, detail, stride, tail... );
  }
  template< typename ...T >
  void set_vertex_buffer_values(
    std::uint8_t *head,
    std::uint32_t vertex_index,
    const vertex_input_detail_t &detail,
    std::uint32_t stride,
    T ...value
  ) {
    set_vertex_buffer_values_internal< 0u >( head, vertex_index, detail, stride, value... );
  }
  /*template< typename T >
  void set_vertex_attribute_value(
    std::uint8_t *head,
    vertex_attribute_usage_t usage,
    std::uint32_t comp_index,
    std::uint32_t vert_index,
    const vk::VertexInputAttributeDescription &attr,
    std::uint32_t stride,
    T value
  ) {
    auto attr_head = std::next( head, stride * vert_index + offset );
    if constexpr ( std::is_integral_v< T > ) {
      
    }
    else if constexpr ( std::is_floating_point_v< T > ) {
      if( is_unorm( attr.format ) ) {
      }
      else if( is_snorm( attr.format ) ) {
      }
      else {
        if( is_int8( attr.format ) )
      }
    }
  }*/


}

#endif

