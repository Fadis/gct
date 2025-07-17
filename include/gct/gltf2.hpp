#ifndef GCT_GLTF2_HPP
#define GCT_GLTF2_HPP

#include <memory>
#include <unordered_map>
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
  LIBGCT_SETTER( prim )
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
  [[nodiscard]] const std::vector< image_pool::views > &get_image() const {
    return image;
  }
  [[nodiscard]] const std::vector< texture_pool::views > &get_texture() const {
    return texture;
  }
  [[nodiscard]] vertex_buffer_pool::vertex_buffer_descriptor
  get_morph_vertex_buffer() const {
    return morph_vertex_buffer;
  }
  [[nodiscard]] std::pair< std::uint32_t, std::uint32_t >
  get_morph_vertex_buffer_offset( const pool< std::shared_ptr< scene_graph::primitive > >::descriptor &prim ) const {
    const auto match = doc_primitive_id.find( prim );
    if( match == doc_primitive_id.end() ) {
      return std::make_pair( 0u, 0u );
    }
    const auto count = morph_vertex_count.find( match->second );
    if( count == morph_vertex_count.end() ) {
      return std::make_pair( 0u, 0u );
    }
    return std::make_pair( count->second.first, count->second.second );
  }
  [[nodiscard]] const std::vector< pool< std::shared_ptr< scene_graph::instance > >::descriptor >&
  get_descriptor() const {
    return inst;
  }
  [[nodiscard]] const std::vector< pool< std::shared_ptr< scene_graph::primitive > >::descriptor >&
  get_primitive_descriptor() const {
    return prim_;
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
  [[nodiscard]] std::pair< scene_graph::primitive, nlohmann::json > create_primitive(
    const fx::gltf::Document &doc,
    const fx::gltf::Primitive &primitive,
    std::uint32_t mesh_id,
    std::uint32_t prim_id
  );
  [[nodiscard]] static const std::string get_attribute_name_in_mesh( const std::string &gltf_name );
  [[nodiscard]] std::shared_ptr< mesh > create_mesh(
    const fx::gltf::Document &doc,
    const fx::gltf::Mesh &mesh,
    std::uint32_t mesh_id
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
  [[nodiscard]] graphics_pipeline_create_info_t create_pipeline(
    const fx::gltf::Document &doc,
    const fx::gltf::Primitive &doc_primitive,
    const scene_graph::primitive &primitive_
  );
  void get_lod_morph_vertex_count(
    const fx::gltf::Document &doc
  );
  gltf2_create_info props;
  std::vector< vertex_buffer_pool::vertex_buffer_descriptor > buffer;
  vertex_buffer_pool::vertex_buffer_descriptor morph_vertex_buffer;
  sampler_pool::sampler_descriptor default_sampler;
  std::vector< sampler_pool::sampler_descriptor > sampler;
  image_pool::views default_image;
  std::vector< image_pool::views > image;
  std::vector< texture_pool::views > texture;
  std::vector< std::shared_ptr< mesh > > mesh_;
  std::vector< matrix_pool::matrix_descriptor > camera;
  std::filesystem::path cd;
  std::uint32_t accessor_count = 0u;
  std::unordered_map<
    pool< std::shared_ptr< scene_graph::primitive > >::descriptor,
    nlohmann::json
  > primitive_ext;
  std::unordered_map< std::uint64_t, std::pair< std::uint32_t, std::uint32_t > > morph_vertex_count;
  std::unordered_map< pool< std::shared_ptr< scene_graph::primitive > >::descriptor, std::uint64_t > doc_primitive_id;
  std::uint32_t total_morph_vertex_count = 0u;
  std::vector< pool< std::shared_ptr< scene_graph::instance > >::descriptor > inst;
  std::vector< pool< std::shared_ptr< scene_graph::primitive > >::descriptor > prim_;
};

void to_json( nlohmann::json&, const gltf2& );

}

#endif


