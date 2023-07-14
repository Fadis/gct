#include <gct/format.hpp>

namespace gct {
  enum class component_type_t {
    SIGNED_INTEGER,
    UNSIGNED_INTEGER,
    SIGNED_NORMALIZED,
    UNSIGNED_NORMALIZED,
    SIGNED_SCALED,
    UNSIGNED_SCALED,
    SRGB,
    FLOAT
  };
  vk::Format get_format(
    component_type_t component_type,
    unsigned int component_length,
    unsigned int channels
  ) {
    if( component_type ==  )
  }
  vk::Flags<vk::ImageAspectFlagBits> format_to_aspect( vk::Format format ) {
    if( format == vk::Format::eUndefined) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR4G4UnormPack8) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR4G4B4A4UnormPack16) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eB4G4R4A4UnormPack16) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR5G6B5UnormPack16) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eB5G6R5UnormPack16) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR5G5B5A1UnormPack16) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eB5G5R5A1UnormPack16) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eA1R5G5B5UnormPack16) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR8Unorm) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR8Snorm) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR8Uscaled) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR8Sscaled) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR8Uint) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR8Sint) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR8Srgb) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR8G8Unorm) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR8G8Snorm) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR8G8Uscaled) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR8G8Sscaled) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR8G8Uint) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR8G8Sint) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR8G8Srgb) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR8G8B8Unorm) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR8G8B8Snorm) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR8G8B8Uscaled) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR8G8B8Sscaled) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR8G8B8Uint) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR8G8B8Sint) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR8G8B8Srgb) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eB8G8R8Unorm) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eB8G8R8Snorm) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eB8G8R8Uscaled) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eB8G8R8Sscaled) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eB8G8R8Uint) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eB8G8R8Sint) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eB8G8R8Srgb) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR8G8B8A8Unorm) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR8G8B8A8Snorm) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR8G8B8A8Uscaled) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR8G8B8A8Sscaled) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR8G8B8A8Uint) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR8G8B8A8Sint) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR8G8B8A8Srgb) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eB8G8R8A8Unorm) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eB8G8R8A8Snorm) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eB8G8R8A8Uscaled) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eB8G8R8A8Sscaled) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eB8G8R8A8Uint) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eB8G8R8A8Sint) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eB8G8R8A8Srgb) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eA8B8G8R8UnormPack32) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eA8B8G8R8SnormPack32) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eA8B8G8R8UscaledPack32) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eA8B8G8R8SscaledPack32) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eA8B8G8R8UintPack32) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eA8B8G8R8SintPack32) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eA8B8G8R8SrgbPack32) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eA2R10G10B10UnormPack32) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eA2R10G10B10SnormPack32) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eA2R10G10B10UscaledPack32) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eA2R10G10B10SscaledPack32) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eA2R10G10B10UintPack32) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eA2R10G10B10SintPack32) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eA2B10G10R10UnormPack32) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eA2B10G10R10SnormPack32) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eA2B10G10R10UscaledPack32) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eA2B10G10R10SscaledPack32) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eA2B10G10R10UintPack32) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eA2B10G10R10SintPack32) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR16Unorm) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR16Snorm) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR16Uscaled) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR16Sscaled) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR16Uint) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR16Sint) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR16Sfloat) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR16G16Unorm) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR16G16Snorm) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR16G16Uscaled) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR16G16Sscaled) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR16G16Uint) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR16G16Sint) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR16G16Sfloat) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR16G16B16Unorm) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR16G16B16Snorm) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR16G16B16Uscaled) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR16G16B16Sscaled) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR16G16B16Uint) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR16G16B16Sint) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR16G16B16Sfloat) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR16G16B16A16Unorm) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR16G16B16A16Snorm) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR16G16B16A16Uscaled) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR16G16B16A16Sscaled) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR16G16B16A16Uint) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR16G16B16A16Sint) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR16G16B16A16Sfloat) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR32Uint) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR32Sint) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR32Sfloat) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR32G32Uint) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR32G32Sint) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR32G32Sfloat) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR32G32B32Uint) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR32G32B32Sint) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR32G32B32Sfloat) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR32G32B32A32Uint) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR32G32B32A32Sint) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR32G32B32A32Sfloat) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR64Uint) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR64Sint) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR64Sfloat) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR64G64Uint) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR64G64Sint) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR64G64Sfloat) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR64G64B64Uint) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR64G64B64Sint) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR64G64B64Sfloat) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR64G64B64A64Uint) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR64G64B64A64Sint) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR64G64B64A64Sfloat) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eB10G11R11UfloatPack32) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eE5B9G9R9UfloatPack32) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eD16Unorm) return vk::ImageAspectFlagBits::eDepth;
    else if( format == vk::Format::eX8D24UnormPack32) return vk::ImageAspectFlagBits::eDepth;
    else if( format == vk::Format::eD32Sfloat) return vk::ImageAspectFlagBits::eDepth;
    else if( format == vk::Format::eS8Uint) return vk::ImageAspectFlagBits::eStencil;
    else if( format == vk::Format::eD16UnormS8Uint) return vk::ImageAspectFlagBits::eDepth|vk::ImageAspectFlagBits::eStencil;
    else if( format == vk::Format::eD24UnormS8Uint) return vk::ImageAspectFlagBits::eDepth|vk::ImageAspectFlagBits::eStencil;
    else if( format == vk::Format::eD32SfloatS8Uint) return vk::ImageAspectFlagBits::eDepth|vk::ImageAspectFlagBits::eStencil;
    else if( format == vk::Format::eBc1RgbUnormBlock) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eBc1RgbSrgbBlock) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eBc1RgbaUnormBlock) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eBc1RgbaSrgbBlock) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eBc2UnormBlock) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eBc2SrgbBlock) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eBc3UnormBlock) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eBc3SrgbBlock) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eBc4UnormBlock) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eBc4SnormBlock) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eBc5UnormBlock) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eBc5SnormBlock) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eBc6HUfloatBlock) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eBc6HSfloatBlock) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eBc7UnormBlock) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eBc7SrgbBlock) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eEtc2R8G8B8UnormBlock) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eEtc2R8G8B8SrgbBlock) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eEtc2R8G8B8A1UnormBlock) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eEtc2R8G8B8A1SrgbBlock) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eEtc2R8G8B8A8UnormBlock) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eEtc2R8G8B8A8SrgbBlock) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eEacR11UnormBlock) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eEacR11SnormBlock) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eEacR11G11UnormBlock) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eEacR11G11SnormBlock) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eAstc4x4UnormBlock) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eAstc4x4SrgbBlock) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eAstc5x4UnormBlock) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eAstc5x4SrgbBlock) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eAstc5x5UnormBlock) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eAstc5x5SrgbBlock) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eAstc6x5UnormBlock) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eAstc6x5SrgbBlock) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eAstc6x6UnormBlock) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eAstc6x6SrgbBlock) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eAstc8x5UnormBlock) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eAstc8x5SrgbBlock) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eAstc8x6UnormBlock) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eAstc8x6SrgbBlock) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eAstc8x8UnormBlock) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eAstc8x8SrgbBlock) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eAstc10x5UnormBlock) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eAstc10x5SrgbBlock) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eAstc10x6UnormBlock) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eAstc10x6SrgbBlock) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eAstc10x8UnormBlock) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eAstc10x8SrgbBlock) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eAstc10x10UnormBlock) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eAstc10x10SrgbBlock) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eAstc12x10UnormBlock) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eAstc12x10SrgbBlock) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eAstc12x12UnormBlock) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eAstc12x12SrgbBlock) return vk::ImageAspectFlagBits::eColor;
#ifdef VK_VERSION_1_1
    else if( format == vk::Format::eG8B8G8R8422Unorm) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eB8G8R8G8422Unorm) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eG8B8R83Plane420Unorm) return vk::ImageAspectFlagBits::ePlane0|vk::ImageAspectFlagBits::ePlane1|vk::ImageAspectFlagBits::ePlane2;
    else if( format == vk::Format::eG8B8R82Plane420Unorm) return vk::ImageAspectFlagBits::ePlane0|vk::ImageAspectFlagBits::ePlane1;
    else if( format == vk::Format::eG8B8R83Plane422Unorm) return vk::ImageAspectFlagBits::ePlane0|vk::ImageAspectFlagBits::ePlane1|vk::ImageAspectFlagBits::ePlane2;
    else if( format == vk::Format::eG8B8R82Plane422Unorm) return vk::ImageAspectFlagBits::ePlane0|vk::ImageAspectFlagBits::ePlane1;
    else if( format == vk::Format::eG8B8R83Plane444Unorm) return vk::ImageAspectFlagBits::ePlane0|vk::ImageAspectFlagBits::ePlane1|vk::ImageAspectFlagBits::ePlane2;
    else if( format == vk::Format::eR10X6UnormPack16) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR10X6G10X6Unorm2Pack16) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR10X6G10X6B10X6A10X6Unorm4Pack16) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eG10X6B10X6G10X6R10X6422Unorm4Pack16) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eB10X6G10X6R10X6G10X6422Unorm4Pack16) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eG10X6B10X6R10X63Plane420Unorm3Pack16) return vk::ImageAspectFlagBits::ePlane0|vk::ImageAspectFlagBits::ePlane1|vk::ImageAspectFlagBits::ePlane2;
    else if( format == vk::Format::eG10X6B10X6R10X62Plane420Unorm3Pack16) return vk::ImageAspectFlagBits::ePlane0|vk::ImageAspectFlagBits::ePlane1;
    else if( format == vk::Format::eG10X6B10X6R10X63Plane422Unorm3Pack16) return vk::ImageAspectFlagBits::ePlane0|vk::ImageAspectFlagBits::ePlane1|vk::ImageAspectFlagBits::ePlane2;
    else if( format == vk::Format::eG10X6B10X6R10X62Plane422Unorm3Pack16) return vk::ImageAspectFlagBits::ePlane0|vk::ImageAspectFlagBits::ePlane1;
    else if( format == vk::Format::eG10X6B10X6R10X63Plane444Unorm3Pack16) return vk::ImageAspectFlagBits::ePlane0|vk::ImageAspectFlagBits::ePlane1|vk::ImageAspectFlagBits::ePlane2;
    else if( format == vk::Format::eR12X4UnormPack16) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR12X4G12X4Unorm2Pack16) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR12X4G12X4B12X4A12X4Unorm4Pack16) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eG12X4B12X4G12X4R12X4422Unorm4Pack16) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eB12X4G12X4R12X4G12X4422Unorm4Pack16) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eG12X4B12X4R12X43Plane420Unorm3Pack16) return vk::ImageAspectFlagBits::ePlane0|vk::ImageAspectFlagBits::ePlane1|vk::ImageAspectFlagBits::ePlane2;
    else if( format == vk::Format::eG12X4B12X4R12X42Plane420Unorm3Pack16) return vk::ImageAspectFlagBits::ePlane0|vk::ImageAspectFlagBits::ePlane1;
    else if( format == vk::Format::eG12X4B12X4R12X43Plane422Unorm3Pack16) return vk::ImageAspectFlagBits::ePlane0|vk::ImageAspectFlagBits::ePlane1|vk::ImageAspectFlagBits::ePlane2;
    else if( format == vk::Format::eG12X4B12X4R12X42Plane422Unorm3Pack16) return vk::ImageAspectFlagBits::ePlane0|vk::ImageAspectFlagBits::ePlane1;
    else if( format == vk::Format::eG12X4B12X4R12X43Plane444Unorm3Pack16) return vk::ImageAspectFlagBits::ePlane0|vk::ImageAspectFlagBits::ePlane1|vk::ImageAspectFlagBits::ePlane2;
    else if( format == vk::Format::eG16B16G16R16422Unorm) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eB16G16R16G16422Unorm) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eG16B16R163Plane420Unorm) return vk::ImageAspectFlagBits::ePlane0|vk::ImageAspectFlagBits::ePlane1|vk::ImageAspectFlagBits::ePlane2;
    else if( format == vk::Format::eG16B16R162Plane420Unorm) return vk::ImageAspectFlagBits::ePlane0|vk::ImageAspectFlagBits::ePlane1;
    else if( format == vk::Format::eG16B16R163Plane422Unorm) return vk::ImageAspectFlagBits::ePlane0|vk::ImageAspectFlagBits::ePlane1|vk::ImageAspectFlagBits::ePlane2;
    else if( format == vk::Format::eG16B16R162Plane422Unorm) return vk::ImageAspectFlagBits::ePlane0|vk::ImageAspectFlagBits::ePlane1;
    else if( format == vk::Format::eG16B16R163Plane444Unorm) return vk::ImageAspectFlagBits::ePlane0|vk::ImageAspectFlagBits::ePlane1|vk::ImageAspectFlagBits::ePlane2;
#endif
#ifdef VK_IMG_FORMAT_PVRTC_EXTENSION_NAME
    else if( format == vk::Format::ePvrtc12BppUnormBlockIMG) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::ePvrtc14BppUnormBlockIMG) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::ePvrtc22BppUnormBlockIMG) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::ePvrtc24BppUnormBlockIMG) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::ePvrtc12BppSrgbBlockIMG) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::ePvrtc14BppSrgbBlockIMG) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::ePvrtc22BppSrgbBlockIMG) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::ePvrtc24BppSrgbBlockIMG) return vk::ImageAspectFlagBits::eColor;
#endif
#ifdef VK_EXT_TEXTURE_COMPRESSION_ASTC_HDR_EXTENSION_NAME
    else if( format == vk::Format::eAstc4x4SfloatBlockEXT) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eAstc5x4SfloatBlockEXT) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eAstc5x5SfloatBlockEXT) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eAstc6x5SfloatBlockEXT) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eAstc6x6SfloatBlockEXT) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eAstc8x5SfloatBlockEXT) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eAstc8x6SfloatBlockEXT) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eAstc8x8SfloatBlockEXT) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eAstc10x5SfloatBlockEXT) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eAstc10x6SfloatBlockEXT) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eAstc10x8SfloatBlockEXT) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eAstc10x10SfloatBlockEXT) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eAstc12x10SfloatBlockEXT) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eAstc12x12SfloatBlockEXT) return vk::ImageAspectFlagBits::eColor;
#endif
#ifdef VK_EXT_YCBCR_2PLANE_444_FORMATS_EXTENSION_NAME
    else if( format == vk::Format::eG8B8R82Plane444UnormEXT) return vk::ImageAspectFlagBits::ePlane0|vk::ImageAspectFlagBits::ePlane1;
    else if( format == vk::Format::eG10X6B10X6R10X62Plane444Unorm3Pack16EXT) return vk::ImageAspectFlagBits::ePlane0|vk::ImageAspectFlagBits::ePlane1;
    else if( format == vk::Format::eG12X4B12X4R12X42Plane444Unorm3Pack16EXT) return vk::ImageAspectFlagBits::ePlane0|vk::ImageAspectFlagBits::ePlane1;
    else if( format == vk::Format::eG16B16R162Plane444UnormEXT) return vk::ImageAspectFlagBits::ePlane0|vk::ImageAspectFlagBits::ePlane1;
#endif
#ifdef VK_EXT_4444_FORMATS_EXTENSION_NAME
    else if( format == vk::Format::eA4R4G4B4UnormPack16EXT) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eA4B4G4R4UnormPack16EXT) return vk::ImageAspectFlagBits::eColor;
#endif
#ifdef VK_KHR_SAMPLER_YCBCR_CONVERSION_EXTENSION_NAME
    else if( format == vk::Format::eB10X6G10X6R10X6G10X6422Unorm4Pack16KHR) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eB12X4G12X4R12X4G12X4422Unorm4Pack16KHR) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eB16G16R16G16422UnormKHR) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eB8G8R8G8422UnormKHR) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eG10X6B10X6G10X6R10X6422Unorm4Pack16KHR) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eG10X6B10X6R10X62Plane420Unorm3Pack16KHR) return vk::ImageAspectFlagBits::ePlane0|vk::ImageAspectFlagBits::ePlane1;
    else if( format == vk::Format::eG10X6B10X6R10X62Plane422Unorm3Pack16KHR) return vk::ImageAspectFlagBits::ePlane0|vk::ImageAspectFlagBits::ePlane1;
    else if( format == vk::Format::eG10X6B10X6R10X63Plane420Unorm3Pack16KHR) return vk::ImageAspectFlagBits::ePlane0|vk::ImageAspectFlagBits::ePlane1|vk::ImageAspectFlagBits::ePlane2;
    else if( format == vk::Format::eG10X6B10X6R10X63Plane422Unorm3Pack16KHR) return vk::ImageAspectFlagBits::ePlane0|vk::ImageAspectFlagBits::ePlane1|vk::ImageAspectFlagBits::ePlane2;
    else if( format == vk::Format::eG10X6B10X6R10X63Plane444Unorm3Pack16KHR) return vk::ImageAspectFlagBits::ePlane0|vk::ImageAspectFlagBits::ePlane1|vk::ImageAspectFlagBits::ePlane2;
    else if( format == vk::Format::eG12X4B12X4G12X4R12X4422Unorm4Pack16KHR) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eG12X4B12X4R12X42Plane420Unorm3Pack16KHR) return vk::ImageAspectFlagBits::ePlane0|vk::ImageAspectFlagBits::ePlane1;
    else if( format == vk::Format::eG12X4B12X4R12X42Plane422Unorm3Pack16KHR) return vk::ImageAspectFlagBits::ePlane0|vk::ImageAspectFlagBits::ePlane1;
    else if( format == vk::Format::eG12X4B12X4R12X43Plane420Unorm3Pack16KHR) return vk::ImageAspectFlagBits::ePlane0|vk::ImageAspectFlagBits::ePlane1|vk::ImageAspectFlagBits::ePlane2;
    else if( format == vk::Format::eG12X4B12X4R12X43Plane422Unorm3Pack16KHR) return vk::ImageAspectFlagBits::ePlane0|vk::ImageAspectFlagBits::ePlane1|vk::ImageAspectFlagBits::ePlane2;
    else if( format == vk::Format::eG12X4B12X4R12X43Plane444Unorm3Pack16KHR) return vk::ImageAspectFlagBits::ePlane0|vk::ImageAspectFlagBits::ePlane1|vk::ImageAspectFlagBits::ePlane2;
    else if( format == vk::Format::eG16B16G16R16422UnormKHR) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eG16B16R162Plane420UnormKHR) return vk::ImageAspectFlagBits::ePlane0|vk::ImageAspectFlagBits::ePlane1;
    else if( format == vk::Format::eG16B16R162Plane422UnormKHR) return vk::ImageAspectFlagBits::ePlane0|vk::ImageAspectFlagBits::ePlane1;
    else if( format == vk::Format::eG16B16R163Plane420UnormKHR) return vk::ImageAspectFlagBits::ePlane0|vk::ImageAspectFlagBits::ePlane1|vk::ImageAspectFlagBits::ePlane2;
    else if( format == vk::Format::eG16B16R163Plane422UnormKHR) return vk::ImageAspectFlagBits::ePlane0|vk::ImageAspectFlagBits::ePlane1|vk::ImageAspectFlagBits::ePlane2;
    else if( format == vk::Format::eG16B16R163Plane444UnormKHR) return vk::ImageAspectFlagBits::ePlane0|vk::ImageAspectFlagBits::ePlane1|vk::ImageAspectFlagBits::ePlane2;
    else if( format == vk::Format::eG8B8G8R8422UnormKHR) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eG8B8R82Plane420UnormKHR) return vk::ImageAspectFlagBits::ePlane0|vk::ImageAspectFlagBits::ePlane1;
    else if( format == vk::Format::eG8B8R82Plane422UnormKHR) return vk::ImageAspectFlagBits::ePlane0|vk::ImageAspectFlagBits::ePlane1;
    else if( format == vk::Format::eG8B8R83Plane420UnormKHR) return vk::ImageAspectFlagBits::ePlane0|vk::ImageAspectFlagBits::ePlane1|vk::ImageAspectFlagBits::ePlane2;
    else if( format == vk::Format::eG8B8R83Plane422UnormKHR) return vk::ImageAspectFlagBits::ePlane0|vk::ImageAspectFlagBits::ePlane1|vk::ImageAspectFlagBits::ePlane2;
    else if( format == vk::Format::eG8B8R83Plane444UnormKHR) return vk::ImageAspectFlagBits::ePlane0|vk::ImageAspectFlagBits::ePlane1|vk::ImageAspectFlagBits::ePlane2;
    else if( format == vk::Format::eR10X6G10X6B10X6A10X6Unorm4Pack16KHR) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR10X6G10X6Unorm2Pack16KHR) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR10X6UnormPack16KHR) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR12X4G12X4B12X4A12X4Unorm4Pack16KHR) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR12X4G12X4Unorm2Pack16KHR) return vk::ImageAspectFlagBits::eColor;
    else if( format == vk::Format::eR12X4UnormPack16KHR) return vk::ImageAspectFlagBits::eColor;
#endif
    else return vk::ImageAspectFlagBits::eColor;
  }
}
