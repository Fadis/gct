#ifndef GCT_NOHC_PARAMETER_HPP
#define GCT_NOHC_PARAMETER_HPP
#include <gct/setter.hpp>

namespace gct {

struct nohc_parameter {
  LIBGCT_SETTER( tos )
  LIBGCT_SETTER( top )
  LIBGCT_SETTER( trs )
  LIBGCT_SETTER( trv )
  LIBGCT_SETTER( trf )
  float tos = 0.f;
  float top = 0.f;
  float trs = 0.f;
  float trv = 0.f;
  float trf = 0.f;
};

}

#endif

