#ifndef GCT_SHADER_GRAPH_COMMAND_HPP
#define GCT_SHADER_GRAPH_COMMAND_HPP

#include <memory>
#include <vector>
#include <string>
#include <variant>
#include <nlohmann/json_fwd.hpp>
#include <gct/shader_graph_barrier.hpp>

namespace gct {

class image_io;
class image_fill_create_info;
class image_blit_create_info;

enum class shader_graph_command_id {
  call = 0,
  barrier = 1,
  fill = 2,
  blit = 3
};

using shader_graph_command = std::variant<
  std::shared_ptr< image_io >,
  shader_graph_barrier,
  std::shared_ptr< image_fill_create_info >,
  std::shared_ptr< image_blit_create_info >
>;

void to_json( nlohmann::json &dest, const shader_graph_command& );
std::string to_string( const shader_graph_command& );

using shader_graph_command_list = std::vector<
  shader_graph_command
>;

void to_json( nlohmann::json &dest, const shader_graph_command_list& );
std::string to_string( const shader_graph_command_list& );

}

#endif

