#include <gct/format.hpp>

namespace gct {

bool is_hdr( vk::Format f ) {
  if( f == vk::Format::eR16Sfloat ) return true;
  if( f == vk::Format::eR16G16Sfloat ) return true;
  if( f == vk::Format::eR16G16B16Sfloat ) return true;
  if( f == vk::Format::eR16G16B16A16Sfloat ) return true;
  if( f == vk::Format::eR32Sfloat ) return true;
  if( f == vk::Format::eR32G32Sfloat ) return true;
  if( f == vk::Format::eR32G32B32Sfloat ) return true;
  if( f == vk::Format::eR32G32B32A32Sfloat ) return true;
  if( f == vk::Format::eR64Sfloat ) return true;
  if( f == vk::Format::eR64G64Sfloat ) return true;
  if( f == vk::Format::eR64G64B64Sfloat ) return true;
  if( f == vk::Format::eR64G64B64A64Sfloat ) return true;
  if( f == vk::Format::eBc6HUfloatBlock ) return true;
  if( f == vk::Format::eBc6HSfloatBlock ) return true;
#if defined(VK_VERSION_1_3)
  if( f == vk::Format::eAstc4x4SfloatBlock ) return true;
  if( f == vk::Format::eAstc5x4SfloatBlock ) return true;
  if( f == vk::Format::eAstc5x5SfloatBlock ) return true;
  if( f == vk::Format::eAstc6x5SfloatBlock ) return true;
  if( f == vk::Format::eAstc6x6SfloatBlock ) return true;
  if( f == vk::Format::eAstc8x5SfloatBlock ) return true;
  if( f == vk::Format::eAstc8x6SfloatBlock ) return true;
  if( f == vk::Format::eAstc8x8SfloatBlock ) return true;
  if( f == vk::Format::eAstc10x5SfloatBlock ) return true;
  if( f == vk::Format::eAstc10x6SfloatBlock ) return true;
  if( f == vk::Format::eAstc10x8SfloatBlock ) return true;
  if( f == vk::Format::eAstc10x10SfloatBlock ) return true;
  if( f == vk::Format::eAstc12x10SfloatBlock ) return true;
  if( f == vk::Format::eAstc12x12SfloatBlock ) return true;
#endif
#ifdef VK_EXT_TEXTURE_COMPRESSION_ASTC_3D_EXTENSION_NAME
  if( f == vk::Format::eAstc3x3x3SfloatBlockEXT ) return true;
  if( f == vk::Format::eAstc4x3x3SfloatBlockEXT ) return true;
  if( f == vk::Format::eAstc4x4x3SfloatBlockEXT ) return true;
  if( f == vk::Format::eAstc4x4x4SfloatBlockEXT ) return true;
  if( f == vk::Format::eAstc5x4x4SfloatBlockEXT ) return true;
  if( f == vk::Format::eAstc5x5x4SfloatBlockEXT ) return true;
  if( f == vk::Format::eAstc5x5x5SfloatBlockEXT ) return true;
  if( f == vk::Format::eAstc6x5x5SfloatBlockEXT ) return true;
  if( f == vk::Format::eAstc6x6x5SfloatBlockEXT ) return true;
  if( f == vk::Format::eAstc6x6x6SfloatBlockEXT ) return true;
#endif
  return false;
}

}

