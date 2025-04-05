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

namespace shader_graph {

enum class command_id {
  call = 0,
  barrier = 1,
  fill = 2,
  blit = 3
};

using command = std::variant<
  std::shared_ptr< image_io >,
  barrier,
  std::shared_ptr< image_fill_create_info >,
  std::shared_ptr< image_blit_create_info >
>;

void to_json( nlohmann::json &dest, const command& );
std::string to_string( const command& );

using command_list = std::vector<
  command
>;

void to_json( nlohmann::json &dest, const command_list& );
std::string to_string( const command_list& );

}

}

#endif

