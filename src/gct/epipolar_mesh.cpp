#include <gct/buffer.hpp>
#include <gct/mappable_buffer.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/compute.hpp>
#include <gct/exception.hpp>
#include <gct/allocator.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/compute_pipeline.hpp>
#include <gct/pipeline_layout.hpp>
#include <gct/epipolar_mesh.hpp>

namespace gct {

epipolar_mesh::epipolar_mesh(
  const epipolar_mesh_create_info &ci
) : props( ci ) {
  if( props.stride % sizeof( float ) ) {
    throw exception::invalid_argument( "Stride must be aligned to 4 bytes.", __FILE__, __LINE__ );
  }
  const auto pos = props.attrs.find( vertex_attribute_usage_t::POSITION );
  if( pos == props.attrs.end() ) {
    throw exception::invalid_argument( "Vertex attribute POSITION is required.", __FILE__, __LINE__ );
  }
  if( pos->second.offset % sizeof( float ) ) {
    throw exception::invalid_argument( "Vertex attribute POSITION must be aligned to 4 bytes.", __FILE__, __LINE__ );
  }
  if( pos->second.format.depth != 32u ) {
    throw exception::invalid_argument( "Vertex attribute POSITION must be a vector of float.", __FILE__, __LINE__ );
  }
  if( pos->second.format.component != numeric_component_type_t::float_ ) {
    throw exception::invalid_argument( "Vertex attribute POSITION must be a vector of float.", __FILE__, __LINE__ );
  }
  if( pos->second.format.composite != numeric_composite_type_t::vector ) {
    throw exception::invalid_argument( "Vertex attribute POSITION must be a vector of float.", __FILE__, __LINE__ );
  }
  if( pos->second.format.rows < 2u ) {
    throw exception::invalid_argument( "Vertex attribute POSITION must be a vector of float.", __FILE__, __LINE__ );
  }
  const auto texcoord = props.attrs.find( vertex_attribute_usage_t::TEXCOORD );
  if( texcoord == props.attrs.end() ) {
    throw exception::invalid_argument( "Vertex attribute TEXCOORD is required.", __FILE__, __LINE__ );
  }
  if( texcoord->second.offset % sizeof( float ) ) {
    throw exception::invalid_argument( "Vertex attribute TEXCOORD must be aligned to 4 bytes.", __FILE__, __LINE__ );
  }
  if( texcoord->second.format.depth != 32u ) {
    throw exception::invalid_argument( "Vertex attribute TEXCOORD must be a vector of float.", __FILE__, __LINE__ );
  }
  if( texcoord->second.format.component != numeric_component_type_t::float_ ) {
    throw exception::invalid_argument( "Vertex attribute TEXCOORD must be a vector of float.", __FILE__, __LINE__ );
  }
  if( texcoord->second.format.composite != numeric_composite_type_t::vector ) {
    throw exception::invalid_argument( "Vertex attribute TEXCOORD must be a vector of float.", __FILE__, __LINE__ );
  }
  if( texcoord->second.format.rows < 2u ) {
    throw exception::invalid_argument( "Vertex attribute TEXCOORD must be a vector of float.", __FILE__, __LINE__ );
  }
  pc.local_to_screen_matrix = glm::mat2( 1.0, 0.0, 0.0, 1.0 );
  pc.light_pos = glm::vec2( 0.0, 0.0 );
  pc.stride = props.stride / sizeof( float );
  pc.vertex_offset = pos->second.offset / sizeof( float );
  pc.texcoord_offset = texcoord->second.offset / sizeof( float );

  for( std::size_t i = 0u; i != props.swapchain_image_count; ++i ) {
    vertex_buffer.push_back(
      props.allocator_set.allocator->create_mappable_buffer(
        sizeof( float ) * pc.stride * props.vertex_count * props.pole_count,
        vk::BufferUsageFlagBits::eStorageBuffer|
        vk::BufferUsageFlagBits::eVertexBuffer
      )
    );
  }
  index_buffer_staging =
    props.allocator_set.allocator->create_mappable_buffer(
      sizeof( std::uint16_t ) * 6u * ( props.pole_count - 1u ) * ( props.vertex_count - 1u ),
      vk::BufferUsageFlagBits::eStorageBuffer|
      vk::BufferUsageFlagBits::eIndexBuffer
    );
  {
    std::vector< std::uint16_t > index_buffer_host;
    index_buffer_host.reserve( 6u * ( props.pole_count - 1u ) * ( props.vertex_count - 1u ) );
    for( unsigned int p = 0u; p != ( props.pole_count - 1u ); ++p ) {
      for( unsigned int v = 0u; v != ( props.vertex_count - 1u ); ++v ) {
        unsigned int v0 = p * props.vertex_count + v;
        unsigned int v1 = p * props.vertex_count + v + 1u;
        unsigned int v2 = ( p + 1u ) * props.vertex_count + v;
        unsigned int v3 = ( p + 1u ) * props.vertex_count + v + 1u;
        index_buffer_host.push_back( v0 );
        index_buffer_host.push_back( v1 );
        index_buffer_host.push_back( v2 );
        index_buffer_host.push_back( v2 );
        index_buffer_host.push_back( v1 );
        index_buffer_host.push_back( v3 );
      }
    }
    {
      auto mapped = index_buffer_staging->get_staging_buffer()->map< std::uint16_t >();
      std::copy(
        index_buffer_host.begin(),
        index_buffer_host.end(),
        mapped.begin()
      );
    }
  }
  index_buffer = index_buffer_staging->get_buffer();
  generate.push_back(
    compute(
      compute_create_info()
        .set_allocator_set( props.allocator_set )
        .set_swapchain_image_count( props.swapchain_image_count )
        .set_shader( props.generate2_shader )
        .set_resources( props.resources )
        .add_resource( { "vertex_buffer", vertex_buffer } )
    )
  );
  generate.push_back(
    compute(
      compute_create_info()
        .set_allocator_set( props.allocator_set )
        .set_swapchain_image_count( props.swapchain_image_count )
        .set_shader( props.generate3_shader )
        .set_resources( props.resources )
        .add_resource( { "vertex_buffer", vertex_buffer } )
    )
  );
  generate.push_back(
    compute(
      compute_create_info()
        .set_allocator_set( props.allocator_set )
        .set_swapchain_image_count( props.swapchain_image_count )
        .set_shader( props.generate4_shader )
        .set_resources( props.resources )
        .add_resource( { "vertex_buffer", vertex_buffer } )
    )
  );
}
void epipolar_mesh::init(
  command_buffer_recorder_t &rec
) const {
  rec.copy(
    index_buffer_staging->get_staging_buffer(),
    index_buffer_staging->get_buffer()
  );
  rec.transfer_to_compute_barrier(
    { index_buffer_staging->get_buffer() },
    {}
  );
  index_buffer_staging.reset();
}

void epipolar_mesh::operator()(
  command_buffer_recorder_t &rec,
  unsigned int image_index,
  const glm::vec2 &center
) const {
  glm::mat2 screen_to_local_matrix;
  unsigned int mode = 0u;
  if( center.x >= 1.0 ) {
    if( center.y >= 1.0 ) {
      screen_to_local_matrix = glm::mat2(
         0,  1,
        -1,  0
      );
      mode = 0u;
    }
    else if( center.y > -1.0 ) {
      screen_to_local_matrix = glm::mat2(
        -1,  0,
         0, -1
      );
      mode = 1u;
    }
    else {
      screen_to_local_matrix = glm::mat2(
        -1,  0,
         0, -1
      );
      mode = 0u;
    }
  }
  else if( center.x > -1.0 ) {
    if( center.y >= 1.0 ) {
      screen_to_local_matrix = glm::mat2(
         0,  1,
        -1,  0
      );
      mode = 1u;
    }
    else if( center.y > -1.0 ) {
      screen_to_local_matrix = glm::mat2(
         1,  0,
         0,  1
      );
      mode = 2u;
    }
    else {
      screen_to_local_matrix = glm::mat2(
         0, -1,
         1,  0
      );
      mode = 1u;
    }
  }
  else {
    if( center.y >= 1.0 ) {
      screen_to_local_matrix = glm::mat2(
         1,  0,
         0,  1
      );
      mode = 0u;
    }
    else if( center.y > -1.0 ) {
      screen_to_local_matrix = glm::mat2(
         1,  0,
         0,  1
      );
      mode = 1u;
    }
    else {
      screen_to_local_matrix = glm::mat2(
         0,  -1,
         1,  0
      );
      mode = 0u;
    }
  }
  pc.light_pos = screen_to_local_matrix * center;
  if( pc.light_pos.x <= -1.0 && pc.light_pos.x > -1.01 ) {
    pc.light_pos.x = -1.01;
  }
  pc.local_to_screen_matrix = glm::inverse( screen_to_local_matrix );
  rec->pushConstants(
    **generate[ mode ].get_pipeline()->get_props().get_layout(),
    vk::ShaderStageFlagBits::eCompute,
    0u,
    sizeof( push_constants ),
    reinterpret_cast< void* >( &pc )
  );
  generate[ mode ]( rec, image_index, props.pole_count, props.vertex_count, 1u );
  rec.barrier(
    vk::AccessFlagBits::eShaderWrite,
    vk::AccessFlagBits::eVertexAttributeRead,
    vk::PipelineStageFlagBits::eComputeShader,
    vk::PipelineStageFlagBits::eVertexInput,
    vk::DependencyFlagBits( 0 ),
    {
      vertex_buffer[ image_index ]->get_buffer()
    },
    {}
  );
  rec.compute_to_transfer_barrier(
    {
      vertex_buffer[ image_index ]->get_buffer()
    },
    {}
  );
}

}
