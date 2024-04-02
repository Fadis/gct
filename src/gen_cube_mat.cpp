#include <iostream>
#include <boost/program_options.hpp>
#include <nlohmann/json.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <gct/cubemap.hpp>

int main( int argc, char *argv[] ) {
  namespace po = boost::program_options;
  po::options_description desc( "Options" );
  desc.add_options()
    ( "help,h", "show this message" )
    ( "x", po::value< float >()->default_value( 0.f ), "x" )
    ( "y", po::value< float >()->default_value( 0.f ), "y" )
    ( "z", po::value< float >()->default_value( 0.f ), "z" )
    ( "near,n", po::value< float >()->default_value( 0.01f ), "near" )
    ( "far,f", po::value< float >()->default_value( 1.f ), "far" );
  po::variables_map vm;
  po::store( po::parse_command_line( argc, argv, desc ), vm );
  po::notify( vm );
  if( vm.count( "help" ) ) {
    std::cout << desc << std::endl;
    return 0;
  }
  const float x = vm[ "x" ].as< float >(); 
  const float y = vm[ "y" ].as< float >(); 
  const float z = vm[ "z" ].as< float >(); 
  const float near = vm[ "near" ].as< float >();
  const float far = vm[ "far" ].as< float >();
  const auto mat = gct::cubemap_matrix(
    glm::vec3( x, y, z ),
    near,
    far
  );
  for( unsigned int i = 0u; i != 6u; ++i ) {
    std::cout << glm::to_string( mat.get_projection_matrix() * mat.get_view_matrix( i ) ) << std::endl;
  }
  std::cout << nlohmann::json( mat ).dump( 2 ) << std::endl;
}

