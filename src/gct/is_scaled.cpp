#include <gct/format.hpp>

namespace gct {
  bool is_scaled( vk::Format format ) {
    if( format == vk::Format::eR8Uscaled) return true;
    else if( format == vk::Format::eR8Sscaled) return true;
    else if( format == vk::Format::eR8G8Uscaled) return true;
    else if( format == vk::Format::eR8G8Sscaled) return true;
    else if( format == vk::Format::eR8G8B8Uscaled) return true;
    else if( format == vk::Format::eR8G8B8Sscaled) return true;
    else if( format == vk::Format::eB8G8R8Uscaled) return true;
    else if( format == vk::Format::eB8G8R8Sscaled) return true;
    else if( format == vk::Format::eR8G8B8A8Uscaled) return true;
    else if( format == vk::Format::eR8G8B8A8Sscaled) return true;
    else if( format == vk::Format::eB8G8R8A8Uscaled) return true;
    else if( format == vk::Format::eB8G8R8A8Sscaled) return true;
    else if( format == vk::Format::eA8B8G8R8UscaledPack32) return true;
    else if( format == vk::Format::eA8B8G8R8SscaledPack32) return true;
    else if( format == vk::Format::eA2R10G10B10UscaledPack32) return true;
    else if( format == vk::Format::eA2R10G10B10SscaledPack32) return true;
    else if( format == vk::Format::eA2B10G10R10UscaledPack32) return true;
    else if( format == vk::Format::eA2B10G10R10SscaledPack32) return true;
    else if( format == vk::Format::eR16Uscaled) return true;
    else if( format == vk::Format::eR16Sscaled) return true;
    else if( format == vk::Format::eR16G16Uscaled) return true;
    else if( format == vk::Format::eR16G16Sscaled) return true;
    else if( format == vk::Format::eR16G16B16Uscaled) return true;
    else if( format == vk::Format::eR16G16B16Sscaled) return true;
    else if( format == vk::Format::eR16G16B16A16Uscaled) return true;
    else if( format == vk::Format::eR16G16B16A16Sscaled) return true;
    else return false;
  }
}

