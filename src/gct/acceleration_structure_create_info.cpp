#include <gct/buffer.hpp>
#include <gct/acceleration_structure_create_info.hpp>
#ifdef VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME
#include <vulkan2json/AccelerationStructureCreateInfoKHR.hpp>
#ifdef VK_NV_RAY_TRACING_MOTION_BLUR_EXTENSION_NAME
#include <vulkan2json/AccelerationStructureMotionInfoNV.hpp>
#endif
#endif
namespace gct {
#ifdef VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME
  void to_json( nlohmann::json &root, const acceleration_structure_create_info_t &v ) {
    root = nlohmann::json::object();
    root[ "basic" ] = v.get_basic();
#ifdef VK_NV_RAY_TRACING_MOTION_BLUR_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( motion )
#endif
  }
  void from_json( const nlohmann::json &root, acceleration_structure_create_info_t &v ) {
    if( !root.is_object() ) throw incompatible_json( "The JSON is incompatible to acceleration_structure_create_info_t", __FILE__, __LINE__ );
    LIBGCT_EXTENSION_FROM_JSON( basic )
#ifdef VK_NV_RAY_TRACING_MOTION_BLUR_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( motion )
#endif
  }

  acceleration_structure_create_info_t &acceleration_structure_create_info_t::rebuild_chain() {
    if( chained ) return *this;
    if( buffer && get_basic().size == 0u ) {
      auto basic = get_basic();
      if( buffer->get_props().get_basic().size < basic.offset ) throw -1;
      basic
        .setSize( buffer->get_props().get_basic().size - basic.offset )
        .setBuffer( **buffer );
      set_basic( std::move( basic ) );
    }
    else {
      auto basic = get_basic();
      if( buffer->get_props().get_basic().size < basic.offset ) throw -1;
      if( buffer->get_props().get_basic().size < basic.offset + basic.size ) throw -1;
      basic
        .setBuffer( **buffer );
      set_basic( std::move( basic ) );
    }
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
#ifdef VK_NV_RAY_TRACING_MOTION_BLUR_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( motion ) 
#endif
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }

  acceleration_structure_create_info_t &acceleration_structure_create_info_t::set_buffer( const std::shared_ptr< buffer_t > &v ) {
    buffer = v;
    chained = false;
    return *this;
  }
#endif
}

