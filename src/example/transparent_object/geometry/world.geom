#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (triangles) in;
layout (triangle_strip,max_vertices=3) out;

in gl_PerVertex
{
    vec4 gl_Position;
} gl_in[];


out gl_PerVertex
{
    vec4 gl_Position;
};

layout (location = 0) in vec4 input_position[];
layout (location = 1) in vec3 input_normal[];
layout (location = 3) in vec2 input_tex_coord[];

layout (location = 0) out vec4 output_position;
layout (location = 1) out vec3 output_normal;
layout (location = 3) out vec2 output_tex_coord;
layout (location = 4) out vec3 output_face_normal;

void main() {
  gl_PrimitiveID = gl_PrimitiveIDIn;
  for( int i = 0; i < gl_in.length(); i++ ) {
    gl_Position = gl_in[ i ].gl_Position;
    output_position = input_position[ i ];
    output_normal = input_normal[ i ];
    output_face_normal = normalize( cross( input_position[ 2 ].xyz - input_position[ 0 ].xyz, input_position[ 1 ].xyz - input_position[ 0 ].xyz ) );
    output_tex_coord = input_tex_coord[ i ];
    EmitVertex();
  }
  EndPrimitive();
}

