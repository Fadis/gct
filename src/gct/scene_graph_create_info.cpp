#include <nlohmann/json.hpp>
#include <gct/allocator.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/sampler.hpp>
#include <gct/scene_graph_create_info.hpp>

namespace gct::scene_graph {

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
  dest[ "prim_pool_size" ] = src.prim_pool_size;
  dest[ "inst_pool_size" ] = src.inst_pool_size;
  dest[ "descriptor_set_id" ] = src.descriptor_set_id;
  dest[ "texture_descriptor_set_id" ] = src.texture_descriptor_set_id;
  dest[ "vertex_buffer_descriptor_set_id" ] = src.vertex_buffer_descriptor_set_id;
  dest[ "enable_linear" ] = src.enable_linear;
}

}

