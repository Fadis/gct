#ifndef GCT_VALIDATION_CACHE_CREATE_INFO_HPP
#define GCT_VALIDATION_CACHE_CREATE_INFO_HPP
#include <cstdint>
#include <string>
#include <memory>
#include <vector>
#include <nlohmann/json_fwd.hpp>
#include <vulkan/vulkan.hpp>
#include <gct/extension.hpp>

#ifdef VK_EXT_VALIDATION_CACHE_EXTENSION_NAME
namespace gct {
  class validation_cache_create_info_t : public chained_t {
  public:
    using self_type = validation_cache_create_info_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
    LIBGCT_BASIC_SETTER( vk::ValidationCacheCreateInfoEXT )
  private:
    std::vector< std::uint8_t > data;
  public:
    validation_cache_create_info_t &load( const std::string& );
    validation_cache_create_info_t &load_from_memory( const std::vector< std::uint8_t >& );
    [[nodiscard]] const std::vector< std::uint8_t > &get_data() const { return data; }
  };
  void to_json( nlohmann::json &root, const validation_cache_create_info_t &v );
  void from_json( const nlohmann::json &root, validation_cache_create_info_t &v );
}
#endif

#endif


