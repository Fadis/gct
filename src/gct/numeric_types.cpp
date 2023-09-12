#include <gct/numeric_types.hpp>
#include <gct/spirv_reflect.h>
#include <gct/format.hpp>
#include <gct/extension.hpp>
#include <nlohmann/json.hpp>
namespace gct {
  numeric_type_t spv2numeric_type( const SpvReflectTypeDescription &v ) {
    numeric_type_t temp;
    if( v.type_flags & SPV_REFLECT_TYPE_FLAG_MATRIX ) {
      temp.composite = numeric_composite_type_t::matrix;
      temp.rows = v.traits.numeric.matrix.row_count;
      temp.cols = v.traits.numeric.matrix.column_count;
    }
    else if( v.type_flags & SPV_REFLECT_TYPE_FLAG_VECTOR ) {
      temp.composite = numeric_composite_type_t::vector;
      temp.rows = v.traits.numeric.vector.component_count;
    }
    else {
      temp.composite = numeric_composite_type_t::scalar;
    }
    temp.depth = v.traits.numeric.scalar.width;
    temp.sign = v.traits.numeric.scalar.signedness;
    if( v.type_flags & SPV_REFLECT_TYPE_FLAG_VOID )
      return numeric_type_t();
    else if( v.type_flags & SPV_REFLECT_TYPE_FLAG_BOOL ) {
      temp.component = numeric_component_type_t::bool_;
    }
    else if( v.type_flags & SPV_REFLECT_TYPE_FLAG_INT ) {
      temp.component = numeric_component_type_t::int_; 
    }
    else if( v.type_flags & SPV_REFLECT_TYPE_FLAG_FLOAT ) {
      temp.component = numeric_component_type_t::float_; 
    }
    else return numeric_type_t();
    return temp;
  }
  const std::vector< vk::Format > &get_compatible_1d_format( const numeric_type_t &v ) {
    if( v.component == numeric_component_type_t::bool_ ) {
      static const std::vector< vk::Format > instance{
        vk::Format::eR8Uint,
        vk::Format::eR8Sint,
        vk::Format::eR8Unorm,
        vk::Format::eR8Snorm,
        vk::Format::eR8Uscaled,
        vk::Format::eR8Sscaled,
        vk::Format::eR16Unorm,
        vk::Format::eR16Snorm,
        vk::Format::eR16Uscaled,
        vk::Format::eR16Sscaled,
        vk::Format::eR16Uint,
        vk::Format::eR16Sint,
        vk::Format::eR16Sfloat,
        vk::Format::eR32Uint,
        vk::Format::eR32Sint,
        vk::Format::eR32Sfloat,
        vk::Format::eR64Uint,
        vk::Format::eR64Sint,
        vk::Format::eR64Sfloat,
        vk::Format::eR8G8Uint,
        vk::Format::eR8G8Sint,
        vk::Format::eR8G8Unorm,
        vk::Format::eR8G8Snorm,
        vk::Format::eR8G8Uscaled,
        vk::Format::eR8G8Sscaled,
        vk::Format::eR16G16Unorm,
        vk::Format::eR16G16Snorm,
        vk::Format::eR16G16Uscaled,
        vk::Format::eR16G16Sscaled,
        vk::Format::eR16G16Uint,
        vk::Format::eR16G16Sint,
        vk::Format::eR16G16Sfloat,
        vk::Format::eR32G32Uint,
        vk::Format::eR32G32Sint,
        vk::Format::eR32G32Sfloat,
        vk::Format::eR64G64Uint,
        vk::Format::eR64G64Sint,
        vk::Format::eR64G64Sfloat,
        vk::Format::eR8G8B8Uint,
        vk::Format::eR8G8B8Sint,
        vk::Format::eR8G8B8Unorm,
        vk::Format::eR8G8B8Snorm,
        vk::Format::eR8G8B8Uscaled,
        vk::Format::eR8G8B8Sscaled,
        vk::Format::eR16G16B16Unorm,
        vk::Format::eR16G16B16Snorm,
        vk::Format::eR16G16B16Uscaled,
        vk::Format::eR16G16B16Sscaled,
        vk::Format::eR16G16B16Uint,
        vk::Format::eR16G16B16Sint,
        vk::Format::eR16G16B16Sfloat,
        vk::Format::eR32G32B32Uint,
        vk::Format::eR32G32B32Sint,
        vk::Format::eR32G32B32Sfloat,
        vk::Format::eR64G64B64Uint,
        vk::Format::eR64G64B64Sint,
        vk::Format::eR64G64B64Sfloat,
        vk::Format::eR8G8B8A8Uint,
        vk::Format::eR8G8B8A8Sint,
        vk::Format::eR8G8B8A8Unorm,
        vk::Format::eR8G8B8A8Snorm,
        vk::Format::eR8G8B8A8Uscaled,
        vk::Format::eR8G8B8A8Sscaled,
        vk::Format::eR16G16B16A16Unorm,
        vk::Format::eR16G16B16A16Snorm,
        vk::Format::eR16G16B16A16Uscaled,
        vk::Format::eR16G16B16A16Sscaled,
        vk::Format::eR16G16B16A16Uint,
        vk::Format::eR16G16B16A16Sint,
        vk::Format::eR16G16B16A16Sfloat,
        vk::Format::eR32G32B32A32Uint,
        vk::Format::eR32G32B32A32Sint,
        vk::Format::eR32G32B32A32Sfloat,
        vk::Format::eR64G64B64A64Uint,
        vk::Format::eR64G64B64A64Sint,
        vk::Format::eR64G64B64A64Sfloat
      };
      return instance;
    }
    if( v.component == numeric_component_type_t::int_ ) {
      if( v.sign ) {
        if( v.depth <= 8u ) {
          if( v.attr == integer_attribute_t::asis ) {
            static const std::vector< vk::Format > instance{
              vk::Format::eR8Sint,
              vk::Format::eR16Sint,
              vk::Format::eR16Sfloat,
              vk::Format::eR32Sint,
              vk::Format::eR32Sfloat,
              vk::Format::eR64Sint,
              vk::Format::eR64Sfloat,
              vk::Format::eR8G8Sint,
              vk::Format::eR16G16Sint,
              vk::Format::eR16G16Sfloat,
              vk::Format::eR32G32Sint,
              vk::Format::eR32G32Sfloat,
              vk::Format::eR64G64Sint,
              vk::Format::eR64G64Sfloat,
              vk::Format::eR8G8B8Sint,
              vk::Format::eR16G16B16Sint,
              vk::Format::eR16G16B16Sfloat,
              vk::Format::eR32G32B32Sint,
              vk::Format::eR32G32B32Sfloat,
              vk::Format::eR64G64B64Sint,
              vk::Format::eR64G64B64Sfloat,
              vk::Format::eR8G8B8A8Sint,
              vk::Format::eR16G16B16A16Sint,
              vk::Format::eR16G16B16A16Sfloat,
              vk::Format::eR32G32B32A32Sint,
              vk::Format::eR32G32B32A32Sfloat,
              vk::Format::eR64G64B64A64Sint,
              vk::Format::eR64G64B64A64Sfloat
            };
            return instance;
          }
          else if( v.attr == integer_attribute_t::normalized ) {
            static const std::vector< vk::Format > instance{
              vk::Format::eR8Snorm,
              vk::Format::eR16Snorm,
              vk::Format::eR8G8Snorm,
              vk::Format::eR16G16Snorm,
              vk::Format::eR8G8B8Snorm,
              vk::Format::eR16G16B16Snorm,
              vk::Format::eR8G8B8A8Snorm,
              vk::Format::eR16G16B16A16Snorm
            };
          }
          else if( v.attr == integer_attribute_t::scaled ) {
            static const std::vector< vk::Format > instance{
              vk::Format::eR8Sscaled,
              vk::Format::eR16Sscaled,
              vk::Format::eR8G8Sscaled,
              vk::Format::eR16G16Sscaled,
              vk::Format::eR8G8B8Sscaled,
              vk::Format::eR16G16B16Sscaled,
              vk::Format::eR8G8B8A8Sscaled,
              vk::Format::eR16G16B16A16Sscaled
            };
          }
        }
        else if( v.depth <= 16u ) {
          if( v.attr == integer_attribute_t::asis ) {
            static const std::vector< vk::Format > instance{
              vk::Format::eR16Sint,
              vk::Format::eR32Sint,
              vk::Format::eR32Sfloat,
              vk::Format::eR64Sint,
              vk::Format::eR64Sfloat,
              vk::Format::eR16G16Sint,
              vk::Format::eR32G32Sint,
              vk::Format::eR32G32Sfloat,
              vk::Format::eR64G64Sint,
              vk::Format::eR64G64Sfloat,
              vk::Format::eR16G16B16Sint,
              vk::Format::eR32G32B32Sint,
              vk::Format::eR32G32B32Sfloat,
              vk::Format::eR64G64B64Sint,
              vk::Format::eR64G64B64Sfloat,
              vk::Format::eR16G16B16A16Sint,
              vk::Format::eR32G32B32A32Sint,
              vk::Format::eR32G32B32A32Sfloat,
              vk::Format::eR64G64B64A64Sint,
              vk::Format::eR64G64B64A64Sfloat
            };
            return instance;
          }
          else if( v.attr == integer_attribute_t::normalized ) {
            static const std::vector< vk::Format > instance{
              vk::Format::eR16Snorm,
              vk::Format::eR16G16Snorm,
              vk::Format::eR16G16B16Snorm,
              vk::Format::eR16G16B16A16Snorm
            };
            return instance;
          }
          else if( v.attr == integer_attribute_t::scaled ) {
            static const std::vector< vk::Format > instance{
              vk::Format::eR16Sscaled,
              vk::Format::eR16G16Sscaled,
              vk::Format::eR16G16B16Sscaled,
              vk::Format::eR16G16B16A16Sscaled
            };
            return instance;
          }
        }
        else if( v.depth <= 32u ) {
          if( v.attr == integer_attribute_t::asis ) {
            static const std::vector< vk::Format > instance{
              vk::Format::eR32Sint,
              vk::Format::eR64Sint,
              vk::Format::eR64Sfloat,
              vk::Format::eR32G32Sint,
              vk::Format::eR64G64Sint,
              vk::Format::eR64G64Sfloat,
              vk::Format::eR32G32B32Sint,
              vk::Format::eR64G64B64Sint,
              vk::Format::eR64G64B64Sfloat,
              vk::Format::eR32G32B32A32Sint,
              vk::Format::eR64G64B64A64Sint,
              vk::Format::eR64G64B64A64Sfloat
            };
            return instance;
          }
        }
        else if( v.depth <= 64u ) {
          if( v.attr == integer_attribute_t::asis ) {
            static const std::vector< vk::Format > instance{
              vk::Format::eR64Sint,
              vk::Format::eR64G64Sint,
              vk::Format::eR64G64B64Sint,
              vk::Format::eR64G64B64A64Sint
            };
            return instance;
          }
        }
      }
      else {
        if( v.depth <= 8u ) {
          if( v.attr == integer_attribute_t::asis ) {
            static const std::vector< vk::Format > instance{
              vk::Format::eR8Uint,
              vk::Format::eR16Uint,
              vk::Format::eR16Sfloat,
              vk::Format::eR32Uint,
              vk::Format::eR32Sfloat,
              vk::Format::eR64Uint,
              vk::Format::eR64Sfloat,
              vk::Format::eR8G8Uint,
              vk::Format::eR16G16Uint,
              vk::Format::eR16G16Sfloat,
              vk::Format::eR32G32Uint,
              vk::Format::eR32G32Sfloat,
              vk::Format::eR64G64Uint,
              vk::Format::eR64G64Sfloat,
              vk::Format::eR8G8B8Uint,
              vk::Format::eR16G16B16Uint,
              vk::Format::eR16G16B16Sfloat,
              vk::Format::eR32G32B32Uint,
              vk::Format::eR32G32B32Sfloat,
              vk::Format::eR64G64B64Uint,
              vk::Format::eR64G64B64Sfloat,
              vk::Format::eR8G8B8A8Uint,
              vk::Format::eR16G16B16A16Uint,
              vk::Format::eR16G16B16A16Sfloat,
              vk::Format::eR32G32B32A32Uint,
              vk::Format::eR32G32B32A32Sfloat,
              vk::Format::eR64G64B64A64Uint,
              vk::Format::eR64G64B64A64Sfloat
            };
            return instance;
          }
          else if( v.attr == integer_attribute_t::normalized ) {
            static const std::vector< vk::Format > instance{
              vk::Format::eR8Unorm,
              vk::Format::eR16Unorm,
              vk::Format::eR8G8Unorm,
              vk::Format::eR16G16Unorm,
              vk::Format::eR8G8B8Unorm,
              vk::Format::eR16G16B16Unorm,
              vk::Format::eR8G8B8A8Unorm,
              vk::Format::eR16G16B16A16Unorm
            };
            return instance;
          }
          else if( v.attr == integer_attribute_t::scaled ) {
            static const std::vector< vk::Format > instance{
              vk::Format::eR8Uscaled,
              vk::Format::eR16Uscaled,
              vk::Format::eR8G8Uscaled,
              vk::Format::eR16G16Uscaled,
              vk::Format::eR8G8B8Uscaled,
              vk::Format::eR16G16B16Uscaled,
              vk::Format::eR8G8B8A8Uscaled,
              vk::Format::eR16G16B16A16Uscaled
            };
            return instance;
          }
          else if( v.attr == integer_attribute_t::srgb ) {
            static const std::vector< vk::Format > instance{
              vk::Format::eR8Srgb,
              vk::Format::eR8G8Srgb,
              vk::Format::eR8G8B8Srgb,
              vk::Format::eR8G8B8A8Srgb
            };
            return instance;
          }
        }
        else if( v.depth <= 16u ) {
          if( v.attr == integer_attribute_t::asis ) {
            static const std::vector< vk::Format > instance{
              vk::Format::eR16Uint,
              vk::Format::eR32Uint,
              vk::Format::eR32Sfloat,
              vk::Format::eR64Uint,
              vk::Format::eR64Sfloat,
              vk::Format::eR16G16Uint,
              vk::Format::eR32G32Uint,
              vk::Format::eR32G32Sfloat,
              vk::Format::eR64G64Uint,
              vk::Format::eR64G64Sfloat,
              vk::Format::eR16G16B16Uint,
              vk::Format::eR32G32B32Uint,
              vk::Format::eR32G32B32Sfloat,
              vk::Format::eR64G64B64Uint,
              vk::Format::eR64G64B64Sfloat,
              vk::Format::eR16G16B16A16Uint,
              vk::Format::eR32G32B32A32Uint,
              vk::Format::eR32G32B32A32Sfloat,
              vk::Format::eR64G64B64A64Uint,
              vk::Format::eR64G64B64A64Sfloat
            };
            return instance;
          }
          else if( v.attr == integer_attribute_t::normalized ) {
            static const std::vector< vk::Format > instance{
              vk::Format::eR16Unorm,
              vk::Format::eR16G16Unorm,
              vk::Format::eR16G16B16Unorm,
              vk::Format::eR16G16B16A16Unorm
            };
            return instance;
          }
          else if( v.attr == integer_attribute_t::scaled ) {
            static const std::vector< vk::Format > instance{
              vk::Format::eR16Uscaled,
              vk::Format::eR16G16Uscaled,
              vk::Format::eR16G16B16Uscaled,
              vk::Format::eR16G16B16A16Uscaled
            };
            return instance;
          }
        }
        else if( v.depth <= 32u ) {
          if( v.attr == integer_attribute_t::asis ) {
            static const std::vector< vk::Format > instance{
              vk::Format::eR32Uint,
              vk::Format::eR64Uint,
              vk::Format::eR64Sfloat,
              vk::Format::eR32G32Uint,
              vk::Format::eR64G64Uint,
              vk::Format::eR64G64Sfloat,
              vk::Format::eR32G32B32Uint,
              vk::Format::eR64G64B64Uint,
              vk::Format::eR64G64B64Sfloat,
              vk::Format::eR32G32B32A32Uint,
              vk::Format::eR64G64B64A64Uint,
              vk::Format::eR64G64B64A64Sfloat
            };
            return instance;
          }
        }
        else if( v.depth <= 64u ) {
          if( v.attr == integer_attribute_t::asis ) {
            static const std::vector< vk::Format > instance{
              vk::Format::eR64Uint,
              vk::Format::eR64G64Uint,
              vk::Format::eR64G64B64Uint,
              vk::Format::eR64G64B64A64Uint
            };
            return instance;
          }
        }
      }
    }
    if( v.component == numeric_component_type_t::float_ ) {
      if( v.depth <= 16u ) {
        static const std::vector< vk::Format > instance{
          vk::Format::eR16Sfloat,
          vk::Format::eR32Sfloat,
          vk::Format::eR64Sfloat,
          vk::Format::eR16G16Sfloat,
          vk::Format::eR32G32Sfloat,
          vk::Format::eR64G64Sfloat,
          vk::Format::eR16G16B16Sfloat,
          vk::Format::eR32G32B32Sfloat,
          vk::Format::eR64G64B64Sfloat,
          vk::Format::eR16G16B16A16Sfloat,
          vk::Format::eR32G32B32A32Sfloat,
          vk::Format::eR64G64B64A64Sfloat
        };
        return instance;
      }
      else if( v.depth <= 32u ) {
        static const std::vector< vk::Format > instance{
          vk::Format::eR32Sfloat,
          vk::Format::eR64Sfloat,
          vk::Format::eR32G32Sfloat,
          vk::Format::eR64G64Sfloat,
          vk::Format::eR32G32B32Sfloat,
          vk::Format::eR64G64B64Sfloat,
          vk::Format::eR32G32B32A32Sfloat,
          vk::Format::eR64G64B64A64Sfloat
        };
        return instance;
      }
      else if( v.depth <= 64u ) {
        static const std::vector< vk::Format > instance{
          vk::Format::eR64Sfloat,
          vk::Format::eR64G64Sfloat,
          vk::Format::eR64G64B64Sfloat,
          vk::Format::eR64G64B64A64Sfloat
        };
        return instance;
      }
    }
    static const std::vector< vk::Format > instance;
    return instance;
  }
  const std::vector< vk::Format > &get_compatible_2d_format( const numeric_type_t &v ) {
    if( v.component == numeric_component_type_t::bool_ ) {
      static const std::vector< vk::Format > instance{
        vk::Format::eR8G8Uint,
        vk::Format::eR8G8Sint,
        vk::Format::eR8G8Unorm,
        vk::Format::eR8G8Snorm,
        vk::Format::eR8G8Uscaled,
        vk::Format::eR8G8Sscaled,
        vk::Format::eR16G16Unorm,
        vk::Format::eR16G16Snorm,
        vk::Format::eR16G16Uscaled,
        vk::Format::eR16G16Sscaled,
        vk::Format::eR16G16Uint,
        vk::Format::eR16G16Sint,
        vk::Format::eR16G16Sfloat,
        vk::Format::eR32G32Uint,
        vk::Format::eR32G32Sint,
        vk::Format::eR32G32Sfloat,
        vk::Format::eR64G64Uint,
        vk::Format::eR64G64Sint,
        vk::Format::eR64G64Sfloat,
        vk::Format::eR8G8B8Uint,
        vk::Format::eR8G8B8Sint,
        vk::Format::eR8G8B8Unorm,
        vk::Format::eR8G8B8Snorm,
        vk::Format::eR8G8B8Uscaled,
        vk::Format::eR8G8B8Sscaled,
        vk::Format::eR16G16B16Unorm,
        vk::Format::eR16G16B16Snorm,
        vk::Format::eR16G16B16Uscaled,
        vk::Format::eR16G16B16Sscaled,
        vk::Format::eR16G16B16Uint,
        vk::Format::eR16G16B16Sint,
        vk::Format::eR16G16B16Sfloat,
        vk::Format::eR32G32B32Uint,
        vk::Format::eR32G32B32Sint,
        vk::Format::eR32G32B32Sfloat,
        vk::Format::eR64G64B64Uint,
        vk::Format::eR64G64B64Sint,
        vk::Format::eR64G64B64Sfloat,
        vk::Format::eR8G8B8A8Uint,
        vk::Format::eR8G8B8A8Sint,
        vk::Format::eR8G8B8A8Unorm,
        vk::Format::eR8G8B8A8Snorm,
        vk::Format::eR8G8B8A8Uscaled,
        vk::Format::eR8G8B8A8Sscaled,
        vk::Format::eR16G16B16A16Unorm,
        vk::Format::eR16G16B16A16Snorm,
        vk::Format::eR16G16B16A16Uscaled,
        vk::Format::eR16G16B16A16Sscaled,
        vk::Format::eR16G16B16A16Uint,
        vk::Format::eR16G16B16A16Sint,
        vk::Format::eR16G16B16A16Sfloat,
        vk::Format::eR32G32B32A32Uint,
        vk::Format::eR32G32B32A32Sint,
        vk::Format::eR32G32B32A32Sfloat,
        vk::Format::eR64G64B64A64Uint,
        vk::Format::eR64G64B64A64Sint,
        vk::Format::eR64G64B64A64Sfloat
      };
      return instance;
    }
    if( v.component == numeric_component_type_t::int_ ) {
      if( v.sign ) {
        if( v.depth <= 8u ) {
          if( v.attr == integer_attribute_t::asis ) {
            static const std::vector< vk::Format > instance{
              vk::Format::eR8G8Sint,
              vk::Format::eR16G16Sint,
              vk::Format::eR16G16Sfloat,
              vk::Format::eR32G32Sint,
              vk::Format::eR32G32Sfloat,
              vk::Format::eR64G64Sint,
              vk::Format::eR64G64Sfloat,
              vk::Format::eR8G8B8Sint,
              vk::Format::eR16G16B16Sint,
              vk::Format::eR16G16B16Sfloat,
              vk::Format::eR32G32B32Sint,
              vk::Format::eR32G32B32Sfloat,
              vk::Format::eR64G64B64Sint,
              vk::Format::eR64G64B64Sfloat,
              vk::Format::eR8G8B8A8Sint,
              vk::Format::eR16G16B16A16Sint,
              vk::Format::eR16G16B16A16Sfloat,
              vk::Format::eR32G32B32A32Sint,
              vk::Format::eR32G32B32A32Sfloat,
              vk::Format::eR64G64B64A64Sint,
              vk::Format::eR64G64B64A64Sfloat
            };
            return instance;
          }
          else if( v.attr == integer_attribute_t::normalized ) {
            static const std::vector< vk::Format > instance{
              vk::Format::eR8G8Snorm,
              vk::Format::eR16G16Snorm,
              vk::Format::eR8G8B8Snorm,
              vk::Format::eR16G16B16Snorm,
              vk::Format::eR8G8B8A8Snorm,
              vk::Format::eR16G16B16A16Snorm
            };
            return instance;
          }
          else if( v.attr == integer_attribute_t::scaled ) {
            static const std::vector< vk::Format > instance{
              vk::Format::eR8G8Sscaled,
              vk::Format::eR16G16Sscaled,
              vk::Format::eR8G8B8Sscaled,
              vk::Format::eR16G16B16Sscaled,
              vk::Format::eR8G8B8A8Sscaled,
              vk::Format::eR16G16B16A16Sscaled
            };
            return instance;
          }
        }
        else if( v.depth <= 16u ) {
          if( v.attr == integer_attribute_t::asis ) {
            static const std::vector< vk::Format > instance{
              vk::Format::eR16G16Sint,
              vk::Format::eR32G32Sint,
              vk::Format::eR32G32Sfloat,
              vk::Format::eR64G64Sint,
              vk::Format::eR64G64Sfloat,
              vk::Format::eR16G16B16Sint,
              vk::Format::eR32G32B32Sint,
              vk::Format::eR32G32B32Sfloat,
              vk::Format::eR64G64B64Sint,
              vk::Format::eR64G64B64Sfloat,
              vk::Format::eR16G16B16A16Sint,
              vk::Format::eR32G32B32A32Sint,
              vk::Format::eR32G32B32A32Sfloat,
              vk::Format::eR64G64B64A64Sint,
              vk::Format::eR64G64B64A64Sfloat
            };
            return instance;
          }
          else if( v.attr == integer_attribute_t::normalized ) {
            static const std::vector< vk::Format > instance{
              vk::Format::eR16G16Snorm,
              vk::Format::eR16G16B16Snorm,
              vk::Format::eR16G16B16A16Snorm
            };
            return instance;
          }
          else if( v.attr == integer_attribute_t::scaled ) {
            static const std::vector< vk::Format > instance{
              vk::Format::eR16G16Sscaled,
              vk::Format::eR16G16B16Sscaled,
              vk::Format::eR16G16B16A16Sscaled
            };
            return instance;
          }
        }
        else if( v.depth <= 32u ) {
          if( v.attr == integer_attribute_t::asis ) {
            static const std::vector< vk::Format > instance{
              vk::Format::eR32G32Sint,
              vk::Format::eR64G64Sint,
              vk::Format::eR64G64Sfloat,
              vk::Format::eR32G32B32Sint,
              vk::Format::eR64G64B64Sint,
              vk::Format::eR64G64B64Sfloat,
              vk::Format::eR32G32B32A32Sint,
              vk::Format::eR64G64B64A64Sint,
              vk::Format::eR64G64B64A64Sfloat
            };
            return instance;
          }
        }
        else if( v.depth <= 64u ) {
          if( v.attr == integer_attribute_t::asis ) {
            static const std::vector< vk::Format > instance{
              vk::Format::eR64G64Sint,
              vk::Format::eR64G64B64Sint,
              vk::Format::eR64G64B64A64Sint
            };
            return instance;
          }
        }
      }
      else {
        if( v.depth <= 8u ) {
          if( v.attr == integer_attribute_t::asis ) {
            static const std::vector< vk::Format > instance{
              vk::Format::eR8G8Uint,
              vk::Format::eR16G16Uint,
              vk::Format::eR16G16Sfloat,
              vk::Format::eR32G32Uint,
              vk::Format::eR32G32Sfloat,
              vk::Format::eR64G64Uint,
              vk::Format::eR64G64Sfloat,
              vk::Format::eR8G8B8Uint,
              vk::Format::eR16G16B16Uint,
              vk::Format::eR16G16B16Sfloat,
              vk::Format::eR32G32B32Uint,
              vk::Format::eR32G32B32Sfloat,
              vk::Format::eR64G64B64Uint,
              vk::Format::eR64G64B64Sfloat,
              vk::Format::eR8G8B8A8Uint,
              vk::Format::eR16G16B16A16Uint,
              vk::Format::eR16G16B16A16Sfloat,
              vk::Format::eR32G32B32A32Uint,
              vk::Format::eR32G32B32A32Sfloat,
              vk::Format::eR64G64B64A64Uint,
              vk::Format::eR64G64B64A64Sfloat
            };
            return instance;
          }
          else if( v.attr == integer_attribute_t::normalized ) {
            static const std::vector< vk::Format > instance{
              vk::Format::eR8G8Unorm,
              vk::Format::eR16G16Unorm,
              vk::Format::eR8G8B8Unorm,
              vk::Format::eR16G16B16Unorm,
              vk::Format::eR8G8B8A8Unorm,
              vk::Format::eR16G16B16A16Unorm
            };
            return instance;
          }
          else if( v.attr == integer_attribute_t::scaled ) {
            static const std::vector< vk::Format > instance{
              vk::Format::eR8G8Uscaled,
              vk::Format::eR16G16Uscaled,
              vk::Format::eR8G8B8Uscaled,
              vk::Format::eR16G16B16Uscaled,
              vk::Format::eR8G8B8A8Uscaled,
              vk::Format::eR16G16B16A16Uscaled
            };
            return instance;
          }
          else if( v.attr == integer_attribute_t::srgb ) {
            static const std::vector< vk::Format > instance{
              vk::Format::eR8G8Srgb,
              vk::Format::eR8G8B8Srgb,
              vk::Format::eR8G8B8A8Srgb
            };
            return instance;
          }
        }
        else if( v.depth <= 16u ) {
          if( v.attr == integer_attribute_t::asis ) {
            static const std::vector< vk::Format > instance{
              vk::Format::eR16G16Uint,
              vk::Format::eR32G32Uint,
              vk::Format::eR32G32Sfloat,
              vk::Format::eR64G64Uint,
              vk::Format::eR64G64Sfloat,
              vk::Format::eR16G16B16Uint,
              vk::Format::eR32G32B32Uint,
              vk::Format::eR32G32B32Sfloat,
              vk::Format::eR64G64B64Uint,
              vk::Format::eR64G64B64Sfloat,
              vk::Format::eR16G16B16A16Uint,
              vk::Format::eR32G32B32A32Uint,
              vk::Format::eR32G32B32A32Sfloat,
              vk::Format::eR64G64B64A64Uint,
              vk::Format::eR64G64B64A64Sfloat
            };
            return instance;
          }
          else if( v.attr == integer_attribute_t::normalized ) {
            static const std::vector< vk::Format > instance{
              vk::Format::eR16G16Unorm,
              vk::Format::eR16G16B16Unorm,
              vk::Format::eR16G16B16A16Unorm
            };
            return instance;
          }
          else if( v.attr == integer_attribute_t::scaled ) {
            static const std::vector< vk::Format > instance{
              vk::Format::eR16G16Uscaled,
              vk::Format::eR16G16B16Uscaled,
              vk::Format::eR16G16B16A16Uscaled
            };
            return instance;
          }
        }
        else if( v.depth <= 32u ) {
          if( v.attr == integer_attribute_t::asis ) {
            static const std::vector< vk::Format > instance{
              vk::Format::eR32G32Uint,
              vk::Format::eR64G64Uint,
              vk::Format::eR64G64Sfloat,
              vk::Format::eR32G32B32Uint,
              vk::Format::eR64G64B64Uint,
              vk::Format::eR64G64B64Sfloat,
              vk::Format::eR32G32B32A32Uint,
              vk::Format::eR64G64B64A64Uint,
              vk::Format::eR64G64B64A64Sfloat
            };
            return instance;
          }
        }
        else if( v.depth <= 64u ) {
          if( v.attr == integer_attribute_t::asis ) {
            static const std::vector< vk::Format > instance{
              vk::Format::eR64G64Uint,
              vk::Format::eR64G64B64Uint,
              vk::Format::eR64G64B64A64Uint
            };
            return instance;
          }
        }
      }
    }
    if( v.component == numeric_component_type_t::float_ ) {
      if( v.depth <= 16u ) {
        static const std::vector< vk::Format > instance{
          vk::Format::eR16G16Sfloat,
          vk::Format::eR32G32Sfloat,
          vk::Format::eR64G64Sfloat,
          vk::Format::eR16G16B16Sfloat,
          vk::Format::eR32G32B32Sfloat,
          vk::Format::eR64G64B64Sfloat,
          vk::Format::eR16G16B16A16Sfloat,
          vk::Format::eR32G32B32A32Sfloat,
          vk::Format::eR64G64B64A64Sfloat
        };
        return instance;
      }
      else if( v.depth <= 32u ) {
        static const std::vector< vk::Format > instance{
          vk::Format::eR32G32Sfloat,
          vk::Format::eR64G64Sfloat,
          vk::Format::eR32G32B32Sfloat,
          vk::Format::eR64G64B64Sfloat,
          vk::Format::eR32G32B32A32Sfloat,
          vk::Format::eR64G64B64A64Sfloat
        };
        return instance;
      }
      else if( v.depth <= 64u ) {
        static const std::vector< vk::Format > instance{
          vk::Format::eR64G64Sfloat,
          vk::Format::eR64G64B64Sfloat,
          vk::Format::eR64G64B64A64Sfloat
        };
        return instance;
      }
      else {
        static const std::vector< vk::Format > instance;
        return instance;
      }
    }
    static const std::vector< vk::Format > instance;
    return instance;
  }
  const std::vector< vk::Format > &get_compatible_3d_format( const numeric_type_t &v ) {
    if( v.component == numeric_component_type_t::bool_ ) {
      static const std::vector< vk::Format > instance{
        vk::Format::eR8G8B8Uint,
        vk::Format::eR8G8B8Sint,
        vk::Format::eR8G8B8Unorm,
        vk::Format::eR8G8B8Snorm,
        vk::Format::eR8G8B8Uscaled,
        vk::Format::eR8G8B8Sscaled,
        vk::Format::eR16G16B16Unorm,
        vk::Format::eR16G16B16Snorm,
        vk::Format::eR16G16B16Uscaled,
        vk::Format::eR16G16B16Sscaled,
        vk::Format::eR16G16B16Uint,
        vk::Format::eR16G16B16Sint,
        vk::Format::eR16G16B16Sfloat,
        vk::Format::eR32G32B32Uint,
        vk::Format::eR32G32B32Sint,
        vk::Format::eR32G32B32Sfloat,
        vk::Format::eR64G64B64Uint,
        vk::Format::eR64G64B64Sint,
        vk::Format::eR64G64B64Sfloat,
        vk::Format::eR8G8B8A8Uint,
        vk::Format::eR8G8B8A8Sint,
        vk::Format::eR8G8B8A8Unorm,
        vk::Format::eR8G8B8A8Snorm,
        vk::Format::eR8G8B8A8Uscaled,
        vk::Format::eR8G8B8A8Sscaled,
        vk::Format::eR16G16B16A16Unorm,
        vk::Format::eR16G16B16A16Snorm,
        vk::Format::eR16G16B16A16Uscaled,
        vk::Format::eR16G16B16A16Sscaled,
        vk::Format::eR16G16B16A16Uint,
        vk::Format::eR16G16B16A16Sint,
        vk::Format::eR16G16B16A16Sfloat,
        vk::Format::eR32G32B32A32Uint,
        vk::Format::eR32G32B32A32Sint,
        vk::Format::eR32G32B32A32Sfloat,
        vk::Format::eR64G64B64A64Uint,
        vk::Format::eR64G64B64A64Sint,
        vk::Format::eR64G64B64A64Sfloat
      };
      return instance;
    }
    if( v.component == numeric_component_type_t::int_ ) {
      if( v.sign ) {
        if( v.depth <= 8u ) {
          if( v.attr == integer_attribute_t::asis ) {
            static const std::vector< vk::Format > instance{
              vk::Format::eR8G8B8Sint,
              vk::Format::eR16G16B16Sint,
              vk::Format::eR16G16B16Sfloat,
              vk::Format::eR32G32B32Sint,
              vk::Format::eR32G32B32Sfloat,
              vk::Format::eR64G64B64Sint,
              vk::Format::eR64G64B64Sfloat,
              vk::Format::eR8G8B8A8Sint,
              vk::Format::eR16G16B16A16Sint,
              vk::Format::eR16G16B16A16Sfloat,
              vk::Format::eR32G32B32A32Sint,
              vk::Format::eR32G32B32A32Sfloat,
              vk::Format::eR64G64B64A64Sint,
              vk::Format::eR64G64B64A64Sfloat
            };
            return instance;
          }
          else if( v.attr == integer_attribute_t::normalized ) {
            static const std::vector< vk::Format > instance{
              vk::Format::eR8G8B8Snorm,
              vk::Format::eR16G16B16Snorm,
              vk::Format::eR8G8B8A8Snorm,
              vk::Format::eR16G16B16A16Snorm
            };
            return instance;
          }
          else if( v.attr == integer_attribute_t::scaled ) {
            static const std::vector< vk::Format > instance{
              vk::Format::eR8G8B8Sscaled,
              vk::Format::eR16G16B16Sscaled,
              vk::Format::eR8G8B8A8Sscaled,
              vk::Format::eR16G16B16A16Sscaled
            };
            return instance;
          }
        }
        else if( v.depth <= 16u ) {
          if( v.attr == integer_attribute_t::asis ) {
            static const std::vector< vk::Format > instance{
              vk::Format::eR16G16B16Sint,
              vk::Format::eR32G32B32Sint,
              vk::Format::eR32G32B32Sfloat,
              vk::Format::eR64G64B64Sint,
              vk::Format::eR64G64B64Sfloat,
              vk::Format::eR16G16B16A16Sint,
              vk::Format::eR32G32B32A32Sint,
              vk::Format::eR32G32B32A32Sfloat,
              vk::Format::eR64G64B64A64Sint,
              vk::Format::eR64G64B64A64Sfloat
            };
            return instance;
          }
          else if( v.attr == integer_attribute_t::normalized ) {
            static const std::vector< vk::Format > instance{
              vk::Format::eR16G16B16Snorm,
              vk::Format::eR16G16B16A16Snorm
            };
            return instance;
          }
          else if( v.attr == integer_attribute_t::scaled ) {
            static const std::vector< vk::Format > instance{
              vk::Format::eR16G16B16Sscaled,
              vk::Format::eR16G16B16A16Sscaled
            };
            return instance;
          }
        }
        else if( v.depth <= 32u ) {
          if( v.attr == integer_attribute_t::asis ) {
            static const std::vector< vk::Format > instance{
              vk::Format::eR32G32B32Sint,
              vk::Format::eR64G64B64Sint,
              vk::Format::eR64G64B64Sfloat,
              vk::Format::eR32G32B32A32Sint,
              vk::Format::eR64G64B64A64Sint,
              vk::Format::eR64G64B64A64Sfloat
            };
            return instance;
          }
        }
        else if( v.depth <= 64u ) {
          if( v.attr == integer_attribute_t::asis ) {
            static const std::vector< vk::Format > instance{
              vk::Format::eR64G64B64Sint,
              vk::Format::eR64G64B64A64Sint
            };
            return instance;
          }
        }
      }
      else {
        if( v.depth <= 8u ) {
          if( v.attr == integer_attribute_t::asis ) {
            static const std::vector< vk::Format > instance{
              vk::Format::eR8G8B8Uint,
              vk::Format::eR16G16B16Uint,
              vk::Format::eR16G16B16Sfloat,
              vk::Format::eR32G32B32Uint,
              vk::Format::eR32G32B32Sfloat,
              vk::Format::eR64G64B64Uint,
              vk::Format::eR64G64B64Sfloat,
              vk::Format::eR8G8B8A8Uint,
              vk::Format::eR16G16B16A16Uint,
              vk::Format::eR16G16B16A16Sfloat,
              vk::Format::eR32G32B32A32Uint,
              vk::Format::eR32G32B32A32Sfloat,
              vk::Format::eR64G64B64A64Uint,
              vk::Format::eR64G64B64A64Sfloat
            };
            return instance;
          }
          else if( v.attr == integer_attribute_t::normalized ) {
            static const std::vector< vk::Format > instance{
              vk::Format::eR8G8B8Unorm,
              vk::Format::eR16G16B16Unorm,
              vk::Format::eR8G8B8A8Unorm,
              vk::Format::eR16G16B16A16Unorm
            };
            return instance;
          }
          else if( v.attr == integer_attribute_t::scaled ) {
            static const std::vector< vk::Format > instance{
              vk::Format::eR8G8B8Uscaled,
              vk::Format::eR16G16B16Uscaled,
              vk::Format::eR8G8B8A8Uscaled,
              vk::Format::eR16G16B16A16Uscaled
            };
            return instance;
          }
          else if( v.attr == integer_attribute_t::srgb ) {
            static const std::vector< vk::Format > instance{
              vk::Format::eR8G8B8Srgb,
              vk::Format::eR8G8B8A8Srgb
            };
            return instance;
          }
        }
        else if( v.depth <= 16u ) {
          if( v.attr == integer_attribute_t::asis ) {
            static const std::vector< vk::Format > instance{
              vk::Format::eR16G16B16Uint,
              vk::Format::eR32G32B32Uint,
              vk::Format::eR32G32B32Sfloat,
              vk::Format::eR64G64B64Uint,
              vk::Format::eR64G64B64Sfloat,
              vk::Format::eR16G16B16A16Uint,
              vk::Format::eR32G32B32A32Uint,
              vk::Format::eR32G32B32A32Sfloat,
              vk::Format::eR64G64B64A64Uint,
              vk::Format::eR64G64B64A64Sfloat
            };
            return instance;
          }
          else if( v.attr == integer_attribute_t::normalized ) {
            static const std::vector< vk::Format > instance{
              vk::Format::eR16G16B16Unorm,
              vk::Format::eR16G16B16A16Unorm
            };
            return instance;
          }
          else if( v.attr == integer_attribute_t::scaled ) {
            static const std::vector< vk::Format > instance{
              vk::Format::eR16G16B16Uscaled,
              vk::Format::eR16G16B16A16Uscaled
            };
            return instance;
          }
        }
        else if( v.depth <= 32u ) {
          if( v.attr == integer_attribute_t::asis ) {
            static const std::vector< vk::Format > instance{
              vk::Format::eR32G32B32Uint,
              vk::Format::eR64G64B64Uint,
              vk::Format::eR64G64B64Sfloat,
              vk::Format::eR32G32B32A32Uint,
              vk::Format::eR64G64B64A64Uint,
              vk::Format::eR64G64B64A64Sfloat
            };
            return instance;
          }
        }
        else if( v.depth <= 64u ) {
          if( v.attr == integer_attribute_t::asis ) {
            static const std::vector< vk::Format > instance{
              vk::Format::eR64G64B64Uint,
              vk::Format::eR64G64B64A64Uint
            };
            return instance;
          }
        }
      }
    }
    if( v.component == numeric_component_type_t::float_ ) {
      if( v.depth <= 16u ) {
        static const std::vector< vk::Format > instance{
          vk::Format::eR16G16B16Sfloat,
          vk::Format::eR32G32B32Sfloat,
          vk::Format::eR64G64B64Sfloat,
          vk::Format::eR16G16B16A16Sfloat,
          vk::Format::eR32G32B32A32Sfloat,
          vk::Format::eR64G64B64A64Sfloat
        };
        return instance;
      }
      else if( v.depth <= 32u ) {
        static const std::vector< vk::Format > instance{
          vk::Format::eR32G32B32Sfloat,
          vk::Format::eR64G64B64Sfloat,
          vk::Format::eR32G32B32A32Sfloat,
          vk::Format::eR64G64B64A64Sfloat
        };
        return instance;
      }
      else if( v.depth <= 64u ) {
        static const std::vector< vk::Format > instance{
          vk::Format::eR64G64B64Sfloat,
          vk::Format::eR64G64B64A64Sfloat
        };
        return instance;
      }
      else {
        static const std::vector< vk::Format > instance;
        return instance;
      }
    }
    static const std::vector< vk::Format > instance;
    return instance;
  }
  const std::vector< vk::Format > &get_compatible_4d_format( const numeric_type_t &v ) {
    if( v.component == numeric_component_type_t::bool_ ) {
      static const std::vector< vk::Format > instance{
        vk::Format::eR8G8B8A8Uint,
        vk::Format::eR8G8B8A8Sint,
        vk::Format::eR8G8B8A8Unorm,
        vk::Format::eR8G8B8A8Snorm,
        vk::Format::eR8G8B8A8Uscaled,
        vk::Format::eR8G8B8A8Sscaled,
        vk::Format::eR16G16B16A16Unorm,
        vk::Format::eR16G16B16A16Snorm,
        vk::Format::eR16G16B16A16Uscaled,
        vk::Format::eR16G16B16A16Sscaled,
        vk::Format::eR16G16B16A16Uint,
        vk::Format::eR16G16B16A16Sint,
        vk::Format::eR16G16B16A16Sfloat,
        vk::Format::eR32G32B32A32Uint,
        vk::Format::eR32G32B32A32Sint,
        vk::Format::eR32G32B32A32Sfloat,
        vk::Format::eR64G64B64A64Uint,
        vk::Format::eR64G64B64A64Sint,
        vk::Format::eR64G64B64A64Sfloat
      };
      return instance;
    }
    if( v.component == numeric_component_type_t::int_ ) {
      if( v.sign ) {
        if( v.depth <= 8u ) {
          if( v.attr == integer_attribute_t::asis ) {
            static const std::vector< vk::Format > instance{
              vk::Format::eR8G8B8A8Sint,
              vk::Format::eR16G16B16A16Sint,
              vk::Format::eR16G16B16A16Sfloat,
              vk::Format::eR32G32B32A32Sint,
              vk::Format::eR32G32B32A32Sfloat,
              vk::Format::eR64G64B64A64Sint,
              vk::Format::eR64G64B64A64Sfloat
            };
            return instance;
          }
          else if( v.attr == integer_attribute_t::normalized ) {
            static const std::vector< vk::Format > instance{
              vk::Format::eR8G8B8A8Snorm,
              vk::Format::eR16G16B16A16Snorm
            };
            return instance;
          }
          else if( v.attr == integer_attribute_t::scaled ) {
            static const std::vector< vk::Format > instance{
              vk::Format::eR8G8B8A8Sscaled,
              vk::Format::eR16G16B16A16Sscaled
            };
            return instance;
          }
        }
        else if( v.depth <= 16u ) {
          if( v.attr == integer_attribute_t::asis ) {
            static const std::vector< vk::Format > instance{
              vk::Format::eR16G16B16A16Sint,
              vk::Format::eR32G32B32A32Sint,
              vk::Format::eR32G32B32A32Sfloat,
              vk::Format::eR64G64B64A64Sint,
              vk::Format::eR64G64B64A64Sfloat
            };
            return instance;
          }
          else if( v.attr == integer_attribute_t::normalized ) {
            static const std::vector< vk::Format > instance{
              vk::Format::eR16G16B16A16Snorm
            };
            return instance;
          }
          else if( v.attr == integer_attribute_t::scaled ) {
            static const std::vector< vk::Format > instance{
              vk::Format::eR16G16B16A16Sscaled
            };
            return instance;
          }
        }
        else if( v.depth <= 32u ) {
          if( v.attr == integer_attribute_t::asis ) {
            static const std::vector< vk::Format > instance{
              vk::Format::eR32G32B32A32Sint,
              vk::Format::eR64G64B64A64Sint,
              vk::Format::eR64G64B64A64Sfloat
            };
            return instance;
          }
        }
        else if( v.depth <= 64u ) {
          if( v.attr == integer_attribute_t::asis ) {
            static const std::vector< vk::Format > instance{
              vk::Format::eR64G64B64A64Sint
            };
            return instance;
          }
        }
      }
      else {
        if( v.depth <= 8u ) {
          if( v.attr == integer_attribute_t::asis ) {
            static const std::vector< vk::Format > instance{
              vk::Format::eR8G8B8A8Uint,
              vk::Format::eR16G16B16A16Uint,
              vk::Format::eR16G16B16A16Sfloat,
              vk::Format::eR32G32B32A32Uint,
              vk::Format::eR32G32B32A32Sfloat,
              vk::Format::eR64G64B64A64Uint,
              vk::Format::eR64G64B64A64Sfloat
            };
            return instance;
          }
          else if( v.attr == integer_attribute_t::normalized ) {
            static const std::vector< vk::Format > instance{
              vk::Format::eR8G8B8A8Unorm,
              vk::Format::eR16G16B16A16Unorm
            };
            return instance;
          }
          else if( v.attr == integer_attribute_t::scaled ) {
            static const std::vector< vk::Format > instance{
              vk::Format::eR8G8B8A8Uscaled,
              vk::Format::eR16G16B16A16Uscaled
            };
            return instance;
          }
          else if( v.attr == integer_attribute_t::srgb ) {
            static const std::vector< vk::Format > instance{
              vk::Format::eR8G8B8A8Srgb
            };
            return instance;
          }
        }
        else if( v.depth <= 16u ) {
          if( v.attr == integer_attribute_t::asis ) {
            static const std::vector< vk::Format > instance{
              vk::Format::eR16G16B16A16Uint,
              vk::Format::eR32G32B32A32Uint,
              vk::Format::eR32G32B32A32Sfloat,
              vk::Format::eR64G64B64A64Uint,
              vk::Format::eR64G64B64A64Sfloat
            };
            return instance;
          }
          else if( v.attr == integer_attribute_t::normalized ) {
            static const std::vector< vk::Format > instance{
              vk::Format::eR16G16B16A16Unorm
            };
            return instance;
          }
          else if( v.attr == integer_attribute_t::scaled ) {
            static const std::vector< vk::Format > instance{
              vk::Format::eR16G16B16A16Uscaled
            };
            return instance;
          }
        }
        else if( v.depth <= 32u ) {
          if( v.attr == integer_attribute_t::asis ) {
            static const std::vector< vk::Format > instance{
              vk::Format::eR32G32B32A32Uint,
              vk::Format::eR64G64B64A64Uint,
              vk::Format::eR64G64B64A64Sfloat
            };
            return instance;
          }
        }
        else if( v.depth <= 64u ) {
          if( v.attr == integer_attribute_t::asis ) {
            static const std::vector< vk::Format > instance{
              vk::Format::eR64G64B64Uint,
              vk::Format::eR64G64B64A64Uint
            };
            return instance;
          }
        }
      }
    }
    if( v.component == numeric_component_type_t::float_ ) {
      if( v.depth <= 16u ) {
        static const std::vector< vk::Format > instance{
          vk::Format::eR16G16B16A16Sfloat,
          vk::Format::eR32G32B32A32Sfloat,
          vk::Format::eR64G64B64A64Sfloat
        };
        return instance;
      }
      else if( v.depth <= 32u ) {
        static const std::vector< vk::Format > instance{
          vk::Format::eR32G32B32A32Sfloat,
          vk::Format::eR64G64B64A64Sfloat
        };
        return instance;
      }
      else if( v.depth <= 64u ) {
        static const std::vector< vk::Format > instance{
          vk::Format::eR64G64B64A64Sfloat
        };
        return instance;
      }
      else {
        static const std::vector< vk::Format > instance;
        return instance;
      }
    }
    static const std::vector< vk::Format > instance;
    return instance;
  }
  std::uint64_t get_one( const numeric_type_t &v ) {
    if( v.component == numeric_component_type_t::bool_ ) {
      return 1ul;
    }
    else if( v.component == numeric_component_type_t::int_ ) {
      if(
        v.attr == integer_attribute_t::normalized ||
        v.attr == integer_attribute_t::srgb
      ) {
        if( v.depth <= 8u ) {
          if( v.sign ) return std::numeric_limits< std::int8_t >::max();
          else return std::numeric_limits< std::uint8_t >::max();
        }
        else if( v.depth <= 16u ) {
          if( v.sign ) return std::numeric_limits< std::int16_t >::max();
          else return std::numeric_limits< std::uint16_t >::max();
        }
        else if( v.depth <= 32u ) {
          if( v.sign ) return std::numeric_limits< std::int32_t >::max();
          else return std::numeric_limits< std::uint32_t >::max();
        }
        else {
          if( v.sign ) return std::numeric_limits< std::int64_t >::max();
          else return std::numeric_limits< std::uint64_t >::max();
        }
      }
      else {
        return 1ul;
      }
    }
    else if( v.component == numeric_component_type_t::float_ ) {
      if( v.depth <= 16u ) {
        return 0x3c00u;
      }
      else if( v.depth <= 32u ) {
        return 0x3f800000ul;
      }
      else {
        return 0x3ff0000000000000ull;
      }
    }
    return 0u;
  }

  const std::vector< vk::Format > &get_compatible_format( const numeric_type_t &v ) {
    if( v.composite == numeric_composite_type_t::matrix ) {
      if( v.rows * v.cols == 1u ) return get_compatible_1d_format( v );
      if( v.rows * v.cols == 2u ) return get_compatible_2d_format( v );
      if( v.rows * v.cols == 3u ) return get_compatible_3d_format( v );
      if( v.rows * v.cols == 4u ) return get_compatible_4d_format( v );
      static const std::vector< vk::Format > instance;
      return instance;
    }
    if( v.composite == numeric_composite_type_t::vector ) {
      if( v.rows == 1u ) return get_compatible_1d_format( v );
      if( v.rows == 2u ) return get_compatible_2d_format( v );
      if( v.rows == 3u ) return get_compatible_3d_format( v );
      if( v.rows == 4u ) return get_compatible_4d_format( v );
      static const std::vector< vk::Format > instance;
      return instance;
    }
    if( v.composite == numeric_composite_type_t::scalar ) {
      return get_compatible_1d_format( v );
    }
    static const std::vector< vk::Format > instance;
    return instance;
  }
  bool operator==( const numeric_type_t &l, const numeric_type_t &r ) {
    return
      l.component == r.component &&
      l.depth == r.depth &&
      l.sign == r.sign &&
      l.attr == r.attr &&
      l.composite == r.composite &&
      l.rows == r.rows &&
      l.cols == r.cols;
  }
  bool operator!=( const numeric_type_t &l, const numeric_type_t &r ) {
    return
      l.component != r.component ||
      l.depth != r.depth ||
      l.sign != r.sign ||
      l.attr != r.attr ||
      l.composite != r.composite ||
      l.rows != r.rows ||
      l.cols != r.cols;
  }
  void to_json( nlohmann::json &dest, const numeric_component_type_t &src ) {
    if( src == numeric_component_type_t::void_ ) dest = "void";
    else if( src == numeric_component_type_t::bool_ ) dest = "bool";
    else if( src == numeric_component_type_t::int_ ) dest = "int";
    else if( src == numeric_component_type_t::float_ ) dest = "float";
    else dest = int( src );
  }
  void from_json( const nlohmann::json &src, numeric_component_type_t &dest ) {
    if( src.is_string() ) {
      if( src == "void" ) dest = numeric_component_type_t::void_;
      else if( src == "bool" ) dest = numeric_component_type_t::bool_;
      else if( src == "int" ) dest = numeric_component_type_t::int_;
      else if( src == "float" ) dest = numeric_component_type_t::float_;
      else throw incompatible_json( "The JSON is incompatible to numeric_component_type_t", __FILE__, __LINE__ );
    }
    else if( src.is_number() ) {
      dest = numeric_component_type_t( int( src ) );
    }
    else throw incompatible_json( "The JSON is incompatible to numeric_component_type_t", __FILE__, __LINE__ );
  }
  void to_json( nlohmann::json &dest, const integer_attribute_t &src ) {
    if( src == integer_attribute_t::asis ) dest = "asis";
    else if( src == integer_attribute_t::normalized ) dest = "normalized";
    else if( src == integer_attribute_t::scaled ) dest = "scaled";
    else if( src == integer_attribute_t::srgb ) dest = "srgb";
    else dest = int( src );
  }
  void from_json( const nlohmann::json &src, integer_attribute_t &dest ) {
    if( src.is_string() ) {
      if( src == "asis" ) dest = integer_attribute_t::asis;
      else if( src == "normalized" ) dest = integer_attribute_t::normalized;
      else if( src == "scaled" ) dest = integer_attribute_t::scaled;
      else if( src == "srgb" ) dest = integer_attribute_t::srgb;
      else throw incompatible_json( "The JSON is incompatible to integer_attribute_t", __FILE__, __LINE__ );
    }
    else if( src.is_number() ) {
      dest = integer_attribute_t( int( src ) );
    }
    else throw incompatible_json( "The JSON is incompatible to integer_attribute_t", __FILE__, __LINE__ );
  }
  void to_json( nlohmann::json &dest, const numeric_composite_type_t &src ) {
    if( src == numeric_composite_type_t::scalar ) dest = "scalar";
    else if( src == numeric_composite_type_t::vector ) dest = "vector";
    else if( src == numeric_composite_type_t::matrix ) dest = "matrix";
    else dest = int( src );
  }
  void from_json( const nlohmann::json &src, numeric_composite_type_t &dest ) {
    if( src.is_string() ) {
      if( src == "scalar" ) dest = numeric_composite_type_t::scalar;
      else if( src == "vector" ) dest = numeric_composite_type_t::vector;
      else if( src == "matrix" ) dest = numeric_composite_type_t::matrix;
      else throw incompatible_json( "The JSON is incompatible to numeric_composite_type_t", __FILE__, __LINE__ );
    }
    else if( src.is_number() ) {
      dest = numeric_composite_type_t( int( src ) );
    }
    else throw incompatible_json( "The JSON is incompatible to numeric_composite_type_t", __FILE__, __LINE__ );
  }
  void to_json( nlohmann::json &dest, const numeric_type_t &src ) {
    dest = nlohmann::json::object();
    dest[ "component" ] = src.component;
    dest[ "depth" ] = src.depth;
    dest[ "sign" ] = src.sign;
    dest[ "attr" ] = src.attr;
    dest[ "composite" ] = src.composite;
    dest[ "rows" ] = src.rows;
    dest[ "cols" ] = src.cols;
  }
  void from_json( const nlohmann::json &src, numeric_type_t &dest ) {
    if( !src.is_object() ) throw incompatible_json( "The JSON is incompatible to numeric_type_t", __FILE__, __LINE__ );
    dest.component = src[ "component" ];
    dest.depth = src[ "depth" ];
    dest.sign = src[ "sign" ];
    dest.attr = src[ "attr" ];
    dest.composite = src[ "composite" ];
    dest.rows = src[ "rows" ];
    dest.cols = src[ "cols" ]; 
  }
}

