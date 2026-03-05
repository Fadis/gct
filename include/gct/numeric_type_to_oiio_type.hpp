#ifndef GCT_NUMERIC_TYPES_TO_OIIO_TYPE_HPP
#define GCT_NUMERIC_TYPES_TO_OIIO_TYPE_HPP
#include <gct/numeric_types.hpp>
#include <OpenImageIO/imageio.h>
#if OIIO_VERSION_MAJOR <= 2 || ( OIIO_VERSION_MAJOR == 3 && OIIO_VERSION_MINOR == 0 )
#define OIIO_CURRENT_NAMESPACE OIIO_NAMESPACE
#endif

namespace gct {
  [[nodiscard]] OIIO_CURRENT_NAMESPACE :: TypeDesc numeric_type_to_oiio_type(
    const numeric_type_t &nt
  );
}

#endif

