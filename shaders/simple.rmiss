#version 460
#extension GL_EXT_ray_tracing : require

layout(binding = 2) uniform accelerationStructureEXT as;
layout(location = 0) rayPayloadInEXT vec3 payload;

void main() {
    payload = vec3(0.513f, 0.81f, 1.0f);
}

