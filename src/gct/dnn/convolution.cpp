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
#include <gct/dnn/convolution.hpp>

namespace gct::dnn::operation {

convolution::convolution(
  const std::shared_ptr< allocator_t > &allocator,
  const std::shared_ptr< descriptor_pool_t > &descriptor_pool,
  const std::shared_ptr< pipeline_cache_t > &pipeline_cache,
  const nnef::Operation &op,
  const std::unordered_map< std::string, std::shared_ptr< shader_module_t > > shaders,
  const std::unordered_map< std::string, nnef_data_t > &bufs
) :
  input( get_buffer( op, bufs, "input" ) ),
  weight( get_buffer( op, bufs, "filter" ) ),
  bias( get_buffer_maybe( op, bufs, "bias" ) ) {

  if( input.dim.size() < 3u ) {
    throw -1;
  }
  if( weight.dim.size() != 4u ) {
    throw -1;
  }
  spec_t spec;
  spec.filter_size_x = weight.dim[ 3 ];
  spec.filter_size_y = weight.dim[ 2 ];
  spec.filter_size_z = weight.dim[ 1 ];
  spec.input_dim_x = input.dim[ 3 ];
  spec.input_dim_y = input.dim[ 2 ];
  spec.input_dim_z = input.dim[ 1 ];
  const auto &attr = op.attribs;
  const auto &padding = std::find_if(
    attr.begin(), attr.end(),
    []( const auto &v ) {
      return v.first == "padding";
    }
  );
  if( padding != attr.end() ) {
    const auto &padding_ = padding->second.array();
    if( padding_.size() != 2 ) {
      throw -1;
    }
    const auto &x = padding_[ 0 ].tuple();
    if( x.size() != 2 ) {
      throw -1;
    }
    const auto &y = padding_[ 1 ].tuple();
    if( y.size() != 2 ) {
      throw -1;
    }
    spec.lpadding = x[ 0 ].integer();
    spec.rpadding = x[ 1 ].integer();
    spec.tpadding = y[ 0 ].integer();
    spec.bpadding = y[ 1 ].integer();
  }
  const auto &stride = std::find_if(
    attr.begin(), attr.end(),
    []( const auto &v ) {
      return v.first == "stride";
    }
  );
  if( stride != attr.end() ) {
    const auto &stride_ = stride->second.array();
    if( stride_.size() != 2 ) {
      throw -1;
    }
    spec.stride_x = stride_[ 0 ].integer();
    spec.stride_y = stride_[ 1 ].integer();
  }
  else {
    spec.stride_x = 1;
    spec.stride_y = 1;
  }
  const auto &dilation = std::find_if(
    attr.begin(), attr.end(),
    []( const auto &v ) {
      return v.first == "dilation";
    }
  );
  if( dilation != attr.end() ) {
    const auto &dilation_ = dilation->second.array();
    if( dilation_.size() != 2 ) {
      throw -1;
    }
    spec.dilation_x = dilation_[ 0 ].integer();
    spec.dilation_y = dilation_[ 1 ].integer();
  }
  else {
    spec.dilation_x = 1;
    spec.dilation_y = 1;
  }
  
  output.type.component = numeric_component_type_t::float_;
  output.type.depth = 32u;
  output.type.rows = 1u;
  output.dim.resize( 4 );
  output.dim[ 3 ] = ( spec.input_dim_x + spec.lpadding + spec.rpadding - ( spec.filter_size_x - 1 ) * spec.dilation_x ) / spec.stride_x;
  output.dim[ 2 ] = ( spec.input_dim_y + spec.tpadding + spec.bpadding - ( spec.filter_size_y - 1 ) * spec.dilation_y ) / spec.stride_y;
  output.dim[ 1 ] = weight.dim[ 0 ];
  output.dim[ 0 ] = input.dim[ 0 ];
  output.buffer = allocator->create_buffer(
    sizeof( float ) * get_length( output.dim.begin(), output.dim.end() ),
    vk::BufferUsageFlagBits::eStorageBuffer|
    vk::BufferUsageFlagBits::eTransferDst|
    vk::BufferUsageFlagBits::eTransferSrc,
    VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE
  );
  
  spec.output_dim_x = output.dim[ 3 ];
  spec.output_dim_y = output.dim[ 2 ];
  spec.output_dim_z = output.dim[ 1 ];
  spec.bias_mode = bias.buffer ? 1 : 0;
  
  std::string shader_name( "convolution_" );
  shader_name += numeric_type_to_suffix( input.type );
  shader_name += "_";
  shader_name += numeric_type_to_suffix( weight.type );
  shader_name += "_";
  shader_name += bias.buffer ? numeric_type_to_suffix( bias.type ) : std::string( "f32" );
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

  if( bias.buffer ) {
    descriptor_set->update(
      {
        gct::write_descriptor_set_t()
          .set_basic( (*descriptor_set)[ "input_vector" ] )
          .add_buffer( input.buffer ),
        gct::write_descriptor_set_t()
          .set_basic( (*descriptor_set)[ "weight" ] )
          .add_buffer( weight.buffer ),
        gct::write_descriptor_set_t()
          .set_basic( (*descriptor_set)[ "bias" ] )
          .add_buffer( bias.buffer ),
        gct::write_descriptor_set_t()
          .set_basic( (*descriptor_set)[ "output_vector" ] )
          .add_buffer( output.buffer )
      }
    );
  }
  else {
    descriptor_set->update(
      {
        gct::write_descriptor_set_t()
          .set_basic( (*descriptor_set)[ "input_vector" ] )
          .add_buffer( input.buffer ),
        gct::write_descriptor_set_t()
          .set_basic( (*descriptor_set)[ "weight" ] )
          .add_buffer( weight.buffer ),
        gct::write_descriptor_set_t()
          .set_basic( (*descriptor_set)[ "output_vector" ] )
          .add_buffer( output.buffer )
      }
    );
  }

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

  pipeline = pipeline_cache->get_pipeline(
    gct::compute_pipeline_create_info_t()
      .set_stage(
        gct::pipeline_shader_stage_create_info_t()
          .set_shader_module( shader->second )
          .set_specialization_info(
            gct::specialization_info_t< spec_t >()
              .set_data( spec_t( spec ) )
              .add_map< std::int32_t >( 1, offsetof( spec_t, filter_size_x ) )
              .add_map< std::int32_t >( 2, offsetof( spec_t, filter_size_y ) )
              .add_map< std::int32_t >( 3, offsetof( spec_t, filter_size_z ) )
              .add_map< std::int32_t >( 4, offsetof( spec_t, lpadding ) )
              .add_map< std::int32_t >( 5, offsetof( spec_t, rpadding ) )
              .add_map< std::int32_t >( 6, offsetof( spec_t, tpadding ) )
              .add_map< std::int32_t >( 7, offsetof( spec_t, bpadding ) )
              .add_map< std::int32_t >( 8, offsetof( spec_t, stride_x ) )
              .add_map< std::int32_t >( 9, offsetof( spec_t, stride_y ) )
              .add_map< std::int32_t >( 10, offsetof( spec_t, dilation_x ) )
              .add_map< std::int32_t >( 11, offsetof( spec_t, dilation_y ) )
              .add_map< std::int32_t >( 12, offsetof( spec_t, input_dim_x ) )
              .add_map< std::int32_t >( 13, offsetof( spec_t, input_dim_y ) )
              .add_map< std::int32_t >( 14, offsetof( spec_t, input_dim_z ) )
              .add_map< std::int32_t >( 15, offsetof( spec_t, output_dim_x ) )
              .add_map< std::int32_t >( 16, offsetof( spec_t, output_dim_y ) )
              .add_map< std::int32_t >( 17, offsetof( spec_t, output_dim_z ) )
              .add_map< float >( 18, offsetof( spec_t, border_value ) )
              .add_map< float >( 19, offsetof( spec_t, bias_mode ) )
              .add_map< float >( 20, offsetof( spec_t, bias_value ) )
          )
      )
      .set_layout( pipeline_layout )
  );

  constexpr static std::int32_t block_size = 32;
  exec_dim[ 0 ] = ( spec.output_dim_x / block_size + ( ( spec.output_dim_x % block_size ) ? 1 : 0 ) ) * block_size;
  exec_dim[ 1 ] = ( spec.output_dim_y / block_size + ( ( spec.output_dim_y % block_size ) ? 1 : 0 ) ) * block_size;
  exec_dim[ 2 ] = spec.output_dim_z * output.dim[ 0 ];
}
void convolution::operator()( command_buffer_recorder_t &rec ) {
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

