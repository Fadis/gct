#include <gct/device.hpp>
#include <gct/render_pass.hpp>

namespace gct {

  render_pass_t::render_pass_t(
    const std::shared_ptr< device_t > &device,
    const render_pass_create_info_t &create_info
  ) :
    created_from< device_t >( device ),
    props( create_info ) {
    props.rebuild_chain();
    handle = (*device)->createRenderPassUnique(
      props.get_basic()
    );
  }
  std::shared_ptr< framebuffer_t > render_pass_t::get_framebuffer(
    const framebuffer_create_info_t &create_info
  ) {
    return std::shared_ptr< framebuffer_t >(
      new framebuffer_t(
        shared_from_this(),
        create_info
      )
    );
  }
}

