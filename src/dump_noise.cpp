#include <iostream>
#include <boost/program_options.hpp>
#include <OpenImageIO/imageio.h>
#include <OpenImageIO/version.h>
#include <gct/fft.hpp>
#include <gct/shader/glsl/gct/simplex_noise.h>
#include <gct/shader/glsl/gct/perlin_noise.h>
#include <gct/shader/glsl/gct/cellular_noise.h>
#include <gct/shader/glsl/gct/terrain_noise.h>
#include <gct/shader/glsl/gct/phasor_noise.h>
#include <gct/shader/glsl/gct/terrain_erosion.h>
#include <gct/shader/glsl/gct/random.h>

int main(
  int argc,
  const char *argv[]
) {
  namespace po = boost::program_options;
  po::options_description desc( "Options" );
  desc.add_options()
    ( "help,h", "show this message" )
    ( "output,o", po::value< std::string >(), "output filename" )
    ( "type,t", po::value< std::string >()->default_value( "white" ), "noise type" )
    ( "fft,f", po::bool_switch(), "generate frequency image" )
    ( "scale,s", po::value< float >()->default_value( 1.0f ), "scale" );
  po::variables_map vm;
  po::store( po::parse_command_line( argc, argv, desc ), vm );
  po::notify( vm );
  if( vm.count( "help" ) || !vm.count( "output" ) ) {
    std::cout << desc << std::endl;
    exit( 0 );
  }

  const std::uint32_t width = 1024u;
  const std::uint32_t height = 1024u;

  std::vector< float > data( width * height );
  const float scale = vm[ "scale" ].as< float >();

  const auto type = vm[ "type" ].as< std::string >();
  for( unsigned int y = 0u; y != height; ++y ) {
    for( unsigned int x = 0u; x != width; ++x ) {
      float a = 0.0f;
      glm::vec2 p = glm::vec2( scale * float( x )/width - 0.0f, scale * float( y )/height - 0.0f );
      if( type == "white" ) {
        a = rand1( p );
      }
      else if( type == "perlin" ) {
        a = noise_perlin2d( p );
      }
      else if( type == "perlin_fbm5" ) {
        a = noise_perlin2d_fbm5( p );
      }
      else if( type == "simplex" ) {
        a = noise_simplex2d( p );
      }
      else if( type == "simplex_fbm5" ) {
        a = noise_simplex2d_fbm5( p );
      }
      else if( type == "simplex_fbm10" ) {
        a = noise_simplex2d_fbm10( p );
      }
      else if( type == "cellular0" ) {
        const auto v = noise_cellular2d( p );
        a = v.x;
      }
      else if( type == "cellular1" ) {
        const auto v = noise_cellular2d( p );
        a = v.y;
      }
      else if( type == "cellular2" ) {
        const auto v = noise_cellular2d( p );
        a = -v.x + v.y;
      }
      else if( type == "cellular3" ) {
        const auto v = noise_cellular2d( p );
        a = v.x - v.y;
      }
      else if( type == "terrain" ) {
        a = noise_terrain2d( p );
      }
      else if( type == "phaser" ) {
        a = phasor_noise_pwm( p );
      }
      data[ x + y * width ] = a * 0.5f + 0.5f;
    }
  }

  if( vm[ "fft" ].as< bool >() ) {
    fft2d( data.data(), data.data(), width );
    std::cout << data[ 0 ] << std::endl;
  }

  const std::string output_name = vm[ "output" ].as< std::string >();

  if( output_name.substr( output_name.size() - 4, output_name.size() ) == ".exr" ) {
#if OIIO_VERSION_MAJOR >= 4 || ( OIIO_VERSION_MAJOR == 3 && OIIO_VERSION_MINOR >= 1 )
  using namespace OIIO_CURRENT_NAMESPACE;
#else
  using namespace OIIO_NAMESPACE;
#endif
  ImageSpec spec( width, height, 1, TypeDesc::FLOAT );
#if OIIO_VERSION_MAJOR >= 2 
  auto output_file = ImageOutput::create( output_name );
#else
  std::shared_ptr< ImageOutput > output_file(
    ImageOutput::open( output_name ),
    []( auto p ) { if( p ) ImageOutput::destroy( p ); }
  );
#endif
  output_file->open( output_name, spec );
  output_file->write_image( make_cspan( data ) );
  output_file->close();
  }
  else {
#if OIIO_VERSION_MAJOR >= 4 || ( OIIO_VERSION_MAJOR == 3 && OIIO_VERSION_MINOR >= 1 )
  using namespace OIIO_CURRENT_NAMESPACE;
#else
  using namespace OIIO_NAMESPACE;
#endif
  ImageSpec spec( width, height, 3, TypeDesc::UINT8 );
#if OIIO_VERSION_MAJOR >= 2 
  auto output_file = ImageOutput::create( output_name );
#else
  std::shared_ptr< ImageOutput > output_file(
    ImageOutput::open( output_name ),
    []( auto p ) { if( p ) ImageOutput::destroy( p ); }
  );
#endif
  std::vector< std::uint8_t > pixel( spec.width * spec.height * spec.nchannels );
  for( unsigned int y = 0u; y != height; ++y ) {
    for( unsigned int x = 0u; x != width; ++x ) {
      const std::uint8_t a = std::max( std::min( data[ x + y * width ], 1.0f ), 0.0f ) * 255.f;
      pixel[ 0 + x * spec.nchannels + y * spec.width * spec.nchannels ] = a;
      pixel[ 1 + x * spec.nchannels + y * spec.width * spec.nchannels ] = a;
      pixel[ 2 + x * spec.nchannels + y * spec.width * spec.nchannels ] = a;
    }
  }
  output_file->open( output_name, spec );
  output_file->write_image( make_cspan( pixel ) );
  output_file->close();
  }
}

