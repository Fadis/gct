#include <algorithm>
#include <gct/spirv_reflect.h>
#include <gct/exception.hpp>
#include <gct/alignment.hpp>

namespace gct::alignment {

std::size_t align_to( std::size_t size, std::size_t alignment_ ) {
  return ( ( size / alignment_ ) * alignment_ ) + ( ( size % alignment_ ) ? alignment_ : 0u );
}

std::size_t get_scalar_alignment( const SpvReflectTypeDescription& ) {
  return 4u;
}
std::size_t get_aligned_scalar_size( const SpvReflectTypeDescription &type ) {
  const auto bytes = type.traits.numeric.scalar.width / 8u;
  return align_to( bytes, get_scalar_alignment( type ) );
}
std::size_t get_vector_alignment( const SpvReflectTypeDescription &type ) {
  if( type.traits.numeric.vector.component_count == 2u ) return 8u;
  else return 16u;
}
std::size_t get_aligned_vector_size( const SpvReflectTypeDescription &type ) {
  const auto bytes = get_aligned_scalar_size( type ) * type.traits.numeric.vector.component_count;
  return align_to( bytes, get_vector_alignment( type ) );
}
std::size_t get_matrix_column_alignment( const SpvReflectTypeDescription &type ) {
  if( type.traits.numeric.matrix.column_count == 2u ) return 8u;
  else return 16u;
}
std::size_t get_aligned_matrix_column_size( const SpvReflectTypeDescription &type ) {
  const auto bytes = get_aligned_scalar_size( type ) * type.traits.numeric.matrix.column_count;
  return align_to( bytes, get_matrix_column_alignment( type ) );
}
std::size_t get_matrix_alignment( const SpvReflectTypeDescription &type, memory_layout layout ) {
  if( layout == memory_layout::std140 ) {
    return 16u;
  }
  else if( layout == memory_layout::std430 ) {
    return get_matrix_column_alignment( type );
  }
  else {
    const char *name = type.struct_member_name ? type.struct_member_name : "(unknown)";
    const char *tname = type.type_name ? type.type_name : "(unknown)";
    throw exception::invalid_argument( std::string( "get_matrix_alignment : The " ) + tname + " value " + name + " is in unsupported layout.", __FILE__, __LINE__ );
  }
}
std::size_t get_aligned_matrix_size( const SpvReflectTypeDescription &type, memory_layout layout ) {
  return align_to( get_aligned_matrix_column_size( type ), get_matrix_alignment( type, layout ) ) * type.traits.numeric.matrix.row_count;
}
std::size_t get_array_count( const SpvReflectTypeDescription &type ) {
  std::size_t count = 0u;
  for( std::uint32_t i = 0u; i != type.traits.array.dims_count; ++i ) {
    if( type.traits.array.dims[ i ] == 0xFFFFFFFFu ) {
      const char *name = type.struct_member_name ? type.struct_member_name : "(unknown)";
      const char *tname = type.type_name ? type.type_name : "(unknown)";
      throw exception::invalid_argument( std::string( "get_array_count : The size of " ) + tname + " value " + name + " depends on specialization.", __FILE__, __LINE__ );
    }
    count *= type.traits.array.dims[ i ];
  }
  return count;
}
bool is_scalar( const SpvReflectTypeDescription &type ) {
  if( type.type_flags & SPV_REFLECT_TYPE_FLAG_VECTOR ) return false;
  else if( type.type_flags & SPV_REFLECT_TYPE_FLAG_MATRIX ) return false;
  else if( type.type_flags & SPV_REFLECT_TYPE_FLAG_STRUCT ) return false;
  else return true;
}
bool is_vector( const SpvReflectTypeDescription &type ) {
  if( type.type_flags & SPV_REFLECT_TYPE_FLAG_VECTOR ) return true;
  else return false;
}
bool is_matrix( const SpvReflectTypeDescription &type ) {
  if( type.type_flags & SPV_REFLECT_TYPE_FLAG_MATRIX ) return true;
  else return false;
}
bool is_struct( const SpvReflectTypeDescription &type ) {
  if( type.type_flags & SPV_REFLECT_TYPE_FLAG_STRUCT ) return true;
  else return false;
}
std::size_t get_elem_alignment( const SpvReflectTypeDescription &type, memory_layout layout ) {
  if( layout == memory_layout::std140 ) {
    return 16u;
  }
  else if( layout == memory_layout::std430 ) {
    if( is_scalar( type ) ) {
      return get_scalar_alignment( type );
    }
    else if( is_vector( type ) ) {
      return get_vector_alignment( type );
    }
    else if( is_matrix( type ) ) {
      return get_matrix_alignment( type, layout );
    }
    else if( is_struct( type ) ) {
      return get_struct_alignment( type, layout );
    }
    else {
      const char *name = type.struct_member_name ? type.struct_member_name : "(unknown)";
      const char *tname = type.type_name ? type.type_name : "(unknown)";
      throw exception::invalid_argument( std::string( "get_elem_alignment : The value " ) + name + " has unsupported type " + tname + ".", __FILE__, __LINE__ );
    }
  }
  else {
    const char *name = type.struct_member_name ? type.struct_member_name : "(unknown)";
    const char *tname = type.type_name ? type.type_name : "(unknown)";
    throw exception::invalid_argument( std::string( "get_elem_alignment : The " ) + tname + " value " + name + " is in unsupported layout.", __FILE__, __LINE__ );
  }
}
std::size_t get_aligned_elem_size( const SpvReflectTypeDescription &type, memory_layout layout ) {
  if( is_scalar( type ) ) {
    return align_to( get_aligned_scalar_size( type ), get_elem_alignment( type, layout ) );
  }
  else if( is_vector( type ) ) {
    return align_to( get_aligned_vector_size( type ), get_elem_alignment( type, layout ) );
  }
  else if( is_matrix( type ) ) {
    return align_to( get_aligned_matrix_size( type, layout ), get_elem_alignment( type, layout ) );
  }
  else if( is_struct( type ) ) {
    return align_to( get_aligned_struct_size( type, layout ), get_elem_alignment( type, layout ) );
  }
  else {
    const char *name = type.struct_member_name ? type.struct_member_name : "(unknown)";
    const char *tname = type.type_name ? type.type_name : "(unknown)";
    throw exception::invalid_argument( std::string( "get_aligned_elem_size : The value " ) + name + " has unsupported type " + tname + ".", __FILE__, __LINE__ );
  }
}
std::size_t get_array_alignment( const SpvReflectTypeDescription &type, memory_layout layout ) {
  return get_elem_alignment( type, layout );
}
std::size_t get_aligned_array_size( const SpvReflectTypeDescription &type, memory_layout layout ) {
  return get_aligned_elem_size( type, layout ) * get_array_count( type );
}
std::size_t get_struct_alignment( const SpvReflectTypeDescription &type, memory_layout layout ) {
  std::size_t max = 16u;
  for( std::uint32_t i = 0u; i != type.member_count; ++i ) {
    max = std::max( max, get_alignment( type.members[ i ], layout ) );
  }
  return max;
}
std::size_t get_aligned_struct_size( const SpvReflectTypeDescription &type, memory_layout layout ) {
  std::size_t total = 0u;
  for( std::uint32_t i = 0u; i != type.member_count; ++i ) {
    total += get_aligned_size( type.members[ i ], layout );
  }
  return align_to( total, get_struct_alignment( type, layout ) );
}
bool is_array( const SpvReflectTypeDescription &type ) {
  if( type.type_flags & SPV_REFLECT_TYPE_FLAG_ARRAY ) return true;
  else return false;
}
std::size_t get_alignment( const SpvReflectTypeDescription &type, memory_layout layout ) {
  if( is_array( type ) ) {
    return get_array_alignment( type, layout );
  }
  else if( is_scalar( type ) ) {
    return get_scalar_alignment( type );
  }
  else if( is_vector( type ) ) {
    return get_vector_alignment( type );
  }
  else if( is_matrix( type ) ) {
    return get_matrix_alignment( type, layout );
  }
  else if( is_struct( type ) ) {
    return get_struct_alignment( type, layout );
  }
  else {
    const char *name = type.struct_member_name ? type.struct_member_name : "(unknown)";
    const char *tname = type.type_name ? type.type_name : "(unknown)";
    throw exception::invalid_argument( std::string( "get_alignment : The value " ) + name + " has unsupported type " + tname + ".", __FILE__, __LINE__ );
  }
}
std::size_t get_aligned_size( const SpvReflectTypeDescription &type, memory_layout layout ) {
  if( is_array( type ) ) {
    return get_aligned_array_size( type, layout );
  }
  else if( is_scalar( type ) ) {
    return get_aligned_scalar_size( type );
  }
  else if( is_vector( type ) ) {
    return get_aligned_vector_size( type );
  }
  else if( is_matrix( type ) ) {
    return get_aligned_matrix_size( type, layout );
  }
  else if( is_struct( type ) ) {
    return get_aligned_struct_size( type, layout );
  }
  else {
    const char *name = type.struct_member_name ? type.struct_member_name : "(unknown)";
    const char *tname = type.type_name ? type.type_name : "(unknown)";
    throw exception::invalid_argument( std::string( "get_aligned_size : The value " ) + name + " has unsupported type " + tname + ".", __FILE__, __LINE__ );
  }
}
}


