#ifndef GCT_PARTICLE_BUFFER_CREATE_INFO_HPP
#define GCT_PARTICLE_BUFFER_CREATE_INFO_HPP

#include <unordered_map>
#include <cstdint>
#include <filesystem>
#include <gct/setter.hpp>
#include <gct/allocator_set.hpp>
#include <gct/vertex_attributes.hpp>

namespace gct {

struct particle_buffer_create_info {
  LIBGCT_SETTER( allocator_set )
  LIBGCT_ALLOCATOR_SET_LEGACY_SETTER( allocator_set )
  LIBGCT_SETTER( filename )
  LIBGCT_SETTER( vamap )
  LIBGCT_SETTER( stride )
  allocator_set_t allocator_set;
  std::filesystem::path filename;
  std::unordered_map< vertex_attribute_usage_t, vertex_input_detail_t > vamap;
  std::uint32_t stride;
};

void to_json( nlohmann::json &dest, const particle_buffer_create_info &src );

}

#endif



