#include <gct/format.hpp>

namespace gct {

vk::Format remove_srgb( vk::Format f ) {
  if( f == vk::Format::eR8Srgb ) return vk::Format::eR8Unorm;
  if( f == vk::Format::eR8G8Srgb ) return vk::Format::eR8G8Unorm;
  if( f == vk::Format::eR8G8B8Srgb ) return vk::Format::eR8G8B8Unorm;
  if( f == vk::Format::eB8G8R8Srgb ) return vk::Format::eB8G8R8Unorm;
  if( f == vk::Format::eR8G8B8A8Srgb ) return vk::Format::eR8G8B8A8Unorm;
  if( f == vk::Format::eB8G8R8A8Srgb ) return vk::Format::eB8G8R8A8Unorm;
  if( f == vk::Format::eA8B8G8R8SrgbPack32 ) return vk::Format::eA8B8G8R8UnormPack32;
  if( f == vk::Format::eBc1RgbSrgbBlock ) return vk::Format::eBc1RgbUnormBlock;
  if( f == vk::Format::eBc1RgbaSrgbBlock ) return vk::Format::eBc1RgbaUnormBlock;
  if( f == vk::Format::eBc2SrgbBlock ) return vk::Format::eBc2UnormBlock;
  if( f == vk::Format::eBc3SrgbBlock ) return vk::Format::eBc3UnormBlock;
  if( f == vk::Format::eBc7SrgbBlock ) return vk::Format::eBc7UnormBlock;
  if( f == vk::Format::eAstc4x4SrgbBlock ) return vk::Format::eAstc4x4UnormBlock;
  if( f == vk::Format::eAstc5x4SrgbBlock ) return vk::Format::eAstc5x4UnormBlock;
  if( f == vk::Format::eAstc5x5SrgbBlock ) return vk::Format::eAstc5x5UnormBlock;
  if( f == vk::Format::eAstc6x5SrgbBlock ) return vk::Format::eAstc6x5UnormBlock;
  if( f == vk::Format::eAstc6x6SrgbBlock ) return vk::Format::eAstc6x6UnormBlock;
  if( f == vk::Format::eAstc8x5SrgbBlock ) return vk::Format::eAstc8x5UnormBlock;
  if( f == vk::Format::eAstc8x6SrgbBlock ) return vk::Format::eAstc8x6UnormBlock;
  if( f == vk::Format::eAstc8x8SrgbBlock ) return vk::Format::eAstc8x8UnormBlock;
  if( f == vk::Format::eAstc10x5SrgbBlock ) return vk::Format::eAstc10x5UnormBlock;
  if( f == vk::Format::eAstc10x6SrgbBlock ) return vk::Format::eAstc10x6UnormBlock;
  if( f == vk::Format::eAstc10x8SrgbBlock ) return vk::Format::eAstc10x8UnormBlock;
  if( f == vk::Format::eAstc10x10SrgbBlock ) return vk::Format::eAstc10x10UnormBlock;
  if( f == vk::Format::eAstc12x10SrgbBlock ) return vk::Format::eAstc12x10UnormBlock;
  if( f == vk::Format::eAstc12x12SrgbBlock ) return vk::Format::eAstc12x12UnormBlock;
#ifdef VK_IMG_FORMAT_PVRTC_EXTENSION_NAME
  if( f == vk::Format::ePvrtc12BppSrgbBlockIMG ) return vk::Format::ePvrtc12BppUnormBlockIMG;
  if( f == vk::Format::ePvrtc14BppSrgbBlockIMG ) return vk::Format::ePvrtc14BppUnormBlockIMG;
  if( f == vk::Format::ePvrtc22BppSrgbBlockIMG ) return vk::Format::ePvrtc22BppUnormBlockIMG;
  if( f == vk::Format::ePvrtc24BppSrgbBlockIMG ) return vk::Format::ePvrtc24BppUnormBlockIMG;
#endif
#ifdef VK_EXT_TEXTURE_COMPRESSION_ASTC_3D_EXTENSION_NAME
  if( f == vk::Format::eAstc3x3x3SrgbBlockEXT ) return vk::Format::eAstc3x3x3UnormBlockEXT;
  if( f == vk::Format::eAstc4x3x3SrgbBlockEXT ) return vk::Format::eAstc4x3x3UnormBlockEXT;
  if( f == vk::Format::eAstc4x4x3SrgbBlockEXT ) return vk::Format::eAstc4x4x3UnormBlockEXT;
  if( f == vk::Format::eAstc4x4x4SrgbBlockEXT ) return vk::Format::eAstc4x4x4UnormBlockEXT;
  if( f == vk::Format::eAstc5x4x4SrgbBlockEXT ) return vk::Format::eAstc5x4x4UnormBlockEXT;
  if( f == vk::Format::eAstc5x5x4SrgbBlockEXT ) return vk::Format::eAstc5x5x4UnormBlockEXT;
  if( f == vk::Format::eAstc5x5x5SrgbBlockEXT ) return vk::Format::eAstc5x5x5UnormBlockEXT;
  if( f == vk::Format::eAstc6x5x5SrgbBlockEXT ) return vk::Format::eAstc6x5x5UnormBlockEXT;
  if( f == vk::Format::eAstc6x6x5SrgbBlockEXT ) return vk::Format::eAstc6x6x5UnormBlockEXT;
  if( f == vk::Format::eAstc6x6x6SrgbBlockEXT ) return vk::Format::eAstc6x6x6UnormBlockEXT;
#endif
  return f;
}

}


