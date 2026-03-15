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
    dest[ "attribute" ][ std::to_string( v.first ) ] = v.second;
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

