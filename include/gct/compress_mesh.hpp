#ifndef GCT_COMPRESS_MESH_HPP
#define GCT_COMPRESS_MESH_HPP

#include <optional>
#include <unordered_map>
#include <glm/vec4.hpp>
#include <gct/fx/gltf.h>
#include <gct/setter.hpp>
#include <gct/scene_graph_accessor.hpp>
#include <gct/vertex_attribute.hpp>


namespace gct::gltf {

struct primitive_key {
  LIBGCT_SETTER( mesh_id )
  LIBGCT_SETTER( primitive_id )
  std::uint32_t mesh_id = 0u;
  std::uint32_t primitive_id = 0u;
};

bool operator==( const primitive_key &l, const primitive_key &r );
bool operator!=( const primitive_key &l, const primitive_key &r );

struct accessor_link {
  LIBGCT_SETTER( mesh_id )
  LIBGCT_SETTER( primitive_id )
  LIBGCT_SETTER( attr_id )
  std::uint32_t mesh_id = 0u;
  std::uint32_t primitive_id = 0u;
  vertex_attribute_id attr_id = vertex_attribute_id::index;
};

bool operator==( const accessor_link &l, const accessor_link &r );
bool operator!=( const accessor_link &l, const accessor_link &r );

struct buffer_view_key {
  LIBGCT_SETTER( buffer_view_id )
  LIBGCT_SETTER( offset )
  std::uint32_t buffer_view_id = 0u;
  std::uint32_t offset = 0u;
};

bool operator==( const buffer_view_key &l, const buffer_view_key &r );
bool operator!=( const buffer_view_key &l, const buffer_view_key &r );

}

namespace std {
  template<>
  struct hash< gct::gltf::primitive_key > {
    std::size_t operator()( const gct::gltf::primitive_key &v ) const {
      return std::hash< std::uint64_t >()( ( std::uint64_t( v.mesh_id ) << 32u ) | std::uint64_t( v.primitive_id ) );
    }
  };
  template<>
  struct hash< gct::gltf::accessor_link > {
    std::size_t operator()( const gct::gltf::accessor_link &v ) const {
      return std::hash< std::uint64_t >()( ( std::uint64_t( v.mesh_id ) << 32u ) | std::uint64_t( v.primitive_id ) ) ^
             std::hash< std::uint32_t >()( std::uint32_t( v.attr_id ) );
    }
  };
  template<>
  struct hash< gct::gltf::buffer_view_key > {
    std::size_t operator()( const gct::gltf::buffer_view_key &v ) const {
      return std::hash< std::uint64_t >()( ( std::uint64_t( v.buffer_view_id ) << 32u ) | std::uint64_t( v.offset ) );
    }
  };
}

namespace gct::gltf {


struct loaded_vertex_attribute_buffer {
  std::uint32_t block_size = 0u;
  std::uint32_t component_count = 1u;
  scene_graph::accessor_type_id type = scene_graph::accessor_type_id::float_;
  glm::vec4 min = glm::vec4( 0.0f, 0.0f, 0.0f, 0.0f );
  glm::vec4 max = glm::vec4( 0.0f, 0.0f, 0.0f, 0.0f );
  bool normalized = false;
  std::vector< std::uint8_t > data;
  std::optional< accessor_link > reuse;
  std::uint32_t output_accessor_index = 0u;
  std::uint32_t output_buffer_view_index = 0u;
};

struct loaded_vertex_buffer {
  std::uint32_t vertex_count = 0u;
  std::uint32_t unique_vertex_count = 0u;
  std::uint32_t meshlet_count = 0u;
  vk::PrimitiveTopology topology = vk::PrimitiveTopology::eTriangleList;
  std::unordered_map< vertex_attribute_id, loaded_vertex_attribute_buffer > attribute;
};

using loaded_mesh = std::unordered_map< primitive_key, loaded_vertex_buffer >;

loaded_vertex_buffer load_vertex_on_cpu(
  const std::filesystem::path &base_dir,
  const fx::gltf::Document &doc,
  const fx::gltf::Primitive &primitive_
);

loaded_mesh load_mesh_on_cpu(
  const std::filesystem::path &base_dir,
  const fx::gltf::Document &doc,
  const std::unordered_set< std::uint32_t > &primitive_filter = std::unordered_set< std::uint32_t >{}
);

void store_mesh_on_cpu(
  const std::filesystem::path &base_dir,
  fx::gltf::Document &doc,
  loaded_mesh &mesh,
  const std::string &buffer_name
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
  face_attribute get_position(
    std::uint32_t local_face_id
  ) const;
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

void convert_to_dgf(
  loaded_vertex_buffer &buffer,
  std::uint32_t
);

void convert_from_dgf(
  loaded_vertex_buffer &buffer
);

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

void generate_minmax(
  loaded_vertex_buffer &buffer
);

void meshlet_statistics(
  const loaded_mesh &mesh
);

void mesh_statistics(
  const loaded_mesh &mesh
);

void dump_vertex(
  const loaded_mesh &mesh
);

void dedup(
  const loaded_mesh &mesh
);

void dump_attribute_size(
  loaded_mesh &mesh
);

void convert_mesh( loaded_mesh &mesh, const std::vector< std::string > &command );

using vertex_diff = std::unordered_map< vertex_attribute_id, vertex_attribute_diff >;

vertex_diff diff(
  const loaded_vertex_buffer &a,
  const loaded_vertex_buffer &b
);

}

#endif
