#ifndef GCT_MATRIX_POOL_CREATE_INFO_HPP
#define GCT_MATRIX_POOL_CREATE_INFO_HPP

#include <string>
#include <filesystem>
#include <vector>
#include <cstdint>
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>
#include <gct/named_resource.hpp>
#include <gct/allocator_set.hpp>

namespace gct {

class compute;
class command_buffer_recorder_t;
class descriptor_set_t;
struct matrix_pool_create_info {
  LIBGCT_SETTER( allocator_set )
  LIBGCT_ALLOCATOR_SET_LEGACY_SETTER( allocator_set )
  LIBGCT_SETTER( write_shader )
  LIBGCT_SETTER( read_shader )
  LIBGCT_SETTER( update_shader )
  LIBGCT_SETTER( copy_shader )
  LIBGCT_SETTER( inverse_shader )
  LIBGCT_SETTER( max_matrix_count )
  LIBGCT_SETTER( staging_matrix_buffer_name )
  LIBGCT_SETTER( matrix_buffer_name )
  LIBGCT_SETTER( write_request_buffer_name )
  LIBGCT_SETTER( read_request_buffer_name )
  LIBGCT_SETTER( update_request_buffer_name )
  LIBGCT_SETTER( copy_request_buffer_name )
  LIBGCT_SETTER( inverse_request_buffer_name )
  LIBGCT_SETTER( resources )
  LIBGCT_NAMED_RESOURCE_SETTER( resources )
  matrix_pool_create_info &set_shader(
    const std::filesystem::path &
  );
  bool shader_exists() const;
  allocator_set_t allocator_set;
  std::filesystem::path write_shader;
  std::filesystem::path read_shader;
  std::filesystem::path update_shader;
  std::filesystem::path copy_shader;
  std::filesystem::path inverse_shader;
  std::uint32_t max_matrix_count = 65536u;
  std::string staging_matrix_buffer_name = "staging";
  std::string matrix_buffer_name = "matrix";
  std::string write_request_buffer_name = "request";
  std::string read_request_buffer_name = "request";
  std::string update_request_buffer_name = "request";
  std::string copy_request_buffer_name = "request";
  std::string inverse_request_buffer_name = "request";
  std::vector< named_resource > resources;
};

void to_json( nlohmann::json &dest, const matrix_pool_create_info& );

}

#endif

