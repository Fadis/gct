#ifndef GCT_SHADER_DEPTH_H
#define GCT_SHADER_DEPTH_H

float decode_depth(float d,float zNear,float zFar)
{
  return zNear * zFar / (zFar + d * (zNear - zFar));
}

float encode_depth(float d,float zNear,float zFar)
{
  return zFar * ( zNear - d )/( d * ( zNear - zFar ) );
}

#endif

