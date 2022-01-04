#include <gct/format.hpp>

namespace gct {
  bool is_rgba( vk::Format format ) {
    if( format == vk::Format::eR4G4UnormPack8) return true;
    else if( format == vk::Format::eR4G4B4A4UnormPack16) return true;
    else if( format == vk::Format::eR5G6B5UnormPack16) return true;
    else if( format == vk::Format::eR5G5B5A1UnormPack16) return true;
    else if( format == vk::Format::eR8Unorm) return true;
    else if( format == vk::Format::eR8Snorm) return true;
    else if( format == vk::Format::eR8Uscaled) return true;
    else if( format == vk::Format::eR8Sscaled) return true;
    else if( format == vk::Format::eR8Uint) return true;
    else if( format == vk::Format::eR8Sint) return true;
    else if( format == vk::Format::eR8Srgb) return true;
    else if( format == vk::Format::eR8G8Unorm) return true;
    else if( format == vk::Format::eR8G8Snorm) return true;
    else if( format == vk::Format::eR8G8Uscaled) return true;
    else if( format == vk::Format::eR8G8Sscaled) return true;
    else if( format == vk::Format::eR8G8Uint) return true;
    else if( format == vk::Format::eR8G8Sint) return true;
    else if( format == vk::Format::eR8G8Srgb) return true;
    else if( format == vk::Format::eR8G8B8Unorm) return true;
    else if( format == vk::Format::eR8G8B8Snorm) return true;
    else if( format == vk::Format::eR8G8B8Uscaled) return true;
    else if( format == vk::Format::eR8G8B8Sscaled) return true;
    else if( format == vk::Format::eR8G8B8Uint) return true;
    else if( format == vk::Format::eR8G8B8Sint) return true;
    else if( format == vk::Format::eR8G8B8Srgb) return true;
    else if( format == vk::Format::eR8G8B8A8Unorm) return true;
    else if( format == vk::Format::eR8G8B8A8Snorm) return true;
    else if( format == vk::Format::eR8G8B8A8Uscaled) return true;
    else if( format == vk::Format::eR8G8B8A8Sscaled) return true;
    else if( format == vk::Format::eR8G8B8A8Uint) return true;
    else if( format == vk::Format::eR8G8B8A8Sint) return true;
    else if( format == vk::Format::eR8G8B8A8Srgb) return true;
    else if( format == vk::Format::eR16Unorm) return true;
    else if( format == vk::Format::eR16Snorm) return true;
    else if( format == vk::Format::eR16Uscaled) return true;
    else if( format == vk::Format::eR16Sscaled) return true;
    else if( format == vk::Format::eR16Uint) return true;
    else if( format == vk::Format::eR16Sint) return true;
    else if( format == vk::Format::eR16Sfloat) return true;
    else if( format == vk::Format::eR16G16Unorm) return true;
    else if( format == vk::Format::eR16G16Snorm) return true;
    else if( format == vk::Format::eR16G16Uscaled) return true;
    else if( format == vk::Format::eR16G16Sscaled) return true;
    else if( format == vk::Format::eR16G16Uint) return true;
    else if( format == vk::Format::eR16G16Sint) return true;
    else if( format == vk::Format::eR16G16Sfloat) return true;
    else if( format == vk::Format::eR16G16B16Unorm) return true;
    else if( format == vk::Format::eR16G16B16Snorm) return true;
    else if( format == vk::Format::eR16G16B16Uscaled) return true;
    else if( format == vk::Format::eR16G16B16Sscaled) return true;
    else if( format == vk::Format::eR16G16B16Uint) return true;
    else if( format == vk::Format::eR16G16B16Sint) return true;
    else if( format == vk::Format::eR16G16B16Sfloat) return true;
    else if( format == vk::Format::eR16G16B16A16Unorm) return true;
    else if( format == vk::Format::eR16G16B16A16Snorm) return true;
    else if( format == vk::Format::eR16G16B16A16Uscaled) return true;
    else if( format == vk::Format::eR16G16B16A16Sscaled) return true;
    else if( format == vk::Format::eR16G16B16A16Uint) return true;
    else if( format == vk::Format::eR16G16B16A16Sint) return true;
    else if( format == vk::Format::eR16G16B16A16Sfloat) return true;
    else if( format == vk::Format::eR32Uint) return true;
    else if( format == vk::Format::eR32Sint) return true;
    else if( format == vk::Format::eR32Sfloat) return true;
    else if( format == vk::Format::eR32G32Uint) return true;
    else if( format == vk::Format::eR32G32Sint) return true;
    else if( format == vk::Format::eR32G32Sfloat) return true;
    else if( format == vk::Format::eR32G32B32Uint) return true;
    else if( format == vk::Format::eR32G32B32Sint) return true;
    else if( format == vk::Format::eR32G32B32Sfloat) return true;
    else if( format == vk::Format::eR32G32B32A32Uint) return true;
    else if( format == vk::Format::eR32G32B32A32Sint) return true;
    else if( format == vk::Format::eR32G32B32A32Sfloat) return true;
    else if( format == vk::Format::eR64Uint) return true;
    else if( format == vk::Format::eR64Sint) return true;
    else if( format == vk::Format::eR64Sfloat) return true;
    else if( format == vk::Format::eR64G64Uint) return true;
    else if( format == vk::Format::eR64G64Sint) return true;
    else if( format == vk::Format::eR64G64Sfloat) return true;
    else if( format == vk::Format::eR64G64B64Uint) return true;
    else if( format == vk::Format::eR64G64B64Sint) return true;
    else if( format == vk::Format::eR64G64B64Sfloat) return true;
    else if( format == vk::Format::eR64G64B64A64Uint) return true;
    else if( format == vk::Format::eR64G64B64A64Sint) return true;
    else if( format == vk::Format::eR64G64B64A64Sfloat) return true;
#ifdef VK_VERSION_1_1
    else if( format == vk::Format::eR10X6UnormPack16) return true;
    else if( format == vk::Format::eR10X6G10X6Unorm2Pack16) return true;
    else if( format == vk::Format::eR10X6G10X6B10X6A10X6Unorm4Pack16) return true;
    else if( format == vk::Format::eR12X4UnormPack16) return true;
    else if( format == vk::Format::eR12X4G12X4Unorm2Pack16) return true;
    else if( format == vk::Format::eR12X4G12X4B12X4A12X4Unorm4Pack16) return true;
#endif
#ifdef VK_KHR_SAMPLER_YCBCR_CONVERSION_EXTENSION_NAME
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

