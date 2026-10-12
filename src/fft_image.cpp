#include <iostream>
#include <boost/program_options.hpp>
#include <OpenImageIO/imageio.h>
#include <OpenImageIO/version.h>
#include <gct/fft.hpp>

int main(
  int argc,
  const char *argv[]
) {
  namespace po = boost::program_options;
  po::options_description desc( "Options" );
  desc.add_options()
    ( "help,h", "show this message" )
    ( "input,i", po::value< std::string >(), "input filename" )
    ( "output,o", po::value< std::string >(), "output filename" );
  po::variables_map vm;
  po::store( po::parse_command_line( argc, argv, desc ), vm );
  po::notify( vm );
  if( vm.count( "help" ) || !vm.count( "output" ) || !vm.count( "input" ) ) {
    std::cout << desc << std::endl;
    exit( 0 );
  }

#if OIIO_VERSION_MAJOR >= 4 || ( OIIO_VERSION_MAJOR == 3 && OIIO_VERSION_MINOR >= 1 )
    using namespace OIIO_CURRENT_NAMESPACE;
#else
    using namespace OIIO_NAMESPACE;
#endif
#if OIIO_VERSION_MAJOR >= 2 
  auto input_file = ImageInput::open( vm[ "input" ].as< std::string >() );
#else
  std::shared_ptr< ImageInput > input_file(
    ImageInput::open( vm[ "first" ].as< std::string >() ),
    []( auto p ) { if( p ) ImageInput::destroy( p ); }
  );
#endif
  const ImageSpec &spec = input_file->spec();

  std::uint32_t width = std::min( spec.width, spec.height );

  if( width < 2u ) width = 1u;
  else if( width < 4u ) width = 2u;
  else if( width < 8u ) width = 4u;
  else if( width < 16u ) width = 8u;
  else if( width < 32u ) width = 16u;
  else if( width < 64u ) width = 32u;
  else if( width < 128u ) width = 64u;
  else if( width < 256u ) width = 128u;
  else if( width < 512u ) width = 256u;
  else if( width < 1024u ) width = 512u;
  else if( width < 2048u ) width = 1024u;
  else if( width < 4096u ) width = 2048u;
  else if( width < 8192u ) width = 4096u;
  else width = 8192u;

  const std::uint32_t height = width;

  std::cout << spec.width << " " << spec.height << " " << width << " " << height << std::endl;

  std::vector< float > data( width * height );
  {
    if( spec.format == TypeDesc::UINT8 ) {
      std::vector<std::uint8_t> temp( spec.width * spec.height * spec.nchannels );
      input_file->read_image(
#if OIIO_VERSION_MAJOR >= 2 
        0u, 0u,
#endif
        0u, spec.nchannels,
        spec.format,
        temp.data()
      );
      for( unsigned int y = 0u; y < height; ++y ) {
        for( unsigned int x = 0u; x < width; ++x ) {
          data[ x + y * width ] = temp[ x * spec.nchannels + y * spec.width * spec.nchannels ] / float( std::numeric_limits< std::uint8_t >::max() );
        }
      }
    }
    if( spec.format == TypeDesc::UINT16 ) {
      std::vector<std::uint16_t> temp( spec.width * spec.height * spec.nchannels );
      input_file->read_image(
#if OIIO_VERSION_MAJOR >= 2 
        0u, 0u,
#endif
        0u, spec.nchannels,
        spec.format,
        temp.data()
      );
      for( unsigned int y = 0u; y < height; ++y ) {
        for( unsigned int x = 0u; x < width; ++x ) {
          data[ x + y * width ] = temp[ x * spec.nchannels + y * spec.width * spec.nchannels ] / float( std::numeric_limits< std::uint16_t >::max() );
        }
      }
    }
    if( spec.format == TypeDesc::UINT32 ) {
      std::vector<std::uint32_t> temp( spec.width * spec.height * spec.nchannels );
      input_file->read_image(
#if OIIO_VERSION_MAJOR >= 2 
        0u, 0u,
#endif
        0u, spec.nchannels,
        spec.format,
        temp.data()
      );
      for( unsigned int y = 0u; y < height; ++y ) {
        for( unsigned int x = 0u; x < width; ++x ) {
          data[ x + y * width ] = temp[ x * spec.nchannels + y * spec.width * spec.nchannels ] / float( std::numeric_limits< std::uint32_t >::max() );
        }
      }
    }
    if( spec.format == TypeDesc::FLOAT ) {
      std::vector<float> temp( spec.width * spec.height * spec.nchannels );
      input_file->read_image(
#if OIIO_VERSION_MAJOR >= 2 
        0u, 0u,
#endif
        0u, spec.nchannels,
        spec.format,
        temp.data()
      );
      for( unsigned int y = 0u; y < height; ++y ) {
        for( unsigned int x = 0u; x < width; ++x ) {
          data[ x + y * width ] = temp[ x * spec.nchannels + y * spec.width * spec.nchannels ];
        }
      }
    }
  }

  gct::fft2d( data.data(), data.data(), width );

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
  std::vector< std::uint8_t > pixel( width * height * spec.nchannels );
  for( unsigned int y = 0u; y != height; ++y ) {
    for( unsigned int x = 0u; x != width; ++x ) {
      const std::uint8_t a = std::max( std::min( data[ x + y * width ], 1.0f ), 0.0f ) * 255.f;
      pixel[ 0 + x * spec.nchannels + y * width * spec.nchannels ] = a;
      pixel[ 1 + x * spec.nchannels + y * width * spec.nchannels ] = a;
      pixel[ 2 + x * spec.nchannels + y * width * spec.nchannels ] = a;
    }
  }
  output_file->open( output_name, spec );
  output_file->write_image( make_cspan( pixel ) );
  output_file->close();
  }
}

