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
  {
    auto prev = gct::create_host_image_from_file( "daichi.png", true, false, true );
    for( auto &v: prev.image ) {
      v *= 3000.f;
    }
    gct::terrain_mesh m(
      gct::terrain_mesh_create_info()
        .set_point_count( 40000 )
        .set_uniform_grid( true )
        .set_stochastic( true )
        .set_previous_height( prev )
        .set_connect_lake( false )
        .set_erosion_rate( 2.0e-12f )
        //.set_max_uplift( 5.0e-3f )
        .set_uplift( gct::create_host_image_from_file( "daichi.png", true, false, true ) )
    );
    /*for( unsigned int i = 0u; i != 301u; ++i ) */{
      m.generate_river_tree();
      m.erosion( 300 * 5.0e+5f );
      m.dump_river( "t2.txt" );
    }
    m.dump_triangle( "r2.txt" );
    m.dump_vertex( "p2.txt" );
    m.dump_lake( "l2.txt" );
    m.dump_edge( "q2.txt" );
    auto dest_image = gct::create_host_image( 1024, 1024, 1u );
    m.rasterize( dest_image );
    for( auto &v: dest_image.image ) {
      v /= 1000.f;
      //v = std::min( std::max( v, 0.0f ), 1.0f );
    }
    gct::save_host_image( dest_image, "output2.exr" );
  }
  exit(0);
}

