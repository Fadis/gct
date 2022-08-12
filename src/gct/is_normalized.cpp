#include <gct/format.hpp>

namespace gct {
  bool is_normalized( vk::Format format ) {
    if( format == vk::Format::eR4G4UnormPack8) return true;
    else if( format == vk::Format::eR4G4B4A4UnormPack16) return true;
    else if( format == vk::Format::eB4G4R4A4UnormPack16) return true;
    else if( format == vk::Format::eR5G6B5UnormPack16) return true;
    else if( format == vk::Format::eB5G6R5UnormPack16) return true;
    else if( format == vk::Format::eR5G5B5A1UnormPack16) return true;
    else if( format == vk::Format::eB5G5R5A1UnormPack16) return true;
    else if( format == vk::Format::eA1R5G5B5UnormPack16) return true;
    else if( format == vk::Format::eR8Unorm) return true;
    else if( format == vk::Format::eR8Snorm) return true;
    else if( format == vk::Format::eR8G8Unorm) return true;
    else if( format == vk::Format::eR8G8Snorm) return true;
    else if( format == vk::Format::eR8G8B8Unorm) return true;
    else if( format == vk::Format::eR8G8B8Snorm) return true;
    else if( format == vk::Format::eB8G8R8Unorm) return true;
    else if( format == vk::Format::eB8G8R8Snorm) return true;
    else if( format == vk::Format::eR8G8B8A8Unorm) return true;
    else if( format == vk::Format::eR8G8B8A8Snorm) return true;
    else if( format == vk::Format::eB8G8R8A8Unorm) return true;
    else if( format == vk::Format::eB8G8R8A8Snorm) return true;
    else if( format == vk::Format::eA8B8G8R8UnormPack32) return true;
    else if( format == vk::Format::eA8B8G8R8SnormPack32) return true;
    else if( format == vk::Format::eA2R10G10B10UnormPack32) return true;
    else if( format == vk::Format::eA2R10G10B10SnormPack32) return true;
    else if( format == vk::Format::eA2B10G10R10UnormPack32) return true;
    else if( format == vk::Format::eA2B10G10R10SnormPack32) return true;
    else if( format == vk::Format::eR16Unorm) return true;
    else if( format == vk::Format::eR16Snorm) return true;
    else if( format == vk::Format::eR16G16Unorm) return true;
    else if( format == vk::Format::eR16G16Snorm) return true;
    else if( format == vk::Format::eR16G16B16Unorm) return true;
    else if( format == vk::Format::eR16G16B16Snorm) return true;
    else if( format == vk::Format::eR16G16B16A16Unorm) return true;
    else if( format == vk::Format::eR16G16B16A16Snorm) return true;
    else if( format == vk::Format::eD16Unorm) return true;
    else if( format == vk::Format::eX8D24UnormPack32) return true;
    else if( format == vk::Format::eD16UnormS8Uint) return true;
    else if( format == vk::Format::eD24UnormS8Uint) return true;
    else if( format == vk::Format::eBc1RgbUnormBlock) return true;
    else if( format == vk::Format::eBc1RgbaUnormBlock) return true;
    else if( format == vk::Format::eBc2UnormBlock) return true;
    else if( format == vk::Format::eBc3UnormBlock) return true;
    else if( format == vk::Format::eBc4UnormBlock) return true;
    else if( format == vk::Format::eBc4SnormBlock) return true;
    else if( format == vk::Format::eBc5UnormBlock) return true;
    else if( format == vk::Format::eBc5SnormBlock) return true;
    else if( format == vk::Format::eBc7UnormBlock) return true;
    else if( format == vk::Format::eEtc2R8G8B8UnormBlock) return true;
    else if( format == vk::Format::eEtc2R8G8B8A1UnormBlock) return true;
    else if( format == vk::Format::eEtc2R8G8B8A8UnormBlock) return true;
    else if( format == vk::Format::eEacR11UnormBlock) return true;
    else if( format == vk::Format::eEacR11SnormBlock) return true;
    else if( format == vk::Format::eEacR11G11UnormBlock) return true;
    else if( format == vk::Format::eEacR11G11SnormBlock) return true;
    else if( format == vk::Format::eAstc4x4UnormBlock) return true;
    else if( format == vk::Format::eAstc5x4UnormBlock) return true;
    else if( format == vk::Format::eAstc5x5UnormBlock) return true;
    else if( format == vk::Format::eAstc6x5UnormBlock) return true;
    else if( format == vk::Format::eAstc6x6UnormBlock) return true;
    else if( format == vk::Format::eAstc8x5UnormBlock) return true;
    else if( format == vk::Format::eAstc8x6UnormBlock) return true;
    else if( format == vk::Format::eAstc8x8UnormBlock) return true;
    else if( format == vk::Format::eAstc10x5UnormBlock) return true;
    else if( format == vk::Format::eAstc10x6UnormBlock) return true;
    else if( format == vk::Format::eAstc10x8UnormBlock) return true;
    else if( format == vk::Format::eAstc10x10UnormBlock) return true;
    else if( format == vk::Format::eAstc12x10UnormBlock) return true;
    else if( format == vk::Format::eAstc12x12UnormBlock) return true;
#ifdef VK_VERSION_1_1
    else if( format == vk::Format::eG8B8G8R8422Unorm) return true;
    else if( format == vk::Format::eB8G8R8G8422Unorm) return true;
    else if( format == vk::Format::eG8B8R83Plane420Unorm) return true;
    else if( format == vk::Format::eG8B8R82Plane420Unorm) return true;
    else if( format == vk::Format::eG8B8R83Plane422Unorm) return true;
    else if( format == vk::Format::eG8B8R82Plane422Unorm) return true;
    else if( format == vk::Format::eG8B8R83Plane444Unorm) return true;
    else if( format == vk::Format::eR10X6UnormPack16) return true;
    else if( format == vk::Format::eR10X6G10X6Unorm2Pack16) return true;
    else if( format == vk::Format::eR10X6G10X6B10X6A10X6Unorm4Pack16) return true;
    else if( format == vk::Format::eG10X6B10X6G10X6R10X6422Unorm4Pack16) return true;
    else if( format == vk::Format::eB10X6G10X6R10X6G10X6422Unorm4Pack16) return true;
    else if( format == vk::Format::eG10X6B10X6R10X63Plane420Unorm3Pack16) return true;
    else if( format == vk::Format::eG10X6B10X6R10X62Plane420Unorm3Pack16) return true;
    else if( format == vk::Format::eG10X6B10X6R10X63Plane422Unorm3Pack16) return true;
    else if( format == vk::Format::eG10X6B10X6R10X62Plane422Unorm3Pack16) return true;
    else if( format == vk::Format::eG10X6B10X6R10X63Plane444Unorm3Pack16) return true;
    else if( format == vk::Format::eR12X4UnormPack16) return true;
    else if( format == vk::Format::eR12X4G12X4Unorm2Pack16) return true;
    else if( format == vk::Format::eR12X4G12X4B12X4A12X4Unorm4Pack16) return true;
    else if( format == vk::Format::eG12X4B12X4G12X4R12X4422Unorm4Pack16) return true;
    else if( format == vk::Format::eB12X4G12X4R12X4G12X4422Unorm4Pack16) return true;
    else if( format == vk::Format::eG12X4B12X4R12X43Plane420Unorm3Pack16) return true;
    else if( format == vk::Format::eG12X4B12X4R12X42Plane420Unorm3Pack16) return true;
    else if( format == vk::Format::eG12X4B12X4R12X43Plane422Unorm3Pack16) return true;
    else if( format == vk::Format::eG12X4B12X4R12X42Plane422Unorm3Pack16) return true;
    else if( format == vk::Format::eG12X4B12X4R12X43Plane444Unorm3Pack16) return true;
    else if( format == vk::Format::eG16B16G16R16422Unorm) return true;
    else if( format == vk::Format::eB16G16R16G16422Unorm) return true;
    else if( format == vk::Format::eG16B16R163Plane420Unorm) return true;
    else if( format == vk::Format::eG16B16R162Plane420Unorm) return true;
    else if( format == vk::Format::eG16B16R163Plane422Unorm) return true;
    else if( format == vk::Format::eG16B16R162Plane422Unorm) return true;
    else if( format == vk::Format::eG16B16R163Plane444Unorm) return true;
#endif
#ifdef VK_IMG_FORMAT_PVRTC_EXTENSION_NAME
    else if( format == vk::Format::ePvrtc12BppUnormBlockIMG) return true;
    else if( format == vk::Format::ePvrtc14BppUnormBlockIMG) return true;
    else if( format == vk::Format::ePvrtc22BppUnormBlockIMG) return true;
    else if( format == vk::Format::ePvrtc24BppUnormBlockIMG) return true;
#endif
#ifdef VK_EXT_YCBCR_2PLANE_444_FORMATS_EXTENSION_NAME
    else if( format == vk::Format::eG8B8R82Plane444UnormEXT) return true;
    else if( format == vk::Format::eG10X6B10X6R10X62Plane444Unorm3Pack16EXT) return true;
    else if( format == vk::Format::eG12X4B12X4R12X42Plane444Unorm3Pack16EXT) return true;
    else if( format == vk::Format::eG16B16R162Plane444UnormEXT) return true;
#endif
#ifdef VK_EXT_4444_FORMATS_EXTENSION_NAME
    else if( format == vk::Format::eA4R4G4B4UnormPack16EXT) return true;
    else if( format == vk::Format::eA4B4G4R4UnormPack16EXT) return true;
#endif
#ifdef VK_KHR_SAMPLER_YCBCR_CONVERSION_EXTENSION_NAME
    else if( format == vk::Format::eB10X6G10X6R10X6G10X6422Unorm4Pack16KHR) return true;
    else if( format == vk::Format::eB12X4G12X4R12X4G12X4422Unorm4Pack16KHR) return true;
    else if( format == vk::Format::eB16G16R16G16422UnormKHR) return true;
    else if( format == vk::Format::eB8G8R8G8422UnormKHR) return true;
    else if( format == vk::Format::eG10X6B10X6G10X6R10X6422Unorm4Pack16KHR) return true;
    else if( format == vk::Format::eG10X6B10X6R10X62Plane420Unorm3Pack16KHR) return true;
    else if( format == vk::Format::eG10X6B10X6R10X62Plane422Unorm3Pack16KHR) return true;
    else if( format == vk::Format::eG10X6B10X6R10X63Plane420Unorm3Pack16KHR) return true;
    else if( format == vk::Format::eG10X6B10X6R10X63Plane422Unorm3Pack16KHR) return true;
    else if( format == vk::Format::eG10X6B10X6R10X63Plane444Unorm3Pack16KHR) return true;
    else if( format == vk::Format::eG12X4B12X4G12X4R12X4422Unorm4Pack16KHR) return true;
    else if( format == vk::Format::eG12X4B12X4R12X42Plane420Unorm3Pack16KHR) return true;
    else if( format == vk::Format::eG12X4B12X4R12X42Plane422Unorm3Pack16KHR) return true;
    else if( format == vk::Format::eG12X4B12X4R12X43Plane420Unorm3Pack16KHR) return true;
    else if( format == vk::Format::eG12X4B12X4R12X43Plane422Unorm3Pack16KHR) return true;
    else if( format == vk::Format::eG12X4B12X4R12X43Plane444Unorm3Pack16KHR) return true;
    else if( format == vk::Format::eG16B16G16R16422UnormKHR) return true;
    else if( format == vk::Format::eG16B16R162Plane420UnormKHR) return true;
    else if( format == vk::Format::eG16B16R162Plane422UnormKHR) return true;
    else if( format == vk::Format::eG16B16R163Plane420UnormKHR) return true;
    else if( format == vk::Format::eG16B16R163Plane422UnormKHR) return true;
    else if( format == vk::Format::eG16B16R163Plane444UnormKHR) return true;
    else if( format == vk::Format::eG8B8G8R8422UnormKHR) return true;
    else if( format == vk::Format::eG8B8R82Plane420UnormKHR) return true;
    else if( format == vk::Format::eG8B8R82Plane422UnormKHR) return true;
    else if( format == vk::Format::eG8B8R83Plane420UnormKHR) return true;
    else if( format == vk::Format::eG8B8R83Plane422UnormKHR) return true;
    else if( format == vk::Format::eG8B8R83Plane444UnormKHR) return true;
    else if( format == vk::Format::eR10X6G10X6B10X6A10X6Unorm4Pack16KHR) return true;
    else if( format == vk::Format::eR10X6G10X6Unorm2Pack16KHR) return true;
    else if( format == vk::Format::eR10X6UnormPack16KHR) return true;
    else if( format == vk::Format::eR12X4G12X4B12X4A12X4Unorm4Pack16KHR) return true;
    else if( format == vk::Format::eR12X4G12X4Unorm2Pack16KHR) return true;
    else if( format == vk::Format::eR12X4UnormPack16KHR) return true;
#endif
    else return false;
  }
}

