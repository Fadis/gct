#ifndef GCT_RTAO_CREATE_INFO_HPP
#define GCT_RTAO_CREATE_INFO_HPP

#include <memory>
#include <vector>
#include <filesystem>
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>
#include <gct/named_resource.hpp>
#include <gct/allocator_set.hpp>

namespace gct {

class image_view_t;
struct rtao_create_info {
  LIBGCT_SETTER( allocator_set )
  LIBGCT_ALLOCATOR_SET_LEGACY_SETTER( allocator_set )
  LIBGCT_SETTER( hgauss_shader )
  LIBGCT_SETTER( vgauss_shader )
  LIBGCT_SETTER( rtao_shader )
  LIBGCT_SETTER( output )
  LIBGCT_SETTER( previous_ao_name )
  LIBGCT_SETTER( previous_history_name )
  LIBGCT_SETTER( history_name )
  LIBGCT_SETTER( temporary_name )
  LIBGCT_SETTER( ao_name )
  LIBGCT_SETTER( resources )
  rtao_create_info &add_resource(
    const named_resource &n
  );
  rtao_create_info &clear_resource(
    const named_resource &n
  );
  rtao_create_info &set_shader(
    const std::filesystem::path &
  );
  allocator_set_t allocator_set;
  std::filesystem::path hgauss_shader;
  std::filesystem::path vgauss_shader;
  std::filesystem::path rtao_shader;
  std::string previous_ao_name = "previous_ao";
  std::string previous_history_name = "previous_history";
  std::string history_name = "history";
  std::string temporary_name = "temporary";
  std::string ao_name = "ao";
  std::shared_ptr< image_view_t > output;
  std::vector< named_resource > resources;
};

void to_json( nlohmann::json &dest, const rtao_create_info &src );

}

#endif

