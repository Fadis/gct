#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <gct/shadow_map.hpp>

namespace gct {
  glm::vec3 scale_vec( const glm::vec4 &v ) {
    return glm::vec3( v[ 0 ]/v[ 3 ], v[ 1 ]/v[ 3 ], v[ 2 ]/v[ 3 ] );
  }

  std::array< glm::vec3, 8u > get_projection_box(
    glm::mat4 projection_matrix,
    glm::mat4 camera_matrix,
    float /*light_distance*/
  ) {
    glm::mat4 combined_matrix = projection_matrix * camera_matrix;
    glm::mat4 icm = glm::inverse( combined_matrix );
    const float depth_limit = 1.f/*std::max( std::min( light_distance, 1.f ), 0.1f )*/;
    return std::array< glm::vec3, 8u >{
      scale_vec( icm * glm::vec4( -1, -1, -depth_limit, 1 ) ),
      scale_vec( icm * glm::vec4( 1, -1, -depth_limit, 1 ) ),
      scale_vec( icm * glm::vec4( -1, 1, -depth_limit, 1 ) ),
      scale_vec( icm * glm::vec4( 1, 1, -depth_limit, 1 ) ),
      scale_vec( icm * glm::vec4( -1, -1, depth_limit, 1 ) ),
      scale_vec( icm * glm::vec4( 1, -1, depth_limit, 1 ) ),
      scale_vec( icm * glm::vec4( -1, 1, depth_limit, 1 ) ),
      scale_vec( icm * glm::vec4( 1, 1, depth_limit, 1 ) )
    };
  }

  std::tuple< glm::mat4, glm::mat4, float, float, float > get_aabb_light_matrix(
    const glm::vec3 &min,
    const glm::vec3 &max,
    const glm::vec3 &light_pos
  ) {
    std::array< glm::vec3, 8u > world{
      glm::vec3( min[ 0 ], min[ 1 ], min[ 2 ] ),
      glm::vec3( max[ 0 ], min[ 1 ], min[ 2 ] ),
      glm::vec3( min[ 0 ], max[ 1 ], min[ 2 ] ),
      glm::vec3( max[ 0 ], max[ 1 ], min[ 2 ] ),
      glm::vec3( min[ 0 ], min[ 1 ], max[ 2 ] ),
      glm::vec3( max[ 0 ], min[ 1 ], max[ 2 ] ),
      glm::vec3( min[ 0 ], max[ 1 ], max[ 2 ] ),
      glm::vec3( max[ 0 ], max[ 1 ], max[ 2 ] )
    };
    auto center = ( min + max ) / 2.f;
    auto up1 = glm::vec3( 0.f, 1000.f, 0.f );
    if( glm::dot( glm::normalize( light_pos - center ), glm::normalize( up1 - center ) ) > 0.8f )
      up1 = glm::vec3( 0.f, 0.f, 1000.f );
    auto light_view_matrix =
      glm::lookAt(
        light_pos,
        center,
        up1
      );
    for( auto &v: world ) v = scale_vec( light_view_matrix * glm::vec4( v, 1.f ) );
    float znear = std::numeric_limits< float >::max();
    float zfar = std::numeric_limits< float >::min();
    for( const auto &v: world ) {
      if( zfar < -v[ 2 ] ) zfar = -v[ 2 ];
      if( znear > -v[ 2 ] ) znear = -v[ 2 ];
    }
    znear *= 0.5f;
    for( auto &v: world ) v = v*( -znear/v[ 2 ] );
    float left = std::numeric_limits< float >::max();
    float right = std::numeric_limits< float >::min();
    float bottom = std::numeric_limits< float >::max();
    float top = std::numeric_limits< float >::min();
    for( const auto &v: world ) {
      if( right < v[ 0 ] ) right = v[ 0 ];
      if( left > v[ 0 ] ) left = v[ 0 ];
      if( top < v[ 1 ] ) top = v[ 1 ];
      if( bottom > v[ 1 ] ) bottom = v[ 1 ];
    }
    auto w = std::max( std::abs( left ), std::abs( right ) );
    auto light_projection_matrix = glm::frustum(
      left, right, bottom, top, znear, zfar
    );
    return std::make_tuple(
      std::move( light_projection_matrix ),
      std::move( light_view_matrix ),
      znear, zfar, w*2
    );
  }

  std::tuple< glm::mat4, glm::mat4, float, float, float > get_projection_light_matrix(
    const glm::mat4 &camera_projection_matrix,
    const glm::mat4 &camera_view_matrix,
    const glm::vec3 &light_pos,
    float distance_offset
  ) {
    auto projection_box = get_projection_box( camera_projection_matrix, camera_view_matrix, distance_offset );
    glm::vec3 proj_min = projection_box[ 0 ];
    glm::vec3 proj_max = projection_box[ 0 ];
    for( auto v: projection_box ) {
      if( v.x < proj_min.x ) proj_min.x = v.x;
      if( v.x > proj_max.x ) proj_max.x = v.x;
      if( v.y < proj_min.y ) proj_min.y = v.y;
      if( v.y > proj_max.y ) proj_max.y = v.y;
      if( v.z < proj_min.z ) proj_min.z = v.z;
      if( v.z > proj_max.z ) proj_max.z = v.z;
    }
    return get_aabb_light_matrix( proj_min, proj_max, light_pos ); 
  }
}

