#include <vulkan//vulkan.hpp>
#include <gli/format.hpp>

namespace gct {

vk::Format gliformat_to_vkformat( gli::format f ) {
  if( f == gli::format::FORMAT_UNDEFINED ) {
    return vk::Format::eUndefined;
  }
  else if( f == gli::format::FORMAT_RG4_UNORM_PACK8 ) {
    return vk::Format::eR4G4UnormPack8;
  }
  else if( f == gli::format::FORMAT_RGBA4_UNORM_PACK16 ) {
    return vk::Format::eR4G4B4A4UnormPack16;
  }
  else if( f == gli::format::FORMAT_BGRA4_UNORM_PACK16 ) {
    return vk::Format::eB4G4R4A4UnormPack16;
  }
  else if( f == gli::format::FORMAT_R5G6B5_UNORM_PACK16 ) {
    return vk::Format::eR5G6B5UnormPack16;
  }
  else if( f == gli::format::FORMAT_B5G6R5_UNORM_PACK16 ) {
    return vk::Format::eB5G6R5UnormPack16;
  }
  else if( f == gli::format::FORMAT_RGB5A1_UNORM_PACK16 ) {
    return vk::Format::eR5G5B5A1UnormPack16;
  }
  else if( f == gli::format::FORMAT_BGR5A1_UNORM_PACK16 ) {
    return vk::Format::eB5G5R5A1UnormPack16;
  }
  else if( f == gli::format::FORMAT_A1RGB5_UNORM_PACK16 ) {
    return vk::Format::eA1R5G5B5UnormPack16;
  }
  else if( f == gli::format::FORMAT_BGR5A1_UNORM_PACK16 ) {
    return vk::Format::eB5G5R5A1UnormPack16;
  }
  else if( f == gli::format::FORMAT_R8_UNORM_PACK8 ) {
    return vk::Format::eR8Unorm;
  }
  else if( f == gli::format::FORMAT_R8_SNORM_PACK8 ) {
    return vk::Format::eR8Snorm;
  }
  else if( f == gli::format::FORMAT_R8_USCALED_PACK8 ) {
    return vk::Format::eR8Uscaled;
  }
  else if( f == gli::format::FORMAT_R8_SSCALED_PACK8 ) {
    return vk::Format::eR8Sscaled;
  }
  else if( f == gli::format::FORMAT_R8_UINT_PACK8 ) {
    return vk::Format::eR8Uint;
  }
  else if( f == gli::format::FORMAT_R8_SINT_PACK8 ) {
    return vk::Format::eR8Sint;
  }
  else if( f == gli::format::FORMAT_R8_SRGB_PACK8 ) {
    return vk::Format::eR8Srgb;
  }
  else if( f == gli::format::FORMAT_RG8_UNORM_PACK8 ) {
    return vk::Format::eR8G8Unorm;
  }
  else if( f == gli::format::FORMAT_RG8_SNORM_PACK8 ) {
    return vk::Format::eR8G8Snorm;
  }
  else if( f == gli::format::FORMAT_RG8_USCALED_PACK8 ) {
    return vk::Format::eR8G8Uscaled;
  }
  else if( f == gli::format::FORMAT_RG8_SSCALED_PACK8 ) {
    return vk::Format::eR8G8Sscaled;
  }
  else if( f == gli::format::FORMAT_RG8_UINT_PACK8 ) {
    return vk::Format::eR8G8Uint;
  }
  else if( f == gli::format::FORMAT_RG8_SINT_PACK8 ) {
    return vk::Format::eR8G8Sint;
  }
  else if( f == gli::format::FORMAT_RG8_SRGB_PACK8 ) {
    return vk::Format::eR8G8Srgb;
  }
  else if( f == gli::format::FORMAT_RGB8_UNORM_PACK8 ) {
    return vk::Format::eR8G8B8Unorm;
  }
  else if( f == gli::format::FORMAT_RGB8_SNORM_PACK8 ) {
    return vk::Format::eR8G8B8Snorm;
  }
  else if( f == gli::format::FORMAT_RGB8_USCALED_PACK8 ) {
    return vk::Format::eR8G8B8Uscaled;
  }
  else if( f == gli::format::FORMAT_RGB8_SSCALED_PACK8 ) {
    return vk::Format::eR8G8B8Sscaled;
  }
  else if( f == gli::format::FORMAT_RGB8_UINT_PACK8 ) {
    return vk::Format::eR8G8B8Uint;
  }
  else if( f == gli::format::FORMAT_RGB8_SINT_PACK8 ) {
    return vk::Format::eR8G8B8Sint;
  }
  else if( f == gli::format::FORMAT_RGB8_SRGB_PACK8 ) {
    return vk::Format::eR8G8B8Srgb;
  }
  else if( f == gli::format::FORMAT_BGR8_UNORM_PACK8 ) {
    return vk::Format::eB8G8R8Unorm;
  }
  else if( f == gli::format::FORMAT_BGR8_SNORM_PACK8 ) {
    return vk::Format::eB8G8R8Snorm;
  }
  else if( f == gli::format::FORMAT_BGR8_USCALED_PACK8 ) {
    return vk::Format::eB8G8R8Uscaled;
  }
  else if( f == gli::format::FORMAT_BGR8_SSCALED_PACK8 ) {
    return vk::Format::eB8G8R8Sscaled;
  }
  else if( f == gli::format::FORMAT_BGR8_UINT_PACK8 ) {
    return vk::Format::eB8G8R8Uint;
  }
  else if( f == gli::format::FORMAT_BGR8_SINT_PACK8 ) {
    return vk::Format::eB8G8R8Sint;
  }
  else if( f == gli::format::FORMAT_BGR8_SRGB_PACK8 ) {
    return vk::Format::eB8G8R8Srgb;
  }
  else if( f == gli::format::FORMAT_RGBA8_UNORM_PACK8 ) {
    return vk::Format::eR8G8B8A8Unorm;
  }
  else if( f == gli::format::FORMAT_RGBA8_SNORM_PACK8 ) {
    return vk::Format::eR8G8B8A8Snorm;
  }
  else if( f == gli::format::FORMAT_RGBA8_USCALED_PACK8 ) {
    return vk::Format::eR8G8B8A8Uscaled;
  }
  else if( f == gli::format::FORMAT_RGBA8_SSCALED_PACK8 ) {
    return vk::Format::eR8G8B8A8Sscaled;
  }
  else if( f == gli::format::FORMAT_RGBA8_UINT_PACK8 ) {
    return vk::Format::eR8G8B8A8Uint;
  }
  else if( f == gli::format::FORMAT_RGBA8_SINT_PACK8 ) {
    return vk::Format::eR8G8B8A8Sint;
  }
  else if( f == gli::format::FORMAT_RGBA8_SRGB_PACK8 ) {
    return vk::Format::eR8G8B8A8Srgb;
  }
  else if( f == gli::format::FORMAT_BGRA8_UNORM_PACK8 ) {
    return vk::Format::eB8G8R8A8Unorm;
  }
  else if( f == gli::format::FORMAT_BGRA8_SNORM_PACK8 ) {
    return vk::Format::eB8G8R8A8Snorm;
  }
  else if( f == gli::format::FORMAT_BGRA8_USCALED_PACK8 ) {
    return vk::Format::eB8G8R8A8Uscaled;
  }
  else if( f == gli::format::FORMAT_BGRA8_SSCALED_PACK8 ) {
    return vk::Format::eB8G8R8A8Sscaled;
  }
  else if( f == gli::format::FORMAT_BGRA8_UINT_PACK8 ) {
    return vk::Format::eB8G8R8A8Uint;
  }
  else if( f == gli::format::FORMAT_BGRA8_SINT_PACK8 ) {
    return vk::Format::eB8G8R8A8Sint;
  }
  else if( f == gli::format::FORMAT_BGRA8_SRGB_PACK8 ) {
    return vk::Format::eB8G8R8A8Srgb;
  }
  else if( f == gli::format::FORMAT_RGBA8_UNORM_PACK32 ) {
    return vk::Format::eA8B8G8R8UnormPack32;
  }
  else if( f == gli::format::FORMAT_RGBA8_SNORM_PACK32 ) {
    return vk::Format::eA8B8G8R8SnormPack32;
  }
  else if( f == gli::format::FORMAT_RGBA8_USCALED_PACK32 ) {
    return vk::Format::eA8B8G8R8UscaledPack32;
  }
  else if( f == gli::format::FORMAT_RGBA8_SSCALED_PACK32 ) {
    return vk::Format::eA8B8G8R8SscaledPack32;
  }
  else if( f == gli::format::FORMAT_RGBA8_UINT_PACK32 ) {
    return vk::Format::eA8B8G8R8UintPack32;
  }
  else if( f == gli::format::FORMAT_RGBA8_SINT_PACK32 ) {
    return vk::Format::eA8B8G8R8SintPack32;
  }
  else if( f == gli::format::FORMAT_RGBA8_SRGB_PACK32 ) {
    return vk::Format::eA8B8G8R8SrgbPack32;
  }
  else if( f == gli::format::FORMAT_RGB10A2_UNORM_PACK32 ) {
    return vk::Format::eA2B10G10R10UnormPack32;
  }
  else if( f == gli::format::FORMAT_RGB10A2_SNORM_PACK32 ) {
    return vk::Format::eA2B10G10R10SnormPack32;
  }
  else if( f == gli::format::FORMAT_RGB10A2_USCALED_PACK32 ) {
    return vk::Format::eA2B10G10R10UscaledPack32;
  }
  else if( f == gli::format::FORMAT_RGB10A2_SSCALED_PACK32 ) {
    return vk::Format::eA2B10G10R10SscaledPack32;
  }
  else if( f == gli::format::FORMAT_RGB10A2_UINT_PACK32 ) {
    return vk::Format::eA2B10G10R10UintPack32;
  }
  else if( f == gli::format::FORMAT_RGB10A2_SINT_PACK32 ) {
    return vk::Format::eA2B10G10R10SintPack32;
  }
  else if( f == gli::format::FORMAT_BGR10A2_UNORM_PACK32 ) {
    return vk::Format::eA2R10G10B10UnormPack32;
  }
  else if( f == gli::format::FORMAT_BGR10A2_SNORM_PACK32 ) {
    return vk::Format::eA2R10G10B10SnormPack32;
  }
  else if( f == gli::format::FORMAT_BGR10A2_USCALED_PACK32 ) {
    return vk::Format::eA2R10G10B10UscaledPack32;
  }
  else if( f == gli::format::FORMAT_BGR10A2_SSCALED_PACK32 ) {
    return vk::Format::eA2R10G10B10SscaledPack32;
  }
  else if( f == gli::format::FORMAT_BGR10A2_UINT_PACK32 ) {
    return vk::Format::eA2R10G10B10UintPack32;
  }
  else if( f == gli::format::FORMAT_BGR10A2_SINT_PACK32 ) {
    return vk::Format::eA2R10G10B10SintPack32;
  }
  else if( f == gli::format::FORMAT_R16_UNORM_PACK16 ) {
    return vk::Format::eR16Unorm;
  }
  else if( f == gli::format::FORMAT_R16_SNORM_PACK16 ) {
    return vk::Format::eR16Snorm;
  }
  else if( f == gli::format::FORMAT_R16_USCALED_PACK16 ) {
    return vk::Format::eR16Uscaled;
  }
  else if( f == gli::format::FORMAT_R16_SSCALED_PACK16 ) {
    return vk::Format::eR16Sscaled;
  }
  else if( f == gli::format::FORMAT_R16_UINT_PACK16 ) {
    return vk::Format::eR16Uint;
  }
  else if( f == gli::format::FORMAT_R16_SINT_PACK16 ) {
    return vk::Format::eR16Sint;
  }
  else if( f == gli::format::FORMAT_R16_SFLOAT_PACK16 ) {
    return vk::Format::eR16Sfloat;
  }
  else if( f == gli::format::FORMAT_RG16_UNORM_PACK16 ) {
    return vk::Format::eR16G16Unorm;
  }
  else if( f == gli::format::FORMAT_RG16_SNORM_PACK16 ) {
    return vk::Format::eR16G16Snorm;
  }
  else if( f == gli::format::FORMAT_RG16_USCALED_PACK16 ) {
    return vk::Format::eR16G16Uscaled;
  }
  else if( f == gli::format::FORMAT_RG16_SSCALED_PACK16 ) {
    return vk::Format::eR16G16Sscaled;
  }
  else if( f == gli::format::FORMAT_RG16_UINT_PACK16 ) {
    return vk::Format::eR16G16Uint;
  }
  else if( f == gli::format::FORMAT_RG16_SINT_PACK16 ) {
    return vk::Format::eR16G16Sint;
  }
  else if( f == gli::format::FORMAT_RG16_SFLOAT_PACK16 ) {
    return vk::Format::eR16G16Sfloat;
  }
  else if( f == gli::format::FORMAT_RGB16_UNORM_PACK16 ) {
    return vk::Format::eR16G16B16Unorm;
  }
  else if( f == gli::format::FORMAT_RGB16_SNORM_PACK16 ) {
    return vk::Format::eR16G16B16Snorm;
  }
  else if( f == gli::format::FORMAT_RGB16_USCALED_PACK16 ) {
    return vk::Format::eR16G16B16Uscaled;
  }
  else if( f == gli::format::FORMAT_RGB16_SSCALED_PACK16 ) {
    return vk::Format::eR16G16B16Sscaled;
  }
  else if( f == gli::format::FORMAT_RGB16_UINT_PACK16 ) {
    return vk::Format::eR16G16B16Uint;
  }
  else if( f == gli::format::FORMAT_RGB16_SINT_PACK16 ) {
    return vk::Format::eR16G16B16Sint;
  }
  else if( f == gli::format::FORMAT_RGB16_SFLOAT_PACK16 ) {
    return vk::Format::eR16G16B16Sfloat;
  }
  else if( f == gli::format::FORMAT_RGBA16_UNORM_PACK16 ) {
    return vk::Format::eR16G16B16A16Unorm;
  }
  else if( f == gli::format::FORMAT_RGBA16_SNORM_PACK16 ) {
    return vk::Format::eR16G16B16A16Snorm;
  }
  else if( f == gli::format::FORMAT_RGBA16_USCALED_PACK16 ) {
    return vk::Format::eR16G16B16A16Uscaled;
  }
  else if( f == gli::format::FORMAT_RGBA16_SSCALED_PACK16 ) {
    return vk::Format::eR16G16B16A16Sscaled;
  }
  else if( f == gli::format::FORMAT_RGBA16_UINT_PACK16 ) {
    return vk::Format::eR16G16B16A16Uint;
  }
  else if( f == gli::format::FORMAT_RGBA16_SINT_PACK16 ) {
    return vk::Format::eR16G16B16A16Sint;
  }
  else if( f == gli::format::FORMAT_RGBA16_SFLOAT_PACK16 ) {
    return vk::Format::eR16G16B16A16Sfloat;
  }
  else if( f == gli::format::FORMAT_R32_UINT_PACK32 ) {
    return vk::Format::eR32Uint;
  }
  else if( f == gli::format::FORMAT_R32_SINT_PACK32 ) {
    return vk::Format::eR32Sint;
  }
  else if( f == gli::format::FORMAT_R32_SFLOAT_PACK32 ) {
    return vk::Format::eR32Sfloat;
  }
  else if( f == gli::format::FORMAT_RG32_UINT_PACK32 ) {
    return vk::Format::eR32G32Uint;
  }
  else if( f == gli::format::FORMAT_RG32_SINT_PACK32 ) {
    return vk::Format::eR32G32Sint;
  }
  else if( f == gli::format::FORMAT_RG32_SFLOAT_PACK32 ) {
    return vk::Format::eR32G32Sfloat;
  }
  else if( f == gli::format::FORMAT_RGB32_UINT_PACK32 ) {
    return vk::Format::eR32G32B32Uint;
  }
  else if( f == gli::format::FORMAT_RGB32_SINT_PACK32 ) {
    return vk::Format::eR32G32B32Sint;
  }
  else if( f == gli::format::FORMAT_RGB32_SFLOAT_PACK32 ) {
    return vk::Format::eR32G32B32Sfloat;
  }
  else if( f == gli::format::FORMAT_RGBA32_UINT_PACK32 ) {
    return vk::Format::eR32G32B32A32Uint;
  }
  else if( f == gli::format::FORMAT_RGBA32_SINT_PACK32 ) {
    return vk::Format::eR32G32B32A32Sint;
  }
  else if( f == gli::format::FORMAT_RGBA32_SFLOAT_PACK32 ) {
    return vk::Format::eR32G32B32A32Sfloat;
  }
  else if( f == gli::format::FORMAT_R32_UINT_PACK32 ) {
    return vk::Format::eR32Uint;
  }
  else if( f == gli::format::FORMAT_R32_SINT_PACK32 ) {
    return vk::Format::eR32Sint;
  }
  else if( f == gli::format::FORMAT_R32_SFLOAT_PACK32 ) {
    return vk::Format::eR32Sfloat;
  }
  else if( f == gli::format::FORMAT_RG32_UINT_PACK32 ) {
    return vk::Format::eR32G32Uint;
  }
  else if( f == gli::format::FORMAT_RG32_SINT_PACK32 ) {
    return vk::Format::eR32G32Sint;
  }
  else if( f == gli::format::FORMAT_RG32_SFLOAT_PACK32 ) {
    return vk::Format::eR32G32Sfloat;
  }
  else if( f == gli::format::FORMAT_RGB32_UINT_PACK32 ) {
    return vk::Format::eR32G32B32Uint;
  }
  else if( f == gli::format::FORMAT_RGB32_SINT_PACK32 ) {
    return vk::Format::eR32G32B32Sint;
  }
  else if( f == gli::format::FORMAT_RGB32_SFLOAT_PACK32 ) {
    return vk::Format::eR32G32B32Sfloat;
  }
  else if( f == gli::format::FORMAT_RGBA32_UINT_PACK32 ) {
    return vk::Format::eR32G32B32A32Uint;
  }
  else if( f == gli::format::FORMAT_RGBA32_SINT_PACK32 ) {
    return vk::Format::eR32G32B32A32Sint;
  }
  else if( f == gli::format::FORMAT_RGBA32_SFLOAT_PACK32 ) {
    return vk::Format::eR32G32B32A32Sfloat;
  }
  else if( f == gli::format::FORMAT_R64_UINT_PACK64 ) {
    return vk::Format::eR64Uint;
  }
  else if( f == gli::format::FORMAT_R64_SINT_PACK64 ) {
    return vk::Format::eR64Sint;
  }
  else if( f == gli::format::FORMAT_R64_SFLOAT_PACK64 ) {
    return vk::Format::eR64Sfloat;
  }
  else if( f == gli::format::FORMAT_RG64_UINT_PACK64 ) {
    return vk::Format::eR64G64Uint;
  }
  else if( f == gli::format::FORMAT_RG64_SINT_PACK64 ) {
    return vk::Format::eR64G64Sint;
  }
  else if( f == gli::format::FORMAT_RG64_SFLOAT_PACK64 ) {
    return vk::Format::eR64G64Sfloat;
  }
  else if( f == gli::format::FORMAT_RGB64_UINT_PACK64 ) {
    return vk::Format::eR64G64B64Uint;
  }
  else if( f == gli::format::FORMAT_RGB64_SINT_PACK64 ) {
    return vk::Format::eR64G64B64Sint;
  }
  else if( f == gli::format::FORMAT_RGB64_SFLOAT_PACK64 ) {
    return vk::Format::eR64G64B64Sfloat;
  }
  else if( f == gli::format::FORMAT_RGBA64_UINT_PACK64 ) {
    return vk::Format::eR64G64B64A64Uint;
  }
  else if( f == gli::format::FORMAT_RGBA64_SINT_PACK64 ) {
    return vk::Format::eR64G64B64A64Sint;
  }
  else if( f == gli::format::FORMAT_RGBA64_SFLOAT_PACK64 ) {
    return vk::Format::eR64G64B64A64Sfloat;
  }



  return vk::Format::eUndefined;
}

}
