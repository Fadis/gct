#ifndef GCT_SHADER_SET_HPP
#define GCT_SHADER_SET_HPP

#include <unordered_map>
#include <string>
#include <filesystem>
#include <nlohmann/json_fwd.hpp>

namespace gct {
  using shader_set_t = std::unordered_map< std::string, std::filesystem::path >;
}

void to_json( nlohmann::json &dest, const gct::shader_set_t& );

#define LIBGCT_SHADER_SET( name ) \
  decltype(auto) add_ ## name ( \
    const std::string &n, \
    const std::filesystem::path &p \
  ) { \
    name .insert( std::make_pair( n, p ) ); \
    return *this; \
  } \
  decltype(auto) clear_ ## name () { \
    name .clear(); \
    return *this; \
  }

#endif

