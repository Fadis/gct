#include <nlohmann/json.hpp>
#include <gct/image_blit_create_info.hpp>

namespace gct {

void to_json( nlohmann::json &dest, const image_blit_create_info &src ) {
  dest = nlohmann::json::object();
  dest[ "input_name" ] = src.input_name;
  dest[ "output_name" ] = src.output_name;
  if( src.input ) {
    dest[ "input" ] = *src.input;
  }
  if( src.output.index() == 0 ) {
    dest[ "output" ] = *std::get< image_pool::image_descriptor >( src.output );
  }
  else if( src.output.index() == 1 ) {
    dest[ "output" ] = std::get< image_allocate_info >( src.output );
  }
  else if( src.output.index() == 2 ) {
    dest[ "output" ] = std::get< dynamic_size_image_allocate_info >( src.output );
  }
  dest[ "independent" ] = src.independent;
}

}

