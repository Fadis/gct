#ifndef GCT_SHADER_GRAPH_VERTEX_COMMAND_HPP
#define GCT_SHADER_GRAPH_VERTEX_COMMAND_HPP

#include <memory>
#include <variant>
#include <nlohmann/json_fwd.hpp>
#include <gct/image_pool.hpp>

namespace gct {

enum class shader_graph_vertex_command_id {
  call = 0,
  fill = 1,
  blit = 2,
  immediate = 3
};

class image_io_create_info;
class image_fill_create_info;
class image_blit_create_info;

using shader_graph_vertex_command = std::variant<
  std::shared_ptr< image_io_create_info >,
  std::shared_ptr< image_fill_create_info >,
  std::shared_ptr< image_blit_create_info >,
  image_pool::image_descriptor
>;

void to_json( nlohmann::json &dest, const shader_graph_vertex_command &src );

}

#endif

