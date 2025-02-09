#ifndef GCT_FILTER_CACHE
#define GCT_FILTER_CACHE

ivec4 horizontal_filter_cache_pos(
  ivec2 screen_pos,
  ivec2 image_size,
  uint workgroup_size,
  int kernel_size
) {
  ivec2 lsample_pos = screen_pos;
  lsample_pos.x = lsample_pos.x - kernel_size;
  ivec2 hsample_pos = lsample_pos;
  hsample_pos.x += int( workgroup_size );
  lsample_pos.x = max( min( lsample_pos.x, image_size.x ), 0 );
  lsample_pos.y = max( min( lsample_pos.y, image_size.y ), 0 );
  hsample_pos.x = max( min( hsample_pos.x, image_size.x ), 0 );
  hsample_pos.y = max( min( hsample_pos.y, image_size.y ), 0 );
  return ivec4( lsample_pos.x, lsample_pos.y, hsample_pos.x, hsample_pos.y );
}

ivec4 vertical_filter_cache_pos(
  ivec2 screen_pos,
  ivec2 image_size,
  uint workgroup_size,
  int kernel_size
) {
  ivec2 lsample_pos = screen_pos;
  lsample_pos.y = lsample_pos.y - kernel_size;
  ivec2 hsample_pos = lsample_pos;
  hsample_pos.y += int( workgroup_size );
  lsample_pos.x = max( min( lsample_pos.x, image_size.x ), 0 );
  lsample_pos.y = max( min( lsample_pos.y, image_size.y ), 0 );
  hsample_pos.x = max( min( hsample_pos.x, image_size.x ), 0 );
  hsample_pos.y = max( min( hsample_pos.y, image_size.y ), 0 );
  return ivec4( lsample_pos.x, lsample_pos.y, hsample_pos.x, hsample_pos.y );
}

#endif

