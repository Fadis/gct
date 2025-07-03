#include <nlohmann/json.hpp>
#include <gct/allocator.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/sampler.hpp>
#include <gct/scene_graph_create_info.hpp>

namespace gct::scene_graph {

scene_graph_create_info::scene_graph_create_info() {
  primitive_resource_index.set_buffer_name( "primitive_resource_index" );
  instance_resource_index.set_buffer_name( "instance_resource_index" );
  visibility.set_buffer_name( "visibility" );  
  accessor.set_buffer_name( "accessor_pool" );  
  mesh.set_buffer_name( "mesh_pool" );  
  meshlet.set_buffer_name( "meshlet_pool" );  
  meshlet.set_max_buffer_count( 1024u * 1024u * 64u );
  meshlet_index.set_buffer_name( "meshlet_index_pool" );  
  meshlet_index.set_max_buffer_count( 1024u * 1024u * 64u );
  resource_pair.set_buffer_name( "resource_pair" );
  particle.set_buffer_name( "particle_pool" );
  rigid.set_buffer_name( "rigid_pool" );
  distance_constraint.set_buffer_name( "distance_constraint_pool" );
  constraint.set_buffer_name( "constraint_pool" );
  spatial_hash.set_buffer_name( "spatial_hash_pool" );
  vertex_to_primitive.set_buffer_name( "vertex_to_primitive_pool" );
  const std::uint32_t max_particle_count = 2097152u;
  particle.set_max_buffer_count( max_particle_count );
  particle.set_max_request_count( 64u );
  distance_constraint.set_max_buffer_count( max_particle_count * 32u );
  distance_constraint.set_max_request_count( 64u );
  constraint.set_max_buffer_count( max_particle_count * 32u );
  constraint.set_max_request_count( 64u );
  spatial_hash.set_max_buffer_count( max_particle_count * 4u );
  spatial_hash.set_max_request_count( 64u );
  vertex_to_primitive.set_max_buffer_count( max_particle_count * 32u );
  vertex_to_primitive.set_max_request_count( 64u );
}

scene_graph_create_info &scene_graph_create_info::set_shader( const std::filesystem::path &dir ) {
  matrix.set_shader( dir / "matrix_pool" );
  aabb.set_shader( dir / "aabb_pool" );
  image.set_shader( dir / "image_pool" );
  primitive_resource_index.set_shader( dir / "primitive_resource_index_pool" );
  instance_resource_index.set_shader( dir / "instance_resource_index_pool" );
  visibility.set_shader( dir / "visibility_pool" );
  accessor.set_shader( dir / "accessor" );
  mesh.set_shader( dir / "mesh" );
  meshlet.set_shader( dir / "meshlet" );
  meshlet_index.set_shader( dir / "meshlet_index" );
  resource_pair.set_shader( dir / "resource_pair" );
  light.set_shader( dir / "light_pool" );
  particle.set_shader( dir / "particle_pool" );
  rigid.set_shader( dir / "rigid_pool" );
  distance_constraint.set_shader( dir / "distance_constraint_pool" );
  constraint.set_shader( dir / "constraint_pool" );
  spatial_hash.set_shader( dir / "spatial_hash_pool" );
  vertex_to_primitive.set_shader( dir / "vertex_to_primitive_pool" );
  return *this;
}


void to_json( nlohmann::json &dest, const scene_graph_create_info &src ) {
  dest = nlohmann::json::object();
  dest[ "allocator_set" ] = src.allocator_set;
  dest[ "master_shader" ] = nlohmann::json::array();
  for( const auto &v: src.master_shader ) {
    dest[ "master_shader" ].push_back( v );
  }
  dest[ "matrix" ] = src.matrix;
  dest[ "aabb" ] = src.aabb;
  dest[ "image" ] = src.image;
  dest[ "texture" ] = src.texture;
  dest[ "sampler" ] = src.sampler;
  dest[ "primitive_resource_index" ] = src.primitive_resource_index;
  dest[ "visibility" ] = src.visibility;
  dest[ "accessor" ] = src.accessor;
  dest[ "mesh" ] = src.mesh;
  dest[ "meshlet" ] = src.meshlet;
  dest[ "meshlet_index" ] = src.meshlet_index;
  dest[ "resource_pair" ] = src.resource_pair;
  dest[ "vertex" ] = src.vertex;
  dest[ "light" ] = src.light;
  dest[ "particle" ] = src.particle;
  dest[ "rigid" ] = src.rigid;
  dest[ "distance_constraint" ] = src.distance_constraint;
  dest[ "constraint" ] = src.constraint;
  dest[ "spatial_hash" ] = src.spatial_hash;
  dest[ "vertex_to_primitive" ] = src.vertex_to_primitive;
  dest[ "prim_pool_size" ] = src.prim_pool_size;
  dest[ "inst_pool_size" ] = src.inst_pool_size;
  dest[ "descriptor_set_id" ] = src.descriptor_set_id;
  dest[ "texture_descriptor_set_id" ] = src.texture_descriptor_set_id;
  dest[ "vertex_buffer_descriptor_set_id" ] = src.vertex_buffer_descriptor_set_id;
  dest[ "enable_linear" ] = src.enable_linear;
}

}

