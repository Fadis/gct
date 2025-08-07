
mat4 get_rigid_local_to_world_matrix( uint rigid_id ) {
  return mat4( 0.0 );
}

void update_rigid_particle_position(
  uint rigid_id,
  uint particle_offset,
  uint unique_vertex_count
) {
  const mat4 l2w = get_rigid_local_to_world_matrix( rigid_id );

  for( uint vertex_id = gl_SubgroupInvocationID; vertex_id < unique_vertex_count; vertex_id += gl_SubgroupSize ) {
    particle_pool[ particle_offset + vertex_id ].position =
      l2w * particle_pool[ particle_offset + vertex_id ].local_r;
  }
}

