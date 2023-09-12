#ifndef GCT_TONE_MAPPING_CREATE_INFO_HPP
#define GCT_TONE_MAPPING_CREATE_INFO_HPP

#include <memory>
#include <vector>
#include <filesystem>
#include <gct/setter.hpp>
#include <gct/named_resource.hpp>

namespace gct {

class allocator_t;
class descriptor_pool_t;
class pipeline_cache_t;
class image_view_t;
struct tone_mapping_create_info {
  LIBGCT_SETTER( allocator )
  LIBGCT_SETTER( descriptor_pool )
  LIBGCT_SETTER( pipeline_cache )
  LIBGCT_SETTER( shader )
  LIBGCT_SETTER( input )
  LIBGCT_SETTER( input_name )
  LIBGCT_SETTER( resources )
  tone_mapping_create_info &add_resource(
    const named_resource &n
  );
  tone_mapping_create_info &clear_resource(
    const named_resource &n
  );
  std::shared_ptr< allocator_t > allocator;
  std::shared_ptr< descriptor_pool_t > descriptor_pool;
  std::shared_ptr< pipeline_cache_t > pipeline_cache;
  std::filesystem::path shader;
  std::vector< std::shared_ptr< image_view_t > > input;
  std::string input_name = "src_image";
  std::string output_name = "tone";
  std::vector< named_resource > resources;
};

}

#endif

