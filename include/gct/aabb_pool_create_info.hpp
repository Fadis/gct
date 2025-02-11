#ifndef GCT_AABB_POOL_CREATE_INFO_HPP
#define GCT_AABB_POOL_CREATE_INFO_HPP
#include <cstdint>
#include <memory>
#include <filesystem>
#include <vector>
#include <string>
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>
#include <gct/named_resource.hpp>
#include <gct/allocator_set.hpp>

namespace gct {

class buffer_t;
class compute;
class command_buffer_recorder_t;
struct aabb_pool_create_info {
  LIBGCT_SETTER( allocator_set )
  LIBGCT_ALLOCATOR_SET_LEGACY_SETTER( allocator_set )
  LIBGCT_SETTER( matrix_pool )
  LIBGCT_SETTER( write_shader )
  LIBGCT_SETTER( read_shader )
  LIBGCT_SETTER( update_shader )
  LIBGCT_SETTER( max_aabb_count )
  LIBGCT_SETTER( staging_aabb_buffer_name )
  LIBGCT_SETTER( aabb_buffer_name )
  LIBGCT_SETTER( matrix_buffer_name )
  LIBGCT_SETTER( write_request_buffer_name )
  LIBGCT_SETTER( read_request_buffer_name )
  LIBGCT_SETTER( update_request_buffer_name )
  LIBGCT_SETTER( resources )
  aabb_pool_create_info &add_resource(
    const named_resource &n
  );
  aabb_pool_create_info &clear_resource();
  aabb_pool_create_info &set_shader(
    const std::filesystem::path &
  );
  allocator_set_t allocator_set;
  std::shared_ptr< buffer_t > matrix_pool;
  std::filesystem::path write_shader;
  std::filesystem::path read_shader;
  std::filesystem::path update_shader;
  std::uint32_t max_aabb_count = 65536u;
  std::string staging_aabb_buffer_name = "staging";
  std::string aabb_buffer_name = "aabb";
  std::string matrix_buffer_name = "matrix";
  std::string write_request_buffer_name = "request";
  std::string read_request_buffer_name = "request";
  std::string update_request_buffer_name = "request";
  std::vector< named_resource > resources;
};
void to_json( nlohmann::json &dest, const aabb_pool_create_info &src );

}

#endif

