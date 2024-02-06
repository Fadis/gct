#ifndef GCT_ACCELERATION_STRUCTURE_CREATE_INFO_HPP
#define GCT_ACCELERATION_STRUCTURE_CREATE_INFO_HPP
#include <memory>
#include <vulkan/vulkan.hpp>
#include <gct/extension.hpp>
#include <nlohmann/json_fwd.hpp>

namespace gct {
#ifdef VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME
  class buffer_t;
  class acceleration_structure_create_info_t : public chained_t {
  public:
    using self_type = acceleration_structure_create_info_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
    LIBGCT_BASIC_SETTER( vk::AccelerationStructureCreateInfoKHR )
#ifdef VK_NV_RAY_TRACING_MOTION_BLUR_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::AccelerationStructureMotionInfoNV , motion )
#endif
#ifdef VK_EXT_DESCRIPTOR_BUFFER_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::OpaqueCaptureDescriptorDataCreateInfoEXT, opaque_capture_descriptor_data )
#endif
  public:
    acceleration_structure_create_info_t &set_buffer( const std::shared_ptr< buffer_t >& );
    const std::shared_ptr< buffer_t > &get_buffer() const { return buffer; }
  private:
    std::shared_ptr< buffer_t > buffer;
  };
  void to_json( nlohmann::json &root, const acceleration_structure_create_info_t &v );
  void from_json( const nlohmann::json &root, acceleration_structure_create_info_t &v );
#endif
}

#endif

