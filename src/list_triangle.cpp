#include <cstdint>
#include <iostream>
#include <filesystem>
#include <cstdlib>
#include <fx/gltf.h>
#include <boost/program_options.hpp>
#include <nlohmann/json.hpp>
#include <gct/exception.hpp>
#include <gct/gltf.hpp>
#include <gct/mmaped_file.hpp>
#include <gct/to_matrix.hpp>

void inspect_primitive(
  const std::filesystem::path &base_dir,
  const fx::gltf::Document &doc,
  const fx::gltf::Primitive &p,
  const glm::mat4 &mat
) {
  if( p.indices == 0 ) {
    for( const auto &[target,index]: p.attributes ) {
      if( doc.accessors.size() <= size_t( index ) ) throw gct::exception::invalid_argument( "参照されたaccessorsが存在しない", __FILE__, __LINE__ );
      if( target == "POSITION" ) {
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
        std::cout << "offset : " << offset << std::endl;
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
        std::cout << "stride : " << stride << std::endl;
        std::cout << "count : " << accessor.count << std::endl;
        std::cout << "format : " << nlohmann::json( vertex_format ) << std::endl;
        std::cout << "uri : " << uri << std::endl;
        std::cout << "vec" << component_count << " vertex[ " << accessor.count << " ] = vec3[](" << std::endl;
        gct::mmaped_file m( uri );
        for( unsigned int i = 0u; i != accessor.count; ++i ) {
          const std::uint32_t o = offset + i * stride;
          glm::vec4 pos = glm::vec4( 0, 0, 0, 1 );
          for( unsigned int j = 0u; j != component_count; ++j ) {
            if( accessor.componentType == fx::gltf::Accessor::ComponentType::Byte ) {
              pos[ j ] = *reinterpret_cast< const std::int8_t* >( &m[ o + j * sizeof( std::int8_t ) ] );
            }
            else if( accessor.componentType == fx::gltf::Accessor::ComponentType::UnsignedByte ) {
              pos[ j ] = *reinterpret_cast< const std::uint8_t* >( &m[ o + j * sizeof( std::uint8_t ) ] );
            }
            else if( accessor.componentType == fx::gltf::Accessor::ComponentType::Short ) {
              pos[ j ] = *reinterpret_cast< const std::int16_t* >( &m[ o + j * sizeof( std::int16_t ) ] );
            }
            else if( accessor.componentType == fx::gltf::Accessor::ComponentType::UnsignedShort ) {
              pos[ j ] = *reinterpret_cast< const std::int16_t* >( &m[ o + j * sizeof( std::int16_t ) ] );
            }
            else if( accessor.componentType == fx::gltf::Accessor::ComponentType::UnsignedInt ) {
              pos[ j ] = *reinterpret_cast< const std::int32_t* >( &m[ o + j * sizeof( std::int32_t ) ] );
            }
            else if( accessor.componentType == fx::gltf::Accessor::ComponentType::Float ) {
              pos[ j ] = *reinterpret_cast< const float* >( &m[ o + j * sizeof( float ) ] );
            }
          }
          pos = mat * pos;
          pos /= pos.w;
          bool initial = true;
          std::cout << "vec" << component_count << "( ";
          for( unsigned int j = 0u; j != component_count; ++j ) {
            if( initial ) initial = false;
            else std::cout << ", ";
            std::cout << pos[ j ];
          }
          std::cout << ")," << std::endl;
        }
        std::cout << ");" << std::endl;
      }
      else if( target == "NORMAL" ) {
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
        std::cout << "offset : " << offset << std::endl;
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
        std::cout << "stride : " << stride << std::endl;
        std::cout << "count : " << accessor.count << std::endl;
        std::cout << "format : " << nlohmann::json( vertex_format ) << std::endl;
        std::cout << "uri : " << uri << std::endl;
        std::cout << "vec" << component_count << " normal[ " << accessor.count << " ] = vec3[](" << std::endl;
        gct::mmaped_file m( uri );
        for( unsigned int i = 0u; i != accessor.count; ++i ) {
          const std::uint32_t o = offset + i * stride;
          glm::vec3 pos = glm::vec3( 0, 0, 0 );
          for( unsigned int j = 0u; j != component_count; ++j ) {
            if( accessor.componentType == fx::gltf::Accessor::ComponentType::Byte ) {
              pos[ j ] = *reinterpret_cast< const std::int8_t* >( &m[ o + j * sizeof( std::int8_t ) ] );
            }
            else if( accessor.componentType == fx::gltf::Accessor::ComponentType::UnsignedByte ) {
              pos[ j ] = *reinterpret_cast< const std::uint8_t* >( &m[ o + j * sizeof( std::uint8_t ) ] );
            }
            else if( accessor.componentType == fx::gltf::Accessor::ComponentType::Short ) {
              pos[ j ] = *reinterpret_cast< const std::int16_t* >( &m[ o + j * sizeof( std::int16_t ) ] );
            }
            else if( accessor.componentType == fx::gltf::Accessor::ComponentType::UnsignedShort ) {
              pos[ j ] = *reinterpret_cast< const std::int16_t* >( &m[ o + j * sizeof( std::int16_t ) ] );
            }
            else if( accessor.componentType == fx::gltf::Accessor::ComponentType::UnsignedInt ) {
              pos[ j ] = *reinterpret_cast< const std::int32_t* >( &m[ o + j * sizeof( std::int32_t ) ] );
            }
            else if( accessor.componentType == fx::gltf::Accessor::ComponentType::Float ) {
              pos[ j ] = *reinterpret_cast< const float* >( &m[ o + j * sizeof( float ) ] );
            }
          }
          pos = glm::mat3( mat ) * pos;
          bool initial = true;
          std::cout << "vec" << component_count << "( ";
          for( unsigned int j = 0u; j != component_count; ++j ) {
            if( initial ) initial = false;
            else std::cout << ", ";
            std::cout << pos[ j ];
          }
          std::cout << ")," << std::endl;
        }
        std::cout << ");" << std::endl;
      }
    }
  }
  else {
    const auto &index_accessor = doc.accessors[ p.indices ];
    if( index_accessor.bufferView < 0 || doc.bufferViews.size() <= size_t( index_accessor.bufferView ) ) throw gct::exception::invalid_argument( "参照されたbufferViewが存在しない", __FILE__, __LINE__ );
    const auto &view = doc.bufferViews[ index_accessor.bufferView ];
    const std::uint32_t index_default_stride = gct::gltf::to_size( index_accessor.componentType, index_accessor.type );
    const std::uint32_t index_stride = view.byteStride ? view.byteStride : index_default_stride;
    const std::uint32_t index_max_count = ( view.byteLength - ( index_accessor.byteOffset ) ) / index_stride;
    if( view.buffer < 0 || doc.buffers.size() <= size_t( view.buffer ) ) throw gct::exception::invalid_argument( "参照されたbufferが存在しない", __FILE__, __LINE__ );
    if( index_accessor.count > index_max_count ) throw gct::exception::invalid_argument( "指定された要素数に対してbufferViewが小さすぎる" );
    const std::uint32_t vertex_count = index_accessor.count;
    const std::uint32_t index_offset = index_accessor.byteOffset + view.byteOffset;
    const std::filesystem::path index_uri = base_dir / doc.buffers[ view.buffer ].uri;
    std::cout << "offset : " << index_offset << std::endl;
    const vk::Format index_format = gct::gltf::to_vulkan_format( index_accessor.componentType, index_accessor.type, index_accessor.normalized );
    const std::uint32_t component_count =
      ( index_accessor.type == fx::gltf::Accessor::Type::Scalar ) ?
      1 :
      (
        ( index_accessor.type == fx::gltf::Accessor::Type::Vec2 ) ?
        2 :
        (
          ( index_accessor.type == fx::gltf::Accessor::Type::Vec3 ) ?
          3 :
          4
        )
      );

    for( const auto &[target,index]: p.attributes ) {
      if( doc.accessors.size() <= size_t( index ) ) throw gct::exception::invalid_argument( "参照されたaccessorsが存在しない", __FILE__, __LINE__ );
      if( target == "POSITION" ) {
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
        std::cout << "offset : " << offset << std::endl;
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
        std::cout << "vec" << component_count << " vertex[ " << index_accessor.count << " ] = vec3[](" << std::endl;
        gct::mmaped_file im( index_uri );
        gct::mmaped_file vm( uri );

        for( unsigned int i = 0u; i != index_accessor.count; ++i ) {
          const std::uint32_t index_index = index_offset + i * index_stride;
          std::uint32_t vi = 0u;
          if( index_accessor.componentType == fx::gltf::Accessor::ComponentType::Byte ) {
            vi = *reinterpret_cast< const std::int8_t* >( &im[ index_index ] );
          }
          else if( index_accessor.componentType == fx::gltf::Accessor::ComponentType::UnsignedByte ) {
            vi = *reinterpret_cast< const std::uint8_t* >( &im[ index_index ] );
          }
          else if( index_accessor.componentType == fx::gltf::Accessor::ComponentType::Short ) {
            vi = *reinterpret_cast< const std::int16_t* >( &im[ index_index ] );
          }
          else if( index_accessor.componentType == fx::gltf::Accessor::ComponentType::UnsignedShort ) {
            vi = *reinterpret_cast< const std::int16_t* >( &im[ index_index ] );
          }
          else if( index_accessor.componentType == fx::gltf::Accessor::ComponentType::UnsignedInt ) {
            vi = *reinterpret_cast< const std::int32_t* >( &im[ index_index ] );
          }
          else if( index_accessor.componentType == fx::gltf::Accessor::ComponentType::Float ) {
            vi = *reinterpret_cast< const float* >( &im[ index_index ] );
          }
          
          const std::uint32_t vertex_index = offset + vi * stride;

          glm::vec4 pos = glm::vec4( 0, 0, 0, 1 );
          for( unsigned int j = 0u; j != component_count; ++j ) {
            if( accessor.componentType == fx::gltf::Accessor::ComponentType::Byte ) {
              pos[ j ] = *reinterpret_cast< const std::int8_t* >( &vm[ vertex_index + j * sizeof( std::int8_t ) ] );
            }
            else if( accessor.componentType == fx::gltf::Accessor::ComponentType::UnsignedByte ) {
              pos[ j ] = *reinterpret_cast< const std::uint8_t* >( &vm[ vertex_index + j * sizeof( std::uint8_t ) ] );
            }
            else if( accessor.componentType == fx::gltf::Accessor::ComponentType::Short ) {
              pos[ j ] = *reinterpret_cast< const std::int16_t* >( &vm[ vertex_index + j * sizeof( std::int16_t ) ] );
            }
            else if( accessor.componentType == fx::gltf::Accessor::ComponentType::UnsignedShort ) {
              pos[ j ] = *reinterpret_cast< const std::int16_t* >( &vm[ vertex_index + j * sizeof( std::int16_t ) ] );
            }
            else if( accessor.componentType == fx::gltf::Accessor::ComponentType::UnsignedInt ) {
              pos[ j ] = *reinterpret_cast< const std::int32_t* >( &vm[ vertex_index + j * sizeof( std::int32_t ) ] );
            }
            else if( accessor.componentType == fx::gltf::Accessor::ComponentType::Float ) {
              pos[ j ] = *reinterpret_cast< const float* >( &vm[ vertex_index + j * sizeof( float ) ] );
            }
          }
          pos = mat * pos;
          pos /= pos.w;
          bool initial = true;
          std::cout << "vec" << component_count << "( ";
          for( unsigned int j = 0u; j != component_count; ++j ) {
            if( initial ) initial = false;
            else std::cout << ", ";
            std::cout << pos[ j ];
          }
          std::cout << ")," << std::endl;
        }
        std::cout << ");" << std::endl;
      }
      else if( target == "NORMAL" ) {
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
        std::cout << "offset : " << offset << std::endl;
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
        std::cout << "vec" << component_count << " normal[ " << index_accessor.count << " ] = vec3[](" << std::endl;
        gct::mmaped_file im( index_uri );
        gct::mmaped_file vm( uri );

        for( unsigned int i = 0u; i != index_accessor.count; ++i ) {
          const std::uint32_t index_index = index_offset + i * index_stride;
          std::uint32_t vi = 0u;
          if( index_accessor.componentType == fx::gltf::Accessor::ComponentType::Byte ) {
            vi = *reinterpret_cast< const std::int8_t* >( &im[ index_index ] );
          }
          else if( index_accessor.componentType == fx::gltf::Accessor::ComponentType::UnsignedByte ) {
            vi = *reinterpret_cast< const std::uint8_t* >( &im[ index_index ] );
          }
          else if( index_accessor.componentType == fx::gltf::Accessor::ComponentType::Short ) {
            vi = *reinterpret_cast< const std::int16_t* >( &im[ index_index ] );
          }
          else if( index_accessor.componentType == fx::gltf::Accessor::ComponentType::UnsignedShort ) {
            vi = *reinterpret_cast< const std::int16_t* >( &im[ index_index ] );
          }
          else if( index_accessor.componentType == fx::gltf::Accessor::ComponentType::UnsignedInt ) {
            vi = *reinterpret_cast< const std::int32_t* >( &im[ index_index ] );
          }
          else if( index_accessor.componentType == fx::gltf::Accessor::ComponentType::Float ) {
            vi = *reinterpret_cast< const float* >( &im[ index_index ] );
          }
          
          const std::uint32_t vertex_index = offset + vi * stride;

          glm::vec3 pos = glm::vec3( 0, 0, 0 );
          for( unsigned int j = 0u; j != component_count; ++j ) {
            if( accessor.componentType == fx::gltf::Accessor::ComponentType::Byte ) {
              pos[ j ] = *reinterpret_cast< const std::int8_t* >( &vm[ vertex_index + j * sizeof( std::int8_t ) ] );
            }
            else if( accessor.componentType == fx::gltf::Accessor::ComponentType::UnsignedByte ) {
              pos[ j ] = *reinterpret_cast< const std::uint8_t* >( &vm[ vertex_index + j * sizeof( std::uint8_t ) ] );
            }
            else if( accessor.componentType == fx::gltf::Accessor::ComponentType::Short ) {
              pos[ j ] = *reinterpret_cast< const std::int16_t* >( &vm[ vertex_index + j * sizeof( std::int16_t ) ] );
            }
            else if( accessor.componentType == fx::gltf::Accessor::ComponentType::UnsignedShort ) {
              pos[ j ] = *reinterpret_cast< const std::int16_t* >( &vm[ vertex_index + j * sizeof( std::int16_t ) ] );
            }
            else if( accessor.componentType == fx::gltf::Accessor::ComponentType::UnsignedInt ) {
              pos[ j ] = *reinterpret_cast< const std::int32_t* >( &vm[ vertex_index + j * sizeof( std::int32_t ) ] );
            }
            else if( accessor.componentType == fx::gltf::Accessor::ComponentType::Float ) {
              pos[ j ] = *reinterpret_cast< const float* >( &vm[ vertex_index + j * sizeof( float ) ] );
            }
          }
          pos = glm::mat3( mat ) * pos;
          bool initial = true;
          std::cout << "vec" << component_count << "( ";
          for( unsigned int j = 0u; j != component_count; ++j ) {
            if( initial ) initial = false;
            else std::cout << ", ";
            std::cout << pos[ j ];
          }
          std::cout << ")," << std::endl;
        }
        std::cout << ");" << std::endl;
      }
    }
  }
}

void inspect_mesh(
  const std::filesystem::path &base_dir,
  const fx::gltf::Document &doc,
  const fx::gltf::Mesh &m,
  const glm::mat4 &mat
) {
  for( const auto &p: m.primitives ) {
    inspect_primitive( base_dir, doc, p, mat );
  }
}

void inspect_node(
  const std::filesystem::path &base_dir,
  const fx::gltf::Document &doc,
  const fx::gltf::Node &doc_node,
  const glm::mat4 &mat
) {
  std::cout << "node" << std::endl;
  glm::mat4 cur = mat;
  if( doc_node.matrix != fx::gltf::defaults::IdentityMatrix ) {
    cur *= gct::to_matrix(
      doc_node.matrix
    );
  }
  else {
    cur *= gct::to_matrix(
      doc_node.translation, doc_node.rotation, doc_node.scale
    );
  }
  if( doc.meshes.size() <= size_t( doc_node.mesh ) ) throw gct::exception::invalid_argument( "参照されたmeshが存在しない", __FILE__, __LINE__ );
  inspect_mesh(
    base_dir,
    doc,
    doc.meshes[ doc_node.mesh ],
    cur
  );
  for( const auto &c: doc_node.children ) {
    inspect_node( base_dir, doc, doc.nodes[ c ], cur );
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
  int32_t index = doc.scene;
  if( index < 0 || doc.scenes.size() <= size_t( index ) ) throw gct::exception::invalid_argument( "参照されたsceneが存在しない", __FILE__, __LINE__ );
  glm::mat4 mat = glm::mat4( 1 );
  const auto &scene = doc.scenes[ index ];
  for( const auto c: scene.nodes ) {
    inspect_node( base_dir, doc, doc.nodes[ c ], mat );
  }
}

