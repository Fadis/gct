#include <gct/shader/glsl/gct/dgf.h>
#include <gct/shader/glsl/gct/scene_graph/accessor.h>
#include <DGF.h>
#include <iostream>
#include <filesystem>
#include <cstdlib>
#include <cstdint>
#include <unordered_set>
#include <boost/program_options.hpp>
#include <nlohmann/json.hpp>
#include <gct/fx/gltf.h>
#include <gct/exception.hpp>
#include <gct/gltf.hpp>
#include <gct/compress_mesh.hpp>

void inspect(
  const std::filesystem::path &base_dir,
  fx::gltf::Document &doc,
  const std::vector< std::string > &convert,
  const std::vector< std::uint32_t > &pf
) {
  std::unordered_set< std::uint32_t > primitive_filter;
  std::copy( pf.begin(), pf.end(), std::inserter( primitive_filter, primitive_filter.begin() ) );
  auto mesh = gct::gltf::load_mesh_on_cpu( base_dir, doc, primitive_filter );


  for( auto &[key,vb] : mesh ) {
    gct::get_vertex_buffer()[ 0u ] = vb.attribute[ gct::vertex_attribute_id::position ].data;
    gct::get_vertex_buffer()[ 1u ] = vb.attribute[ gct::vertex_attribute_id::index ].data;
    for( std::uint32_t m = 0u; m != vb.meshlet_count; ++m ) {
      std::vector< glm::vec3 > host;
      std::vector< std::uint32_t > host_primitive_id;
      std::vector< std::uint32_t > host_index;
      gct::gltf::meshlet_reader reader( vb, m );
      DGF::MetaData meta;
      DGF::DecodeMetaData( &meta, vb.attribute[ gct::vertex_attribute_id::position ].data.data() + m * 128u );
      for( std::uint32_t i = 0u; i != reader.get_face_count(); ++i ) {
        const auto f = reader( i );
        host.push_back( f.vertex[ 0 ].position );
        host.push_back( f.vertex[ 1 ].position );
        host.push_back( f.vertex[ 2 ].position );
        host_primitive_id.push_back( f.primitive_id );
        host_index.push_back( f.vertex[ 0 ].index );
        host_index.push_back( f.vertex[ 1 ].index );
        host_index.push_back( f.vertex[ 2 ].index );
      }
      std::vector< glm::vec3 > device;
      std::vector< std::uint32_t > device_primitive_id;
      std::vector< std::uint32_t > device_index;
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
        device_primitive_id.push_back( block_info.header.userData + i );
        device_index.push_back( reinterpret_cast< std::uint16_t* >( vb.attribute[ gct::vertex_attribute_id::index ].data.data() )[  device_primitive_id.back() * 3u + 0u ] );
        device_index.push_back( reinterpret_cast< std::uint16_t* >( vb.attribute[ gct::vertex_attribute_id::index ].data.data() )[  device_primitive_id.back() * 3u + 1u ] );
        device_index.push_back( reinterpret_cast< std::uint16_t* >( vb.attribute[ gct::vertex_attribute_id::index ].data.data() )[  device_primitive_id.back() * 3u + 2u ] );
      }
      for( std::uint32_t i = 0u; i != host.size(); ++i ) {
        if( host[ i ] != device[ i ] || host_index[ i ] != device_index[ i ] ) {
          std::cout << "diff at " << i <<
                       " (" << host[i][0] << "," << host[i][1] << "," << host[i][2] << ") (" <<
                       " (" << device[i][0] << "," << device[i][1] << "," << device[i][2] << ") " <<
                       host_index[ i ] << " " << device_index[ i ] << " " <<
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
    ( "convert,c", po::value< std::vector< std::string > >()->multitoken(), "convert" )
    ( "primfilter,p", po::value< std::vector< std::uint32_t > >()->multitoken(), "primitive_filter" );
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
  inspect( base_dir, doc, vm.count( "convert" ) ? vm[ "convert" ].as< std::vector< std::string > >() : std::vector< std::string >{}, vm[ "primfilter" ].as< std::vector< std::uint32_t > >() );

}

