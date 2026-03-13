#include <gct/format.hpp>

namespace gct {
  unsigned int format_to_block_width( vk::Format format ) {
    if( format == vk::Format::eUndefined) return 0u;
    else if( format == vk::Format::eBc1RgbUnormBlock) return 4u;
    else if( format == vk::Format::eBc1RgbSrgbBlock) return 4u;
    else if( format == vk::Format::eBc1RgbaUnormBlock) return 4u;
    else if( format == vk::Format::eBc1RgbaSrgbBlock) return 4u;
    else if( format == vk::Format::eBc2UnormBlock) return 4u;
    else if( format == vk::Format::eBc2SrgbBlock) return 4u;
    else if( format == vk::Format::eBc3UnormBlock) return 4u;
    else if( format == vk::Format::eBc3SrgbBlock) return 4u;
    else if( format == vk::Format::eBc4UnormBlock) return 4u;
    else if( format == vk::Format::eBc4SnormBlock) return 4u;
    else if( format == vk::Format::eBc5UnormBlock) return 4u;
    else if( format == vk::Format::eBc5SnormBlock) return 4u;
    else if( format == vk::Format::eBc6HUfloatBlock) return 4u;
    else if( format == vk::Format::eBc6HSfloatBlock) return 4u;
    else if( format == vk::Format::eBc7UnormBlock) return 4u;
    else if( format == vk::Format::eBc7SrgbBlock) return 4u;
    else if( format == vk::Format::eEtc2R8G8B8UnormBlock) return 4u;
    else if( format == vk::Format::eEtc2R8G8B8SrgbBlock) return 4u;
    else if( format == vk::Format::eEtc2R8G8B8A1UnormBlock) return 4u;
    else if( format == vk::Format::eEtc2R8G8B8A1SrgbBlock) return 4u;
    else if( format == vk::Format::eEtc2R8G8B8A8UnormBlock) return 4u;
    else if( format == vk::Format::eEtc2R8G8B8A8SrgbBlock) return 4u;
    else if( format == vk::Format::eEacR11UnormBlock) return 4u;
    else if( format == vk::Format::eEacR11SnormBlock) return 4u;
    else if( format == vk::Format::eEacR11G11UnormBlock) return 4u;
    else if( format == vk::Format::eEacR11G11SnormBlock) return 4u;
    else if( format == vk::Format::eAstc4x4UnormBlock) return 4u;
    else if( format == vk::Format::eAstc4x4SrgbBlock) return 4u;
    else if( format == vk::Format::eAstc5x4UnormBlock) return 5u;
    else if( format == vk::Format::eAstc5x4SrgbBlock) return 5u;
    else if( format == vk::Format::eAstc5x5UnormBlock) return 5u;
    else if( format == vk::Format::eAstc5x5SrgbBlock) return 6u;
    else if( format == vk::Format::eAstc6x5UnormBlock) return 6u;
    else if( format == vk::Format::eAstc6x5SrgbBlock) return 6u;
    else if( format == vk::Format::eAstc6x6UnormBlock) return 6u;
    else if( format == vk::Format::eAstc6x6SrgbBlock) return 8u;
    else if( format == vk::Format::eAstc8x5UnormBlock) return 8u;
    else if( format == vk::Format::eAstc8x5SrgbBlock) return 8u;
    else if( format == vk::Format::eAstc8x6UnormBlock) return 8u;
    else if( format == vk::Format::eAstc8x6SrgbBlock) return 8u;
    else if( format == vk::Format::eAstc8x8UnormBlock) return 8u;
    else if( format == vk::Format::eAstc8x8SrgbBlock) return 8u;
    else if( format == vk::Format::eAstc10x5UnormBlock) return 10u;
    else if( format == vk::Format::eAstc10x5SrgbBlock) return 10u;
    else if( format == vk::Format::eAstc10x6UnormBlock) return 10u;
    else if( format == vk::Format::eAstc10x6SrgbBlock) return 10u;
    else if( format == vk::Format::eAstc10x8UnormBlock) return 10u;
    else if( format == vk::Format::eAstc10x8SrgbBlock) return 10u;
    else if( format == vk::Format::eAstc10x10UnormBlock) return 10u;
    else if( format == vk::Format::eAstc10x10SrgbBlock) return 10u;
    else if( format == vk::Format::eAstc12x10UnormBlock) return 12u;
    else if( format == vk::Format::eAstc12x10SrgbBlock) return 12u;
    else if( format == vk::Format::eAstc12x12UnormBlock) return 12u;
    else if( format == vk::Format::eAstc12x12SrgbBlock) return 12u;
#ifdef VK_IMG_FORMAT_PVRTC_EXTENSION_NAME
    else if( format == vk::Format::ePvrtc12BppUnormBlockIMG) return 8u;
    else if( format == vk::Format::ePvrtc14BppUnormBlockIMG) return 4u;
    else if( format == vk::Format::ePvrtc22BppUnormBlockIMG) return 8u;
    else if( format == vk::Format::ePvrtc24BppUnormBlockIMG) return 4u;
    else if( format == vk::Format::ePvrtc12BppSrgbBlockIMG) return 8u;
    else if( format == vk::Format::ePvrtc14BppSrgbBlockIMG) return 4u;
    else if( format == vk::Format::ePvrtc22BppSrgbBlockIMG) return 8u;
    else if( format == vk::Format::ePvrtc24BppSrgbBlockIMG) return 4u;
#endif
#ifdef VK_EXT_TEXTURE_COMPRESSION_ASTC_HDR_EXTENSION_NAME
    else if( format == vk::Format::eAstc4x4SfloatBlockEXT) return 4u;
    else if( format == vk::Format::eAstc5x4SfloatBlockEXT) return 5u;
    else if( format == vk::Format::eAstc5x5SfloatBlockEXT) return 5u;
    else if( format == vk::Format::eAstc6x5SfloatBlockEXT) return 6u;
    else if( format == vk::Format::eAstc6x6SfloatBlockEXT) return 6u;
    else if( format == vk::Format::eAstc8x5SfloatBlockEXT) return 8u;
    else if( format == vk::Format::eAstc8x6SfloatBlockEXT) return 8u;
    else if( format == vk::Format::eAstc8x8SfloatBlockEXT) return 8u;
    else if( format == vk::Format::eAstc10x5SfloatBlockEXT) return 10u;
    else if( format == vk::Format::eAstc10x6SfloatBlockEXT) return 10u;
    else if( format == vk::Format::eAstc10x8SfloatBlockEXT) return 10u;
    else if( format == vk::Format::eAstc10x10SfloatBlockEXT) return 10u;
    else if( format == vk::Format::eAstc12x10SfloatBlockEXT) return 12u;
    else if( format == vk::Format::eAstc12x12SfloatBlockEXT) return 12u;
#endif
    else return 1u;
  }
  unsigned int format_to_block_height( vk::Format format ) {
    if( format == vk::Format::eUndefined) return 0u;
    else if( format == vk::Format::eBc1RgbUnormBlock) return 4u;
    else if( format == vk::Format::eBc1RgbSrgbBlock) return 4u;
    else if( format == vk::Format::eBc1RgbaUnormBlock) return 4u;
    else if( format == vk::Format::eBc1RgbaSrgbBlock) return 4u;
    else if( format == vk::Format::eBc2UnormBlock) return 4u;
    else if( format == vk::Format::eBc2SrgbBlock) return 4u;
    else if( format == vk::Format::eBc3UnormBlock) return 4u;
    else if( format == vk::Format::eBc3SrgbBlock) return 4u;
    else if( format == vk::Format::eBc4UnormBlock) return 4u;
    else if( format == vk::Format::eBc4SnormBlock) return 4u;
    else if( format == vk::Format::eBc5UnormBlock) return 4u;
    else if( format == vk::Format::eBc5SnormBlock) return 4u;
    else if( format == vk::Format::eBc6HUfloatBlock) return 4u;
    else if( format == vk::Format::eBc6HSfloatBlock) return 4u;
    else if( format == vk::Format::eBc7UnormBlock) return 4u;
    else if( format == vk::Format::eBc7SrgbBlock) return 4u;
    else if( format == vk::Format::eEtc2R8G8B8UnormBlock) return 4u;
    else if( format == vk::Format::eEtc2R8G8B8SrgbBlock) return 4u;
    else if( format == vk::Format::eEtc2R8G8B8A1UnormBlock) return 4u;
    else if( format == vk::Format::eEtc2R8G8B8A1SrgbBlock) return 4u;
    else if( format == vk::Format::eEtc2R8G8B8A8UnormBlock) return 4u;
    else if( format == vk::Format::eEtc2R8G8B8A8SrgbBlock) return 4u;
    else if( format == vk::Format::eEacR11UnormBlock) return 4u;
    else if( format == vk::Format::eEacR11SnormBlock) return 4u;
    else if( format == vk::Format::eEacR11G11UnormBlock) return 4u;
    else if( format == vk::Format::eEacR11G11SnormBlock) return 4u;
    else if( format == vk::Format::eAstc4x4UnormBlock) return 4u;
    else if( format == vk::Format::eAstc4x4SrgbBlock) return 4u;
    else if( format == vk::Format::eAstc5x4UnormBlock) return 4u;
    else if( format == vk::Format::eAstc5x4SrgbBlock) return 4u;
    else if( format == vk::Format::eAstc5x5UnormBlock) return 5u;
    else if( format == vk::Format::eAstc5x5SrgbBlock) return 5u;
    else if( format == vk::Format::eAstc6x5UnormBlock) return 5u;
    else if( format == vk::Format::eAstc6x5SrgbBlock) return 5u;
    else if( format == vk::Format::eAstc6x6UnormBlock) return 6u;
    else if( format == vk::Format::eAstc6x6SrgbBlock) return 6u;
    else if( format == vk::Format::eAstc8x5UnormBlock) return 5u;
    else if( format == vk::Format::eAstc8x5SrgbBlock) return 5u;
    else if( format == vk::Format::eAstc8x6UnormBlock) return 6u;
    else if( format == vk::Format::eAstc8x6SrgbBlock) return 6u;
    else if( format == vk::Format::eAstc8x8UnormBlock) return 8u;
    else if( format == vk::Format::eAstc8x8SrgbBlock) return 8u;
    else if( format == vk::Format::eAstc10x5UnormBlock) return 5u;
    else if( format == vk::Format::eAstc10x5SrgbBlock) return 5u;
    else if( format == vk::Format::eAstc10x6UnormBlock) return 6u;
    else if( format == vk::Format::eAstc10x6SrgbBlock) return 6u;
    else if( format == vk::Format::eAstc10x8UnormBlock) return 8u;
    else if( format == vk::Format::eAstc10x8SrgbBlock) return 8u;
    else if( format == vk::Format::eAstc10x10UnormBlock) return 10u;
    else if( format == vk::Format::eAstc10x10SrgbBlock) return 10u;
    else if( format == vk::Format::eAstc12x10UnormBlock) return 10u;
    else if( format == vk::Format::eAstc12x10SrgbBlock) return 10u;
    else if( format == vk::Format::eAstc12x12UnormBlock) return 12u;
    else if( format == vk::Format::eAstc12x12SrgbBlock) return 12u;
#ifdef VK_IMG_FORMAT_PVRTC_EXTENSION_NAME
    else if( format == vk::Format::ePvrtc12BppUnormBlockIMG) return 4u;
    else if( format == vk::Format::ePvrtc14BppUnormBlockIMG) return 4u;
    else if( format == vk::Format::ePvrtc22BppUnormBlockIMG) return 4u;
    else if( format == vk::Format::ePvrtc24BppUnormBlockIMG) return 4u;
    else if( format == vk::Format::ePvrtc12BppSrgbBlockIMG) return 4u;
    else if( format == vk::Format::ePvrtc14BppSrgbBlockIMG) return 4u;
    else if( format == vk::Format::ePvrtc22BppSrgbBlockIMG) return 4u;
    else if( format == vk::Format::ePvrtc24BppSrgbBlockIMG) return 4u;
#endif
#ifdef VK_EXT_TEXTURE_COMPRESSION_ASTC_HDR_EXTENSION_NAME
    else if( format == vk::Format::eAstc4x4SfloatBlockEXT) return 4u;
    else if( format == vk::Format::eAstc5x4SfloatBlockEXT) return 4u;
    else if( format == vk::Format::eAstc5x5SfloatBlockEXT) return 5u;
    else if( format == vk::Format::eAstc6x5SfloatBlockEXT) return 5u;
    else if( format == vk::Format::eAstc6x6SfloatBlockEXT) return 6u;
    else if( format == vk::Format::eAstc8x5SfloatBlockEXT) return 5u;
    else if( format == vk::Format::eAstc8x6SfloatBlockEXT) return 6u;
    else if( format == vk::Format::eAstc8x8SfloatBlockEXT) return 8u;
    else if( format == vk::Format::eAstc10x5SfloatBlockEXT) return 5u;
    else if( format == vk::Format::eAstc10x6SfloatBlockEXT) return 6u;
    else if( format == vk::Format::eAstc10x8SfloatBlockEXT) return 8u;
    else if( format == vk::Format::eAstc10x10SfloatBlockEXT) return 10u;
    else if( format == vk::Format::eAstc12x10SfloatBlockEXT) return 10u;
    else if( format == vk::Format::eAstc12x12SfloatBlockEXT) return 12u;
#endif
    else return 1u;
  }
}

