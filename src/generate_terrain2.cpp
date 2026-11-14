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
    ( "output,o", po::value< std::vector< std::string > >()->multitoken(), "output filename" )
    ( "cycle,c", po::value< std::uint32_t >()->default_value( 10u ), "cycle" )
    ( "optimize,t", po::value< std::uint32_t >()->default_value( 10u ), "optimize" );
  po::variables_map vm;
  po::store( po::parse_command_line( argc, argv, desc ), vm );
  po::notify( vm );
  if( vm.count( "help" ) || !vm.count( "input" ) || !vm.count( "output" ) ) {
    std::cout << desc << std::endl;
    exit( 0 );
  }
  {
    auto prev = gct::create_host_image_from_file( vm[ "input" ].as< std::string >(), true, false, true );
    const std::uint32_t cycle = vm[ "cycle" ].as< std::uint32_t >();
    for( auto c = 0u; c != cycle; ++c ) {
      std::cout << "cycle : " << c << std::endl;
      gct::terrain_mesh m(
        gct::terrain_mesh_create_info()
          //.set_point_count( std::max( 640000 >> ( ( cycle - 1  - c ) * 2u ), 64 ) )
          .set_point_count( c < cycle - 1 ? 2500 : 40000 )
          .set_uniform_grid( true )
          .set_stochastic( true )
          .set_previous_height( prev )
          .set_connect_lake( false )
          .set_max_uplift( 5e-3f )
          .set_erosion_rate( 2.61e-10f )
          .set_uplift( gct::create_host_image_from_file( vm[ "input" ].as< std::string >(), true, false, true ) )
      );
      {
        m.generate_river_tree();
        m.erosion( 1000 * 2.5e+2f );
        if( cycle - 1 == c ) m.optimize( vm[ "optimize" ].as< std::uint32_t >() );
        //m.dump_river( "t2.txt" );
      }
      //m.dump_triangle( "r2.txt" );
      //m.dump_vertex( "p2.txt" );
      //m.dump_lake( "l2.txt" );
      //m.dump_edge( "q2.txt" );
      auto dest_image = gct::create_host_image( 4096, 4096, 1u );
      m.rasterize( dest_image );
      prev = std::move( dest_image );
    }
    float min = std::numeric_limits< float >::max();
    float max = std::numeric_limits< float >::min();
    for( auto &v: prev.image ) {
      if( min > v ) min = v;
      if( max < v ) max = v;
    }
    auto image = prev.image;
    for( const auto &output_filename: vm[ "output" ].as< std::vector< std::string > >() ) {
      const bool is_exr = output_filename.substr( output_filename.size() - 4 ) == ".exr";
      if( is_exr ) std::cout << "exr" << std::endl;
      for( auto &v: prev.image ) {
        v -= min;
        if( is_exr ) {
          v /= 1000.f;
        }
        else {
          v /= ( max - min );
        }
      }
      gct::save_host_image( prev, output_filename );
      prev.image = image;
    }
  }
}

