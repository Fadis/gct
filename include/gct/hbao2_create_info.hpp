#ifndef GCT_HBAO2_CREATE_INFO_HPP
#define GCT_HBAO2_CREATE_INFO_HPP

#include <vector>
#include <string>
#include <unordered_map>
#include <filesystem>
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>
#include <gct/named_resource.hpp>
#include <gct/allocator_set.hpp>
#include <gct/shader_set.hpp>

namespace gct {

namespace scene_graph {
  class scene_graph_resource;
}

struct hbao2_create_info {
  LIBGCT_SETTER( allocator_set )
  LIBGCT_ALLOCATOR_SET_LEGACY_SETTER( allocator_set )
  LIBGCT_SETTER( shader )
  LIBGCT_SHADER_SET( shader )
  LIBGCT_SETTER( input_name )
  LIBGCT_SETTER( partial0_name )
  LIBGCT_SETTER( partial45_name )
  LIBGCT_SETTER( partial90_name )
  LIBGCT_SETTER( partial135_name )
  LIBGCT_SETTER( output_name )
  LIBGCT_SETTER( resources )
  LIBGCT_NAMED_RESOURCE_SETTER( resources )
  LIBGCT_SETTER( scene_graph )
  hbao2_create_info &set_shader(
    const std::filesystem::path &
  );
  allocator_set_t allocator_set;
  shader_set_t shader;
  std::string input_name = "position";
  std::string partial0_name = "partial0";
  std::string partial45_name = "partial45";
  std::string partial90_name = "partial90";
  std::string partial135_name = "partial135";
  std::string output_name = "output";
  std::vector< named_resource > resources;
  std::shared_ptr< scene_graph::scene_graph_resource > scene_graph;
};

void to_json( nlohmann::json&, const hbao2_create_info& );

}

#endif


