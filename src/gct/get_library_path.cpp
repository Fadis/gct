#include <gct/get_library_path.hpp>
#include <gct/exception.hpp>
#include <dlfcn.h>

namespace gct {

std::optional< std::filesystem::path > get_library_path() {
  Dl_info info;
  const int result = dladdr( reinterpret_cast< void* >( &get_library_path ), &info );
  if( result == 0 ) return std::nullopt;
  else return std::filesystem::path( info.dli_fname );
}
std::filesystem::path get_system_shader_path() {
  const auto library_path = get_library_path(); 
  std::filesystem::path shader_path; 
  if( library_path ) { 
    shader_path = library_path->parent_path() / "gct" / "shader"; 
  } 
  else { 
    throw gct::exception::runtime_error( "get_library_path failed", __FILE__, __LINE__ ); 
  } 
  return shader_path;
}

}
