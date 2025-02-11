#ifndef GCT_RADIX_SORT_CREATE_INFO_HPP
#define GCT_RADIX_SORT_CREATE_INFO_HPP

#include <memory>
#include <vector>
#include <filesystem>
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>
#include <gct/named_resource.hpp>
#include <gct/allocator_set.hpp>

namespace gct {

struct radix_sort_create_info {
  LIBGCT_SETTER( allocator_set )
  LIBGCT_ALLOCATOR_SET_LEGACY_SETTER( allocator_set )
  LIBGCT_SETTER( local_sum_shader )
  LIBGCT_SETTER( global_sum_shader )
  LIBGCT_SETTER( sort_shader )
  LIBGCT_SETTER( small_sort_shader )
  LIBGCT_SETTER( input )
  LIBGCT_SETTER( input_name )
  LIBGCT_SETTER( output )
  LIBGCT_SETTER( output_name )
  LIBGCT_SETTER( local_offset_name )
  LIBGCT_SETTER( workgroup_offset_name )
  LIBGCT_SETTER( resources )
  LIBGCT_NAMED_RESOURCE_SETTER( resources )
  radix_sort_create_info &set_shader(
    const std::filesystem::path &
  );
  allocator_set_t allocator_set;
  std::filesystem::path local_sum_shader;
  std::filesystem::path global_sum_shader;
  std::filesystem::path sort_shader;
  std::filesystem::path small_sort_shader;
  std::shared_ptr< buffer_t > input;
  std::string input_name = "input_data";
  std::shared_ptr< buffer_t > output;
  std::string output_name = "output_data";
  std::string local_offset_name = "local_offset";
  std::string workgroup_offset_name = "workgroup_offset";
  std::vector< named_resource > resources;
};

void to_json( nlohmann::json&, const radix_sort_create_info& );

}

#endif

