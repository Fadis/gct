#include <cmath>
#include <OpenImageIO/imageio.h>
#include <OpenImageIO/version.h>
#include <iostream>
#include <boost/program_options.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/geometric.hpp>
#include <gct/spectrum.hpp>

int main(
  int argc,
  const char *argv[]
) {
  namespace po = boost::program_options;
  po::options_description desc( "Options" );
  po::positional_options_description p;
  p.add( "first", 1 );
  p.add( "second", 1 );
  p.add( "output", 1 );
  desc.add_options()
    ( "help,h", "show this message" )
    ("first", po::value<std::string>(), "first")
    ("second", po::value<std::string>(), "second")
    ("output", po::value<std::string>(), "output");
  po::variables_map vm;
  po::store( po::command_line_parser( argc, argv ).options( desc ).positional( p ).run(), vm );
  po::notify( vm );
  if( vm.count( "help" ) || !vm.count( "first" ) || !vm.count( "second" ) || !vm.count( "output" ) ) {
    std::cout << desc << std::endl;
    exit( 0 );
  }

#if OIIO_VERSION_MAJOR >= 4 || ( OIIO_VERSION_MAJOR == 3 && OIIO_VERSION_MINOR >= 1 )
    using namespace OIIO_CURRENT_NAMESPACE;
#else
    using namespace OIIO_NAMESPACE;
#endif
#if OIIO_VERSION_MAJOR >= 2 
  auto first_file = ImageInput::open( vm[ "first" ].as< std::string >() );
#else
  std::shared_ptr< ImageInput > first_file(
    ImageInput::open( vm[ "first" ].as< std::string >() ),
    []( auto p ) { if( p ) ImageInput::destroy( p ); }
  );
#endif
  const ImageSpec &first_spec = first_file->spec();
#if OIIO_VERSION_MAJOR >= 2 
  auto second_file = ImageInput::open( vm[ "second" ].as< std::string >() );
#else
  std::shared_ptr< ImageInput > second_file(
    ImageInput::open( vm[ "second" ].as< std::string >() ),
    []( auto p ) { if( p ) ImageInput::destroy( p ); }
  );
#endif
  const ImageSpec &second_spec = second_file->spec();
  if(
    first_spec.width != second_spec.width ||
    first_spec.height != second_spec.height ||
    first_spec.nchannels != second_spec.nchannels
  ) {
    std::cout << "different size" << std::endl;
    exit( 1 );
  }

  std::vector<std::uint8_t> first_pixels(first_spec.width * first_spec.height * first_spec.nchannels);
  first_file->read_image(
#if OIIO_VERSION_MAJOR >= 2 
    0u, 0u,
#endif
    0u, first_spec.nchannels,
    first_spec.format,
    first_pixels.data()
  );
  std::vector<std::uint8_t> second_pixels(second_spec.width * second_spec.height * second_spec.nchannels);
  second_file->read_image(
#if OIIO_VERSION_MAJOR >= 2 
    0u, 0u,
#endif
    0u, second_spec.nchannels,
    second_spec.format,
    second_pixels.data()
  );
  std::vector<std::uint8_t> output_pixels(second_spec.width * second_spec.height * 3u );
  for( std::uint32_t h = 0u; h != first_spec.height; ++h ) {
    for( std::uint32_t w = 0u; w != first_spec.width; ++w ) {
      float d = 0.0f;
      if( first_spec.nchannels == 1 && second_spec.nchannels == 1 ) {
        d = std::abs(
          float(
            first_pixels[ 0 + w * first_spec.nchannels + h * first_spec.width * first_spec.nchannels ]
          ) -
          float(
            second_pixels[ 0 + w * second_spec.nchannels + h * second_spec.width * second_spec.nchannels ]
          )
        );
      }
      else if( first_spec.nchannels == 2 && second_spec.nchannels == 2 ) {
        d = glm::distance(
          glm::vec2(
            first_pixels[ 0 + w * first_spec.nchannels + h * first_spec.width * first_spec.nchannels ],
            first_pixels[ 1 + w * first_spec.nchannels + h * first_spec.width * first_spec.nchannels ]
          ),
          glm::vec2(
            second_pixels[ 0 + w * second_spec.nchannels + h * second_spec.width * second_spec.nchannels ],
            second_pixels[ 1 + w * second_spec.nchannels + h * second_spec.width * second_spec.nchannels ]
          )
        );
      }
      else if( first_spec.nchannels == 3 && second_spec.nchannels == 3 ) {
        d = glm::distance(
          glm::vec3(
            first_pixels[ 0 + w * first_spec.nchannels + h * first_spec.width * first_spec.nchannels ],
            first_pixels[ 1 + w * first_spec.nchannels + h * first_spec.width * first_spec.nchannels ],
            first_pixels[ 2 + w * first_spec.nchannels + h * first_spec.width * first_spec.nchannels ]
          ),
          glm::vec3(
            second_pixels[ 0 + w * second_spec.nchannels + h * second_spec.width * second_spec.nchannels ],
            second_pixels[ 1 + w * second_spec.nchannels + h * second_spec.width * second_spec.nchannels ],
            second_pixels[ 2 + w * second_spec.nchannels + h * second_spec.width * second_spec.nchannels ]
          )
        );
      }
      else if( first_spec.nchannels == 3 && second_spec.nchannels == 4 ) {
        d = glm::distance(
          glm::vec4(
            first_pixels[ 0 + w * first_spec.nchannels + h * first_spec.width * first_spec.nchannels ],
            first_pixels[ 1 + w * first_spec.nchannels + h * first_spec.width * first_spec.nchannels ],
            first_pixels[ 2 + w * first_spec.nchannels + h * first_spec.width * first_spec.nchannels ],
            255u
          ),
          glm::vec4(
            second_pixels[ 0 + w * second_spec.nchannels + h * second_spec.width * second_spec.nchannels ],
            second_pixels[ 1 + w * second_spec.nchannels + h * second_spec.width * second_spec.nchannels ],
            second_pixels[ 2 + w * second_spec.nchannels + h * second_spec.width * second_spec.nchannels ],
            second_pixels[ 3 + w * second_spec.nchannels + h * second_spec.width * second_spec.nchannels ]
          )
        );
      }
      else if( first_spec.nchannels == 4 && second_spec.nchannels == 3 ) {
        d = glm::distance(
          glm::vec4(
            first_pixels[ 0 + w * first_spec.nchannels + h * first_spec.width * first_spec.nchannels ],
            first_pixels[ 1 + w * first_spec.nchannels + h * first_spec.width * first_spec.nchannels ],
            first_pixels[ 2 + w * first_spec.nchannels + h * first_spec.width * first_spec.nchannels ],
            first_pixels[ 3 + w * first_spec.nchannels + h * first_spec.width * first_spec.nchannels ]
          ),
          glm::vec4(
            second_pixels[ 0 + w * second_spec.nchannels + h * second_spec.width * second_spec.nchannels ],
            second_pixels[ 1 + w * second_spec.nchannels + h * second_spec.width * second_spec.nchannels ],
            second_pixels[ 2 + w * second_spec.nchannels + h * second_spec.width * second_spec.nchannels ],
            255u
          )
        );
      }
      else if( first_spec.nchannels == 4 && second_spec.nchannels == 4 ) {
        d = glm::distance(
          glm::vec4(
            first_pixels[ 0 + w * first_spec.nchannels + h * first_spec.width * first_spec.nchannels ],
            first_pixels[ 1 + w * first_spec.nchannels + h * first_spec.width * first_spec.nchannels ],
            first_pixels[ 2 + w * first_spec.nchannels + h * first_spec.width * first_spec.nchannels ],
            first_pixels[ 3 + w * first_spec.nchannels + h * first_spec.width * first_spec.nchannels ]
          ),
          glm::vec4(
            second_pixels[ 0 + w * second_spec.nchannels + h * second_spec.width * second_spec.nchannels ],
            second_pixels[ 1 + w * second_spec.nchannels + h * second_spec.width * second_spec.nchannels ],
            second_pixels[ 2 + w * second_spec.nchannels + h * second_spec.width * second_spec.nchannels ],
            second_pixels[ 3 + w * second_spec.nchannels + h * second_spec.width * second_spec.nchannels ]
          )
        );
      }
      const auto c = gct::level_to_color( d, -32.f, 256.f-32.f );
      output_pixels[ 0 + w * 3 + h * second_spec.width * 3 ] = c[ 0 ] * 255.f;
      output_pixels[ 1 + w * 3 + h * second_spec.width * 3 ] = c[ 1 ] * 255.f;
      output_pixels[ 2 + w * 3 + h * second_spec.width * 3 ] = c[ 2 ] * 255.f;
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
  ImageSpec output_spec( second_spec.width, second_spec.height, 3u, TypeDesc::UINT8 );
  output_file->open( vm[ "output" ].as< std::string >(), output_spec );
  output_file->write_image( make_cspan( output_pixels ) );
  output_file->close();
}

