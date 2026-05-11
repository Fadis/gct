#ifndef GCT_COMPRESS_MESH_HPP
#define GCT_COMPRESS_MESH_HPP

#include <glm/vec4.hpp>
#include <fx/gltf.h>
#include <gct/scene_graph_accessor.hpp>
#include <gct/vertex_attribute.hpp>

namespace gct::gltf {

struct loaded_vertex_attribute_buffer {
  std::uint32_t block_size = 0u;
  std::uint32_t component_count = 1u;
  scene_graph::accessor_type_id type = scene_graph::accessor_type_id::float_;
  std::vector< std::uint8_t > data;
};

struct loaded_vertex_buffer {
  std::uint32_t vertex_count = 0u;
  std::uint32_t unique_vertex_count = 0u;
  std::uint32_t meshlet_count = 0u;
  vk::PrimitiveTopology topology = vk::PrimitiveTopology::eTriangleList;
  std::unordered_map< vertex_attribute_id, loaded_vertex_attribute_buffer > attribute;
};

scene_graph::accessor_type_id to_accessor_type_id(
  const fx::gltf::Accessor &accessor
);

std::uint32_t to_accessor_component_count(
  const fx::gltf::Accessor &accessor
);

loaded_vertex_buffer load_vertex(
  const std::filesystem::path &base_dir,
  const fx::gltf::Document &doc,
  const fx::gltf::Primitive &primitive_
);

class meshlet_reader {
public:
  meshlet_reader(
    const loaded_vertex_buffer &b,
    std::uint32_t m
  );
  face_attribute operator()(
    std::uint32_t local_face_id
  ) const;
  std::uint32_t get_face_count() const;
private:
  const loaded_vertex_buffer &buffer;
  std::uint32_t meshlet_id = 0u;
  std::uint32_t vertex_offset = 0u;
  std::uint32_t vertex_count = 0u;
  std::vector< glm::vec4 > dgf_verts;
  bool valid = false;
};

face_attribute read_vertex_attribute(
  const loaded_vertex_buffer &buffer,
  std::uint32_t meshlet_id,
  std::uint32_t face_id
);

struct vertex_attribute_diff {
  float min = 0.0f;
  float max = 0.0f;
  float average = 0.0f;
};


void convert_to_n31(
  loaded_vertex_buffer &buffer
);

void convert_to_t31(
  loaded_vertex_buffer &buffer
);

void convert_to_n21t11(
  loaded_vertex_buffer &buffer
);

void convert_to_n20t11b1(
  loaded_vertex_buffer &buffer
);

void convert_to_float(
  loaded_vertex_buffer &buffer,
  vertex_attribute_id
);

void convert_to_half(
  loaded_vertex_buffer &buffer,
  vertex_attribute_id
);

void convert_to_fixed(
  loaded_vertex_buffer &buffer,
  vertex_attribute_id
);

void generate_tangent(
  loaded_vertex_buffer &buffer
);

using vertex_diff = std::unordered_map< vertex_attribute_id, vertex_attribute_diff >;

vertex_diff diff(
  const loaded_vertex_buffer &a,
  const loaded_vertex_buffer &b
);

}

#endif
