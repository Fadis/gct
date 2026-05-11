#include <fx/gltf.h>
#ifdef GCT_ENABLE_DGF
#include <DGF.h>
#endif
#include <gct/scene_graph_accessor.hpp>
#include <gct/gltf.hpp>
#include <gct/gltf2.hpp>
#include <gct/compress_mesh.hpp>
#include <gct/mmaped_file.hpp>
#include <gct/n21t11.hpp>
#include <gct/n20t11b1.hpp>
#include <gct/n31.hpp>
#include <gct/half.hpp>
#include <gct/fixed.hpp>
#include <gct/mikktspace.hpp>

namespace gct::gltf {

std::size_t get_component_size(
  const loaded_vertex_attribute_buffer &attr
) {
  if( attr.type == scene_graph::accessor_type_id::i8 ) return 1u;
  else if( attr.type == scene_graph::accessor_type_id::u8 ) return 1u;
  else if( attr.type == scene_graph::accessor_type_id::i16 ) return 2u;
  else if( attr.type == scene_graph::accessor_type_id::u16 ) return 2u;
  else if( attr.type == scene_graph::accessor_type_id::u32 ) return 4u;
  else if( attr.type == scene_graph::accessor_type_id::float_ ) return 4u;
  else if( attr.type == scene_graph::accessor_type_id::dgf ) return 128u;
  else if( attr.type == scene_graph::accessor_type_id::half ) return 2u;
  else if( attr.type == scene_graph::accessor_type_id::fixed ) return 2u;
  else if( attr.type == scene_graph::accessor_type_id::n21t11 ) return 4u;
  else if( attr.type == scene_graph::accessor_type_id::n20t11b1 ) return 4u;
  else if( attr.type == scene_graph::accessor_type_id::n31 ) return 4u;
  else {
    throw exception::invalid_argument( "get_component_size : 使用できないアクセサの型", __FILE__, __LINE__ );
  }
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

std::uint32_t get_face_vertex_count( vk::PrimitiveTopology topo ) {
  if( topo == vk::PrimitiveTopology::eTriangleList ) return 3u;
  else if( topo == vk::PrimitiveTopology::eLineList ) return 2u;
  else if( topo == vk::PrimitiveTopology::ePointList ) return 1u;
  else {
    throw invalid_gltf( "サポートされていないトポロジ" );
  }
}


scene_graph::accessor_type_id to_accessor_type_id(
  const fx::gltf::Accessor &accessor
) {
  const bool has_extended_type =
    accessor.extensionsAndExtras.find( "extensions" ) != accessor.extensionsAndExtras.end() &&
    accessor.extensionsAndExtras[ "extensions" ].find( "GCT_extended_type" ) != accessor.extensionsAndExtras[ "extensions" ].end() &&
    accessor.extensionsAndExtras[ "extensions" ][ "GCT_extended_type" ].find( "type" ) != accessor.extensionsAndExtras[ "extensions" ][ "GCT_extended_type" ].end();
  if( has_extended_type ) {
    const std::string typestr = accessor.extensionsAndExtras[ "extensions" ][ "GCT_extended_type" ][ "type" ];
    return scene_graph::to_accessor_type_id( typestr );
  }
  else {
    if( accessor.componentType == fx::gltf::Accessor::ComponentType::Byte ) {
      return scene_graph::accessor_type_id::i8;
    }
    else if( accessor.componentType == fx::gltf::Accessor::ComponentType::UnsignedByte ) {
      return scene_graph::accessor_type_id::u8;
    }
    else if( accessor.componentType == fx::gltf::Accessor::ComponentType::Short ) {
      return scene_graph::accessor_type_id::i16;
    }
    else if( accessor.componentType == fx::gltf::Accessor::ComponentType::UnsignedShort ) {
      return scene_graph::accessor_type_id::u16;
    }
    else if( accessor.componentType == fx::gltf::Accessor::ComponentType::UnsignedInt ) {
      return scene_graph::accessor_type_id::u32;
    }
    else if( accessor.componentType == fx::gltf::Accessor::ComponentType::Float ) {
      return scene_graph::accessor_type_id::float_;
    }
    else {
      throw exception::invalid_argument( "to_numeric_type : 使用できないアクセサの型", __FILE__, __LINE__ );
    }
  }
}

std::uint32_t to_accessor_component_count(
  const fx::gltf::Accessor &accessor
) {
  const auto type = to_accessor_type_id( accessor );
  if( type == scene_graph::accessor_type_id::dgf )  return 1u;
  else if( type == scene_graph::accessor_type_id::n21t11 )  return 1u;
  else if( type == scene_graph::accessor_type_id::n20t11b1 )  return 1u;
  else if( type == scene_graph::accessor_type_id::n31 )  return 1u;
  else if( accessor.type == fx::gltf::Accessor::Type::Scalar ) return 1u;
  else if( accessor.type == fx::gltf::Accessor::Type::Vec2 ) return 2u;
  else if( accessor.type == fx::gltf::Accessor::Type::Vec3 ) return 3u;
  else if( accessor.type == fx::gltf::Accessor::Type::Vec4 ) return 4u;
  else {
    throw exception::invalid_argument( "to_accessor_component_count : 使用できないアクセサの型", __FILE__, __LINE__ );
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



loaded_vertex_buffer load_vertex(
  const std::filesystem::path &base_dir,
  const fx::gltf::Document &doc,
  const fx::gltf::Primitive &primitive_
) {
  loaded_vertex_buffer buffer;

  std::uint32_t vertex_count = std::numeric_limits< uint32_t >::max();
  std::uint32_t unique_vertex_count = std::numeric_limits< uint32_t >::max();
  scene_graph::accessor_type_id position_type_id = scene_graph::accessor_type_id::float_;

  for( const auto &[target,index]: primitive_.attributes ) {
    if( doc.accessors.size() <= size_t( index ) ) throw invalid_gltf( "参照されたaccessorsが存在しない", __FILE__, __LINE__ );
    const auto &accessor = doc.accessors[ index ];
    if( accessor.bufferView < 0 || doc.bufferViews.size() <= size_t( accessor.bufferView ) ) throw invalid_gltf( "参照されたbufferViewが存在しない", __FILE__, __LINE__ );
    const auto &view = doc.bufferViews[ accessor.bufferView ];
    const uint32_t default_stride = to_size( accessor.componentType, accessor.type );
    const uint32_t stride = view.byteStride ? view.byteStride : default_stride;
    const uint32_t max_count = ( view.byteLength - ( accessor.byteOffset ) ) / stride;
    if( view.buffer < 0 || doc.buffers.size() <= size_t( view.buffer ) ) throw invalid_gltf( "参照されたbufferが存在しない", __FILE__, __LINE__ );
    if( accessor.count > max_count ) throw invalid_gltf( "指定された要素数に対してbufferViewが小さすぎる" );
  }

  for( const auto &[target,index]: primitive_.attributes ) {
    if( target == "POSITION" ) {
      const auto &accessor = doc.accessors[ index ];
      position_type_id = to_accessor_type_id( accessor );
    }
  }

  std::uint32_t position_block_count = 0u;

  if( position_type_id == scene_graph::accessor_type_id::dgf ) {
    throw -1;
  }
  else {
    for( const auto &[target,index]: primitive_.attributes ) {
      const auto &accessor = doc.accessors[ index ];
      const auto &view = doc.bufferViews[ accessor.bufferView ];
      const uint32_t default_stride = to_size( accessor.componentType, accessor.type );
      const uint32_t stride = view.byteStride ? view.byteStride : default_stride;
      const uint32_t max_count = ( view.byteLength - ( accessor.byteOffset ) ) / stride;
      vertex_count = std::min( vertex_count, accessor.count );
    }
    position_block_count = vertex_count;
  }
 
  for( const auto &[target,index]: primitive_.attributes ) {
    const auto &accessor = doc.accessors[ index ];
    const auto &view = doc.bufferViews[ accessor.bufferView ];
    const std::uint32_t default_stride = to_size( accessor.componentType, accessor.type );
    const std::uint32_t stride = view.byteStride ? view.byteStride : default_stride;
    const std::uint32_t offset = accessor.byteOffset + view.byteOffset;
    const vk::Format vertex_format = to_vulkan_format( accessor.componentType, accessor.type, accessor.normalized );
 
    const std::filesystem::path uri = base_dir / doc.buffers[ view.buffer ].uri;
 
    const auto attr_id = to_vertex_attribute_id( target );
 
    buffer.attribute[ attr_id ].type = to_accessor_type_id( accessor );
    buffer.attribute[ attr_id ].component_count = to_accessor_component_count( accessor );
    buffer.attribute[ attr_id ].block_size = get_block_size( buffer.attribute[ attr_id ] );
    if( !check_accessor_type( attr_id, buffer.attribute[ attr_id ].type, buffer.attribute[ attr_id ].component_count ) ) {
      throw invalid_gltf( "サポートされていない型が設定されている" );
    }
    const uint32_t block_count =
      ( buffer.attribute[ attr_id ].type == scene_graph::accessor_type_id::dgf ) ? position_block_count : vertex_count; 
    resize_attribute_buffer( buffer.attribute[ attr_id ], block_count );
 
    gct::mmaped_file m( uri );
    write_vertex( m.data(), buffer.attribute[ attr_id ], offset, stride, block_count );
  }
 
  if( primitive_.indices >= 0 ) {
    const auto &accessor = doc.accessors[ primitive_.indices ];
    if( accessor.bufferView < 0 || doc.bufferViews.size() <= size_t( accessor.bufferView ) ) throw gct::exception::invalid_argument( "参照されたbufferViewが存在しない", __FILE__, __LINE__ );
    const auto &view = doc.bufferViews[ accessor.bufferView ];
    const std::uint32_t default_stride = gct::gltf::to_size( accessor.componentType, accessor.type );
    const std::uint32_t stride = view.byteStride ? view.byteStride : default_stride;
    const std::uint32_t max_count = ( view.byteLength - ( accessor.byteOffset ) ) / stride;
    if( view.buffer < 0 || doc.buffers.size() <= size_t( view.buffer ) ) throw gct::exception::invalid_argument( "参照されたbufferが存在しない", __FILE__, __LINE__ );
    if( accessor.count > max_count ) throw gct::exception::invalid_argument( "指定された要素数に対してbufferViewが小さすぎる" );
    const std::uint32_t vertex_count = accessor.count;
    const std::uint32_t offset = accessor.byteOffset + view.byteOffset;
    const std::filesystem::path uri = base_dir / doc.buffers[ view.buffer ].uri;
    const vk::Format vertex_format = gct::gltf::to_vulkan_format( accessor.componentType, accessor.type, accessor.normalized );
 
    const auto attr_id = vertex_attribute_id::index;
    buffer.attribute[ attr_id ].type = to_accessor_type_id( accessor );
    buffer.attribute[ attr_id ].component_count = to_accessor_component_count( accessor );
    buffer.attribute[ attr_id ].block_size = get_block_size( buffer.attribute[ attr_id ] );
    if( !check_accessor_type( attr_id, buffer.attribute[ attr_id ].type, buffer.attribute[ attr_id ].component_count ) ) {
      throw invalid_gltf( "サポートされていない型が設定されている" );
    }
 
    unique_vertex_count = vertex_count;
 
    resize_attribute_buffer( buffer.attribute[ attr_id ], vertex_count );
 
    gct::mmaped_file m( uri );
    write_vertex( m.data(), buffer.attribute[ attr_id ], offset, stride, vertex_count );
  }
  else {
    unique_vertex_count = vertex_count;
  }

  if( position_type_id == scene_graph::accessor_type_id::dgf ) {
    buffer.meshlet_count = position_block_count;
  }
  else {
    buffer.meshlet_count = unique_vertex_count / 96u + ( ( unique_vertex_count % 96u ) ? 1u : 0u );
  }

  buffer.topology = gltf_topology_to_vulkan_topology( primitive_.mode );
  buffer.vertex_count = vertex_count;
  buffer.unique_vertex_count = unique_vertex_count;

  return buffer;
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
    for( unsigned int i = 0u; i != meta.numVerts; ++i ) {
      dgf_verts.push_back( glm::vec4( dgf_verts_[ i ].xyz[ 0 ], dgf_verts_[ i ].xyz[ 1 ], dgf_verts_[ i ].xyz[ 2 ], 1.0f ) );
    }

    std::uint32_t primitive_offset = 0u;
    DGF::ReadUserData( head, &primitive_offset, 0, sizeof( std::uint32_t ) );
    vertex_offset = primitive_offset * fvc;
    vertex_count = meta.numVerts;
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

face_attribute meshlet_reader::operator()(
  std::uint32_t local_face_id
) const {
  if( !valid ) return face_attribute{};
  if( local_face_id >= get_face_count() ) return face_attribute{};
  face_attribute f;
  const std::uint32_t fvc = get_face_vertex_count( buffer.topology );
  f.vertex.resize( fvc );
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
    ( buffer.attribute.find( vertex_attribute_id::tangent ) != buffer.attribute.end() && buffer.attribute.find( vertex_attribute_id::tangent ) != buffer.attribute.end() ) ||
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
    ( buffer.attribute.find( vertex_attribute_id::tangent ) != buffer.attribute.end() && buffer.attribute.find( vertex_attribute_id::tangent ) != buffer.attribute.end() ) ||
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
    buffer.attribute[ attr_id ].type = scene_graph::accessor_type_id::half;
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
    for( unsigned int i = 0u; i != count.size(); ++i ) {
      for( unsigned int j = 0u; j != 3u; ++j ) {
        reinterpret_cast< float* >( encoded.data() )[ i * 4u + j ] /= count[ i ];
      }
    }
    for( unsigned int i = 0u; i != count.size(); ++i ) {
      glm::vec3 temp = glm::normalize( glm::vec3(
        reinterpret_cast< float* >( encoded.data() )[ i * 4u + 0u ],
        reinterpret_cast< float* >( encoded.data() )[ i * 4u + 1u ],
        reinterpret_cast< float* >( encoded.data() )[ i * 4u + 2u ] 
      ) );
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

