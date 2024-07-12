#ifndef GCT_LIGHT_POOL_CREATE_INFO_HPP
#define GCT_LIGHT_POOL_CREATE_INFO_HPP
#include <cstdint>
#include <memory>
#include <filesystem>
#include <vector>
#include <string>
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>
#include <gct/named_resource.hpp>

namespace gct {

class buffer_t;
class allocator_t;
class descriptor_pool_t;
class pipeline_cache_t;
class compute;
class command_buffer_recorder_t;
struct light_pool_create_info {
  LIBGCT_SETTER( allocator )
  LIBGCT_SETTER( descriptor_pool )
  LIBGCT_SETTER( pipeline_cache )
  LIBGCT_SETTER( matrix_pool )
  LIBGCT_SETTER( write_shader )
  LIBGCT_SETTER( read_shader )
  LIBGCT_SETTER( update_shader )
  LIBGCT_SETTER( max_light_count )
  LIBGCT_SETTER( staging_light_buffer_name )
  LIBGCT_SETTER( light_buffer_name )
  LIBGCT_SETTER( active_light_buffer_name )
  LIBGCT_SETTER( matrix_buffer_name )
  LIBGCT_SETTER( write_request_buffer_name )
  LIBGCT_SETTER( read_request_buffer_name )
  LIBGCT_SETTER( update_request_buffer_name )
  LIBGCT_SETTER( resources )
  light_pool_create_info &add_resource(
    const named_resource &n
  );
  light_pool_create_info &clear_resource();
  light_pool_create_info &set_shader(
    const std::filesystem::path &
  );
  std::shared_ptr< allocator_t > allocator;
  std::shared_ptr< descriptor_pool_t > descriptor_pool;
  std::shared_ptr< pipeline_cache_t > pipeline_cache;
  std::shared_ptr< buffer_t > matrix_pool;
  std::filesystem::path write_shader;
  std::filesystem::path read_shader;
  std::filesystem::path update_shader;
  std::uint32_t max_light_count = 65536u;
  std::string staging_light_buffer_name = "staging";
  std::string light_buffer_name = "light";
  std::string active_light_buffer_name = "active_light";
  std::string matrix_buffer_name = "matrix";
  std::string write_request_buffer_name = "request";
  std::string read_request_buffer_name = "request";
  std::string update_request_buffer_name = "request";
  std::vector< named_resource > resources;
};
void to_json( nlohmann::json &dest, const light_pool_create_info &src );

}

#endif

