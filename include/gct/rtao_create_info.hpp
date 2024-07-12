#ifndef GCT_RTAO_CREATE_INFO_HPP
#define GCT_RTAO_CREATE_INFO_HPP

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
struct rtao_create_info {
  LIBGCT_SETTER( allocator )
  LIBGCT_SETTER( descriptor_pool )
  LIBGCT_SETTER( pipeline_cache )
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
  std::shared_ptr< allocator_t > allocator;
  std::shared_ptr< descriptor_pool_t > descriptor_pool;
  std::shared_ptr< pipeline_cache_t > pipeline_cache;
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

}

#endif

