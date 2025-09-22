#ifndef GCT_GRAPHICS_EXECUTION_SHAPE_HPP
#define GCT_GRAPHICS_EXECUTION_SHAPE_HPP

#include <memory>
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>

namespace gct {

namespace scene_graph {

class instance_list;

}

struct graphics_execution_shape {
  LIBGCT_SETTER( list )
  LIBGCT_SETTER( offset )
  LIBGCT_SETTER( count )
  LIBGCT_SETTER( x )
  LIBGCT_SETTER( y )
  LIBGCT_SETTER( z )
  std::shared_ptr< scene_graph::instance_list > list;
  std::uint32_t offset = 0u;
  std::uint32_t count = 0u;
  std::uint32_t x = 0u;
  std::uint32_t y = 0u;
  std::uint32_t z = 0u;
};

void to_json( nlohmann::json&, const graphics_execution_shape& );
void from_json( const nlohmann::json&, graphics_execution_shape& );

}

#endif

