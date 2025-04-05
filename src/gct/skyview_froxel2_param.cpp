#include <nlohmann/json.hpp>
#include <gct/skyview_froxel2_param.hpp>
#include <gct/color_space.hpp>

namespace gct {

void to_json( nlohmann::json &dest, const skyview_froxel2_param &src ) {
  dest = nlohmann::json::object();
  dest[ "sigma" ] = src.sigma;
  dest[ "g" ] = src.g;
  dest[ "ground_radius" ] = src.ground_radius;
  dest[ "top_radius" ] = src.top_radius;
  dest[ "altitude" ] = src.altitude;
}

glm::mat4 get_default_skyview_sigma() {
  const auto rgb_to_xyz = get_rgb_to_xyz( color_space::bt709 );
  return glm::mat4(
    rgb_to_xyz * glm::vec4( 4.40e-6, 4.40e-6, 4.40e-6, 0.0 ),
    rgb_to_xyz * glm::vec4( 0.650e-6, 1.881e-6, 0.085e-6, 0.0 ),
    rgb_to_xyz * glm::vec4( 5.802e-6, 13.558e-6, 33.1e-6, 0.0 ),
    rgb_to_xyz * glm::vec4( 3.996e-6, 3.996e-6, 3.996e-6, 0.0 )
  );
}

}
