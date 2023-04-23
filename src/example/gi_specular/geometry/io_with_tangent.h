#ifndef GCT_SHADER_IO_H
#define GCT_SHADER_IO_H

layout (location = 0) in vec4 input_position;
layout (location = 1) in vec3 input_normal;
layout (location = 2) in vec3 input_tangent;
layout (location = 3) in vec2 input_texcoord;
layout (location = 4) in vec3 input_face_normal;
layout (location = 0) out vec4 output_position;
layout (location = 1) out vec4 output_albedo; // [albedo.r] [albedo.g] [albedo.b] [albedo.a]
layout (location = 2) out vec4 output_normal; // [normal.x] [normal.y] [tangent.x] [tangent.y]
layout (location = 3) out vec4 output_emissive; // [emissive.r] [emissive.g] [emissive.b]
layout (location = 4) out vec4 output_material; // [metallicness] [roughness] [inst_id] [prim_id]
layout (location = 5) out vec4 output_optflow; // [optflow.x] [optflow.y] [optflow.z]
layout (location = 6) out vec4 output_shadow; // [shadow0] [shadow1] [shadow2] [shadow3]
layout (location = 7) out vec4 output_face_normal;

#endif

