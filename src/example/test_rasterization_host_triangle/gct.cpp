#include <gct/shader/glsl/gct/rasterize.h>
#include <iostream>
#include <filesystem>
#include <cstdlib>
#include <cstdint>
#include <random>
#include <unordered_set>
#include <boost/program_options.hpp>
#include <boost/fusion/container/vector.hpp>
#include <boost/spirit/include/qi.hpp>
#include <nlohmann/json.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtx/transform.hpp>
#include <OpenImageIO/imageio.h>
#include <OpenImageIO/version.h>
#include <gct/exception.hpp>
#include <gct/gamma.hpp>

int main( int argc, const char *argv[] ) {
  namespace po = boost::program_options;
  po::options_description desc( "Options" );
  desc.add_options()
    ( "output,o", po::value< std::string >()->default_value("output.png"), "output filename" )
    ( "size,s", po::value< std::string >()->default_value("320x240"), "image size" )
    ( "perspective,p", po::bool_switch(), "enable perspective correction" )
    ( "help,h", "show this message" );
  po::variables_map vm;
  po::store( po::parse_command_line( argc, argv, desc ), vm );
  po::notify( vm );
  if( vm.count( "help" ) ) {
    std::cout << desc << std::endl;
    std::exit( 0 );
  }

  const std::string image_size = vm[ "size" ].as< std::string >();
  boost::fusion::vector< unsigned int, unsigned int > parsed_image_size;
  {
    auto iter = image_size.begin();
    const auto end = image_size.end();
    namespace qi = boost::spirit::qi;
    if( !qi::parse( iter, end, qi::uint_ >> 'x' >> qi::uint_, parsed_image_size ) ) {
      std::cerr << "Invalid window size: " << image_size << std::endl;
      exit( 1 );
    }
  }
  const std::uint32_t width = boost::fusion::at_c< 0 >( parsed_image_size );
  const std::uint32_t height = boost::fusion::at_c< 1 >( parsed_image_size );
  std::vector< std::uint8_t > color_buffer( width * height * 4u, 0u );
  for( unsigned int p = 0u; p != color_buffer.size(); p += 4u ) {
    color_buffer[ p + 0u ] = 128u;
    color_buffer[ p + 1u ] = 128u;
    color_buffer[ p + 2u ] = 128u;
    color_buffer[ p + 3u ] = 255u;
  }

  std::mt19937 gen( std::random_device{}() );
  std::uniform_real_distribution<> dist( -1.2, 1.2 );


  std::vector< glm::vec4 > vertex{
    glm::vec4{ -0.08020833333333333f,  0.6351851851851851f,  0.0f, 1.0f },
    glm::vec4{  0.4593750000000001f,  -0.09259259259259256f, 0.0f, 1.0f },
    glm::vec4{ -0.44166666666666665f, -0.6851851851851851f,  0.0f, 1.0f }
  };
  
  std::vector< glm::vec3 > color{
    glm::vec3{ 1.0f, 0.0f, 0.0f },
    glm::vec3{ 0.0f, 1.0f, 0.0f },
    glm::vec3{ 0.0f, 0.0f, 1.0f }
  };

  auto camera_pos = glm::vec3{ 0.f, -3.0f, 6.0f };

  auto rotate = glm::rotate( float( M_PI * 9.0f/16.0f ), glm::vec3( 1.0f, 0.0f, 0.0f ) );

  auto camera = glm::lookAt(
    camera_pos,
    glm::vec3( 0.f, 0.f, 0.f ),
    glm::vec3{ 0.f, camera_pos[ 1 ] + 100.f, 0.f }
  );

  auto projection =
    glm::perspective( 0.39959648408210363f, (float(width)/float(height)), 0.1f, 10.f );

  for( unsigned int t = 0u; t != vertex.size(); t += 3u ) {

    auto v0 = vertex[ t + 0 ];
    auto v1 = vertex[ t + 1 ];
    auto v2 = vertex[ t + 2 ];
    
    auto c0 = color[ t + 0 ];
    auto c1 = color[ t + 1 ];
    auto c2 = color[ t + 2 ];
 
    rasterization_state rast = rasterization_init(
      v0,
      v1,
      v2,
      width,
      height
    );
    
    std::uint32_t cand_count = rasterization_get_candidate_count( rast );
 
    std::cout << "cand count : " << cand_count << std::endl;
    
 
    const bool pci = vm[ "perspective" ].as< bool >();
 
    for( std::uint32_t i = 0u; i != cand_count; ++i ) {
      rasterization_result p = rasterization_rasterize( rast, i );
      if( p.valid ) {
        glm::vec3 c =
          pci ?
          rasterization_perspective_correct_interpolate( rast, p, c0, c1, c2 ) :
          rasterization_interpolate( rast, p, c0, c1, c2 );
        glm::vec3 c_srgb = glm::vec3( c.x, c.y, c.z );
        const std::uint32_t pixel_offset = ( p.pixel.x + p.pixel.y * width ) * 4u;
        color_buffer[ pixel_offset + 0u ] = c_srgb.x * 255;
        color_buffer[ pixel_offset + 1u ] = c_srgb.y * 255;
        color_buffer[ pixel_offset + 2u ] = c_srgb.z * 255;
        color_buffer[ pixel_offset + 3u ] = 255u;
      }
    }
  }

#if OIIO_VERSION_MAJOR >= 4 || ( OIIO_VERSION_MAJOR == 3 && OIIO_VERSION_MINOR >= 1 )
    using namespace OIIO_CURRENT_NAMESPACE;
#else
    using namespace OIIO_NAMESPACE;
#endif
  auto oiio_type = TypeDesc::UINT8;
  auto out = ImageOutput::create( vm[ "output" ].as< std::string >() );
  if( !out ) throw -1;
  ImageSpec spec( width, height, 4u, oiio_type );
  out->open( vm[ "output" ].as< std::string >(), spec );
  out->write_image( oiio_type, color_buffer.data() );
  out->close();
}

