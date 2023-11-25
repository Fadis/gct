float atan2( float y, float x ) {
  bool s = abs( x ) > abs( y );
  return s ? atan( y, x ) : pi / 2.0 - atan( x, y );
  if( s ) {
    if( x >= 0.0 ) {
      return atan( y, x );
    }
    else {
      return pi * sign( y ) + atan( y, x );
    }
  }
  else {
    return pi/2.0 * sign( y ) - atan( x, y );
  }
}

float get_min_rad( float altitude, float ground_radius ) {
  return atan2( -sqrt( altitude*altitude - ground_radius*ground_radius ), ground_radius );
}

float get_end_x( float tangent, float altitude, float top_radius, float ground_radius ) {
  return ( - tangent * altitude + sqrt( - altitude * altitude + top_radius * top_radius + tangent * tangent * top_radius * top_radius ) )/( 1 + tangent * tangent );
}

float get_end_y( float tangent, float altitude, float end_x ) {
  return tangent * end_x + altitude;
}

float relative_rayleigh_density( float altitude_from_ground ) {
  return exp( -altitude_from_ground / 8000.0 );
}

float relative_mie_density( float altitude_from_ground ) {
  return exp( -altitude_from_ground / 1200.0 );
}

float relative_ozone_density( float altitude_from_ground ) {
  return max( 0, 1 - abs( altitude_from_ground - 25000.0 )/15000.0 );
}


vec3 llar_to_world( float latitude, float longitude ) {
  return vec3(
    cos( latitude ) * cos( longitude ),
    sin( latitude ),
    cos( latitude ) * sin( longitude )
  );
}

vec2 lat_to_world( float latitude ) {
  return vec2(
    cos( latitude ),
    sin( latitude )
  );
}

vec2 world_to_llar( vec3 n ) {
  return vec2(
    atan2( n.y, sqrt( n.x*n.x+n.z*n.z ) ),
    atan2( n.z, n.x )
  );
}


float get_atmosphere_distance( float altitude, vec3 direction, float radius ) {
  const float ad2 = 2.0*altitude*direction.y;
  return (
    -2.0*altitude*direction.y
    + sqrt(
      ad2 * ad2
      -4.0*(
          altitude*altitude
        - radius*radius
      )*(
          direction.x*direction.x
        + direction.y*direction.y 
        + direction.z*direction.z
      )
    )
  )/(
    2.0*(
        direction.x*direction.x
      + direction.y*direction.y 
      + direction.z*direction.z
    )
  );
}

float skyview_latitude_to_texcoord( float l ) {
  return 0.5 - 0.5 * sign( l ) * sqrt( abs( l )/( pi / 2.0 ) );
}

float skyview_texcoord_to_latitude( float t ) {
  const float a = ( ( - t + 0.5 )/ 0.5 );
  return sign( a ) * a * a * ( pi / 2.0 );
}

