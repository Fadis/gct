#ifndef GCT_IMAGE_IO_CREATE_INFO_HPP
#define GCT_IMAGE_IO_CREATE_INFO_HPP

#include <memory>
#include <optional>
#include <unordered_set>
#include <unordered_map>
#include <nlohmann/json_fwd.hpp>
#include <glm/mat4x4.hpp>
#include <gct/setter.hpp>
#include <gct/image_pool.hpp>
#include <gct/image_allocate_info.hpp>

namespace gct {

class compute;
class image_view_t;

namespace scene_graph {
  class scene_graph_resource;
}

struct image_io_dimension {
  LIBGCT_SETTER( relative_to )
  LIBGCT_SETTER( size_transform )
  std::optional< std::string > relative_to;
  glm::mat4x4 size_transform = glm::mat4x4(
    1.f, 0.f, 0.f, 0.f,
    0.f, 1.f, 0.f, 0.f,
    0.f, 0.f, 1.f, 0.f,
    0.f, 0.f, 0.f, 1.f
  );
};

void to_json( nlohmann::json&, const image_io_dimension& );

struct image_io_plan {
  LIBGCT_SETTER( input )
  LIBGCT_SETTER( output )
  LIBGCT_SETTER( inout )
  LIBGCT_SETTER( dim )
  image_io_plan &add_input(
    const std::string &name
  ) {
    input.insert( name );
    return *this;
  }
  image_io_plan &add_output(
    const std::string &name,
    const image_allocate_info &desc
  ) {
    output.insert( std::make_pair( name, desc ) );
    return *this;
  }
  image_io_plan &add_output(
    const std::string &name,
    const image_pool::image_descriptor &desc
  ) {
    output.insert( std::make_pair( name, desc ) );
    return *this;
  }
  image_io_plan &add_inout(
    const std::string &name
  ) {
    inout.insert( name );
    return *this;
  }
  std::unordered_set< std::string > input;
  std::unordered_map< std::string, std::variant< image_pool::image_descriptor, image_allocate_info > > output;
  std::unordered_set< std::string > inout;
  image_io_dimension dim;
};

void to_json( nlohmann::json&, const image_io_plan& );

struct image_io_create_info {
  image_io_create_info(
    const std::shared_ptr< compute >&,
    const std::shared_ptr< scene_graph::scene_graph_resource >&,
    const image_io_plan&
  );
  image_io_create_info &add_input(
    const std::string &name,
    const image_pool::image_descriptor &desc
  );
  image_io_create_info &add_output(
    const std::string &name,
    const image_pool::image_descriptor &desc
  );
  image_io_create_info &add_inout(
    const std::string &name,
    const image_pool::image_descriptor &desc
  );
  [[nodiscard]] const std::shared_ptr< compute > &get_executable() const {
    return executable;
  }
  [[nodiscard]] const image_io_plan &get_plan() const {
    return plan;
  }
  [[nodiscard]] const std::unordered_map< std::string, image_pool::image_descriptor > get_input() const {
    return input;
  }
  /*[[nodiscard]] const std::unordered_map< std::string, std:: image_pool::image_descriptor > get_output() const {
    return output;
  }*/
  [[nodiscard]] const std::unordered_map< std::string, image_pool::image_descriptor > get_inout() const {
    return inout;
  }
  [[nodiscard]] const glm::ivec3 &get_dim() const {
    return dim;
  }
  bool filled() const;
  [[nodiscard]] const std::vector< std::uint8_t > &get_push_constant() const {
    return push_constant;
  }
private:
  void update_size(
    const std::string &name,
    const image_pool::image_descriptor &desc
  );
  void update_pc(
    const std::string &name,
    const image_pool::image_descriptor &desc
  );
  std::shared_ptr< compute > executable;
  std::shared_ptr< scene_graph::scene_graph_resource > resource;
  image_io_plan plan;
  std::unordered_map< std::string, image_pool::image_descriptor > input;
  std::unordered_map< std::string, image_pool::image_descriptor > inout;
  glm::ivec3 dim = glm::ivec3( 1, 1, 1 );
  std::vector< std::uint8_t > push_constant;
};

void to_json( nlohmann::json&, const image_io_create_info& );

}

#endif

