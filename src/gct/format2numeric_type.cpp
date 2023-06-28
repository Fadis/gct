#include <gct/format.hpp>
namespace gct {
  numeric_type_t format2numeric_type( vk::Format format ) {
    integer_attribute_t attr = integer_attribute_t::asis;
    if( is_normalized( format ) ) attr = integer_attribute_t::normalized;
    else if( is_scaled( format ) ) attr = integer_attribute_t::scaled;
    else if( is_srgb( format ) ) attr = integer_attribute_t::srgb;
    return numeric_type_t{
      format_to_component_type( format ),
      format_to_component_size( format ),
      is_signed( format ),
      attr,
      numeric_composite_type_t::vector,
      format_to_channels( format ),
      0u
    };
  }
}

