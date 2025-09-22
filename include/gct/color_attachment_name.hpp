#ifndef GCT_COLOR_ATTACHMENT_NAME_HPP
#define GCT_COLOR_ATTACHMENT_NAME_HPP

#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>
#include <gct/numeric_types.hpp>

namespace gct {

struct color_attachment_name {
  LIBGCT_SETTER( location )
  LIBGCT_SETTER( name )
  LIBGCT_SETTER( type )
  std::uint32_t location = 0u;
  std::string name;
  numeric_type_t type;
};

void to_json( nlohmann::json&, const color_attachment_name& );
void from_json( const nlohmann::json&, color_attachment_name& );

}

#endif


