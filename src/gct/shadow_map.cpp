#include <iostream>
#include <algorithm>
#include <numeric>
#include <array>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtx/string_cast.hpp>
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
    std::array< glm::vec3, 8u > vertices{
      scale_vec( icm * glm::vec4( -1, -1, -1, 1 ) ),
      scale_vec( icm * glm::vec4( 1, -1, -1, 1 ) ),
      scale_vec( icm * glm::vec4( -1, 1, -1, 1 ) ),
      scale_vec( icm * glm::vec4( 1, 1, -1, 1 ) ),
      scale_vec( icm * glm::vec4( -1, -1, 1, 1 ) ),
      scale_vec( icm * glm::vec4( 1, -1, 1, 1 ) ),
      scale_vec( icm * glm::vec4( -1, 1, 1, 1 ) ),
      scale_vec( icm * glm::vec4( 1, 1, 1, 1 ) )
    };
    return vertices;
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

  std::optional< std::tuple< glm::mat4, glm::mat4, float, float > > get_projection_light_matrix(
    const glm::mat4 &camera_projection_matrix,
    const glm::mat4 &camera_view_matrix,
    const glm::vec3 &light_pos,
    float distance_offset
  ) {
    auto projection_box = get_projection_box( camera_projection_matrix, camera_view_matrix, distance_offset );
    std::array< float, 8u > distance;
    std::transform(
      projection_box.begin(),
      projection_box.end(),
      distance.begin(),
      [&light_pos]( const auto &v ) {
        return glm::length( v - light_pos );
      }
    );
    const auto nearest = std::distance(
      distance.begin(),
      std::min_element( distance.begin(), distance.end() )
    );
    for( auto &v: projection_box ) {
      std::cout << glm::to_string( v ) << " ";
    }
    std::cout << std::endl;
    if( distance[nearest] < 0.001f ) {
      std::cout << __FILE__ << " " << __LINE__ << " " << distance[ nearest ] << " " << glm::to_string( projection_box[ nearest ] ) << " " << glm::to_string( light_pos ) << std::endl;
      return std::nullopt;
    }
    const auto farest = std::distance(
      distance.begin(),
      std::max_element( distance.begin(), distance.end() )
    );
    std::array< glm::vec3, 8u > equal_length;
    std::transform(
      projection_box.begin(),
      projection_box.end(),
      equal_length.begin(),
      [&light_pos]( const auto &v ) {
        const auto rel = v - light_pos;
        const auto len = glm::length( rel );
        return rel / len;
      }
    );
    auto center = std::accumulate(
      equal_length.begin(),
      equal_length.end(),
      glm::vec3( 0.f, 0.f, 0.f )
    ) / float( equal_length.size() );
    center /= glm::length( center );
    std::array< float, 8u > dots;
    std::transform(
      projection_box.begin(),
      projection_box.end(),
      dots.begin(),
      [&center]( const auto &v ) {
        return glm::dot( center, v );
      }
    );
    const auto back = std::find_if(
      dots.begin(),
      dots.end(),
      []( float v ) {
        return v <= 0.7071067811865476f; // cos( pi/4 )
      }
    );
    if( back != dots.end() ) {
      for( const auto &d: dots ) {
        std::cout << d << " ";
      }
      std::cout << std::endl;
      std::cout << __FILE__ << " " << __LINE__ <<  std::endl;
      return std::nullopt;
    }
    const auto max_angle = std::accumulate(
      dots.begin(),
      dots.end(),
      0.f,
      [&center]( float max, const float dot ) {
        const auto angle = std::acos( dot );
        return std::max( max, angle );
      }
    );
    const auto projection = glm::perspective(
      max_angle,
      1.f,
      glm::dot( projection_box[nearest], center ),
      distance[farest]
    );
    const auto looking_y = std::abs( glm::dot( center, glm::vec3( 0.f, 1.f, 0.f ) ) ) > 0.7071067811865476f;
    if( looking_y ) {
      const auto lookat = glm::lookAt(
        light_pos,
        light_pos + center,
        glm::vec3{ 0.f, 0.f, light_pos[ 1 ] + 100.f }
      );
      return std::make_tuple(
        std::move( projection ),
        std::move( lookat ),
        glm::dot( projection_box[nearest], center ),
        distance[farest]
      );
    }
    else {
      const auto lookat = glm::lookAt(
        light_pos,
        light_pos + center,
        glm::vec3{ 0.f, light_pos[ 1 ] + 100.f, 0.f }
      );
      return std::make_tuple(
        std::move( projection ),
        std::move( lookat ),
        glm::dot( projection_box[nearest], center ),
        distance[farest]
      );
    }
  }
  void test_light_matrix(
    const glm::mat4 &camera_projection_matrix,
    const glm::mat4 &camera_view_matrix,
    const glm::mat4 &light_projection_matrix,
    const glm::mat4 &light_view_matrix
  ) {
    auto projection_box = get_projection_box( camera_projection_matrix, camera_view_matrix, 1.0f );
    for( const auto &v3: projection_box ) {
      const glm::vec4 v(  v3[ 0 ], v3[ 1 ], v3[ 2 ], 1.0f );
      const auto p = scale_vec( light_projection_matrix * light_view_matrix * v );
      if( std::abs( p[ 0 ] ) > 1.0f ) {
        std::cout << "out of range " << p[ 0 ] << ", " << p[ 1 ] << ", " << p[ 2 ] << " " << v[ 0 ] << ", " << v[ 1 ] << ", " << v[ 2 ] << std::endl;
      }
      else if( std::abs( p[ 1 ] ) > 1.0f ) {
        std::cout << "out of range " << p[ 0 ] << ", " << p[ 1 ] << ", " << p[ 2 ] << " " << v[ 0 ] << ", " << v[ 1 ] << ", " << v[ 2 ] << std::endl;
      }
      else if( std::abs( p[ 2 ] ) > 1.0f ) {
        std::cout << "out of range " << p[ 0 ] << ", " << p[ 1 ] << ", " << p[ 2 ] << " " << v[ 0 ] << ", " << v[ 1 ] << ", " << v[ 2 ] << std::endl;
      }
    }
  }
}

