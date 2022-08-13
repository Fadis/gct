#include <gct/surface.hpp>
#include <gct/render_pass_begin_info.hpp>
#include <vulkan2json/RenderPassBeginInfo.hpp>
#ifdef VK_VERSION_1_1
#include <vulkan2json/DeviceGroupRenderPassBeginInfo.hpp>
#endif
#ifdef VK_VERSION_1_2
#include <vulkan2json/RenderPassAttachmentBeginInfo.hpp>
#elif defined(VK_KHR_IMAGELESS_FRAMEBUFFER_EXTENSION_NAME)
#include <vulkan2json/RenderPassAttachmentBeginInfoKHR.hpp>
#endif
#ifdef VK_EXT_SAMPLE_LOCATIONS_EXTENSION_NAME
#include <vulkan2json/RenderPassSampleLocationsBeginInfoEXT.hpp>
#endif
#ifdef VK_QCOM_RENDER_PASS_TRANSFORM_EXTENSION_NAME
#include <vulkan2json/RenderPassTransformBeginInfoQCOM.hpp>
#endif

namespace gct {
  void to_json( nlohmann::json &root, const render_pass_begin_info_t &v ) {
     root = nlohmann::json::object();
     root[ "basic" ] = v.get_basic();
#ifdef VK_VERSION_1_1
    LIBGCT_EXTENSION_TO_JSON( group )
#endif
#ifdef VK_VERSION_1_2
    LIBGCT_EXTENSION_TO_JSON( attachment )
#elif defined(VK_KHR_IMAGELESS_FRAMEBUFFER_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( attachment )
#endif
#ifdef VK_EXT_SAMPLE_LOCATIONS_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( sample_location )
#endif
#ifdef VK_QCOM_RENDER_PASS_TRANSFORM_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( transform )
#endif
  }
  void from_json( const nlohmann::json &root, render_pass_begin_info_t &v ) {
    if( !root.is_object() ) throw incompatible_json( "The JSON is incompatible to render_pass_create_info_t", __FILE__, __LINE__ );
    LIBGCT_EXTENSION_FROM_JSON( basic )
#ifdef VK_VERSION_1_1
    LIBGCT_EXTENSION_FROM_JSON( group )
#endif
#ifdef VK_VERSION_1_2
    LIBGCT_EXTENSION_FROM_JSON( attachment )
#elif defined(VK_KHR_IMAGELESS_FRAMEBUFFER_EXTENSION_NAME)
    LIBGCT_EXTENSION_FROM_JSON( attachment )
#endif
#ifdef VK_EXT_SAMPLE_LOCATIONS_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( sample_location )
#endif
#ifdef VK_QCOM_RENDER_PASS_TRANSFORM_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( transform )
#endif
  }
  render_pass_begin_info_t &render_pass_begin_info_t::rebuild_chain() {
    basic
      .setClearValueCount( clear_value.size() )
      .setPClearValues( clear_value.data() );
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
#ifdef VK_VERSION_1_1
    LIBGCT_EXTENSION_REBUILD_CHAIN( group )
#endif
#ifdef VK_VERSION_1_2
    LIBGCT_EXTENSION_REBUILD_CHAIN( attachment )
#elif defined(VK_KHR_IMAGELESS_FRAMEBUFFER_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( attachment )
#endif
#ifdef VK_EXT_SAMPLE_LOCATIONS_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( sample_location )
#endif
#ifdef VK_QCOM_RENDER_PASS_TRANSFORM_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( transform )
#endif
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
  render_pass_begin_info_t &render_pass_begin_info_t::add_clear_value( const vk::ClearValue &v ) {
    clear_value.push_back( v );
    chained = false;
    return *this;
  }
  render_pass_begin_info_t &render_pass_begin_info_t::clear_clear_value() {
    clear_value.clear();
    chained = false;
    return *this;
  }
}

