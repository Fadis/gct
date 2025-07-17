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

void inspect_primitive(
  const std::filesystem::path &base_dir,
  const fx::gltf::Document &doc,
  const fx::gltf::Primitive &p
) {
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
        std::cout << "vec" << component_count << "( ";
        bool initial = true;
        for( unsigned int j = 0u; j != component_count; ++j ) {
          if( initial ) initial = false;
          else std::cout << ", ";
          if( accessor.componentType == fx::gltf::Accessor::ComponentType::Byte ) {
            std::cout << *reinterpret_cast< const std::int8_t* >( &m[ o + j * sizeof( std::int8_t ) ] );
          }
          else if( accessor.componentType == fx::gltf::Accessor::ComponentType::UnsignedByte ) {
            std::cout << *reinterpret_cast< const std::uint8_t* >( &m[ o + j * sizeof( std::uint8_t ) ] );
          }
          else if( accessor.componentType == fx::gltf::Accessor::ComponentType::Short ) {
            std::cout << *reinterpret_cast< const std::int16_t* >( &m[ o + j * sizeof( std::int16_t ) ] );
          }
          else if( accessor.componentType == fx::gltf::Accessor::ComponentType::UnsignedShort ) {
            std::cout << *reinterpret_cast< const std::int16_t* >( &m[ o + j * sizeof( std::int16_t ) ] );
          }
          else if( accessor.componentType == fx::gltf::Accessor::ComponentType::UnsignedInt ) {
            std::cout << *reinterpret_cast< const std::int32_t* >( &m[ o + j * sizeof( std::int32_t ) ] );
          }
          else if( accessor.componentType == fx::gltf::Accessor::ComponentType::Float ) {
            std::cout << *reinterpret_cast< const float* >( &m[ o + j * sizeof( float ) ] );
          }
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
        std::cout << "vec" << component_count << "( ";
        bool initial = true;
        for( unsigned int j = 0u; j != component_count; ++j ) {
          if( initial ) initial = false;
          else std::cout << ", ";
          if( accessor.componentType == fx::gltf::Accessor::ComponentType::Byte ) {
            std::cout << *reinterpret_cast< const std::int8_t* >( &m[ o + j * sizeof( std::int8_t ) ] );
          }
          else if( accessor.componentType == fx::gltf::Accessor::ComponentType::UnsignedByte ) {
            std::cout << *reinterpret_cast< const std::uint8_t* >( &m[ o + j * sizeof( std::uint8_t ) ] );
          }
          else if( accessor.componentType == fx::gltf::Accessor::ComponentType::Short ) {
            std::cout << *reinterpret_cast< const std::int16_t* >( &m[ o + j * sizeof( std::int16_t ) ] );
          }
          else if( accessor.componentType == fx::gltf::Accessor::ComponentType::UnsignedShort ) {
            std::cout << *reinterpret_cast< const std::int16_t* >( &m[ o + j * sizeof( std::int16_t ) ] );
          }
          else if( accessor.componentType == fx::gltf::Accessor::ComponentType::UnsignedInt ) {
            std::cout << *reinterpret_cast< const std::int32_t* >( &m[ o + j * sizeof( std::int32_t ) ] );
          }
          else if( accessor.componentType == fx::gltf::Accessor::ComponentType::Float ) {
            std::cout << *reinterpret_cast< const float* >( &m[ o + j * sizeof( float ) ] );
          }
        }
        std::cout << ")," << std::endl;
      }
      std::cout << ");" << std::endl;
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
    std::cout << "uvec3 index[ " << accessor.count/3 << " ] = uvec3[](" << std::endl;
    gct::mmaped_file m( uri );
    for( unsigned int i = 0u; i != accessor.count; ++i ) {
      const std::uint32_t o = offset + i * stride;
      if( ( i % 3 ) == 0 ) {
        std::cout << "uvec3( ";
      }
      else {
        std::cout << ", ";
      }
      for( unsigned int j = 0u; j != component_count; ++j ) {
        if( accessor.componentType == fx::gltf::Accessor::ComponentType::Byte ) {
          std::cout << *reinterpret_cast< const std::int8_t* >( &m[ o + j * sizeof( std::int8_t ) ] );
        }
        else if( accessor.componentType == fx::gltf::Accessor::ComponentType::UnsignedByte ) {
          std::cout << *reinterpret_cast< const std::uint8_t* >( &m[ o + j * sizeof( std::uint8_t ) ] );
        }
        else if( accessor.componentType == fx::gltf::Accessor::ComponentType::Short ) {
          std::cout << *reinterpret_cast< const std::int16_t* >( &m[ o + j * sizeof( std::int16_t ) ] );
        }
        else if( accessor.componentType == fx::gltf::Accessor::ComponentType::UnsignedShort ) {
          std::cout << *reinterpret_cast< const std::int16_t* >( &m[ o + j * sizeof( std::int16_t ) ] );
        }
        else if( accessor.componentType == fx::gltf::Accessor::ComponentType::UnsignedInt ) {
          std::cout << *reinterpret_cast< const std::int32_t* >( &m[ o + j * sizeof( std::int32_t ) ] );
        }
        else if( accessor.componentType == fx::gltf::Accessor::ComponentType::Float ) {
          std::cout << *reinterpret_cast< const float* >( &m[ o + j * sizeof( float ) ] );
        }
      }
      if( ( i % 3 ) == 2 ) {
        std::cout << ")," << std::endl;
      }
    }
    std::cout << ");" << std::endl;
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

