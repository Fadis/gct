#ifndef GCT_CUBEMAP_MATRIX_GENERATOR2_CREATE_INFO_HPP
#define GCT_CUBEMAP_MATRIX_GENERATOR2_CREATE_INFO_HPP

#include <vector>
#include <filesystem>
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>
#include <gct/named_resource.hpp>
#include <gct/allocator_set.hpp>
#include <gct/shader_set.hpp>
#include <gct/matrix_pool.hpp>

namespace gct {

namespace scene_graph {
  class scene_graph_resource;
}

struct cubemap_matrix_generator2_create_info {
  LIBGCT_SETTER( allocator_set )
  LIBGCT_ALLOCATOR_SET_LEGACY_SETTER( allocator_set )
  LIBGCT_SETTER( shader_set )
  LIBGCT_SHADER_SET( shader_set )
  LIBGCT_SETTER( resources )
  LIBGCT_NAMED_RESOURCE_SETTER( resources )
  LIBGCT_SETTER( scene_graph )
  LIBGCT_SETTER( node_name )
  cubemap_matrix_generator2_create_info &set_shader(
    const std::filesystem::path &
  );
  bool shader_exists() const;
  allocator_set_t allocator_set;
  shader_set_t shader_set;
  std::vector< named_resource > resources;
  std::shared_ptr< scene_graph::scene_graph_resource > scene_graph;
  std::string node_name;
};

void to_json( nlohmann::json&, const cubemap_matrix_generator2_create_info& );

}

#endif



