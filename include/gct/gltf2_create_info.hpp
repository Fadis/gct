#ifndef GCT_GLTF2_CREATE_INFO_HPP
#define GCT_GLTF2_CREATE_INFO_HPP
#include <unordered_map>
#include <string>
#include <cstdint>
#include <gct/setter.hpp>
#include <gct/matrix_pool.hpp>
#include <gct/aabb_pool.hpp>
#include <gct/texture_pool.hpp>
#include <gct/buffer_pool.hpp>
#include <gct/render_pass.hpp>
#include <gct/scene_graph.hpp>

namespace gct::gltf {

struct gltf2_create_info {
  LIBGCT_SETTER( filename )
  LIBGCT_SETTER( graph )
  LIBGCT_SETTER( root )
  LIBGCT_SETTER( aspect_ratio )
  LIBGCT_SETTER( vertex_attribute_map )
  LIBGCT_SETTER( meshlet_size )
  std::filesystem::path filename;
  std::shared_ptr< scene_graph::scene_graph > graph;
  std::shared_ptr< scene_graph::node > root;
  float aspect_ratio = 1920.f/1080.f;
  std::unordered_map< std::string, std::uint32_t > vertex_attribute_map;
  std::uint32_t meshlet_size = 32u;
};

}

#endif

