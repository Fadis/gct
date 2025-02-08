#ifndef GCT_HTML_COLOR_HPP
#define GCT_HTML_COLOR_HPP

#include <array>
#include <cstdint>

namespace gct {

[[nodiscard]] std::array< float, 4u > html_color( std::uint32_t v );

}
#endif

