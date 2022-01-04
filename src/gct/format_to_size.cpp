#include <gct/format.hpp>

namespace gct {
  unsigned int format_to_size( vk::Format format ) {
    if( format == vk::Format::eUndefined) return 0u;
    else if( format == vk::Format::eR4G4UnormPack8) return 1u;
    else if( format == vk::Format::eR4G4B4A4UnormPack16) return 2u;
    else if( format == vk::Format::eB4G4R4A4UnormPack16) return 2u;
    else if( format == vk::Format::eR5G6B5UnormPack16) return 2u;
    else if( format == vk::Format::eB5G6R5UnormPack16) return 2u;
    else if( format == vk::Format::eR5G5B5A1UnormPack16) return 2u;
    else if( format == vk::Format::eB5G5R5A1UnormPack16) return 2u;
    else if( format == vk::Format::eA1R5G5B5UnormPack16) return 2u;
    else if( format == vk::Format::eR8Unorm) return 1u;
    else if( format == vk::Format::eR8Snorm) return 1u;
    else if( format == vk::Format::eR8Uscaled) return 1u;
    else if( format == vk::Format::eR8Sscaled) return 1u;
    else if( format == vk::Format::eR8Uint) return 1u;
    else if( format == vk::Format::eR8Sint) return 1u;
    else if( format == vk::Format::eR8Srgb) return 1u;
    else if( format == vk::Format::eR8G8Unorm) return 2u;
    else if( format == vk::Format::eR8G8Snorm) return 2u;
    else if( format == vk::Format::eR8G8Uscaled) return 2u;
    else if( format == vk::Format::eR8G8Sscaled) return 2u;
    else if( format == vk::Format::eR8G8Uint) return 2u;
    else if( format == vk::Format::eR8G8Sint) return 2u;
    else if( format == vk::Format::eR8G8Srgb) return 2u;
    else if( format == vk::Format::eR8G8B8Unorm) return 3u;
    else if( format == vk::Format::eR8G8B8Snorm) return 3u;
    else if( format == vk::Format::eR8G8B8Uscaled) return 3u;
    else if( format == vk::Format::eR8G8B8Sscaled) return 3u;
    else if( format == vk::Format::eR8G8B8Uint) return 3u;
    else if( format == vk::Format::eR8G8B8Sint) return 3u;
    else if( format == vk::Format::eR8G8B8Srgb) return 3u;
    else if( format == vk::Format::eB8G8R8Unorm) return 3u;
    else if( format == vk::Format::eB8G8R8Snorm) return 3u;
    else if( format == vk::Format::eB8G8R8Uscaled) return 3u;
    else if( format == vk::Format::eB8G8R8Sscaled) return 3u;
    else if( format == vk::Format::eB8G8R8Uint) return 3u;
    else if( format == vk::Format::eB8G8R8Sint) return 3u;
    else if( format == vk::Format::eB8G8R8Srgb) return 3u;
    else if( format == vk::Format::eR8G8B8A8Unorm) return 4u;
    else if( format == vk::Format::eR8G8B8A8Snorm) return 4u;
    else if( format == vk::Format::eR8G8B8A8Uscaled) return 4u;
    else if( format == vk::Format::eR8G8B8A8Sscaled) return 4u;
    else if( format == vk::Format::eR8G8B8A8Uint) return 4u;
    else if( format == vk::Format::eR8G8B8A8Sint) return 4u;
    else if( format == vk::Format::eR8G8B8A8Srgb) return 4u;
    else if( format == vk::Format::eB8G8R8A8Unorm) return 4u;
    else if( format == vk::Format::eB8G8R8A8Snorm) return 4u;
    else if( format == vk::Format::eB8G8R8A8Uscaled) return 4u;
    else if( format == vk::Format::eB8G8R8A8Sscaled) return 4u;
    else if( format == vk::Format::eB8G8R8A8Uint) return 4u;
    else if( format == vk::Format::eB8G8R8A8Sint) return 4u;
    else if( format == vk::Format::eB8G8R8A8Srgb) return 4u;
    else if( format == vk::Format::eA8B8G8R8UnormPack32) return 4u;
    else if( format == vk::Format::eA8B8G8R8SnormPack32) return 4u;
    else if( format == vk::Format::eA8B8G8R8UscaledPack32) return 4u;
    else if( format == vk::Format::eA8B8G8R8SscaledPack32) return 4u;
    else if( format == vk::Format::eA8B8G8R8UintPack32) return 4u;
    else if( format == vk::Format::eA8B8G8R8SintPack32) return 4u;
    else if( format == vk::Format::eA8B8G8R8SrgbPack32) return 4u;
    else if( format == vk::Format::eA2R10G10B10UnormPack32) return 4u;
    else if( format == vk::Format::eA2R10G10B10SnormPack32) return 4u;
    else if( format == vk::Format::eA2R10G10B10UscaledPack32) return 4u;
    else if( format == vk::Format::eA2R10G10B10SscaledPack32) return 4u;
    else if( format == vk::Format::eA2R10G10B10UintPack32) return 4u;
    else if( format == vk::Format::eA2R10G10B10SintPack32) return 4u;
    else if( format == vk::Format::eA2B10G10R10UnormPack32) return 4u;
    else if( format == vk::Format::eA2B10G10R10SnormPack32) return 4u;
    else if( format == vk::Format::eA2B10G10R10UscaledPack32) return 4u;
    else if( format == vk::Format::eA2B10G10R10SscaledPack32) return 4u;
    else if( format == vk::Format::eA2B10G10R10UintPack32) return 4u;
    else if( format == vk::Format::eA2B10G10R10SintPack32) return 4u;
    else if( format == vk::Format::eR16Unorm) return 2u;
    else if( format == vk::Format::eR16Snorm) return 2u;
    else if( format == vk::Format::eR16Uscaled) return 2u;
    else if( format == vk::Format::eR16Sscaled) return 2u;
    else if( format == vk::Format::eR16Uint) return 2u;
    else if( format == vk::Format::eR16Sint) return 2u;
    else if( format == vk::Format::eR16Sfloat) return 2u;
    else if( format == vk::Format::eR16G16Unorm) return 4u;
    else if( format == vk::Format::eR16G16Snorm) return 4u;
    else if( format == vk::Format::eR16G16Uscaled) return 4u;
    else if( format == vk::Format::eR16G16Sscaled) return 4u;
    else if( format == vk::Format::eR16G16Uint) return 4u;
    else if( format == vk::Format::eR16G16Sint) return 4u;
    else if( format == vk::Format::eR16G16Sfloat) return 4u;
    else if( format == vk::Format::eR16G16B16Unorm) return 6u;
    else if( format == vk::Format::eR16G16B16Snorm) return 6u;
    else if( format == vk::Format::eR16G16B16Uscaled) return 6u;
    else if( format == vk::Format::eR16G16B16Sscaled) return 6u;
    else if( format == vk::Format::eR16G16B16Uint) return 6u;
    else if( format == vk::Format::eR16G16B16Sint) return 6u;
    else if( format == vk::Format::eR16G16B16Sfloat) return 6u;
    else if( format == vk::Format::eR16G16B16A16Unorm) return 8u;
    else if( format == vk::Format::eR16G16B16A16Snorm) return 8u;
    else if( format == vk::Format::eR16G16B16A16Uscaled) return 8u;
    else if( format == vk::Format::eR16G16B16A16Sscaled) return 8u;
    else if( format == vk::Format::eR16G16B16A16Uint) return 8u;
    else if( format == vk::Format::eR16G16B16A16Sint) return 8u;
    else if( format == vk::Format::eR16G16B16A16Sfloat) return 8u;
    else if( format == vk::Format::eR32Uint) return 4u;
    else if( format == vk::Format::eR32Sint) return 4u;
    else if( format == vk::Format::eR32Sfloat) return 4u;
    else if( format == vk::Format::eR32G32Uint) return 8u;
    else if( format == vk::Format::eR32G32Sint) return 8u;
    else if( format == vk::Format::eR32G32Sfloat) return 8u;
    else if( format == vk::Format::eR32G32B32Uint) return 12u;
    else if( format == vk::Format::eR32G32B32Sint) return 12u;
    else if( format == vk::Format::eR32G32B32Sfloat) return 12u;
    else if( format == vk::Format::eR32G32B32A32Uint) return 16u;
    else if( format == vk::Format::eR32G32B32A32Sint) return 16u;
    else if( format == vk::Format::eR32G32B32A32Sfloat) return 16u;
    else if( format == vk::Format::eR64Uint) return 8u;
    else if( format == vk::Format::eR64Sint) return 8u;
    else if( format == vk::Format::eR64Sfloat) return 8u;
    else if( format == vk::Format::eR64G64Uint) return 16u;
    else if( format == vk::Format::eR64G64Sint) return 16u;
    else if( format == vk::Format::eR64G64Sfloat) return 16u;
    else if( format == vk::Format::eR64G64B64Uint) return 24u;
    else if( format == vk::Format::eR64G64B64Sint) return 24u;
    else if( format == vk::Format::eR64G64B64Sfloat) return 24u;
    else if( format == vk::Format::eR64G64B64A64Uint) return 32u;
    else if( format == vk::Format::eR64G64B64A64Sint) return 32u;
    else if( format == vk::Format::eR64G64B64A64Sfloat) return 32u;
    else if( format == vk::Format::eB10G11R11UfloatPack32) return 4u;
    else if( format == vk::Format::eE5B9G9R9UfloatPack32) return 4u;
    else if( format == vk::Format::eD16Unorm) return 2u;
    else if( format == vk::Format::eX8D24UnormPack32) return 4u;
    else if( format == vk::Format::eD32Sfloat) return 4u;
    else if( format == vk::Format::eS8Uint) return 1u;
    else if( format == vk::Format::eD16UnormS8Uint) return 3u;
    else if( format == vk::Format::eD24UnormS8Uint) return 4u;
    else if( format == vk::Format::eD32SfloatS8Uint) return 8u;
    else if( format == vk::Format::eBc1RgbUnormBlock) return 8u;
    else if( format == vk::Format::eBc1RgbSrgbBlock) return 8u;
    else if( format == vk::Format::eBc1RgbaUnormBlock) return 8u;
    else if( format == vk::Format::eBc1RgbaSrgbBlock) return 8u;
    else if( format == vk::Format::eBc2UnormBlock) return 16u;
    else if( format == vk::Format::eBc2SrgbBlock) return 16u;
    else if( format == vk::Format::eBc3UnormBlock) return 16u;
    else if( format == vk::Format::eBc3SrgbBlock) return 16u;
    else if( format == vk::Format::eBc4UnormBlock) return 8u;
    else if( format == vk::Format::eBc4SnormBlock) return 8u;
    else if( format == vk::Format::eBc5UnormBlock) return 16u;
    else if( format == vk::Format::eBc5SnormBlock) return 16u;
    else if( format == vk::Format::eBc6HUfloatBlock) return 16u;
    else if( format == vk::Format::eBc6HSfloatBlock) return 16u;
    else if( format == vk::Format::eBc7UnormBlock) return 16u;
    else if( format == vk::Format::eBc7SrgbBlock) return 16u;
    else if( format == vk::Format::eEtc2R8G8B8UnormBlock) return 8u;
    else if( format == vk::Format::eEtc2R8G8B8SrgbBlock) return 8u;
    else if( format == vk::Format::eEtc2R8G8B8A1UnormBlock) return 8u;
    else if( format == vk::Format::eEtc2R8G8B8A1SrgbBlock) return 8u;
    else if( format == vk::Format::eEtc2R8G8B8A8UnormBlock) return 16u;
    else if( format == vk::Format::eEtc2R8G8B8A8SrgbBlock) return 16u;
    else if( format == vk::Format::eEacR11UnormBlock) return 8u;
    else if( format == vk::Format::eEacR11SnormBlock) return 8u;
    else if( format == vk::Format::eEacR11G11UnormBlock) return 16u;
    else if( format == vk::Format::eEacR11G11SnormBlock) return 16u;
    else if( format == vk::Format::eAstc4x4UnormBlock) return 16u;
    else if( format == vk::Format::eAstc4x4SrgbBlock) return 16u;
    else if( format == vk::Format::eAstc5x4UnormBlock) return 16u;
    else if( format == vk::Format::eAstc5x4SrgbBlock) return 16u;
    else if( format == vk::Format::eAstc5x5UnormBlock) return 16u;
    else if( format == vk::Format::eAstc5x5SrgbBlock) return 16u;
    else if( format == vk::Format::eAstc6x5UnormBlock) return 16u;
    else if( format == vk::Format::eAstc6x5SrgbBlock) return 16u;
    else if( format == vk::Format::eAstc6x6UnormBlock) return 16u;
    else if( format == vk::Format::eAstc6x6SrgbBlock) return 16u;
    else if( format == vk::Format::eAstc8x5UnormBlock) return 16u;
    else if( format == vk::Format::eAstc8x5SrgbBlock) return 16u;
    else if( format == vk::Format::eAstc8x6UnormBlock) return 16u;
    else if( format == vk::Format::eAstc8x6SrgbBlock) return 16u;
    else if( format == vk::Format::eAstc8x8UnormBlock) return 16u;
    else if( format == vk::Format::eAstc8x8SrgbBlock) return 16u;
    else if( format == vk::Format::eAstc10x5UnormBlock) return 16u;
    else if( format == vk::Format::eAstc10x5SrgbBlock) return 16u;
    else if( format == vk::Format::eAstc10x6UnormBlock) return 16u;
    else if( format == vk::Format::eAstc10x6SrgbBlock) return 16u;
    else if( format == vk::Format::eAstc10x8UnormBlock) return 16u;
    else if( format == vk::Format::eAstc10x8SrgbBlock) return 16u;
    else if( format == vk::Format::eAstc10x10UnormBlock) return 16u;
    else if( format == vk::Format::eAstc10x10SrgbBlock) return 16u;
    else if( format == vk::Format::eAstc12x10UnormBlock) return 16u;
    else if( format == vk::Format::eAstc12x10SrgbBlock) return 16u;
    else if( format == vk::Format::eAstc12x12UnormBlock) return 16u;
    else if( format == vk::Format::eAstc12x12SrgbBlock) return 16u;
#ifdef VK_VERSION_1_1
    else if( format == vk::Format::eG8B8G8R8422Unorm) return 4u;
    else if( format == vk::Format::eB8G8R8G8422Unorm) return 4u;
    else if( format == vk::Format::eG8B8R83Plane420Unorm) return 0u;
    else if( format == vk::Format::eG8B8R82Plane420Unorm) return 0u;
    else if( format == vk::Format::eG8B8R83Plane422Unorm) return 0u;
    else if( format == vk::Format::eG8B8R82Plane422Unorm) return 0u;
    else if( format == vk::Format::eG8B8R83Plane444Unorm) return 0u;
    else if( format == vk::Format::eR10X6UnormPack16) return 2u;
    else if( format == vk::Format::eR10X6G10X6Unorm2Pack16) return 4u;
    else if( format == vk::Format::eR10X6G10X6B10X6A10X6Unorm4Pack16) return 8u;
    else if( format == vk::Format::eG10X6B10X6G10X6R10X6422Unorm4Pack16) return 8u;
    else if( format == vk::Format::eB10X6G10X6R10X6G10X6422Unorm4Pack16) return 8u;
    else if( format == vk::Format::eG10X6B10X6R10X63Plane420Unorm3Pack16) return 0u;
    else if( format == vk::Format::eG10X6B10X6R10X62Plane420Unorm3Pack16) return 0u;
    else if( format == vk::Format::eG10X6B10X6R10X63Plane422Unorm3Pack16) return 0u;
    else if( format == vk::Format::eG10X6B10X6R10X62Plane422Unorm3Pack16) return 0u;
    else if( format == vk::Format::eG10X6B10X6R10X63Plane444Unorm3Pack16) return 0u;
    else if( format == vk::Format::eR12X4UnormPack16) return 2u;
    else if( format == vk::Format::eR12X4G12X4Unorm2Pack16) return 4u;
    else if( format == vk::Format::eR12X4G12X4B12X4A12X4Unorm4Pack16) return 6u;
    else if( format == vk::Format::eG12X4B12X4G12X4R12X4422Unorm4Pack16) return 8u;
    else if( format == vk::Format::eB12X4G12X4R12X4G12X4422Unorm4Pack16) return 8u;
    else if( format == vk::Format::eG12X4B12X4R12X43Plane420Unorm3Pack16) return 0u;
    else if( format == vk::Format::eG12X4B12X4R12X42Plane420Unorm3Pack16) return 0u;
    else if( format == vk::Format::eG12X4B12X4R12X43Plane422Unorm3Pack16) return 0u;
    else if( format == vk::Format::eG12X4B12X4R12X42Plane422Unorm3Pack16) return 0u;
    else if( format == vk::Format::eG12X4B12X4R12X43Plane444Unorm3Pack16) return 0u;
    else if( format == vk::Format::eG16B16G16R16422Unorm) return 0u;
    else if( format == vk::Format::eB16G16R16G16422Unorm) return 0u;
    else if( format == vk::Format::eG16B16R163Plane420Unorm) return 0u;
    else if( format == vk::Format::eG16B16R162Plane420Unorm) return 0u;
    else if( format == vk::Format::eG16B16R163Plane422Unorm) return 0u;
    else if( format == vk::Format::eG16B16R162Plane422Unorm) return 0u;
    else if( format == vk::Format::eG16B16R163Plane444Unorm) return 0u;
#endif
#ifdef VK_IMG_FORMAT_PVRTC_EXTENSION_NAME
    else if( format == vk::Format::ePvrtc12BppUnormBlockIMG) return 8u;
    else if( format == vk::Format::ePvrtc14BppUnormBlockIMG) return 8u;
    else if( format == vk::Format::ePvrtc22BppUnormBlockIMG) return 8u;
    else if( format == vk::Format::ePvrtc24BppUnormBlockIMG) return 8u;
    else if( format == vk::Format::ePvrtc12BppSrgbBlockIMG) return 8u;
    else if( format == vk::Format::ePvrtc14BppSrgbBlockIMG) return 8u;
    else if( format == vk::Format::ePvrtc22BppSrgbBlockIMG) return 8u;
    else if( format == vk::Format::ePvrtc24BppSrgbBlockIMG) return 8u;
#endif
#ifdef VK_EXT_TEXTURE_COMPRESSION_ASTC_HDR_EXTENSION_NAME
    else if( format == vk::Format::eAstc4x4SfloatBlockEXT) return 16u;
    else if( format == vk::Format::eAstc5x4SfloatBlockEXT) return 16u;
    else if( format == vk::Format::eAstc5x5SfloatBlockEXT) return 16u;
    else if( format == vk::Format::eAstc6x5SfloatBlockEXT) return 16u;
    else if( format == vk::Format::eAstc6x6SfloatBlockEXT) return 16u;
    else if( format == vk::Format::eAstc8x5SfloatBlockEXT) return 16u;
    else if( format == vk::Format::eAstc8x6SfloatBlockEXT) return 16u;
    else if( format == vk::Format::eAstc8x8SfloatBlockEXT) return 16u;
    else if( format == vk::Format::eAstc10x5SfloatBlockEXT) return 16u;
    else if( format == vk::Format::eAstc10x6SfloatBlockEXT) return 16u;
    else if( format == vk::Format::eAstc10x8SfloatBlockEXT) return 16u;
    else if( format == vk::Format::eAstc10x10SfloatBlockEXT) return 16u;
    else if( format == vk::Format::eAstc12x10SfloatBlockEXT) return 16u;
    else if( format == vk::Format::eAstc12x12SfloatBlockEXT) return 16u;
#endif
#ifdef VK_EXT_YCBCR_2PLANE_444_FORMATS_EXTENSION_NAME
    else if( format == vk::Format::eG8B8R82Plane444UnormEXT) return 0u;
    else if( format == vk::Format::eG10X6B10X6R10X62Plane444Unorm3Pack16EXT) return 0u;
    else if( format == vk::Format::eG12X4B12X4R12X42Plane444Unorm3Pack16EXT) return 0u;
    else if( format == vk::Format::eG16B16R162Plane444UnormEXT) return 0u;
#endif
#ifdef VK_EXT_4444_FORMATS_EXTENSION_NAME
    else if( format == vk::Format::eA4R4G4B4UnormPack16EXT) return 2u;
    else if( format == vk::Format::eA4B4G4R4UnormPack16EXT) return 2u;
#endif
#ifdef VK_KHR_SAMPLER_YCBCR_CONVERSION_EXTENSION_NAME
    else if( format == vk::Format::eB10X6G10X6R10X6G10X6422Unorm4Pack16KHR) return 8u;
    else if( format == vk::Format::eB12X4G12X4R12X4G12X4422Unorm4Pack16KHR) return 8u;
    else if( format == vk::Format::eB16G16R16G16422UnormKHR) return 8u;
    else if( format == vk::Format::eB8G8R8G8422UnormKHR) return 4u;
    else if( format == vk::Format::eG10X6B10X6G10X6R10X6422Unorm4Pack16KHR) return 8u;
    else if( format == vk::Format::eG10X6B10X6R10X62Plane420Unorm3Pack16KHR) return 0u;
    else if( format == vk::Format::eG10X6B10X6R10X62Plane422Unorm3Pack16KHR) return 0u;
    else if( format == vk::Format::eG10X6B10X6R10X63Plane420Unorm3Pack16KHR) return 0u;
    else if( format == vk::Format::eG10X6B10X6R10X63Plane422Unorm3Pack16KHR) return 0u;
    else if( format == vk::Format::eG10X6B10X6R10X63Plane444Unorm3Pack16KHR) return 0u;
    else if( format == vk::Format::eG12X4B12X4G12X4R12X4422Unorm4Pack16KHR) return 8u;
    else if( format == vk::Format::eG12X4B12X4R12X42Plane420Unorm3Pack16KHR) return 0u;
    else if( format == vk::Format::eG12X4B12X4R12X42Plane422Unorm3Pack16KHR) return 0u;
    else if( format == vk::Format::eG12X4B12X4R12X43Plane420Unorm3Pack16KHR) return 0u;
    else if( format == vk::Format::eG12X4B12X4R12X43Plane422Unorm3Pack16KHR) return 0u;
    else if( format == vk::Format::eG12X4B12X4R12X43Plane444Unorm3Pack16KHR) return 0u;
    else if( format == vk::Format::eG16B16G16R16422UnormKHR) return 8u;
    else if( format == vk::Format::eG16B16R162Plane420UnormKHR) return 0u;
    else if( format == vk::Format::eG16B16R162Plane422UnormKHR) return 0u;
    else if( format == vk::Format::eG16B16R163Plane420UnormKHR) return 0u;
    else if( format == vk::Format::eG16B16R163Plane422UnormKHR) return 0u;
    else if( format == vk::Format::eG16B16R163Plane444UnormKHR) return 0u;
    else if( format == vk::Format::eG8B8G8R8422UnormKHR) return 4u;
    else if( format == vk::Format::eG8B8R82Plane420UnormKHR) return 0u;
    else if( format == vk::Format::eG8B8R82Plane422UnormKHR) return 0u;
    else if( format == vk::Format::eG8B8R83Plane420UnormKHR) return 0u;
    else if( format == vk::Format::eG8B8R83Plane422UnormKHR) return 0u;
    else if( format == vk::Format::eG8B8R83Plane444UnormKHR) return 0u;
    else if( format == vk::Format::eR10X6G10X6B10X6A10X6Unorm4Pack16KHR) return 8u;
    else if( format == vk::Format::eR10X6G10X6Unorm2Pack16KHR) return 4u;
    else if( format == vk::Format::eR10X6UnormPack16KHR) return 2u;
    else if( format == vk::Format::eR12X4G12X4B12X4A12X4Unorm4Pack16KHR) return 8u;
    else if( format == vk::Format::eR12X4G12X4Unorm2Pack16KHR) return 4u;
    else if( format == vk::Format::eR12X4UnormPack16KHR) return 2u;
#endif
    else return 0u;
  }
}

