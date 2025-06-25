#include <gct/get_library_path.hpp>
#include <dlfcn.h>

namespace gct {

std::optional< std::filesystem::path > get_library_path() {
  Dl_info info;
  const int result = dladdr( reinterpret_cast< void* >( &get_library_path ), &info );
  if( result == 0 ) return std::nullopt;
  else return std::filesystem::path( info.dli_fname );
}

}
