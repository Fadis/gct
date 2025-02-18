#include <nlohmann/json.hpp>
#include <gct/af_state.hpp>

namespace gct {

void to_json( nlohmann::json &dest, const af_state &src ) {
  dest = nlohmann::json::object();
  dest[ "history" ] = src.history;
  dest[ "max_history" ] = src.max_history;
  dest[ "depth" ] = src.depth;
  dest[ "znear" ] = src.znear;
  dest[ "zfar" ] = src.zfar;
  dest[ "focal_distance" ] = src.focal_distance;
  dest[ "lens_size" ] = src.lens_size;
  dest[ "sensor_size" ] = src.sensor_size;
  dest[ "visible_range" ] = src.visible_range;
}

void from_json( const nlohmann::json &src, af_state &dest ) {
  dest = af_state();
  if( src.find( "history" ) != src.end() ) {
    dest.set_history( std::uint32_t( src[ "history" ] ) );
  }
  if( src.find( "max_history" ) != src.end() ) {
    dest.set_max_history( std::uint32_t( src[ "max_history" ] ) );
  }
  if( src.find( "depth" ) != src.end() ) {
    dest.set_depth( float( src[ "depth" ] ) );
  }
  if( src.find( "znear" ) != src.end() ) {
    dest.set_znear( float( src[ "znear" ] ) );
  }
  if( src.find( "zfar" ) != src.end() ) {
    dest.set_zfar( float( src[ "zfar" ] ) );
  }
  if( src.find( "focal_distance" ) != src.end() ) {
    dest.set_focal_distance( float( src[ "focal_distance" ] ) );
  }
  if( src.find( "lens_size" ) != src.end() ) {
    dest.set_lens_size( float( src[ "lens_size" ] ) );
  }
  if( src.find( "sensor_size" ) != src.end() ) {
    dest.set_sensor_size( float( src[ "sensor_size" ] ) );
  }
  if( src.find( "visible_range" ) != src.end() ) {
    dest.set_sensor_size( float( src[ "visible_range" ] ) );
  }
}

}
