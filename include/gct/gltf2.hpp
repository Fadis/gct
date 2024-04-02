#ifndef GCT_GLTF2_HPP
#define GCT_GLTF2_HPP

#include <memory>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnarrowing"
#include <fx/gltf.h>
#pragma GCC diagnostic pop
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>
#include <gct/mappable_buffer.hpp>
#include <gct/shader_flag.hpp>
#include <gct/buffer_window.hpp>
#include <gct/gltf2_create_info.hpp>
#include <gct/vertex_buffer_pool.hpp>
#include <gct/image_pool.hpp>
#include <gct/aabb.hpp>
#include <gct/color.hpp>
#include <gct/graphics_pipeline_create_info.hpp>

namespace gct::gltf {

class descriptor_set_layout_t;

struct mesh {
  std::vector< pool< std::shared_ptr< scene_graph::primitive > >::descriptor > prim;
};

class gltf2 {
public:
  /*struct buffer_state {
    LIBGCT_SETTER( filename )
    LIBGCT_SETTER( buffer )
    std::filesystem::path filename;
    std::shared_ptr< mappable_buffer_t > buffer;
  };*/
  gltf2(
    const gltf2_create_info &ci
  );
  const std::vector< image_pool::views > &get_image() const {
    return image;
  }
  const std::vector< texture_pool::views > &get_texture() const {
    return texture;
  }
private:
  void load_buffer(
    const fx::gltf::Document &doc
  );
  /*std::unordered_map< shader_flag_t, std::shared_ptr< shader_module_t > > load_shader(
    const fx::gltf::Document &doc,
    const std::filesystem::path &shader_dir
  );*/
  void load_sampler(
    const fx::gltf::Document &doc
  );
  void load_image(
    const fx::gltf::Document &doc
  );
  void load_texture(
    const fx::gltf::Document &doc
  );
  scene_graph::primitive create_primitive(
    const fx::gltf::Document &doc,
    const fx::gltf::Primitive &primitive
  );
  std::shared_ptr< mesh > create_mesh(
    const fx::gltf::Document &doc,
    const fx::gltf::Mesh &mesh
  );
  void load_mesh(
    const fx::gltf::Document &doc
  );
  void load_camera(
    const fx::gltf::Document &doc
  );
  void load_node(
    const std::shared_ptr< scene_graph::node > &parent,
    const fx::gltf::Document &doc,
    int32_t index
  );
  void load_node(
    const fx::gltf::Document &doc
  );
  graphics_pipeline_create_info_t create_pipeline(
    const fx::gltf::Document &doc,
    const fx::gltf::Primitive &doc_primitive,
    const scene_graph::primitive &primitive_
  );
  gltf2_create_info props;
  std::vector< vertex_buffer_pool::vertex_buffer_descriptor > buffer;
  sampler_pool::sampler_descriptor default_sampler;
  std::vector< sampler_pool::sampler_descriptor > sampler;
  image_pool::views default_image;
  std::vector< image_pool::views > image;
  std::vector< texture_pool::views > texture;
  std::vector< std::shared_ptr< mesh > > mesh_;
  std::vector< matrix_pool::matrix_descriptor > camera;
  std::filesystem::path cd;
};

void to_json( nlohmann::json&, const gltf2& );

}

#endif


