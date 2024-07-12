#include <algorithm>
#include <nlohmann/json.hpp>
#include <gct/spirv_reflect.h>
#include <gct/alignment.hpp>
#include <gct/spv_member_pointer.hpp>

namespace gct {
  spv_member_pointer::spv_member_pointer(
    std::size_t b,
    const SpvReflectTypeDescription &type,
    memory_layout layout
  ) : begin_( b ), impl( new spv_member_pointer_impl() ) {
    aligned_size = alignment::get_aligned_size( type, layout );
    if( alignment::is_array( type ) ) {
      if( type.op == SpvOpTypeRuntimeArray ) {
        runtime_array = true;
        count = 1u;
        aligned_size = type.traits.array.stride;
        stride = type.traits.array.stride;
      }
      else {
        count = alignment::get_array_count( type );
        stride = alignment::get_aligned_elem_size( type, layout );
      }
    }
    if( alignment::is_struct( type ) ) {
      child.reset( new std::unordered_map< std::string, spv_member_pointer >() );
      std::size_t cur = begin_;
      for( std::uint32_t i = 0u; i != type.member_count; ++i ) {
        auto c = spv_member_pointer(
          cur,
          type.members[ i ],
          layout
        );
        cur += c.get_aligned_size();
        child->insert(
          child->end(),
          std::make_pair(
            std::string( type.members[ i ].struct_member_name ),
            std::move( c )
          )
        );
      }
      impl->cur_child = child->end();
    }
    else {
      numeric = spv2numeric_type( type );
    }
  }
  const numeric_type_t &spv_member_pointer::get_numeric() const {
    if( stride != 0u ) {
      throw exception::invalid_argument( "spv_member_pointer::get_numeric : Not a numeric.", __FILE__, __LINE__ );
    }
    if( child && !child->empty() ) {
      throw exception::invalid_argument( "spv_member_pointer::get_numeric : Not a numeric.", __FILE__, __LINE__ );
    }
    return numeric;
  }
  const spv_member_pointer &spv_member_pointer::operator[]( const std::string &name ) const {
    if( stride != 0u ) {
      throw exception::invalid_argument( "spv_member_pointer::operator[] : Not a struct.", __FILE__, __LINE__ );
    }
    if( !child || child->empty() ) {
      throw exception::invalid_argument( "spv_member_pointer::operator[] : Not a struct.", __FILE__, __LINE__ );
    }
    const auto iter = child->find( name );
    if( iter == child->end() ) {
      throw exception::invalid_argument( std::string( "spv_member_pointer::operator[] : There are no member named " ) + name + ".", __FILE__, __LINE__ );
    }
    return iter->second;
  }
  spv_member_pointer spv_member_pointer::operator[]( int i ) const {
    if( stride == 0u ) {
      throw exception::invalid_argument( "spv_member_pointer::operator[] : Not an array.", __FILE__, __LINE__ );
    }
    if( count && i >= count && !runtime_array ) {
      throw exception::invalid_argument( "spv_member_pointer::operator[] : Out of range.", __FILE__, __LINE__ );
    }
    auto temp = *this;
    temp.count = 1u;
    temp.begin_ += temp.stride * i;
    temp.stride = 0u;
    return temp;
  }
  spv_member_pointer spv_member_pointer::begin() const {
    if( stride != 0u ) {
      return *this;
    }
    else if( child && !child->empty() ) {
      auto temp = *this;
      temp.impl->cur_child = temp.child->begin();
      return temp;
    }
    else {
      throw exception::invalid_argument( "spv_member_pointer::begin : Not an array or struct.", __FILE__, __LINE__ );
    }
  }
  spv_member_pointer spv_member_pointer::end() const {
    if( runtime_array ) {
      throw exception::invalid_argument( "spv_member_pointer::operator[] : runtime array", __FILE__, __LINE__ );
    }
    if( stride != 0u ) {
      auto temp = *this;
      temp.begin_ += temp.stride * temp.count;
      temp.count = 0u;
      return temp;
    }
    else if( child && !child->empty() ) {
      auto temp = *this;
      temp.impl->cur_child = temp.child->end();
      return temp;
    }
    else {
      throw exception::invalid_argument( "spv_member_pointer::end : Not an array or struct.", __FILE__, __LINE__ );
    }
  }
  spv_member_pointer &spv_member_pointer::operator++() {
    if( stride != 0u ) {
      if( count == 0u ) {
        throw exception::invalid_argument( "spv_member_pointer::operator++ : Out of range.", __FILE__, __LINE__ );
      }
      if( !runtime_array ) {
        --count;
      }
      begin_ += stride;
    }
    else if( child && !child->empty() && impl->cur_child != child->end() ) {
      ++impl->cur_child;
    }
    else {
      throw exception::invalid_argument( "spv_member_pointer::operator++ : Not an iterator.", __FILE__, __LINE__ );
    }
    return *this;
  }
  spv_member_pointer spv_member_pointer::operator++( int ) {
    auto temp = *this;
    if( stride != 0u ) {
      if( count == 0u ) {
        throw exception::invalid_argument( "spv_member_pointer::operator++ : Out of range.", __FILE__, __LINE__ );
      }
      if( !runtime_array ) {
        --count;
      }
      begin_ += stride;
    }
    else if( child && !child->empty() && impl->cur_child != child->end() ) {
      ++impl->cur_child;
    }
    else {
      throw exception::invalid_argument( "spv_member_pointer::operator++ : Not an iterator.", __FILE__, __LINE__ );
    }
    return temp;
  }
  bool spv_member_pointer::operator==( const spv_member_pointer &r ) const {
    return
      begin_ == r.begin_ &&
      aligned_size == r.aligned_size &&
      runtime_array == r.runtime_array &&
      count == r.count &&
      stride == r.stride &&
      child == r.child &&
      impl->cur_child == r.impl->cur_child &&
      numeric == r.numeric;
  }
  bool spv_member_pointer::operator!=( const spv_member_pointer &r ) const {
    return
      begin_ != r.begin_ ||
      aligned_size != r.aligned_size ||
      runtime_array != r.runtime_array ||
      count != r.count ||
      stride != r.stride ||
      child != r.child ||
      impl->cur_child != r.impl->cur_child ||
      numeric != r.numeric;
  }
  spv_member_pointer spv_member_pointer::operator*() const {
    if( stride != 0u ) {
      if( count == 0u ) {
        throw exception::invalid_argument( "spv_member_pointer::operator* : Out of range.", __FILE__, __LINE__ );
      }
      auto temp = *this;
      temp.stride = 0u;
      return *this;
    }
    else if( child && !child->empty() && impl->cur_child != child->end() ) {
      return impl->cur_child->second;
    }
    else {
      throw exception::invalid_argument( "spv_member_pointer::operator* : Not an iterator.", __FILE__, __LINE__ );
    }
  }
  bool spv_member_pointer::has( const std::string &name ) const {
    if( stride != 0u ) {
      throw exception::invalid_argument( "spv_member_pointer::operator[] : Not a struct.", __FILE__, __LINE__ );
    }
    if( !child || child->empty() ) {
      throw exception::invalid_argument( "spv_member_pointer::operator[] : Not a struct.", __FILE__, __LINE__ );
    }
    const auto iter = child->find( name );
    if( iter == child->end() ) {
      return false;
    }
    return true;
  }
  void spv_member_pointer::to_json( nlohmann::json &dest ) const {
    dest = nlohmann::json::object();
    dest[ "begin" ] = begin_;
    dest[ "aligned_size" ] = aligned_size;
    if( stride != 0 ) {
      dest[ "count" ] = count;
      dest[ "stride" ] = stride;
    }
    if( child ) {
      dest[ "child" ] = nlohmann::json::object();
      for( const auto &c: *child ) {
        dest[ "child" ][ c.first ] = c.second;
      }
      if( impl->cur_child != child->cend() ) {
        dest[ "cur_child" ] = std::distance( child->cbegin(), impl->cur_child );
      }
    }
    else {
      dest[ "numeric" ] = numeric;
    }
  }
  void to_json( nlohmann::json &dest, const spv_member_pointer &src ) {
    src.to_json( dest );
  }
#if 0
  spv_member_pointer::spv_member_pointer(
    std::uint8_t *b,
    std::uint8_t *e,
    const SpvReflectTypeDescription *t
  ) : begin_( b ), end_( e ), type( t ) {
  }
#define LIBGCT_SPVTYPE_CHECK( cpptype, spv_member_pointerenum ) \
    if( type->type_flags != ( spv_member_pointerenum ) ) { \
      const char *name = type->struct_member_name ? type->struct_member_name : "(unknown)"; \
      const char *tname = type->type_name ? type->type_name : "(unknown)"; \
      throw exception::invalid_argument( std::string( "spv_member_pointer::operator= : " #cpptype " is not compatible to " ) + name + " which has type " + tname + "(" + std::to_string( int( type->type_flags ) ) + ") in SPIR-V", __FILE__, __LINE__ ); \
    } \
    if( std::distance( begin_, end_ ) <= sizeof( cpptype ) ) { \
      const char *name = type->struct_member_name ? type->struct_member_name : "(unknown)"; \
      const char *tname = type->type_name ? type->type_name : "(unknown)"; \
      throw exception::invalid_argument( std::string( "spv_member_pointer::operator= : The buffer doesn't have enough space for " #cpptype " value at " ) + name + " which has type " + tname + "(" + std::to_string( int( type->type_flags ) ) + ") in SPIR-V", __FILE__, __LINE__ ); \
    }
#define LIBGCT_SPVTYPE_CHECK_SIZE( cpptype ) \
    if( type->traits.numeric.scalar.width != sizeof( cpptype ) ) { \
      const char *name = type->struct_member_name ? type->struct_member_name : "(unknown)"; \
      const char *tname = type->type_name ? type->type_name : "(unknown)"; \
      throw exception::invalid_argument( std::string( "spv_member_pointer::operator= : " #cpptype " is not compatible to " ) + name + " which has type " + tname + "(" + std::to_string( int( type->type_flags ) ) + ") in SPIR-V", __FILE__, __LINE__ ); \
    }

      const char *name = type->struct_member_name ? type->struct_member_name : "(unknown)"; \
      const char *tname = type->type_name ? type->type_name : "(unknown)"; \
      throw exception::invalid_argument( std::string( "spv_member_pointer::operator= : " #cpptype " is not compatible to " ) + name + " which has type " + tname + "(" + std::to_string( int( type->type_flags ) ) + ") in SPIR-V", __FILE__, __LINE__ ); \
    } \
    if( type->traits.numeric.scalar.width * type->traits.numeric.vector.component_count != sizeof( cpptype ) ) { \
      const char *name = type->struct_member_name ? type->struct_member_name : "(unknown)"; \
      const char *tname = type->type_name ? type->type_name : "(unknown)"; \
      throw exception::invalid_argument( std::string( "spv_member_pointer::operator= : " #cpptype " is not compatible to " ) + name + " which has type " + tname + "(" + std::to_string( int( type->type_flags ) ) + ") in SPIR-V", __FILE__, __LINE__ ); \
    }
#define LIBGCT_SPVTYPE_CHECK_MATRIX_SIZE( cpptype, row, col ) \
    if( type->traits.numeric.scalar.width != sizeof( cpptype :: value_type ) ) { \
      const char *name = type->struct_member_name ? type->struct_member_name : "(unknown)"; \
      const char *tname = type->type_name ? type->type_name : "(unknown)"; \
      throw exception::invalid_argument( std::string( "spv_member_pointer::operator= : " #cpptype " is not compatible to " ) + name + " which has type " + tname + "(" + std::to_string( int( type->type_flags ) ) + ") in SPIR-V", __FILE__, __LINE__ ); \
    } \
    if( type->traits.numeric.matrix.column_count != col ) { \
      const char *name = type->struct_member_name ? type->struct_member_name : "(unknown)"; \
      const char *tname = type->type_name ? type->type_name : "(unknown)"; \
      throw exception::invalid_argument( std::string( "spv_member_pointer::operator= : " #cpptype " is not compatible to " ) + name + " which has type " + tname + "(" + std::to_string( int( type->type_flags ) ) + ") in SPIR-V", __FILE__, __LINE__ ); \
    } \
    if( type->traits.numeric.matrix.row_count != row ) { \
      const char *name = type->struct_member_name ? type->struct_member_name : "(unknown)"; \
      const char *tname = type->type_name ? type->type_name : "(unknown)"; \
      throw exception::invalid_argument( std::string( "spv_member_pointer::operator= : " #cpptype " is not compatible to " ) + name + " which has type " + tname + "(" + std::to_string( int( type->type_flags ) ) + ") in SPIR-V", __FILE__, __LINE__ ); \
    }

  spv_member_pointer &spv_member_pointer::operator=( bool v ) {
    LIBGCT_SPVTYPE_CHECK( bool, SPV_REFLECT_TYPE_FLAG_BOOL )
    *reinterpret_cast< bool* >( begin_ ) = v;
    return *this;
  }
  spv_member_pointer &spv_member_pointer::operator=( std::int8_t v ) {
    LIBGCT_SPVTYPE_CHECK( std::int8_t, SPV_REFLECT_TYPE_FLAG_INT )
    if( type->traits.numeric.scalar.width == 64 ) {
      return (*this) = std::int64_t( v );
    }
    if( type->traits.numeric.scalar.width == 32 ) {
      return (*this) = std::int32_t( v );
    }
    if( type->traits.numeric.scalar.width == 16 ) {
      return (*this) = std::int16_t( v );
    }
    LIBGCT_SPVTYPE_CHECK_SIZE( std::int8_t );
    if( !type->traits.numeric.scalar.signedness ) {
      return (*this) = std::uint8_t( v );
    }
    *reinterpret_cast< std::int8_t* >( begin_ ) = v;
    return *this;
  }
  spv_member_pointer &spv_member_pointer::operator=( std::int16_t v ) {
    LIBGCT_SPVTYPE_CHECK( std::int16_t, SPV_REFLECT_TYPE_FLAG_INT )
    if( type->traits.numeric.scalar.width == 64 ) {
      return (*this) = std::int64_t( v );
    }
    if( type->traits.numeric.scalar.width == 32 ) {
      return (*this) = std::int32_t( v );
    }
    LIBGCT_SPVTYPE_CHECK_SIZE( std::int16_t );
    if( !type->traits.numeric.scalar.signedness ) {
      return (*this) = std::uint16_t( v );
    }
    *reinterpret_cast< std::int16_t* >( begin_ ) = v;
    return *this;
  }
  spv_member_pointer &spv_member_pointer::operator=( std::int32_t v ) {
    LIBGCT_SPVTYPE_CHECK( std::int32_t, SPV_REFLECT_TYPE_FLAG_INT )
    if( type->traits.numeric.scalar.width == 64 ) {
      return (*this) = std::int64_t( v );
    }
    LIBGCT_SPVTYPE_CHECK_SIZE( std::int32_t );
    if( !type->traits.numeric.scalar.signedness ) {
      return (*this) = std::uint32_t( v );
    }
    *reinterpret_cast< std::int32_t* >( begin_ ) = v;
    return *this;
  }
  spv_member_pointer &spv_member_pointer::operator=( std::int64_t v ) {
    LIBGCT_SPVTYPE_CHECK( std::int64_t, SPV_REFLECT_TYPE_FLAG_INT )
    LIBGCT_SPVTYPE_CHECK_SIZE( std::int64_t );
    if( !type->traits.numeric.scalar.signedness ) {
      return (*this) = std::uint64_t( v );
    }
    *reinterpret_cast< std::int64_t* >( begin_ ) = v;
    return *this;
  }
  spv_member_pointer &spv_member_pointer::operator=( std::uint8_t v ) {
    LIBGCT_SPVTYPE_CHECK( std::uint8_t, SPV_REFLECT_TYPE_FLAG_INT )
    if( type->traits.numeric.scalar.width == 64 ) {
      return (*this) = std::uint64_t( v );
    }
    if( type->traits.numeric.scalar.width == 32 ) {
      return (*this) = std::uint32_t( v );
    }
    if( type->traits.numeric.scalar.width == 16 ) {
      return (*this) = std::uint16_t( v );
    }
    LIBGCT_SPVTYPE_CHECK_SIZE( std::uint8_t );
    if( type->traits.numeric.scalar.signedness ) {
      return (*this) = std::int8_t( v );
    }
    *reinterpret_cast< std::uint8_t* >( begin_ ) = v;
    return *this;
  }
  spv_member_pointer &spv_member_pointer::operator=( std::uint16_t v ) {
    LIBGCT_SPVTYPE_CHECK( std::uint16_t, SPV_REFLECT_TYPE_FLAG_INT )
    if( type->traits.numeric.scalar.width == 64 ) {
      return (*this) = std::uint64_t( v );
    }
    if( type->traits.numeric.scalar.width == 32 ) {
      return (*this) = std::uint32_t( v );
    }
    LIBGCT_SPVTYPE_CHECK_SIZE( std::uint16_t );
    if( type->traits.numeric.scalar.signedness ) {
      return (*this) = std::int16_t( v );
    }
    *reinterpret_cast< std::uint16_t* >( begin_ ) = v;
    return *this;
  }
  spv_member_pointer &spv_member_pointer::operator=( std::uint32_t v ) {
    LIBGCT_SPVTYPE_CHECK( std::uint32_t, SPV_REFLECT_TYPE_FLAG_INT )
    if( type->traits.numeric.scalar.width == 64 ) {
      return (*this) = std::uint64_t( v );
    }
    LIBGCT_SPVTYPE_CHECK_SIZE( std::uint32_t );
    if( type->traits.numeric.scalar.signedness ) {
      return (*this) = std::int32_t( v );
    }
    *reinterpret_cast< std::uint32_t* >( begin_ ) = v;
    return *this;
  }
  spv_member_pointer &spv_member_pointer::operator=( std::uint64_t v ) {
    LIBGCT_SPVTYPE_CHECK( std::uint64_t, SPV_REFLECT_TYPE_FLAG_INT )
    LIBGCT_SPVTYPE_CHECK_SIZE( std::uint64_t );
    if( type->traits.numeric.scalar.signedness ) {
      return (*this) = std::int64_t( v );
    }
    *reinterpret_cast< std::uint64_t* >( begin_ ) = v;
    return *this;
  }
  spv_member_pointer &spv_member_pointer::operator=( float v ) {
    LIBGCT_SPVTYPE_CHECK( float, SPV_REFLECT_TYPE_FLAG_FLOAT )
    if( type->traits.numeric.scalar.width == 64 ) {
      return (*this) = double( v );
    }
    LIBGCT_SPVTYPE_CHECK_SIZE( float );
    *reinterpret_cast< float* >( begin_ ) = v;
    return *this;
  }
  spv_member_pointer &spv_member_pointer::operator=( double v ) {
    LIBGCT_SPVTYPE_CHECK( double, SPV_REFLECT_TYPE_FLAG_FLOAT )
    LIBGCT_SPVTYPE_CHECK_SIZE( double );
    *reinterpret_cast< double* >( begin_ ) = v;
    return *this;
  }
  spv_member_pointer &spv_member_pointer::operator=( const glm::vec2 &v ) {
    LIBGCT_SPVTYPE_CHECK( glm::vec2, SPV_REFLECT_TYPE_FLAG_FLOAT|SPV_REFLECT_TYPE_FLAG_VECTOR );
    LIBGCT_SPVTYPE_CHECK_VECTOR_SIZE( glm::vec2 );
    *reinterpret_cast< glm::vec2* >( begin_ ) = v;
    return *this;
  }
  spv_member_pointer &spv_member_pointer::operator=( const glm::vec3 &v ) {
    LIBGCT_SPVTYPE_CHECK( glm::vec3, SPV_REFLECT_TYPE_FLAG_FLOAT|SPV_REFLECT_TYPE_FLAG_VECTOR );
    LIBGCT_SPVTYPE_CHECK_VECTOR_SIZE( glm::vec3 );
    *reinterpret_cast< glm::vec3* >( begin_ ) = v;
    return *this;
  }
  spv_member_pointer &spv_member_pointer::operator=( const glm::vec4 &v ) {
    LIBGCT_SPVTYPE_CHECK( glm::vec4, SPV_REFLECT_TYPE_FLAG_FLOAT|SPV_REFLECT_TYPE_FLAG_VECTOR );
    LIBGCT_SPVTYPE_CHECK_VECTOR_SIZE( glm::vec4 );
    *reinterpret_cast< glm::vec4* >( begin_ ) = v;
    return *this;
  }
  spv_member_pointer &spv_member_pointer::operator=( const glm::ivec2 &v ) {
    LIBGCT_SPVTYPE_CHECK( glm::ivec2, SPV_REFLECT_TYPE_FLAG_INT|SPV_REFLECT_TYPE_FLAG_VECTOR );
    LIBGCT_SPVTYPE_CHECK_VECTOR_SIZE( glm::ivec2 );
    *reinterpret_cast< glm::ivec2* >( begin_ ) = v;
    return *this;
  }
  spv_member_pointer &spv_member_pointer::operator=( const glm::ivec3 &v ) {
    LIBGCT_SPVTYPE_CHECK( glm::ivec3, SPV_REFLECT_TYPE_FLAG_INT|SPV_REFLECT_TYPE_FLAG_VECTOR );
    LIBGCT_SPVTYPE_CHECK_VECTOR_SIZE( glm::ivec3 );
    *reinterpret_cast< glm::ivec3* >( begin_ ) = v;
    return *this;
  }
  spv_member_pointer &spv_member_pointer::operator=( const glm::ivec4 &v ) {
    LIBGCT_SPVTYPE_CHECK( glm::ivec4, SPV_REFLECT_TYPE_FLAG_INT|SPV_REFLECT_TYPE_FLAG_VECTOR );
    LIBGCT_SPVTYPE_CHECK_VECTOR_SIZE( glm::ivec4 );
    *reinterpret_cast< glm::ivec4* >( begin_ ) = v;
    return *this;
  }
  spv_member_pointer &spv_member_pointer::operator=( const glm::uvec2 &v ) {
    LIBGCT_SPVTYPE_CHECK( glm::uvec2, SPV_REFLECT_TYPE_FLAG_INT|SPV_REFLECT_TYPE_FLAG_VECTOR );
    LIBGCT_SPVTYPE_CHECK_VECTOR_SIZE( glm::uvec2 );
    *reinterpret_cast< glm::uvec2* >( begin_ ) = v;
    return *this;
  }
  spv_member_pointer &spv_member_pointer::operator=( const glm::uvec3 &v ) {
    LIBGCT_SPVTYPE_CHECK( glm::uvec3, SPV_REFLECT_TYPE_FLAG_INT|SPV_REFLECT_TYPE_FLAG_VECTOR );
    LIBGCT_SPVTYPE_CHECK_VECTOR_SIZE( glm::uvec3 );
    *reinterpret_cast< glm::uvec3* >( begin_ ) = v;
    return *this;
  }
  spv_member_pointer &spv_member_pointer::operator=( const glm::uvec4 &v ) {
    LIBGCT_SPVTYPE_CHECK( glm::uvec4, SPV_REFLECT_TYPE_FLAG_INT|SPV_REFLECT_TYPE_FLAG_VECTOR );
    LIBGCT_SPVTYPE_CHECK_VECTOR_SIZE( glm::uvec4 );
    *reinterpret_cast< glm::uvec4* >( begin_ ) = v;
    return *this;
  }
  spv_member_pointer &spv_member_pointer::operator=( const glm::bvec2 &v ) {
    LIBGCT_SPVTYPE_CHECK( glm::bvec2, SPV_REFLECT_TYPE_FLAG_INT|SPV_REFLECT_TYPE_FLAG_VECTOR );
    LIBGCT_SPVTYPE_CHECK_VECTOR_SIZE( glm::bvec2 );
    *reinterpret_cast< glm::bvec2* >( begin_ ) = v;
    return *this;
  }
  spv_member_pointer &spv_member_pointer::operator=( const glm::bvec3 &v ) {
    LIBGCT_SPVTYPE_CHECK( glm::bvec3, SPV_REFLECT_TYPE_FLAG_INT|SPV_REFLECT_TYPE_FLAG_VECTOR );
    LIBGCT_SPVTYPE_CHECK_VECTOR_SIZE( glm::bvec3 );
    *reinterpret_cast< glm::bvec3* >( begin_ ) = v;
    return *this;
  }
  spv_member_pointer &spv_member_pointer::operator=( const glm::bvec4 &v ) {
    LIBGCT_SPVTYPE_CHECK( glm::bvec4, SPV_REFLECT_TYPE_FLAG_INT|SPV_REFLECT_TYPE_FLAG_VECTOR );
    LIBGCT_SPVTYPE_CHECK_VECTOR_SIZE( glm::bvec4 );
    *reinterpret_cast< glm::bvec4* >( begin_ ) = v;
    return *this;
  }
  spv_member_pointer &spv_member_pointer::operator=( const glm::dvec2 &v ) {
    LIBGCT_SPVTYPE_CHECK( glm::dvec2, SPV_REFLECT_TYPE_FLAG_FLOAT|SPV_REFLECT_TYPE_FLAG_VECTOR );
    LIBGCT_SPVTYPE_CHECK_VECTOR_SIZE( glm::dvec2 );
    *reinterpret_cast< glm::dvec2* >( begin_ ) = v;
    return *this;
  }
  spv_member_pointer &spv_member_pointer::operator=( const glm::dvec3 &v ) {
    LIBGCT_SPVTYPE_CHECK( glm::dvec3, SPV_REFLECT_TYPE_FLAG_FLOAT|SPV_REFLECT_TYPE_FLAG_VECTOR );
    LIBGCT_SPVTYPE_CHECK_VECTOR_SIZE( glm::dvec3 );
    *reinterpret_cast< glm::dvec3* >( begin_ ) = v;
    return *this;
  }
  spv_member_pointer &spv_member_pointer::operator=( const glm::dvec4 &v ) {
    LIBGCT_SPVTYPE_CHECK( glm::dvec4, SPV_REFLECT_TYPE_FLAG_FLOAT|SPV_REFLECT_TYPE_FLAG_VECTOR );
    LIBGCT_SPVTYPE_CHECK_VECTOR_SIZE( glm::dvec4 );
    *reinterpret_cast< glm::dvec4* >( begin_ ) = v;
    return *this;
  }
  spv_member_pointer &spv_member_pointer::operator=( const glm::mat2 &v ) {
    LIBGCT_SPVTYPE_CHECK( glm::mat2, SPV_REFLECT_TYPE_FLAG_FLOAT|SPV_REFLECT_TYPE_FLAG_MATRIX );
    LIBGCT_SPVTYPE_CHECK_MATRIX_SIZE( glm::mat2, 2, 2 );
    *reinterpret_cast< glm::mat2* >( begin_ ) = v;
    return *this;
  }
  spv_member_pointer &spv_member_pointer::operator=( const glm::mat3 &v ) {
    LIBGCT_SPVTYPE_CHECK( glm::mat3, SPV_REFLECT_TYPE_FLAG_FLOAT|SPV_REFLECT_TYPE_FLAG_MATRIX );
    LIBGCT_SPVTYPE_CHECK_MATRIX_SIZE( glm::mat3, 3, 3 );
    *reinterpret_cast< glm::mat3* >( begin_ ) = v;
    return *this;
  }
  spv_member_pointer &spv_member_pointer::operator=( const glm::mat4 &v ) {
    LIBGCT_SPVTYPE_CHECK( glm::mat4, SPV_REFLECT_TYPE_FLAG_FLOAT|SPV_REFLECT_TYPE_FLAG_MATRIX );
    LIBGCT_SPVTYPE_CHECK_MATRIX_SIZE( glm::mat4, 4, 4 );
    *reinterpret_cast< glm::mat4* >( begin_ ) = v;
    return *this;
  }
  spv_member_pointer &spv_member_pointer::operator=( const glm::mat2x3 &v ) {
    LIBGCT_SPVTYPE_CHECK( glm::mat2x3, SPV_REFLECT_TYPE_FLAG_FLOAT|SPV_REFLECT_TYPE_FLAG_MATRIX );
    LIBGCT_SPVTYPE_CHECK_MATRIX_SIZE( glm::mat2x3, 2, 3 );
    *reinterpret_cast< glm::mat2x3* >( begin_ ) = v;
    return *this;
  }
  spv_member_pointer &spv_member_pointer::operator=( const glm::mat3x2 &v ) {
    LIBGCT_SPVTYPE_CHECK( glm::mat3x2, SPV_REFLECT_TYPE_FLAG_FLOAT|SPV_REFLECT_TYPE_FLAG_MATRIX );
    LIBGCT_SPVTYPE_CHECK_MATRIX_SIZE( glm::mat3x2, 3, 2 );
    *reinterpret_cast< glm::mat3x2* >( begin_ ) = v;
    return *this;
  }
  spv_member_pointer &spv_member_pointer::operator=( const glm::mat2x4 &v ) {
    LIBGCT_SPVTYPE_CHECK( glm::mat2x4, SPV_REFLECT_TYPE_FLAG_FLOAT|SPV_REFLECT_TYPE_FLAG_MATRIX );
    LIBGCT_SPVTYPE_CHECK_MATRIX_SIZE( glm::mat2x4, 2, 4 );
    *reinterpret_cast< glm::mat2x4* >( begin_ ) = v;
    return *this;
  }
  spv_member_pointer &spv_member_pointer::operator=( const glm::mat4x2 &v ) {
    LIBGCT_SPVTYPE_CHECK( glm::mat2x4, SPV_REFLECT_TYPE_FLAG_FLOAT|SPV_REFLECT_TYPE_FLAG_MATRIX );
    LIBGCT_SPVTYPE_CHECK_MATRIX_SIZE( glm::mat2x4, 4, 2 );
    *reinterpret_cast< glm::mat2x4* >( begin_ ) = v;
    return *this;
  }
  spv_member_pointer &spv_member_pointer::operator=( const glm::mat3x4 &v ) {
    LIBGCT_SPVTYPE_CHECK( glm::mat3x4, SPV_REFLECT_TYPE_FLAG_FLOAT|SPV_REFLECT_TYPE_FLAG_MATRIX );
    LIBGCT_SPVTYPE_CHECK_MATRIX_SIZE( glm::mat3x4, 3, 4 );
    *reinterpret_cast< glm::mat3x4* >( begin_ ) = v;
    return *this;
  }
  spv_member_pointer &spv_member_pointer::operator=( const glm::mat4x3 &v ) {
    LIBGCT_SPVTYPE_CHECK( glm::mat4x3, SPV_REFLECT_TYPE_FLAG_FLOAT|SPV_REFLECT_TYPE_FLAG_MATRIX );
    LIBGCT_SPVTYPE_CHECK_MATRIX_SIZE( glm::mat4x3, 4, 3 );
    *reinterpret_cast< glm::mat4x3* >( begin_ ) = v;
    return *this;
  }
  spv_member_pointer &spv_member_pointer::operator=( const glm::dmat2 &v ) {
    LIBGCT_SPVTYPE_CHECK( glm::dmat2, SPV_REFLECT_TYPE_FLAG_FLOAT|SPV_REFLECT_TYPE_FLAG_MATRIX );
    LIBGCT_SPVTYPE_CHECK_MATRIX_SIZE( glm::dmat2, 2, 2 );
    *reinterpret_cast< glm::dmat2* >( begin_ ) = v;
    return *this;
  }
  spv_member_pointer &spv_member_pointer::operator=( const glm::dmat3 &v ) {
    LIBGCT_SPVTYPE_CHECK( glm::dmat3, SPV_REFLECT_TYPE_FLAG_FLOAT|SPV_REFLECT_TYPE_FLAG_MATRIX );
    LIBGCT_SPVTYPE_CHECK_MATRIX_SIZE( glm::dmat3, 3, 3 );
    *reinterpret_cast< glm::dmat3* >( begin_ ) = v;
    return *this;
  }
  spv_member_pointer &spv_member_pointer::operator=( const glm::dmat4 &v ) {
    LIBGCT_SPVTYPE_CHECK( glm::dmat4, SPV_REFLECT_TYPE_FLAG_FLOAT|SPV_REFLECT_TYPE_FLAG_MATRIX );
    LIBGCT_SPVTYPE_CHECK_MATRIX_SIZE( glm::dmat4, 4, 4 );
    *reinterpret_cast< glm::dmat4* >( begin_ ) = v;
    return *this;
  }
  spv_member_pointer &spv_member_pointer::operator=( const glm::dmat2x3 &v ) {
    LIBGCT_SPVTYPE_CHECK( glm::dmat2x3, SPV_REFLECT_TYPE_FLAG_FLOAT|SPV_REFLECT_TYPE_FLAG_MATRIX );
    LIBGCT_SPVTYPE_CHECK_MATRIX_SIZE( glm::dmat2x3, 2, 3 );
    *reinterpret_cast< glm::dmat2x3* >( begin_ ) = v;
    return *this;
  }
  spv_member_pointer &spv_member_pointer::operator=( const glm::dmat3x2 &v ) {
    LIBGCT_SPVTYPE_CHECK( glm::dmat3x2, SPV_REFLECT_TYPE_FLAG_FLOAT|SPV_REFLECT_TYPE_FLAG_MATRIX );
    LIBGCT_SPVTYPE_CHECK_MATRIX_SIZE( glm::dmat3x2, 3, 2 );
    *reinterpret_cast< glm::dmat3x2* >( begin_ ) = v;
    return *this;
  }
  spv_member_pointer &spv_member_pointer::operator=( const glm::dmat2x4 &v ) {
    LIBGCT_SPVTYPE_CHECK( glm::dmat2x4, SPV_REFLECT_TYPE_FLAG_FLOAT|SPV_REFLECT_TYPE_FLAG_MATRIX );
    LIBGCT_SPVTYPE_CHECK_MATRIX_SIZE( glm::dmat2x4, 2, 4 );
    *reinterpret_cast< glm::dmat2x4* >( begin_ ) = v;
    return *this;
  }
  spv_member_pointer &spv_member_pointer::operator=( const glm::dmat4x2 &v ) {
    LIBGCT_SPVTYPE_CHECK( glm::dmat2x4, SPV_REFLECT_TYPE_FLAG_FLOAT|SPV_REFLECT_TYPE_FLAG_MATRIX );
    LIBGCT_SPVTYPE_CHECK_MATRIX_SIZE( glm::dmat2x4, 4, 2 );
    *reinterpret_cast< glm::dmat2x4* >( begin_ ) = v;
    return *this;
  }
  spv_member_pointer &spv_member_pointer::operator=( const glm::dmat3x4 &v ) {
    LIBGCT_SPVTYPE_CHECK( glm::dmat3x4, SPV_REFLECT_TYPE_FLAG_FLOAT|SPV_REFLECT_TYPE_FLAG_MATRIX );
    LIBGCT_SPVTYPE_CHECK_MATRIX_SIZE( glm::dmat3x4, 3, 4 );
    *reinterpret_cast< glm::dmat3x4* >( begin_ ) = v;
    return *this;
  }
  spv_member_pointer &spv_member_pointer::operator=( const glm::dmat4x3 &v ) {
    LIBGCT_SPVTYPE_CHECK( glm::dmat4x3, SPV_REFLECT_TYPE_FLAG_FLOAT|SPV_REFLECT_TYPE_FLAG_MATRIX );
    LIBGCT_SPVTYPE_CHECK_MATRIX_SIZE( glm::dmat4x3, 4, 3 );
    *reinterpret_cast< glm::dmat4x3* >( begin_ ) = v;
    return *this;
  }
  spv_member_pointer::operator bool() const {
    LIBGCT_SPVTYPE_CHECK( bool, SPV_REFLECT_TYPE_FLAG_BOOL )
    return *reinterpret_cast< bool* >( begin_ );
  }
  spv_member_pointer::operator std::int8_t() const {
    LIBGCT_SPVTYPE_CHECK( std::int8_t, SPV_REFLECT_TYPE_FLAG_INT )
    if( type->traits.numeric.scalar.width == 64 ) {
      return std::int64_t( *this );
    }
    if( type->traits.numeric.scalar.width == 32 ) {
      return std::int32_t( *this );
    }
    if( type->traits.numeric.scalar.width == 16 ) {
      return std::int16_t( *this );
    }
    LIBGCT_SPVTYPE_CHECK_SIZE( std::int8_t );
    if( !type->traits.numeric.scalar.signedness ) {
      return std::uint8_t( *this );
    }
    return *reinterpret_cast< std::int8_t* >( begin_ );
  }
  spv_member_pointer::operator std::int16_t() const {
    LIBGCT_SPVTYPE_CHECK( std::int16_t, SPV_REFLECT_TYPE_FLAG_INT )
    if( type->traits.numeric.scalar.width == 64 ) {
      return std::int64_t( *this );
    }
    if( type->traits.numeric.scalar.width == 32 ) {
      return std::int32_t( *this );
    }
    LIBGCT_SPVTYPE_CHECK_SIZE( std::int16_t );
    if( !type->traits.numeric.scalar.signedness ) {
      return std::uint8_t( *this );
    }
    return *reinterpret_cast< std::int16_t* >( begin_ );
  }
  spv_member_pointer::operator std::int32_t() const {
    LIBGCT_SPVTYPE_CHECK( std::int32_t, SPV_REFLECT_TYPE_FLAG_INT )
    if( type->traits.numeric.scalar.width == 64 ) {
      return std::int64_t( *this );
    }
    LIBGCT_SPVTYPE_CHECK_SIZE( std::int32_t );
    if( !type->traits.numeric.scalar.signedness ) {
      return std::uint8_t( *this );
    }
    return *reinterpret_cast< std::int32_t* >( begin_ );
  }
  spv_member_pointer::operator std::int64_t() const {
    LIBGCT_SPVTYPE_CHECK( std::int64_t, SPV_REFLECT_TYPE_FLAG_INT )
    LIBGCT_SPVTYPE_CHECK_SIZE( std::int64_t );
    if( !type->traits.numeric.scalar.signedness ) {
      return std::uint8_t( *this );
    }
    return *reinterpret_cast< std::int64_t* >( begin_ );
  }
  spv_member_pointer::operator std::uint8_t() const {
    LIBGCT_SPVTYPE_CHECK( std::uint8_t, SPV_REFLECT_TYPE_FLAG_INT )
    if( type->traits.numeric.scalar.width == 64 ) {
      return std::uint64_t( *this );
    }
    if( type->traits.numeric.scalar.width == 32 ) {
      return std::uint32_t( *this );
    }
    if( type->traits.numeric.scalar.width == 16 ) {
      return std::uint16_t( *this );
    }
    LIBGCT_SPVTYPE_CHECK_SIZE( std::uint8_t );
    if( type->traits.numeric.scalar.signedness ) {
      return std::int8_t( *this );
    }
    return *reinterpret_cast< std::uint8_t* >( begin_ );
  }
  spv_member_pointer::operator std::uint16_t() const {
    LIBGCT_SPVTYPE_CHECK( std::uint16_t, SPV_REFLECT_TYPE_FLAG_INT )
    if( type->traits.numeric.scalar.width == 64 ) {
      return std::uint64_t( *this );
    }
    if( type->traits.numeric.scalar.width == 32 ) {
      return std::uint32_t( *this );
    }
    LIBGCT_SPVTYPE_CHECK_SIZE( std::uint16_t );
    if( type->traits.numeric.scalar.signedness ) {
      return std::int16_t( *this );
    }
    return *reinterpret_cast< std::uint16_t* >( begin_ );
  }
  spv_member_pointer::operator std::uint32_t() const {
    LIBGCT_SPVTYPE_CHECK( std::uint32_t, SPV_REFLECT_TYPE_FLAG_INT )
    if( type->traits.numeric.scalar.width == 64 ) {
      return std::uint64_t( *this );
    }
    LIBGCT_SPVTYPE_CHECK_SIZE( std::uint32_t );
    if( type->traits.numeric.scalar.signedness ) {
      return std::int32_t( *this );
    }
    return *reinterpret_cast< std::uint32_t* >( begin_ );
  }
  spv_member_pointer::operator std::uint64_t() const {
    LIBGCT_SPVTYPE_CHECK( std::uint64_t, SPV_REFLECT_TYPE_FLAG_INT )
    LIBGCT_SPVTYPE_CHECK_SIZE( std::uint64_t );
    if( type->traits.numeric.scalar.signedness ) {
      return std::int64_t( *this );
    }
    return *reinterpret_cast< std::uint64_t* >( begin_ );
  }
  spv_member_pointer::operator float() const {
    LIBGCT_SPVTYPE_CHECK( float, SPV_REFLECT_TYPE_FLAG_FLOAT )
    LIBGCT_SPVTYPE_CHECK_SIZE( float );
    return *reinterpret_cast< float* >( begin_ );
  }
  spv_member_pointer::operator double() const {
    LIBGCT_SPVTYPE_CHECK( double, SPV_REFLECT_TYPE_FLAG_FLOAT )
    LIBGCT_SPVTYPE_CHECK_SIZE( double );
    return *reinterpret_cast< double* >( begin_ );
  }
  spv_member_pointer::operator glm::vec2() const {
    LIBGCT_SPVTYPE_CHECK( glm::vec2, SPV_REFLECT_TYPE_FLAG_FLOAT|SPV_REFLECT_TYPE_FLAG_VECTOR )
    LIBGCT_SPVTYPE_CHECK_VECTOR_SIZE( glm::vec2 );
    return *reinterpret_cast< glm::vec2* >( begin_ );
  }
  spv_member_pointer::operator glm::vec3() const {
    LIBGCT_SPVTYPE_CHECK( glm::vec3, SPV_REFLECT_TYPE_FLAG_FLOAT|SPV_REFLECT_TYPE_FLAG_VECTOR )
    LIBGCT_SPVTYPE_CHECK_VECTOR_SIZE( glm::vec3 );
    return *reinterpret_cast< glm::vec3* >( begin_ );
  }
  spv_member_pointer::operator glm::vec4() const {
    LIBGCT_SPVTYPE_CHECK( glm::vec4, SPV_REFLECT_TYPE_FLAG_FLOAT|SPV_REFLECT_TYPE_FLAG_VECTOR )
    LIBGCT_SPVTYPE_CHECK_VECTOR_SIZE( glm::vec4 );
    return *reinterpret_cast< glm::vec4* >( begin_ );
  }
  spv_member_pointer::operator glm::ivec2() const {
    LIBGCT_SPVTYPE_CHECK( glm::ivec2, SPV_REFLECT_TYPE_FLAG_INT|SPV_REFLECT_TYPE_FLAG_VECTOR )
    LIBGCT_SPVTYPE_CHECK_VECTOR_SIZE( glm::ivec2 );
    return *reinterpret_cast< glm::ivec2* >( begin_ );
  }
  spv_member_pointer::operator glm::ivec3() const {
    LIBGCT_SPVTYPE_CHECK( glm::ivec3, SPV_REFLECT_TYPE_FLAG_INT|SPV_REFLECT_TYPE_FLAG_VECTOR )
    LIBGCT_SPVTYPE_CHECK_VECTOR_SIZE( glm::ivec3 );
    return *reinterpret_cast< glm::ivec3* >( begin_ );
  }
  spv_member_pointer::operator glm::ivec4() const {
    LIBGCT_SPVTYPE_CHECK( glm::ivec4, SPV_REFLECT_TYPE_FLAG_INT|SPV_REFLECT_TYPE_FLAG_VECTOR )
    LIBGCT_SPVTYPE_CHECK_VECTOR_SIZE( glm::ivec4 );
    return *reinterpret_cast< glm::ivec4* >( begin_ );
  }
  spv_member_pointer::operator glm::uvec2() const {
    LIBGCT_SPVTYPE_CHECK( glm::uvec2, SPV_REFLECT_TYPE_FLAG_INT|SPV_REFLECT_TYPE_FLAG_VECTOR )
    LIBGCT_SPVTYPE_CHECK_VECTOR_SIZE( glm::uvec2 );
    return *reinterpret_cast< glm::uvec2* >( begin_ );
  }
  spv_member_pointer::operator glm::uvec3() const {
    LIBGCT_SPVTYPE_CHECK( glm::uvec3, SPV_REFLECT_TYPE_FLAG_INT|SPV_REFLECT_TYPE_FLAG_VECTOR )
    LIBGCT_SPVTYPE_CHECK_VECTOR_SIZE( glm::uvec3 );
    return *reinterpret_cast< glm::uvec3* >( begin_ );
  }
  spv_member_pointer::operator glm::uvec4() const {
    LIBGCT_SPVTYPE_CHECK( glm::uvec4, SPV_REFLECT_TYPE_FLAG_INT|SPV_REFLECT_TYPE_FLAG_VECTOR )
    LIBGCT_SPVTYPE_CHECK_VECTOR_SIZE( glm::uvec4 );
    return *reinterpret_cast< glm::uvec4* >( begin_ );
  }
  spv_member_pointer::operator glm::bvec2() const {
    LIBGCT_SPVTYPE_CHECK( glm::bvec2, SPV_REFLECT_TYPE_FLAG_INT|SPV_REFLECT_TYPE_FLAG_VECTOR )
    LIBGCT_SPVTYPE_CHECK_VECTOR_SIZE( glm::bvec2 );
    return *reinterpret_cast< glm::bvec2* >( begin_ );
  }
  spv_member_pointer::operator glm::bvec3() const {
    LIBGCT_SPVTYPE_CHECK( glm::bvec3, SPV_REFLECT_TYPE_FLAG_INT|SPV_REFLECT_TYPE_FLAG_VECTOR )
    LIBGCT_SPVTYPE_CHECK_VECTOR_SIZE( glm::bvec3 );
    return *reinterpret_cast< glm::bvec3* >( begin_ );
  }
  spv_member_pointer::operator glm::bvec4() const {
    LIBGCT_SPVTYPE_CHECK( glm::bvec4, SPV_REFLECT_TYPE_FLAG_INT|SPV_REFLECT_TYPE_FLAG_VECTOR )
    LIBGCT_SPVTYPE_CHECK_VECTOR_SIZE( glm::bvec4 );
    return *reinterpret_cast< glm::bvec4* >( begin_ );
  }
  spv_member_pointer::operator glm::dvec2() const {
    LIBGCT_SPVTYPE_CHECK( glm::dvec2, SPV_REFLECT_TYPE_FLAG_FLOAT|SPV_REFLECT_TYPE_FLAG_VECTOR )
    LIBGCT_SPVTYPE_CHECK_VECTOR_SIZE( glm::dvec2 );
    return *reinterpret_cast< glm::dvec2* >( begin_ );
  }
  spv_member_pointer::operator glm::dvec3() const {
    LIBGCT_SPVTYPE_CHECK( glm::dvec3, SPV_REFLECT_TYPE_FLAG_FLOAT|SPV_REFLECT_TYPE_FLAG_VECTOR )
    LIBGCT_SPVTYPE_CHECK_VECTOR_SIZE( glm::dvec3 );
    return *reinterpret_cast< glm::dvec3* >( begin_ );
  }
  spv_member_pointer::operator glm::dvec4() const {
    LIBGCT_SPVTYPE_CHECK( glm::dvec4, SPV_REFLECT_TYPE_FLAG_FLOAT|SPV_REFLECT_TYPE_FLAG_VECTOR )
    LIBGCT_SPVTYPE_CHECK_VECTOR_SIZE( glm::dvec4 );
    return *reinterpret_cast< glm::dvec4* >( begin_ );
  }
  /*
  spv_member_pointer::operator glm::mat2() const;
  spv_member_pointer::operator glm::mat3() const;
  spv_member_pointer::operator glm::mat4() const;
  spv_member_pointer::operator glm::mat2x3() const;
  spv_member_pointer::operator glm::mat3x2() const;
  spv_member_pointer::operator glm::mat2x4() const;
  spv_member_pointer::operator glm::mat4x2() const;
  spv_member_pointer::operator glm::mat3x4() const;
  spv_member_pointer::operator glm::mat4x3() const;
  spv_member_pointer::operator glm::dmat2() const;
  spv_member_pointer::operator glm::dmat3() const;
  spv_member_pointer::operator glm::dmat4() const;
  spv_member_pointer::operator glm::dmat2x3() const;
  spv_member_pointer::operator glm::dmat3x2() const;
  spv_member_pointer::operator glm::dmat2x4() const;
  spv_member_pointer::operator glm::dmat4x2() const;
  spv_member_pointer::operator glm::dmat3x4() const;
  spv_member_pointer::operator glm::dmat4x3() const;
  spv_member_pointer spv_member_pointer::operator[]( const char* ) const;
  spv_member_pointer spv_member_pointer::operator[]( const std::string& ) const;
  spv_member_pointer spv_member_pointer::operator[]( int ) const;
  */
#endif
}


