#include <gct/device.hpp>
#include <gct/exception.hpp>
#include <gct/command_pool.hpp>
#include <gct/command_buffer.hpp>
#include <gct/command_buffer_recorder.hpp>

namespace gct {
  void command_buffer_recorder_t::dispatch_threads(
    std::uint32_t x,
    std::uint32_t y,
    std::uint32_t z
  ) {
    if( !local_size_is_available ) {
      throw exception::invalid_argument( "command_buffer_recorder_t::dispatch_threads : Unable to detect local size.", __FILE__, __LINE__ );
    }
    (*get_factory())->dispatch(
      x / local_size[ 0 ] + ( x % local_size[ 0 ] ? 1u : 0u ),
      y / local_size[ 1 ] + ( y % local_size[ 1 ] ? 1u : 0u ),
      z / local_size[ 2 ] + ( z % local_size[ 2 ] ? 1u : 0u )
    );
  }
}

