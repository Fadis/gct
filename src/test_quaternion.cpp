#include <glm/detail/qualifier.hpp>
#define BOOST_TEST_MAIN
#include <random>
#include <boost/test/included/unit_test.hpp>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>

using namespace glm;

#include <gct/shader/glsl/gct/quaternion.h>

BOOST_AUTO_TEST_CASE(test_mat3_cast)
{
  std::default_random_engine engine( std::random_device{}() );
  std::uniform_real_distribution< float > dist( -64.0f, 64.0f );
  
  for( unsigned int try_count = 0u; try_count != 100u; ++try_count ) {
    tquat< float > a( dist( engine ), dist( engine ), dist( engine ), dist( engine ) );
 
    vec4 a_vec;
    a_vec.x = a.x;
    a_vec.y = a.y;
    a_vec.z = a.z;
    a_vec.w = a.w;
 
    mat3 m1 = mat3_cast( a );
 
    mat3 m2 = quaternion_to_matrix( a_vec );
 
    for( unsigned int x = 0u; x != 3u; ++x ) {
      for( unsigned int y = 0u; y != 3u; ++y ) {
        BOOST_CHECK_CLOSE( m1[x][y], m2[x][y], 0.0001);
      }
    }
  }
}

BOOST_AUTO_TEST_CASE(test_quat_cast)
{
  std::default_random_engine engine( std::random_device{}() );
  std::uniform_real_distribution< float > dist( -64.0f, 64.0f );
  
  for( unsigned int try_count = 0u; try_count != 100u; ++try_count ) {
    mat3 m;
    for( unsigned int x = 0u; x != 3u; ++x ) {
      for( unsigned int y = 0u; y != 3u; ++y ) {
        m[x][y] = dist( engine );
      }
    }

    tquat< float > a1 = quat_cast< float >( m );
    vec4 a2 = matrix_to_quaternion( m );
 
    for( unsigned int x = 0u; x != 4u; ++x ) {
      BOOST_CHECK_CLOSE( a1[x], a2[x], 0.0001);
    }
  }
}

BOOST_AUTO_TEST_CASE(test_quaternion_quaternion_mult)
{
  std::default_random_engine engine( std::random_device{}() );
  std::uniform_real_distribution< float > dist( -64.0f, 64.0f );
  
  for( unsigned int try_count = 0u; try_count != 100u; ++try_count ) {
    tquat< float > a( dist( engine ), dist( engine ), dist( engine ), dist( engine ) );
    vec4 a_vec;
    a_vec.x = a.x;
    a_vec.y = a.y;
    a_vec.z = a.z;
    a_vec.w = a.w;
    
    tquat< float > b( dist( engine ), dist( engine ), dist( engine ), dist( engine ) );
    vec4 b_vec;
    b_vec.x = b.x;
    b_vec.y = b.y;
    b_vec.z = b.z;
    b_vec.w = b.w;

    tquat< float > c = a * b;
    vec4 c_vec = quaternion_quaternion_mult( a_vec, b_vec );

    for( unsigned int x = 0u; x != 4u; ++x ) {
      BOOST_CHECK_CLOSE( c[x], c_vec[x], 0.1);
    }
  }
}

BOOST_AUTO_TEST_CASE(test_quaternion_vector_mult)
{
  std::default_random_engine engine( std::random_device{}() );
  std::uniform_real_distribution< float > dist( -64.0f, 64.0f );
  
  for( unsigned int try_count = 0u; try_count != 100u; ++try_count ) {
    tquat< float > a( dist( engine ), dist( engine ), dist( engine ), dist( engine ) );
    vec4 a_vec;
    a_vec.x = a.x;
    a_vec.y = a.y;
    a_vec.z = a.z;
    a_vec.w = a.w;
    
    vec3 b( dist( engine ), dist( engine ), dist( engine ) );

    vec3 c = a * b;
    vec3 c_vec = quaternion_vector_mult( a_vec, b );

    for( unsigned int x = 0u; x != 3u; ++x ) {
      BOOST_CHECK_CLOSE( c[x], c_vec[x], 0.1);
    }
  }
}

BOOST_AUTO_TEST_CASE(test_quaternion_conjugate)
{
  std::default_random_engine engine( std::random_device{}() );
  std::uniform_real_distribution< float > dist( -64.0f, 64.0f );
  
  for( unsigned int try_count = 0u; try_count != 100u; ++try_count ) {
    tquat< float > a( dist( engine ), dist( engine ), dist( engine ), dist( engine ) );
    vec4 a_vec;
    a_vec.x = a.x;
    a_vec.y = a.y;
    a_vec.z = a.z;
    a_vec.w = a.w;
    
    tquat< float > b = glm::conjugate( a );
    vec4 b_vec = quaternion_conjugate( a_vec );

    for( unsigned int x = 0u; x != 4u; ++x ) {
      BOOST_CHECK_CLOSE( b[x], b_vec[x], 0.01);
    }
  }
}

BOOST_AUTO_TEST_CASE(test_quaternion_inverse)
{
  std::default_random_engine engine( std::random_device{}() );
  std::uniform_real_distribution< float > dist( -64.0f, 64.0f );
  
  for( unsigned int try_count = 0u; try_count != 100u; ++try_count ) {
    tquat< float > a( dist( engine ), dist( engine ), dist( engine ), dist( engine ) );
    vec4 a_vec;
    a_vec.x = a.x;
    a_vec.y = a.y;
    a_vec.z = a.z;
    a_vec.w = a.w;
    
    tquat< float > b = glm::inverse( a );
    vec4 b_vec = quaternion_inverse( a_vec );

    for( unsigned int x = 0u; x != 4u; ++x ) {
      BOOST_CHECK_CLOSE( b[x], b_vec[x], 0.01);
    }
  }
}

BOOST_AUTO_TEST_CASE(test_quaternion_normalize)
{
  std::default_random_engine engine( std::random_device{}() );
  std::uniform_real_distribution< float > dist( -64.0f, 64.0f );
  
  for( unsigned int try_count = 0u; try_count != 100u; ++try_count ) {
    tquat< float > a( dist( engine ), dist( engine ), dist( engine ), dist( engine ) );
    vec4 a_vec;
    a_vec.x = a.x;
    a_vec.y = a.y;
    a_vec.z = a.z;
    a_vec.w = a.w;
    
    tquat< float > b = glm::normalize( a );
    vec4 b_vec = quaternion_normalize( a_vec );

    for( unsigned int x = 0u; x != 4u; ++x ) {
      BOOST_CHECK_CLOSE( b[x], b_vec[x], 0.01);
    }
  }
}

/*BOOST_AUTO_TEST_CASE(test_quaternion_mix)
{
  std::default_random_engine engine( std::random_device{}() );
  std::uniform_real_distribution< float > dist( -64.0f, 64.0f );
  std::uniform_real_distribution< float > dist01( 0.0f, 1.0f );
  
  for( unsigned int try_count = 0u; try_count != 100u; ++try_count ) {
    tquat< float > a( dist( engine ), dist( engine ), dist( engine ), dist( engine ) );
    a = glm::normalize( a );
    vec4 a_vec;
    a_vec.x = a.x;
    a_vec.y = a.y;
    a_vec.z = a.z;
    a_vec.w = a.w;
    
    tquat< float > b( dist( engine ), dist( engine ), dist( engine ), dist( engine ) );
    b = glm::normalize( b );
    vec4 b_vec;
    b_vec.x = b.x;
    b_vec.y = b.y;
    b_vec.z = b.z;
    b_vec.w = b.w;

    float c = dist01( engine );

    tquat< float > d = glm::mix( a, b, c );
    vec4 d_vec = quaternion_mix( a_vec, b_vec, c );

    for( unsigned int x = 0u; x != 4u; ++x ) {
      BOOST_CHECK_CLOSE( d[x], d_vec[x], 1.0);
    }
  }
}*/

BOOST_AUTO_TEST_CASE(test_quaternion_slerp)
{
  std::default_random_engine engine( std::random_device{}() );
  std::uniform_real_distribution< float > dist( -64.0f, 64.0f );
  std::uniform_real_distribution< float > dist01( 0.0f, 1.0f );
  
  for( unsigned int try_count = 0u; try_count != 100u; ++try_count ) {
    tquat< float > a( dist( engine ), dist( engine ), dist( engine ), dist( engine ) );
    a = glm::normalize( a );
    vec4 a_vec;
    a_vec.x = a.x;
    a_vec.y = a.y;
    a_vec.z = a.z;
    a_vec.w = a.w;
    
    tquat< float > b( dist( engine ), dist( engine ), dist( engine ), dist( engine ) );
    b = glm::normalize( b );
    vec4 b_vec;
    b_vec.x = b.x;
    b_vec.y = b.y;
    b_vec.z = b.z;
    b_vec.w = b.w;

    float c = dist01( engine );

    tquat< float > d = glm::slerp( a, b, c );
    vec4 d_vec = quaternion_slerp( a_vec, b_vec, c );

    for( unsigned int x = 0u; x != 4u; ++x ) {
      BOOST_CHECK_CLOSE( d[x], d_vec[x], 0.1);
    }
  }
}

