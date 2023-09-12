#ifndef GCT_HBAO_CREATE_INFO_HPP
#define GCT_HBAO_CREATE_INFO_HPP

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
class image_view_t;
struct hbao_create_info {
  LIBGCT_SETTER( allocator )
  LIBGCT_SETTER( descriptor_pool )
  LIBGCT_SETTER( pipeline_cache )
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
  hbao_create_info &add_resource(
    const named_resource &n
  );
  hbao_create_info &clear_resource(
    const named_resource &n
  );
  hbao_create_info &set_shader(
    const std::filesystem::path &
  );
  std::shared_ptr< allocator_t > allocator;
  std::shared_ptr< descriptor_pool_t > descriptor_pool;
  std::shared_ptr< pipeline_cache_t > pipeline_cache;
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

}

#endif

