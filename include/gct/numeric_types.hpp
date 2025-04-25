#ifndef GCT_NUMERIC_TYPES_HPP
#define GCT_NUMERIC_TYPES_HPP
#include <cstdint>
#include <vector>
#include <vulkan/vulkan.hpp>
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>
struct SpvReflectTypeDescription;
namespace gct {
  enum class numeric_component_type_t {
    void_,
    bool_,
    int_,
    float_
  };
  enum class numeric_composite_type_t {
    scalar,
    vector,
    matrix
  };
  enum class integer_attribute_t {
    asis,
    normalized,
    scaled,
    srgb
  };
  struct numeric_type_t {
    LIBGCT_SETTER( component )
    LIBGCT_SETTER( depth )
    LIBGCT_SETTER( sign )
    LIBGCT_SETTER( attr )
    LIBGCT_SETTER( composite )
    LIBGCT_SETTER( rows )
    LIBGCT_SETTER( cols )
    numeric_component_type_t component = numeric_component_type_t::void_;
    std::uint32_t depth = 0u;
    bool sign = true;
    integer_attribute_t attr = integer_attribute_t::asis;
    numeric_composite_type_t composite = numeric_composite_type_t::scalar;
    std::uint32_t rows = 0u;
    std::uint32_t cols = 0u;
  };
  [[nodiscard]] bool operator==( const numeric_type_t&, const numeric_type_t& );
  [[nodiscard]] bool operator!=( const numeric_type_t&, const numeric_type_t& );
  [[nodiscard]] numeric_type_t spv2numeric_type( const SpvReflectTypeDescription& );
  [[nodiscard]] const std::vector< vk::Format > &get_compatible_format( const numeric_type_t &v );
  [[nodiscard]] std::uint64_t get_one( const numeric_type_t &v );
  void to_json( nlohmann::json &dest, const numeric_component_type_t &src );
  void from_json( const nlohmann::json &src, numeric_component_type_t &dest );
  void to_json( nlohmann::json &dest, const integer_attribute_t &src );
  void from_json( const nlohmann::json &src, integer_attribute_t &dest );
  void to_json( nlohmann::json &dest, const numeric_composite_type_t &src );
  void from_json( const nlohmann::json &src, numeric_composite_type_t &dest );
  void to_json( nlohmann::json &dest, const numeric_type_t& );
  void from_json( const nlohmann::json &, numeric_type_t& );
}
#endif

