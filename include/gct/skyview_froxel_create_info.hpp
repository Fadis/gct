#ifndef GCT_SKYVIEW_FROXEL_CREATE_INFO_HPP
#define GCT_SKYVIEW_FROXEL_CREATE_INFO_HPP

#include <cstdint>
#include <memory>
#include <vector>
#include <filesystem>
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>
#include <gct/named_resource.hpp>
#include <gct/allocator_set.hpp>

namespace gct {

struct skyview_froxel_create_info {
  LIBGCT_SETTER( allocator_set )
  LIBGCT_ALLOCATOR_SET_LEGACY_SETTER( allocator_set )
  LIBGCT_SETTER( froxel_xy_resolution )
  LIBGCT_SETTER( froxel_z_resolution )
  LIBGCT_SETTER( width )
  LIBGCT_SETTER( height )
  LIBGCT_SETTER( generate_shader )
  LIBGCT_SETTER( render_shader )
  LIBGCT_SETTER( gbuffer )
  LIBGCT_SETTER( transmittance )
  LIBGCT_SETTER( multiscat )
  LIBGCT_SETTER( gbuffer_name )
  LIBGCT_SETTER( transmittance_name )
  LIBGCT_SETTER( multiscat_name )
  LIBGCT_SETTER( froxel_name )
  LIBGCT_SETTER( froxel_state_name )
  LIBGCT_SETTER( output_name )
  LIBGCT_SETTER( resources )
  LIBGCT_NAMED_RESOURCE_SETTER( resources )
  skyview_froxel_create_info &set_shader(
    const std::filesystem::path &
  );
  allocator_set_t allocator_set;
  std::uint32_t froxel_xy_resolution = 32u;
  std::uint32_t froxel_z_resolution = 32u;
  std::uint32_t width = 1920u;
  std::uint32_t height = 1080u;
  std::filesystem::path generate_shader;
  std::filesystem::path render_shader;
  std::shared_ptr< image_view_t > gbuffer;
  std::shared_ptr< image_view_t > transmittance;
  std::shared_ptr< image_view_t > multiscat;
  std::string gbuffer_name = "gbuffer";
  std::string transmittance_name = "transmittance";
  std::string multiscat_name = "multiscattering";
  std::string froxel_name = "scattering";
  std::string froxel_state_name = "froxel_state";
  std::string output_name = "dest_image";
  std::vector< named_resource > resources;
};

void to_json( nlohmann::json&, const skyview_froxel_create_info& );

}

#endif


