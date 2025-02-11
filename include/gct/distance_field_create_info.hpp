#ifndef GCT_DISTANCE_FIELD_CREATE_INFO_HPP
#define GCT_DISTANCE_FIELD_CREATE_INFO_HPP

#include <filesystem>
#include <gct/setter.hpp>
#include <gct/allocator_set.hpp>

namespace gct {

struct distance_field_create_info {
  LIBGCT_SETTER( allocator_set )
  LIBGCT_ALLOCATOR_SET_LEGACY_SETTER( allocator_set )
  LIBGCT_SETTER( clear_shader )
  LIBGCT_SETTER( voronoi_shader )
  LIBGCT_SETTER( distance_field_shader )
  LIBGCT_SETTER( size_factor )
  allocator_set_t allocator_set;
  std::filesystem::path clear_shader;
  std::filesystem::path voronoi_shader;
  std::filesystem::path distance_field_shader;
  unsigned int size_factor = 9u;
};

void to_json( nlohmann::json &dest, const distance_field_create_info &src );

}

#endif

