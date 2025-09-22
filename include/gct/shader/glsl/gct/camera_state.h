#ifndef GCT_SHADER_CAMERA_STATE_H
#define GCT_SHADER_CAMERA_STATE_H

struct auto_focus_state {
  uint history;
  uint max_history;
  float depth;
  float znear;
  float zfar;
  float focal_distance;
  float lens_size;
  float sensor_size;
  float apeture_rotation;
  float visible_range;
  uint reserved[ 2 ];
};

struct tone_mapping_state {
  uint value;
  float scale;
};

struct camera_state {
  vec4 position;
  vec4 direction;
  uint camera_matrix;
  uint projection_matrix;
  uint visibility;
  uint reserved;
};

#endif

