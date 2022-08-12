#include <gct/format.hpp>

namespace gct {
  bool is_srgb( vk::Format format ) {
    if( format == vk::Format::eR8Srgb) return true;
    else if( format == vk::Format::eR8G8Srgb) return true;
    else if( format == vk::Format::eR8G8B8Srgb) return true;
    else if( format == vk::Format::eB8G8R8Srgb) return true;
    else if( format == vk::Format::eR8G8B8A8Srgb) return true;
    else if( format == vk::Format::eB8G8R8A8Srgb) return true;
    else if( format == vk::Format::eA8B8G8R8SrgbPack32) return true;
    else if( format == vk::Format::eBc1RgbSrgbBlock) return true;
    else if( format == vk::Format::eBc1RgbaSrgbBlock) return true;
    else if( format == vk::Format::eBc2SrgbBlock) return true;
    else if( format == vk::Format::eBc3SrgbBlock) return true;
    else if( format == vk::Format::eBc7SrgbBlock) return true;
    else if( format == vk::Format::eEtc2R8G8B8SrgbBlock) return true;
    else if( format == vk::Format::eEtc2R8G8B8A1SrgbBlock) return true;
    else if( format == vk::Format::eEtc2R8G8B8A8SrgbBlock) return true;
    else if( format == vk::Format::eAstc4x4SrgbBlock) return true;
    else if( format == vk::Format::eAstc5x4SrgbBlock) return true;
    else if( format == vk::Format::eAstc5x5SrgbBlock) return true;
    else if( format == vk::Format::eAstc6x5SrgbBlock) return true;
    else if( format == vk::Format::eAstc6x6SrgbBlock) return true;
    else if( format == vk::Format::eAstc8x5SrgbBlock) return true;
    else if( format == vk::Format::eAstc8x6SrgbBlock) return true;
    else if( format == vk::Format::eAstc8x8SrgbBlock) return true;
    else if( format == vk::Format::eAstc10x5SrgbBlock) return true;
    else if( format == vk::Format::eAstc10x6SrgbBlock) return true;
    else if( format == vk::Format::eAstc10x8SrgbBlock) return true;
    else if( format == vk::Format::eAstc10x10SrgbBlock) return true;
    else if( format == vk::Format::eAstc12x10SrgbBlock) return true;
    else if( format == vk::Format::eAstc12x12SrgbBlock) return true;
#ifdef VK_IMG_FORMAT_PVRTC_EXTENSION_NAME
    else if( format == vk::Format::ePvrtc12BppSrgbBlockIMG) return true;
    else if( format == vk::Format::ePvrtc14BppSrgbBlockIMG) return true;
    else if( format == vk::Format::ePvrtc22BppSrgbBlockIMG) return true;
    else if( format == vk::Format::ePvrtc24BppSrgbBlockIMG) return true;
#endif
    else return false;
  }
}

