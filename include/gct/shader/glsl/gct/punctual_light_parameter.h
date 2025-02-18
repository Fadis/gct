#ifndef GCT_SHADER_PUNCTUAL_LIGHT_PARAMETER_H
#define GCT_SHADER_PUNCTUAL_LIGHT_PARAMETER_H

struct punctual_light_parameter {
  vec4 local_position;
  vec4 world_position;
  vec4 local_direction;
  vec4 world_direction;
  vec4 energy;
  uint type;
  float light_size;
  float theta;
  float phi;
  float falloff;
  uint matrix;
  int shadow_map;
  uint reserved;
};

#endif

