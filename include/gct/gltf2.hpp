#ifndef GCT_GLTF2_HPP
#define GCT_GLTF2_HPP

#include <memory>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnarrowing"
#include <fx/gltf.h>
#pragma GCC diagnostic pop
#include <gct/setter.hpp>
#include <gct/mappable_buffer.hpp>
#include <gct/shader_flag.hpp>
#include <gct/gltf2_create_info.hpp>

namespace gct::gltf {

class descriptor_set_layout_t;
class gltf2 {
  struct buffer_state {
    LIBGCT_SETTER( filename )
    LIBGCT_SETTER( buffer )
    std::filesystem::path filename;
    std::shared_ptr< mappable_buffer_t > buffer;
  };
  gltf2(
    const gltf2_create_info &ci
  );
  void load_buffer(
    const fx::gltf::Document &doc
  );
  std::unordered_map< shader_flag_t, std::shared_ptr< shader_module_t > > load_shader(
    const fx::gltf::Document &doc,
    const std::filesystem::path &shader_dir
  );
  void load_sampler(
    const fx::gltf::Document &doc
  );
private:
  gltf2_create_info props;
  std::vector< buffer_state > buffer;
  std::unordered_map< shader_flag_t, std::shared_ptr< shader_module_t > > shader;
  std::vector< std::shared_ptr< sampler_t > > sampler;
  std::filesystem::path cd;
};

}

#endif


