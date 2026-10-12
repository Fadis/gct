#include <iostream>
#include <boost/program_options.hpp>
#include <OpenImageIO/imageio.h>
#include <OpenImageIO/version.h>
#include <gct/shader/glsl/gct/simplex_noise.h>
#include <gct/shader/glsl/gct/perlin_noise.h>
#include <gct/shader/glsl/gct/cellular_noise.h>
#include <gct/shader/glsl/gct/terrain_noise.h>
#include <gct/shader/glsl/gct/phasor_noise.h>
#include <gct/shader/glsl/gct/terrain_erosion.h>

struct image_statistics {
  glm::vec4 max = glm::vec4( 0, 0, 0, 0 );
  glm::vec4 min = glm::vec4( 0, 0, 0, 0 );
  glm::vec4 average = glm::vec4( 0, 0, 0, 0 );
  std::uint32_t sample_count = 0u;
  void add_sample( const glm::vec4 &s ) {
    max = glm::max( max, s );
    min = glm::min( min, s );
    average = ( average * float( sample_count ) + s )/float( sample_count + 1 );
    ++sample_count;
  }
};

void dump_image( std::uint32_t width, std::uint32_t height, std::uint32_t desc, const std::string &name ) {
#if OIIO_VERSION_MAJOR >= 4 || ( OIIO_VERSION_MAJOR == 3 && OIIO_VERSION_MINOR >= 1 )
    using namespace OIIO_CURRENT_NAMESPACE;
#else
    using namespace OIIO_NAMESPACE;
#endif
  ImageSpec spec( width, height, 3, TypeDesc::UINT8 );
  std::vector<std::uint8_t> output_pixels( spec.width * spec.height * spec.nchannels, 255u );

  for( unsigned int y = 0u; y != spec.height; ++y ) {
    for( unsigned int x = 0u; x != spec.width; ++x ) {
      const float h = imageLoad( image_pool_2d[ desc ], ivec2( x, y ) ).x;// + imageLoad( image_pool_2d[ terrain_desc ], ivec2( x, y ) ).z;// - initial[ y * width + x ].x;
      const float b = ( std::max( std::min( h, 1.0f ), -1.0f ) + 1.0f ) * 127.0f;
      output_pixels[ 0 + x * spec.nchannels + y * spec.width * spec.nchannels ] = b;
      output_pixels[ 1 + x * spec.nchannels + y * spec.width * spec.nchannels ] = b;
      output_pixels[ 2 + x * spec.nchannels + y * spec.width * spec.nchannels ] = b;
    }
  }


#if OIIO_VERSION_MAJOR >= 2 
  auto output_file = ImageOutput::create( name );
#else
  std::shared_ptr< ImageOutput > output_file(
    ImageOutput::open( name ),
    []( auto p ) { if( p ) ImageOutput::destroy( p ); }
  );
#endif
  output_file->open( name, spec );
  output_file->write_image( make_cspan( output_pixels ) );
  output_file->close();
}



int main(
  int argc,
  const char *argv[]
) {
  namespace po = boost::program_options;
  po::options_description desc( "Options" );
  desc.add_options()
    ( "help,h", "show this message" )
    ( "output,o", po::value< std::string >(), "output filename" );
  po::variables_map vm;
  po::store( po::parse_command_line( argc, argv, desc ), vm );
  po::notify( vm );
  if( vm.count( "help" ) || !vm.count( "output" ) ) {
    std::cout << desc << std::endl;
    exit( 0 );
  }

  const std::uint32_t width = 1024;
  const std::uint32_t height = 1024;
  const std::uint32_t N = 1024;

  const std::uint32_t terrain_desc = 1u;
  auto &terrain = gct::create_dummy_image_2d( terrain_desc, width, height );
  const std::uint32_t temp_desc = 2u;
  auto &temp = gct::create_dummy_image_2d( temp_desc, width, height );
  const std::uint32_t dwdi_desc = 3u;
  auto &dwdi = gct::create_dummy_image_2d( dwdi_desc, width, height );


  for( unsigned int y = 0u; y != height; ++y ) {
    for( unsigned int x = 0u; x != width; ++x ) {
//      std::cout << "debug 1 " << x << " " << y << std::endl;
      const float h = noise_terrain2d( glm::vec2( 8.0f * float( x )/width - 0.0f, 8.0f * float( y )/height - 0.0f ) );
      imageStore( image_pool_2d[ terrain_desc ], glm::ivec2( x, y ), glm::vec4( h, 0.0f, 0.0f, 0.0f ) );
    }
  }

/*  for( unsigned int y = 0u; y != height; ++y ) {
    for( unsigned int x = 0u; x != width; ++x ) {
      if( x < 32 && y < 32 ) 
        imageStore( image_pool_2d[ terrain_desc ], glm::ivec2( x, y ), glm::vec4( 1.0f, 0.0f, 0.0f, 0.0f ) );
      else if( x >= 32 && y >= 32 ) 
        imageStore( image_pool_2d[ terrain_desc ], glm::ivec2( x, y ), glm::vec4( 1.0f, 0.0f, 0.0f, 0.0f ) );
      else
        imageStore( image_pool_2d[ terrain_desc ], glm::ivec2( x, y ), glm::vec4( 0.0f, 0.0f, 0.0f, 0.0f ) );
    }
  }
*/

  //const float T = 4.0f / N;
  const float T = 16.0f / N;
  const float c = 0.5f;


  const float Kr = 0.01f;
  const float Ks = 0.01f;
  const float Ke = 0.5f;
  const float Kc = 0.01f;

  auto initial = image_pool_2d[ terrain_desc ].pixel;

  for( unsigned int iter = 0u; iter != 50000u; ++iter ) {
    std::cout << "cycle : " << iter << std::endl;
    //std::cout << image_pool_2d[ terrain_desc ].pixel[ 256 * width + 256 ].x << std::endl;
    //std::cout << initial - imageLoad( image_pool_2d[ terrain_desc ], glm::ivec2( 256, 256 ) ).x << std::endl;
    /*for( unsigned int y = 0u; y != height; ++y ) {
      for( unsigned int x = 0u; x != width; ++x ) {
//        std::cout << "debug 2 " << x << " " << y << std::endl;
        thermal_erosion_calc_dmax( terrain_desc, temp_desc, ivec2( x, y ), T, c );
      }
    }
    for( unsigned int y = 0u; y != height; ++y ) {
      for( unsigned int x = 0u; x != width; ++x ) {
//        std::cout << "debug 3 " << x << " " << y << std::endl;
        thermal_erosion_update_height( temp_desc, terrain_desc, ivec2( x, y ), T );
      }
    }*/
    for( unsigned int y = 0u; y != height; ++y ) {
      for( unsigned int x = 0u; x != width; ++x ) {
        hydraulic_erosion_rain( terrain_desc, temp_desc, ivec2( x, y ), Kr, Ks );
      }
    }
    for( unsigned int y = 0u; y != height; ++y ) {
      for( unsigned int x = 0u; x != width; ++x ) {
        hydraulic_erosion_flow2( temp_desc, dwdi_desc, ivec2( x, y ) );
      }
    }
    for( unsigned int y = 0u; y != height; ++y ) {
      for( unsigned int x = 0u; x != width; ++x ) {
        hydraulic_erosion_calc_update2( temp_desc, dwdi_desc, terrain_desc, ivec2( x, y ), Ke, Kc );
      }
    }
    /*for( unsigned int y = 0u; y != height; ++y ) {
      for( unsigned int x = 0u; x != width; ++x ) {
        simple_hydraulic_erosion_calc_dmax( terrain_desc, temp_desc, ivec2( x, y ), T );
      }
    }
    for( unsigned int y = 0u; y != height; ++y ) {
      for( unsigned int x = 0u; x != width; ++x ) {
        simple_hydraulic_erosion_update( temp_desc, terrain_desc, ivec2( x, y ), T );
      }
    }*/
    if( ( iter % 1000 ) == 0 ) {
      dump_image( width, height, terrain_desc, std::string( "prog_" ) + std::to_string( iter ) + ".png" );
    }
  }

  dump_image( width, height, terrain_desc, vm[ "output" ].as< std::string >() );
}

