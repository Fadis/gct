#ifndef GCT_SHADER_MODULE_CREATE_INFO_HPP
#define GCT_SHADER_MODULE_CREATE_INFO_HPP
#include <memory>
#include <optional>
#include <nlohmann/json_fwd.hpp>
#include <vulkan/vulkan.hpp>
#include <gct/extension.hpp>

namespace gct {
  class shader_module_create_info_t : public chained_t {
  public:
    using self_type = shader_module_create_info_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
    LIBGCT_BASIC_SETTER( vk::ShaderModuleCreateInfo )
#ifdef VK_EXT_VALIDATION_CACHE_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::ShaderModuleValidationCacheCreateInfoEXT , validation_cache )
#endif
  private:
    std::vector< std::uint8_t > code;
  public:
    shader_module_create_info_t &load( const std::string& );
  };
  void to_json( nlohmann::json &root, const shader_module_create_info_t &v );
  void from_json( const nlohmann::json &root, shader_module_create_info_t &v );
}

#endif

