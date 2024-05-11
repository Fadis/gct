#include <nlohmann/json.hpp>
#include <gct/buffer.hpp>
#include <gct/conditional_rendering_begin_info.hpp>
#ifdef VK_EXT_CONDITIONAL_RENDERING_EXTENSION_NAME
#include <vulkan2json/ConditionalRenderingBeginInfoEXT.hpp>

namespace gct {
  void to_json( nlohmann::json &root, const conditional_rendering_begin_info_t &v ) {
     root = nlohmann::json::object();
     root[ "basic" ] = v.get_basic();
     if( v.get_buffer() ) {
       root[ "basic" ][ "buffer" ] = *v.get_buffer();
     }
  }
  void from_json( const nlohmann::json &root, conditional_rendering_begin_info_t &v ) {
    if( !root.is_object() ) throw incompatible_json( "The JSON is incompatible to conditional_rendering_begin_info_t", __FILE__, __LINE__ );
    LIBGCT_EXTENSION_FROM_JSON( basic )
  }
  conditional_rendering_begin_info_t &conditional_rendering_begin_info_t::rebuild_chain() {
    if( buf ) {
      basic.buffer = **buf;
    }
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
}

#endif

