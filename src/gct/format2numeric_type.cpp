#include <gct/format.hpp>
namespace gct {
  numeric_type_t format2numeric_type( vk::Format format ) {
    return numeric_type_t{
      format_to_component_type( format ),
      format_to_component_size( format ),
      is_signed( format ),
      is_normalized( format ),
      is_scaled( format ),
      is_srgb( format ),
      numeric_composite_type_t::vector,
      format_to_channels( format ),
      0u
    };
  }
}

