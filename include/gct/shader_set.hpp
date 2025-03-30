#ifndef GCT_SHADER_SET_HPP
#define GCT_SHADER_SET_HPP

#include <unordered_map>
#include <string>
#include <filesystem>
#include <nlohmann/json_fwd.hpp>

namespace gct {
  using shader_set_t = std::unordered_map< std::string, std::filesystem::path >;
  using rename_map_t = std::unordered_map< std::string, std::string >;
}

void to_json( nlohmann::json &dest, const gct::shader_set_t& );

#define LIBGCT_SHADER_SET( name ) \
  decltype(auto) add_shader( \
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

#define LIBGCT_RENAME_MAP( name ) \
  decltype(auto) add_shader( \
    const std::string &n, \
    const std::string &p \
  ) { \
    name .insert( std::make_pair( n, p ) ); \
    return *this; \
  } \
  decltype(auto) clear_ ## name () { \
    name .clear(); \
    return *this; \
  }
  
#define LIBGCT_SHADER_SET_VALIDATE( name, location, value_name, shader_name ) \
  std::filesystem::path value_name ; \
  { \
    const auto temp = props. name .find( shader_name ); \
    if( temp == props. name .end() ) { \
      throw exception::invalid_argument( std::string( location ) + " : required shader \"" + ( shader_name ) + "\" is not set", __FILE__, __LINE__ ); \
    } \
    value_name = temp->second; \
  }

#define LIBGCT_SHADER_SET_RENAME( name, original_name, value_name ) \
  std::string value_name = original_name; \
  { \
    const auto temp = props. name .find( original_name ); \
    if( temp != props. name .end() ) { \
      value_name = temp->second; \
    } \
  }

#endif

