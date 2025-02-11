#include <cctype>
#include <nlohmann/json.hpp>
#include <gct/color_space.hpp>

namespace gct {
glm::mat4 get_rgb_to_xyz( color_space cs ) {
  if( cs == color_space::cie_xyz ) {
    return glm::mat4( 1.0 );
  }
  else if( cs == color_space::ntsc ) {
    return glm::mat4(
      0.3935390, 0.2123861, 0.0187400, 0.0,
      0.3652650, 0.7010731, 0.1119360, 0.0,
      0.1916261, 0.0865408, 0.9581305, 0.0,
      0.0,       0.0,       0.0,       1.0
    );
  }
  else if( cs == color_space::bt709 ) {
    return glm::mat4(
      0.4124079, 0.2126478, 0.0193316, 0.0,
      0.3575896, 0.7151791, 0.1191965, 0.0,
      0.1804326, 0.0721730, 0.9502783, 0.0,
      0.0,       0.0,       0.0,       1.0
    );
  }
  else if( cs == color_space::apple_rgb ) {
    return glm::mat4(
      0.4496808, 0.2446264, 0.0251821, 0.0,
      0.3162620, 0.6720568, 0.1411884, 0.0,
      0.1844872, 0.0833168, 0.9224360, 0.0,
      0.0,       0.0,       0.0,       1.0
    );
  }
  else if( cs == color_space::adobe_rgb ) {
    return glm::mat4(
      0.5766885, 0.2973550, 0.0270323, 0.0,
      0.1855610, 0.6273727, 0.0706899, 0.0,
      0.1881806, 0.0752722, 0.9910843, 0.0,
      0.0,       0.0,       0.0,       1.0
    );
  }
  else if( cs == color_space::display_p3 ) {
    return glm::mat4(
      0.4865888, 0.2289830, 0.0000000, 0.0,
      0.2656718, 0.6917493, 0.0451141, 0.0,
      0.1981694, 0.0792678, 1.0436924, 0.0,
      0.0,       0.0,       0.0,       1.0
    );
  }
  else if( cs == color_space::bt2020 ) {
    return glm::mat4(
      0.6369703, 0.2627053, 0.0000000, 0.0,
      0.1446188, 0.6780071, 0.0280731, 0.0,
      0.1688409, 0.0592877, 1.0607334, 0.0,
      0.0,       0.0,       0.0,       1.0
    );
  }
  else if( cs == color_space::aces_cg ) {
    return glm::mat4(
      0.6624541811, 0.1340042065, 0.1561876870, 0.0,
      0.2722287168, 0.6740817658, 0.0536895174, 0.0,
      -0.0055746495, 0.0040607335, 1.0103391003, 0.0,
      0.0,          0.0,          0.0,          1.0
    );
  }
  else {
    throw -1;
  }
}

glm::mat4 get_xyz_to_rgb( color_space cs ) {
  if( cs == color_space::cie_xyz ) {
    return glm::mat4( 1.0 );
  }
  else if( cs == color_space::ntsc ) {
    return glm::mat4(
      3.5058421,  -1.0690274, 0.0563215,  0.0,
      -1.7397107, 1.9777417,  -0.1970279, 0.0,
      -0.5440333, 0.0351708,  1.0502309,  0.0,
      0.0,        0.0,        0.0,        1.0
    );
  }
  else if( cs == color_space::bt709 ) {
    return glm::mat4(
      3.2408357,  -0.9692294, 0.0556449,  0.0,
      -1.5373195, 1.8759400,  -0.2040314, 0.0,
      -0.4985901, 0.0415544,  1.0572538,  0.0,
      0.0,        0.0,        0.0,        1.0
    );
  }
  else if( cs == color_space::apple_rgb ) {
    return glm::mat4(
      2.9518525,  -1.0850633, 0.0854958,  0.0,
      -1.2895492, 1.9907721,  -0.2695042, 0.0,
      -0.4738951, 0.0372010,  1.0913292,  0.0,
      0.0,        0.0,        0.0,        1.0
    );
  }
  else if( cs == color_space::adobe_rgb ) {
    return glm::mat4(
      2.0415189,  -0.9692294, 0.0134477,  0.0,
      -0.5649879, 1.8759400,  -0.1183926, 0.0,
      -0.3447197, 0.0415544,  1.0154344,  0.0,
      0.0,        0.0,        0.0,        1.0
    );
  }
  else if( cs == color_space::display_p3 ) {
    return glm::mat4(
      2.4934055,  -0.8294761, 0.0358545,  0.0,
      -0.9313495, 1.7626367,  -0.0761908, 0.0,
      -0.4026960, 0.0236243,  0.9571155,  0.0,
      0.0,        0.0,        0.0,        1.0
    );
  }
  else if( cs == color_space::bt2020 ) {
    return glm::mat4(
      1.7166182,  -0.6666755, 0.0176440,  0.0,
      -0.3556639, 1.6164598,  -0.0427808, 0.0,
      -0.2533614, 0.0157683,  0.9423266,  0.0,
      0.0,        0.0,        0.0,        1.0
    );
  }
  else if( cs == color_space::aces_cg ) {
    return glm::mat4(
      1.6410233797, -0.3248032942, -0.2364246952, 0.0,
      -0.6636628587, 1.6153315917, 0.0167563477,  0.0,
      0.0117218943, -0.0082844420, 0.9883948585,  0.0,
      0.0,          0.0,           0.0,           1.0
    );
  }
  else {
    throw -1;
  }
}

color_space_matrix allocate_color_space_matrix( matrix_pool &pool ) {
  return color_space_matrix()
    .emplace_from( color_space_matrix_map{
      { color_space::ntsc, pool.allocate( get_rgb_to_xyz( color_space::ntsc ) ) },
      { color_space::bt709, pool.allocate( get_rgb_to_xyz( color_space::bt709 ) ) },
      { color_space::apple_rgb, pool.allocate( get_rgb_to_xyz( color_space::apple_rgb ) ) },
      { color_space::adobe_rgb, pool.allocate( get_rgb_to_xyz( color_space::adobe_rgb ) ) },
      { color_space::display_p3, pool.allocate( get_rgb_to_xyz( color_space::display_p3 ) ) },
      { color_space::bt2020, pool.allocate( get_rgb_to_xyz( color_space::bt2020 ) ) },
      { color_space::aces_cg, pool.allocate( get_rgb_to_xyz( color_space::aces_cg ) ) }
    } )
    .emplace_to( color_space_matrix_map{
      { color_space::ntsc, pool.allocate( get_xyz_to_rgb( color_space::ntsc ) ) },
      { color_space::bt709, pool.allocate( get_xyz_to_rgb( color_space::bt709 ) ) },
      { color_space::apple_rgb, pool.allocate( get_xyz_to_rgb( color_space::apple_rgb ) ) },
      { color_space::adobe_rgb, pool.allocate( get_xyz_to_rgb( color_space::adobe_rgb ) ) },
      { color_space::display_p3, pool.allocate( get_xyz_to_rgb( color_space::display_p3 ) ) },
      { color_space::bt2020, pool.allocate( get_xyz_to_rgb( color_space::bt2020 ) ) },
      { color_space::aces_cg, pool.allocate( get_xyz_to_rgb( color_space::aces_cg ) ) }
    } );
}

color_profile parse_color_space( const std::string_view &name ) {
  std::string simplified;
  std::transform(
    name.begin(), name.end(), std::back_inserter( simplified ),
    []( char c ) -> char {
      if( c == '_' ) {
        return ' ';
      }
      else if( c == '-' ) {
        return ' ';
      }
      else if( c < 0x80 ) {
        return tolower( c );
      }
      else {
        return c;
      }
    }
  );
  {
    std::string copied = std::move( simplified );
    simplified.clear();
    bool blank_cont = true;
    for( char c : copied ) {
      if( c == ' ' ) {
        if( !blank_cont ) {
          simplified += c;
          blank_cont = true;
        }
      }
      else {
        simplified += c;
        blank_cont = false;
      }
    }
    if( blank_cont ) {
      simplified.pop_back();
    }
  }
  const auto attr_begin = std::find( simplified.begin(), simplified.end(), '(' );
  const auto attr_end = std::find( attr_begin, simplified.end(), ')' );
  std::string_view attr;
  std::string_view prefix;
  std::string_view variant;
  if( attr_begin != simplified.end() && attr_end != simplified.end() ) {
    attr = std::string_view( std::next( attr_begin ), attr_end );
    simplified.erase( attr_begin, std::next( attr_end ) );
  }
  color_space space;
  color_gamma gamma;
  float max_intensity = 100.0;
  auto cs_name_head = simplified.find( "srgb" );
  if(
    cs_name_head != std::string::npos
  ) {
    space = color_space::bt709;
    gamma = color_gamma::srgb;
    max_intensity = 100.0;
    prefix = std::string_view( simplified.begin(), std::next( std::next( simplified.begin(), cs_name_head ) ) );
    variant = std::string_view( std::next( simplified.begin(), cs_name_head + 4 ), simplified.end() );
  }
  else if(
    ( cs_name_head = simplified.find( "scene_linear" ) ) != std::string::npos
  ) {
    space = color_space::aces_cg;
    gamma = color_gamma::linear;
    max_intensity = 10000.0;
    prefix = std::string_view( simplified.begin(), std::next( std::next( simplified.begin(), cs_name_head ) ) );
    variant = std::string_view( std::next( simplified.begin(), cs_name_head + 12 ), simplified.end() );
  }
  else if(
    ( cs_name_head = simplified.find( "adobe rgb" ) ) != std::string::npos
  ) {
    space = color_space::adobe_rgb;
    gamma = color_gamma::srgb;
    max_intensity = 100.0;
    prefix = std::string_view( simplified.begin(), std::next( simplified.begin(), cs_name_head ) );
    variant = std::string_view( std::next( simplified.begin(), cs_name_head + 9 ), simplified.end() );
  }
  else if(
    ( cs_name_head = simplified.find( "adobergb" ) ) != std::string::npos
  ) {
    space = color_space::adobe_rgb;
    gamma = color_gamma::srgb;
    max_intensity = 100.0;
    prefix = std::string_view( simplified.begin(), std::next( simplified.begin(), cs_name_head ) );
    variant = std::string_view( std::next( simplified.begin(), cs_name_head + 8 ), simplified.end() );
  }
  else if(
    ( cs_name_head = simplified.find( "adobe" ) ) != std::string::npos
  ) {
    space = color_space::adobe_rgb;
    gamma = color_gamma::srgb;
    max_intensity = 100.0;
    prefix = std::string_view( simplified.begin(), std::next( simplified.begin(), cs_name_head ) );
    variant = std::string_view( std::next( simplified.begin(), cs_name_head + 5 ), simplified.end() );
  }
  else if(
    ( cs_name_head = simplified.find( "rec.709" ) ) != std::string::npos
  ) {
    space = color_space::bt709;
    gamma = color_gamma::bt709;
    max_intensity = 100.0;
    prefix = std::string_view( simplified.begin(), std::next( simplified.begin(), cs_name_head ) );
    variant = std::string_view( std::next( simplified.begin(), cs_name_head + 7 ), simplified.end() );
  }
  else if(
    ( cs_name_head = simplified.find( "rec709" ) ) != std::string::npos
  ) {
    space = color_space::bt709;
    gamma = color_gamma::bt709;
    max_intensity = 100.0;
    prefix = std::string_view( simplified.begin(), std::next( simplified.begin(), cs_name_head ) );
    variant = std::string_view( std::next( simplified.begin(), cs_name_head + 6 ), simplified.end() );
  }
  else if(
    ( cs_name_head = simplified.find( "bt.709" ) ) != std::string::npos
  ) {
    space = color_space::bt709;
    gamma = color_gamma::bt709;
    max_intensity = 100.0;
    prefix = std::string_view( simplified.begin(), std::next( simplified.begin(), cs_name_head ) );
    variant = std::string_view( std::next( simplified.begin(), cs_name_head + 6 ), simplified.end() );
  }
  else if(
    ( cs_name_head = simplified.find( "bt709" ) ) != std::string::npos
  ) {
    space = color_space::bt709;
    gamma = color_gamma::bt709;
    max_intensity = 100.0;
    prefix = std::string_view( simplified.begin(), std::next( simplified.begin(), cs_name_head ) );
    variant = std::string_view( std::next( simplified.begin(), cs_name_head + 5 ), simplified.end() );
  }
  else if(
    ( cs_name_head = simplified.find( "rec.2020" ) ) != std::string::npos
  ) {
    space = color_space::bt2020;
    gamma = color_gamma::bt709;
    max_intensity = 100.0;
    prefix = std::string_view( simplified.begin(), std::next( simplified.begin(), cs_name_head ) );
    variant = std::string_view( std::next( simplified.begin(), cs_name_head + 8 ), simplified.end() );
  }
  else if(
    ( cs_name_head = simplified.find( "rec2020" ) ) != std::string::npos
  ) {
    space = color_space::bt2020;
    gamma = color_gamma::bt709;
    max_intensity = 100.0;
    prefix = std::string_view( simplified.begin(), std::next( simplified.begin(), cs_name_head ) );
    variant = std::string_view( std::next( simplified.begin(), cs_name_head + 7 ), simplified.end() );
  }
  else if(
    ( cs_name_head = simplified.find( "bt.2020" ) ) != std::string::npos
  ) {
    space = color_space::bt2020;
    gamma = color_gamma::bt709;
    max_intensity = 100.0;
    prefix = std::string_view( simplified.begin(), std::next( simplified.begin(), cs_name_head ) );
    variant = std::string_view( std::next( simplified.begin(), cs_name_head + 7 ), simplified.end() );
  }
  else if(
    ( cs_name_head = simplified.find( "bt2020" ) ) != std::string::npos
  ) {
    space = color_space::bt2020;
    gamma = color_gamma::bt709;
    max_intensity = 100.0;
    prefix = std::string_view( simplified.begin(), std::next( simplified.begin(), cs_name_head ) );
    variant = std::string_view( std::next( simplified.begin(), cs_name_head + 6 ), simplified.end() );
  }
  else if(
    ( cs_name_head = simplified.find( "rec.2100" ) ) != std::string::npos
  ) {
    space = color_space::bt2020;
    gamma = color_gamma::pq;
    max_intensity = 10000.0;
    prefix = std::string_view( simplified.begin(), std::next( simplified.begin(), cs_name_head ) );
    variant = std::string_view( std::next( simplified.begin(), cs_name_head + 8 ), simplified.end() );
  }
  else if(
    ( cs_name_head = simplified.find( "rec2100" ) ) != std::string::npos
  ) {
    space = color_space::bt2020;
    gamma = color_gamma::pq;
    max_intensity = 10000.0;
    prefix = std::string_view( simplified.begin(), std::next( simplified.begin(), cs_name_head ) );
    variant = std::string_view( std::next( simplified.begin(), cs_name_head + 7 ), simplified.end() );
  }
  else if(
    ( cs_name_head = simplified.find( "bt.2100" ) ) != std::string::npos
  ) {
    space = color_space::bt2020;
    gamma = color_gamma::pq;
    max_intensity = 10000.0;
    prefix = std::string_view( simplified.begin(), std::next( simplified.begin(), cs_name_head ) );
    variant = std::string_view( std::next( simplified.begin(), cs_name_head + 7 ), simplified.end() );
  }
  else if(
    ( cs_name_head = simplified.find( "bt2100" ) ) != std::string::npos
  ) {
    space = color_space::bt2020;
    gamma = color_gamma::pq;
    max_intensity = 10000.0;
    prefix = std::string_view( simplified.begin(), std::next( simplified.begin(), cs_name_head ) );
    variant = std::string_view( std::next( simplified.begin(), cs_name_head + 6 ), simplified.end() );
  }
  else if(
    ( cs_name_head = simplified.find( "acescg" ) ) != std::string::npos
  ) {
    space = color_space::aces_cg;
    gamma = color_gamma::linear;
    max_intensity = 10000.0;
    prefix = std::string_view( simplified.begin(), std::next( simplified.begin(), cs_name_head ) );
    variant = std::string_view( std::next( simplified.begin(), cs_name_head + 6 ), simplified.end() );
  }
  else if(
    ( cs_name_head = simplified.find( "aces" ) ) != std::string::npos
  ) {
    space = color_space::aces_cg;
    gamma = color_gamma::linear;
    max_intensity = 10000.0;
    prefix = std::string_view( simplified.begin(), std::next( simplified.begin(), cs_name_head ) );
    variant = std::string_view( std::next( simplified.begin(), cs_name_head + 4 ), simplified.end() );
  }
  else if(
    ( cs_name_head = simplified.find( "cie xyz" ) ) != std::string::npos
  ) {
    space = color_space::cie_xyz;
    gamma = color_gamma::linear;
    max_intensity = 10000.0;
    prefix = std::string_view( simplified.begin(), std::next( simplified.begin(), cs_name_head ) );
    variant = std::string_view( std::next( simplified.begin(), cs_name_head + 7 ), simplified.end() );
  }
  else if(
    ( cs_name_head = simplified.find( "xyz" ) ) != std::string::npos
  ) {
    space = color_space::cie_xyz;
    gamma = color_gamma::linear;
    max_intensity = 10000.0;
    prefix = std::string_view( simplified.begin(), std::next( simplified.begin(), cs_name_head ) );
    variant = std::string_view( std::next( simplified.begin(), cs_name_head + 3 ), simplified.end() );
  }
  else if(
    ( cs_name_head = simplified.find( "display p3" ) ) != std::string::npos
  ) {
    space = color_space::display_p3;
    gamma = color_gamma::srgb;
    max_intensity = 100.0;
    prefix = std::string_view( simplified.begin(), std::next( simplified.begin(), cs_name_head ) );
    variant = std::string_view( std::next( simplified.begin(), cs_name_head + 10 ), simplified.end() );
  }
  else if(
    ( cs_name_head = simplified.find( "p3" ) ) != std::string::npos
  ) {
    space = color_space::display_p3;
    gamma = color_gamma::dci_p3;
    max_intensity = 100.0;
    prefix = std::string_view( simplified.begin(), std::next( simplified.begin(), cs_name_head ) );
    variant = std::string_view( std::next( simplified.begin(), cs_name_head + 2 ), simplified.end() );
  }
  else if(
    ( cs_name_head = simplified.find( "ntsc" ) ) != std::string::npos
  ) {
    space = color_space::ntsc;
    gamma = color_gamma::srgb;
    max_intensity = 100.0;
    prefix = std::string_view( simplified.begin(), std::next( simplified.begin(), cs_name_head ) );
    variant = std::string_view( std::next( simplified.begin(), cs_name_head + 4 ), simplified.end() );
  }
  else if(
    ( cs_name_head = simplified.find( "apple rgb" ) ) != std::string::npos
  ) {
    space = color_space::apple_rgb;
    gamma = color_gamma::apple_rgb;
    max_intensity = 100.0;
    prefix = std::string_view( simplified.begin(), std::next( simplified.begin(), cs_name_head ) );
    variant = std::string_view( std::next( simplified.begin(), cs_name_head + 9 ), simplified.end() );
  }
  else if(
    ( cs_name_head = simplified.find( "applergb" ) ) != std::string::npos
  ) {
    space = color_space::apple_rgb;
    gamma = color_gamma::apple_rgb;
    max_intensity = 100.0;
    prefix = std::string_view( simplified.begin(), std::next( simplified.begin(), cs_name_head ) );
    variant = std::string_view( std::next( simplified.begin(), cs_name_head + 9 ), simplified.end() );
  }
  else {
    throw -1;
  }

  if(
    variant.find( "linear" ) != std::string_view::npos ||
    prefix.find( "linear" ) != std::string_view::npos ||
    variant.find( "lin" ) != std::string_view::npos ||
    prefix.find( "lin" ) != std::string_view::npos
  ) {
    gamma = color_gamma::linear;
  }
  else if( variant.find( "st2048" ) != std::string_view::npos ) {
    gamma = color_gamma::pq;
  }
  else if( variant.find( "pq" ) != std::string_view::npos ) {
    gamma = color_gamma::pq;
  }
  else if( variant.find( "hlg" ) != std::string_view::npos ) {
    gamma = color_gamma::hlg;
  }
  else if( attr.find( "rec.709" ) != std::string_view::npos ) {
    gamma = color_gamma::bt709;
  }
  else if( attr.find( "rec709" ) != std::string_view::npos ) {
    gamma = color_gamma::bt709;
  }

  if( attr.find( "10000 nit" ) != std::string_view::npos ) {
    max_intensity = 10000.0;
  }
  else if( attr.find( "10000nit" ) != std::string_view::npos ) {
    max_intensity = 10000.0;
  }
  else if( attr.find( "4000 nit" ) != std::string_view::npos ) {
    max_intensity = 4000.0;
  }
  else if( attr.find( "4000nit" ) != std::string_view::npos ) {
    max_intensity = 4000.0;
  }
  else if( attr.find( "2000 nit" ) != std::string_view::npos ) {
    max_intensity = 2000.0;
  }
  else if( attr.find( "2000nit" ) != std::string_view::npos ) {
    max_intensity = 2000.0;
  }
  else if( attr.find( "1000 nit" ) != std::string_view::npos ) {
    max_intensity = 1000.0;
  }
  else if( attr.find( "1000nit" ) != std::string_view::npos ) {
    max_intensity = 1000.0;
  }
  else if( attr.find( "100 nit" ) != std::string_view::npos ) {
    max_intensity = 100.0;
  }
  else if( attr.find( "100nit" ) != std::string_view::npos ) {
    max_intensity = 100.0;
  }

  return color_profile()
    .set_space( space )
    .set_gamma( gamma )
    .set_max_intensity( max_intensity );
}

float get_gamma_value( color_gamma v ) {
  if( v == color_gamma::linear ) {
    return 1.0f;
  }
  else if( v == color_gamma::apple_rgb ) {
    return 1.8f;
  }
  else if( v == color_gamma::srgb ) {
    return 2.2f;
  }
  else if( v == color_gamma::bt709 ) {
    return 2.4f;
  }
  else if( v == color_gamma::dci_p3 ) {
    return 2.6f;
  }
  else if( v == color_gamma::hlg ) {
    return 2.2f;
  }
  else if( v == color_gamma::pq ) {
    return 1.0f;
  }
  else {
    return 1.0f;
  }
};
void to_json( nlohmann::json &to, color_space from ) {
  if( from == color_space::cie_xyz ) {
    to = "cie_xyz";
  }
  else if( from == color_space::ntsc ) {
    to = "ntsc";
  }
  else if( from == color_space::bt709 ) {
    to = "bt709";
  }
  else if( from == color_space::apple_rgb ) {
    to = "apple_rgb";
  }
  else if( from == color_space::adobe_rgb ) {
    to = "adobe_rgb";
  }
  else if( from == color_space::display_p3 ) {
    to = "display_p3";
  }
  else if( from == color_space::bt2020 ) {
    to = "bt2020";
  }
  else if( from == color_space::aces_cg ) {
    to = "aces_cg";
  }
  else {
    to = "unknown";
  }
}
void to_json( nlohmann::json &to, color_gamma from ) {
  if( from == color_gamma::linear ) {
    to = "linear";
  }
  else if( from == color_gamma::apple_rgb ) {
    to = "apple_rgb";
  }
  else if( from == color_gamma::srgb ) {
    to = "srgb";
  }
  else if( from == color_gamma::bt709 ) {
    to = "bt709";
  }
  else if( from == color_gamma::dci_p3 ) {
    to = "dci_p3";
  }
  else if( from == color_gamma::hlg ) {
    to = "hlg";
  }
  else if( from == color_gamma::pq ) {
    to = "pq";
  }
  else {
    to = "unknown";
  }
}
void to_json( nlohmann::json &to, const color_profile &from ) {
  to = nlohmann::json::object();
  to[ "space" ] = from.space;
  to[ "gamma" ] = from.gamma;
  to[ "max_intensity" ] = from.max_intensity;
}
std::string to_string( color_space from ) {
  if( from == color_space::cie_xyz ) {
    return "cie_xyz";
  }
  else if( from == color_space::ntsc ) {
    return "ntsc";
  }
  else if( from == color_space::bt709 ) {
    return "bt709";
  }
  else if( from == color_space::apple_rgb ) {
    return "apple_rgb";
  }
  else if( from == color_space::adobe_rgb ) {
    return "adobe_rgb";
  }
  else if( from == color_space::display_p3 ) {
    return "display_p3";
  }
  else if( from == color_space::bt2020 ) {
    return "bt2020";
  }
  else if( from == color_space::aces_cg ) {
    return "aces_cg";
  }
  else {
    return "unknown";
  }
}
void to_json( nlohmann::json &dest, const color_space_matrix &src ) {
  dest = nlohmann::json::object();
  dest[ "from" ] = nlohmann::json::object();
  for( const auto &p: src.from ) {
    if( p.second ) {
      dest[ "from" ][ to_string( p.first ) ] = *p.second;
    }
    else {
      dest[ "from" ][ to_string( p.first ) ] = nullptr;
    }
  }
  dest[ "to" ] = nlohmann::json::object();
  for( const auto &p: src.to ) {
    if( p.second ) {
      dest[ "to" ][ to_string( p.first ) ] = *p.second;
    }
    else {
      dest[ "to" ][ to_string( p.first ) ] = nullptr;
    }
  }
}

}

