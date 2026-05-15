#include <gct/shader/glsl/gct/dgf.h>
#include <DGF.h>
#include <cstdint>
#include <iostream>
#include <filesystem>
#include <cstdlib>
#include <fx/gltf.h>
#include <boost/program_options.hpp>
#include <nlohmann/json.hpp>
#include <gct/exception.hpp>
#include <gct/gltf.hpp>
#include <gct/compress_mesh.hpp>

void inspect(
  const std::filesystem::path &base_dir,
  fx::gltf::Document &doc,
  const std::vector< std::string > &convert
) {
  auto mesh = gct::gltf::load_mesh_on_cpu( base_dir, doc );

  for( auto &[key,vb] : mesh ) {
    gct::get_vertex_buffer()[ 0u ] = vb.attribute[ gct::vertex_attribute_id::position ].data;
    for( std::uint32_t m = 0u; m != vb.meshlet_count; ++m ) {
      std::vector< glm::vec3 > host;
      gct::gltf::meshlet_reader reader( vb, m );
      DGF::MetaData meta;
      DGF::DecodeMetaData( &meta, vb.attribute[ gct::vertex_attribute_id::position ].data.data() + m * 128u );
      for( std::uint32_t i = 0u; i != reader.get_face_count(); ++i ) {
        const auto f = reader( i );
        host.push_back( f.vertex[ 0 ].position );
        host.push_back( f.vertex[ 1 ].position );
        host.push_back( f.vertex[ 2 ].position );
      }
      std::vector< glm::vec3 > device;
      std::vector< std::uint32_t > di;
      DGFBlockInfo block_info = DGFInit( 0u, m );
      for( std::uint32_t i = 0u; i != block_info.header.numTriangles; ++i ) {
        const glm::uvec3 f = DGFGetTriangle_BitScan_Lane( block_info, i );
        device.push_back( DGFGetVertex( block_info, f[ 0 ] ) );
        device.push_back( DGFGetVertex( block_info, f[ 1 ] ) );
        device.push_back( DGFGetVertex( block_info, f[ 2 ] ) );
        di.push_back( f[ 0 ] );
        di.push_back( f[ 1 ] );
        di.push_back( f[ 2 ] );
      }
      for( std::uint32_t i = 0u; i != host.size(); ++i ) {
        if( host[ i ] != device[ i ] ) {
          std::cout << "diff at " << i <<
                       " (" << host[i][0] << "," << host[i][1] << "," << host[i][2] << ") (" <<
                       " (" << device[i][0] << "," << device[i][1] << "," << device[i][2] << ") " <<
                       di[ i ] <<
                       std::endl;
        }
      }
    }
  }
}

int main( int argc, const char *argv[] ) {
  namespace po = boost::program_options;
  po::options_description desc( "Options" );
  desc.add_options()
    ( "help,h", "show this message" )
    ( "model,m", po::value< std::string >(), "glTF filename" )
    ( "convert,c", po::value< std::vector< std::string > >()->multitoken(), "convert" );
  po::variables_map vm;
  po::store( po::parse_command_line( argc, argv, desc ), vm );
  po::notify( vm );
  if( vm.count( "help" ) || !vm.count( "model" ) ) {
    std::cout << desc << std::endl;
    std::exit( 0 );
  }
  fx::gltf::ReadQuotas quota;
  quota.MaxBufferByteLength = 1024 * 1024 * 1024;
  fx::gltf::Document doc = fx::gltf::LoadFromText( vm[ "model" ].as< std::string >(), quota );
  const auto base_dir = std::filesystem::path( vm[ "model" ].as< std::string >() ).parent_path();
  inspect( base_dir, doc, vm.count( "convert" ) ? vm[ "convert" ].as< std::vector< std::string > >() : std::vector< std::string >{} );

}

