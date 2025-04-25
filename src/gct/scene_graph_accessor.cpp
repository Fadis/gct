#include <nlohmann/json.hpp>
#include <gct/scene_graph_accessor.hpp>

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
  else if( src == mesh_topology_id::point ) {
    dest = "line";
  }
  else if( src == mesh_topology_id::point ) {
    dest = "triangle";
  }
  else {
    dest = "unknown";
  }
}

}

