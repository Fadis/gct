#include <cstdint>
#include <locale>
#include <gct/format.hpp>
#include <gct/vertex_attributes.hpp>
#include <gct/spirv_reflect.h>
namespace gct {

  std::tuple<
    std::shared_ptr< gct::buffer_t >,
    gct::pipeline_vertex_input_state_create_info_t
  >
  std::shared_ptr< image_t > command_buffer_recorder_t::create_triangle(
    const std::shared_ptr< allocator_t > &allocator,
    const shader_module_reflection_t &reflection,
    bool smooth
  ) {
    std::shared_ptr< gct::buffer_t > vertex_buffer;
    const std::vector< float > vertex{
      0.f, 0.f, 0.f,
      0.f, 1.f, 0.f,
      1.f, 0.f, 0.f
    };
    vertex_buffer = rec.load_buffer(
      allocator,
      vertex.data(),
      sizeof( float ) * vertex.size(),
      vk::BufferUsageFlagBits::eVertexBuffer
    );
    barrier(
      vk::AccessFlagBits::eTransferWrite,
      vk::AccessFlagBits::eVertexAttributeRead,
      vk::PipelineStageFlagBits::eTransfer,
      vk::PipelineStageFlagBits::eVertexInput,
      vk::DependencyFlagBits( 0 ),
      { vertex_buffer },
      {}
    );
    auto vistat = gct::pipeline_vertex_input_state_create_info_t()
      .add_vertex_input_binding_description(
        vk::VertexInputBindingDescription()
          .setBinding( 0 )
          .setInputRate( vk::VertexInputRate::eVertex )
          .setStride( sizeof( float ) * 3 )
      )
      .add_vertex_input_attribute_description(
        vk::VertexInputAttributeDescription()
          .setLocation( 0 )
          .setFormat( vk::Format::eR32G32B32Sfloat )
          .setBinding( 0 )
          .setOffset( 0 )
    );

  const auto input_assembly =
    gct::pipeline_input_assembly_state_create_info_t()
      .set_basic(
        vk::PipelineInputAssemblyStateCreateInfo()
          .setTopology( vk::PrimitiveTopology::eTriangleList )
      );

