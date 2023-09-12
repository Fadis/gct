#include <gct/allocator.hpp>
#include <gct/render_pass.hpp>
#include <gct/device.hpp>
#include <gct/framebuffer_create_info.hpp>
#include <gct/framebuffer.hpp>

namespace gct {
  framebuffer_t::framebuffer_t(
    const std::shared_ptr< render_pass_t > &render_pass,
    const framebuffer_create_info_t &create_info
  ) :
    created_from< render_pass_t >( render_pass ),
    props( create_info ) {
    auto basic = props.get_basic();
    basic
      .setWidth( props.get_width() )
      .setHeight( props.get_height() )
      .setRenderPass( **render_pass );
    props
      .set_basic( std::move( basic ) )
      .rebuild_chain();
    handle = (*render_pass->get_factory())->createFramebufferUnique( props.get_basic() );
  }
}

