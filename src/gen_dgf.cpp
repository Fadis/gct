#include <cstdint>
#include <iostream>
#include <charconv>
#include <filesystem>
#include <cstdlib>
#include <fx/gltf.h>
#include <boost/program_options.hpp>
#include <nlohmann/json.hpp>
#include <DGF.h>
#include <DGFBaker.h>
#include <gct/exception.hpp>
#include <gct/gltf.hpp>
#include <gct/mmaped_file.hpp>
#include <gct/n21t11.hpp>

std::tuple<
  std::vector< std::uint8_t >,
  std::vector< DGFBaker::TriangleRemapInfo >,
  vk::Format,
  std::vector< std::uint8_t >
> bake_dgf(
  const std::vector< float > &vertex,
  const std::vector< std::uint32_t > &index 
) {
  DGFBaker::Config config = {};
  config.blockMaxTris = 32u;
  config.blockMaxVerts = 96u;
  config.targetBitWidth = 16;
  config.generateTriangleRemap = true;
  config.enableUserData = true;
  DGFBaker::Baker baker( config );
  
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
  vk::Format remap_format;

  if( vertex.size() < 256u ) {
    remap_format = vk::Format::eR8Uint;
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
    remap_format = vk::Format::eR16Uint;
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
    remap_format = vk::Format::eR32Uint;
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

  return std::make_tuple(
    std::move( output.dgfBlocks ),
    std::move( output.triangleRemap ),
    remap_format,
    std::move( remap )
  );
}

std::uint32_t read_remap(
  vk::Format remap_format,
  const std::vector< std::uint8_t > &remap,
  std::size_t index
) {
  if( remap_format == vk::Format::eR8Uint ) {
    return reinterpret_cast< const std::uint8_t* >( remap.data() )[ index ];
  }
  else if( remap_format == vk::Format::eR16Uint ) {
    return reinterpret_cast< const std::uint16_t* >( remap.data() )[ index ];
  }
  else if( remap_format == vk::Format::eR32Uint ) {
    return reinterpret_cast< const std::uint32_t* >( remap.data() )[ index ];
  }
  else {
    throw gct::exception::invalid_argument( "未知のremap format", __FILE__, __LINE__ );
  }
}

std::size_t decode_dgf(
  const std::vector< std::uint8_t > &block,
  std::size_t index,
  const std::vector< DGFBaker::TriangleRemapInfo > &remap_info,
  vk::Format remap_format,
  const std::vector< std::uint8_t > &remap
) {
  const std::uint8_t *head = block.data() + index * 128u;
  DGF::MetaData meta;
  DGF::DecodeMetaData( &meta, head );
  DGF::TriControlValues controlValues[ DGF::MAX_TRIS ];
  std::uint8_t indexBuffer[ DGF::MAX_INDICES ];
  DGF::DecodeTopology( controlValues, indexBuffer, head );
  
  std::uint8_t triangleList[ DGF::MAX_INDICES ];
  DGF::ConvertTopologyToTriangleList( triangleList, controlValues, indexBuffer, meta.numTris );
  
  DGF::OffsetVert offsetVerts[ DGF::MAX_VERTS ];
  DGF::DecodeOffsetVerts( meta.numVerts, offsetVerts, head );

  DGF::FloatVert floatVerts[ DGF::MAX_VERTS ];
  DGF::ConvertOffsetsToFloat( meta.numVerts, floatVerts, offsetVerts, meta );

  std::uint32_t offset = 0u;
  DGF::ReadUserData( head, &offset, 0, sizeof( std::uint32_t ) );

  //const auto n21t11_head = reinterpret_cast< const std::uint32_t* >( n21t11.data() );

  for( std::size_t i = 0u; i != meta.numTris; ++i ) {
    const auto &r = remap_info[ offset + i ];
    for( unsigned int j = 0u; j != 3u; ++j ) {
      const auto attr_index = read_remap( remap_format, remap, ( offset + i ) * 3 + j );
      const auto &position = floatVerts[ triangleList[ i * 3 + j ] ];
      //const auto normal = gct::n21t11_decode_normal( n21t11_head[ attr_index ] );
      //const auto tangent = gct::n21t11_decode_tangent( n21t11_head[ attr_index ], normal );
      std::cout << "t " << index << " ( " << position.xyz[ 0 ] << ", " <<
      position.xyz[ 1 ] << ", " <<
      position.xyz[ 2 ] << " ) " <<
      r.InputPrimIndex * 3 + r.IndexRotation[ j ] << " " <<
      attr_index << " " <<
      //"( " << normal.x << ", " << normal.y << ", " << normal.z << " ) " <<
      //"( " << tangent.x << ", " << tangent.y << ", " << tangent.z << " ) " <<
      std::endl;
    }
  }
  return meta.numTris;
}

void inspect_primitive(
  const std::filesystem::path &base_dir,
  const fx::gltf::Document &doc,
  const fx::gltf::Primitive &p
) {
  std::vector< float > vertex_buffer;
  std::vector< float > normal_buffer;
  std::vector< float > tangent_buffer;
  std::unordered_map< std::uint32_t, std::vector< float > > texcoord_buffer;
  std::vector< std::uint32_t > index_buffer;
  for( const auto &[target,index]: p.attributes ) {
    if( doc.accessors.size() <= size_t( index ) ) throw gct::exception::invalid_argument( "参照されたaccessorsが存在しない", __FILE__, __LINE__ );
    const auto &accessor = doc.accessors[ index ];
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
    if( target == "POSITION" ) {
      if( accessor.componentType != fx::gltf::Accessor::ComponentType::Float ) {
        throw gct::exception::invalid_argument( "POSITIONにfloat以外の型が指定されている", __FILE__, __LINE__ );
      }
      if( accessor.type != fx::gltf::Accessor::Type::Vec3 ) {
        throw gct::exception::invalid_argument( "POSITIONに3次元のベクトル以外の型が指定されている", __FILE__, __LINE__ );
      }
      gct::mmaped_file m( uri );
      for( unsigned int i = 0u; i != accessor.count; ++i ) {
        const std::uint32_t o = offset + i * stride;
        bool initial = true;
        for( unsigned int j = 0u; j != 3u; ++j ) {
          vertex_buffer.push_back(
            *reinterpret_cast< const float* >( &m[ o + j * sizeof( float ) ] )
          );
        }
      }
    }
    else if( target == "NORMAL" ) {
      if( accessor.componentType != fx::gltf::Accessor::ComponentType::Float ) {
        throw gct::exception::invalid_argument( "NORMALにfloat以外の型が指定されている", __FILE__, __LINE__ );
      }
      if( accessor.type != fx::gltf::Accessor::Type::Vec3 ) {
        throw gct::exception::invalid_argument( "NORMALに3次元のベクトル以外の型が指定されている", __FILE__, __LINE__ );
      }
      gct::mmaped_file m( uri );
      for( unsigned int i = 0u; i != accessor.count; ++i ) {
        const std::uint32_t o = offset + i * stride;
        bool initial = true;
        for( unsigned int j = 0u; j != 3u; ++j ) {
          normal_buffer.push_back(
            *reinterpret_cast< const float* >( &m[ o + j * sizeof( float ) ] )
          );
        }
      }
    }
    else if( target == "TANGENT" ) {
      if( accessor.componentType != fx::gltf::Accessor::ComponentType::Float ) {
        throw gct::exception::invalid_argument( "TANGENTにfloat以外の型が指定されている", __FILE__, __LINE__ );
      }
      if( accessor.type != fx::gltf::Accessor::Type::Vec4 ) {
        throw gct::exception::invalid_argument( "TANGENTに5次元のベクトル以外の型が指定されている", __FILE__, __LINE__ );
      }
      gct::mmaped_file m( uri );
      for( unsigned int i = 0u; i != accessor.count; ++i ) {
        const std::uint32_t o = offset + i * stride;
        bool initial = true;
        for( unsigned int j = 0u; j != 3u; ++j ) {
          tangent_buffer.push_back(
            *reinterpret_cast< const float* >( &m[ o + j * sizeof( float ) ] )
          );
        }
        if( *reinterpret_cast< const float* >( &m[ o + 3u * sizeof( float ) ] ) < 0.0f ) {
          std::cout << "flipped tangent" << std::endl;
        }
      }
    }
    else if( target.substr( 0, 9 ) == "TEXCOORD_" ) {
      if( accessor.componentType != fx::gltf::Accessor::ComponentType::Float ) {
        throw gct::exception::invalid_argument( "TEXCOORDにfloat以外の型が指定されている", __FILE__, __LINE__ );
      }
      if( accessor.type != fx::gltf::Accessor::Type::Vec2 ) {
        throw gct::exception::invalid_argument( "TEXCOORDに2次元のベクトル以外の型が指定されている", __FILE__, __LINE__ );
      }
      if( target.size() <= 9 ) {
        throw gct::exception::invalid_argument( "TEXCOORDにインデックスが無い", __FILE__, __LINE__ );
      }
      unsigned int tex_index = 0u;
      std::from_chars( target.c_str() + 9, target.c_str() + target.size(), tex_index );
      gct::mmaped_file m( uri );
      for( unsigned int i = 0u; i != accessor.count; ++i ) {
        const std::uint32_t o = offset + i * stride;
        bool initial = true;
        for( unsigned int j = 0u; j != 2u; ++j ) {
          texcoord_buffer[ tex_index ].push_back(
            *reinterpret_cast< const float* >( &m[ o + j * sizeof( float ) ] )
          );
        }
      }
    }
  }
  if( p.indices >= 0 ) {
    const auto &accessor = doc.accessors[ p.indices ];
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
    const std::uint32_t component_count =
      ( accessor.type == fx::gltf::Accessor::Type::Scalar ) ?
      1 :
      (
        ( accessor.type == fx::gltf::Accessor::Type::Vec2 ) ?
        2 :
        (
          ( accessor.type == fx::gltf::Accessor::Type::Vec3 ) ?
          3 :
          4
        )
      );
    if( accessor.type != fx::gltf::Accessor::Type::Scalar ) {
      throw gct::exception::invalid_argument( "INDEXにスカラ以外の型が指定されている", __FILE__, __LINE__ );
    }
    gct::mmaped_file m( uri );
    for( unsigned int i = 0u; i != accessor.count; ++i ) {
      const std::uint32_t o = offset + i * stride;
      if( accessor.componentType == fx::gltf::Accessor::ComponentType::Byte ) {
        index_buffer.push_back( *reinterpret_cast< const std::int8_t* >( &m[ o ] ) );
      }
      else if( accessor.componentType == fx::gltf::Accessor::ComponentType::UnsignedByte ) {
        index_buffer.push_back( *reinterpret_cast< const std::uint8_t* >( &m[ o ] ) );
      }
      else if( accessor.componentType == fx::gltf::Accessor::ComponentType::Short ) {
        index_buffer.push_back( *reinterpret_cast< const std::int16_t* >( &m[ o ] ) );
      }
      else if( accessor.componentType == fx::gltf::Accessor::ComponentType::UnsignedShort ) {
        index_buffer.push_back( *reinterpret_cast< const std::int16_t* >( &m[ o ] ) );
      }
      else if( accessor.componentType == fx::gltf::Accessor::ComponentType::UnsignedInt ) {
        index_buffer.push_back( *reinterpret_cast< const std::int32_t* >( &m[ o ] ) );
      }
      else if( accessor.componentType == fx::gltf::Accessor::ComponentType::Float ) {
        index_buffer.push_back( *reinterpret_cast< const float* >( &m[ o ] ) );
      }
      if( index_buffer.back() >= vertex_buffer.size() ) {
        throw gct::exception::invalid_argument( "範囲外の頂点が参照されている", __FILE__, __LINE__ );
      }
    }
  }

  if( vertex_buffer.size() != normal_buffer.size() || vertex_buffer.size() != tangent_buffer.size() ) {
    throw gct::exception::invalid_argument( "POSITION NORMAL TANGENTの頂点数が一致しない", __FILE__, __LINE__ );
  }

  std::cout << "p " << vertex_buffer.size() << " " << normal_buffer.size() << " " << index_buffer.size() << std::endl;
  auto [dgf,remap_info,remap_format,remap] = bake_dgf( vertex_buffer, index_buffer );

  /*std::vector< std::uint8_t > n21t11;
  {
    n21t11.resize( normal_buffer.size()/3u * sizeof( std::uint32_t ) );
    auto cur = reinterpret_cast< std::uint32_t* >( n21t11.data() );
    for( unsigned int i = 0u; i != normal_buffer.size(); i += 3u ) {
      *cur = gct::n21t11_encode(
        glm::vec3(
          normal_buffer[ i + 0u ],
          normal_buffer[ i + 1u ],
          normal_buffer[ i + 2u ]
        ),
        glm::vec3(
          tangent_buffer[ i + 0u ],
          tangent_buffer[ i + 1u ],
          tangent_buffer[ i + 2u ]
        )
      );
      ++cur;
    }
  }*/

  std::cout << "d " << dgf.size() << " " << remap.size() << " " << int( remap_format ) << std::endl;
  for( std::size_t i = 0u; i < dgf.size() / 128u; ++i ) {
    decode_dgf( dgf, i, remap_info, remap_format, remap );
  }
}

void inspect_mesh(
  const std::filesystem::path &base_dir,
  const fx::gltf::Document &doc,
  const fx::gltf::Mesh &m
) {
  for( const auto &p: m.primitives ) {
    inspect_primitive( base_dir, doc, p );
  }
}

int main( int argc, const char *argv[] ) {
  namespace po = boost::program_options;
  po::options_description desc( "Options" );
  desc.add_options()
    ( "help,h", "show this message" )
    ( "model,m", po::value< std::string >(), "glTF filename" );
  po::variables_map vm;
  po::store( po::parse_command_line( argc, argv, desc ), vm );
  po::notify( vm );
  if( vm.count( "help" ) || !vm.count( "model" ) ) {
    std::cout << desc << std::endl;
    std::exit( 0 );
  }
  fx::gltf::ReadQuotas quota;
  quota.MaxBufferByteLength = 1024 * 1024 * 1024;
  fx::gltf::Document doc = fx::gltf::LoadFromText( vm[ "model" ].as< std::string >(), quota );
  const auto base_dir = std::filesystem::path( vm[ "model" ].as< std::string >() ).parent_path();
  for( const auto &m: doc.meshes ) {
    inspect_mesh( base_dir, doc, m );
  }

}

