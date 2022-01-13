#version 460
#extension GL_EXT_ray_tracing : require

layout(binding = 2) uniform accelerationStructureEXT as;
layout(location = 0) rayPayloadInEXT vec3 payload;
hitAttributeEXT vec2 HitAttribs;

void main() {
  const vec3 barycentrics = vec3(1.0f - HitAttribs.x - HitAttribs.y, HitAttribs.x, HitAttribs.y);
  payload = vec3(barycentrics);
}

