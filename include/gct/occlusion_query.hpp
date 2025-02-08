#ifndef GCT_OCCLUSION_QUERY_HPP
#define GCT_OCCLUSION_QUERY_HPP
#include <cmath>
#include <unordered_map>
#include <memory>
#include <glm/mat4x4.hpp>
#include <gct/setter.hpp>
#include <gct/aabb.hpp>
#include <gct/occlusion_query_create_info.hpp>

namespace gct {
class image_view_t;
class graphics;
class gbuffer;
class command_buffer_recorder_t;
class query_pool_t;
using occlusion_query_id_t = std::uint32_t;
class basic_occlusion_query {
  struct push_constant_type {
    LIBGCT_SETTER( matrix )
    LIBGCT_SETTER( head )
    glm::mat4 matrix;
    std::uint32_t head = 0u;
  };
public:
  basic_occlusion_query(
    const occlusion_query_create_info&
  );
  occlusion_query_id_t push( const aabb4& );
  occlusion_query_id_t push( const std::vector< aabb4 >& );
  void operator()(
    command_buffer_recorder_t&,
    const glm::mat4 &matrix
  );
  [[nodiscard]] std::uint32_t operator[]( occlusion_query_id_t ) const;
  void reset();
  [[nodiscard]] const std::shared_ptr< image_view_t > &get_image_view() const;
  bool empty() const {
    return pushed.empty();
  }
private:
  occlusion_query_create_info props;
  std::shared_ptr< gbuffer > output;
  std::shared_ptr< graphics > pipeline;
  std::vector< std::vector< aabb4 > > pushed;
  mutable std::vector< std::uint32_t > result;
  std::shared_ptr< mappable_buffer_t > point_mesh;
  std::shared_ptr< query_pool_t > query_pool;
  mutable bool received = false;
  bool transfered = false;
  std::shared_ptr< mappable_buffer_t > aabb_buf;
};

template< typename T >
class occlusion_query : public basic_occlusion_query {
public:
  occlusion_query(
    const occlusion_query_create_info &ci
  ) : basic_occlusion_query( ci ) {}
  void push( const aabb4 &box, const T &v ) {
    occ_map.emplace( basic_occlusion_query::push( box ), v );
  }
  void push( const std::vector< aabb4 > &box, const T &v ) {
    occ_map.emplace( basic_occlusion_query::push( box ), v );
  }
  void get_result(
    std::vector< T > &dest
  ) {
    for( const auto &v: occ_map ) {
      if( (*this)[ v.first ] ) {
        dest.push_back( v.second );
      }
    }
  }
  void reset() {
    basic_occlusion_query::reset();
    occ_map.clear();
  }
private:
  std::unordered_map< occlusion_query_id_t, T > occ_map;
};

}

#endif

