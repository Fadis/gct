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

