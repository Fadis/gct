#include <gct/format.hpp>
#include <vulkan/vulkan_enums.hpp>
namespace gct {
bool is_available_for( vk::ImageUsageFlags usage, vk::FormatFeatureFlags feature ) {
  if( usage & vk::ImageUsageFlagBits::eSampled && !( feature & vk::FormatFeatureFlagBits::eSampledImage ) ) return false;
  if( usage & vk::ImageUsageFlagBits::eStorage && !( feature & vk::FormatFeatureFlagBits::eStorageImage ) ) return false;
  if( usage & vk::ImageUsageFlagBits::eColorAttachment && !( feature & vk::FormatFeatureFlagBits::eColorAttachment ) ) return false;
  if( usage & vk::ImageUsageFlagBits::eInputAttachment && !( feature & vk::FormatFeatureFlagBits::eColorAttachment ) ) return false;
  if( usage & vk::ImageUsageFlagBits::eDepthStencilAttachment && !( feature & vk::FormatFeatureFlagBits::eDepthStencilAttachment ) ) return false;
#ifdef VK_VERSION_1_1
  if( usage & vk::ImageUsageFlagBits::eTransferSrc && !( feature & vk::FormatFeatureFlagBits::eTransferSrc ) ) return false;
  if( usage & vk::ImageUsageFlagBits::eTransferDst && !( feature & vk::FormatFeatureFlagBits::eTransferDst ) ) return false;
#elif defiend(VK_KHR_MAINTENANCE1_EXTENSION_NAME)
  if( usage & vk::ImageUsageFlagBits::eTransferSrc && !( feature & vk::FormatFeatureFlagBits::eTransferSrcKHR ) ) return false;
  if( usage & vk::ImageUsageFlagBits::eTransferDst && !( feature & vk::FormatFeatureFlagBits::eTransferDstKHR ) ) return false;
#endif
#ifdef VK_EXT_FRAGMENT_DENSITY_MAP_EXTENSION_NAME
  if( usage & vk::ImageUsageFlagBits::eFragmentDensityMapEXT && !( feature & vk::FormatFeatureFlagBits::eFragmentDensityMapEXT ) ) return false;
#endif
#ifdef VK_KHR_FRAGMENT_SHADING_RATE_EXTENSION_NAME
  if( usage & vk::ImageUsageFlagBits::eFragmentShadingRateAttachmentKHR && !( feature & vk::FormatFeatureFlagBits::eFragmentShadingRateAttachmentKHR ) ) return false;
#endif
#ifdef VK_KHR_VIDEO_DECODE_QUEUE_EXTENSION_NAME
  if( usage & vk::ImageUsageFlagBits::eVideoDecodeDpbKHR && !( feature & vk::FormatFeatureFlagBits::eVideoDecodeDpbKHR ) ) return false;
  if( usage & vk::ImageUsageFlagBits::eVideoDecodeDstKHR && !( feature & vk::FormatFeatureFlagBits::eVideoDecodeOutputKHR ) ) return false;
  if( usage & vk::ImageUsageFlagBits::eVideoDecodeSrcKHR && !( feature & vk::FormatFeatureFlagBits::eVideoDecodeDpbKHR ) ) return false;
#endif
#ifdef VK_KHR_VIDEO_ENCODE_QUEUE_EXTENSION_NAME
  if( usage & vk::ImageUsageFlagBits::eVideoEncodeDpbKHR && !( feature & vk::FormatFeatureFlagBits::eVideoEncodeDpbKHR ) ) return false;
  if( usage & vk::ImageUsageFlagBits::eVideoEncodeSrcKHR && !( feature & vk::FormatFeatureFlagBits::eVideoEncodeInputKHR ) ) return false;
  if( usage & vk::ImageUsageFlagBits::eVideoEncodeDstKHR && !( feature & vk::FormatFeatureFlagBits::eVideoEncodeDpbKHR ) ) return false;
#endif
  return true;
}
}
