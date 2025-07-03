#ifndef GCT_BUFFER_POOL_CREATE_INFO_HPP
#define GCT_BUFFER_POOL_CREATE_INFO_HPP

#include <string>
#include <filesystem>
#include <vector>
#include <cstdint>
#include <nlohmann/json_fwd.hpp>
#include <gct/alignment.hpp>
#include <gct/setter.hpp>
#include <gct/named_resource.hpp>
#include <gct/allocator_set.hpp>
namespace gct {

class compute;
class command_buffer_recorder_t;
class descriptor_set_t;
struct buffer_pool_create_info {
  LIBGCT_SETTER( allocator_set )
  LIBGCT_ALLOCATOR_SET_LEGACY_SETTER( allocator_set )
  LIBGCT_SETTER( write_shader )
  LIBGCT_SETTER( read_shader )
  LIBGCT_SETTER( max_buffer_count )
  LIBGCT_SETTER( max_request_count )
  LIBGCT_SETTER( staging_buffer_name )
  LIBGCT_SETTER( buffer_name )
  LIBGCT_SETTER( write_request_buffer_name )
  LIBGCT_SETTER( read_request_buffer_name )
  LIBGCT_SETTER( resources )
  LIBGCT_NAMED_RESOURCE_SETTER( resources )
  buffer_pool_create_info &set_shader(
    const std::filesystem::path &
  );
  bool shader_exists() const;
  allocator_set_t allocator_set;
  std::filesystem::path write_shader;
  std::filesystem::path read_shader;
  std::uint32_t max_buffer_count = 65536u;
  std::uint32_t max_request_count = 65536u;
  std::string staging_buffer_name = "staging";
  std::string buffer_name;
  std::string write_request_buffer_name = "request";
  std::string read_request_buffer_name = "request";
  memory_layout layout = memory_layout::std430;
  std::vector< named_resource > resources;
};

void to_json( nlohmann::json &dest, const buffer_pool_create_info& );

}

#endif

