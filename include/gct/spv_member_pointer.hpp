#ifndef GCT_SPV_MEMBER_POINTER_HPP
#define GCT_SPV_MEMBER_POINTER_HPP
#include <cstdint>
#include <variant>
#include <unordered_map>
#include <string>
#include <memory>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat2x2.hpp>
#include <glm/mat2x3.hpp>
#include <glm/mat3x2.hpp>
#include <glm/mat2x4.hpp>
#include <glm/mat4x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat3x4.hpp>
#include <glm/mat4x3.hpp>
#include <glm/mat4x4.hpp>
#include <nlohmann/json_fwd.hpp>
#include <gct/exception.hpp>
#include <gct/alignment.hpp>
#include <gct/numeric_types.hpp>
#include <gct/numeric_type_match.hpp>
#include <gct/deep_copy.hpp>

struct SpvReflectTypeDescription;
namespace gct {
class spv_member_pointer_impl;
class spv_member_pointer {
  using child_type = std::unordered_map< std::string, spv_member_pointer >;
public:
  using difference_type = child_type::difference_type;
  using value_type = spv_member_pointer;
  using reference = spv_member_pointer;
  using iterator_category = std::forward_iterator_tag;
  spv_member_pointer(
    std::size_t,
    const SpvReflectTypeDescription&,
    memory_layout layout
  );
  std::size_t get_aligned_size() const {
    return aligned_size;
  }
  std::size_t get_offset() const {
    return begin_;
  }
  std::size_t get_stride() const {
    return stride;
  }
  const numeric_type_t &get_numeric() const;
  const spv_member_pointer &operator[]( const std::string &name ) const;
  spv_member_pointer operator[]( int ) const;
  spv_member_pointer begin() const;
  spv_member_pointer end() const;
  spv_member_pointer &operator++();
  spv_member_pointer operator++( int );
  spv_member_pointer operator*() const;
  bool operator==( const spv_member_pointer& ) const;
  bool operator!=( const spv_member_pointer& ) const;
  void to_json( nlohmann::json& ) const;
private:
  std::size_t begin_;
  const SpvReflectTypeDescription *type;
  std::size_t aligned_size = 0u;
  std::size_t count = 0u;
  std::size_t stride = 0u;
  std::shared_ptr< child_type > child;
  deep_copy_unique_ptr< spv_member_pointer_impl > impl;
  numeric_type_t numeric;
};
class spv_member_pointer_impl {
  using child_type = std::unordered_map< std::string, spv_member_pointer >;
public:
  child_type::const_iterator cur_child;
};

void to_json( nlohmann::json&, const spv_member_pointer& );
template< typename T >
class spv_reference {
public:
  spv_reference( T h, const spv_member_pointer &p ) : head( h ), mp( p ) {
  }
  template< typename U >
  spv_reference &operator=( U &&v ) {
    const auto cur = std::next( reinterpret_cast< std::uint8_t* >( &*head ), mp.get_offset() );
    if( numeryc_type_match< U >()( mp.get_numeric() ) ) {
      *reinterpret_cast< U* >( cur ) = std::move( v );
    }
    else {
      throw exception::invalid_argument( "spv_reference::operator= : Incompatible value.", __FILE__, __LINE__ );
    }
    return *this;
  }
  template< typename U >
  spv_reference &operator=( const U &v ) {
    const auto cur = std::next( reinterpret_cast< std::uint8_t* >( &*head ), mp.get_offset() );
    if( numeryc_type_match< U >()( mp.get_numeric() ) ) {
      *reinterpret_cast< U* >( cur ) = v;
    }
    else {
      throw exception::invalid_argument( "spv_reference::operator= : Incompatible value.", __FILE__, __LINE__ );
    }
    return *this;
  }
  template< typename U >
  operator U& () const {
    const auto cur = std::next( reinterpret_cast< std::uint8_t* >( &*head ), mp.get_offset() );
    if( numeryc_type_match< U >()( mp.get_numeric() ) ) {
      return *reinterpret_cast< U* >( cur );
    }
    else {
      throw exception::invalid_argument( "spv_reference::operator U& : Incompatible value.", __FILE__, __LINE__ );
    }
  }
private:
  T head;
  spv_member_pointer mp;
};

template< typename T >
spv_reference< T > operator->*( const T &head, const spv_member_pointer &mp ) {
  return spv_reference< T >( head, mp );
}

}
#endif


