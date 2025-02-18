#ifndef GCT_IMAGE_IO_HPP
#define GCT_IMAGE_IO_HPP

#include <memory>
#include <nlohmann/json_fwd.hpp>
#include <gct/property.hpp>
#include <gct/image_io_create_info.hpp>

namespace gct {

class command_buffer_recorder_t;

namespace scene_graph {
  class scene_graph_resource;
}

class image_io :
  public property< image_io_create_info >,
  public std::enable_shared_from_this< compute > {
public:
  image_io(
    image_io_create_info &ci
  );
  void operator()(
    command_buffer_recorder_t &rec
  ) const;
};
void to_json( nlohmann::json&, const image_io& );

}

#endif


