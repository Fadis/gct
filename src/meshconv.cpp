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

void convert(
  const std::filesystem::path &base_dir,
  fx::gltf::Document &doc,
  const std::string &out_path,
  const std::vector< std::string > &convert
) {
  auto mesh = gct::gltf::load_mesh_on_cpu( base_dir, doc );

  gct::gltf::convert_mesh( mesh, convert );

  const auto out_base_dir = std::filesystem::path( out_path ).parent_path();
  const std::string out_filename = std::filesystem::path( out_path ).filename();
  auto ext_begin = out_filename.find( ".gltf" );
  if( ext_begin == std::string::npos ) {
    ext_begin = out_filename.find( ".glb" );
  }
  std::string bin_filename;
  if( ext_begin != std::string::npos ) {
    bin_filename = out_filename.substr( 0u, ext_begin ) + ".bin";
  }
  else {
    bin_filename = out_filename + ".bin";
  }
  gct::gltf::store_mesh_on_cpu( base_dir, doc, mesh, bin_filename );
  std::fstream output( out_base_dir / out_filename, std::ios::out|std::ios::binary );
  fx::gltf::Save( doc, output, out_base_dir, false );
}

int main( int argc, const char *argv[] ) {
  namespace po = boost::program_options;
  po::options_description desc( "Options" );
  desc.add_options()
    ( "help,h", "show this message" )
    ( "input,i", po::value< std::string >(), "input glTF filename" )
    ( "output,o", po::value< std::string >(), "output glTF filename" )
    ( "convert,c", po::value< std::vector< std::string > >()->multitoken(), "convert" );
  po::variables_map vm;
  po::store( po::parse_command_line( argc, argv, desc ), vm );
  po::notify( vm );
  if( vm.count( "help" ) || !vm.count( "input" ) || !vm.count( "output" ) ) {
    std::cout << desc << std::endl;
    std::exit( 0 );
  }
  fx::gltf::ReadQuotas quota;
  quota.MaxBufferByteLength = 1024 * 1024 * 1024;
  fx::gltf::Document doc = fx::gltf::LoadFromText( vm[ "input" ].as< std::string >(), quota );
  const auto base_dir = std::filesystem::path( vm[ "input" ].as< std::string >() ).parent_path();
  convert(
    base_dir,
    doc,
    vm[ "output" ].as< std::string >(),
    vm.count( "convert" ) ? vm[ "convert" ].as< std::vector< std::string > >() : std::vector< std::string >{}
  );

}

