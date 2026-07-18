#ifndef GCT_SHADER_RASTERIZE_H
#define GCT_SHADER_RASTERIZE_H

#ifdef __cplusplus
#include <cstdint>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <gct/shader/glsl/gct/aabb_type.h>
#include <gct/shader/glsl/gct/vertex_attribute.h>
using namespace glm;
using namespace std;
using uint = std::uint32_t;
#else
#include <gct/aabb_type.h>
#include <gct/vertex_attribute.h>
#endif

uint rasterization_to_framebuffer_index( int x, int y, int width ) {
	return x + width * y;
}

vec2 rasterization_ndc_to_screen( vec3 ndc, float width, float height ) {
	return vec2(
		( ndc.x * 0.5f + 0.5f ) * width,
		( ndc.y * 0.5f + 0.5f ) * height
	);
}

vec4 rasterization_to_screen_coord( vec3 p, mat4 transform, int width, int height ) {
	vec4 pos = transform * vec4( p.x, p.y, p.z, 1.0f );

	pos.x = pos.x / pos.w;
	pos.y = pos.y / pos.w;

	vec4 imgPos = vec4(
		( pos.x * 0.5f + 0.5f ) * width,
		( pos.y * 0.5f + 0.5f ) * height,
		pos.z,
		pos.w
  );

	return imgPos;
}

bool rasterization_frustum_culling(
  vec3 v0_view,
  vec3 v1_view,
  vec3 v2_view
) {
  if( v0_view.x > +1.0f && v1_view.x > +1.0f && v2_view.x > +1.0f ) return false;
  if( v0_view.x < -1.0f && v1_view.x < -1.0f && v2_view.x < -1.0f ) return false;
  if( v0_view.y > +1.0f && v1_view.y > +1.0f && v2_view.y > +1.0f ) return false;
  if( v0_view.y < -1.0f && v1_view.y < -1.0f && v2_view.y < -1.0f ) return false;
  if( v0_view.z > +1.0f && v1_view.z > +1.0f && v2_view.z > +1.0f ) return false;
  //if( v0_view.z < -1.0f && v1_view.z < -1.0f && v2_view.z < -1.0f ) return false;
  if( v0_view.z < 0.0f && v1_view.z < 0.0f && v2_view.z < 0.0f ) return false;
  return true;
}

bool rasterization_check_non_trivial(
  vec3 v0_view,
  vec3 v1_view,
  vec3 v2_view
) {
  return v0_view.z <= 0.0f || v1_view.z <= 0.0f || v2_view.z <= 0.0f;
}

bool rasterization_check_tiny_bounding_box(
  aabb2_type screen_space_bounding_box
) {
  float sample_x = floor( screen_space_bounding_box.min.x );
  float sample_y = floor( screen_space_bounding_box.min.y );
  return
    (
      screen_space_bounding_box.min.x > sample_x &&
      screen_space_bounding_box.max.x < sample_x + 1.0f
    ) ||
    (
      screen_space_bounding_box.min.y > sample_y &&
      screen_space_bounding_box.max.y < sample_y + 1.0f
    );
}

float rasterization_cross2( vec2 a, vec2 b ){ 
	return a.x * b.y - a.y * b.x; 
}

struct rasterization_state {
  ivec2 left_top_pixel;
  ivec2 bounding_box_size;
  vec2 left_top_pixel_pos_barycentric;

  float inv_v0w;
  float inv_v1w;
  float inv_v2w;

  float ds_dx;
  float ds_dy;
	float dt_dx;
	float dt_dy;
};

rasterization_state rasterization_init(
  vec3 v0_view3,
  vec3 v1_view3,
  vec3 v2_view3,
  uint width,
  uint height
) {
  if( !rasterization_frustum_culling( v0_view3, v1_view3, v2_view3 ) ) {
    return rasterization_state(
      ivec2( 0, 0 ),
      ivec2( 0, 0 ),
      vec2( 0, 0 ),
      0, 0, 0, 0, 0, 0, 0
    );
  }

  vec2 v0_screen = rasterization_ndc_to_screen( v0_view3, width, height );
  vec2 v1_screen = rasterization_ndc_to_screen( v1_view3, width, height );
  vec2 v2_screen = rasterization_ndc_to_screen( v2_view3, width, height );

  v0_screen -= vec2( 0.5f, 0.5f );
	v1_screen -= vec2( 0.5f, 0.5f );
	v2_screen -= vec2( 0.5f, 0.5f );

  aabb2_type screen_space_bounding_box = aabb2_type(
    v0_screen,
    v0_screen
  );
  aabb_append( screen_space_bounding_box, v1_screen );
  aabb_append( screen_space_bounding_box, v2_screen );

  screen_space_bounding_box =
    aabb_and(
      screen_space_bounding_box,
      aabb2_type(
        vec2( 0.0f, 0.0f ),
        vec2( width, height )
      )
    );

  if( rasterization_check_tiny_bounding_box( screen_space_bounding_box ) ) {
    return rasterization_state(
      ivec2( 0, 0 ),
      ivec2( 0, 0 ),
      vec2( 0, 0 ),
      0, 0, 0, 0, 0, 0, 0
    );
  }

  const ivec2 bounding_box_pixel_left_top = ivec2( ceil( screen_space_bounding_box.min ) );
  const ivec2 bounding_box_pixel_count =
    ivec2( ceil( screen_space_bounding_box.max ) ) - bounding_box_pixel_left_top;

  const int pixel_count = bounding_box_pixel_count.x * bounding_box_pixel_count.y;

  const vec2 v_ab = v1_screen - v0_screen;
  const vec2 v_ac = v2_screen - v0_screen;

  const float factor = 1.0f / rasterization_cross2( v_ab, v_ac );

  const float ds_dx =  v_ac.y * factor;
  const float ds_dy = -v_ac.x * factor;
	const float dt_dx = -v_ab.y * factor;
	const float dt_dy =  v_ab.x * factor;

  // bounding box内での左上のピクセルの位置
  const vec2 bounding_box_left_top_pixel_pos = vec2(
    bounding_box_pixel_left_top.x - v0_screen.x,
	  bounding_box_pixel_left_top.y - v0_screen.y
  );

  const vec2 bounding_box_left_top_pixel_pos_barycentric = vec2(
    ( bounding_box_left_top_pixel_pos.x * v_ac.y - bounding_box_left_top_pixel_pos.y * v_ac.x ) * factor,
    ( v_ab.x * bounding_box_left_top_pixel_pos.y - v_ab.y * bounding_box_left_top_pixel_pos.x ) * factor
  );

  return rasterization_state(
    bounding_box_pixel_left_top,
    bounding_box_pixel_count,
    bounding_box_left_top_pixel_pos_barycentric,
    1.0f,
    1.0f,
    1.0f,
    ds_dx,
    ds_dy,
 	  dt_dx,
 	  dt_dy
  );
}


rasterization_state rasterization_init(
  vec4 v0_view,
  vec4 v1_view,
  vec4 v2_view,
  uint width,
  uint height
) {
  const float inv_v0w = 1.0f/v0_view.w;
  const float inv_v1w = 1.0f/v1_view.w;
  const float inv_v2w = 1.0f/v2_view.w;
  const vec3 v0_view3 = vec3( v0_view ) * inv_v0w;
  const vec3 v1_view3 = vec3( v1_view ) * inv_v1w;
  const vec3 v2_view3 = vec3( v2_view ) * inv_v2w;
  if( !rasterization_frustum_culling( v0_view3, v1_view3, v2_view3 ) ) {
    return rasterization_state(
      ivec2( 0, 0 ),
      ivec2( 0, 0 ),
      vec2( 0, 0 ),
      0, 0, 0, 0, 0, 0, 0
    );
  }

  vec2 v0_screen = rasterization_ndc_to_screen( v0_view3, width, height );
  vec2 v1_screen = rasterization_ndc_to_screen( v1_view3, width, height );
  vec2 v2_screen = rasterization_ndc_to_screen( v2_view3, width, height );

  v0_screen -= vec2( 0.5f, 0.5f );
	v1_screen -= vec2( 0.5f, 0.5f );
	v2_screen -= vec2( 0.5f, 0.5f );

  aabb2_type screen_space_bounding_box = aabb2_type(
    v0_screen,
    v0_screen
  );
  aabb_append( screen_space_bounding_box, v1_screen );
  aabb_append( screen_space_bounding_box, v2_screen );

  screen_space_bounding_box =
    aabb_and(
      screen_space_bounding_box,
      aabb2_type(
        vec2( 0.0f, 0.0f ),
        vec2( width, height )
      )
    );

  if( rasterization_check_tiny_bounding_box( screen_space_bounding_box ) ) {
    return rasterization_state(
      ivec2( 0, 0 ),
      ivec2( 0, 0 ),
      vec2( 0, 0 ),
      0, 0, 0, 0, 0, 0, 0
    );
  }

  const ivec2 bounding_box_pixel_left_top = ivec2( ceil( screen_space_bounding_box.min ) );
  const ivec2 bounding_box_pixel_count =
    ivec2( ceil( screen_space_bounding_box.max ) ) - bounding_box_pixel_left_top;

  const int pixel_count = bounding_box_pixel_count.x * bounding_box_pixel_count.y;

  const vec2 v_ab = v1_screen - v0_screen;
  const vec2 v_ac = v2_screen - v0_screen;

  const float factor = 1.0f / rasterization_cross2( v_ab, v_ac );

  const float ds_dx =  v_ac.y * factor;
  const float ds_dy = -v_ac.x * factor;
	const float dt_dx = -v_ab.y * factor;
	const float dt_dy =  v_ab.x * factor;

  // bounding box内での左上のピクセルの位置
  const vec2 bounding_box_left_top_pixel_pos = vec2(
    bounding_box_pixel_left_top.x - v0_screen.x,
	  bounding_box_pixel_left_top.y - v0_screen.y
  );

  const vec2 bounding_box_left_top_pixel_pos_barycentric = vec2(
    ( bounding_box_left_top_pixel_pos.x * v_ac.y - bounding_box_left_top_pixel_pos.y * v_ac.x ) * factor,
    ( v_ab.x * bounding_box_left_top_pixel_pos.y - v_ab.y * bounding_box_left_top_pixel_pos.x ) * factor
  );

  return rasterization_state(
    bounding_box_pixel_left_top,
    bounding_box_pixel_count,
    bounding_box_left_top_pixel_pos_barycentric,
    inv_v0w,
    inv_v1w,
    inv_v2w,
    ds_dx,
    ds_dy,
 	  dt_dx,
 	  dt_dy
  );
}

/*
struct rasterization_state {
  ivec2 left_top_pixel;
  ivec2 bounding_box_size;
  vec2 left_top_pixel_pos_barycentric;

  float inv_v0w;
  float inv_v1w;
  float inv_v2w;

  float ds_dx;
  float ds_dy;
	float dt_dx;
	float dt_dy;
};
 */

struct rasterization_result {
  ivec2 pixel;
  vec2 st;
  float w;
  bool valid;
};

uint rasterization_get_candidate_count(
  rasterization_state state
) {
  return state.bounding_box_size.x * state.bounding_box_size.y;
}

rasterization_result rasterization_rasterize(
  rasterization_state state,
  uint index
) {
  const ivec2 pixel_in_bounding_box = ivec2(
    index % state.bounding_box_size.x,
    index / state.bounding_box_size.x
  );
  if( pixel_in_bounding_box.y >= state.bounding_box_size.y ) {
    return rasterization_result( ivec2( 0, 0 ), vec2( 0, 0 ), 0, false );
  }
  const float s = state.left_top_pixel_pos_barycentric.x +
            pixel_in_bounding_box.x * state.ds_dx +
            pixel_in_bounding_box.y * state.ds_dy;
	const float t = state.left_top_pixel_pos_barycentric.y +
            pixel_in_bounding_box.x * state.dt_dx +
            pixel_in_bounding_box.y * state.dt_dy;

  const float v = 1.0f - ( s + t );
  if( s < 0.0f || t < 0.0f || v < 0.0f ) {
    return rasterization_result( ivec2( 0, 0 ), vec2( 0, 0 ), 0, false );
  }

  const float inv_w = v * state.inv_v0w + s * state.inv_v1w + t * state.inv_v2w;
  float w = 1.0f / inv_w; 

  return rasterization_result(
    state.left_top_pixel + pixel_in_bounding_box,
    vec2( s, t ),
    w,
    true
  );
}

float rasterization_interpolate(
  rasterization_state state,
  rasterization_result rast,
  float v0,
  float v1,
  float v2
) {
  float v_ds = v1 - v0;
  float v_dt = v2 - v0;
  return v0 + rast.st.x * v_ds + rast.st.y * v_dt;
}

float rasterization_perspective_correct_interpolate(
  rasterization_state state,
  rasterization_result rast,
  float v0,
  float v1,
  float v2
) {
  v0 *= state.inv_v0w;
  v1 *= state.inv_v1w;
  v2 *= state.inv_v2w;
  float v_ds = v1 - v0;
  float v_dt = v2 - v0;
  return rast.w * ( v0 + rast.st.x * v_ds + rast.st.y * v_dt );
}

vec2 rasterization_interpolate(
  rasterization_state state,
  rasterization_result rast,
  vec2 v0,
  vec2 v1,
  vec2 v2
) {
  vec2 v_ds = v1 - v0;
  vec2 v_dt = v2 - v0;
  return v0 + rast.st.x * v_ds + rast.st.y * v_dt;
}

vec2 rasterization_perspective_correct_interpolate(
  rasterization_state state,
  rasterization_result rast,
  vec2 v0,
  vec2 v1,
  vec2 v2
) {
  v0 *= state.inv_v0w;
  v1 *= state.inv_v1w;
  v2 *= state.inv_v2w;
  vec2 v_ds = v1 - v0;
  vec2 v_dt = v2 - v0;
  return rast.w * ( v0 + rast.st.x * v_ds + rast.st.y * v_dt );
}

vec3 rasterization_interpolate(
  rasterization_state state,
  rasterization_result rast,
  vec3 v0,
  vec3 v1,
  vec3 v2
) {
  vec3 v_ds = v1 - v0;
  vec3 v_dt = v2 - v0;
  return v0 + rast.st.x * v_ds + rast.st.y * v_dt;
}

vec3 rasterization_perspective_correct_interpolate(
  rasterization_state state,
  rasterization_result rast,
  vec3 v0,
  vec3 v1,
  vec3 v2
) {
  v0 *= state.inv_v0w;
  v1 *= state.inv_v1w;
  v2 *= state.inv_v2w;
  vec3 v_ds = v1 - v0;
  vec3 v_dt = v2 - v0;
  return rast.w * ( v0 + rast.st.x * v_ds + rast.st.y * v_dt );
}

vec4 rasterization_interpolate(
  rasterization_state state,
  rasterization_result rast,
  vec4 v0,
  vec4 v1,
  vec4 v2
) {
  vec4 v_ds = v1 - v0;
  vec4 v_dt = v2 - v0;
  return v0 + rast.st.x * v_ds + rast.st.y * v_dt;
}

vec4 rasterization_perspective_correct_interpolate(
  rasterization_state state,
  rasterization_result rast,
  vec4 v0,
  vec4 v1,
  vec4 v2
) {
  v0 *= state.inv_v0w;
  v1 *= state.inv_v1w;
  v2 *= state.inv_v2w;
  vec4 v_ds = v1 - v0;
  vec4 v_dt = v2 - v0;
  return rast.w * ( v0 + rast.st.x * v_ds + rast.st.y * v_dt );
}

rasterizable_vertex_attribute rasterization_interpolate(
  rasterization_state state,
  rasterization_result rast,
  rasterizable_face_attribute v
) {
  return rasterizable_vertex_attribute(
    rasterization_interpolate( state, rast, v.vertex[ 0 ].position, v.vertex[ 1 ].position, v.vertex[ 2 ].position ),
    rasterization_interpolate( state, rast, v.vertex[ 0 ].normal, v.vertex[ 1 ].normal, v.vertex[ 2 ].normal ),
    rasterization_interpolate( state, rast, v.vertex[ 0 ].tangent, v.vertex[ 1 ].tangent, v.vertex[ 2 ].tangent ),
    rasterization_interpolate( state, rast, v.vertex[ 0 ].texcoord, v.vertex[ 1 ].texcoord, v.vertex[ 2 ].texcoord ),
    rasterization_interpolate( state, rast, v.vertex[ 0 ].optflow, v.vertex[ 1 ].optflow, v.vertex[ 2 ].optflow ),
    v.vertex[ 0 ].id,
    rasterization_interpolate( state, rast, v.vertex[ 0 ].previous_position, v.vertex[ 1 ].previous_position, v.vertex[ 2 ].previous_position )
  );
}

rasterizable_vertex_attribute rasterization_perspective_correct_interpolate(
  rasterization_state state,
  rasterization_result rast,
  rasterizable_face_attribute v
) {
  return rasterizable_vertex_attribute(
    rasterization_perspective_correct_interpolate( state, rast, v.vertex[ 0 ].position, v.vertex[ 1 ].position, v.vertex[ 2 ].position ),
    rasterization_perspective_correct_interpolate( state, rast, v.vertex[ 0 ].normal, v.vertex[ 1 ].normal, v.vertex[ 2 ].normal ),
    rasterization_perspective_correct_interpolate( state, rast, v.vertex[ 0 ].tangent, v.vertex[ 1 ].tangent, v.vertex[ 2 ].tangent ),
    rasterization_perspective_correct_interpolate( state, rast, v.vertex[ 0 ].texcoord, v.vertex[ 1 ].texcoord, v.vertex[ 2 ].texcoord ),
    rasterization_perspective_correct_interpolate( state, rast, v.vertex[ 0 ].optflow, v.vertex[ 1 ].optflow, v.vertex[ 2 ].optflow ),
    v.vertex[ 0 ].id,
    rasterization_perspective_correct_interpolate( state, rast, v.vertex[ 0 ].previous_position, v.vertex[ 1 ].previous_position, v.vertex[ 2 ].previous_position )
  );
}

struct rasterization_statistics {
  uint x;
  uint y;
  uint z;
  uint discarded_triangle_count;
  uint softrast_triangle_count;
  uint hardrast_triangle_count;
};

struct rasterization_task {
  uint info_index;
  uint offset;
};

struct rasterization_info {
  rasterization_state state;
  rasterizable_face_attribute v;
};


#endif

