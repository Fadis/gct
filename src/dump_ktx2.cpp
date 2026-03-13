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

bool is_uastc( ktxTexture2 *tex ) {
  const khr_df_model_e color_model = ktxTexture2_GetColorModel_e( tex );
  return color_model == KHR_DF_MODEL_UASTC;
}

gct::color_profile ktx_profile_to_gct_profile( ktxTexture2 *tex ) {
  gct::color_profile profile;
  const khr_df_transfer_e gamma = ktxTexture2_GetOETF_e( tex );
  if(
    gamma == KHR_DF_TRANSFER_UNSPECIFIED ||
    gamma == KHR_DF_TRANSFER_LINEAR
  ) {
    profile.set_gamma( gct::color_gamma::linear );
    profile.set_max_intensity( 10000.0f );
  }
  else if(
    gamma == KHR_DF_TRANSFER_SRGB ||
    gamma == KHR_DF_TRANSFER_SRGB_EOTF ||
    gamma == KHR_DF_TRANSFER_SCRGB ||
    gamma == KHR_DF_TRANSFER_SCRGB_EOTF
  ) {
    profile.set_gamma( gct::color_gamma::srgb );
  }
  else if(
    gamma == KHR_DF_TRANSFER_ITU ||
    gamma == KHR_DF_TRANSFER_ITU_OETF ||
    gamma == KHR_DF_TRANSFER_BT601 ||
    gamma == KHR_DF_TRANSFER_BT601_OETF ||
    gamma == KHR_DF_TRANSFER_BT709 ||
    gamma == KHR_DF_TRANSFER_BT709_OETF ||
    gamma == KHR_DF_TRANSFER_BT2020 ||
    gamma == KHR_DF_TRANSFER_BT2020_OETF ||
    gamma == KHR_DF_TRANSFER_SMTPE170M ||
    gamma == KHR_DF_TRANSFER_SMTPE170M_OETF ||
    gamma == KHR_DF_TRANSFER_SMTPE170M_EOTF
  ) {
    profile.set_gamma( gct::color_gamma::bt709 );
  }
  else if(
    gamma == KHR_DF_TRANSFER_NTSC ||
    gamma == KHR_DF_TRANSFER_NTSC_EOTF ||
    gamma == KHR_DF_TRANSFER_ADOBERGB ||
    gamma == KHR_DF_TRANSFER_ADOBERGB_EOTF
  ) {
    profile.set_gamma( gct::color_gamma::srgb );
  }
  else if(
    gamma == KHR_DF_TRANSFER_DCIP3 ||
    gamma == KHR_DF_TRANSFER_DCIP3_EOTF
  ) {
    profile.set_gamma( gct::color_gamma::dci_p3 );
  }
  else if(
    gamma == KHR_DF_TRANSFER_HLG_OETF ||
    gamma == KHR_DF_TRANSFER_HLG_EOTF
  ) {
    profile.set_gamma( gct::color_gamma::hlg );
    profile.set_max_intensity( 10000.0f );
  }
  else if(
    gamma == KHR_DF_TRANSFER_PQ_OETF ||
    gamma == KHR_DF_TRANSFER_PQ_EOTF
  ) {
    profile.set_gamma( gct::color_gamma::pq );
    profile.set_max_intensity( 10000.0f );
  }
  else {
    profile.set_gamma( gct::color_gamma::unknown );
  }
  const khr_df_primaries_e space = ktxTexture2_GetPrimaries_e( tex );
  std::cout << "space : " << int( space ) << std::endl;
  if(
    space == KHR_DF_PRIMARIES_BT709
  ) {
    profile.set_space( gct::color_space::bt709 );
  }
  else if(
    space == KHR_DF_PRIMARIES_BT601_EBU ||
    space == KHR_DF_PRIMARIES_BT601_SMPTE
  ) {
    profile.set_space( gct::color_space::ntsc );
  }
  else if(
    space == KHR_DF_PRIMARIES_BT2020 
  ) {
    profile.set_space( gct::color_space::bt2020 );
  }
  else if(
    space == KHR_DF_PRIMARIES_CIEXYZ
  ) {
    profile.set_space( gct::color_space::cie_xyz );
  }
  else if(
    space == KHR_DF_PRIMARIES_ACES ||
    space == KHR_DF_PRIMARIES_ACESCC
  ) {
    profile.set_space( gct::color_space::aces_cg );
  }
  else if(
    space == KHR_DF_PRIMARIES_DISPLAYP3
  ) {
    profile.set_space( gct::color_space::display_p3 );
  }
  else if(
    space == KHR_DF_PRIMARIES_ADOBERGB
  ) {
    profile.set_space( gct::color_space::adobe_rgb );
  }
  else {
    profile.set_space( gct::color_space::unknown );
  }
  return profile;
}

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
  ktxTexture2* tex;
  KTX_error_code txresult = ktxTexture2_CreateFromNamedFile(
    vm[ "input" ].as< std::string >().c_str(),
    KTX_TEXTURE_CREATE_NO_FLAGS,
    &tex
  );

  const bool transcode_is_needed = ktxTexture2_NeedsTranscoding( tex );

  if( transcode_is_needed ) {
    const ktx_transcode_fmt_e transcoded_format =
      //is_uastc( tex ) ?
      //KTX_TTF_BC6HU :
      KTX_TTF_BC7_RGBA;
    KTX_error_code txresult = ktxTexture2_TranscodeBasis( tex, transcoded_format, 0u );
  }

  vk::Format format = static_cast< vk::Format >( ktxTexture2_GetVkFormat( tex ) );
  const auto profile = ktx_profile_to_gct_profile( tex );
  std::cout << "profile : " << nlohmann::json( profile ).dump( 2 ) << std::endl;
  std::cout << "format : " << nlohmann::json( format ) << std::endl;
  std::cout << "data : " << reinterpret_cast< void* >( tex->pData ) << std::endl;
  for( std::uint32_t m = 0u; m != tex->numLevels; ++m ) {
    for( std::uint32_t l = 0u; l != tex->numLayers; ++l ) {
      ktx_size_t offset = 0u;
      KTX_error_code txresult = ktxTexture2_GetImageOffset( tex, m, l, 0u, &offset );
      const ktx_size_t size = ktxTexture2_GetImageSize( tex, m );
      std::cout  << "subimage : offset=" << offset << " size=" << size << std::endl;
    }
  }
}
