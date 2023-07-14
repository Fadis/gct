#include <gct/numeric_type_to_oiio_type.hpp>
#include <OpenImageIO/imageio.h>

namespace gct {
  OIIO_NAMESPACE :: TypeDesc numeric_type_to_oiio_type(
    const numeric_type_t &nt
  ) {
    using namespace OIIO_NAMESPACE;
    TypeDesc desc;
    desc.vecsemantics = TypeDesc::NOSEMANTICS;
    desc.arraylen = 0;
    if( nt.component == numeric_component_type_t::void_ ) {
      desc.basetype = TypeDesc::NONE;
    }
    if( nt.component == numeric_component_type_t::bool_ ) {
      desc.basetype = TypeDesc::UINT8;
    }
    if( nt.component == numeric_component_type_t::int_ ) {
      if( nt.sign ) {
        if( nt.depth <= 8 ) {
          desc.basetype = TypeDesc::INT8;
        }
        else if( nt.depth <= 16 ) {
          desc.basetype = TypeDesc::INT16;
        }
        else if( nt.depth <= 32 ) {
          desc.basetype = TypeDesc::INT32;
        }
        else if( nt.depth <= 64 ) {
          desc.basetype = TypeDesc::INT64;
        }
        else {
          throw -1;
        }
      }
      else {
        if( nt.depth <= 8 ) {
          desc.basetype = TypeDesc::UINT8;
        }
        else if( nt.depth <= 16 ) {
          desc.basetype = TypeDesc::UINT16;
        }
        else if( nt.depth <= 32 ) {
          desc.basetype = TypeDesc::UINT32;
        }
        else if( nt.depth <= 64 ) {
          desc.basetype = TypeDesc::UINT64;
        }
        else {
          throw -1;
        }
      }
    }
    if( nt.component == numeric_component_type_t::float_ ) {
      if( nt.depth <= 16 ) {
        desc.basetype = TypeDesc::HALF;
      }
      else if( nt.depth <= 32 ) {
        desc.basetype = TypeDesc::FLOAT;
      }
      else if( nt.depth <= 64 ) {
        desc.basetype = TypeDesc::DOUBLE;
      }
      else {
        throw -1;
      }
    }
    if( nt.rows == 0 ) {
      if( nt.cols == 0 ) {
        desc.aggregate = TypeDesc::SCALAR;
      }
      else if( nt.cols == 1 ) {
        desc.aggregate = TypeDesc::SCALAR;
      }
      else if( nt.cols == 2 ) {
        desc.aggregate = TypeDesc::VEC2;
      }
      else if( nt.cols == 3 ) {
        desc.aggregate = TypeDesc::VEC3;
      }
      else if( nt.cols == 4 ) {
        desc.aggregate = TypeDesc::VEC4;
      }
      else {
        throw -1;
      }
    }
    else if( nt.rows == 1 ) {
      if( nt.cols == 0 ) {
        desc.aggregate = TypeDesc::SCALAR;
      }
      else if( nt.cols == 1 ) {
        desc.aggregate = TypeDesc::SCALAR;
      }
      else if( nt.cols == 2 ) {
        desc.aggregate = TypeDesc::VEC2;
      }
      else if( nt.cols == 3 ) {
        desc.aggregate = TypeDesc::VEC3;
      }
      else if( nt.cols == 4 ) {
        desc.aggregate = TypeDesc::VEC4;
      }
      else {
        throw -1;
      }
    }
    else if( nt.rows == 2 ) {
      if( nt.cols == 0 ) {
        desc.aggregate = TypeDesc::VEC2;
      }
      else if( nt.cols == 1 ) {
        desc.aggregate = TypeDesc::VEC2;
      }
      else {
        throw -1;
      }
    }
    else if( nt.rows == 3 ) {
      if( nt.cols == 0 ) {
        desc.aggregate = TypeDesc::VEC3;
      }
      else if( nt.cols == 1 ) {
        desc.aggregate = TypeDesc::VEC3;
      }
      else if( nt.cols == 3 ) {
        desc.aggregate = TypeDesc::MATRIX33;
      }
      else {
        throw -1;
      }
    }
    else if( nt.rows == 3 ) {
      if( nt.cols == 0 ) {
        desc.aggregate = TypeDesc::VEC4;
      }
      else if( nt.cols == 1 ) {
        desc.aggregate = TypeDesc::VEC4;
      }
      else if( nt.cols == 4 ) {
        desc.aggregate = TypeDesc::MATRIX44;
      }
      else {
        throw -1;
      }
    }
    else {
      throw -1;
    }
    return desc;
  }
}

