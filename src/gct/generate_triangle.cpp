#include <fstream>
#include <gct/buffer.hpp>
#include <gct/allocator.hpp>
#include <gct/device.hpp>
#include <gct/command_pool.hpp>
#include <gct/command_buffer.hpp>
#include <gct/command_buffer_recorder.hpp>

namespace gct {
  std::tuple<
    std::shared_ptr< buffer_t >,
    pipeline_vertex_input_state_create_info_t,
    std::uint32_t
  >
  command_buffer_recorder_t::generate_triangle(
    const std::shared_ptr< allocator_t > &allocator
  ) {
    const std::vector< float > data{
      // position
      -1.0f, 1.0f, 0.0f,
      1.0f, 1.0f, 0.0f,
      0.0f, -1.0f, 0.0f,
      // normal
      0.0f, 0.0f, -1.0f,
      0.0f, 0.0f, -1.0f,
      0.0f, 0.0f, -1.0f,
      // tangent
      1.0f, 0.0f, 0.0f,
      1.0f, 0.0f, 0.0f,
      1.0f, 0.0f, 0.0f,
      // texcoord
      0.0f, 0.0f,
      0.0f, 1.0f,
      1.0f, 0.0f,
      // color
      1.0f, 1.0f, 1.0f,
      1.0f, 1.0f, 1.0f,
      1.0f, 1.0f, 1.0f,
    };
    auto buffer = load_buffer(
      allocator,
      data.data(),
      data.size() * sizeof( float ),
      vk::BufferUsageFlagBits::eVertexBuffer|
      vk::BufferUsageFlagBits::eAccelerationStructureBuildInputReadOnlyKHR
    );
    /*
    static const std::vector< vk::VertexInputBindingDescription > binding{
      vk::VertexInputBindingDescription()
        .setBinding( 0 )
        .setStride( sizeof( float ) * 3 )
        .setInputRate( vk::VertexInputRate::eVertex ),
      vk::VertexInputBindingDescription()
        .setBinding( 1 )
        .setStride( sizeof( float ) * 3 )
        .setInputRate( vk::VertexInputRate::eVertex ),
      vk::VertexInputBindingDescription()
        .setBinding( 2 )
        .setStride( sizeof( float ) * 3 )
        .setInputRate( vk::VertexInputRate::eVertex ),
      vk::VertexInputBindingDescription()
        .setBinding( 3 )
        .setStride( sizeof( float ) * 2 )
        .setInputRate( vk::VertexInputRate::eVertex ),
      vk::VeGrtexInputBindingDescription()
        .setBinding( 4 )
        .setStride( sizeof( float ) * 3 )
        .setInputRate( vk::VertexInputRate::eVertex )
    };
    static const std::vector< vk::VertexInputAttributeDescription > attribute{
      vk::VertexInputBindingDescription()
        .setLocation( 0 )
        .setBinding( 0 )
        .setFormat( vk::Format::eR32G32B32Sfloat )
        .setOffset( 0 ),
      vk::VertexInputBindingDescription()
        .setLocation( 1 )
        .setBinding( 1 )
        .setFormat( vk::Format::eR32G32B32Sfloat )
        .setOffset( sizeof( float ) * 9 ),
      vk::VertexInputBindingDescription()
        .setLocation( 2 )
        .setBinding( 2 )
        .setFormat( vk::Format::eR32G32B32Sfloat )
        .setOffset( sizeof( float ) * 18 ),
      vk::VertexInputBindingDescription()
        .setLocation( 3 )
        .setBinding( 3 )
        .setFormat( vk::Format::eR32G32B32Sfloat )
        .setOffset( sizeof( float ) * 27 ),
      vk::VertexInputBindingDescription()
        .setLocation( 4 )
        .setBinding( 4 )
        .setFormat( vk::Format::eR32G32B32Sfloat )
        .setOffset( sizeof( float ) * 33 )
    };
    */
    auto vistat = pipeline_vertex_input_state_create_info_t()
      .add_vertex_input_binding_description(
        vk::VertexInputBindingDescription()
          .setBinding( 0 )
          .setStride( sizeof( float ) * 3 )
          .setInputRate( vk::VertexInputRate::eVertex )
      )
      .add_vertex_input_binding_description(
        vk::VertexInputBindingDescription()
          .setBinding( 1 )
          .setStride( sizeof( float ) * 3 )
          .setInputRate( vk::VertexInputRate::eVertex )
      )
      .add_vertex_input_binding_description(
        vk::VertexInputBindingDescription()
          .setBinding( 2 )
          .setStride( sizeof( float ) * 3 )
          .setInputRate( vk::VertexInputRate::eVertex )
      )
      .add_vertex_input_binding_description(
        vk::VertexInputBindingDescription()
          .setBinding( 3 )
          .setStride( sizeof( float ) * 2 )
          .setInputRate( vk::VertexInputRate::eVertex )
      )
      .add_vertex_input_binding_description(
        vk::VertexInputBindingDescription()
          .setBinding( 4 )
          .setStride( sizeof( float ) * 3 )
          .setInputRate( vk::VertexInputRate::eVertex )
      )
      .add_vertex_input_attribute_description(
        vk::VertexInputAttributeDescription()
          .setLocation( 0 )
          .setBinding( 0 )
          .setFormat( vk::Format::eR32G32B32Sfloat )
          .setOffset( 0 )
      )
      .add_vertex_input_attribute_description(
        vk::VertexInputAttributeDescription()
          .setLocation( 1 )
          .setBinding( 1 )
          .setFormat( vk::Format::eR32G32B32Sfloat )
          .setOffset( sizeof( float ) * 9 )
      )
      .add_vertex_input_attribute_description(
        vk::VertexInputAttributeDescription()
          .setLocation( 2 )
          .setBinding( 2 )
          .setFormat( vk::Format::eR32G32B32Sfloat )
          .setOffset( sizeof( float ) * 18 )
      )
      .add_vertex_input_attribute_description(
        vk::VertexInputAttributeDescription()
          .setLocation( 3 )
          .setBinding( 3 )
          .setFormat( vk::Format::eR32G32B32Sfloat )
          .setOffset( sizeof( float ) * 27 )
      )
      .add_vertex_input_attribute_description(
        vk::VertexInputAttributeDescription()
          .setLocation( 4 )
          .setBinding( 4 )
          .setFormat( vk::Format::eR32G32B32Sfloat )
          .setOffset( sizeof( float ) * 33 )
      );
    return std::tuple( buffer, vistat, 3 );
  }
}
