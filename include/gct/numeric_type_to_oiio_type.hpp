#ifndef GCT_NUMERIC_TYPES_TO_OIIO_TYPE_HPP
#define GCT_NUMERIC_TYPES_TO_OIIO_TYPE_HPP
#include <gct/numeric_types.hpp>
#include <OpenImageIO/imageio.h>

namespace gct {
  [[nodiscard]] OIIO_NAMESPACE :: TypeDesc numeric_type_to_oiio_type(
    const numeric_type_t &nt
  );
}

#endif

