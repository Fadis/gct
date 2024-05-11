#include <gct/command_buffer_recorder.hpp>
#include <gct/conditional_rendering_begin_info.hpp>
#include <gct/command_buffer.hpp>

#ifdef VK_EXT_CONDITIONAL_RENDERING_EXTENSION_NAME
namespace gct {
  std::shared_ptr< void > command_buffer_recorder_t::begin(
    const conditional_rendering_begin_info_t &bi
  ) {
    auto temp = bi;
    temp.rebuild_chain();
    (*get_factory())->beginConditionalRenderingEXT(
      temp.get_basic()
    );
    get_factory()->unbound()->keep.push_back( temp );
    return std::shared_ptr< void >(
      nullptr,
      [command_buffer=get_factory()]( void* ) {
        (*command_buffer)->endConditionalRenderingEXT();
      }
    );
  }
}
#endif

