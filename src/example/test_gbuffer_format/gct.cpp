#include <gct/shader/glsl/gct/gbuffer_format.h>
#include <iostream>
#include <boost/program_options.hpp>
#include <gct/gbuffer_format.hpp>

int main( int argc, const char *argv[] ) {
  namespace po = boost::program_options;
  po::options_description desc( "Options" );
  desc.add_options()
    ( "help,h", "show this message" );
  po::variables_map vm;
  po::store( po::parse_command_line( argc, argv, desc ), vm );
  po::notify( vm );
  if( vm.count( "help" ) ) {
    std::cout << desc << std::endl;
    std::exit( 0 );
  }

  const auto gbuffer_format =
      gct::gbuffer_format::albedo_alpha |
      gct::gbuffer_format::normal |
      gct::gbuffer_format::emissive_occlusion |
      gct::gbuffer_format::metallic_roughness_id;

  std::cout << "layer count : " << gbuffer_get_layer_count( std::uint32_t( gbuffer_format ) ) << std::endl;
  
  std::cout << "position_depth offset : " << gbuffer_get_layer( std::uint32_t( gbuffer_format ), std::uint32_t( gct::gbuffer_format::position_depth ) ) << std::endl;
  std::cout << "albedo_alpha offset : " << gbuffer_get_layer( std::uint32_t( gbuffer_format ), std::uint32_t( gct::gbuffer_format::albedo_alpha ) ) << std::endl;
  std::cout << "normal offset : " << gbuffer_get_layer( std::uint32_t( gbuffer_format ), std::uint32_t( gct::gbuffer_format::normal ) ) << std::endl;
  std::cout << "emissive_occlusion offset : " << gbuffer_get_layer( std::uint32_t( gbuffer_format ), std::uint32_t( gct::gbuffer_format::emissive_occlusion ) ) << std::endl;
  std::cout << "metallic_roughness_id offset : " << gbuffer_get_layer( std::uint32_t( gbuffer_format ), std::uint32_t( gct::gbuffer_format::metallic_roughness_id ) ) << std::endl;
  std::cout << "optflow_mark offset : " << gbuffer_get_layer( std::uint32_t( gbuffer_format ), std::uint32_t( gct::gbuffer_format::optflow_mark ) ) << std::endl;
  std::cout << "shadow_level offset : " << gbuffer_get_layer( std::uint32_t( gbuffer_format ), std::uint32_t( gct::gbuffer_format::shadow_level ) ) << std::endl;
  std::cout << "tangent offset : " << gbuffer_get_layer( std::uint32_t( gbuffer_format ), std::uint32_t( gct::gbuffer_format::tangent ) ) << std::endl;
  std::cout << "texcoord0_texcoord1 ofset : " << gbuffer_get_layer( std::uint32_t( gbuffer_format ), std::uint32_t( gct::gbuffer_format::texcoord0_texcoord1 ) ) << std::endl;
  std::cout << "texcoord2_texcoord3 ofset : " << gbuffer_get_layer( std::uint32_t( gbuffer_format ), std::uint32_t( gct::gbuffer_format::texcoord2_texcoord3 ) ) << std::endl;
  std::cout << "color0 offset : " << gbuffer_get_layer( std::uint32_t( gbuffer_format ), std::uint32_t( gct::gbuffer_format::color0 ) ) << std::endl;
  std::cout << "color1 offset : " << gbuffer_get_layer( std::uint32_t( gbuffer_format ), std::uint32_t( gct::gbuffer_format::color1 ) ) << std::endl;
  
  std::cout << "has position_depth : " << gbuffer_has_layer( std::uint32_t( gbuffer_format ), std::uint32_t( gct::gbuffer_format::position_depth ) ) << std::endl;
  std::cout << "has albedo_alpha : " << gbuffer_has_layer( std::uint32_t( gbuffer_format ), std::uint32_t( gct::gbuffer_format::albedo_alpha ) ) << std::endl;
  std::cout << "has normal : " << gbuffer_has_layer( std::uint32_t( gbuffer_format ), std::uint32_t( gct::gbuffer_format::normal ) ) << std::endl;
  std::cout << "has emissive_occlusion : " << gbuffer_has_layer( std::uint32_t( gbuffer_format ), std::uint32_t( gct::gbuffer_format::emissive_occlusion ) ) << std::endl;
  std::cout << "has metallic_roughness_id : " << gbuffer_has_layer( std::uint32_t( gbuffer_format ), std::uint32_t( gct::gbuffer_format::metallic_roughness_id ) ) << std::endl;
  std::cout << "has optflow_mark : " << gbuffer_has_layer( std::uint32_t( gbuffer_format ), std::uint32_t( gct::gbuffer_format::optflow_mark ) ) << std::endl;
  std::cout << "has shadow_level : " << gbuffer_has_layer( std::uint32_t( gbuffer_format ), std::uint32_t( gct::gbuffer_format::shadow_level ) ) << std::endl;
  std::cout << "has tangent : " << gbuffer_has_layer( std::uint32_t( gbuffer_format ), std::uint32_t( gct::gbuffer_format::tangent ) ) << std::endl;
  std::cout << "has texcoord0_texcoord1 : " << gbuffer_has_layer( std::uint32_t( gbuffer_format ), std::uint32_t( gct::gbuffer_format::texcoord0_texcoord1 ) ) << std::endl;
  std::cout << "has texcoord2_texcoord3 : " << gbuffer_has_layer( std::uint32_t( gbuffer_format ), std::uint32_t( gct::gbuffer_format::texcoord2_texcoord3 ) ) << std::endl;
  std::cout << "has color0 : " << gbuffer_has_layer( std::uint32_t( gbuffer_format ), std::uint32_t( gct::gbuffer_format::color0 ) ) << std::endl;
  std::cout << "has color1 : " << gbuffer_has_layer( std::uint32_t( gbuffer_format ), std::uint32_t( gct::gbuffer_format::color1 ) ) << std::endl;
}

