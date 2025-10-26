#ifndef GCT_SHADER_PRE_DOF_PIXEL_H
#define GCT_SHADER_PRE_DOF_PIXEL_H

struct pre_dof_pixel {
  vec4 near_total;
  vec4 far_total;
  float near_depth;
  float far_depth;
};

pre_dof_pixel pdp_mix(
  pre_dof_pixel l,
  pre_dof_pixel r
) {
  pre_dof_pixel mixed;
  /*mixed.near_total =
    ( l.near_depth > r.near_depth ) ?
    vec4(
      mix( l.near_total.xyz, r.near_total.xyz, r.near_total.a ),
      1.0 - ( 1.0 - l.near_total.a ) * ( 1.0 - r.near_total.a )
    ) :
    vec4(
      mix( r.near_total.xyz, l.near_total.xyz, l.near_total.a ),
      1.0 - ( 1.0 - l.near_total.a ) * ( 1.0 - r.near_total.a )
    );
  mixed.far_total =
    ( l.far_depth > r.far_depth ) ?
    vec4(
      mix( l.far_total.xyz, r.far_total.xyz, r.far_total.a ),
      1.0 - ( 1.0 - l.far_total.a ) * ( 1.0 - r.far_total.a )
    ) :
    vec4(
      mix( r.far_total.xyz, l.far_total.xyz, l.far_total.a ),
      1.0 - ( 1.0 - l.far_total.a ) * ( 1.0 - r.far_total.a )
    );*/
  mixed.near_total = 
    vec4(
      mix( l.near_total.xyz, r.near_total.xyz, r.near_total.a ),
      1.0 - ( 1.0 - l.near_total.a ) * ( 1.0 - r.near_total.a )
    );
  mixed.far_total =
    vec4(
      mix( l.far_total.xyz, r.far_total.xyz, r.far_total.a ),
      1.0 - ( 1.0 - l.far_total.a ) * ( 1.0 - r.far_total.a )
    );
    
  mixed.near_depth = min( l.near_depth, r.near_depth );
  mixed.far_depth = min( l.far_depth, r.far_depth );
  return mixed;
}

#endif

