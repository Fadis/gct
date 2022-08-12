#include <unordered_set>
#include <gct/vertex_attributes.hpp>

namespace gct {
  std::optional< vertex_attribute_usage_t > vertex_attribute_name_to_usage( const std::string &name ) {
    static const std::unordered_map< std::string, vertex_attribute_usage_t > name2vertex_attribute_usage {
      { "position", vertex_attribute_usage_t::POSITION },
      { "pos", vertex_attribute_usage_t::POSITION },
      { "inputposition", vertex_attribute_usage_t::POSITION },
      { "inputpos", vertex_attribute_usage_t::POSITION },
      { "inposition", vertex_attribute_usage_t::POSITION },
      { "inpos", vertex_attribute_usage_t::POSITION },
      { "normal", vertex_attribute_usage_t::NORMAL },
      { "norm", vertex_attribute_usage_t::NORMAL },
      { "inputnormal", vertex_attribute_usage_t::NORMAL },
      { "inputnorm", vertex_attribute_usage_t::NORMAL },
      { "innormal", vertex_attribute_usage_t::NORMAL },
      { "innorm", vertex_attribute_usage_t::NORMAL },
      { "tangent", vertex_attribute_usage_t::TANGENT },
      { "tan", vertex_attribute_usage_t::TANGENT },
      { "inputtangent", vertex_attribute_usage_t::TANGENT },
      { "inputtan", vertex_attribute_usage_t::TANGENT },
      { "intangent", vertex_attribute_usage_t::TANGENT },
      { "intan", vertex_attribute_usage_t::TANGENT },
      { "texcoord", vertex_attribute_usage_t::TEXCOORD },
      { "tex", vertex_attribute_usage_t::TEXCOORD },
      { "inputtexcoord", vertex_attribute_usage_t::TEXCOORD },
      { "inputtex", vertex_attribute_usage_t::TEXCOORD },
      { "intexcoord", vertex_attribute_usage_t::TEXCOORD },
      { "intex", vertex_attribute_usage_t::TEXCOORD }
    };
    const auto normalized = get_normalized_vertex_attribute_name( name );
    const auto found = name2vertex_attribute_usage.find( normalized );
    if( found == name2vertex_attribute_usage.end() ) return std::nullopt;
    return found->second;
  }
}
