#include <gct/mesh_type.hpp>
#include <nlohmann/json.hpp>

namespace gct {

void to_json( nlohmann::json &dest, const mesh_type &src ) {
  dest = nlohmann::json::object();
  dest[ "accessor" ] = src.accessor;
  dest[ "vertex_count" ] = src.vertex_count;
  dest[ "topology" ] = src.topology;
  dest[ "meshlet" ] = src.meshlet;
  dest[ "unique_vertex_count" ] = src.unique_vertex_count;
  dest[ "particle_offset" ] = src.particle_offset;
  dest[ "distance_constraint_offset" ] = src.distance_constraint_offset;
  dest[ "constraint_offset" ] = src.constraint_offset;
  dest[ "fluid_constraint_offset" ] = src.fluid_constraint_offset;
  dest[ "lambda_offset" ] = src.lambda_offset;
  dest[ "vertex_to_primitive_offset" ] = src.vertex_to_primitive_offset;
  dest[ "stiffness" ] = src.stiffness;
  dest[ "adjacency_offset" ] = src.adjacency_offset;
  dest[ "same_position_offset" ] = src.same_position_offset;
  dest[ "compression_method" ] = src.compression_method;
  dest[ "meshlet_count" ] = src.meshlet_count;
}
void from_json( const nlohmann::json &src, mesh_type &dest ) {
  if( src.is_object() ) {
    if( src.find( "accessor" ) != src.end() ) {
      dest.accessor = src[ "accessor" ];
    }
    if( src.find( "vertex_count" ) != src.end() ) {
      dest.vertex_count = src[ "vertex_count" ];
    }
    if( src.find( "topology" ) != src.end() ) {
      dest.topology = src[ "topology" ];
    }
    if( src.find( "meshlet" ) != src.end() ) {
      dest.meshlet = src[ "meshlet" ];
    }
    if( src.find( "unique_vertex_count" ) != src.end() ) {
      dest.unique_vertex_count = src[ "unique_vertex_count" ];
    }
    if( src.find( "particle_offset" ) != src.end() ) {
      dest.particle_offset = src[ "particle_offset" ];
    }
    if( src.find( "distance_constraint_offset" ) != src.end() ) {
      dest.distance_constraint_offset = src[ "distance_constraint_offset" ];
    }
    if( src.find( "constraint_offset" ) != src.end() ) {
      dest.constraint_offset = src[ "constraint_offset" ];
    }
    if( src.find( "fluid_constraint_offset" ) != src.end() ) {
      dest.fluid_constraint_offset = src[ "fluid_constraint_offset" ];
    }
    if( src.find( "lambda_offset" ) != src.end() ) {
      dest.lambda_offset = src[ "lambda_offset" ];
    }
    if( src.find( "vertex_to_primitive_offset" ) != src.end() ) {
      dest.vertex_to_primitive_offset = src[ "vertex_to_primitive_offset" ];
    }
    if( src.find( "stiffness" ) != src.end() ) {
      dest.stiffness = src[ "stiffness" ];
    }
    if( src.find( "adjacency_offset" ) != src.end() ) {
      dest.adjacency_offset = src[ "adjacency_offset" ];
    }
    if( src.find( "same_position_offset" ) != src.end() ) {
      dest.same_position_offset = src[ "same_position_offset" ];
    }
    if( src.find( "compression_method" ) != src.end() ) {
      dest.compression_method = src[ "compression_method" ];
    }
    if( src.find( "meshlet_count" ) != src.end() ) {
      dest.meshlet_count = src[ "meshlet_count" ];
    }
  }
}

}

