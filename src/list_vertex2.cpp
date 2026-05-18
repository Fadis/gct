#include <iostream>
#include <filesystem>
#include <cstdlib>
#include <boost/program_options.hpp>
#include <nlohmann/json.hpp>
#include <gct/fx/gltf.h>
#include <gct/exception.hpp>
#include <gct/gltf.hpp>
#include <gct/compress_mesh.hpp>

void inspect(
  const std::filesystem::path &base_dir,
  fx::gltf::Document &doc,
  const std::vector< std::string > &convert
) {
  auto mesh = gct::gltf::load_mesh_on_cpu( base_dir, doc );
  gct::gltf::convert_mesh( mesh, convert );
  //gct::gltf::store_mesh_on_cpu( base_dir, doc, mesh, "test.bin" );
  //fx::gltf::Save( doc, std::cout, base_dir, false );

  for( auto &[key,vb] : mesh ) {
    std::cout << "primitive " << key.mesh_id << " " << key.primitive_id << std::endl;
    std::cout << "  vertex_count=" << vb.vertex_count <<
                  " unique_vertex_count=" << vb.unique_vertex_count <<
                  " meshlet_count=" << vb.meshlet_count <<
                  std::endl;
    for( const auto &[attr_id,attr]: vb.attribute ) {
      std::cout << "  " << nlohmann::json( attr_id ) << " min=(" <<
        attr.min[ 0 ] << "," << attr.min[ 1 ] << "," << attr.min[ 2 ] << "," << attr.min[ 3 ] << ") max=(" <<
        attr.max[ 0 ] << "," << attr.max[ 1 ] << "," << attr.max[ 2 ] << "," << attr.max[ 3 ] << ") size=" <<
        attr.data.size() <<
        std::endl;
    }
    for( std::uint32_t m = 0u; m != vb.meshlet_count; ++m ) {
      gct::gltf::meshlet_reader reader( vb, m );
      std::cout << "  meshlet " << m << " face_count=" << reader.get_face_count() << std::endl;
      for( std::uint32_t i = 0u; i != reader.get_face_count(); ++i ) {
        const auto f = reader( i );
        if( f.valid ) {
          std::cout << "    face " << i << std::endl;
          for( std::uint32_t j = 0u; j != f.vertex.size(); ++j ) {
            std::cout << "      ";
            if( vb.attribute.find( gct::vertex_attribute_id::position ) != vb.attribute.end() ) {
              std::cout << "p(" <<
                           f.vertex[ j ].position.x << "," <<
                           f.vertex[ j ].position.y << "," <<
                           f.vertex[ j ].position.z << ")";
            }
            if( vb.attribute.find( gct::vertex_attribute_id::normal ) != vb.attribute.end() ) {
              std::cout << "n(" <<
                           f.vertex[ j ].normal.x << "," <<
                           f.vertex[ j ].normal.y << "," <<
                           f.vertex[ j ].normal.z << ")";
            }
            if( vb.attribute.find( gct::vertex_attribute_id::tangent ) != vb.attribute.end() ) {
              std::cout << "t(" <<
                           f.vertex[ j ].tangent.x << "," <<
                           f.vertex[ j ].tangent.y << "," <<
                           f.vertex[ j ].tangent.z << "," <<
                           f.vertex[ j ].tangent.w << ")";
            }
            else if(
              vb.attribute.find( gct::vertex_attribute_id::normal ) != vb.attribute.end() &&
              vb.attribute.at( gct::vertex_attribute_id::normal ).type == gct::scene_graph::accessor_type_id::n21t11
            ) {
              std::cout << "t(" <<
                           f.vertex[ j ].tangent.x << "," <<
                           f.vertex[ j ].tangent.y << "," <<
                           f.vertex[ j ].tangent.z << "," <<
                           f.vertex[ j ].tangent.w << ")";
            }
            else if(
              vb.attribute.find( gct::vertex_attribute_id::normal ) != vb.attribute.end() &&
              vb.attribute.at( gct::vertex_attribute_id::normal ).type == gct::scene_graph::accessor_type_id::n20t11b1
            ) {
              std::cout << "t(" <<
                           f.vertex[ j ].tangent.x << "," <<
                           f.vertex[ j ].tangent.y << "," <<
                           f.vertex[ j ].tangent.z << "," <<
                           f.vertex[ j ].tangent.w << ")";
            }
            if( vb.attribute.find( gct::vertex_attribute_id::texcoord_0 ) != vb.attribute.end() ) {
              std::cout << "u(" <<
                           f.vertex[ j ].tex_coord0.x << "," <<
                           f.vertex[ j ].tex_coord0.y << ")" <<
                           std::endl;
            }
            if( vb.attribute.find( gct::vertex_attribute_id::color_0 ) != vb.attribute.end() ) {
              std::cout << "c(" <<
                           f.vertex[ j ].color0.x << "," <<
                           f.vertex[ j ].color0.y << "," <<
                           f.vertex[ j ].color0.z << "," <<
                           f.vertex[ j ].color0.w << ")" <<
                           std::endl;
            }
            if( vb.attribute.find( gct::vertex_attribute_id::joint_0 ) != vb.attribute.end() ) {
              std::cout << "j(" <<
                           f.vertex[ j ].joint0.x << "," <<
                           f.vertex[ j ].joint0.y << "," <<
                           f.vertex[ j ].joint0.z << "," <<
                           f.vertex[ j ].joint0.w << ")" <<
                           std::endl;
            }
            if( vb.attribute.find( gct::vertex_attribute_id::weight_0 ) != vb.attribute.end() ) {
              std::cout << "j(" <<
                           f.vertex[ j ].weight0.x << "," <<
                           f.vertex[ j ].weight0.y << "," <<
                           f.vertex[ j ].weight0.z << "," <<
                           f.vertex[ j ].weight0.w << ")" <<
                           std::endl;
            }
          }
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

