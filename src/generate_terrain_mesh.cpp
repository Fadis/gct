#include <iostream>
#include <boost/program_options.hpp>
#include <glm/vec2.hpp>
#include <gct/gaussian_blue_noise.hpp>
#include <gct/area.hpp>
#include <gct/jc_voronoi.h>

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
  auto result = gct::gaussian_blue_noise( 30, 1000 );
  std::vector< jcv_point > points;
  for( auto &v: result ) {
    std::cout << v.x << " " << v.y << std::endl;
    jcv_point p;
    p.x = v.x;
    p.y = v.y;
    points.push_back( p );
  }
  std::cout << points.size() << std::endl;
  jcv_rect bounding_box = { { 0.0f, 0.0f }, { 1.0f, 1.0f } };
  jcv_diagram diagram;
  memset(&diagram, 0, sizeof(jcv_diagram));
  jcv_diagram_generate( points.size(), points.data(), &bounding_box, 0, &diagram );


  const auto sites = jcv_diagram_get_sites( &diagram );
  jcv_edge_iter edge_iter;
  for ( int i=0; i<diagram.numsites; i++) {
    jcv_edge graph_edge;
    jcv_site_get_edges(&diagram, &sites[i], &edge_iter);
    std::cout << "site " << i << std::endl;
    while (jcv_edge_next(&edge_iter, &graph_edge)) {
      std::cout << "  (" << graph_edge.pos[ 0 ].x << "," << graph_edge.pos[ 0 ].y << ")-(" <<
                            graph_edge.pos[ 1 ].x << "," << graph_edge.pos[ 1 ].y << ")" << std::endl;
    }
  }

  jcv_delaunay_iter delaunay;
  jcv_delaunay_edge delaunay_edge;
  jcv_delaunay_begin( &diagram, &delaunay );
  while( jcv_delaunay_next( &delaunay, &delaunay_edge ) ) {
    jcv_edge_iter edge_b_iter;
    jcv_edge edge_b;
    jcv_site_get_edges( &diagram, &sites[ delaunay_edge.sites[ 1 ]->index ], &edge_b_iter );
    while( jcv_edge_next( &edge_b_iter, &edge_b ) ) {
      jcv_edge_iter edge_c_iter;
      jcv_edge edge_c;
      if( edge_b.sites[ 1 ] ) {
        jcv_site_get_edges( &diagram, &sites[ edge_b.sites[ 1 ]->index ], &edge_c_iter );
        while( jcv_edge_next( &edge_c_iter, &edge_c ) ) {
          if( edge_c.sites[ 1 ] && edge_c.sites[ 1 ]->index == delaunay_edge.sites[ 0 ]->index ) {
            std::cout << "t  " << delaunay_edge.sites[ 0 ]->index << " " <<  delaunay_edge.sites[ 1 ]->index << " " << edge_b.sites[ 1 ]->index << std::endl;
          }
        }
      }
    }
  }
 
  float sum = 0.0f;
  for ( int i=0; i<diagram.numsites; i++) {
    jcv_edge graph_edge;
    jcv_site_get_edges(&diagram, &sites[i], &edge_iter);
    std::vector< glm::vec2 > vertex;
    while (jcv_edge_next(&edge_iter, &graph_edge)) {
      vertex.push_back( glm::vec2( graph_edge.pos[ 0 ].x, graph_edge.pos[ 0 ].y ) );
    }
    const auto a = gct::area( vertex );
    sum += a;
    std::cout << sum << "\t" << a << std::endl;
  }
}

