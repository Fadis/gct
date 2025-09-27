#include <functional>
#include <gct/command_buffer_recorder.hpp>
#include <gct/rendering_info.hpp>
#include <gct/command_buffer.hpp>
#if defined(VK_VERSION_1_3) || defined(VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME)
namespace gct {
  std::shared_ptr< void > command_buffer_recorder_t::begin_rendering(
    const rendering_info_t &ri
  ) {
    if( ri.is_chained() ) {
#ifdef VK_VERSION_1_3
      (*get_factory())->beginRendering(
#else
      (*get_factory())->beginRenderingKHR(
#endif
        ri.get_basic()
      );
      return std::shared_ptr< void >(
        nullptr,
        [command_buffer=get_factory()]( void* ) {
#ifdef VK_VERSION_1_3
          (*command_buffer)->endRendering();
#else
          (*command_buffer)->endRenderingKHR();
#endif
        }
      );
    }
    else {
      auto temp = ri;
      temp.rebuild_chain();
#ifdef VK_VERSION_1_3
      (*get_factory())->beginRendering(
#else
      (*get_factory())->beginRenderingKHR(
#endif
        temp.get_basic()
      );
      return std::shared_ptr< void >(
        nullptr,
        [command_buffer=get_factory()]( void* ) {
#ifdef VK_VERSION_1_3
          (*command_buffer)->endRendering();
#else
          (*command_buffer)->endRenderingKHR();
#endif
        }
      );
    }
  }
}
#endif

