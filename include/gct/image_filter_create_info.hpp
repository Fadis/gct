#ifndef GCT_IMAGE_FILTER_CREATE_INFO_HPP
#define GCT_IMAGE_FILTER_CREATE_INFO_HPP

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
struct image_filter_create_info {
  LIBGCT_SETTER( allocator )
  LIBGCT_SETTER( descriptor_pool )
  LIBGCT_SETTER( pipeline_cache )
  LIBGCT_SETTER( shader )
  LIBGCT_SETTER( input )
  LIBGCT_SETTER( input_name )
  LIBGCT_SETTER( output )
  LIBGCT_SETTER( output_name )
  LIBGCT_SETTER( resources )
  LIBGCT_SETTER( reusable )
  image_filter_create_info &add_resource(
    const named_resource &n
  );
  image_filter_create_info &clear_resource(
    const named_resource &n
  );
  std::shared_ptr< allocator_t > allocator;
  std::shared_ptr< descriptor_pool_t > descriptor_pool;
  std::shared_ptr< pipeline_cache_t > pipeline_cache;
  std::filesystem::path shader;
  std::vector< std::shared_ptr< image_view_t > > input;
  std::string input_name = "src_image";
  std::vector< std::shared_ptr< image_view_t > > output;
  std::string output_name = "dest_image";
  std::vector< named_resource > resources;
  bool reusable = false;
};

}

#endif

