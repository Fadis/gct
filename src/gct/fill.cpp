#include <gct/buffer.hpp>
#include <gct/image.hpp>
#include <gct/image_create_info.hpp>
#include <gct/image_layout.hpp>
#include <gct/mappable_buffer.hpp>
#include <gct/format.hpp>
#include <gct/device.hpp>
#include <gct/command_buffer.hpp>
#include <gct/command_buffer_recorder.hpp>

namespace gct {
void command_buffer_recorder_t::fill(
  const std::shared_ptr< buffer_t > &dest,
  std::uint32_t value
) {
  (*get_factory())->fillBuffer(
    **dest,
    0u,
    dest->get_props().get_basic().size,
    value
  );
  get_factory()->unbound()->keep.push_back( dest );
}
void command_buffer_recorder_t::fill(
  const std::shared_ptr< mappable_buffer_t > &dest,
  std::uint32_t value
) {
  fill( dest->get_buffer(), value );
}
void command_buffer_recorder_t::fill(
  const std::shared_ptr< image_t > &dest,
  const std::array< float, 4u > &color
) {
  const auto layout = dest->get_layout().get_uniform_layout();
  const bool need_convert =
    layout != vk::ImageLayout::eSharedPresentKHR &&
    layout != vk::ImageLayout::eGeneral &&
    layout != vk::ImageLayout::eTransferDstOptimal;
  if( need_convert ) {
    convert_image( dest, vk::ImageLayout::eGeneral );
  }
  (*get_factory())->clearColorImage(
    **dest,
    need_convert ? vk::ImageLayout::eGeneral : layout,
    vk::ClearColorValue()
      .setFloat32(
        color
      ),
    {
      vk::ImageSubresourceRange()
        .setAspectMask( format_to_aspect( dest->get_props().get_basic().format ) )
        .setBaseMipLevel( 0u )
        .setLevelCount( dest->get_props().get_basic().mipLevels )
        .setBaseArrayLayer( 0u )
        .setLayerCount( dest->get_props().get_basic().arrayLayers )
    }
  );
  if( need_convert ) {
    convert_image( dest, layout );
  }
}
void command_buffer_recorder_t::fill(
  const std::shared_ptr< image_t > &dest,
  const vk::ClearColorValue &color
) {
  const auto layout = dest->get_layout().get_uniform_layout();
  const bool need_convert =
    layout != vk::ImageLayout::eSharedPresentKHR &&
    layout != vk::ImageLayout::eGeneral &&
    layout != vk::ImageLayout::eTransferDstOptimal;
  if( need_convert ) {
    convert_image( dest, vk::ImageLayout::eGeneral );
  }
  (*get_factory())->clearColorImage(
    **dest,
    need_convert ? vk::ImageLayout::eGeneral : layout,
    color,
    {
      vk::ImageSubresourceRange()
        .setAspectMask( format_to_aspect( dest->get_props().get_basic().format ) )
        .setBaseMipLevel( 0u )
        .setLevelCount( dest->get_props().get_basic().mipLevels )
        .setBaseArrayLayer( 0u )
        .setLayerCount( dest->get_props().get_basic().arrayLayers )
    }
  );
  if( need_convert ) {
    convert_image( dest, layout );
  }
}
void command_buffer_recorder_t::fill(
  const std::shared_ptr< image_t > &dest,
  const vk::ClearColorValue &color,
  const vk::ImageSubresourceRange &range_
) {
  const auto layout = dest->get_layout().get_uniform_layout();
  const bool need_convert =
    layout != vk::ImageLayout::eSharedPresentKHR &&
    layout != vk::ImageLayout::eGeneral &&
    layout != vk::ImageLayout::eTransferDstOptimal;
  auto range = range_;
  range.setAspectMask( format_to_aspect( dest->get_props().get_basic().format ) );
  if( range.baseMipLevel == 0u && range.levelCount == 0u ) {
    range
      .setBaseMipLevel( 0u )
      .setLevelCount( dest->get_props().get_basic().mipLevels );
  }
  if( range.baseArrayLayer == 0u && range.layerCount == 0u ) {
    range
      .setBaseArrayLayer( 0u )
      .setLayerCount( dest->get_props().get_basic().arrayLayers );
  }
  if( need_convert ) {
    convert_image( dest, vk::ImageLayout::eGeneral );
  }
  (*get_factory())->clearColorImage(
    **dest,
    need_convert ? vk::ImageLayout::eGeneral : layout,
    color,
    { range }
  );
  if( need_convert ) {
    convert_image( dest, layout );
  }
}
void command_buffer_recorder_t::fill(
  const std::shared_ptr< image_t > &dest,
  const std::array< float, 4u > &color,
  const vk::ImageSubresourceRange &range
) {
  fill(
    dest,
    vk::ClearColorValue()
      .setFloat32(
        color
      ),
    range
  );
}

}

