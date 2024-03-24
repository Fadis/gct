#ifndef GCT_GLTF2_CREATE_INFO_HPP
#define GCT_GLTF2_CREATE_INFO_HPP

#include <gct/setter.hpp>
#include <gct/matrix_pool.hpp>
#include <gct/aabb_pool.hpp>
#include <gct/texture_pool.hpp>
#include <gct/buffer_pool.hpp>
#include <gct/render_pass.hpp>
#include <gct/scene_graph.hpp>

namespace gct {

struct gltf2_create_info {
  LIBGCT_SETTER( filename )
  LIBGCT_SETTER( graph )
  LIBGCT_SETTER( root )
  std::filesystem::path filename;
  std::shared_ptr< scene_graph > graph;
  std::shared_ptr< node > root;
};

}

#endif

