#include <fstream>
#include <iostream>
#include <glm/vec2.hpp>
#include <boost/program_options.hpp>
#include <nlohmann/json.hpp>
#include <gct/terrain_mesh.hpp>
#include <gct/host_image.hpp>

int main(
  int argc,
  const char *argv[]
) {
  namespace po = boost::program_options;
  po::options_description desc( "Options" );
  desc.add_options()
    ( "help,h", "show this message" )
    ( "input,i", po::value< std::string >(), "input filename" )
    ( "output,o", po::value< std::vector< std::string > >()->multitoken(), "output filename" );
  po::variables_map vm;
  po::store( po::parse_command_line( argc, argv, desc ), vm );
  po::notify( vm );
  if( vm.count( "help" ) || !vm.count( "input" ) || !vm.count( "output" ) ) {
    std::cout << desc << std::endl;
    exit( 0 );
  }
  {
    gct::terrain_mesh m(
      gct::terrain_mesh_create_info()
        .set_point_count( /*160000*/40000 )
        .set_erosion_rate( 5.61e-10f )
        .set_delta_t( 2.5e+2 )
        //.set_uniform_grid( true )
        //.set_stochastic( true )
        //.set_connect_lake( false )
        .set_uplift( gct::create_host_image_from_file( vm[ "input" ].as< std::string >(), true, false, true ) )
    );
    for( unsigned int i = 0u; i != 1001u; ++i ) {
      std::cout << i << std::endl;
      m.generate_river_tree();
      m.erosion();
      if( i % 100 == 0 ) {
        m.dump_river( "t_" + std::to_string( i ) + ".txt" );
      }
    }
    m.dump_triangle( "r.txt" );
    m.dump_vertex( "p.txt" );
    m.dump_edge( "q.txt" );
    auto dest_image = gct::create_host_image( 4096, 4096, 1u );
    m.rasterize( dest_image );
    float min = std::numeric_limits< float >::max();
    float max = std::numeric_limits< float >::min();
    for( auto &v: dest_image.image ) {
      if( min > v ) min = v;
      if( max < v ) max = v;
    }
    auto image = dest_image.image;
    for( const auto &output_filename: vm[ "output" ].as< std::vector< std::string > >() ) {
      const bool is_exr = output_filename.substr( output_filename.size() - 4 ) == ".exr";
      if( is_exr ) std::cout << "exr" << std::endl;
      for( auto &v: dest_image.image ) {
        v -= min;
        if( is_exr ) {
          v /= 1000.f;
        }
        else {
          v /= ( max - min );
        }
      }
      gct::save_host_image( dest_image, output_filename );
      dest_image.image = image;
    }
  }
}

