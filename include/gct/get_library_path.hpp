#ifndef GCT_GET_LIBRARY_PATH_HPP
#define GCT_GET_LIBRARY_PATH_HPP

#include <filesystem>
#include <optional>

namespace gct {

std::optional< std::filesystem::path > get_library_path();
std::filesystem::path get_system_shader_path();

}

#endif

