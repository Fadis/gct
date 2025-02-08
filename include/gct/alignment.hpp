#ifndef GCT_ALIGNMENT_HPP
#define GCT_ALIGNMENT_HPP
#include <cstddef>
#include <nlohmann/json_fwd.hpp>
namespace gct {
enum class memory_layout {
  std140,
  std430
};
void to_json( nlohmann::json &dest, const memory_layout &src );
void from_json( const nlohmann::json &src, memory_layout &dest );
}

struct SpvReflectTypeDescription;

namespace gct::alignment {
std::size_t align_to( std::size_t size, std::size_t alignment_ );
[[nodiscard]] std::size_t get_scalar_alignment( const SpvReflectTypeDescription& );
[[nodiscard]] std::size_t get_aligned_scalar_size( const SpvReflectTypeDescription &type );
[[nodiscard]] std::size_t get_vector_alignment( const SpvReflectTypeDescription &type );
[[nodiscard]] std::size_t get_aligned_vector_size( const SpvReflectTypeDescription &type );
[[nodiscard]] std::size_t get_matrix_column_alignment( const SpvReflectTypeDescription &type );
[[nodiscard]] std::size_t get_aligned_matrix_column_size( const SpvReflectTypeDescription &type );
[[nodiscard]] std::size_t get_matrix_alignment( const SpvReflectTypeDescription &type, memory_layout layout );
[[nodiscard]] std::size_t get_aligned_matrix_size( const SpvReflectTypeDescription &type, memory_layout layout );
[[nodiscard]] std::size_t get_array_count( const SpvReflectTypeDescription &type );
[[nodiscard]] bool is_scalar( const SpvReflectTypeDescription &type );
[[nodiscard]] bool is_vector( const SpvReflectTypeDescription &type );
[[nodiscard]] bool is_matrix( const SpvReflectTypeDescription &type );
[[nodiscard]] bool is_struct( const SpvReflectTypeDescription &type );
[[nodiscard]] std::size_t get_elem_alignment( const SpvReflectTypeDescription &type, memory_layout layout );
[[nodiscard]] std::size_t get_aligned_elem_size( const SpvReflectTypeDescription &type, memory_layout layout );
[[nodiscard]] std::size_t get_array_alignment( const SpvReflectTypeDescription &type, memory_layout layout );
[[nodiscard]] std::size_t get_aligned_array_size( const SpvReflectTypeDescription &type, memory_layout layout );
[[nodiscard]] std::size_t get_struct_alignment( const SpvReflectTypeDescription &type, memory_layout layout );
[[nodiscard]] std::size_t get_aligned_struct_size( const SpvReflectTypeDescription &type, memory_layout layout );
[[nodiscard]] bool is_array( const SpvReflectTypeDescription &type );
[[nodiscard]] std::size_t get_alignment( const SpvReflectTypeDescription &type, memory_layout layout );
[[nodiscard]] std::size_t get_aligned_size( const SpvReflectTypeDescription &type, memory_layout layout );
}

#endif

