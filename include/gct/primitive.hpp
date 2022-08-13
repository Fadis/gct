#ifndef GCT_PRIMITIVE_HPP
#define GCT_PRIMITIVE_HPP
#include <cstdint>
#include <vector>
#include <tuple>
#include <unordered_map>
#include <gct/vertex_attributes.hpp>
#include <gct/pipeline_input_assembly_state_create_info.hpp>
namespace gct::primitive {
  std::tuple<
    pipeline_input_assembly_state_create_info_t,
    std::vector< std::uint8_t >
  > create_triangle(
    const std::unordered_map< vertex_attribute_usage_t, vertex_input_detail_t > &detail,
    std::uint32_t stride
  );
}
#endif

