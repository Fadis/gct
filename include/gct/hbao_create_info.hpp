#ifndef GCT_HBAO_CREATE_INFO_HPP
#define GCT_HBAO_CREATE_INFO_HPP

#include <memory>
#include <vector>
#include <filesystem>
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>
#include <gct/named_resource.hpp>
#include <gct/allocator_set.hpp>

namespace gct {

class image_view_t;
struct hbao_create_info {
  LIBGCT_SETTER( allocator_set )
  LIBGCT_ALLOCATOR_SET_LEGACY_SETTER( allocator_set )
  LIBGCT_SETTER( ao0_shader )
  LIBGCT_SETTER( ao45_shader )
  LIBGCT_SETTER( ao90_shader )
  LIBGCT_SETTER( ao135_shader )
  LIBGCT_SETTER( sum_shader )
  LIBGCT_SETTER( input )
  LIBGCT_SETTER( input_name )
  LIBGCT_SETTER( output )
  LIBGCT_SETTER( output_name )
  LIBGCT_SETTER( temp )
  LIBGCT_SETTER( temp_name )
  LIBGCT_SETTER( resources )
  LIBGCT_NAMED_RESOURCE_SETTER( resources )
  hbao_create_info &set_shader(
    const std::filesystem::path &
  );
  allocator_set_t allocator_set;
  std::filesystem::path ao0_shader;
  std::filesystem::path ao45_shader;
  std::filesystem::path ao90_shader;
  std::filesystem::path ao135_shader;
  std::filesystem::path sum_shader;
  std::vector< std::shared_ptr< image_view_t > > input;
  std::string input_name = "src_image";
  std::vector< std::shared_ptr< image_view_t > > output;
  std::string output_name = "dest_image";
  std::vector< std::shared_ptr< image_view_t > > temp;
  std::string temp_name = "temp_image";
  std::vector< named_resource > resources;
};

void to_json( nlohmann::json&, const hbao_create_info& );

}

#endif

