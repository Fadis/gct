#include <charconv>
#include <boost/crc.hpp>
#ifdef GCT_ENABLE_DGF
#include <DGF.h>
#include <DGFBaker.h>
#endif
#include <gct/fx/gltf.h>
#include <gct/scene_graph_accessor.hpp>
#include <gct/gltf.hpp>
#include <gct/gltf2.hpp>
#include <gct/compress_mesh.hpp>
#include <gct/mmaped_file.hpp>
#include <gct/n21t11.hpp>
#include <gct/n20t11b1.hpp>
#include <gct/n31.hpp>
#include <gct/fixed.hpp>
#include <gct/mikktspace.hpp>
#include <gct/half.hpp>

namespace gct::gltf {

bool operator==( const primitive_key &l, const primitive_key &r ) {
  return l.mesh_id == r.mesh_id && l.primitive_id == r.primitive_id;
}

bool operator!=( const primitive_key &l, const primitive_key &r ) {
  return l.mesh_id != r.mesh_id || l.primitive_id != r.primitive_id;
}

bool operator==( const accessor_link &l, const accessor_link &r ) {
  return l.mesh_id == r.mesh_id && l.primitive_id == r.primitive_id && l.attr_id == r.attr_id;
}

bool operator!=( const accessor_link &l, const accessor_link &r ) {
  return l.mesh_id != r.mesh_id || l.primitive_id != r.primitive_id || l.attr_id != r.attr_id;
}

bool operator==( const buffer_view_key &l, const buffer_view_key &r ) {
  return l.buffer_view_id == r.buffer_view_id && l.offset == r.offset;
}

bool operator!=( const buffer_view_key &l, const buffer_view_key &r ) {
  return l.buffer_view_id != r.buffer_view_id || l.offset != r.offset;
}

std::size_t get_component_size(
  const loaded_vertex_attribute_buffer &attr
) {
  return scene_graph::get_component_size( attr.type );
}

std::size_t get_decoded_component_count(
  const loaded_vertex_buffer &buffer,
  vertex_attribute_id attr_id
) {
  if( buffer.attribute.find( attr_id ) != buffer.attribute.end() ) {
    if( buffer.attribute.at( attr_id ).type == scene_graph::accessor_type_id::dgf ) return 3u;
    else if( buffer.attribute.at( attr_id ).type == scene_graph::accessor_type_id::n21t11 ) return 3u;
    else if( buffer.attribute.at( attr_id ).type == scene_graph::accessor_type_id::n20t11b1 ) return 3u;
    else if( buffer.attribute.at( attr_id ).type == scene_graph::accessor_type_id::n31 ) return 3u;
    else return buffer.attribute.at( attr_id ).component_count;
  }
  else if( attr_id == vertex_attribute_id::tangent && buffer.attribute.find( vertex_attribute_id::normal ) != buffer.attribute.end() ) {
    if( buffer.attribute.at( vertex_attribute_id::normal ).type == scene_graph::accessor_type_id::n21t11 ) return 4u;
    else if( buffer.attribute.at( vertex_attribute_id::normal ).type == scene_graph::accessor_type_id::n20t11b1 ) return 4u;
    else if( buffer.attribute.at( vertex_attribute_id::normal ).type == scene_graph::accessor_type_id::n31 ) return 4u;
  }
  return 0u;
}

std::size_t get_decoded_component_count(
  const loaded_vertex_attribute_buffer &attr
) {
  if( attr.type == scene_graph::accessor_type_id::dgf ) return 3u;
  else if( attr.type == scene_graph::accessor_type_id::n21t11 ) return 3u;
  else if( attr.type == scene_graph::accessor_type_id::n20t11b1 ) return 3u;
  else if( attr.type == scene_graph::accessor_type_id::n31 ) return 3u;
  else return attr.component_count;
  return 0u;
}

std::uint32_t get_face_vertex_count( vk::PrimitiveTopology topo ) {
  if( topo == vk::PrimitiveTopology::eTriangleList ) return 3u;
  else if( topo == vk::PrimitiveTopology::eLineList ) return 2u;
  else if( topo == vk::PrimitiveTopology::ePointList ) return 1u;
  else {
    throw invalid_gltf( "サポートされていないトポロジ" );
  }
}


std::size_t get_block_size(
  const loaded_vertex_attribute_buffer &attr
) {
  return get_component_size( attr ) * attr.component_count;
}

void resize_attribute_buffer(
  loaded_vertex_attribute_buffer &attr,
  std::uint32_t count
) {
  attr.data.resize( get_block_size( attr ) * count );
}

void write_vertex(
  const loaded_vertex_attribute_buffer &attr,
  std::uint8_t *dest,
  std::uint32_t src_offset,
  std::uint32_t dest_offset
) {
  std::memcpy(
    dest + dest_offset,
    attr.data.data() + src_offset,
    get_block_size( attr )
  );
}

void write_vertex(
  const std::uint8_t *src,
  loaded_vertex_attribute_buffer &attr,
  std::uint32_t src_offset,
  std::uint32_t dest_offset
) {
  std::memcpy(
    attr.data.data() + dest_offset,
    src + src_offset,
    get_block_size( attr )
  );
}

void write_vertex(
  const std::uint8_t *src,
  loaded_vertex_attribute_buffer &attr,
  std::uint32_t src_offset,
  std::uint32_t src_stride,
  std::uint32_t vertex_count
) {
  for( unsigned int i = 0u; i != vertex_count; ++i ) {
    const std::uint32_t input_offset = src_offset + i * src_stride;
    const std::uint32_t output_offset = i * get_block_size( attr );
    write_vertex( src, attr, input_offset, output_offset );
  }
}

void write_vertex(
  const loaded_vertex_attribute_buffer &attr,
  std::uint8_t *dest,
  std::uint32_t dest_offset,
  std::uint32_t dest_stride,
  std::uint32_t block_count
) {
  for( unsigned int i = 0u; i != block_count; ++i ) {
    const std::uint32_t input_offset = i * get_block_size( attr );
    const std::uint32_t output_offset = dest_offset + i * dest_stride;
    write_vertex( attr, dest, input_offset, output_offset );
  }
}



loaded_vertex_buffer load_vertex_on_cpu(
  const std::filesystem::path &base_dir,
  const fx::gltf::Document &doc,
  const fx::gltf::Primitive &primitive_,
  const primitive_key &key,
  std::unordered_map< std::uint32_t, accessor_link > &accessor_reuse,
  std::unordered_map< buffer_view_key, accessor_link > &buffer_view_reuse
) {
  loaded_vertex_buffer buffer;

  check_primitive( doc, primitive_ );
  const auto [position_type_id,meshlet_count,vertex_count] = get_meshlet_count( doc, primitive_ );

  for( const auto &[target,index]: primitive_.attributes ) {
    const auto &accessor = doc.accessors[ index ];
    const auto &view = doc.bufferViews[ accessor.bufferView ];
    const std::uint32_t default_stride = gct::gltf::to_size( accessor );
    const std::uint32_t stride = view.byteStride ? view.byteStride : default_stride;
    const std::uint32_t offset = accessor.byteOffset + view.byteOffset;
    const vk::Format vertex_format = to_vulkan_format( accessor.componentType, accessor.type, accessor.normalized );
 
    const std::filesystem::path uri = base_dir / doc.buffers[ view.buffer ].uri;
 
    const auto attr_id = gct::to_vertex_attribute_id( target );
 
    buffer.attribute[ attr_id ].type = scene_graph::to_accessor_type_id( accessor );
    buffer.attribute[ attr_id ].component_count = scene_graph::to_accessor_component_count( accessor );
    buffer.attribute[ attr_id ].block_size = get_block_size( buffer.attribute[ attr_id ] );
    if( !check_accessor_type( attr_id, buffer.attribute[ attr_id ].type, buffer.attribute[ attr_id ].component_count ) ) {
      throw invalid_gltf( "サポートされていない型が設定されている" );
    }

    bool reuse = false;
    const auto al = accessor_link()
      .set_mesh_id( key.mesh_id )
      .set_primitive_id( key.primitive_id )
      .set_attr_id( attr_id );
    if( accessor_reuse.find( index ) != accessor_reuse.end() ) {
      buffer.attribute[ attr_id ].reuse = accessor_reuse[ index ];
      reuse = true;
    }
    else {
      accessor_reuse[ index ] = al;
    }
    const auto bvid = buffer_view_key().set_buffer_view_id( accessor.bufferView ).set_offset( offset );
    if( buffer_view_reuse.find( bvid ) != buffer_view_reuse.end() ) {
      buffer.attribute[ attr_id ].reuse = buffer_view_reuse[ bvid ];
      reuse = true;
    }
    else {
      buffer_view_reuse[ bvid ] = al;
    }
    const uint32_t block_count =
      ( buffer.attribute[ attr_id ].type == scene_graph::accessor_type_id::dgf ) ? meshlet_count : vertex_count; 
    resize_attribute_buffer( buffer.attribute[ attr_id ], block_count );
    
    gct::mmaped_file m( uri );
    write_vertex( m.data(), buffer.attribute[ attr_id ], offset, stride, block_count );
  }
  
  std::uint32_t unique_vertex_count = 0u;
 
  if( primitive_.indices >= 0 ) {
    const auto &accessor = doc.accessors[ primitive_.indices ];
    if( accessor.bufferView < 0 || doc.bufferViews.size() <= size_t( accessor.bufferView ) ) throw gct::exception::invalid_argument( "参照されたbufferViewが存在しない", __FILE__, __LINE__ );
    const auto &view = doc.bufferViews[ accessor.bufferView ];
    const std::uint32_t default_stride = gct::gltf::to_size( accessor );
    const std::uint32_t stride = view.byteStride ? view.byteStride : default_stride;
    const std::uint32_t max_count = ( view.byteLength - ( accessor.byteOffset ) ) / stride;
    if( view.buffer < 0 || doc.buffers.size() <= size_t( view.buffer ) ) throw gct::exception::invalid_argument( "参照されたbufferが存在しない", __FILE__, __LINE__ );
    const uint32_t block_count = gltf::get_block_count( accessor );
    if( block_count > max_count ) throw gct::exception::invalid_argument( "指定された要素数に対してbufferViewが小さすぎる" );
    const std::uint32_t vertex_count = accessor.count;
    const std::uint32_t offset = accessor.byteOffset + view.byteOffset;
    const std::filesystem::path uri = base_dir / doc.buffers[ view.buffer ].uri;
    const vk::Format vertex_format = gct::gltf::to_vulkan_format( accessor.componentType, accessor.type, accessor.normalized );
 
    const auto attr_id = vertex_attribute_id::index;
    buffer.attribute[ attr_id ].type = scene_graph::to_accessor_type_id( accessor );
    buffer.attribute[ attr_id ].component_count = scene_graph::to_accessor_component_count( accessor );
    buffer.attribute[ attr_id ].block_size = get_block_size( buffer.attribute[ attr_id ] );
    buffer.attribute[ attr_id ].normalized = accessor.normalized;
    if( !check_accessor_type( attr_id, buffer.attribute[ attr_id ].type, buffer.attribute[ attr_id ].component_count ) ) {
      throw invalid_gltf( "サポートされていない型が設定されている" );
    }
 
    unique_vertex_count = vertex_count;

    bool reuse = false;
    const auto al = accessor_link()
      .set_mesh_id( key.mesh_id )
      .set_primitive_id( key.primitive_id )
      .set_attr_id( attr_id );
    if( accessor_reuse.find( primitive_.indices ) != accessor_reuse.end() ) {
      buffer.attribute[ attr_id ].reuse = accessor_reuse[ primitive_.indices ];
      reuse = true;
    }
    else {
      accessor_reuse[ primitive_.indices ] = al;
    }
    const auto bvid = buffer_view_key().set_buffer_view_id( accessor.bufferView ).set_offset( offset );
    if( buffer_view_reuse.find( bvid ) != buffer_view_reuse.end() ) {
      buffer.attribute[ attr_id ].reuse = buffer_view_reuse[ bvid ];
      reuse = true;
    }
    else {
      buffer_view_reuse[ bvid ] = al;
    }

    resize_attribute_buffer( buffer.attribute[ attr_id ], vertex_count );
 
    gct::mmaped_file m( uri );
    write_vertex( m.data(), buffer.attribute[ attr_id ], offset, stride, vertex_count );
  }
  else {
    unique_vertex_count = vertex_count;
  }

  if( position_type_id == scene_graph::accessor_type_id::dgf ) {
    buffer.meshlet_count = meshlet_count;
  }
  else {
    buffer.meshlet_count = unique_vertex_count / 96u + ( ( unique_vertex_count % 96u ) ? 1u : 0u );
  }

  buffer.topology = gltf_topology_to_vulkan_topology( primitive_.mode );
  buffer.vertex_count = vertex_count;
  buffer.unique_vertex_count = unique_vertex_count;

  return buffer;
}

loaded_mesh load_mesh_on_cpu(
  const std::filesystem::path &base_dir,
  const fx::gltf::Document &doc,
  const std::unordered_set< std::uint32_t > &primitive_filter
) {
  std::unordered_map< std::uint32_t, accessor_link > accessor_reuse;
  std::unordered_map< buffer_view_key, accessor_link > buffer_view_reuse;

  loaded_mesh mesh;
  std::uint32_t mesh_id = 0u;
  for( const auto &m: doc.meshes ) {
    std::uint32_t primitive_id = 0u;
    for( const auto &p: m.primitives ) {
      if( primitive_filter.empty() || primitive_filter.find( primitive_id ) != primitive_filter.end() ) {
        const auto key = primitive_key{}.set_mesh_id( mesh_id ).set_primitive_id( primitive_id );
        mesh[ key ] = gct::gltf::load_vertex_on_cpu( base_dir, doc, p, key, accessor_reuse, buffer_view_reuse );
      }
      ++primitive_id;
    }
    ++mesh_id;
  }
  return mesh;
}

namespace {

void insert_padding(
  std::vector< std::uint8_t > &out,
  std::uint32_t &pos,
  std::uint32_t align
) {
  std::uint32_t rem = pos % align;
  if( rem != 0u ) {
    std::uint32_t pad = align - rem;
    out.resize( out.size() + pad, 0u );
    pos += pad;
  }
}

}

fx::gltf::Accessor::ComponentType to_gltf_standard_component_type( scene_graph::accessor_type_id type ) {
  if( type == scene_graph::accessor_type_id::float_ ) return fx::gltf::Accessor::ComponentType::Float;
  else if( type == scene_graph::accessor_type_id::i8 ) return fx::gltf::Accessor::ComponentType::Byte;
  else if( type == scene_graph::accessor_type_id::u8 ) return fx::gltf::Accessor::ComponentType::UnsignedByte;
  else if( type == scene_graph::accessor_type_id::i16 ) return fx::gltf::Accessor::ComponentType::Short;
  else if( type == scene_graph::accessor_type_id::u16 ) return fx::gltf::Accessor::ComponentType::UnsignedShort;
  else if( type == scene_graph::accessor_type_id::u32 ) return fx::gltf::Accessor::ComponentType::UnsignedInt;
  else if( type == scene_graph::accessor_type_id::n31 ) return fx::gltf::Accessor::ComponentType::UnsignedInt;
  else if( type == scene_graph::accessor_type_id::n21t11 ) return fx::gltf::Accessor::ComponentType::UnsignedInt;
  else if( type == scene_graph::accessor_type_id::n20t11b1 ) return fx::gltf::Accessor::ComponentType::UnsignedInt;
  else if( type == scene_graph::accessor_type_id::half ) return fx::gltf::Accessor::ComponentType::UnsignedShort;
  else if( type == scene_graph::accessor_type_id::fixed ) return fx::gltf::Accessor::ComponentType::Short;
  else if( type == scene_graph::accessor_type_id::dgf ) return fx::gltf::Accessor::ComponentType::Byte;
  else return fx::gltf::Accessor::ComponentType::None;
}

fx::gltf::Accessor::Type to_gltf_standard_type( scene_graph::accessor_type_id type, std::uint32_t component_count ) {
  if( type == scene_graph::accessor_type_id::dgf ) return fx::gltf::Accessor::Type::Scalar;
  else if( type == scene_graph::accessor_type_id::n31 ) return fx::gltf::Accessor::Type::Scalar;
  else if( type == scene_graph::accessor_type_id::n21t11 ) return fx::gltf::Accessor::Type::Scalar;
  else if( type == scene_graph::accessor_type_id::n20t11b1 ) return fx::gltf::Accessor::Type::Scalar;
  else if( component_count == 1u ) return fx::gltf::Accessor::Type::Scalar;
  else if( component_count == 2u ) return fx::gltf::Accessor::Type::Vec2;
  else if( component_count == 3u ) return fx::gltf::Accessor::Type::Vec3;
  else if( component_count == 4u ) return fx::gltf::Accessor::Type::Vec4;
  else return fx::gltf::Accessor::Type::None;
}

nlohmann::json to_extended_type( scene_graph::accessor_type_id type, std::uint32_t component_count, std::uint32_t block_count ) {
  nlohmann::json root = nlohmann::json::object();
  root[ "component_size" ] = get_component_size( type );
  root[ "decoded_component_count" ] = component_count;
  root[ "block_count" ] = block_count;
  if( type == scene_graph::accessor_type_id::float_ ) {
    root[ "type" ] = "FLOAT";
  }
  else if( type == scene_graph::accessor_type_id::i8 ) {
    root[ "type" ] = "BYTE";
  }
  else if( type == scene_graph::accessor_type_id::u8 ) {
    root[ "type" ] = "UNSIGNED_BYTE";
  }
  else if( type == scene_graph::accessor_type_id::i16 ) {
    root[ "type" ] = "SHORT";
  }
  else if( type == scene_graph::accessor_type_id::u16 ) {
    root[ "type" ] = "UNSIGNED SHORT";
  }
  else if( type == scene_graph::accessor_type_id::u32 ) {
    root[ "type" ] = "UNSIGNED INT";
  }
  else if( type == scene_graph::accessor_type_id::float_ ) {
    root[ "type" ] = "FLOAT";
  }
  else if( type == scene_graph::accessor_type_id::dgf ) {
    root[ "type" ] = "DGF";
  }
  else if( type == scene_graph::accessor_type_id::half ) {
    root[ "type" ] = "HALF";
  }
  else if( type == scene_graph::accessor_type_id::fixed ) {
    root[ "type" ] = "FIXED";
  }
  else if( type == scene_graph::accessor_type_id::n21t11 ) {
    root[ "type" ] = "N21T11";
  }
  else if( type == scene_graph::accessor_type_id::n20t11b1 ) {
    root[ "type" ] = "N20T11B1";
  }
  else if( type == scene_graph::accessor_type_id::n31 ) {
    root[ "type" ] = "N31";
  }
  else {
    throw exception::invalid_argument( "to_extended_type : 使用できないアクセサの型", __FILE__, __LINE__ );
  }
  return root;
}

void set_extended_type( nlohmann::json &dest, const loaded_vertex_attribute_buffer &attr ) {
  if( !dest.is_object() ) {
    dest = nlohmann::json::object();
  }
  if( dest.find( "extensions" ) == dest.end() ) {
    dest[ "extensions" ] = nlohmann::json::object();
  }
  if( dest[ "extensions" ].find( "GCT_EXTENDED_TYPE" ) == dest[ "extensions" ].end() ) {
    dest[ "extensions" ][ "GCT_EXTENDED_TYPE" ] = nlohmann::json::object();
  }
  dest[ "extensions" ][ "GCT_EXTENDED_TYPE" ] = to_extended_type(
    attr.type,
    get_decoded_component_count( attr ),
    attr.data.size() / get_block_size( attr )
  );
}

void clear_accessor(
  fx::gltf::Primitive &prim_object
) {
  prim_object.indices = -1;
  prim_object.attributes.clear();
}

void update_accessor(
  fx::gltf::Primitive &prim_object,
  vertex_attribute_id attr_id,
  std::uint32_t accessor_index
) {
  if( accessor_index == 0u && attr_id != vertex_attribute_id::index ) std::abort();
  if( attr_id == vertex_attribute_id::index ) {
    prim_object.indices = accessor_index;
  }
  else if( attr_id == vertex_attribute_id::position ) {
    prim_object.attributes[ "POSITION" ] = accessor_index;
  }
  else if( attr_id == vertex_attribute_id::normal ) {
    prim_object.attributes[ "NORMAL" ] = accessor_index;
  }
  else if( attr_id == vertex_attribute_id::tangent ) {
    prim_object.attributes[ "TANGENT" ] = accessor_index;
  }
  else if( attr_id == vertex_attribute_id::texcoord_0 ) {
    prim_object.attributes[ "TEXCOORD_0" ] = accessor_index;
  }
  else if( attr_id == vertex_attribute_id::color_0 ) {
    prim_object.attributes[ "COLOR_0" ] = accessor_index;
  }
  else if( attr_id == vertex_attribute_id::joint_0 ) {
    prim_object.attributes[ "JOINTS_0" ] = accessor_index;
  }
  else if( attr_id == vertex_attribute_id::weight_0 ) {
    prim_object.attributes[ "WEIGHTS_0" ] = accessor_index;
  }
}

void store_mesh_on_cpu(
  const std::filesystem::path &base_dir,
  fx::gltf::Document &doc,
  loaded_mesh &mesh,
  const std::string &buffer_name
) {
  std::vector< std::uint8_t > bin;
  std::uint32_t pos = 0u;
  doc.accessors.clear();
  doc.bufferViews.clear();
  doc.buffers.clear();

  for( auto &[key,primitive]: mesh ) {
    auto &prim_object = doc.meshes[ key.mesh_id ].primitives[ key.primitive_id ];
    prim_object.indices = -1;
    prim_object.attributes.clear();
    for( auto &[attr_id,attr]: primitive.attribute ) {
      if( !attr.reuse ) {
        insert_padding( bin, pos, get_block_size( attr ) );
        {
          fx::gltf::BufferView v;
          v.buffer = 0u;
          v.byteLength = attr.data.size();
          v.byteOffset = pos;
          v.byteStride = 0u;
          v.target = fx::gltf::BufferView::TargetType::ArrayBuffer;
          doc.bufferViews.push_back( v );
          attr.output_buffer_view_index = doc.bufferViews.size() - 1u;
        }
        {
          fx::gltf::Accessor a;
          a.bufferView = doc.bufferViews.size() - 1u;
          a.byteOffset = 0u;
          a.componentType = to_gltf_standard_component_type( attr.type );
          a.type = to_gltf_standard_type( attr.type, attr.component_count );
          set_extended_type( a.extensionsAndExtras, attr );
          if( attr_id == vertex_attribute_id::index ) {
            a.count = primitive.unique_vertex_count;
          }
          else {
            a.count = primitive.vertex_count;
          }
          const std::uint32_t decoded_component_count = get_decoded_component_count( attr );
          a.min.resize( decoded_component_count );
          a.max.resize( decoded_component_count );
          for( std::uint32_t j = 0u; j != decoded_component_count; ++j ) {
            a.min[ j ] = attr.min[ j ];
            a.max[ j ] = attr.max[ j ];
          }
          //std::cout << "min (" << a.min[ 0 ] << "," << a.min[ 1 ] << "," << a.min[ 2 ] << ")" << std::endl;
          //std::cout << "max (" << a.max[ 0 ] << "," << a.max[ 1 ] << "," << a.max[ 2 ] << ")" << std::endl;
          a.normalized = attr.normalized;
          doc.accessors.push_back( a );
          attr.output_accessor_index = doc.accessors.size() - 1u;
        }
        std::copy( attr.data.begin(), attr.data.end(), std::back_insert_iterator( bin ) );
        pos += attr.data.size();
        update_accessor( prim_object, attr_id, doc.accessors.size() - 1u );
      }
    }
  }
  for( auto &[key,primitive]: mesh ) {
    auto &prim_object = doc.meshes[ key.mesh_id ].primitives[ key.primitive_id ];
    for( auto &[attr_id,attr]: primitive.attribute ) {
      if( attr.reuse ) {
        const auto pk = primitive_key()
          .set_mesh_id( attr.reuse->mesh_id )
          .set_primitive_id( attr.reuse->primitive_id );
        auto *reused_from = &mesh.at( pk ).attribute.at( attr.reuse->attr_id );
        while( reused_from->reuse ) {
          const auto pk = primitive_key()
            .set_mesh_id( reused_from->reuse->mesh_id )
            .set_primitive_id( reused_from->reuse->primitive_id );
          reused_from = &mesh.at( pk ).attribute.at( reused_from->reuse->attr_id );
        }
        update_accessor( prim_object, attr_id, reused_from->output_accessor_index );
      }
    }
  }
  {
    fx::gltf::Buffer b;
    b.byteLength = pos;
    b.uri = buffer_name;
    b.data = std::move( bin );
    doc.buffers.push_back( b );
  }
}


meshlet_reader::meshlet_reader(
  const loaded_vertex_buffer &b,
  std::uint32_t m
) : buffer( b ), meshlet_id( m ) {
  const auto fvc = get_face_vertex_count( buffer.topology );
  if(
    buffer.topology == vk::PrimitiveTopology::eTriangleList &&
    buffer.attribute.find( vertex_attribute_id::position ) != buffer.attribute.end() &&
    buffer.attribute.at( vertex_attribute_id::position ).type == scene_graph::accessor_type_id::dgf &&
    buffer.attribute.at( vertex_attribute_id::position ).data.size() > meshlet_id * buffer.attribute.at( vertex_attribute_id::position ).block_size
  ) {
#ifdef GCT_ENABLE_DGF
    const std::uint8_t *head = buffer.attribute.at( vertex_attribute_id::position ).data.data() + meshlet_id * buffer.attribute.at( vertex_attribute_id::position ).block_size;
    DGF::MetaData meta;
    DGF::DecodeMetaData( &meta, head );
    DGF::TriControlValues controlValues[ DGF::MAX_TRIS ];
    std::uint8_t indexBuffer[ DGF::MAX_INDICES ];
    DGF::DecodeTopology( controlValues, indexBuffer, head );
    
    std::uint8_t triangleList[ DGF::MAX_INDICES ];
    DGF::ConvertTopologyToTriangleList( triangleList, controlValues, indexBuffer, meta.numTris );
    
    DGF::OffsetVert offsetVerts[ DGF::MAX_VERTS ];
    DGF::DecodeOffsetVerts( meta.numVerts, offsetVerts, head );
    
    DGF::FloatVert dgf_verts_[ DGF::MAX_VERTS ];
    DGF::ConvertOffsetsToFloat( meta.numVerts, dgf_verts_, offsetVerts, meta );
    
    dgf_verts.reserve( meta.numVerts );
    for( unsigned int i = 0u; i != meta.numTris * get_face_vertex_count( buffer.topology ); ++i ) {
      dgf_verts.push_back( glm::vec4( dgf_verts_[ triangleList[ i ] ].xyz[ 0 ], dgf_verts_[ triangleList[ i ] ].xyz[ 1 ], dgf_verts_[ triangleList[ i ] ].xyz[ 2 ], 1.0f ) );
    }

    std::uint32_t primitive_offset = 0u;
    DGF::ReadUserData( head, &primitive_offset, 0, sizeof( std::uint32_t ) );
    vertex_offset = primitive_offset * fvc;
    vertex_count = meta.numTris * fvc;
    valid = true;
#endif
  }
  else if(
    buffer.attribute.find( vertex_attribute_id::index ) != buffer.attribute.end() &&
    buffer.attribute.at( vertex_attribute_id::index ).data.size() > meshlet_id * 32u * fvc * buffer.attribute.at( vertex_attribute_id::index ).block_size
  ) {
    vertex_offset = meshlet_id * 32u * fvc;
    vertex_count = std::min( buffer.unique_vertex_count - vertex_offset, 32u * fvc );
    valid = true;
  }
  else if(
    buffer.attribute.find( vertex_attribute_id::position ) != buffer.attribute.end() &&
    buffer.attribute.at( vertex_attribute_id::position ).data.size() > meshlet_id * 32u * fvc * buffer.attribute.at( vertex_attribute_id::position ).block_size
  ) {
    vertex_offset = meshlet_id * 32u * fvc;
    vertex_count = std::min( buffer.unique_vertex_count - vertex_offset, 32u * fvc );
    valid = true;
  }
}

std::uint32_t read_index( const loaded_vertex_buffer &buffer, std::uint32_t i ) {
  if( buffer.attribute.find( vertex_attribute_id::index ) != buffer.attribute.end() ) {
    if( buffer.attribute.at( vertex_attribute_id::index ).type == scene_graph::accessor_type_id::i8 ) {
      return reinterpret_cast< const std::int8_t* >( buffer.attribute.at( vertex_attribute_id::index ).data.data() )[ i ];
    }
    else if( buffer.attribute.at( vertex_attribute_id::index ).type == scene_graph::accessor_type_id::u8 ) {
      return reinterpret_cast< const std::uint8_t* >( buffer.attribute.at( vertex_attribute_id::index ).data.data() )[ i ];
    }
    else if( buffer.attribute.at( vertex_attribute_id::index ).type == scene_graph::accessor_type_id::i16 ) {
      return reinterpret_cast< const std::int16_t* >( buffer.attribute.at( vertex_attribute_id::index ).data.data() )[ i ];
    }
    else if( buffer.attribute.at( vertex_attribute_id::index ).type == scene_graph::accessor_type_id::u16 ) {
      return reinterpret_cast< const std::uint16_t* >( buffer.attribute.at( vertex_attribute_id::index ).data.data() )[ i ];
    }
    else if( buffer.attribute.at( vertex_attribute_id::index ).type == scene_graph::accessor_type_id::u32 ) {
      return reinterpret_cast< const std::uint32_t* >( buffer.attribute.at( vertex_attribute_id::index ).data.data() )[ i ];
    }
    else if( buffer.attribute.at( vertex_attribute_id::index ).type == scene_graph::accessor_type_id::float_ ) {
      return reinterpret_cast< const float* >( buffer.attribute.at( vertex_attribute_id::index ).data.data() )[ i ];
    }
    else {
      return i;
    }
  }
  else {
    return i;
  }
}

glm::vec4 read_vertex( const loaded_vertex_attribute_buffer &buffer, std::uint32_t i ) {
  glm::vec4 value = glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f );
  if( buffer.type == scene_graph::accessor_type_id::i8 ) {
    const auto head = reinterpret_cast< const std::int8_t* >( buffer.data.data() );
    const auto component_count = buffer.component_count;
    for( unsigned int j = 0u; j != component_count; ++j ) {
      value[ j ] = head[ i * component_count + j ];
    }
  }
  else if( buffer.type == scene_graph::accessor_type_id::u8 ) {
    const auto head = reinterpret_cast< const std::uint8_t* >( buffer.data.data() );
    const auto component_count = buffer.component_count;
    for( unsigned int j = 0u; j != component_count; ++j ) {
      value[ j ] = head[ i * component_count + j ];
    }
  }
  else if( buffer.type == scene_graph::accessor_type_id::i16 ) {
    const auto head = reinterpret_cast< const std::int16_t* >( buffer.data.data() );
    const auto component_count = buffer.component_count;
    for( unsigned int j = 0u; j != component_count; ++j ) {
      value[ j ] = head[ i * component_count + j ];
    }
  }
  else if( buffer.type == scene_graph::accessor_type_id::u16 ) {
    const auto head = reinterpret_cast< const std::uint16_t* >( buffer.data.data() );
    const auto component_count = buffer.component_count;
    for( unsigned int j = 0u; j != component_count; ++j ) {
      value[ j ] = head[ i * component_count + j ];
    }
  }
  else if( buffer.type == scene_graph::accessor_type_id::u32 ) {
    const auto head = reinterpret_cast< const std::uint32_t* >( buffer.data.data() );
    const auto component_count = buffer.component_count;
    for( unsigned int j = 0u; j != component_count; ++j ) {
      value[ j ] = head[ i * component_count + j ];
    }
  }
  else if( buffer.type == scene_graph::accessor_type_id::float_ ) {
    const auto head = reinterpret_cast< const float* >( buffer.data.data() );
    const auto component_count = buffer.component_count;
    for( unsigned int j = 0u; j != component_count; ++j ) {
      value[ j ] = head[ i * component_count + j ];
    }
  }
  else if( buffer.type == scene_graph::accessor_type_id::half ) {
    const auto head = reinterpret_cast< const half_float::half* >( buffer.data.data() );
    const auto component_count = buffer.component_count;
    for( unsigned int j = 0u; j != component_count; ++j ) {
      value[ j ] = head[ i * component_count + j ];
    }
  }
  else if( buffer.type == scene_graph::accessor_type_id::fixed ) {
    const auto head = reinterpret_cast< const std::int16_t* >( buffer.data.data() );
    const auto component_count = buffer.component_count;
    for( unsigned int j = 0u; j != component_count; ++j ) {
      value[ j ] = decode_fixed( head[ i * component_count + j ] );
    }
  }
  else if( buffer.type == scene_graph::accessor_type_id::n31 ) {
    const auto head = reinterpret_cast< const std::uint32_t* >( buffer.data.data() );
    const auto decoded = n31_decode( head[ i ] );
    for( unsigned int j = 0u; j != 3u; ++j ) {
      value[ j ] = decoded[ j ];
    }
  }
  else if( buffer.type == scene_graph::accessor_type_id::n21t11 ) {
    const auto head = reinterpret_cast< const std::uint32_t* >( buffer.data.data() );
    const auto decoded = n21t11_decode_normal( head[ i ] );
    for( unsigned int j = 0u; j != 3u; ++j ) {
      value[ j ] = decoded[ j ];
    }
  }
  else if( buffer.type == scene_graph::accessor_type_id::n20t11b1 ) {
    const auto head = reinterpret_cast< const std::uint32_t* >( buffer.data.data() );
    const auto decoded = n20t11b1_decode_normal( head[ i ] );
    for( unsigned int j = 0u; j != 3u; ++j ) {
      value[ j ] = decoded[ j ];
    }
  }
  return value;
}


glm::vec4 read_vertex( const loaded_vertex_buffer &buffer, vertex_attribute_id attr_id, std::uint32_t i ) {
  glm::vec4 value = glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f );
  if( buffer.attribute.find( attr_id ) != buffer.attribute.end() ) {
    value = read_vertex( buffer.attribute.at( attr_id ), i );
  }
  else if( attr_id == vertex_attribute_id::tangent && buffer.attribute.find( vertex_attribute_id::normal ) != buffer.attribute.end() ) {
    if( buffer.attribute.at( vertex_attribute_id::normal ).type == scene_graph::accessor_type_id::n21t11 ) {
      const auto head = reinterpret_cast< const std::uint32_t* >( buffer.attribute.at( vertex_attribute_id::normal ).data.data() );
      const auto normal = n21t11_decode_normal( head[ i ] );
      value = glm::vec4( n21t11_decode_tangent( head[ i ], normal ), 1.0f );
    }
    else if( buffer.attribute.at( vertex_attribute_id::normal ).type == scene_graph::accessor_type_id::n20t11b1 ) {
      const auto head = reinterpret_cast< const std::uint32_t* >( buffer.attribute.at( vertex_attribute_id::normal ).data.data() );
      const auto normal = n20t11b1_decode_normal( head[ i ] );
      value = n20t11b1_decode_tangent( head[ i ], normal );
    }
  }
  return value;
}

std::uint32_t meshlet_reader::get_face_count() const {
  return vertex_count / get_face_vertex_count( buffer.topology );
}

face_attribute meshlet_reader::get_position(
  std::uint32_t local_face_id
) const {
  if( !valid ) return face_attribute{};
  if( local_face_id >= get_face_count() ) return face_attribute{};
  face_attribute f;
  const std::uint32_t fvc = get_face_vertex_count( buffer.topology );
  f.vertex.resize( fvc );
  f.primitive_id = vertex_offset / fvc + local_face_id;
  if(
    buffer.topology == vk::PrimitiveTopology::eTriangleList &&
    buffer.attribute.find( vertex_attribute_id::position ) != buffer.attribute.end() &&
    buffer.attribute.at( vertex_attribute_id::position ).type == scene_graph::accessor_type_id::dgf
  ) {
    for( unsigned int j = 0u; j != fvc; ++j ) {
      const auto vertex_index = read_index( buffer, vertex_offset + local_face_id * fvc + j );
      f.vertex[ j ].index = vertex_index;
      f.vertex[ j ].position = dgf_verts[ local_face_id * 3u + j ];
    }
  }
  else {
    for( unsigned int j = 0u; j != fvc; ++j ) {
      const auto vertex_index = read_index( buffer, vertex_offset + local_face_id * fvc + j );
      f.vertex[ j ].index = vertex_index;
      f.vertex[ j ].position = read_vertex( buffer, vertex_attribute_id::position, vertex_index );
    }
  }
  f.valid = true;
  return f;
}


face_attribute meshlet_reader::operator()(
  std::uint32_t local_face_id
) const {
  if( !valid ) return face_attribute{};
  if( local_face_id >= get_face_count() ) return face_attribute{};
  face_attribute f = get_position( local_face_id );
  const std::uint32_t fvc = get_face_vertex_count( buffer.topology );

  if( buffer.attribute.find( vertex_attribute_id::normal ) != buffer.attribute.end() ) {
    for( unsigned int j = 0u; j != fvc; ++j ) {
      const auto vertex_index = read_index( buffer, vertex_offset + local_face_id * fvc + j );
      f.vertex[ j ].normal = read_vertex( buffer, vertex_attribute_id::normal, vertex_index );
    }
    if( buffer.attribute.find( vertex_attribute_id::tangent ) != buffer.attribute.end() ) {
      for( unsigned int j = 0u; j != fvc; ++j ) {
        const auto vertex_index = read_index( buffer, vertex_offset + local_face_id * fvc + j );
        f.vertex[ j ].tangent = read_vertex( buffer, vertex_attribute_id::tangent, vertex_index );
      }
    }
    else if( buffer.attribute.at( vertex_attribute_id::normal ).type == scene_graph::accessor_type_id::n21t11 ) {
      for( unsigned int j = 0u; j != fvc; ++j ) {
        const auto vertex_index = read_index( buffer, vertex_offset + local_face_id * fvc + j );
        f.vertex[ j ].tangent = read_vertex( buffer, vertex_attribute_id::tangent, vertex_index );
      }
    }
    else if( buffer.attribute.at( vertex_attribute_id::normal ).type == scene_graph::accessor_type_id::n20t11b1 ) {
      for( unsigned int j = 0u; j != fvc; ++j ) {
        const auto vertex_index = read_index( buffer, vertex_offset + local_face_id * fvc + j );
        f.vertex[ j ].tangent = read_vertex( buffer, vertex_attribute_id::tangent, vertex_index );
      }
    }
  }

  for( unsigned int j = 0u; j != fvc; ++j ) {
    const auto vertex_index = read_index( buffer, vertex_offset + local_face_id * fvc + j );
    f.vertex[ j ].tex_coord0 = read_vertex( buffer, vertex_attribute_id::texcoord_0, vertex_index );
    f.vertex[ j ].color0 = read_vertex( buffer, vertex_attribute_id::color_0, vertex_index );
    f.vertex[ j ].joint0 = read_vertex( buffer, vertex_attribute_id::joint_0, vertex_index );
    f.vertex[ j ].weight0 = read_vertex( buffer, vertex_attribute_id::weight_0, vertex_index );
    f.vertex[ j ].lod_morph = read_vertex( buffer, vertex_attribute_id::lod_morph, vertex_index );
  }

  f.valid = true;

  return f;
}

void convert_to_dgf(
  loaded_vertex_buffer &buffer,
  std::uint32_t bit_width
) {
  if( buffer.attribute.find( vertex_attribute_id::position ) != buffer.attribute.end() ) {
    if( buffer.attribute.at( vertex_attribute_id::position ).type == scene_graph::accessor_type_id::dgf ) return;

    DGFBaker::Config config = {};
    config.blockMaxTris = 32u;
    config.blockMaxVerts = 96u;
    config.targetBitWidth = bit_width;
    config.generateTriangleRemap = true;
    config.enableUserData = true;
    DGFBaker::Baker baker( config );
 
    std::vector< std::uint32_t > index;
    index.reserve( buffer.unique_vertex_count );
    for( std::uint32_t i = 0u; i != buffer.unique_vertex_count; ++i ) {
      index.push_back( read_index( buffer, i ) );
    }
    std::vector< float > vertex;
    vertex.reserve( buffer.vertex_count * 3u );
    for( std::uint32_t i = 0u; i != buffer.vertex_count; ++i ) {
      const auto v = read_vertex( buffer.attribute.at( vertex_attribute_id::position ), i );
      vertex.push_back( v[ 0 ] );
      vertex.push_back( v[ 1 ] );
      vertex.push_back( v[ 2 ] );
    }

    DGFBaker::BakerMesh mesh( vertex.data(), index.data(), vertex.size()/3u, index.size()/3u );
    DGFBaker::BakerOutput output = baker.BakeDGF( mesh );
 
    std::uint32_t offset = 0u;
    for( std::size_t i = 0u; i < output.dgfBlocks.size() / 128u; ++i ) {
      std::uint8_t *head = output.dgfBlocks.data() + i * 128u;
      DGF::MetaData meta;
      DGF::DecodeMetaData( &meta, head );
      DGF::WriteUserData( head, &offset, 0, sizeof( std::uint32_t ) );
      offset += meta.numTris;
    }

    std::vector< std::uint8_t > remap;
    scene_graph::accessor_type_id remap_format;

    if( vertex.size() < 256u ) {
      remap_format = scene_graph::accessor_type_id::u8;
      remap.resize( sizeof( std::uint8_t ) * output.triangleRemap.size() * 3u );
      auto cur = reinterpret_cast< std::uint8_t* >( remap.data() );
      for( const auto &r: output.triangleRemap ) {
        for( unsigned int v = 0u; v != 3u; ++v ) {
          const auto j = r.InputPrimIndex * 3u + r.IndexRotation[ v ];
          if( j >= index.size() ) throw gct::exception::invalid_argument( "範囲外の頂点インデックスが参照されている", __FILE__, __LINE__ );
          const auto i = index[ j ];
          if( i >= vertex.size() ) throw gct::exception::invalid_argument( "範囲外の頂点が参照されている", __FILE__, __LINE__ );
          *cur = i;
          ++cur;
        }
      }
    }
    else if( vertex.size() < 65536u ) {
      remap_format = scene_graph::accessor_type_id::u16;
      remap.resize( sizeof( std::uint16_t ) * output.triangleRemap.size() * 3u );
      auto cur = reinterpret_cast< std::uint16_t* >( remap.data() );
      for( const auto &r: output.triangleRemap ) {
        for( unsigned int v = 0u; v != 3u; ++v ) {
          const auto j = r.InputPrimIndex * 3u + r.IndexRotation[ v ];
          if( j >= index.size() ) throw gct::exception::invalid_argument( "範囲外の頂点インデックスが参照されている", __FILE__, __LINE__ );
          const auto i = index[ j ];
          if( i >= vertex.size() ) throw gct::exception::invalid_argument( "範囲外の頂点が参照されている", __FILE__, __LINE__ );
          *cur = i;
          ++cur;
        }
      }
    }
    else {
      remap_format = scene_graph::accessor_type_id::u32;
      remap.resize( sizeof( std::uint32_t ) * output.triangleRemap.size() * 3u );
      auto cur = reinterpret_cast< std::uint32_t* >( remap.data() );
      for( const auto &r: output.triangleRemap ) {
        for( unsigned int v = 0u; v != 3u; ++v ) {
          const auto j = r.InputPrimIndex * 3u + r.IndexRotation[ v ];
          if( j >= index.size() ) throw gct::exception::invalid_argument( "範囲外の頂点インデックスが参照されている", __FILE__, __LINE__ );
          const auto i = index[ j ];
          if( i >= vertex.size() ) throw gct::exception::invalid_argument( "範囲外の頂点が参照されている", __FILE__, __LINE__ );
          *cur = i;
          ++cur;
        }
      }
    }

   if( buffer.attribute[ vertex_attribute_id::position ].reuse && !buffer.attribute[ vertex_attribute_id::index ].reuse ) {
     buffer.attribute[ vertex_attribute_id::position ].reuse = std::nullopt;
   }
   if( !buffer.attribute[ vertex_attribute_id::position ].reuse && buffer.attribute[ vertex_attribute_id::index ].reuse ) {
     buffer.attribute[ vertex_attribute_id::index ].reuse = std::nullopt;
   }

    buffer.attribute[ vertex_attribute_id::position ].type = scene_graph::accessor_type_id::dgf;
    buffer.attribute[ vertex_attribute_id::position ].component_count = 1u;
    buffer.attribute[ vertex_attribute_id::position ].block_size = get_block_size( buffer.attribute[ vertex_attribute_id::position ] );
    buffer.attribute[ vertex_attribute_id::position ].data = std::move( output.dgfBlocks );

    buffer.attribute[ vertex_attribute_id::index ].type = remap_format;
    buffer.attribute[ vertex_attribute_id::index ].component_count = 1u;
    buffer.attribute[ vertex_attribute_id::index ].block_size = get_block_size( buffer.attribute[ vertex_attribute_id::index ] );
    buffer.attribute[ vertex_attribute_id::index ].data = std::move( remap );
    
    buffer.meshlet_count = buffer.attribute[ vertex_attribute_id::position ].data.size() / buffer.attribute[ vertex_attribute_id::position ].block_size;
  }
}

void convert_from_dgf(
  loaded_vertex_buffer &buffer
) {
  if( buffer.attribute.find( vertex_attribute_id::position ) != buffer.attribute.end() ) {
    if( buffer.attribute.at( vertex_attribute_id::position ).type != scene_graph::accessor_type_id::dgf ) return;
    loaded_vertex_attribute_buffer converted;
    converted.component_count = 3u;
    converted.block_size = get_block_size( converted );
    resize_attribute_buffer( converted, buffer.vertex_count );
    const auto fvc = get_face_vertex_count( buffer.topology );
    const auto head = reinterpret_cast< float* >( converted.data.data() );
    for( std::uint32_t m = 0u; m != buffer.meshlet_count; ++m ) {
      gct::gltf::meshlet_reader reader( buffer, m );
      for( std::uint32_t i = 0u; i != reader.get_face_count(); ++i ) {
        const auto f = reader.get_position( i );
        if( f.valid ) {
          for( std::uint32_t i = 0u; i != fvc; ++i ) {
            for( std::uint32_t j = 0u; j != converted.component_count; ++j ) {
              head[ f.vertex[ i ].index * converted.component_count + j ] = f.vertex[ i ].position[ j ];
            }
          }
        }
      }
    }
    buffer.attribute[ vertex_attribute_id::position ] = converted;
    buffer.meshlet_count = buffer.unique_vertex_count / 96u + ( ( buffer.unique_vertex_count % 96u ) ? 1u : 0u );
  }
}

void convert_to_n31(
  loaded_vertex_buffer &buffer
) {
  if( buffer.attribute.find( vertex_attribute_id::normal ) != buffer.attribute.end() ) {
    std::vector< std::uint8_t > encoded( buffer.vertex_count * sizeof( std::uint32_t ) );
    for( std::uint32_t i = 0u; i != buffer.vertex_count; ++i ) {
      const glm::vec4 normal = read_vertex( buffer, vertex_attribute_id::normal, i );
      reinterpret_cast< std::uint32_t* >( encoded.data() )[ i ] = gct::n31_encode( normal );
    }
    buffer.attribute[ vertex_attribute_id::normal ].block_size = sizeof( std::uint32_t );
    buffer.attribute[ vertex_attribute_id::normal ].component_count = 1u;
    buffer.attribute[ vertex_attribute_id::normal ].type = scene_graph::accessor_type_id::n31;
    buffer.attribute[ vertex_attribute_id::normal ].data = std::move( encoded );
  }
}

void convert_to_t31(
  loaded_vertex_buffer &buffer
) {
  if(
    buffer.attribute.find( vertex_attribute_id::tangent ) != buffer.attribute.end() ||
    ( buffer.attribute.find( vertex_attribute_id::normal ) != buffer.attribute.end() && buffer.attribute.at( vertex_attribute_id::normal ).type == scene_graph::accessor_type_id::n21t11 ) ||
    ( buffer.attribute.find( vertex_attribute_id::normal ) != buffer.attribute.end() && buffer.attribute.at( vertex_attribute_id::normal ).type == scene_graph::accessor_type_id::n20t11b1 )
  ) {
    std::vector< std::uint8_t > encoded( buffer.vertex_count * sizeof( std::uint32_t ) );
    for( std::uint32_t i = 0u; i != buffer.vertex_count; ++i ) {
      const glm::vec4 tangent = read_vertex( buffer, vertex_attribute_id::tangent, i );
      reinterpret_cast< std::uint32_t* >( encoded.data() )[ i ] = gct::n31_encode( tangent );
    }
    buffer.attribute[ vertex_attribute_id::tangent ].block_size = sizeof( std::uint32_t );
    buffer.attribute[ vertex_attribute_id::tangent ].component_count = 1u;
    buffer.attribute[ vertex_attribute_id::tangent ].type = scene_graph::accessor_type_id::n31;
    buffer.attribute[ vertex_attribute_id::tangent ].data = std::move( encoded );
  }
}

void convert_to_n21t11(
  loaded_vertex_buffer &buffer
) {
  if(
    ( buffer.attribute.find( vertex_attribute_id::normal ) != buffer.attribute.end() && buffer.attribute.find( vertex_attribute_id::tangent ) != buffer.attribute.end() ) ||
    ( buffer.attribute.find( vertex_attribute_id::normal ) != buffer.attribute.end() && buffer.attribute.at( vertex_attribute_id::normal ).type == scene_graph::accessor_type_id::n21t11 ) ||
    ( buffer.attribute.find( vertex_attribute_id::normal ) != buffer.attribute.end() && buffer.attribute.at( vertex_attribute_id::normal ).type == scene_graph::accessor_type_id::n20t11b1 )
  ) {
    std::vector< std::uint8_t > encoded( buffer.vertex_count * sizeof( std::uint32_t ) );
    for( std::uint32_t i = 0u; i != buffer.vertex_count; ++i ) {
      const glm::vec4 normal = read_vertex( buffer, vertex_attribute_id::normal, i );
      const glm::vec4 tangent = read_vertex( buffer, vertex_attribute_id::tangent, i );
      reinterpret_cast< std::uint32_t* >( encoded.data() )[ i ] = gct::n21t11_encode( normal, tangent );
    }
    buffer.attribute[ vertex_attribute_id::normal ].block_size = sizeof( std::uint32_t );
    buffer.attribute[ vertex_attribute_id::normal ].component_count = 1u;
    buffer.attribute[ vertex_attribute_id::normal ].type = scene_graph::accessor_type_id::n21t11;
    buffer.attribute[ vertex_attribute_id::normal ].data = std::move( encoded );
    buffer.attribute.erase( vertex_attribute_id::tangent );
  }
}

void convert_to_n20t11b1(
  loaded_vertex_buffer &buffer
) {
  if(
    ( buffer.attribute.find( vertex_attribute_id::normal ) != buffer.attribute.end() && buffer.attribute.find( vertex_attribute_id::tangent ) != buffer.attribute.end() ) ||
    ( buffer.attribute.find( vertex_attribute_id::normal ) != buffer.attribute.end() && buffer.attribute.at( vertex_attribute_id::normal ).type == scene_graph::accessor_type_id::n21t11 ) ||
    ( buffer.attribute.find( vertex_attribute_id::normal ) != buffer.attribute.end() && buffer.attribute.at( vertex_attribute_id::normal ).type == scene_graph::accessor_type_id::n20t11b1 )
  ) {
    std::vector< std::uint8_t > encoded( buffer.vertex_count * sizeof( std::uint32_t ) );
    for( std::uint32_t i = 0u; i != buffer.vertex_count; ++i ) {
      const glm::vec4 normal = read_vertex( buffer, vertex_attribute_id::normal, i );
      const glm::vec4 tangent = read_vertex( buffer, vertex_attribute_id::tangent, i );
      reinterpret_cast< std::uint32_t* >( encoded.data() )[ i ] = gct::n20t11b1_encode( normal, tangent );
    }
    buffer.attribute[ vertex_attribute_id::normal ].block_size = sizeof( std::uint32_t );
    buffer.attribute[ vertex_attribute_id::normal ].component_count = 1u;
    buffer.attribute[ vertex_attribute_id::normal ].type = scene_graph::accessor_type_id::n20t11b1;
    buffer.attribute[ vertex_attribute_id::normal ].data = std::move( encoded );
    buffer.attribute.erase( vertex_attribute_id::tangent );
  }
}

void convert_to_float(
  loaded_vertex_buffer &buffer,
  vertex_attribute_id attr_id
) {
  if( buffer.attribute.find( attr_id ) != buffer.attribute.end() ) {
    if( buffer.attribute.at( attr_id ).type == scene_graph::accessor_type_id::dgf ) {
      throw exception::invalid_argument( "convert_to_float : DGF must be decoded first", __FILE__, __LINE__ );
    }
    const std::uint32_t component_count = get_decoded_component_count( buffer, attr_id );
    std::vector< std::uint8_t > encoded( buffer.vertex_count * sizeof( float ) * component_count );
    for( std::uint32_t i = 0u; i != buffer.vertex_count; ++i ) {
      const glm::vec4 attr = read_vertex( buffer, attr_id, i );
      for( unsigned int j = 0u; j != component_count; ++j ) {
        reinterpret_cast< float* >( encoded.data() )[ i * component_count + j ] = attr[ j ];
      }
    }
    buffer.attribute[ attr_id ].block_size = sizeof( float ) * component_count;
    buffer.attribute[ attr_id ].component_count = component_count;
    buffer.attribute[ attr_id ].type = scene_graph::accessor_type_id::float_;
    buffer.attribute[ attr_id ].data = std::move( encoded );
  }
}

void convert_to_half(
  loaded_vertex_buffer &buffer,
  vertex_attribute_id attr_id
) {
  if( buffer.attribute.find( attr_id ) != buffer.attribute.end() ) {
    if( buffer.attribute.at( attr_id ).type == scene_graph::accessor_type_id::dgf ) {
      throw exception::invalid_argument( "convert_to_half : DGF must be decoded first", __FILE__, __LINE__ );
    }
    const std::uint32_t component_count = get_decoded_component_count( buffer, attr_id );
    std::vector< std::uint8_t > encoded( buffer.vertex_count * sizeof( std::uint16_t ) * component_count );
    for( std::uint32_t i = 0u; i != buffer.vertex_count; ++i ) {
      const glm::vec4 attr = read_vertex( buffer, attr_id, i );
      for( unsigned int j = 0u; j != component_count; ++j ) {
        reinterpret_cast< half_float::half* >( encoded.data() )[ i * component_count + j ] = attr[ j ];
      }
    }
    buffer.attribute[ attr_id ].block_size = sizeof( std::uint16_t ) * component_count;
    buffer.attribute[ attr_id ].component_count = component_count;
    buffer.attribute[ attr_id ].type = scene_graph::accessor_type_id::half;
    buffer.attribute[ attr_id ].data = std::move( encoded );
  }
}

void convert_to_fixed(
  loaded_vertex_buffer &buffer,
  vertex_attribute_id attr_id
) {
  if( buffer.attribute.find( attr_id ) != buffer.attribute.end() ) {
    if( buffer.attribute.at( attr_id ).type == scene_graph::accessor_type_id::dgf ) {
      throw exception::invalid_argument( "convert_to_fixed : DGF must be decoded first", __FILE__, __LINE__ );
    }
    const std::uint32_t component_count = get_decoded_component_count( buffer, attr_id );
    std::vector< std::uint8_t > encoded( buffer.vertex_count * sizeof( std::int16_t ) * component_count );
    for( std::uint32_t i = 0u; i != buffer.vertex_count; ++i ) {
      const glm::vec4 attr = read_vertex( buffer, attr_id, i );
      for( unsigned int j = 0u; j != component_count; ++j ) {
        reinterpret_cast< std::int16_t* >( encoded.data() )[ i * component_count + j ] = encode_fixed( attr[ j ] );
      }
    }
    buffer.attribute[ attr_id ].block_size = sizeof( std::int16_t ) * component_count;
    buffer.attribute[ attr_id ].component_count = component_count;
    buffer.attribute[ attr_id ].type = scene_graph::accessor_type_id::fixed;
    buffer.attribute[ attr_id ].data = std::move( encoded );
  }
}

void generate_tangent(
  loaded_vertex_buffer &buffer
) {
  if(
    buffer.attribute.find( vertex_attribute_id::position ) != buffer.attribute.end() &&
    buffer.attribute.find( vertex_attribute_id::texcoord_0 ) != buffer.attribute.end()
  ) {
    std::vector< std::uint32_t > count( buffer.vertex_count );
    std::vector< std::uint8_t > encoded( buffer.vertex_count * sizeof( float ) * 4u, 0u );
    for( std::uint32_t m = 0u; m != buffer.meshlet_count; ++m ) {
      gct::gltf::meshlet_reader reader( buffer, m );
      for( std::uint32_t i = 0u; i != reader.get_face_count(); ++i ) {
        const auto f = reader( i );
        if( f.valid ) {
          glm::vec3 tangent = mikktspace(
            f.vertex[ 0 ].position,
            f.vertex[ 1 ].position,
            f.vertex[ 2 ].position,
            f.vertex[ 0 ].tex_coord0,
            f.vertex[ 1 ].tex_coord0,
            f.vertex[ 2 ].tex_coord0
          );

          if( tangent != glm::vec3( 0.0f, 0.0f, 0.0f ) ) {
            for( unsigned int j = 0u; j != 3u; ++j ) {
              reinterpret_cast< float* >( encoded.data() )[ f.vertex[ 0 ].index * 4u + j ] += tangent[ j ];
              reinterpret_cast< float* >( encoded.data() )[ f.vertex[ 1 ].index * 4u + j ] += tangent[ j ];
              reinterpret_cast< float* >( encoded.data() )[ f.vertex[ 2 ].index * 4u + j ] += tangent[ j ];
            }
            reinterpret_cast< float* >( encoded.data() )[ f.vertex[ 0 ].index * 4u + 3u ] = 1.0f;
            reinterpret_cast< float* >( encoded.data() )[ f.vertex[ 1 ].index * 4u + 3u ] = 1.0f;
            reinterpret_cast< float* >( encoded.data() )[ f.vertex[ 2 ].index * 4u + 3u ] = 1.0f;
            ++count[ f.vertex[ 0 ].index ];
            ++count[ f.vertex[ 1 ].index ];
            ++count[ f.vertex[ 2 ].index ];
          }
        }
      }
    }
    for( unsigned int i = 0u; i != count.size(); ++i ) {
      if( count[ i ] != 0u ) {
        for( unsigned int j = 0u; j != 3u; ++j ) {
          reinterpret_cast< float* >( encoded.data() )[ i * 4u + j ] /= count[ i ];
        }
      }
    }
    for( unsigned int i = 0u; i != count.size(); ++i ) {
      glm::vec3 temp = glm::vec3(
        reinterpret_cast< float* >( encoded.data() )[ i * 4u + 0u ],
        reinterpret_cast< float* >( encoded.data() )[ i * 4u + 1u ],
        reinterpret_cast< float* >( encoded.data() )[ i * 4u + 2u ] 
      );
      if( temp != glm::vec3( 0.0f, 0.0f, 0.0f ) ) {
        temp = glm::normalize( temp );
      }
      for( unsigned int j = 0u; j != 3u; ++j ) {
        reinterpret_cast< float* >( encoded.data() )[ i * 4u + j ] = temp[ j ];
      }
    }
    buffer.attribute[ vertex_attribute_id::tangent ].block_size = sizeof( float ) * 4u;
    buffer.attribute[ vertex_attribute_id::tangent ].component_count = 4u;
    buffer.attribute[ vertex_attribute_id::tangent ].type = scene_graph::accessor_type_id::float_;
    buffer.attribute[ vertex_attribute_id::tangent ].data = std::move( encoded );
  }
}

void generate_minmax(
  loaded_vertex_buffer &buffer
) {
  for( auto &[key,attr]: buffer.attribute ) {
    attr.min = glm::vec4( 0, 0, 0, 0 );
    attr.max = glm::vec4( 0, 0, 0, 0 );
    const std::uint32_t component_count = get_decoded_component_count( buffer, key );
    for( unsigned int j = 0u; j != component_count; ++j ) {
      attr.min[ j ] = std::numeric_limits< float >::max();
      attr.max[ j ] = -std::numeric_limits< float >::max();
    }
  }
  for( std::uint32_t m = 0u; m != buffer.meshlet_count; ++m ) {
    gct::gltf::meshlet_reader reader( buffer, m );
    for( std::uint32_t i = 0u; i != reader.get_face_count(); ++i ) {
      const auto f = reader( i );
      if( f.valid ) {
        for( const auto &v: f.vertex ) {
          if( buffer.attribute.find( vertex_attribute_id::position ) != buffer.attribute.end() ) {
            const std::uint32_t component_count = get_decoded_component_count( buffer, vertex_attribute_id::position );
            for( unsigned int j = 0u; j != component_count; ++j ) {
              buffer.attribute.at( vertex_attribute_id::position ).min[ j ] =
               std::min( buffer.attribute.at( vertex_attribute_id::position ).min[ j ],  v.position[ j ] );
              buffer.attribute.at( vertex_attribute_id::position ).max[ j ] =
               std::max( buffer.attribute.at( vertex_attribute_id::position ).max[ j ],  v.position[ j ] );
            }
          }
          if( buffer.attribute.find( vertex_attribute_id::normal ) != buffer.attribute.end() ) {
            const std::uint32_t component_count = get_decoded_component_count( buffer, vertex_attribute_id::normal );
            for( unsigned int j = 0u; j != component_count; ++j ) {
              buffer.attribute.at( vertex_attribute_id::normal ).min[ j ] =
               std::min( buffer.attribute.at( vertex_attribute_id::normal ).min[ j ],  v.normal[ j ] );
              buffer.attribute.at( vertex_attribute_id::normal ).max[ j ] =
               std::max( buffer.attribute.at( vertex_attribute_id::normal ).max[ j ],  v.normal[ j ] );
            }
          }
          if( buffer.attribute.find( vertex_attribute_id::tangent ) != buffer.attribute.end() ) {
            const std::uint32_t component_count = get_decoded_component_count( buffer, vertex_attribute_id::tangent );
            for( unsigned int j = 0u; j != component_count; ++j ) {
              buffer.attribute.at( vertex_attribute_id::tangent ).min[ j ] =
               std::min( buffer.attribute.at( vertex_attribute_id::tangent ).min[ j ],  v.tangent[ j ] );
              buffer.attribute.at( vertex_attribute_id::tangent ).max[ j ] =
               std::max( buffer.attribute.at( vertex_attribute_id::tangent ).max[ j ],  v.tangent[ j ] );
            }
          }
          if( buffer.attribute.find( vertex_attribute_id::texcoord_0 ) != buffer.attribute.end() ) {
            const std::uint32_t component_count = get_decoded_component_count( buffer, vertex_attribute_id::texcoord_0 );
            for( unsigned int j = 0u; j != component_count; ++j ) {
              buffer.attribute.at( vertex_attribute_id::texcoord_0 ).min[ j ] =
               std::min( buffer.attribute.at( vertex_attribute_id::texcoord_0 ).min[ j ],  v.tex_coord0[ j ] );
              buffer.attribute.at( vertex_attribute_id::texcoord_0 ).max[ j ] =
               std::max( buffer.attribute.at( vertex_attribute_id::texcoord_0 ).max[ j ],  v.tex_coord0[ j ] );
            }
          }
          if( buffer.attribute.find( vertex_attribute_id::color_0 ) != buffer.attribute.end() ) {
            const std::uint32_t component_count = get_decoded_component_count( buffer, vertex_attribute_id::color_0 );
            for( unsigned int j = 0u; j != component_count; ++j ) {
              buffer.attribute.at( vertex_attribute_id::color_0 ).min[ j ] =
               std::min( buffer.attribute.at( vertex_attribute_id::color_0 ).min[ j ],  v.color0[ j ] );
              buffer.attribute.at( vertex_attribute_id::color_0 ).max[ j ] =
               std::max( buffer.attribute.at( vertex_attribute_id::color_0 ).max[ j ],  v.color0[ j ] );
            }
          }
          if( buffer.attribute.find( vertex_attribute_id::joint_0 ) != buffer.attribute.end() ) {
            const std::uint32_t component_count = get_decoded_component_count( buffer, vertex_attribute_id::joint_0 );
            for( unsigned int j = 0u; j != component_count; ++j ) {
              buffer.attribute.at( vertex_attribute_id::joint_0 ).min[ j ] =
               std::min( buffer.attribute.at( vertex_attribute_id::joint_0 ).min[ j ],  v.joint0[ j ] );
              buffer.attribute.at( vertex_attribute_id::joint_0 ).max[ j ] =
               std::max( buffer.attribute.at( vertex_attribute_id::joint_0 ).max[ j ],  v.joint0[ j ] );
            }
          }
          if( buffer.attribute.find( vertex_attribute_id::weight_0 ) != buffer.attribute.end() ) {
            const std::uint32_t component_count = get_decoded_component_count( buffer, vertex_attribute_id::weight_0 );
            for( unsigned int j = 0u; j != component_count; ++j ) {
              buffer.attribute.at( vertex_attribute_id::weight_0 ).min[ j ] =
               std::min( buffer.attribute.at( vertex_attribute_id::weight_0 ).min[ j ],  v.weight0[ j ] );
              buffer.attribute.at( vertex_attribute_id::weight_0 ).max[ j ] =
               std::max( buffer.attribute.at( vertex_attribute_id::weight_0 ).max[ j ],  v.weight0[ j ] );
            }
          }
        }
      }
    }
  }
  if( buffer.attribute.find( vertex_attribute_id::index ) != buffer.attribute.end() ) {
    for( std::uint32_t i = 0u; i != buffer.unique_vertex_count; ++i ) {
      const float index = read_index( buffer, i );
      buffer.attribute.at( vertex_attribute_id::index ).min[ 0 ] =
        std::min( buffer.attribute.at( vertex_attribute_id::index ).min[ 0 ], index );
      buffer.attribute.at( vertex_attribute_id::index ).max[ 0 ] =
        std::max( buffer.attribute.at( vertex_attribute_id::index ).max[ 0 ], index );
    }
  }
}

void drop_reuse(
  loaded_vertex_buffer &buffer
) {
  for( auto &[key,attr]: buffer.attribute ) {
    attr.reuse = std::nullopt;
  }
}

void meshlet_statistics(
  const loaded_mesh &mesh
) {
  std::uint32_t min = 0u;
  std::uint32_t max = 0;
  float average = 0.0f;
  bool initial =true;
  std::uint32_t meshlet_count = 0u;
  for( auto &[key,buffer]: mesh ) {
    for( std::uint32_t m = 0u; m != buffer.meshlet_count; ++m ) {
      gct::gltf::meshlet_reader reader( buffer, m );
      if( initial ) {
        min = reader.get_face_count();
        max = reader.get_face_count();
        average = reader.get_face_count();
        meshlet_count = 1u;
        initial = false;
      }
      else {
        min = std::min( min, reader.get_face_count() );
        max = std::max( max, reader.get_face_count() );
        average = ( average * meshlet_count + reader.get_face_count() )/( meshlet_count + 1u );
        meshlet_count += 1u;
      }
    }
  }
  std::cout << "メッシュレット統計" << std::endl;
  std::cout << "  最小 : " << min << std::endl;
  std::cout << "  最大 : " << max << std::endl;
  std::cout << "  平均 : " << average << std::endl;
  std::cout << "  メッシュレットの数 : " << meshlet_count << std::endl;
}

void mesh_statistics(
  const loaded_mesh &mesh
) {
  std::uint32_t vertex_count = 0u;
  std::uint32_t unique_vertex_count = 0;
  std::uint32_t meshlet_count = 0u;
  for( auto &[key,buffer]: mesh ) {
    vertex_count += buffer.vertex_count;
    unique_vertex_count += buffer.unique_vertex_count;
  }
  std::cout << "メッシュ統計" << std::endl;
  std::cout << "  頂点数 : " << vertex_count << std::endl;
  std::cout << "  ユニーク頂点数 : " << unique_vertex_count << std::endl;
}

void dump_vertex(
  const loaded_mesh &mesh
) {
  for( auto &[key,vb] : mesh ) {
    std::cout << "primitive " << key.mesh_id << " " << key.primitive_id << std::endl;
    std::cout << "  vertex_count=" << vb.vertex_count <<
                  " unique_vertex_count=" << vb.unique_vertex_count <<
                  " meshlet_count=" << vb.meshlet_count <<
                  std::endl;
    for( const auto &[attr_id,attr]: vb.attribute ) {
      std::cout << "  " << nlohmann::json( attr_id ) << " min=(" <<
        attr.min[ 0 ] << "," << attr.min[ 1 ] << "," << attr.min[ 2 ] << "," << attr.min[ 3 ] << ") max=(" <<
        attr.max[ 0 ] << "," << attr.max[ 1 ] << "," << attr.max[ 2 ] << "," << attr.max[ 3 ] << ") size=" <<
        attr.data.size() <<
        std::endl;
    }
    for( std::uint32_t m = 0u; m != vb.meshlet_count; ++m ) {
      gct::gltf::meshlet_reader reader( vb, m );
      std::cout << "  meshlet " << m << " face_count=" << reader.get_face_count() << std::endl;
      for( std::uint32_t i = 0u; i != reader.get_face_count(); ++i ) {
        const auto f = reader( i );
        if( f.valid ) {
          std::cout << "    face " << i << std::endl;
          for( std::uint32_t j = 0u; j != f.vertex.size(); ++j ) {
            std::cout << "      ";
            if( vb.attribute.find( gct::vertex_attribute_id::position ) != vb.attribute.end() ) {
              std::cout << "p(" <<
                           f.vertex[ j ].position.x << "," <<
                           f.vertex[ j ].position.y << "," <<
                           f.vertex[ j ].position.z << ")";
            }
            if( vb.attribute.find( gct::vertex_attribute_id::normal ) != vb.attribute.end() ) {
              std::cout << "n(" <<
                           f.vertex[ j ].normal.x << "," <<
                           f.vertex[ j ].normal.y << "," <<
                           f.vertex[ j ].normal.z << ")";
            }
            if( vb.attribute.find( gct::vertex_attribute_id::tangent ) != vb.attribute.end() ) {
              std::cout << "t(" <<
                           f.vertex[ j ].tangent.x << "," <<
                           f.vertex[ j ].tangent.y << "," <<
                           f.vertex[ j ].tangent.z << "," <<
                           f.vertex[ j ].tangent.w << ")";
            }
            else if(
              vb.attribute.find( gct::vertex_attribute_id::normal ) != vb.attribute.end() &&
              vb.attribute.at( gct::vertex_attribute_id::normal ).type == gct::scene_graph::accessor_type_id::n21t11
            ) {
              std::cout << "t(" <<
                           f.vertex[ j ].tangent.x << "," <<
                           f.vertex[ j ].tangent.y << "," <<
                           f.vertex[ j ].tangent.z << "," <<
                           f.vertex[ j ].tangent.w << ")";
            }
            else if(
              vb.attribute.find( gct::vertex_attribute_id::normal ) != vb.attribute.end() &&
              vb.attribute.at( gct::vertex_attribute_id::normal ).type == gct::scene_graph::accessor_type_id::n20t11b1
            ) {
              std::cout << "t(" <<
                           f.vertex[ j ].tangent.x << "," <<
                           f.vertex[ j ].tangent.y << "," <<
                           f.vertex[ j ].tangent.z << "," <<
                           f.vertex[ j ].tangent.w << ")";
            }
            if( vb.attribute.find( gct::vertex_attribute_id::texcoord_0 ) != vb.attribute.end() ) {
              std::cout << "u(" <<
                           f.vertex[ j ].tex_coord0.x << "," <<
                           f.vertex[ j ].tex_coord0.y << ")" <<
                           std::endl;
            }
            if( vb.attribute.find( gct::vertex_attribute_id::color_0 ) != vb.attribute.end() ) {
              std::cout << "c(" <<
                           f.vertex[ j ].color0.x << "," <<
                           f.vertex[ j ].color0.y << "," <<
                           f.vertex[ j ].color0.z << "," <<
                           f.vertex[ j ].color0.w << ")" <<
                           std::endl;
            }
            if( vb.attribute.find( gct::vertex_attribute_id::joint_0 ) != vb.attribute.end() ) {
              std::cout << "j(" <<
                           f.vertex[ j ].joint0.x << "," <<
                           f.vertex[ j ].joint0.y << "," <<
                           f.vertex[ j ].joint0.z << "," <<
                           f.vertex[ j ].joint0.w << ")" <<
                           std::endl;
            }
            if( vb.attribute.find( gct::vertex_attribute_id::weight_0 ) != vb.attribute.end() ) {
              std::cout << "j(" <<
                           f.vertex[ j ].weight0.x << "," <<
                           f.vertex[ j ].weight0.y << "," <<
                           f.vertex[ j ].weight0.z << "," <<
                           f.vertex[ j ].weight0.w << ")" <<
                           std::endl;
            }
          }
        }
      }
    }
  }
}

void dedup(
  loaded_mesh &mesh
) {
  std::unordered_multimap< std::uint32_t, accessor_link > crcs;
  for( auto &[key,buffer]: mesh ) {
    for( auto &[attr_id,attr]: buffer.attribute ) {
      if( !attr.reuse ) {
        auto self = accessor_link()
          .set_mesh_id( key.mesh_id )
          .set_primitive_id( key.primitive_id )
          .set_attr_id( attr_id );
        boost::crc_32_type crc;
        crc.process_bytes( attr.data.data(), attr.data.size() );
        std::uint32_t crc_value = crc.checksum();
        const auto [cand_begin,cand_end] = crcs.equal_range( crc_value );
        bool reused = false;
        for( auto cur = cand_begin; cur != cand_end; ++cur ) {
          auto &cand_data = mesh.at( primitive_key().set_mesh_id( cur->second.mesh_id ).set_primitive_id( cur->second.primitive_id ) ).attribute.at( cur->second.attr_id ).data;
          if( std::equal( attr.data.begin(), attr.data.end(), cand_data.begin(), cand_data.end() ) ) {
            attr.reuse = cur->second;
            reused = true;
            std::cout << "reuse " << cur->second.mesh_id << "." << cur->second.primitive_id << "." << nlohmann::json( cur->second.attr_id ) << " = " <<  key.mesh_id << "." << key.primitive_id << "." << nlohmann::json( attr_id ) << std::endl;
            break;
          }
        }
        if( !reused ) {
          crcs.insert(
            std::make_pair(
               crc_value,
               accessor_link()
                 .set_mesh_id( key.mesh_id )
                 .set_primitive_id( key.primitive_id )
                 .set_attr_id( attr_id )
            )
          );
        }
      }
    }
  }
}

void dump_attribute_size(
  loaded_mesh &mesh
) {
  std::unordered_map< vertex_attribute_id, std::size_t > bytes;
  for( auto &[key,buffer]: mesh ) {
    for( auto &[attr_id,attr]: buffer.attribute ) {
      if( !attr.reuse ) {
        if( bytes.find( attr_id ) != bytes.end() ) {
          bytes[ attr_id ] += attr.data.size();
        }
        else {
          bytes[ attr_id ] = attr.data.size();
        }
      }
    }
  }
  std::cout << "属性毎のサイズ" << std::endl;
  for( auto &[key,value]: bytes ) {
    std::cout << "  " << nlohmann::json( key ) << " : " << value << std::endl;
  }
}

void convert_mesh( loaded_mesh &mesh, const std::vector< std::string > &command ) {
  for( auto &[key,vb]: mesh ) {
    for( const auto &c: command ) {
      if( c.substr( 0, 4 ) == "dgf:" ) {
        const auto level_str = c.substr( 4 );
        std::uint32_t level = 0u;
        if( std::from_chars( level_str.data(), level_str.data() + level_str.size(), level ).ptr == level_str.data() + level_str.size() ) {
          gct::gltf::convert_to_dgf( vb, level );
          continue;
        }
      }
      if( c == "undgf" ) {
        gct::gltf::convert_from_dgf( vb );
      }
      else if( c == "n31" ) {
        gct::gltf::convert_to_n31( vb );
      }
      else if( c == "t31" ) {
        gct::gltf::convert_to_t31( vb );
      }
      else if( c == "n21t11" ) {
        gct::gltf::convert_to_n21t11( vb );
      }
      else if( c == "n20t11b1" ) {
        gct::gltf::convert_to_n20t11b1( vb );
      }
      else if( c.substr( 0, 6 ) == "float_" ) {
        gct::gltf::convert_to_float( vb, gct::to_vertex_attribute_id( c.substr( 6 ) ) );
      }
      else if( c.substr( 0, 5 ) == "half_" ) {
        gct::gltf::convert_to_half( vb, gct::to_vertex_attribute_id( c.substr( 5 ) ) );
      }
      else if( c.substr( 0, 6 ) == "fixed_" ) {
        gct::gltf::convert_to_fixed( vb, gct::to_vertex_attribute_id( c.substr( 6 ) ) );
      }
      else if( c == "generate_tangent" ) {
        gct::gltf::generate_tangent( vb );
      }
      else if( c == "drop_reuse" ) {
        gct::gltf::drop_reuse( vb );
      }
    }
    gct::gltf::generate_minmax( vb );
  }
  for( const auto &c: command ) {
    if( c == "meshlet_statistics" ) {
      meshlet_statistics( mesh );
    }
    else if( c == "mesh_statistics" ) {
      mesh_statistics( mesh );
    }
    else if( c == "dump_vertex" ) {
      dump_vertex( mesh );
    }
    else if( c == "dedup" ) {
      dedup( mesh );
    }
    else if( c == "dump_attribute_size" ) {
      dump_attribute_size( mesh );
    }
  }
}

vertex_diff diff(
  const loaded_vertex_buffer &a,
  const loaded_vertex_buffer &b
) {
  if( a.unique_vertex_count != b.unique_vertex_count ) {
    throw exception::invalid_argument( "diff : a.unique_vertex_count != b.unique_vertex_count", __FILE__, __LINE__ );
  }
  throw -1;  
}



}

