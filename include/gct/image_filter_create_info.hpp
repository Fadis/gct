#ifndef GCT_IMAGE_FILTER_CREATE_INFO_HPP
#define GCT_IMAGE_FILTER_CREATE_INFO_HPP

#include <memory>
#include <vector>
#include <optional>
#include <filesystem>
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>
#include <gct/named_resource.hpp>
#include <gct/image_create_info.hpp>
#include <gct/allocator_set.hpp>

namespace gct {

class image_view_t;
struct image_filter_create_info {
  LIBGCT_SETTER( allocator_set )
  LIBGCT_ALLOCATOR_SET_LEGACY_SETTER( allocator_set )
  LIBGCT_SETTER( shader )
  LIBGCT_SETTER( input )
  LIBGCT_SETTER( input_name )
  LIBGCT_SETTER( output_create_info )
  LIBGCT_SETTER( output )
  LIBGCT_SETTER( output_name )
  LIBGCT_SETTER( resources )
  LIBGCT_NAMED_RESOURCE_SETTER( resources )
  LIBGCT_SETTER( reusable )
  allocator_set_t allocator_set;
  std::filesystem::path shader;
  std::vector< std::shared_ptr< image_view_t > > input;
  std::string input_name = "src_image";
  std::optional< image_create_info_t > output_create_info;
  std::vector< std::shared_ptr< image_view_t > > output;
  std::string output_name = "dest_image";
  std::vector< named_resource > resources;
  bool reusable = false;
};

void to_json( nlohmann::json&, const image_filter_create_info& );

}

#endif

