#include <nlohmann/json.hpp>
#include <gct/image_fill_create_info.hpp>

namespace gct {

void to_json( nlohmann::json &dest, const image_fill_create_info &src ) {
  dest = nlohmann::json::object();
  dest[ "name" ] = src.name;
  if( src.output.index() == 0 ) {
    dest[ "output" ] = *std::get< image_pool::image_descriptor >( src.output );
  }
  else if( src.output.index() == 1 ) {
    dest[ "output" ] = std::get< image_allocate_info >( src.output );
  }
  dest[ "color" ] = src.color;
  dest[ "independent" ] = src.independent;
}

}

