#include <nlohmann/json.hpp>
#include <gct/image_io.hpp>
#include <gct/compute.hpp>
#include <gct/graphics.hpp>
#include <gct/compute_pipeline.hpp>
#include <gct/graphics_pipeline.hpp>
#include <gct/command_buffer_recorder.hpp>
namespace gct {

image_io::image_io(
  image_io_create_info &ci
) :
  property_type( ci ) {
  if( !get_props().filled() ) {
    throw exception::invalid_argument( "image_io::image_io : At least one image is not attached", __FILE__, __LINE__ );
  }
}    
void image_io::operator()(
  command_buffer_recorder_t &rec
) const {
  for( std::uint32_t cycle = 0u; cycle != get_props().get_plan().loop; ++cycle ) {
    if( get_props().get_push_constant_member_pointer()->has( "loop_counter" ) ) {
      get_props().set_push_constant( "loop_counter", cycle );
    }
#if defined(VK_VERSION_1_3) || defined(VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME)
    if( get_props().get_graphic_executable() ) {
      get_props()( rec, get_props().get_graphic_executable()->get_pipeline() );
      {
        const_cast< rendering_info_t& >( get_props().get_rendering_info() ).rebuild_chain();
        const auto rendering = rec.begin_rendering( get_props().get_rendering_info() );
        const auto dim = get_props().get_dim();
        rec->setViewport(
          0u,
          {
            vk::Viewport()
              .setWidth( get_props().get_rendering_info().get_basic().renderArea.extent.width )
              .setHeight( get_props().get_rendering_info().get_basic().renderArea.extent.height )
              .setMinDepth( 0.0f )
              .setMaxDepth( 1.0f )
          }
        );
        rec->setScissor(
          0u,
          {
            vk::Rect2D(
              vk::Offset2D(0, 0),
              vk::Extent2D()
                .setWidth( get_props().get_rendering_info().get_basic().renderArea.extent.width )
                .setHeight( get_props().get_rendering_info().get_basic().renderArea.extent.height )
            )
          }
        );
        std::cout << "graphics : " << dim.x << " " << dim.y << " " << dim.z << std::endl;
        (*get_props().get_graphic_executable())( rec, 0u, dim.x, dim.y, dim.z );
      }
    }
    else {
#endif
      get_props()( rec, get_props().get_executable()->get_pipeline() );
      const auto dim = get_props().get_dim();
      (*get_props().get_executable())( rec, 0u, dim.x, dim.y, dim.z );
#if defined(VK_VERSION_1_3) || defined(VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME)
    }
#endif
    if( cycle + 1u != get_props().get_plan().loop ) {
      // need sync
    }
  }
}

void to_json( nlohmann::json &dest, const image_io &src ) {
  dest = nlohmann::json::object();
  dest[ "props" ] = src.get_props();
}

}

