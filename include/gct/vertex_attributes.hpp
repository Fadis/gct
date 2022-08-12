#ifndef GCT_VERTEX_ATTRIBUTES_HPP
#define GCT_VERTEX_ATTRIBUTES_HPP

#include <cstdint>
#include <type_traits>
#include <vector>
#include <string>
#include <optional>
#include <utility>
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
    TEXCOORD
  };
  std::string get_normalized_vertex_attribute_name( const std::string &name );
  std::optional< vertex_attribute_usage_t > vertex_attribute_name_to_usage( const std::string &name );
  struct vertex_input_detail_t {
    std::uint32_t location = 0u;
    std::uint32_t offset = 0u;
    numeric_type_t format;
  };
  void to_json( nlohmann::json&, const vertex_attribute_usage_t& );
  void to_json( nlohmann::json&, const vertex_input_detail_t& );
  void to_json( nlohmann::json&, const std::unordered_map< vertex_attribute_usage_t, vertex_input_detail_t >& );
  void from_json( const nlohmann::json&, vertex_attribute_usage_t& );
  void from_json( const nlohmann::json&, vertex_input_detail_t& );
  void from_json( const nlohmann::json&, std::unordered_map< vertex_attribute_usage_t, vertex_input_detail_t >& );
  std::pair<
    gct::pipeline_vertex_input_state_create_info_t,
    std::unordered_map< vertex_attribute_usage_t, vertex_input_detail_t >
  >
  get_vertex_attributes(
    const device_t &device,
    const shader_module_reflection_t &reflection
  );
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

