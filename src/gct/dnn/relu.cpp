#include <gct/nnef_data.hpp>
#include <gct/allocator.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/descriptor_set_layout.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/pipeline_layout_create_info.hpp>
#include <gct/buffer_view_create_info.hpp>
#include <gct/shader_module.hpp>
#include <gct/compute_pipeline_create_info.hpp>
#include <gct/compute_pipeline.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/write_descriptor_set.hpp>
#include <gct/device.hpp>
#include <gct/get_device.hpp>
#include <gct/buffer.hpp>
#include <gct/image.hpp>
#include <gct/dnn/nnef.hpp>
#include <gct/dnn/relu.hpp>

namespace gct::dnn::operation {

relu::relu(
  const std::shared_ptr< allocator_t > &allocator,
  const std::shared_ptr< descriptor_pool_t > &descriptor_pool,
  const std::shared_ptr< pipeline_cache_t > &pipeline_cache,
  const nnef::Operation &op,
  const std::unordered_map< std::string, std::shared_ptr< shader_module_t > > shaders,
  const std::unordered_map< std::string, nnef_data_t > &bufs
) :
  input( get_buffer( op, bufs, "x" ) ) {
  if( input.dim.size() < 1u ) {
    throw -1;
  }
  output.dim = input.dim;
  output.type.component = numeric_component_type_t::float_;
  output.type.depth = 32u;
  output.type.rows = 1u;
  output.buffer = allocator->create_buffer(
    sizeof( float ) * get_length( output.dim.begin(), output.dim.end() ),
    vk::BufferUsageFlagBits::eStorageBuffer|
    vk::BufferUsageFlagBits::eTransferDst|
    vk::BufferUsageFlagBits::eTransferSrc,
    VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE
  );
  std::string shader_name( "relu_" );
  shader_name += numeric_type_to_suffix( input.type );
  shader_name += "_";
  shader_name += numeric_type_to_suffix( output.type );
  const auto shader = shaders.find( shader_name );
  if( shader == shaders.end() ) {
    throw -1;
  }

  auto &device = get_device( *allocator );

  auto descriptor_set_layout = device.get_descriptor_set_layout(
    gct::descriptor_set_layout_create_info_t()
      .add_binding(
        shader->second->get_props().get_reflection()
      )
      .rebuild_chain()
  );

  descriptor_set = descriptor_pool->allocate( descriptor_set_layout );

  descriptor_set->update(
    {
      gct::write_descriptor_set_t()
        .set_basic( (*descriptor_set)[ "input_vector" ] )
        .add_buffer( input.buffer ),
      gct::write_descriptor_set_t()
        .set_basic( (*descriptor_set)[ "output_vector" ] )
        .add_buffer( output.buffer )
    }
  );

  pipeline_layout = device.get_pipeline_layout(
    gct::pipeline_layout_create_info_t()
      .add_descriptor_set_layout( descriptor_set_layout )
      .add_push_constant_range(
        vk::PushConstantRange()
          .setStageFlags( vk::ShaderStageFlagBits::eCompute )
          .setOffset( 0 )
          .setSize( 32u )
      )
  );

  spec_t spec{
    std::uint32_t( get_length( output.dim.begin(), output.dim.end() ) )
  };

  pipeline = pipeline_cache->get_pipeline(
    gct::compute_pipeline_create_info_t()
      .set_stage(
        gct::pipeline_shader_stage_create_info_t()
          .set_shader_module( shader->second )
          .set_specialization_info(
            gct::specialization_info_t< spec_t >()
              .set_data( spec_t( spec ) )
              .add_map< std::uint32_t >( 1, offsetof( spec_t, input_length ) )
          )
      )
      .set_layout( pipeline_layout )
  );

  constexpr static std::uint32_t block_size = 1024;
  const std::uint32_t aligned_input_length = ( spec.input_length / block_size + ( ( spec.input_length % block_size ) ? 1u : 0u ) ) * block_size;

  exec_dim[ 0 ] = aligned_input_length;
  exec_dim[ 1 ] = 1;
  exec_dim[ 2 ] = 1;
}
void relu::operator()( command_buffer_recorder_t &rec ) {
  rec.compute_barrier( { input.buffer }, {} );
  rec.bind_descriptor_set(
    vk::PipelineBindPoint::eCompute,
    pipeline_layout,
    descriptor_set
  );
  rec.bind_pipeline( pipeline );
  rec.dispatch_threads( exec_dim[ 0 ], exec_dim[ 1 ], exec_dim[ 2 ] );
}

}

