#include <fstream>
#include <iterator>
#include <gct/sampler_create_info.hpp>

namespace gct {
  sampler_create_info_t &sampler_create_info_t::rebuild_chain() {
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
#ifdef VK_EXT_BORDER_COLOR_SWIZZLE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( border_color_component_mapping ) 
#endif
#ifdef VK_EXT_CUSTOM_BORDER_COLOR_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( custom_border_color ) 
#endif
#if defined(VK_VERSION_1_2) || defined(VK_EXT_SAMPLER_FILTER_MINMAX_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( reduction_mode ) 
#endif
#if defined(VK_VERSION_1_1) || defined(VK_KHR_SAMPLER_YCBCR_CONVERSION_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( ycbcr_conversion )
#endif
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
}

