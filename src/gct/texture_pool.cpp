#include <nlohmann/json.hpp>
#include <vulkan2json/DescriptorImageInfo.hpp>
#include <gct/allocator.hpp>
#include <gct/buffer.hpp>
#include <gct/format.hpp>
#include <gct/image.hpp>
#include <gct/compute_create_info.hpp>
#include <gct/write_descriptor_set.hpp>
#include <gct/descriptor_set.hpp>
#include <gct/compute.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/command_buffer.hpp>
#include <gct/exception.hpp>
#include <gct/texture_pool.hpp>
#include <gct/sampler_pool.hpp>
#include <gct/shader_module_reflection.hpp>
#include <gct/simplify_buffer_copy.hpp>

namespace gct {

texture_pool::texture_index_t texture_pool::state_type::allocate_index() {
  const auto index = index_allocator.allocate();
  if( index >= texture_state.size() ) {
    texture_state.resize( index + 1u );
  }
  return index; 
}

void texture_pool::state_type::release_index( texture_pool::texture_index_t index ) {
  if( index >= texture_state.size() || !texture_state[ index ].valid ) {
    return;
  }
  texture_state[ index ].valid = false;
  index_allocator.release( index );
}

texture_pool::views texture_pool::state_type::allocate(
  const sampler_pool::sampler_descriptor &sid,
  const image_pool::views &iid
) {
  if( execution_pending ) {
    throw exception::runtime_error( "texture_pool::state_type::allocate : last execution is not completed yet", __FILE__, __LINE__ );
  }

  const auto s = props.sampler->get( sid );
  if( !s ) {
    throw exception::runtime_error( "texture_pool::state_type::allocate : sampler not found", __FILE__, __LINE__ );
  }

  const texture_index_t normalized_index = iid.normalized ? allocate_index() : texture_index_t( 0u );
  const texture_index_t srgb_index = iid.srgb ? allocate_index() : texture_index_t( 0u );
  const texture_index_t linear_index = iid.linear ? allocate_index() : texture_index_t( 0u );

  texture_descriptor normalized_desc;
  texture_descriptor srgb_desc;
  texture_descriptor linear_desc;

  std::shared_ptr< image_view_t > normalized;
  std::shared_ptr< image_view_t > srgb;
  std::shared_ptr< image_view_t > linear;
  if( iid.normalized ) {
    normalized = props.image->get( iid.normalized );
    write_request_list.push_back(
      write_request()
        .set_index( normalized_index )
        .set_sampler( s )
        .set_view( normalized )
    );
    texture_state[ normalized_index ] =
      texture_state_type()
        .set_valid( true )
        .set_write_request_index( write_request_list.size() - 1u )
        .set_sampler( s )
        .set_view( normalized );
    normalized_desc = texture_descriptor(
      new texture_index_t( normalized_index ),
      [self=shared_from_this()]( const texture_index_t *p ) {
        if( p ) {
          self->release( *p );
          delete p;
        }
      }
    );
    used_on_gpu.push_back( normalized_desc );
  }

  if( iid.srgb ) {
    srgb = props.image->get( iid.srgb ); 
    write_request_list.push_back(
      write_request()
        .set_index( srgb_index )
        .set_sampler( s )
        .set_view( srgb )
    );
    texture_state[ srgb_index ] =
      texture_state_type()
        .set_valid( true )
        .set_write_request_index( write_request_list.size() - 1u )
        .set_sampler( s )
        .set_view( srgb );
 
    srgb_desc = texture_descriptor(
      new texture_index_t( srgb_index ),
      [self=shared_from_this()]( const texture_index_t *p ) {
        if( p ) {
          self->release( *p );
          delete p;
        }
      }
    );
    used_on_gpu.push_back( srgb_desc );
  }
  
  if( iid.linear ) {
    linear = props.image->get( iid.linear );
    write_request_list.push_back(
      write_request()
        .set_index( linear_index )
        .set_sampler( s )
        .set_view( linear )
    );
    texture_state[ linear_index ] =
      texture_state_type()
        .set_valid( true )
        .set_write_request_index( write_request_list.size() - 1u )
        .set_sampler( s )
        .set_view( linear );
 
    linear_desc = texture_descriptor(
      new texture_index_t( linear_index ),
      [self=shared_from_this()]( const texture_index_t *p ) {
        if( p ) {
          self->release( *p );
          delete p;
        }
      }
    );
    used_on_gpu.push_back( linear_desc );
  }

  if( metadata_member_pointer ) {
    const auto metadata_aligned_size = metadata_member_pointer->get_stride();
    if( normalized ) {
      const auto &color_prof = normalized->get_factory()->get_props().get_profile();
      const std::uint32_t from_mat = props.csmat.from.find( color_prof.space )->second;
      const std::uint32_t to_mat = props.csmat.to.find( color_prof.space )->second;
      std::vector< std::uint8_t > temp( metadata_aligned_size, 0u );
      temp.data()->*(*metadata_member_pointer)[ 0 ][ "color_profile" ][ "space" ] = std::uint32_t( color_prof.space );
      temp.data()->*(*metadata_member_pointer)[ 0 ][ "color_profile" ][ "gamma" ] = std::uint32_t( color_prof.gamma );
      temp.data()->*(*metadata_member_pointer)[ 0 ][ "color_profile" ][ "max_intensity" ] = 1.0f;
      temp.data()->*(*metadata_member_pointer)[ 0 ][ "color_space_matrix" ][ "from" ] = from_mat;
      temp.data()->*(*metadata_member_pointer)[ 0 ][ "color_space_matrix" ][ "to" ] = to_mat;
      const auto staging_index = staging_index_allocator.allocate( 1u );
      {
        auto mapped = staging_metadata_buffer->map< std::uint8_t >();
        std::copy( temp.begin(), temp.end(), std::next( mapped.begin(), staging_index * metadata_aligned_size ) );
      }
      metadata_write_region.push_back(
        vk::BufferCopy()
          .setSrcOffset( staging_index * metadata_aligned_size )
          .setDstOffset( srgb_index * metadata_aligned_size )
          .setSize( metadata_aligned_size )
      );
    }
    if( srgb ) {
      const auto &color_prof = srgb->get_factory()->get_props().get_profile();
      const std::uint32_t from_mat = props.csmat.from.find( color_prof.space )->second;
      const std::uint32_t to_mat = props.csmat.to.find( color_prof.space )->second;
      std::vector< std::uint8_t > temp( metadata_aligned_size, 0u );
      temp.data()->*(*metadata_member_pointer)[ 0 ][ "color_profile" ][ "space" ] = std::uint32_t( color_prof.space );
      temp.data()->*(*metadata_member_pointer)[ 0 ][ "color_profile" ][ "gamma" ] = std::uint32_t( color_prof.gamma );
      temp.data()->*(*metadata_member_pointer)[ 0 ][ "color_profile" ][ "max_intensity" ] = 1.0f;
      temp.data()->*(*metadata_member_pointer)[ 0 ][ "color_space_matrix" ][ "from" ] = from_mat;
      temp.data()->*(*metadata_member_pointer)[ 0 ][ "color_space_matrix" ][ "to" ] = to_mat;
      const auto staging_index = staging_index_allocator.allocate( 1u );
      {
        auto mapped = staging_metadata_buffer->map< std::uint8_t >();
        std::copy( temp.begin(), temp.end(), std::next( mapped.begin(), staging_index * metadata_aligned_size ) );
      }
      metadata_write_region.push_back(
        vk::BufferCopy()
          .setSrcOffset( staging_index * metadata_aligned_size )
          .setDstOffset( srgb_index * metadata_aligned_size )
          .setSize( metadata_aligned_size )
      );
    }
    if( linear )  {
      const auto &color_prof = linear->get_factory()->get_props().get_profile();
      const std::uint32_t from_mat = props.csmat.from.find( color_prof.space )->second;
      const std::uint32_t to_mat = props.csmat.to.find( color_prof.space )->second;
      std::vector< std::uint8_t > temp( metadata_aligned_size, 0u );
      temp.data()->*(*metadata_member_pointer)[ 0 ][ "color_profile" ][ "space" ] = std::uint32_t( color_prof.space );
      temp.data()->*(*metadata_member_pointer)[ 0 ][ "color_profile" ][ "gamma" ] = std::uint32_t( color_prof.gamma );
      temp.data()->*(*metadata_member_pointer)[ 0 ][ "color_profile" ][ "max_intensity" ] = 1.0f;
      temp.data()->*(*metadata_member_pointer)[ 0 ][ "color_space_matrix" ][ "from" ] = from_mat;
      temp.data()->*(*metadata_member_pointer)[ 0 ][ "color_space_matrix" ][ "to" ] = to_mat;
      const auto staging_index = staging_index_allocator.allocate( 1u );
      {
        auto mapped = staging_metadata_buffer->map< std::uint8_t >();
        std::copy( temp.begin(), temp.end(), std::next( mapped.begin(), staging_index * metadata_aligned_size ) );
      }
      metadata_write_region.push_back(
        vk::BufferCopy()
          .setSrcOffset( staging_index * metadata_aligned_size )
          .setDstOffset( linear_index * metadata_aligned_size )
          .setSize( metadata_aligned_size )
      );
    }
  }


  return views()
    .set_normalized( normalized_desc )
    .set_srgb( srgb_desc )
    .set_linear( linear_desc );
}

texture_pool::texture_descriptor texture_pool::state_type::allocate(
  const sampler_pool::sampler_descriptor &sid,
  const image_pool::image_descriptor &iid
) {
  if( execution_pending ) {
    throw exception::runtime_error( "texture_pool::state_type::allocate : last execution is not completed yet", __FILE__, __LINE__ );
  }

  const texture_index_t index = allocate_index();
    
  const auto s = props.sampler->get( sid );
  const auto linear = props.image->get( iid );

  write_request_list.push_back(
    write_request()
      .set_index( index )
      .set_sampler( s )
      .set_view( linear )
  );
  texture_state[ index ] =
    texture_state_type()
      .set_valid( true )
      .set_write_request_index( write_request_list.size() - 1u )
      .set_sampler( s )
      .set_view( linear );
 
  auto desc = texture_descriptor(
    new texture_index_t( index ),
    [self=shared_from_this()]( const texture_index_t *p ) {
      if( p ) {
        self->release( *p );
        delete p;
      }
    }
  );
  used_on_gpu.push_back( desc );

  if( metadata_member_pointer ) {
    const auto metadata_aligned_size = metadata_member_pointer->get_stride();
    const auto &color_prof = linear->get_factory()->get_props().get_profile();
    const std::uint32_t from_mat = props.csmat.from.find( color_prof.space )->second;
    const std::uint32_t to_mat = props.csmat.to.find( color_prof.space )->second;
    std::vector< std::uint8_t > temp( metadata_aligned_size, 0u );
    temp.data()->*(*metadata_member_pointer)[ 0 ][ "color_profile" ][ "space" ] = std::uint32_t( color_prof.space );
    temp.data()->*(*metadata_member_pointer)[ 0 ][ "color_profile" ][ "gamma" ] = std::uint32_t( color_prof.gamma );
    temp.data()->*(*metadata_member_pointer)[ 0 ][ "color_profile" ][ "max_intensity" ] = 1.0f;
    temp.data()->*(*metadata_member_pointer)[ 0 ][ "color_space_matrix" ][ "from" ] = from_mat;
    temp.data()->*(*metadata_member_pointer)[ 0 ][ "color_space_matrix" ][ "to" ] = to_mat;
    const auto staging_index = staging_index_allocator.allocate( 1u );
    {
      auto mapped = staging_metadata_buffer->map< std::uint8_t >();
      std::copy( temp.begin(), temp.end(), std::next( mapped.begin(), staging_index * metadata_aligned_size ) );
    }
    metadata_write_region.push_back(
      vk::BufferCopy()
        .setSrcOffset( staging_index * metadata_aligned_size )
        .setDstOffset( index * metadata_aligned_size )
        .setSize( metadata_aligned_size )
    );
  }

  return desc;
}

texture_pool::texture_descriptor texture_pool::state_type::allocate(
  const std::shared_ptr< sampler_t > &s,
  const std::shared_ptr< image_view_t > &linear
) {
  if( execution_pending ) {
    throw exception::runtime_error( "texture_pool::state_type::allocate : last execution is not completed yet", __FILE__, __LINE__ );
  }

  const texture_index_t index = allocate_index();

  write_request_list.push_back(
    write_request()
      .set_index( index )
      .set_sampler( s )
      .set_view( linear )
  );
  texture_state[ index ] =
    texture_state_type()
      .set_valid( true )
      .set_write_request_index( write_request_list.size() - 1u )
      .set_sampler( s )
      .set_view( linear );
 
  auto desc = texture_descriptor(
    new texture_index_t( index ),
    [self=shared_from_this()]( const texture_index_t *p ) {
      if( p ) {
        self->release( *p );
        delete p;
      }
    }
  );
  used_on_gpu.push_back( desc );

  if( metadata_member_pointer ) {
    const auto metadata_aligned_size = metadata_member_pointer->get_stride();
    const auto &color_prof = linear->get_factory()->get_props().get_profile();
    const std::uint32_t from_mat = props.csmat.from.find( color_prof.space )->second;
    const std::uint32_t to_mat = props.csmat.to.find( color_prof.space )->second;
    std::vector< std::uint8_t > temp( metadata_aligned_size, 0u );
    temp.data()->*(*metadata_member_pointer)[ 0 ][ "color_profile" ][ "space" ] = std::uint32_t( color_prof.space );
    temp.data()->*(*metadata_member_pointer)[ 0 ][ "color_profile" ][ "gamma" ] = std::uint32_t( color_prof.gamma );
    temp.data()->*(*metadata_member_pointer)[ 0 ][ "color_profile" ][ "max_intensity" ] = 1.0f;
    temp.data()->*(*metadata_member_pointer)[ 0 ][ "color_space_matrix" ][ "from" ] = from_mat;
    temp.data()->*(*metadata_member_pointer)[ 0 ][ "color_space_matrix" ][ "to" ] = to_mat;
    const auto staging_index = staging_index_allocator.allocate( 1u );
    {
      auto mapped = staging_metadata_buffer->map< std::uint8_t >();
      std::copy( temp.begin(), temp.end(), std::next( mapped.begin(), staging_index * metadata_aligned_size ) );
    }
    metadata_write_region.push_back(
      vk::BufferCopy()
        .setSrcOffset( staging_index * metadata_aligned_size )
        .setDstOffset( index * metadata_aligned_size )
        .setSize( metadata_aligned_size )
    );
  }

  return desc;
}


std::pair< std::shared_ptr< image_view_t >, std::shared_ptr< sampler_t > > texture_pool::state_type::get(
  const texture_descriptor &desc
) const {
  if( texture_state.size() <= *desc || !texture_state[ *desc ].valid ) {
    throw exception::runtime_error( "texture_pool::state_type::get : no such texture", __FILE__, __LINE__ );
  }
  return std::make_pair(
    texture_state[ *desc ].view,
    texture_state[ *desc ].sampler
  );
}

void texture_pool::state_type::release( texture_index_t index ) {
  texture_state_type removed;
  {
    std::lock_guard< std::mutex > lock( guard );
    if( texture_state.size() <= index || !texture_state[ index ].valid ) {
      return;
    }
    removed = std::move( texture_state[ index ] );
    texture_state[ index ] = texture_state_type();
    release_index( index );
    /*if( props.descriptor_set ) {
      const auto target = (*props.descriptor_set)[ props.descriptor_name ];
      std::vector< write_descriptor_set_t > updates;
      updates.push_back(
        write_descriptor_set_t()
          .set_basic(
            vk::WriteDescriptorSet( target )
              .setDstArrayElement( index )
              .setDescriptorCount( 1u )
          )
      );
      props.descriptor_set->update(
        std::move( updates )
      );
    }*/
  }
}

texture_pool::state_type::state_type( const texture_pool_create_info &ci ) :
  props( ci ),
  staging_index_allocator( linear_allocator_create_info().set_max( ci.max_request_count ) ),
  index_allocator( linear_allocator_create_info().set_max( ci.max_texture_count ) ) {
  allocate_index();
  if( std::filesystem::exists( props.dummy_shader ) ) {
    reflection.reset(
      new shader_module_reflection_t( props.dummy_shader )
    );
  }
  if( reflection ) {
    metadata_member_pointer = reflection->get_member_pointer_maybe( props.metadata_buffer_name, props.metadata_layout );
    if( metadata_member_pointer ) {
      const auto metadata_aligned_size = metadata_member_pointer->get_stride();
      metadata_buffer = props.allocator_set.allocator->create_buffer(
        metadata_aligned_size * props.max_texture_count,
        vk::BufferUsageFlagBits::eStorageBuffer|
        vk::BufferUsageFlagBits::eTransferSrc|
        vk::BufferUsageFlagBits::eTransferDst,
        VMA_MEMORY_USAGE_GPU_ONLY
      );
      staging_metadata_buffer = props.allocator_set.allocator->create_buffer(
        metadata_aligned_size * props.max_request_count,
        vk::BufferUsageFlagBits::eTransferSrc|
        vk::BufferUsageFlagBits::eTransferDst,
        VMA_MEMORY_USAGE_CPU_TO_GPU
      );
    }
  }
}

void texture_pool::state_type::flush( command_buffer_recorder_t &rec ) {
  if( execution_pending ) {
    return;
  }
  {
    std::lock_guard< std::mutex > lock( guard );
    if( metadata_member_pointer ) {
      simplify_buffer_copy( metadata_write_region );
      if( !metadata_write_region.empty() ) {
        rec->copyBuffer(
          **staging_metadata_buffer,
          **metadata_buffer,
          metadata_write_region
        );
        rec.transfer_barrier( { metadata_buffer }, {} );
      }
    }
    std::vector< write_descriptor_set_t > updates;
    const auto target = (*props.descriptor_set)[ props.descriptor_name ];
    for( const auto &req: write_request_list ) {
      if( props.descriptor_set ) {
        auto u = write_descriptor_set_t()
            .set_basic(
              vk::WriteDescriptorSet( target )
                .setDstArrayElement( req.index )
                .setDescriptorCount( 1u )
            )
            .add_image( req.sampler, req.view, vk::ImageLayout::eShaderReadOnlyOptimal )
            .set_index( req.index );
        updates.push_back( u );
        props.descriptor_set->update(
          std::move( updates )
        );
        updates.clear();
      }
    }
    if( props.descriptor_set ) {
      //props.descriptor_set->update(
      //  std::move( updates )
      //);
    }
  }
  rec.on_executed(
    [self=shared_from_this()]( vk::Result result ) {
      std::vector< texture_descriptor > used_on_gpu;
      {
        std::lock_guard< std::mutex > lock( self->guard );
        for( const auto &desc: self->used_on_gpu ) {
          if( self->texture_state.size() > *desc && self->texture_state[ *desc ].valid ) {
            auto &s = self->texture_state[ *desc ];
            s.write_request_index = std::nullopt;
          }
        }
        self->staging_index_allocator.reset();
        self->metadata_write_region.clear();
        self->write_request_list.clear();
        used_on_gpu = std::move( self->used_on_gpu );
        self->used_on_gpu.clear();
        self->execution_pending = false;
      }
    }
  );
  execution_pending = true;
}

texture_pool::texture_pool( const texture_pool_create_info &ci ) :
  state( new state_type( ci ) ) {
}

texture_pool::views texture_pool::allocate(
  const sampler_pool::sampler_descriptor &sid,
  const image_pool::views &iid
) {
  std::lock_guard< std::mutex > lock( state->guard );
  return state->allocate( sid, iid );
}

texture_pool::texture_descriptor texture_pool::allocate(
  const sampler_pool::sampler_descriptor &sid,
  const image_pool::image_descriptor &iid
) {
  std::lock_guard< std::mutex > lock( state->guard );
  return state->allocate( sid, iid );
}


texture_pool::texture_descriptor texture_pool::allocate(
  const std::shared_ptr< sampler_t > &s,
  const std::shared_ptr< image_view_t > &linear
) {
  std::lock_guard< std::mutex > lock( state->guard );
  return state->allocate( s, linear );
}

std::pair< std::shared_ptr< image_view_t >, std::shared_ptr< sampler_t > > texture_pool::get(
  const texture_descriptor &desc
) const {
  std::lock_guard< std::mutex > lock( state->guard );
  return state->get( desc );
}

void texture_pool::operator()( command_buffer_recorder_t &rec ) {
  state->flush( rec );
}
void texture_pool::to_json( nlohmann::json &dest ) const {
  std::lock_guard< std::mutex > lock( state->guard );
  dest = nlohmann::json::object();
  dest[ "props" ] = get_props();
  dest[ "texture_state" ] = nlohmann::json::object();
  for( std::uint32_t i = 0u; i != state->index_allocator.get_tail(); ++i ) {
    if( state->texture_state[ i ].valid ) {
      auto temp = nlohmann::json::object();
      if( state->texture_state[ i ].write_request_index ) {
        temp[ "write_request_index" ] = *state->texture_state[ i ].write_request_index;
      }
      if( state->texture_state[ i ].view ) {
        temp[ "view" ] = *state->texture_state[ i ].view;
      }
      if( state->texture_state[ i ].sampler ) {
        temp[ "sampler" ] = *state->texture_state[ i ].sampler;
      }
      dest[ "texture_state" ][ std::to_string( i ) ] = temp;
    }
  }
  dest[ "write_request_buffer" ] = nlohmann::json::array();
  for( const auto &w: state->write_request_list ) {
    auto temp = nlohmann::json::object();
    temp[ "index" ] = w.index;
    if( w.view ) {
      temp[ "view" ] = *w.view;
    }
    if( w.sampler ) {
      temp[ "sampler" ] = *w.sampler;
    }
    dest[ "write_request_buffer" ].push_back( temp );
  }
  dest[ "execution_pending" ] = state->execution_pending;
}
void to_json( nlohmann::json &dest, const texture_pool &src ) {
  src.to_json( dest );
}

}

