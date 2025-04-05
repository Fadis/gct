#include <nlohmann/json.hpp>
#include <gct/skyview_froxel_param.hpp>

namespace gct {

void to_json( nlohmann::json &dest, const skyview_froxel_param &src ) {
  dest = nlohmann::json::object();
  dest[ "world_to_screen" ] = nlohmann::json::array();
  dest[ "world_to_screen" ].push_back( src.world_to_screen[ 0 ][ 0 ] );
  dest[ "world_to_screen" ].push_back( src.world_to_screen[ 0 ][ 1 ] );
  dest[ "world_to_screen" ].push_back( src.world_to_screen[ 0 ][ 2 ] );
  dest[ "world_to_screen" ].push_back( src.world_to_screen[ 0 ][ 3 ] );
  dest[ "world_to_screen" ].push_back( src.world_to_screen[ 1 ][ 0 ] );
  dest[ "world_to_screen" ].push_back( src.world_to_screen[ 1 ][ 1 ] );
  dest[ "world_to_screen" ].push_back( src.world_to_screen[ 1 ][ 2 ] );
  dest[ "world_to_screen" ].push_back( src.world_to_screen[ 1 ][ 3 ] );
  dest[ "world_to_screen" ].push_back( src.world_to_screen[ 2 ][ 0 ] );
  dest[ "world_to_screen" ].push_back( src.world_to_screen[ 2 ][ 1 ] );
  dest[ "world_to_screen" ].push_back( src.world_to_screen[ 2 ][ 2 ] );
  dest[ "world_to_screen" ].push_back( src.world_to_screen[ 2 ][ 3 ] );
  dest[ "world_to_screen" ].push_back( src.world_to_screen[ 3 ][ 0 ] );
  dest[ "world_to_screen" ].push_back( src.world_to_screen[ 3 ][ 1 ] );
  dest[ "world_to_screen" ].push_back( src.world_to_screen[ 3 ][ 2 ] );
  dest[ "world_to_screen" ].push_back( src.world_to_screen[ 3 ][ 3 ] );
  dest[ "screen_to_world" ] = nlohmann::json::array();
  dest[ "screen_to_world" ].push_back( src.screen_to_world[ 0 ][ 0 ] );
  dest[ "screen_to_world" ].push_back( src.screen_to_world[ 0 ][ 1 ] );
  dest[ "screen_to_world" ].push_back( src.screen_to_world[ 0 ][ 2 ] );
  dest[ "screen_to_world" ].push_back( src.screen_to_world[ 0 ][ 3 ] );
  dest[ "screen_to_world" ].push_back( src.screen_to_world[ 1 ][ 0 ] );
  dest[ "screen_to_world" ].push_back( src.screen_to_world[ 1 ][ 1 ] );
  dest[ "screen_to_world" ].push_back( src.screen_to_world[ 1 ][ 2 ] );
  dest[ "screen_to_world" ].push_back( src.screen_to_world[ 1 ][ 3 ] );
  dest[ "screen_to_world" ].push_back( src.screen_to_world[ 2 ][ 0 ] );
  dest[ "screen_to_world" ].push_back( src.screen_to_world[ 2 ][ 1 ] );
  dest[ "screen_to_world" ].push_back( src.screen_to_world[ 2 ][ 2 ] );
  dest[ "screen_to_world" ].push_back( src.screen_to_world[ 2 ][ 3 ] );
  dest[ "screen_to_world" ].push_back( src.screen_to_world[ 3 ][ 0 ] );
  dest[ "screen_to_world" ].push_back( src.screen_to_world[ 3 ][ 1 ] );
  dest[ "screen_to_world" ].push_back( src.screen_to_world[ 3 ][ 2 ] );
  dest[ "screen_to_world" ].push_back( src.screen_to_world[ 3 ][ 3 ] );
  dest[ "sigma_ma" ] = nlohmann::json::array();
  dest[ "sigma_ma" ].push_back( src.sigma_ma[ 0 ] );
  dest[ "sigma_ma" ].push_back( src.sigma_ma[ 1 ] );
  dest[ "sigma_ma" ].push_back( src.sigma_ma[ 2 ] );
  dest[ "sigma_ma" ].push_back( src.sigma_ma[ 3 ] );
  dest[ "sigma_oa" ] = nlohmann::json::array();
  dest[ "sigma_oa" ].push_back( src.sigma_oa[ 0 ] );
  dest[ "sigma_oa" ].push_back( src.sigma_oa[ 1 ] );
  dest[ "sigma_oa" ].push_back( src.sigma_oa[ 2 ] );
  dest[ "sigma_oa" ].push_back( src.sigma_oa[ 3 ] );
  dest[ "sigma_rs" ] = nlohmann::json::array();
  dest[ "sigma_rs" ].push_back( src.sigma_rs[ 0 ] );
  dest[ "sigma_rs" ].push_back( src.sigma_rs[ 1 ] );
  dest[ "sigma_rs" ].push_back( src.sigma_rs[ 2 ] );
  dest[ "sigma_rs" ].push_back( src.sigma_rs[ 3 ] );
  dest[ "sigma_ms" ] = nlohmann::json::array();
  dest[ "sigma_ms" ].push_back( src.sigma_ms[ 0 ] );
  dest[ "sigma_ms" ].push_back( src.sigma_ms[ 1 ] );
  dest[ "sigma_ms" ].push_back( src.sigma_ms[ 2 ] );
  dest[ "sigma_ms" ].push_back( src.sigma_ms[ 3 ] );
  dest[ "eye_pos" ] = nlohmann::json::array();
  dest[ "eye_pos" ].push_back( src.eye_pos[ 0 ] );
  dest[ "eye_pos" ].push_back( src.eye_pos[ 1 ] );
  dest[ "eye_pos" ].push_back( src.eye_pos[ 2 ] );
  dest[ "eye_pos" ].push_back( src.eye_pos[ 3 ] );
  dest[ "light_pos" ] = nlohmann::json::array();
  dest[ "light_pos" ].push_back( src.light_pos[ 0 ] );
  dest[ "light_pos" ].push_back( src.light_pos[ 1 ] );
  dest[ "light_pos" ].push_back( src.light_pos[ 2 ] );
  dest[ "light_pos" ].push_back( src.light_pos[ 3 ] );
  dest[ "g" ] = src.g;
  dest[ "ground_radius" ] = src.ground_radius;
  dest[ "top_radius" ] = src.top_radius;
  dest[ "altitude" ] = src.altitude;
  dest[ "light_energy" ] = src.light_energy;
}

}
