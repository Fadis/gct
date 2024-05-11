#ifndef GCT_CONDITIONAL_RENDERING_BEGIN_INFO_HPP
#define GCT_CONDITIONAL_RENDERING_BEGIN_INFO_HPP
#include <nlohmann/json_fwd.hpp>
#include <vulkan/vulkan.hpp>
#include <gct/extension.hpp>

#ifdef VK_EXT_CONDITIONAL_RENDERING_EXTENSION_NAME
namespace gct {
  class buffer_t;
  class conditional_rendering_begin_info_t : public chained_t {
  public:
    using self_type = conditional_rendering_begin_info_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
    LIBGCT_BASIC_SETTER( vk::ConditionalRenderingBeginInfoEXT )
    conditional_rendering_begin_info_t &set_buffer( const std::shared_ptr< buffer_t > &b ) {
      buf = b;
      return *this;
    }
    const std::shared_ptr< buffer_t > &get_buffer() const {
      return buf;
    }
  private:
    std::shared_ptr< buffer_t > buf;
  };
  void to_json( nlohmann::json &root, const conditional_rendering_begin_info_t &v );
  void from_json( const nlohmann::json &root, conditional_rendering_begin_info_t &v );
}
#endif

#endif


