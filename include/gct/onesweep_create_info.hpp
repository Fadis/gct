#ifndef GCT_ONESWEEP_CREATE_INFO_HPP
#define GCT_ONESWEEP_CREATE_INFO_HPP

#include <memory>
#include <vector>
#include <filesystem>
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>
#include <gct/named_resource.hpp>

namespace gct {

class allocator_t;
class descriptor_pool_t;
class pipeline_cache_t;
struct onesweep_create_info {
  LIBGCT_SETTER( allocator )
  LIBGCT_SETTER( descriptor_pool )
  LIBGCT_SETTER( pipeline_cache )
  LIBGCT_SETTER( histgram_shader )
  LIBGCT_SETTER( histgram_sum_shader )
  LIBGCT_SETTER( local_sum_shader )
  LIBGCT_SETTER( global_sum_shader )
  LIBGCT_SETTER( sort_shader )
  LIBGCT_SETTER( small_sort_shader )
  LIBGCT_SETTER( input )
  LIBGCT_SETTER( input_name )
  LIBGCT_SETTER( output )
  LIBGCT_SETTER( output_name )
  LIBGCT_SETTER( histgram_name )
  LIBGCT_SETTER( local_offset_name )
  LIBGCT_SETTER( workgroup_offset_name )
  LIBGCT_SETTER( resources )
  onesweep_create_info &add_resource(
    const named_resource &n
  );
  onesweep_create_info &clear_resource(
    const named_resource &n
  );
  onesweep_create_info &set_shader(
    const std::filesystem::path &
  );
  std::shared_ptr< allocator_t > allocator;
  std::shared_ptr< descriptor_pool_t > descriptor_pool;
  std::shared_ptr< pipeline_cache_t > pipeline_cache;
  std::filesystem::path histgram_shader;
  std::filesystem::path histgram_sum_shader;
  std::filesystem::path local_sum_shader;
  std::filesystem::path global_sum_shader;
  std::filesystem::path sort_shader;
  std::filesystem::path small_sort_shader;
  std::shared_ptr< buffer_t > input;
  std::string input_name = "input_data";
  std::shared_ptr< buffer_t > output;
  std::string output_name = "output_data";
  std::string histgram_name = "histgram";
  std::string local_offset_name = "local_offset";
  std::string workgroup_offset_name = "workgroup_offset";
  std::vector< named_resource > resources;
};

void to_json( nlohmann::json &dest, const onesweep_create_info &src );

}

#endif

