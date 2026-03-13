#include <gct/format.hpp>

namespace gct {

vk::Format add_srgb( vk::Format f ) {
  if( f == vk::Format::eR8Unorm ) return vk::Format::eR8Srgb;
  if( f == vk::Format::eR8G8Unorm ) return vk::Format::eR8G8Srgb;
  if( f == vk::Format::eR8G8B8Unorm ) return vk::Format::eR8G8B8Srgb;
  if( f == vk::Format::eB8G8R8Unorm ) return vk::Format::eB8G8R8Srgb;
  if( f == vk::Format::eR8G8B8A8Unorm ) return vk::Format::eR8G8B8A8Srgb;
  if( f == vk::Format::eB8G8R8A8Unorm ) return vk::Format::eB8G8R8A8Srgb;
  if( f == vk::Format::eA8B8G8R8UnormPack32 ) return vk::Format::eA8B8G8R8SrgbPack32;
  if( f == vk::Format::eBc1RgbUnormBlock ) return vk::Format::eBc1RgbSrgbBlock;
  if( f == vk::Format::eBc1RgbaUnormBlock ) return vk::Format::eBc1RgbaSrgbBlock;
  if( f == vk::Format::eBc2UnormBlock ) return vk::Format::eBc2SrgbBlock;
  if( f == vk::Format::eBc3UnormBlock ) return vk::Format::eBc3SrgbBlock;
  if( f == vk::Format::eBc7UnormBlock ) return vk::Format::eBc7SrgbBlock;
  if( f == vk::Format::eAstc4x4UnormBlock ) return vk::Format::eAstc4x4SrgbBlock;
  if( f == vk::Format::eAstc5x4UnormBlock ) return vk::Format::eAstc5x4SrgbBlock;
  if( f == vk::Format::eAstc5x5UnormBlock ) return vk::Format::eAstc5x5SrgbBlock;
  if( f == vk::Format::eAstc6x5UnormBlock ) return vk::Format::eAstc6x5SrgbBlock;
  if( f == vk::Format::eAstc6x6UnormBlock ) return vk::Format::eAstc6x6SrgbBlock;
  if( f == vk::Format::eAstc8x5UnormBlock ) return vk::Format::eAstc8x5SrgbBlock;
  if( f == vk::Format::eAstc8x6UnormBlock ) return vk::Format::eAstc8x6SrgbBlock;
  if( f == vk::Format::eAstc8x8UnormBlock ) return vk::Format::eAstc8x8SrgbBlock;
  if( f == vk::Format::eAstc10x5UnormBlock ) return vk::Format::eAstc10x5SrgbBlock;
  if( f == vk::Format::eAstc10x6UnormBlock ) return vk::Format::eAstc10x6SrgbBlock;
  if( f == vk::Format::eAstc10x8UnormBlock ) return vk::Format::eAstc10x8SrgbBlock;
  if( f == vk::Format::eAstc10x10UnormBlock ) return vk::Format::eAstc10x10SrgbBlock;
  if( f == vk::Format::eAstc12x10UnormBlock ) return vk::Format::eAstc12x10SrgbBlock;
  if( f == vk::Format::eAstc12x12UnormBlock ) return vk::Format::eAstc12x12SrgbBlock;
#ifdef VK_IMG_FORMAT_PVRTC_EXTENSION_NAME
  if( f == vk::Format::ePvrtc12BppUnormBlockIMG ) return vk::Format::ePvrtc12BppSrgbBlockIMG;
  if( f == vk::Format::ePvrtc14BppUnormBlockIMG ) return vk::Format::ePvrtc14BppSrgbBlockIMG;
  if( f == vk::Format::ePvrtc22BppUnormBlockIMG ) return vk::Format::ePvrtc22BppSrgbBlockIMG;
  if( f == vk::Format::ePvrtc24BppUnormBlockIMG ) return vk::Format::ePvrtc24BppSrgbBlockIMG;
#endif
#ifdef VK_EXT_TEXTURE_COMPRESSION_ASTC_3D_EXTENSION_NAME
  if( f == vk::Format::eAstc3x3x3UnormBlockEXT ) return vk::Format::eAstc3x3x3SrgbBlockEXT;
  if( f == vk::Format::eAstc4x3x3UnormBlockEXT ) return vk::Format::eAstc4x3x3SrgbBlockEXT;
  if( f == vk::Format::eAstc4x4x3UnormBlockEXT ) return vk::Format::eAstc4x4x3SrgbBlockEXT;
  if( f == vk::Format::eAstc4x4x4UnormBlockEXT ) return vk::Format::eAstc4x4x4SrgbBlockEXT;
  if( f == vk::Format::eAstc5x4x4UnormBlockEXT ) return vk::Format::eAstc5x4x4SrgbBlockEXT;
  if( f == vk::Format::eAstc5x5x4UnormBlockEXT ) return vk::Format::eAstc5x5x4SrgbBlockEXT;
  if( f == vk::Format::eAstc5x5x5UnormBlockEXT ) return vk::Format::eAstc5x5x5SrgbBlockEXT;
  if( f == vk::Format::eAstc6x5x5UnormBlockEXT ) return vk::Format::eAstc6x5x5SrgbBlockEXT;
  if( f == vk::Format::eAstc6x6x5UnormBlockEXT ) return vk::Format::eAstc6x6x5SrgbBlockEXT;
  if( f == vk::Format::eAstc6x6x6UnormBlockEXT ) return vk::Format::eAstc6x6x6SrgbBlockEXT;
#endif
  return f;
}

}

