#ifndef GCT_SCENE_GRAPH_ACCESSOR_HPP
#define GCT_SCENE_GRAPH_ACCESSOR_HPP
#include <cstdint>
#include <gct/setter.hpp>
#include <nlohmann/json_fwd.hpp>

namespace gct::scene_graph {

struct accessor {
  LIBGCT_SETTER( vertex_buffer_id )
  LIBGCT_SETTER( type )
  LIBGCT_SETTER( normalized )
  LIBGCT_SETTER( length )
  LIBGCT_SETTER( offset )
  LIBGCT_SETTER( stride )
  std::uint32_t vertex_buffer_id;
  std::uint32_t type;
  std::uint32_t normalized;
  std::uint32_t length;
  std::uint32_t offset;
  std::uint32_t stride;
  std::uint32_t reserved[ 2 ];
};

void to_json( nlohmann::json &dest, const accessor_type &src );

}

#endif

