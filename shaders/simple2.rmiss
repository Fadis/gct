#version 460
#extension GL_EXT_ray_tracing : require

layout(binding = 2) uniform accelerationStructureEXT as;
layout(location = 0) rayPayloadInEXT vec3 payload;

void main() {
    payload = vec3(0.412f, 0.796f, 0.0f);
}

