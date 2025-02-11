#ifndef GCT_TONE_MAPPING_CREATE_INFO_HPP
#define GCT_TONE_MAPPING_CREATE_INFO_HPP

#include <vector>
#include <filesystem>
#include <gct/setter.hpp>
#include <gct/named_resource.hpp>
#include <gct/allocator_set.hpp>

namespace gct {

class image_view_t;
struct tone_mapping_create_info {
  LIBGCT_SETTER( allocator_set )
  LIBGCT_ALLOCATOR_SET_LEGACY_SETTER( allocator_set )
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
  allocator_set_t allocator_set;
  std::filesystem::path shader;
  std::vector< std::shared_ptr< image_view_t > > input;
  std::string input_name = "src_image";
  std::string output_name = "tone";
  std::vector< named_resource > resources;
};
void to_json( nlohmann::json&, const tone_mapping_create_info& );

}

#endif

