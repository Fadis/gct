#include <iostream>
#include <boost/program_options.hpp>
#include <OpenImageIO/imageio.h>
#include <OpenImageIO/version.h>
#include <gct/shader/glsl/gct/simplex_noise.h>
#include <gct/shader/glsl/gct/perlin_noise.h>
#include <gct/shader/glsl/gct/cellular_noise.h>

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

#if OIIO_VERSION_MAJOR >= 4 || ( OIIO_VERSION_MAJOR == 3 && OIIO_VERSION_MINOR >= 1 )
    using namespace OIIO_CURRENT_NAMESPACE;
#else
    using namespace OIIO_NAMESPACE;
#endif
  ImageSpec spec( 1024, 1024, 3, TypeDesc::UINT8 );
  std::vector<std::uint8_t> output_pixels( spec.width * spec.height * spec.nchannels, 255u );

  for( unsigned int y = 0u; y != spec.height; ++y ) {
    for( unsigned int x = 0u; x != spec.width; ++x ) {
      const glm::vec2 a =  noise_cellular( glm::vec2( 4.0f * float( x )/spec.width - 1.0f, 4.0f * float( y )/spec.height - 1.0f ) );
      const float c =  noise_perlin2d_fbm5( glm::vec2( 4.0f * float( x )/spec.width - 1.0f, 4.0f * float( y )/spec.height - 1.0f ) );
      const float b = ( ( -a.x + a.y )*(1.0f/3.0f) + c*(2.0f/3.0f) ) * 127.f + 127.f;
      output_pixels[ 0 + x * spec.nchannels + y * spec.width * spec.nchannels ] = b;
      output_pixels[ 1 + x * spec.nchannels + y * spec.width * spec.nchannels ] = b;
      output_pixels[ 2 + x * spec.nchannels + y * spec.width * spec.nchannels ] = b;
    }
  }


#if OIIO_VERSION_MAJOR >= 2 
  auto output_file = ImageOutput::create( vm[ "output" ].as< std::string >() );
#else
  std::shared_ptr< ImageOutput > output_file(
    ImageOutput::open( vm[ "output" ].as< std::string >() ),
    []( auto p ) { if( p ) ImageOutput::destroy( p ); }
  );
#endif
  output_file->open( vm[ "output" ].as< std::string >(), spec );
  output_file->write_image( make_cspan( output_pixels ) );
  output_file->close();
}

