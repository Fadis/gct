#ifndef GCT_SKYVIEW_CREATE_INFO_HPP
#define GCT_SKYVIEW_CREATE_INFO_HPP

#include <vector>
#include <filesystem>
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>
#include <gct/named_resource.hpp>
#include <gct/allocator_set.hpp>

namespace gct {

struct skyview_create_info {
  LIBGCT_SETTER( allocator_set )
  LIBGCT_ALLOCATOR_SET_LEGACY_SETTER( allocator_set )
  LIBGCT_SETTER( transmittance_width )
  LIBGCT_SETTER( transmittance_height )
  LIBGCT_SETTER( multiscat_size )
  LIBGCT_SETTER( skyview_width )
  LIBGCT_SETTER( skyview_height )
  LIBGCT_SETTER( transmittance_shader )
  LIBGCT_SETTER( multiscat_shader )
  LIBGCT_SETTER( skyview_shader )
  LIBGCT_SETTER( resources )
  LIBGCT_NAMED_RESOURCE_SETTER( resources )
  skyview_create_info &set_shader(
    const std::filesystem::path &
  );
  allocator_set_t allocator_set;
  unsigned int transmittance_width = 256u;
  unsigned int transmittance_height = 64u;
  unsigned int multiscat_size = 32u;
  unsigned int skyview_width = 256u;
  unsigned int skyview_height = 128u;
  std::filesystem::path transmittance_shader;
  std::filesystem::path multiscat_shader;
  std::filesystem::path skyview_shader;
  std::vector< named_resource > resources;
};

void to_json( nlohmann::json&, const skyview_create_info& );

}

#endif

