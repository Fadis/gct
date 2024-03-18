#ifndef GCT_MATRIX_POOL_CREATE_INFO_HPP
#define GCT_MATRIX_POOL_CREATE_INFO_HPP

#include <memory>
#include <string>
#include <filesystem>
#include <vector>
#include <cstdint>
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>
#include <gct/named_resource.hpp>

namespace gct {

class allocator_t;
class descriptor_pool_t;
class pipeline_cache_t;
class compute;
class command_buffer_recorder_t;
struct matrix_pool_create_info {
  LIBGCT_SETTER( allocator )
  LIBGCT_SETTER( descriptor_pool )
  LIBGCT_SETTER( pipeline_cache )
  LIBGCT_SETTER( write_shader )
  LIBGCT_SETTER( read_shader )
  LIBGCT_SETTER( update_shader )
  LIBGCT_SETTER( max_matrix_count )
  LIBGCT_SETTER( staging_matrix_buffer_name )
  LIBGCT_SETTER( matrix_buffer_name )
  LIBGCT_SETTER( write_request_buffer_name )
  LIBGCT_SETTER( read_request_buffer_name )
  LIBGCT_SETTER( update_request_buffer_name )
  LIBGCT_SETTER( resources )
  std::shared_ptr< allocator_t > allocator;
  std::shared_ptr< descriptor_pool_t > descriptor_pool;
  std::shared_ptr< pipeline_cache_t > pipeline_cache;
  std::filesystem::path write_shader;
  std::filesystem::path read_shader;
  std::filesystem::path update_shader;
  std::uint32_t max_matrix_count = 65536u;
  std::string staging_matrix_buffer_name = "staging";
  std::string matrix_buffer_name = "matrix";
  std::string write_request_buffer_name = "request";
  std::string read_request_buffer_name = "request";
  std::string update_request_buffer_name = "request";
  std::vector< named_resource > resources;
};

void to_json( nlohmann::json &dest, const matrix_pool_create_info& );

}

#endif

