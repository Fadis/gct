#include <gct/format.hpp>

namespace gct {
  bool is_bgra( vk::Format format ) {
    if( format == vk::Format::eB4G4R4A4UnormPack16) return true;
    else if( format == vk::Format::eB5G6R5UnormPack16) return true;
    else if( format == vk::Format::eB5G5R5A1UnormPack16) return true;
    else if( format == vk::Format::eB8G8R8Unorm) return true;
    else if( format == vk::Format::eB8G8R8Snorm) return true;
    else if( format == vk::Format::eB8G8R8Uscaled) return true;
    else if( format == vk::Format::eB8G8R8Sscaled) return true;
    else if( format == vk::Format::eB8G8R8Uint) return true;
    else if( format == vk::Format::eB8G8R8Sint) return true;
    else if( format == vk::Format::eB8G8R8Srgb) return true;
    else if( format == vk::Format::eB8G8R8A8Unorm) return true;
    else if( format == vk::Format::eB8G8R8A8Snorm) return true;
    else if( format == vk::Format::eB8G8R8A8Uscaled) return true;
    else if( format == vk::Format::eB8G8R8A8Sscaled) return true;
    else if( format == vk::Format::eB8G8R8A8Uint) return true;
    else if( format == vk::Format::eB8G8R8A8Sint) return true;
    else if( format == vk::Format::eB8G8R8A8Srgb) return true;
    else if( format == vk::Format::eB10G11R11UfloatPack32) return true;
    else return false;
  }
}

