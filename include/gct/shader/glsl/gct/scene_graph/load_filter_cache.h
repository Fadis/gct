#ifndef GCT_SHADER_SCENE_GRAPH_LOAD_FILTER_CACHE_H
#define GCT_SHADER_SCENE_GRAPH_LOAD_FILTER_CACHE_H

#include <gct/scene_graph/image_pool.h>

struct filter_cache_value {
  vec4 low;
  vec4 high;
};

filter_cache_value load_x_filter_cache( uint image_id, ivec3 screen_pos, uint work_group_size, int kernel_size, uint lcpos ) {
  const ivec2 image_size = imageSize( image_pool32f[ nonuniformEXT( image_id ) ] ).xy;
  const uint hcpos = lcpos + work_group_size;
  ivec2 lsample_pos = screen_pos.xy;
  lsample_pos.x = lsample_pos.x - kernel_size;
  ivec2 hsample_pos = lsample_pos;
  hsample_pos.x += int( work_group_size );
  lsample_pos.x = max( min( lsample_pos.x, image_size.x ), 0 );
  lsample_pos.y = max( min( lsample_pos.y, image_size.y ), 0 );
  hsample_pos.x = max( min( hsample_pos.x, image_size.x ), 0 );
  hsample_pos.y = max( min( hsample_pos.y, image_size.y ), 0 );
  filter_cache_value temp;
  temp.low = imageLoad( image_pool32f_array[ nonuniformEXT( image_id ) ], ivec3( lsample_pos, screen_pos.z ) );
  temp.high = imageLoad( image_pool32f_array[ nonuniformEXT( image_id ) ], ivec3( hsample_pos, screen_pos.z ) );
  return temp;
}

filter_cache_value load_y_filter_cache( uint image_id, ivec3 screen_pos, uint work_group_size, int kernel_size, uint lcpos ) {
  const ivec2 image_size = imageSize( image_pool32f[ nonuniformEXT( image_id ) ] ).xy;
  const uint hcpos = lcpos + work_group_size;
  ivec2 lsample_pos = screen_pos.xy;
  lsample_pos.y = lsample_pos.y - kernel_size;
  ivec2 hsample_pos = lsample_pos;
  hsample_pos.y += int( work_group_size );
  lsample_pos.x = max( min( lsample_pos.x, image_size.x ), 0 );
  lsample_pos.y = max( min( lsample_pos.y, image_size.y ), 0 );
  hsample_pos.x = max( min( hsample_pos.x, image_size.x ), 0 );
  hsample_pos.y = max( min( hsample_pos.y, image_size.y ), 0 );
  filter_cache_value temp;
  temp.low = imageLoad( image_pool32f_array[ nonuniformEXT( image_id ) ], ivec3( lsample_pos, screen_pos.z ) );
  temp.high = imageLoad( image_pool32f_array[ nonuniformEXT( image_id ) ], ivec3( hsample_pos, screen_pos.z ) );
  return temp;
}

filter_cache_value load_x_filter_cache( uint image_id, ivec2 screen_pos, uint work_group_size, int kernel_size, uint lcpos ) {
  const ivec2 image_size = imageSize( image_pool32f[ nonuniformEXT( image_id ) ] ).xy;
  const uint hcpos = lcpos + work_group_size;
  ivec2 lsample_pos = screen_pos.xy;
  lsample_pos.x = lsample_pos.x - kernel_size;
  ivec2 hsample_pos = lsample_pos;
  hsample_pos.x += int( work_group_size );
  lsample_pos.x = max( min( lsample_pos.x, image_size.x ), 0 );
  lsample_pos.y = max( min( lsample_pos.y, image_size.y ), 0 );
  hsample_pos.x = max( min( hsample_pos.x, image_size.x ), 0 );
  hsample_pos.y = max( min( hsample_pos.y, image_size.y ), 0 );
  filter_cache_value temp;
  temp.low = imageLoad( image_pool32f[ nonuniformEXT( image_id ) ], lsample_pos );
  temp.high = imageLoad( image_pool32f[ nonuniformEXT( image_id ) ], hsample_pos );
  return temp;
}

filter_cache_value load_y_filter_cache( uint image_id, ivec2 screen_pos, uint work_group_size, int kernel_size, uint lcpos ) {
  const ivec2 image_size = imageSize( image_pool32f[ nonuniformEXT( image_id ) ] ).xy;
  const uint hcpos = lcpos + work_group_size;
  ivec2 lsample_pos = screen_pos.xy;
  lsample_pos.y = lsample_pos.y - kernel_size;
  ivec2 hsample_pos = lsample_pos;
  hsample_pos.y += int( work_group_size );
  lsample_pos.x = max( min( lsample_pos.x, image_size.x ), 0 );
  lsample_pos.y = max( min( lsample_pos.y, image_size.y ), 0 );
  hsample_pos.x = max( min( hsample_pos.x, image_size.x ), 0 );
  hsample_pos.y = max( min( hsample_pos.y, image_size.y ), 0 );
  filter_cache_value temp;
  temp.low = imageLoad( image_pool32f[ nonuniformEXT( image_id ) ], lsample_pos );
  temp.high = imageLoad( image_pool32f[ nonuniformEXT( image_id ) ], hsample_pos );
  return temp;
}

#endif

