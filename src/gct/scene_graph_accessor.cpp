#include <iostream>
#include <cstdint>
#include <string>
#include <nlohmann/json.hpp>
#include <gct/scene_graph_accessor.hpp>
#include <vulkan2json/PrimitiveTopology.hpp>

namespace gct::scene_graph {
void to_json( nlohmann::json &dest, const accessor_type_id &src ) {
  if( src == accessor_type_id::float_ ) {
    dest = "float";
  }
  else if( src == accessor_type_id::i8 ) {
    dest = "i8";
  }
  else if( src == accessor_type_id::u8 ) {
    dest = "u8";
  }
  else if( src == accessor_type_id::i16 ) {
    dest = "i16";
  }
  else if( src == accessor_type_id::u16 ) {
    dest = "u16";
  }
  else if( src == accessor_type_id::u32 ) {
    dest = "u32";
  }
  else {
    dest = "unknown";
  }
}
accessor_type_id to_type_id(
  const numeric_type_t &type
) {
  if( type.component == numeric_component_type_t::float_ ) {
    return accessor_type_id::float_;
  }
  else if( type.component == numeric_component_type_t::int_ ) {
    if( type.depth == 8u ) {
      if( type.sign ) {
        return accessor_type_id::i8;
      }
      else {
        return accessor_type_id::u8;
      }
    }
    else if( type.depth == 16u ) {
      if( type.sign ) {
        return accessor_type_id::i16;
      }
      else {
        return accessor_type_id::u16;
      }
    }
    else if( type.depth == 32u ) {
      return accessor_type_id::u32;
    }
  }
  return accessor_type_id::float_;
}

accessor_type_id to_accessor_type_id( const std::string &src ) {
  std::string simplified;
  for( const auto &c: src ) {
    if( std::isalnum( c ) ) {
      simplified.push_back( std::tolower( c ) );
    }
  }
  if( simplified == "float" )
    return accessor_type_id::float_;
  else if( simplified == "byte" )
    return accessor_type_id::i8;
  else if( simplified == "unsignedbyte" )
    return accessor_type_id::u8;
  else if( simplified == "short" )
    return accessor_type_id::i16;
  else if( simplified == "unsignedshort" )
    return accessor_type_id::u16;
  else if( simplified == "unsignedint" )
    return accessor_type_id::u32;
  else if( simplified == "half" )
    return accessor_type_id::half;
  else if( simplified == "fixed" )
    return accessor_type_id::fixed;
  else if( simplified == "dgf" )
    return accessor_type_id::dgf;
  else if( simplified == "n21t11" )
    return accessor_type_id::n21t11;
  else if( simplified == "n20t11b1" )
    return accessor_type_id::n20t11b1;
  else if( simplified == "n31" )
    return accessor_type_id::n31;
  else {
    throw exception::invalid_argument( "to_acessor_type_id : Unknown type id" );
  }
}

std::size_t get_component_size(
  const scene_graph::accessor_type_id &type
) {
  if( type == scene_graph::accessor_type_id::i8 ) return 1u;
  else if( type == scene_graph::accessor_type_id::u8 ) return 1u;
  else if( type == scene_graph::accessor_type_id::i16 ) return 2u;
  else if( type == scene_graph::accessor_type_id::u16 ) return 2u;
  else if( type == scene_graph::accessor_type_id::u32 ) return 4u;
  else if( type == scene_graph::accessor_type_id::float_ ) return 4u;
  else if( type == scene_graph::accessor_type_id::dgf ) return 128u;
  else if( type == scene_graph::accessor_type_id::half ) return 2u;
  else if( type == scene_graph::accessor_type_id::fixed ) return 2u;
  else if( type == scene_graph::accessor_type_id::n21t11 ) return 4u;
  else if( type == scene_graph::accessor_type_id::n20t11b1 ) return 4u;
  else if( type == scene_graph::accessor_type_id::n31 ) return 4u;
  else {
    throw exception::invalid_argument( "get_component_size : 使用できないアクセサの型", __FILE__, __LINE__ );
  }
}

scene_graph::accessor_type_id to_accessor_type_id(
  const fx::gltf::Accessor &accessor
) {
  const bool has_extended_type =
    accessor.extensionsAndExtras.find( "extensions" ) != accessor.extensionsAndExtras.end() &&
    accessor.extensionsAndExtras[ "extensions" ].find( "GCT_EXTENDED_TYPE" ) != accessor.extensionsAndExtras[ "extensions" ].end() &&
    accessor.extensionsAndExtras[ "extensions" ][ "GCT_EXTENDED_TYPE" ].find( "type" ) != accessor.extensionsAndExtras[ "extensions" ][ "GCT_EXTENDED_TYPE" ].end();

  if( has_extended_type ) {
    const std::string typestr = accessor.extensionsAndExtras[ "extensions" ][ "GCT_EXTENDED_TYPE" ][ "type" ];
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


bool check_accessor_type(
  vertex_attribute_id attr_id,
  accessor_type_id type,
  std::uint32_t component_count
) {
  if( attr_id == vertex_attribute_id::index ) {
    if( !(
      type == accessor_type_id::u8 ||
      type == accessor_type_id::i8 ||
      type == accessor_type_id::u16 ||
      type == accessor_type_id::i16 ||
      type == accessor_type_id::u32 ||
      type == accessor_type_id::float_
    ) ) {
      return false;
    }
    if( component_count != 1u ) {
      return false;
    }
  }
  else if( attr_id == vertex_attribute_id::position ) {
    if( !(
      ( type == accessor_type_id::float_ && component_count == 3u ) ||
      ( type == accessor_type_id::half && component_count == 3u ) ||
      ( type == accessor_type_id::fixed && component_count == 3u ) ||
      ( type == accessor_type_id::dgf && component_count == 1u )
    ) ) {
      return false;
    }
  }
  else if( attr_id == vertex_attribute_id::normal ) {
    if( !(
      ( type == accessor_type_id::float_ && component_count == 3u ) ||
      ( type == accessor_type_id::half && component_count == 3u ) ||
      ( type == accessor_type_id::fixed && component_count == 3u ) ||
      ( type == accessor_type_id::n21t11 && component_count == 1u ) ||
      ( type == accessor_type_id::n20t11b1 && component_count == 1u ) ||
      ( type == accessor_type_id::n31 && component_count == 1u )
    ) ) {
      return false;
    }
  }
  else if( attr_id == vertex_attribute_id::tangent ) {
    if( !(
      ( type == accessor_type_id::float_ && component_count == 4u ) ||
      ( type == accessor_type_id::half && component_count == 4u ) ||
      ( type == accessor_type_id::fixed && component_count == 4u ) ||
      ( type == accessor_type_id::n31 && component_count == 1u )
    ) ) {
      return false;
    }
  }
  else if( attr_id == vertex_attribute_id::texcoord_0 ) {
    if( !(
      ( type == accessor_type_id::float_ && component_count == 2u ) ||
      ( type == accessor_type_id::half && component_count == 2u ) ||
      ( type == accessor_type_id::fixed && component_count == 2u ) ||
      ( type == accessor_type_id::u8 && component_count == 2u ) ||
      ( type == accessor_type_id::u16 && component_count == 2u )
    ) ) {
      return false;
    }
  }
  else if( attr_id == vertex_attribute_id::color_0 ) {
    if( !(
      ( type == accessor_type_id::float_ && component_count == 3u ) ||
      ( type == accessor_type_id::half && component_count == 3u ) ||
      ( type == accessor_type_id::fixed && component_count == 3u ) ||
      ( type == accessor_type_id::u8 && component_count == 3u ) ||
      ( type == accessor_type_id::u16 && component_count == 3u ) ||
      ( type == accessor_type_id::float_ && component_count == 4u ) ||
      ( type == accessor_type_id::half && component_count == 4u ) ||
      ( type == accessor_type_id::fixed && component_count == 4u ) ||
      ( type == accessor_type_id::u8 && component_count == 4u ) ||
      ( type == accessor_type_id::u16 && component_count == 4u )
    ) ) {
      return false;
    }
  }
  else if( attr_id == vertex_attribute_id::joint_0 ) {
    if( !(
      ( type == accessor_type_id::u8 && component_count == 4u ) ||
      ( type == accessor_type_id::u16 && component_count == 4u )
    ) ) {
      return false;
    }
  }
  else if( attr_id == vertex_attribute_id::weight_0 ) {
    if( !(
      ( type == accessor_type_id::float_ && component_count == 4u ) ||
      ( type == accessor_type_id::half && component_count == 4u ) ||
      ( type == accessor_type_id::fixed && component_count == 4u ) ||
      ( type == accessor_type_id::u8 && component_count == 4u ) ||
      ( type == accessor_type_id::u16 && component_count == 4u )
    ) ) {
      return false;
    }
  }
  return true;
}
void to_json( nlohmann::json &dest, const mesh_topology_id &src ) {
  if( src == mesh_topology_id::point ) {
    dest = "point";
  }
  else if( src == mesh_topology_id::line ) {
    dest = "line";
  }
  else if( src == mesh_topology_id::triangle ) {
    dest = "triangle";
  }
  else {
    dest = "unknown";
  }
}

void to_json( nlohmann::json &dest, const mesh_compression_method_id &src ) {
  if( src == mesh_compression_method_id::uncompressed ) {
    dest = "uncompressed";
  }
  else if( src == mesh_compression_method_id::dgf ) {
    dest = "dgf";
  }
  else {
    dest = "unknown";
  }
}
void to_json( nlohmann::json &dest, const accessor_t &src ) {
  dest = nlohmann::json::object();
  dest[ "buffer" ] = std::uint32_t( *src.buffer );
  dest[ "type" ] = src.type;
  dest[ "offset" ] = src.offset;
  dest[ "stride" ] = src.stride;
}
  
void to_json( nlohmann::json &dest, const mesh_t &src ) {
  dest = nlohmann::json::object();
  dest[ "attribute" ] = nlohmann::json::object();
  for( const auto &v: src.attribute ) {
    dest[ "attribute" ][ nlohmann::json( v.first ) ] = v.second;
  }
  dest[ "vertex_count" ] = src.vertex_count;
  dest[ "unique_vertex_count" ] = src.unique_vertex_count;
  dest[ "topology" ] = src.topology;
  dest[ "compression_method" ] = src.compression_method;
}

void to_json( nlohmann::json &dest, const lod_t &src ) {
  dest = nlohmann::json::object();
  for( const auto &v: src.level ) {
    dest[ std::to_string( v.second ) ] = v.first;
  }
}

}

