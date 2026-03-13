#include <gct/format.hpp>

namespace gct {

std::vector< vk::Format >
get_compatible_format(
  vk::Format f
) {
  if(
#if defined(VK_VERSION_1_3)
    f == vk::Format::eA4R4G4B4UnormPack16 ||
    f == vk::Format::eA4B4G4R4UnormPack16 ||
#endif
    f == vk::Format::eR4G4B4A4UnormPack16 ||
    f == vk::Format::eB4G4R4A4UnormPack16
  ) {
    return {
#if defined(VK_VERSION_1_3)
      vk::Format::eA4R4G4B4UnormPack16,
      vk::Format::eA4B4G4R4UnormPack16,
#endif
      vk::Format::eR4G4B4A4UnormPack16,
      vk::Format::eB4G4R4A4UnormPack16
    };
  }
  if(
    f == vk::Format::eR5G6B5UnormPack16 ||
    f == vk::Format::eB5G6R5UnormPack16
  ) {
    return {
      vk::Format::eR5G6B5UnormPack16,
      vk::Format::eB5G6R5UnormPack16
    };
  }
#if defined(VK_VERSION_1_4)
  if(
    f == vk::Format::eA1R5G5B5UnormPack16 ||
    f == vk::Format::eA1B5G5R5UnormPack16
  ) {
    return {
      vk::Format::eA1R5G5B5UnormPack16,
      vk::Format::eA1B5G5R5UnormPack16
    };
  }
#endif
  if(
    f == vk::Format::eR5G5B5A1UnormPack16 ||
    f == vk::Format::eB5G5R5A1UnormPack16
  ) {
    return {
      vk::Format::eR5G5B5A1UnormPack16,
      vk::Format::eB5G5R5A1UnormPack16
    };
  }
  if(
#if defined(VK_VERSION_1_4)
    f == vk::Format::eA8Unorm ||
#endif
    f == vk::Format::eR8Unorm ||
    f == vk::Format::eR8Snorm ||
    f == vk::Format::eR8Uscaled ||
    f == vk::Format::eR8Sscaled ||
    f == vk::Format::eR8Uint ||
    f == vk::Format::eR8Sint ||
    f == vk::Format::eR8Srgb
  ) {
    return {
#if defined(VK_VERSION_1_4)
      vk::Format::eA8Unorm,
#endif
      vk::Format::eR8Unorm,
      vk::Format::eR8Snorm,
      vk::Format::eR8Uscaled,
      vk::Format::eR8Sscaled,
      vk::Format::eR8Uint,
      vk::Format::eR8Sint,
      vk::Format::eR8Srgb
    };
  }
  if(
    f == vk::Format::eR8G8B8Unorm ||
    f == vk::Format::eR8G8B8Snorm ||
    f == vk::Format::eR8G8B8Uscaled ||
    f == vk::Format::eR8G8B8Sscaled ||
    f == vk::Format::eR8G8B8Uint ||
    f == vk::Format::eR8G8B8Sint ||
    f == vk::Format::eR8G8B8Srgb ||
    f == vk::Format::eB8G8R8Unorm ||
    f == vk::Format::eB8G8R8Snorm ||
    f == vk::Format::eB8G8R8Uscaled ||
    f == vk::Format::eB8G8R8Sscaled ||
    f == vk::Format::eB8G8R8Uint ||
    f == vk::Format::eB8G8R8Sint ||
    f == vk::Format::eB8G8R8Srgb
  ) {
    return {
      vk::Format::eR8G8B8Unorm,
      vk::Format::eR8G8B8Snorm,
      vk::Format::eR8G8B8Uscaled,
      vk::Format::eR8G8B8Sscaled,
      vk::Format::eR8G8B8Uint,
      vk::Format::eR8G8B8Sint,
      vk::Format::eR8G8B8Srgb,
      vk::Format::eB8G8R8Unorm,
      vk::Format::eB8G8R8Snorm,
      vk::Format::eB8G8R8Uscaled,
      vk::Format::eB8G8R8Sscaled,
      vk::Format::eB8G8R8Uint,
      vk::Format::eB8G8R8Sint,
      vk::Format::eB8G8R8Srgb
    };
  }
  if(
    f == vk::Format::eR8G8B8A8Unorm ||
    f == vk::Format::eR8G8B8A8Snorm ||
    f == vk::Format::eR8G8B8A8Uscaled ||
    f == vk::Format::eR8G8B8A8Sscaled ||
    f == vk::Format::eR8G8B8A8Uint ||
    f == vk::Format::eR8G8B8A8Sint ||
    f == vk::Format::eR8G8B8A8Srgb ||
    f == vk::Format::eB8G8R8A8Unorm ||
    f == vk::Format::eB8G8R8A8Snorm ||
    f == vk::Format::eB8G8R8A8Uscaled ||
    f == vk::Format::eB8G8R8A8Sscaled ||
    f == vk::Format::eB8G8R8A8Uint ||
    f == vk::Format::eB8G8R8A8Sint ||
    f == vk::Format::eB8G8R8A8Srgb ||
    f == vk::Format::eA8B8G8R8UnormPack32 ||
    f == vk::Format::eA8B8G8R8SnormPack32 ||
    f == vk::Format::eA8B8G8R8UscaledPack32 ||
    f == vk::Format::eA8B8G8R8SscaledPack32 ||
    f == vk::Format::eA8B8G8R8UintPack32 ||
    f == vk::Format::eA8B8G8R8SintPack32 ||
    f == vk::Format::eA8B8G8R8SrgbPack32
  ) {
    return {
      vk::Format::eR8G8B8A8Unorm,
      vk::Format::eR8G8B8A8Snorm,
      vk::Format::eR8G8B8A8Uscaled,
      vk::Format::eR8G8B8A8Sscaled,
      vk::Format::eR8G8B8A8Uint,
      vk::Format::eR8G8B8A8Sint,
      vk::Format::eR8G8B8A8Srgb,
      vk::Format::eB8G8R8A8Unorm,
      vk::Format::eB8G8R8A8Snorm,
      vk::Format::eB8G8R8A8Uscaled,
      vk::Format::eB8G8R8A8Sscaled,
      vk::Format::eB8G8R8A8Uint,
      vk::Format::eB8G8R8A8Sint,
      vk::Format::eB8G8R8A8Srgb,
      vk::Format::eA8B8G8R8UnormPack32,
      vk::Format::eA8B8G8R8SnormPack32,
      vk::Format::eA8B8G8R8UscaledPack32,
      vk::Format::eA8B8G8R8SscaledPack32,
      vk::Format::eA8B8G8R8UintPack32,
      vk::Format::eA8B8G8R8SintPack32,
      vk::Format::eA8B8G8R8SrgbPack32
    };
  }
  if(
    f == vk::Format::eA2B10G10R10UnormPack32 ||
    f == vk::Format::eA2B10G10R10SnormPack32 ||
    f == vk::Format::eA2B10G10R10UscaledPack32 ||
    f == vk::Format::eA2B10G10R10SscaledPack32 ||
    f == vk::Format::eA2B10G10R10UintPack32 ||
    f == vk::Format::eA2B10G10R10SintPack32 ||
    f == vk::Format::eA2R10G10B10UnormPack32 ||
    f == vk::Format::eA2R10G10B10SnormPack32 ||
    f == vk::Format::eA2R10G10B10UscaledPack32 ||
    f == vk::Format::eA2R10G10B10SscaledPack32 ||
    f == vk::Format::eA2R10G10B10UintPack32 ||
    f == vk::Format::eA2R10G10B10SintPack32
  ) {
    return {
      vk::Format::eA2B10G10R10UnormPack32,
      vk::Format::eA2B10G10R10SnormPack32,
      vk::Format::eA2B10G10R10UscaledPack32,
      vk::Format::eA2B10G10R10SscaledPack32,
      vk::Format::eA2B10G10R10UintPack32,
      vk::Format::eA2B10G10R10SintPack32,
      vk::Format::eA2R10G10B10UnormPack32,
      vk::Format::eA2R10G10B10SnormPack32,
      vk::Format::eA2R10G10B10UscaledPack32,
      vk::Format::eA2R10G10B10SscaledPack32,
      vk::Format::eA2R10G10B10UintPack32,
      vk::Format::eA2R10G10B10SintPack32
    };
  }
  if(
    f == vk::Format::eR16Unorm ||
    f == vk::Format::eR16Snorm ||
    f == vk::Format::eR16Uscaled ||
    f == vk::Format::eR16Sscaled ||
    f == vk::Format::eR16Uint ||
    f == vk::Format::eR16Sint
  ) {
    return {
      vk::Format::eR16Unorm,
      vk::Format::eR16Snorm,
      vk::Format::eR16Uscaled,
      vk::Format::eR16Sscaled,
      vk::Format::eR16Uint,
      vk::Format::eR16Sint
    };
  }
  if(
    f == vk::Format::eR16G16Unorm ||
    f == vk::Format::eR16G16Snorm ||
    f == vk::Format::eR16G16Uscaled ||
    f == vk::Format::eR16G16Sscaled ||
    f == vk::Format::eR16G16Uint ||
    f == vk::Format::eR16G16Sint
  ) {
    return {
      vk::Format::eR16G16Unorm,
      vk::Format::eR16G16Snorm,
      vk::Format::eR16G16Uscaled,
      vk::Format::eR16G16Sscaled,
      vk::Format::eR16G16Uint,
      vk::Format::eR16G16Sint
    };
  }
  if(
    f == vk::Format::eR16G16B16Unorm ||
    f == vk::Format::eR16G16B16Snorm ||
    f == vk::Format::eR16G16B16Uscaled ||
    f == vk::Format::eR16G16B16Sscaled ||
    f == vk::Format::eR16G16B16Uint ||
    f == vk::Format::eR16G16B16Sint
  ) {
    return {
      vk::Format::eR16G16B16Unorm,
      vk::Format::eR16G16B16Snorm,
      vk::Format::eR16G16B16Uscaled,
      vk::Format::eR16G16B16Sscaled,
      vk::Format::eR16G16B16Uint,
      vk::Format::eR16G16B16Sint
    };
  }
  if(
    f == vk::Format::eR16G16B16A16Unorm ||
    f == vk::Format::eR16G16B16A16Snorm ||
    f == vk::Format::eR16G16B16A16Uscaled ||
    f == vk::Format::eR16G16B16A16Sscaled ||
    f == vk::Format::eR16G16B16A16Uint ||
    f == vk::Format::eR16G16B16A16Sint
  ) {
    return {
      vk::Format::eR16G16B16A16Unorm,
      vk::Format::eR16G16B16A16Snorm,
      vk::Format::eR16G16B16A16Uscaled,
      vk::Format::eR16G16B16A16Sscaled,
      vk::Format::eR16G16B16A16Uint,
      vk::Format::eR16G16B16A16Sint
    };
  }
  if(
    f == vk::Format::eR32Uint ||
    f == vk::Format::eR32Sint
  ) {
    return {
      vk::Format::eR32Uint,
      vk::Format::eR32Sint
    };
  }
  if(
    f == vk::Format::eR32G32Uint ||
    f == vk::Format::eR32G32Sint
  ) {
    return {
      vk::Format::eR32G32Uint,
      vk::Format::eR32G32Sint
    };
  }
  if(
    f == vk::Format::eR32G32B32Uint ||
    f == vk::Format::eR32G32B32Sint
  ) {
    return {
      vk::Format::eR32G32B32Uint,
      vk::Format::eR32G32B32Sint
    };
  }
  if(
    f == vk::Format::eR32G32B32A32Uint ||
    f == vk::Format::eR32G32B32A32Sint
  ) {
    return {
      vk::Format::eR32G32B32A32Uint,
      vk::Format::eR32G32B32A32Sint
    };
  }
  if(
    f == vk::Format::eR64Uint ||
    f == vk::Format::eR64Sint
  ) {
    return {
      vk::Format::eR64Uint,
      vk::Format::eR64Sint
    };
  }
  if(
    f == vk::Format::eR64G64Uint ||
    f == vk::Format::eR64G64Sint
  ) {
    return {
      vk::Format::eR64G64Uint,
      vk::Format::eR64G64Sint
    };
  }
  if(
    f == vk::Format::eR64G64B64Uint ||
    f == vk::Format::eR64G64B64Sint
  ) {
    return {
      vk::Format::eR64G64B64Uint,
      vk::Format::eR64G64B64Sint
    };
  }
  if(
    f == vk::Format::eR64G64B64A64Uint ||
    f == vk::Format::eR64G64B64A64Sint
  ) {
    return {
      vk::Format::eR64G64B64A64Uint,
      vk::Format::eR64G64B64A64Sint
    };
  }
  if(
    f == vk::Format::eBc1RgbUnormBlock ||
    f == vk::Format::eBc1RgbSrgbBlock
  ) {
    return {
      vk::Format::eBc1RgbUnormBlock,
      vk::Format::eBc1RgbSrgbBlock
    };
  }
  if(
    f == vk::Format::eBc1RgbaUnormBlock ||
    f == vk::Format::eBc1RgbaSrgbBlock
  ) {
    return {
      vk::Format::eBc1RgbaUnormBlock,
      vk::Format::eBc1RgbaSrgbBlock
    };
  }
  if(
    f == vk::Format::eBc2UnormBlock ||
    f == vk::Format::eBc2SrgbBlock
  ) {
    return {
      vk::Format::eBc2UnormBlock,
      vk::Format::eBc2SrgbBlock
    };
  }
  if(
    f == vk::Format::eBc3UnormBlock ||
    f == vk::Format::eBc3SrgbBlock
  ) {
    return {
      vk::Format::eBc3UnormBlock,
      vk::Format::eBc3SrgbBlock
    };
  }
  if(
    f == vk::Format::eBc4UnormBlock ||
    f == vk::Format::eBc4SnormBlock
  ) {
    return {
      vk::Format::eBc4UnormBlock,
      vk::Format::eBc4SnormBlock
    };
  }
  if(
    f == vk::Format::eBc5UnormBlock ||
    f == vk::Format::eBc5SnormBlock
  ) {
    return {
      vk::Format::eBc5UnormBlock,
      vk::Format::eBc5SnormBlock
    };
  }
  if(
    f == vk::Format::eBc7UnormBlock ||
    f == vk::Format::eBc7SrgbBlock
  ) {
    return {
      vk::Format::eBc7UnormBlock,
      vk::Format::eBc7SrgbBlock
    };
  }
  if(
    f == vk::Format::eEtc2R8G8B8UnormBlock ||
    f == vk::Format::eEtc2R8G8B8SrgbBlock
  ) {
    return {
      vk::Format::eEtc2R8G8B8UnormBlock,
      vk::Format::eEtc2R8G8B8SrgbBlock
    };
  }
  if(
    f == vk::Format::eEtc2R8G8B8A1UnormBlock ||
    f == vk::Format::eEtc2R8G8B8A1SrgbBlock
  ) {
    return {
      vk::Format::eEtc2R8G8B8A1UnormBlock,
      vk::Format::eEtc2R8G8B8A1SrgbBlock
    };
  }
  if(
    f == vk::Format::eEtc2R8G8B8A8UnormBlock ||
    f == vk::Format::eEtc2R8G8B8A8SrgbBlock
  ) {
    return {
      vk::Format::eEtc2R8G8B8A8UnormBlock,
      vk::Format::eEtc2R8G8B8A8SrgbBlock
    };
  }
  if(
    f == vk::Format::eEacR11UnormBlock ||
    f == vk::Format::eEacR11SnormBlock
  ) {
    return {
      vk::Format::eEacR11UnormBlock,
      vk::Format::eEacR11SnormBlock
    };
  }
  if(
    f == vk::Format::eAstc4x4UnormBlock ||
    f == vk::Format::eAstc4x4SrgbBlock
  ) {
    return {
      vk::Format::eAstc4x4UnormBlock,
      vk::Format::eAstc4x4SrgbBlock
    };
  }
  if(
    f == vk::Format::eAstc5x4UnormBlock ||
    f == vk::Format::eAstc5x4SrgbBlock
  ) {
    return {
      vk::Format::eAstc5x4UnormBlock,
      vk::Format::eAstc5x4SrgbBlock
    };
  }
  if(
    f == vk::Format::eAstc5x5UnormBlock ||
    f == vk::Format::eAstc5x5SrgbBlock
  ) {
    return {
      vk::Format::eAstc5x5UnormBlock,
      vk::Format::eAstc5x5SrgbBlock
    };
  }
  if(
    f == vk::Format::eAstc6x5UnormBlock ||
    f == vk::Format::eAstc6x5SrgbBlock
  ) {
    return {
      vk::Format::eAstc6x5UnormBlock,
      vk::Format::eAstc6x5SrgbBlock
    };
  }
  if(
    f == vk::Format::eAstc6x6UnormBlock ||
    f == vk::Format::eAstc6x6SrgbBlock
  ) {
    return {
      vk::Format::eAstc6x6UnormBlock,
      vk::Format::eAstc6x6SrgbBlock
    };
  }
  if(
    f == vk::Format::eAstc8x6UnormBlock ||
    f == vk::Format::eAstc8x6SrgbBlock
  ) {
    return {
      vk::Format::eAstc8x6UnormBlock,
      vk::Format::eAstc8x6SrgbBlock
    };
  }
  if(
    f == vk::Format::eAstc8x8UnormBlock ||
    f == vk::Format::eAstc8x8SrgbBlock
  ) {
    return {
      vk::Format::eAstc8x8UnormBlock,
      vk::Format::eAstc8x8SrgbBlock
    };
  }
  if(
    f == vk::Format::eAstc10x5UnormBlock ||
    f == vk::Format::eAstc10x5SrgbBlock
  ) {
    return {
      vk::Format::eAstc10x5UnormBlock,
      vk::Format::eAstc10x5SrgbBlock
    };
  }
  if(
    f == vk::Format::eAstc10x6UnormBlock ||
    f == vk::Format::eAstc10x6SrgbBlock
  ) {
    return {
      vk::Format::eAstc10x6UnormBlock,
      vk::Format::eAstc10x6SrgbBlock
    };
  }
  if(
    f == vk::Format::eAstc10x8UnormBlock ||
    f == vk::Format::eAstc10x8SrgbBlock
  ) {
    return {
      vk::Format::eAstc10x8UnormBlock,
      vk::Format::eAstc10x8SrgbBlock
    };
  }
  if(
    f == vk::Format::eAstc10x10UnormBlock ||
    f == vk::Format::eAstc10x10SrgbBlock
  ) {
    return {
      vk::Format::eAstc10x10UnormBlock,
      vk::Format::eAstc10x10SrgbBlock
    };
  }
  if(
    f == vk::Format::eAstc12x10UnormBlock ||
    f == vk::Format::eAstc12x10SrgbBlock
  ) {
    return {
      vk::Format::eAstc12x10UnormBlock,
      vk::Format::eAstc12x10SrgbBlock
    };
  }
  if(
    f == vk::Format::eAstc12x12UnormBlock ||
    f == vk::Format::eAstc12x12SrgbBlock
  ) {
    return {
      vk::Format::eAstc12x12UnormBlock,
      vk::Format::eAstc12x12SrgbBlock
    };
  }
#ifdef VK_IMG_FORMAT_PVRTC_EXTENSION_NAME
  if(
    f == vk::Format::ePvrtc12BppUnormBlockIMG ||
    f == vk::Format::ePvrtc12BppSrgbBlockIMG
  ) {
    return {
      vk::Format::ePvrtc12BppUnormBlockIMG,
      vk::Format::ePvrtc12BppSrgbBlockIMG
    };
  }
  if(
    f == vk::Format::ePvrtc14BppUnormBlockIMG ||
    f == vk::Format::ePvrtc14BppSrgbBlockIMG
  ) {
    return {
      vk::Format::ePvrtc14BppUnormBlockIMG,
      vk::Format::ePvrtc14BppSrgbBlockIMG
    };
  }
  if(
    f == vk::Format::ePvrtc22BppUnormBlockIMG ||
    f == vk::Format::ePvrtc22BppSrgbBlockIMG
  ) {
    return {
      vk::Format::ePvrtc22BppUnormBlockIMG,
      vk::Format::ePvrtc22BppSrgbBlockIMG
    };
  }
  if(
    f == vk::Format::ePvrtc24BppUnormBlockIMG ||
    f == vk::Format::ePvrtc24BppSrgbBlockIMG
  ) {
    return {
      vk::Format::ePvrtc24BppUnormBlockIMG,
      vk::Format::ePvrtc24BppSrgbBlockIMG
    };
  }
#endif
#ifdef VK_EXT_TEXTURE_COMPRESSION_ASTC_3D_EXTENSION_NAME
  if(
    f == vk::Format::eAstc3x3x3UnormBlockEXT ||
    f == vk::Format::eAstc3x3x3SrgbBlockEXT
  ) {
    return {
      vk::Format::eAstc3x3x3UnormBlockEXT,
      vk::Format::eAstc3x3x3SrgbBlockEXT
    };
  }
  if(
    f == vk::Format::eAstc4x3x3UnormBlockEXT ||
    f == vk::Format::eAstc4x3x3SrgbBlockEXT
  ) {
    return {
      vk::Format::eAstc4x3x3UnormBlockEXT,
      vk::Format::eAstc4x3x3SrgbBlockEXT
    };
  }
  if(
    f == vk::Format::eAstc4x4x3UnormBlockEXT ||
    f == vk::Format::eAstc4x4x3SrgbBlockEXT
  ) {
    return {
      vk::Format::eAstc4x4x3UnormBlockEXT,
      vk::Format::eAstc4x4x3SrgbBlockEXT
    };
  }
  if(
    f == vk::Format::eAstc4x4x4UnormBlockEXT ||
    f == vk::Format::eAstc4x4x4SrgbBlockEXT
  ) {
    return {
      vk::Format::eAstc4x4x4UnormBlockEXT,
      vk::Format::eAstc4x4x4SrgbBlockEXT
    };
  }
  if(
    f == vk::Format::eAstc5x4x4UnormBlockEXT ||
    f == vk::Format::eAstc5x4x4SrgbBlockEXT
  ) {
    return {
      vk::Format::eAstc5x4x4UnormBlockEXT,
      vk::Format::eAstc5x4x4SrgbBlockEXT
    };
  }
  if(
    f == vk::Format::eAstc5x5x4UnormBlockEXT ||
    f == vk::Format::eAstc5x5x4SrgbBlockEXT
  ) {
    return {
      vk::Format::eAstc5x5x4UnormBlockEXT,
      vk::Format::eAstc5x5x4SrgbBlockEXT
    };
  }
  if(
    f == vk::Format::eAstc5x5x5UnormBlockEXT ||
    f == vk::Format::eAstc5x5x5SrgbBlockEXT
  ) {
    return {
      vk::Format::eAstc5x5x5UnormBlockEXT,
      vk::Format::eAstc5x5x5SrgbBlockEXT
    };
  }
  if(
    f == vk::Format::eAstc6x5x5UnormBlockEXT ||
    f == vk::Format::eAstc6x5x5SrgbBlockEXT
  ) {
    return {
      vk::Format::eAstc6x5x5UnormBlockEXT,
      vk::Format::eAstc6x5x5SrgbBlockEXT
    };
  }
  if(
    f == vk::Format::eAstc6x6x5UnormBlockEXT ||
    f == vk::Format::eAstc6x6x5SrgbBlockEXT
  ) {
    return {
      vk::Format::eAstc6x6x5UnormBlockEXT,
      vk::Format::eAstc6x6x5SrgbBlockEXT
    };
  }
  if(
    f == vk::Format::eAstc6x6x6UnormBlockEXT ||
    f == vk::Format::eAstc6x6x6SrgbBlockEXT
  ) {
    return {
      vk::Format::eAstc6x6x6UnormBlockEXT,
      vk::Format::eAstc6x6x6SrgbBlockEXT
    };
  }
#endif
  return { f };
}

}

