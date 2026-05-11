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

void inspect_primitive(
  const std::filesystem::path &base_dir,
  const fx::gltf::Document &doc,
  const fx::gltf::Primitive &p,
  const std::vector< std::string > &convert
) {
  auto vb = gct::gltf::load_vertex( base_dir, doc, p );

  for( const auto &c: convert ) {
    if( c == "n31" ) {
      gct::gltf::convert_to_n31( vb );
    }
    else if( c == "t31" ) {
      gct::gltf::convert_to_t31( vb );
    }
    else if( c == "n21t11" ) {
      gct::gltf::convert_to_n21t11( vb );
    }
    else if( c == "n20t11b1" ) {
      gct::gltf::convert_to_n20t11b1( vb );
    }
    else if( c.substr( 0, 6 ) == "float_" ) {
      gct::gltf::convert_to_fixed( vb, gct::to_vertex_attribute_id( c.substr( 6 ) ) );
    }
    else if( c.substr( 0, 5 ) == "half_" ) {
      gct::gltf::convert_to_half( vb, gct::to_vertex_attribute_id( c.substr( 5 ) ) );
    }
    else if( c.substr( 0, 6 ) == "fixed_" ) {
      gct::gltf::convert_to_fixed( vb, gct::to_vertex_attribute_id( c.substr( 6 ) ) );
    }
    else if( c == "generate_tangent" ) {
      gct::gltf::generate_tangent( vb );
    }
  }

  for( std::uint32_t m = 0u; m != vb.meshlet_count; ++m ) {
    std::cout << "m " << m << std::endl;
    gct::gltf::meshlet_reader reader( vb, m );
    for( std::uint32_t i = 0u; i != reader.get_face_count(); ++i ) {
      const auto f = reader( i );
      if( f.valid ) {
        std::cout << "  f " << i << std::endl;
        for( std::uint32_t j = 0u; j != f.vertex.size(); ++j ) {
          std::cout << "    ";
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

void inspect_mesh(
  const std::filesystem::path &base_dir,
  const fx::gltf::Document &doc,
  const fx::gltf::Mesh &m,
  const std::vector< std::string > &convert
) {
  for( const auto &p: m.primitives ) {
    inspect_primitive( base_dir, doc, p, convert );
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
  for( const auto &m: doc.meshes ) {
    inspect_mesh( base_dir, doc, m, vm.count( "convert" ) ? vm[ "convert" ].as< std::vector< std::string > >() : std::vector< std::string >{} );
  }

}

