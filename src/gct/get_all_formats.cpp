#include <vector>
#include "vulkan/vulkan.hpp"
#include <gct/get_extensions.hpp>
namespace gct {
  std::vector< vk::Format > get_all_formats(
    std::uint32_t api_version,
    const extension_map_t &available_extensions
  ) {
    std::vector< vk::Format > formats{
      vk::Format::eUndefined,
      vk::Format::eR4G4UnormPack8,
      vk::Format::eR4G4B4A4UnormPack16,
      vk::Format::eB4G4R4A4UnormPack16,
      vk::Format::eR5G6B5UnormPack16,
      vk::Format::eB5G6R5UnormPack16,
      vk::Format::eR5G5B5A1UnormPack16,
      vk::Format::eB5G5R5A1UnormPack16,
      vk::Format::eA1R5G5B5UnormPack16,
      vk::Format::eR8Unorm,
      vk::Format::eR8Snorm,
      vk::Format::eR8Uscaled,
      vk::Format::eR8Sscaled,
      vk::Format::eR8Uint,
      vk::Format::eR8Sint,
      vk::Format::eR8Srgb,
      vk::Format::eR8G8Unorm,
      vk::Format::eR8G8Snorm,
      vk::Format::eR8G8Uscaled,
      vk::Format::eR8G8Sscaled,
      vk::Format::eR8G8Uint,
      vk::Format::eR8G8Sint,
      vk::Format::eR8G8Srgb,
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
      vk::Format::eB8G8R8Srgb,
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
      vk::Format::eA8B8G8R8SrgbPack32,
      vk::Format::eA2R10G10B10UnormPack32,
      vk::Format::eA2R10G10B10SnormPack32,
      vk::Format::eA2R10G10B10UscaledPack32,
      vk::Format::eA2R10G10B10SscaledPack32,
      vk::Format::eA2R10G10B10UintPack32,
      vk::Format::eA2R10G10B10SintPack32,
      vk::Format::eA2B10G10R10UnormPack32,
      vk::Format::eA2B10G10R10SnormPack32,
      vk::Format::eA2B10G10R10UscaledPack32,
      vk::Format::eA2B10G10R10SscaledPack32,
      vk::Format::eA2B10G10R10UintPack32,
      vk::Format::eA2B10G10R10SintPack32,
      vk::Format::eR16Unorm,
      vk::Format::eR16Snorm,
      vk::Format::eR16Uscaled,
      vk::Format::eR16Sscaled,
      vk::Format::eR16Uint,
      vk::Format::eR16Sint,
      vk::Format::eR16Sfloat,
      vk::Format::eR16G16Unorm,
      vk::Format::eR16G16Snorm,
      vk::Format::eR16G16Uscaled,
      vk::Format::eR16G16Sscaled,
      vk::Format::eR16G16Uint,
      vk::Format::eR16G16Sint,
      vk::Format::eR16G16Sfloat,
      vk::Format::eR16G16B16Unorm,
      vk::Format::eR16G16B16Snorm,
      vk::Format::eR16G16B16Uscaled,
      vk::Format::eR16G16B16Sscaled,
      vk::Format::eR16G16B16Uint,
      vk::Format::eR16G16B16Sint,
      vk::Format::eR16G16B16Sfloat,
      vk::Format::eR16G16B16A16Unorm,
      vk::Format::eR16G16B16A16Snorm,
      vk::Format::eR16G16B16A16Uscaled,
      vk::Format::eR16G16B16A16Sscaled,
      vk::Format::eR16G16B16A16Uint,
      vk::Format::eR16G16B16A16Sint,
      vk::Format::eR16G16B16A16Sfloat,
      vk::Format::eR32Uint,
      vk::Format::eR32Sint,
      vk::Format::eR32Sfloat,
      vk::Format::eR32G32Uint,
      vk::Format::eR32G32Sint,
      vk::Format::eR32G32Sfloat,
      vk::Format::eR32G32B32Uint,
      vk::Format::eR32G32B32Sint,
      vk::Format::eR32G32B32Sfloat,
      vk::Format::eR32G32B32A32Uint,
      vk::Format::eR32G32B32A32Sint,
      vk::Format::eR32G32B32A32Sfloat,
      vk::Format::eR64Uint,
      vk::Format::eR64Sint,
      vk::Format::eR64Sfloat,
      vk::Format::eR64G64Uint,
      vk::Format::eR64G64Sint,
      vk::Format::eR64G64Sfloat,
      vk::Format::eR64G64B64Uint,
      vk::Format::eR64G64B64Sint,
      vk::Format::eR64G64B64Sfloat,
      vk::Format::eR64G64B64A64Uint,
      vk::Format::eR64G64B64A64Sint,
      vk::Format::eR64G64B64A64Sfloat,
      vk::Format::eB10G11R11UfloatPack32,
      vk::Format::eE5B9G9R9UfloatPack32,
      vk::Format::eD16Unorm,
      vk::Format::eX8D24UnormPack32,
      vk::Format::eD32Sfloat,
      vk::Format::eS8Uint,
      vk::Format::eD16UnormS8Uint,
      vk::Format::eD24UnormS8Uint,
      vk::Format::eD32SfloatS8Uint,
      vk::Format::eBc1RgbUnormBlock,
      vk::Format::eBc1RgbSrgbBlock,
      vk::Format::eBc1RgbaUnormBlock,
      vk::Format::eBc1RgbaSrgbBlock,
      vk::Format::eBc2UnormBlock,
      vk::Format::eBc2SrgbBlock,
      vk::Format::eBc3UnormBlock,
      vk::Format::eBc3SrgbBlock,
      vk::Format::eBc4UnormBlock,
      vk::Format::eBc4SnormBlock,
      vk::Format::eBc5UnormBlock,
      vk::Format::eBc5SnormBlock,
      vk::Format::eBc6HUfloatBlock,
      vk::Format::eBc6HSfloatBlock,
      vk::Format::eBc7UnormBlock,
      vk::Format::eBc7SrgbBlock,
      vk::Format::eEtc2R8G8B8UnormBlock,
      vk::Format::eEtc2R8G8B8SrgbBlock,
      vk::Format::eEtc2R8G8B8A1UnormBlock,
      vk::Format::eEtc2R8G8B8A1SrgbBlock,
      vk::Format::eEtc2R8G8B8A8UnormBlock,
      vk::Format::eEtc2R8G8B8A8SrgbBlock,
      vk::Format::eEacR11UnormBlock,
      vk::Format::eEacR11SnormBlock,
      vk::Format::eEacR11G11UnormBlock,
      vk::Format::eEacR11G11SnormBlock,
      vk::Format::eAstc4x4UnormBlock,
      vk::Format::eAstc4x4SrgbBlock,
      vk::Format::eAstc5x4UnormBlock,
      vk::Format::eAstc5x4SrgbBlock,
      vk::Format::eAstc5x5UnormBlock,
      vk::Format::eAstc5x5SrgbBlock,
      vk::Format::eAstc6x5UnormBlock,
      vk::Format::eAstc6x5SrgbBlock,
      vk::Format::eAstc6x6UnormBlock,
      vk::Format::eAstc6x6SrgbBlock,
      vk::Format::eAstc8x5UnormBlock,
      vk::Format::eAstc8x5SrgbBlock,
      vk::Format::eAstc8x6UnormBlock,
      vk::Format::eAstc8x6SrgbBlock,
      vk::Format::eAstc8x8UnormBlock,
      vk::Format::eAstc8x8SrgbBlock,
      vk::Format::eAstc10x5UnormBlock,
      vk::Format::eAstc10x5SrgbBlock,
      vk::Format::eAstc10x6UnormBlock,
      vk::Format::eAstc10x6SrgbBlock,
      vk::Format::eAstc10x8UnormBlock,
      vk::Format::eAstc10x8SrgbBlock,
      vk::Format::eAstc10x10UnormBlock,
      vk::Format::eAstc10x10SrgbBlock,
      vk::Format::eAstc12x10UnormBlock,
      vk::Format::eAstc12x10SrgbBlock,
      vk::Format::eAstc12x12UnormBlock,
      vk::Format::eAstc12x12SrgbBlock
    };
#ifdef VK_VERSION_1_1
    if( api_version >= VK_MAKE_VERSION( 1, 1, 0 ) ) {
      const static std::vector< vk::Format > additional{
        vk::Format::eG8B8G8R8422Unorm,
        vk::Format::eB8G8R8G8422Unorm,
        vk::Format::eG8B8R83Plane420Unorm,
        vk::Format::eG8B8R82Plane420Unorm,
        vk::Format::eG8B8R83Plane422Unorm,
        vk::Format::eG8B8R82Plane422Unorm,
        vk::Format::eG8B8R83Plane444Unorm,
        vk::Format::eR10X6UnormPack16,
        vk::Format::eR10X6G10X6Unorm2Pack16,
        vk::Format::eR10X6G10X6B10X6A10X6Unorm4Pack16,
        vk::Format::eG10X6B10X6G10X6R10X6422Unorm4Pack16,
        vk::Format::eB10X6G10X6R10X6G10X6422Unorm4Pack16,
        vk::Format::eG10X6B10X6R10X63Plane420Unorm3Pack16,
        vk::Format::eG10X6B10X6R10X62Plane420Unorm3Pack16,
        vk::Format::eG10X6B10X6R10X63Plane422Unorm3Pack16,
        vk::Format::eG10X6B10X6R10X62Plane422Unorm3Pack16,
        vk::Format::eG10X6B10X6R10X63Plane444Unorm3Pack16,
        vk::Format::eR12X4UnormPack16,
        vk::Format::eR12X4G12X4Unorm2Pack16,
        vk::Format::eR12X4G12X4B12X4A12X4Unorm4Pack16,
        vk::Format::eG12X4B12X4G12X4R12X4422Unorm4Pack16,
        vk::Format::eB12X4G12X4R12X4G12X4422Unorm4Pack16,
        vk::Format::eG12X4B12X4R12X43Plane420Unorm3Pack16,
        vk::Format::eG12X4B12X4R12X42Plane420Unorm3Pack16,
        vk::Format::eG12X4B12X4R12X43Plane422Unorm3Pack16,
        vk::Format::eG12X4B12X4R12X42Plane422Unorm3Pack16,
        vk::Format::eG12X4B12X4R12X43Plane444Unorm3Pack16,
        vk::Format::eG16B16G16R16422Unorm,
        vk::Format::eB16G16R16G16422Unorm,
        vk::Format::eG16B16R163Plane420Unorm,
        vk::Format::eG16B16R162Plane420Unorm,
        vk::Format::eG16B16R163Plane422Unorm,
        vk::Format::eG16B16R162Plane422Unorm,
        vk::Format::eG16B16R163Plane444Unorm
      };
      formats.insert( formats.end(), additional.begin(), additional.end() );
    }
#endif
#ifdef VK_VERSION_1_3
    if( api_version >= VK_MAKE_VERSION( 1, 3, 0 ) ) {
      const static std::vector< vk::Format > additional{
        vk::Format::eG8B8R82Plane444Unorm,
        vk::Format::eG10X6B10X6R10X62Plane444Unorm3Pack16,
        vk::Format::eG12X4B12X4R12X42Plane444Unorm3Pack16,
        vk::Format::eG16B16R162Plane444Unorm,
        vk::Format::eA4R4G4B4UnormPack16,
        vk::Format::eA4B4G4R4UnormPack16,
        vk::Format::eAstc4x4SfloatBlock,
        vk::Format::eAstc5x4SfloatBlock,
        vk::Format::eAstc5x5SfloatBlock,
        vk::Format::eAstc6x5SfloatBlock,
        vk::Format::eAstc6x6SfloatBlock,
        vk::Format::eAstc8x5SfloatBlock,
        vk::Format::eAstc8x6SfloatBlock,
        vk::Format::eAstc8x8SfloatBlock,
        vk::Format::eAstc10x5SfloatBlock,
        vk::Format::eAstc10x6SfloatBlock,
        vk::Format::eAstc10x8SfloatBlock,
        vk::Format::eAstc10x10SfloatBlock,
        vk::Format::eAstc12x10SfloatBlock,
        vk::Format::eAstc12x12SfloatBlock
      };
      formats.insert( formats.end(), additional.begin(), additional.end() );
    }
#endif
#ifdef VK_IMG_FORMAT_PVRTC_EXTENSION_NAME
    if( available_extensions.find( VK_IMG_FORMAT_PVRTC_EXTENSION_NAME ) != available_extensions.end() ) {
      const static std::vector< vk::Format > additional{
        vk::Format::ePvrtc12BppUnormBlockIMG,
        vk::Format::ePvrtc14BppUnormBlockIMG,
        vk::Format::ePvrtc22BppUnormBlockIMG,
        vk::Format::ePvrtc24BppUnormBlockIMG,
        vk::Format::ePvrtc12BppSrgbBlockIMG,
        vk::Format::ePvrtc14BppSrgbBlockIMG,
        vk::Format::ePvrtc22BppSrgbBlockIMG,
        vk::Format::ePvrtc24BppSrgbBlockIMG
      };
      formats.insert( formats.end(), additional.begin(), additional.end() );
    }
#endif
/*
#ifdef VK_NV_OPTICAL_FLOW_EXTENSION_NAME
    if( available_extensions.find( VK_NV_OPTICAL_FLOW_EXTENSION_NAME ) != available_extensions.end() ) {
      const static std::vector< vk::Format > additional{
        vk::Format::eR16G16S105NV
      };
      formats.insert( formats.end(), additional.begin(), additional.end() );
    }
#endif
*/
#ifdef VK_EXT_TEXTURE_COMPRESSION_ASTC_HDR_EXTENSION_NAME
    if( available_extensions.find( VK_EXT_TEXTURE_COMPRESSION_ASTC_HDR_EXTENSION_NAME ) != available_extensions.end() ) {
      const static std::vector< vk::Format > additional{
        vk::Format::eAstc10x10SfloatBlockEXT,
        vk::Format::eAstc10x5SfloatBlockEXT,
        vk::Format::eAstc10x6SfloatBlockEXT,
        vk::Format::eAstc10x8SfloatBlockEXT,
        vk::Format::eAstc12x10SfloatBlockEXT,
        vk::Format::eAstc12x12SfloatBlockEXT,
        vk::Format::eAstc4x4SfloatBlockEXT,
        vk::Format::eAstc5x4SfloatBlockEXT,
        vk::Format::eAstc5x5SfloatBlockEXT,
        vk::Format::eAstc6x5SfloatBlockEXT,
        vk::Format::eAstc6x6SfloatBlockEXT,
        vk::Format::eAstc8x5SfloatBlockEXT,
        vk::Format::eAstc8x6SfloatBlockEXT,
        vk::Format::eAstc8x8SfloatBlockEXT,
      };
      formats.insert( formats.end(), additional.begin(), additional.end() );
    }
#endif
#ifdef VK_KHR_SAMPLER_YCBCR_CONVERSION_EXTENSION_NAME
    if( available_extensions.find( VK_KHR_SAMPLER_YCBCR_CONVERSION_EXTENSION_NAME ) != available_extensions.end() ) {
      const static std::vector< vk::Format > additional{
        vk::Format::eB10X6G10X6R10X6G10X6422Unorm4Pack16KHR,
        vk::Format::eB12X4G12X4R12X4G12X4422Unorm4Pack16KHR,
        vk::Format::eB16G16R16G16422UnormKHR,
        vk::Format::eB8G8R8G8422UnormKHR,
        vk::Format::eG10X6B10X6G10X6R10X6422Unorm4Pack16KHR,
        vk::Format::eG10X6B10X6R10X62Plane420Unorm3Pack16KHR,
        vk::Format::eG10X6B10X6R10X62Plane422Unorm3Pack16KHR,
        vk::Format::eG10X6B10X6R10X63Plane420Unorm3Pack16KHR,
        vk::Format::eG10X6B10X6R10X63Plane422Unorm3Pack16KHR,
        vk::Format::eG10X6B10X6R10X63Plane444Unorm3Pack16KHR,
        vk::Format::eG12X4B12X4G12X4R12X4422Unorm4Pack16KHR,
        vk::Format::eG12X4B12X4R12X42Plane420Unorm3Pack16KHR,
        vk::Format::eG12X4B12X4R12X42Plane422Unorm3Pack16KHR,
        vk::Format::eG12X4B12X4R12X43Plane420Unorm3Pack16KHR,
        vk::Format::eG12X4B12X4R12X43Plane422Unorm3Pack16KHR,
        vk::Format::eG12X4B12X4R12X43Plane444Unorm3Pack16KHR,
        vk::Format::eG16B16G16R16422UnormKHR,
        vk::Format::eG16B16R162Plane420UnormKHR,
        vk::Format::eG16B16R162Plane422UnormKHR,
        vk::Format::eG16B16R163Plane420UnormKHR,
        vk::Format::eG16B16R163Plane422UnormKHR,
        vk::Format::eG16B16R163Plane444UnormKHR,
        vk::Format::eG8B8G8R8422UnormKHR,
        vk::Format::eG8B8R82Plane420UnormKHR,
        vk::Format::eG8B8R82Plane422UnormKHR,
        vk::Format::eG8B8R83Plane420UnormKHR,
        vk::Format::eG8B8R83Plane422UnormKHR,
        vk::Format::eG8B8R83Plane444UnormKHR,
        vk::Format::eR10X6G10X6B10X6A10X6Unorm4Pack16KHR,
        vk::Format::eR10X6G10X6Unorm2Pack16KHR,
        vk::Format::eR10X6UnormPack16KHR,
        vk::Format::eR12X4G12X4B12X4A12X4Unorm4Pack16KHR,
        vk::Format::eR12X4G12X4Unorm2Pack16KHR,
        vk::Format::eR12X4UnormPack16KHR
      };
      formats.insert( formats.end(), additional.begin(), additional.end() );
    }
#endif
#ifdef VK_EXT_YCBCR_2PLANE_444_FORMATS_EXTENSION_NAME
    if( available_extensions.find( VK_EXT_YCBCR_2PLANE_444_FORMATS_EXTENSION_NAME ) != available_extensions.end() ) {
      const static std::vector< vk::Format > additional{
        vk::Format::eG10X6B10X6R10X62Plane444Unorm3Pack16EXT,
        vk::Format::eG12X4B12X4R12X42Plane444Unorm3Pack16EXT,
        vk::Format::eG8B8R82Plane444UnormEXT,
        vk::Format::eG16B16R162Plane444UnormEXT
      };
      formats.insert( formats.end(), additional.begin(), additional.end() );
    }
#endif
#ifdef VK_EXT_4444_FORMATS_EXTENSION_NAME
    if( available_extensions.find( VK_EXT_4444_FORMATS_EXTENSION_NAME ) != available_extensions.end() ) {
      const static std::vector< vk::Format > additional{
        vk::Format::eA4B4G4R4UnormPack16EXT,
        vk::Format::eA4R4G4B4UnormPack16EXT
      };
      formats.insert( formats.end(), additional.begin(), additional.end() );
    }
#endif
    return formats;
  }
}

