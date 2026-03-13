#include <iostream>
#include <boost/program_options.hpp>
#include <vulkan/vulkan.hpp>
#include <ktx.h>
#include <ktxvulkan.h>
#include <vulkan2json/Format.hpp>
#include <nlohmann/json.hpp>
#include <gct/format.hpp>
#include <gct/color_space.hpp>

extern "C" ktx_size_t ktxTexture2_GetImageSize(ktxTexture2* This, ktx_uint32_t level);

int main(
  int argc,
  const char *argv[]
) {
  namespace po = boost::program_options;
  po::options_description desc( "Options" );
  desc.add_options()
    ( "help,h", "show this message" )
    ( "input,i", po::value< std::string >(), "DDS filename" );
  po::variables_map vm;
  po::store( po::parse_command_line( argc, argv, desc ), vm );
  po::notify( vm );
  if( vm.count( "help" ) || !vm.count( "input" ) ) {
    std::cout << desc << std::endl;
    exit( 0 );
  }
  ktxTexture* tex;
  KTX_error_code txresult = ktxTexture_CreateFromNamedFile(
    vm[ "input" ].as< std::string >().c_str(),
    KTX_TEXTURE_CREATE_NO_FLAGS,
    &tex
  );

  vk::Format format = static_cast< vk::Format >( ktxTexture_GetVkFormat( tex ) );
  const bool is_hdr = gct::is_hdr( format );
  const gct::color_profile profile =
    is_hdr ?
    gct::color_profile()
      .set_space( gct::color_space::cie_xyz )
      .set_gamma( gct::color_gamma::linear ) :
    gct::color_profile()
      .set_space( gct::color_space::bt709 )
      .set_gamma( gct::color_gamma::srgb );

  std::cout << "profile : " << nlohmann::json( profile ).dump( 2 ) << std::endl;
  std::cout << "format : " << nlohmann::json( format ) << std::endl;
  std::cout << "data : " << reinterpret_cast< void* >( tex->pData ) << std::endl;
  for( std::uint32_t m = 0u; m != tex->numLevels; ++m ) {
    for( std::uint32_t l = 0u; l != tex->numLayers; ++l ) {
      ktx_size_t offset = 0u;
      KTX_error_code txresult = ktxTexture_GetImageOffset( tex, m, l, 0u, &offset );
      const ktx_size_t size = ktxTexture_GetImageSize( tex, m );
      std::cout  << "subimage : offset=" << offset << " size=" << size << std::endl;
    }
  }
}
