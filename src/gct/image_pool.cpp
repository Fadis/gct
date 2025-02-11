#include "gct/exception.hpp"
#include <nlohmann/json.hpp>
#include <gct/device.hpp>
#include <gct/numeric_types.hpp>
#include <gct/shader_module_reflection.hpp>
#include <gct/descriptor_set_layout_create_info.hpp>
#include <gct/descriptor_set.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/sampler_create_info.hpp>
#include <gct/sampler.hpp>
#include <gct/buffer.hpp>
#include <gct/format.hpp>
#include <gct/image.hpp>
#include <gct/compute.hpp>
#include <gct/compute_pipeline.hpp>
#include <gct/pipeline_layout.hpp>
#include <gct/allocator.hpp>
#include <gct/mappable_buffer.hpp>
#include <gct/get_device.hpp>
#include <gct/create_image_from_file.hpp>
#include <gct/image_pool_create_info.hpp>
#include <gct/image_pool.hpp>

namespace gct {

image_pool::image_index_t image_pool::state_type::allocate_index() {
  const auto index = index_allocator.allocate();
  if( index >= image_state.size() ) {
    image_state.resize( index + 1u );
  }
  return index; 
}

void image_pool::state_type::release_index( image_pool::image_index_t index ) {
  if( index >= image_state.size() || !image_state[ index ].valid ) {
    return;
  }
  image_state[ index ].valid = false;
  index_allocator.release( index );
}

image_pool::views image_pool::state_type::allocate() {

  const image_index_t normalized_index = allocate_index();
  const image_index_t srgb_index = allocate_index();

  auto b = props.allocator_set.allocator->create_buffer(
    16 * 16 * 4,
    vk::BufferUsageFlagBits::eTransferSrc,
    VMA_MEMORY_USAGE_CPU_TO_GPU
  );
  
  {
    auto mapped = b->map< std::uint8_t >();
    std::fill( mapped.begin(), mapped.end(), 255u );
  }

  numeric_type_t nt;
  nt.composite = numeric_composite_type_t::vector;
  nt.rows = 4u;
  nt.component = numeric_component_type_t::int_;
  nt.depth = 8u;
  nt.sign = false;
  vk::Format primary_format = vk::Format::eUndefined;
  std::vector< vk::Format > secondary_formats;
  const auto attr = integer_attribute_t::normalized;
  constexpr static std::array< integer_attribute_t, 1u > attrs{
    integer_attribute_t::normalized
  };
  for( const auto a: attrs ) {
    nt.attr = a;
    const auto &formats = get_compatible_format( nt );
    if( formats.empty() ) {
      if( a == attr ) throw -1;
    }
    else {
      secondary_formats.push_back( formats[ 0 ] );
      if( a == attr ) {
        primary_format = formats[ 0 ];
      }
    }
  }

  std::shared_ptr< image_t > i = props.allocator_set.allocator->create_image(
    image_create_info_t()
      .set_basic(
        vk::ImageCreateInfo()
          .setImageType( vk::ImageType::e2D )
          .setFormat( primary_format )
          .setExtent( { 16, 16, 1 } )
          .setMipLevels( 1 )
          .setArrayLayers( 1 )
          .setSamples( vk::SampleCountFlagBits::e1 )
          .setTiling( vk::ImageTiling::eOptimal )
          .setUsage( vk::ImageUsageFlagBits::eTransferSrc | vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled | vk::ImageUsageFlagBits::eStorage )
          .setSharingMode( vk::SharingMode::eExclusive )
          .setQueueFamilyIndexCount( 0 )
          .setPQueueFamilyIndices( nullptr )
          .setInitialLayout( vk::ImageLayout::eUndefined )
      )
      .add_format( secondary_formats.begin(), secondary_formats.end() ),
      VMA_MEMORY_USAGE_GPU_ONLY
  );

  std::shared_ptr< image_view_t > normalized;
  std::shared_ptr< image_view_t > srgb;

  for( auto f: i->get_props().get_format() ) {
    auto view = i->get_view(
      image_view_create_info_t()
        .set_basic(
          vk::ImageViewCreateInfo()
            .setSubresourceRange(
              vk::ImageSubresourceRange()
                .setAspectMask( vk::ImageAspectFlagBits::eColor )
                .setBaseMipLevel( 0 )
                .setLevelCount( i->get_props().get_basic().mipLevels )
                .setBaseArrayLayer( 0 )
                .setLayerCount( i->get_props().get_basic().arrayLayers )
            )
            .setViewType( to_image_view_type( i->get_props().get_basic().imageType, i->get_props().get_basic().arrayLayers, false ) )
            .setFormat( f )
        )
        .rebuild_chain()
    );
    if( is_normalized( f ) ) {
      normalized = view;
    }
    else if( is_srgb( f ) ) {
      srgb = view;
    }
  }

  write_request_list.push_back(
    write_request()
      .set_index( normalized_index )
      .set_mipmap( false )
      .set_staging_buffer( b )
      .set_final_image( normalized )
      .set_layout( props.layout )
  );
  image_state[ normalized_index ] =
    image_state_type()
      .set_valid( true )
      .set_image( normalized )
      .set_write_request_index( write_request_list.size() - 1u );

  image_descriptor normalized_desc(
    new image_index_t( normalized_index ),
    [self=shared_from_this()]( const image_index_t *p ) {
      if( p ) {
        self->release( *p );
        delete p;
      }
    }
  );
  used_on_gpu.push_back( normalized_desc );

  image_descriptor srgb_desc;
  if( srgb ) {
    image_state[ srgb_index ] =
      image_state_type()
        .set_valid( true )
        .set_image( srgb )
        .set_write_request_index( write_request_list.size() - 1u );
 
    srgb_desc = image_descriptor(
      new image_index_t( srgb_index ),
      [self=shared_from_this()]( const image_index_t *p ) {
        if( p ) {
          self->release( *p );
          delete p;
        }
      }
    );
    used_on_gpu.push_back( srgb_desc );
  }
  return views()
    .set_normalized( normalized_desc )
    .set_srgb( srgb_desc );
}

image_pool::views image_pool::state_type::allocate(
  const image_load_info &ci
) {
  if( execution_pending ) {
    throw exception::runtime_error( "image_pool::state_type::allocate : last execution is not completed yet", __FILE__, __LINE__ );
  }
  const image_index_t normalized_index = allocate_index();
  image_index_t srgb_index = 0u;
  image_index_t linear_index = 0u;

  auto &device = get_device( *props.allocator_set.allocator );

  const auto [b,i,l] = create_image_from_file(
    props.allocator_set.allocator,
    ci.filename,
    ci.usage,
    ci.mipmap,
    ci.attr,
    ci.max_channels_per_layer,
    !( props.enable_linear && ci.enable_linear )
  );

  std::shared_ptr< image_view_t > linear;
  std::shared_ptr< image_view_t > normalized;
  std::shared_ptr< image_view_t > srgb;

  for( auto f: i->get_props().get_format() ) {
    auto view = i->get_view(
      image_view_create_info_t()
        .set_basic(
          vk::ImageViewCreateInfo()
            .setSubresourceRange(
              vk::ImageSubresourceRange()
                .setAspectMask( vk::ImageAspectFlagBits::eColor )
                .setBaseMipLevel( 0 )
                .setLevelCount( i->get_props().get_basic().mipLevels )
                .setBaseArrayLayer( 0 )
                .setLayerCount( i->get_props().get_basic().arrayLayers )
            )
            .setViewType( to_image_view_type( i->get_props().get_basic().imageType, i->get_props().get_basic().arrayLayers, false ) )
            .setFormat( f )
        )
        .rebuild_chain()
    );
    if( i->get_props().get_profile().gamma == color_gamma::srgb ) {
      if( is_normalized( f ) ) {
        normalized = view;
      }
      else if( is_srgb( f ) ) {
        srgb = view;
      }
    }
    else {
      normalized = view;
    }
  }

  const unsigned int channels = format_to_channels( i->get_props().get_basic().format );
  const bool enable_linear =
    props.enable_linear &&
    ci.enable_linear &&
    i->get_props().get_basic().arrayLayers == 1 &&
    ( channels == 3u || channels == 4u );

  if( enable_linear ) {
    linear_index = allocate_index();
    linear =
      l->get_view(
        image_view_create_info_t()
          .set_basic(
            vk::ImageViewCreateInfo()
              .setSubresourceRange(
                vk::ImageSubresourceRange()
                  .setAspectMask( vk::ImageAspectFlagBits::eColor )
                  .setBaseMipLevel( 0 )
                  .setLevelCount( l->get_props().get_basic().mipLevels )
                  .setBaseArrayLayer( 0 )
                  .setLayerCount( l->get_props().get_basic().arrayLayers )
              )
              .setViewType( to_image_view_type( l->get_props().get_basic().imageType, l->get_props().get_basic().arrayLayers, false ) )
              .setFormat( l->get_props().get_basic().format )
          )
          .rebuild_chain()
      );
  }

  write_request_list.push_back(
    write_request()
      .set_index( normalized_index )
      .set_mipmap( ci.mipmap )
      .set_staging_buffer( b )
      .set_final_image( normalized )
      .set_layout( ci.layout ? *ci.layout : props.layout )
  );
  image_state[ normalized_index ] =
    image_state_type()
      .set_valid( true )
      .set_image( normalized )
      .set_write_request_index( write_request_list.size() - 1u );

  image_descriptor normalized_desc(
    new image_index_t( normalized_index ),
    [self=shared_from_this()]( const image_index_t *p ) {
      if( p ) {
        self->release( *p );
        delete p;
      }
    }
  );
  used_on_gpu.push_back( normalized_desc );

  image_descriptor srgb_desc;
  if( srgb ) {
    srgb_index = allocate_index();
    image_state[ srgb_index ] =
      image_state_type()
        .set_valid( true )
        .set_image( srgb )
        .set_write_request_index( write_request_list.size() - 1u );
 
    srgb_desc = image_descriptor(
      new image_index_t( srgb_index ),
      [self=shared_from_this()]( const image_index_t *p ) {
        if( p ) {
          self->release( *p );
          delete p;
        }
      }
    );
    used_on_gpu.push_back( srgb_desc );
  }
  image_descriptor linear_desc;
  if( enable_linear ) {
    rgb_to_xyz_request_list.push_back(
      rgb_to_xyz_request()
        .set_rgb( normalized_index )
        .set_xyz( linear_index )
        .set_rgb_image( normalized )
        .set_xyz_image( linear )
        .set_layout( ci.layout ? *ci.layout : props.layout )
    );
    image_state[ linear_index ] =
      image_state_type()
        .set_valid( true )
        .set_image( linear )
        .set_write_request_index( write_request_list.size() - 1u );
 
    linear_desc = image_descriptor(
      new image_index_t( linear_index ),
      [self=shared_from_this()]( const image_index_t *p ) {
        if( p ) {
          self->release( *p );
          delete p;
        }
      }
    );
    used_on_gpu.push_back( linear_desc );
  }

  return views()
    .set_normalized( normalized_desc )
    .set_srgb( srgb_desc )
    .set_linear( linear_desc );
}

image_pool::views image_pool::state_type::allocate(
  const image_allocate_info &alloc
) {
  if( execution_pending ) {
    throw exception::runtime_error( "image_pool::state_type::allocate : last execution is not completed yet", __FILE__, __LINE__ );
  }
  image_index_t linear_index = allocate_index();

  auto &device = get_device( *props.allocator_set.allocator );

  std::shared_ptr< image_t > i = props.allocator_set.allocator->create_image(
    alloc.create_info,
    VMA_MEMORY_USAGE_GPU_ONLY
  );

  const std::shared_ptr< image_view_t > linear = i->get_view( alloc.range );

  image_state[ linear_index ] =
    image_state_type()
      .set_valid( true )
      .set_image( linear );
  convert_request_list.push_back(
    convert_request()
      .set_index( linear_index )
      .set_image( linear )
      .set_layout( alloc.layout ? *alloc.layout : props.layout )
  );

  const image_descriptor linear_desc = image_descriptor(
      new image_index_t( linear_index ),
      [self=shared_from_this()]( const image_index_t *p ) {
        if( p ) {
          self->release( *p );
          delete p;
        }
      }
    );
  used_on_gpu.push_back( linear_desc );

  return views()
    .set_linear( linear_desc );
}


void image_pool::state_type::release( image_index_t index ) {
  image_state_type removed;
  {
    std::lock_guard< std::mutex > lock( guard );
    if( image_state.size() <= index || !image_state[ index ].valid ) {
      return;
    }
    removed = image_state[ index ];
    image_state[ index ] = image_state_type();
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

void image_pool::state_type::flush( command_buffer_recorder_t &rec ) {
  if( execution_pending ) {
    return;
  }
  for( const auto &req: write_request_list ) {
    rec.buffer_to_image( req.mipmap, req.staging_buffer, req.final_image->get_factory() );
  }
  for( const auto &req: write_request_list ) {
    rec.convert_image( req.final_image->get_factory(), vk::ImageLayout::eGeneral );
  }
  if( props.enable_linear && props.external_descriptor_set.find( props.image_descriptor_set_id ) != props.external_descriptor_set.end() ) {
    std::vector< write_descriptor_set_t > updates;
    const auto target = (*props.external_descriptor_set[ props.image_descriptor_set_id ])[ props.descriptor_name ];
    for( const auto &req: write_request_list ) {
      updates.push_back(
        write_descriptor_set_t()
          .set_basic(
            vk::WriteDescriptorSet( target )
              .setDstArrayElement( req.index )
              .setDescriptorCount( 1u )
          )
          .add_image( req.final_image, vk::ImageLayout::eGeneral )
          .set_index( req.index )
      );
    }
    for( const auto &req: rgb_to_xyz_request_list ) {
      updates.push_back(
        write_descriptor_set_t()
          .set_basic(
            vk::WriteDescriptorSet( target )
              .setDstArrayElement( req.xyz )
              .setDescriptorCount( 1u )
          )
          .add_image( req.xyz_image, vk::ImageLayout::eGeneral )
          .set_index( req.xyz )
      );
    }
    for( const auto &req: convert_request_list ) {
      updates.push_back(
        write_descriptor_set_t()
          .set_basic(
            vk::WriteDescriptorSet( target )
              .setDstArrayElement( req.index )
              .setDescriptorCount( 1u )
          )
          .add_image( req.image, vk::ImageLayout::eGeneral )
          .set_index( req.index )
      );
    }
    props.external_descriptor_set[ props.image_descriptor_set_id ]->update(
      std::move( updates )
    );
  }
  for( const auto &req: rgb_to_xyz_request_list ) {
    rec.convert_image( req.xyz_image->get_factory(), vk::ImageLayout::eGeneral );
    rec.barrier( {}, { req.rgb_image->get_factory() } );
  }
  for( const auto &req: rgb_to_xyz_request_list ) {
    //rec.blit( req.rgb_image->get_factory(), req.xyz_image->get_factory() );
    rec.fill( req.xyz_image->get_factory(), std::array< float, 4u >{ 1.0, 0.0, 0.0, 1.0 } );
    rec.barrier( {}, { req.xyz_image->get_factory() } );
  }
  for( const auto &req: rgb_to_xyz_request_list ) {
  //if( !rgb_to_xyz_request_list.empty() ) {
  //  const auto &req = rgb_to_xyz_request_list[ 0 ];
    const auto &color_prof =  req.rgb_image->get_factory()->get_props().get_profile();
    const auto mat = props.csmat.from.find( color_prof.space );
    auto shader = rgba8;
    if( req.rgb_image->get_props().get_basic().format == vk::Format::eR8G8B8A8Unorm ) {
      shader = rgba8;
    }
    else if( req.rgb_image->get_props().get_basic().format == vk::Format::eR16G16B16A16Unorm ) {
      shader = rgba16;
    }
    else if( req.rgb_image->get_props().get_basic().format == vk::Format::eR16G16B16A16Sfloat ) {
      shader = rgba16f;
    }
    else if( req.rgb_image->get_props().get_basic().format == vk::Format::eR32G32B32A32Sfloat ) {
      shader = rgba32f;
    }
    else {
      throw -1;
    }
    const auto pc_head = shader->get_reflection().get_push_constant_member_pointer( "PushConstants" );
    const auto from = pc_head.get_maybe( "from" );
    const auto to = pc_head.get_maybe( "to" );
    const auto gamma = pc_head.get_maybe( "gamma" );
    const auto oetf = pc_head.get_maybe( "oetf" );
    const auto csmat_p = pc_head.get_maybe( "csmat" );
    const auto pc_size = std::max(
        std::max(
          std::max(
            std::max(
              from ? ( from->get_offset() + from->get_aligned_size() ) : 0u,
              to ? ( to->get_offset() + to->get_aligned_size() ) : 0u
            ),
            gamma ? ( gamma->get_offset() + gamma->get_aligned_size() ) : 0u
          ),
          oetf ? ( oetf->get_offset() + oetf->get_aligned_size() ) : 0u
        ),
        csmat_p ? ( csmat_p->get_offset() + csmat_p->get_aligned_size() ) : 0u
      );
    std::vector< std::uint8_t > pc( pc_size, std::uint8_t( 0u ) );
    if( from ) {
      pc.data()->*(*from) = req.rgb;
    }
    if( to ) {
      pc.data()->*(*to) = req.xyz;
    }
    if( gamma ) {
      pc.data()->*(*gamma) = get_gamma_value( color_prof.gamma );
    }
    if( oetf ) {
      pc.data()->*(*oetf) = std::uint32_t( color_prof.gamma );
    }
    if( csmat_p ) {
      pc.data()->*(*csmat_p) = *mat->second;
    }
    rec->pushConstants(
        **shader->get_pipeline()->get_props().get_layout(),
        vk::ShaderStageFlagBits::eCompute,
        0u,
        pc.size(),
        reinterpret_cast< void* >( pc.data() )
    );
    rec.barrier( {}, { req.xyz_image->get_factory() } );
    (*shader)(
      rec,
      0u,
      req.xyz_image->get_factory()->get_props().get_basic().extent.width,
      req.xyz_image->get_factory()->get_props().get_basic().extent.height,
      1u
    );
    //rec.create_mipmap( req.xyz_image->get_factory(), vk::ImageLayout::eGeneral, props.layout );
  }
  for( const auto &req: rgb_to_xyz_request_list ) {
    rec.barrier( {}, { req.xyz_image->get_factory() } );
    rec.convert_image( req.xyz_image->get_factory(), req.layout );
  }
  for( const auto &req: write_request_list ) {
    rec.convert_image( req.final_image->get_factory(), req.layout );
  }
  for( const auto &req: convert_request_list ) {
    rec.convert_image( req.image->get_factory(), req.layout );
  }
  for( const auto &req: dump_request_list ) {
    rec.dump_image(
      props.allocator_set.allocator,
      req.first,
      req.second.filename,
      req.second.mipmap,
      req.second.layer,
      req.second.depth
    );
  }
  rec.on_executed(
    [self=shared_from_this()]( vk::Result result ) {
      std::vector< image_descriptor > used_on_gpu;
      {
        std::lock_guard< std::mutex > lock( self->guard );
        for( const auto &desc: self->used_on_gpu ) {
          if( self->image_state.size() > *desc && self->image_state[ *desc ].valid ) {
            auto &s = self->image_state[ *desc ];
            s.write_request_index = std::nullopt;
          }
        }
        self->write_request_list.clear();
        self->rgb_to_xyz_request_list.clear();
        self->convert_request_list.clear();
        used_on_gpu = std::move( self->used_on_gpu );
        self->used_on_gpu.clear();
        self->execution_pending = false;
      }
    }
  );
  execution_pending = true;
}

std::shared_ptr< image_view_t > image_pool::state_type::get(
  const image_descriptor &desc
) const {
  if( image_state.size() <= *desc || !image_state[ *desc ].valid ) {
    throw exception::invalid_argument( "image_pool::get : No such image" );
  }
  return image_state[ *desc ].image;
}

void image_pool::state_type::dump( const image_descriptor &desc, const image_dump_info &ci ) {
  if( image_state.size() <= *desc || !image_state[ *desc ].valid ) {
    throw exception::invalid_argument( "image_pool::dump : No such image" );
  }
  dump_request_list.push_back( std::make_pair( image_state[ *desc ].image->get_factory(), ci ) );
}

image_pool::state_type::state_type( const image_pool_create_info &ci ) :
  props( ci ),
  index_allocator( linear_allocator_create_info().set_max( ci.max_image_count ) ) {
  if( props.enable_linear ) {
    rgba8.reset( new gct::compute(
      gct::compute_create_info()
        .set_allocator_set( props.allocator_set )
        .set_shader( props.rgba8_shader )
        .set_swapchain_image_count( 1u )
        .set_descriptor_set_layout( props.descriptor_set_layout )
        .set_external_descriptor_set( props.external_descriptor_set )
        .set_resources( props.resources )
        .add_resource( { props.matrix_buffer_name, props.matrix_pool } )
        .set_ignore_unused_descriptor( true )
    ) );
    rgba16.reset( new gct::compute(
      gct::compute_create_info()
        .set_allocator_set( props.allocator_set )
        .set_shader( props.rgba16_shader )
        .set_swapchain_image_count( 1u )
        .set_descriptor_set_layout( props.descriptor_set_layout )
        .set_external_descriptor_set( props.external_descriptor_set )
        .set_resources( props.resources )
        .add_resource( { props.matrix_buffer_name, props.matrix_pool } )
        .set_ignore_unused_descriptor( true )
    ) );
    rgba16f.reset( new gct::compute(
      gct::compute_create_info()
        .set_allocator_set( props.allocator_set )
        .set_shader( props.rgba16f_shader )
        .set_swapchain_image_count( 1u )
        .set_descriptor_set_layout( props.descriptor_set_layout )
        .set_external_descriptor_set( props.external_descriptor_set )
        .set_resources( props.resources )
        .add_resource( { props.matrix_buffer_name, props.matrix_pool } )
        .set_ignore_unused_descriptor( true )
    ) );
    rgba32f.reset( new gct::compute(
      gct::compute_create_info()
        .set_allocator_set( props.allocator_set )
        .set_shader( props.rgba32f_shader )
        .set_swapchain_image_count( 1u )
        .set_descriptor_set_layout( props.descriptor_set_layout )
        .set_external_descriptor_set( props.external_descriptor_set )
        .set_resources( props.resources )
        .add_resource( { props.matrix_buffer_name, props.matrix_pool } )
        .set_ignore_unused_descriptor( true )
    ) );
  }
}

image_pool::image_pool( const image_pool_create_info &ci ) :
  state( new state_type( ci ) ) {
}

image_pool::views image_pool::allocate(
  const image_load_info &ci
) {
  std::lock_guard< std::mutex > lock( state->guard );
  return state->allocate( ci );
}

image_pool::views image_pool::allocate() {
  std::lock_guard< std::mutex > lock( state->guard );
  return state->allocate();
}

std::shared_ptr< image_view_t > image_pool::get(
  const image_descriptor &desc
) const {
  std::lock_guard< std::mutex > lock( state->guard );
  return state->get( desc );
}

void image_pool::dump( const image_descriptor &desc, const image_dump_info &ci ) {
  std::lock_guard< std::mutex > lock( state->guard );
  state->dump( desc, ci );
}

void image_pool::operator()( command_buffer_recorder_t &rec ) {
  std::lock_guard< std::mutex > lock( state->guard );
  state->flush( rec );
}

void image_pool::to_json( nlohmann::json &dest ) const {
  std::lock_guard< std::mutex > lock( state->guard );
  dest = nlohmann::json::object();
  dest[ "props" ] = get_props();
  dest[ "sampler_state" ] = nlohmann::json::object();
  for( std::uint32_t i = 0u; i != state->index_allocator.get_tail(); ++i ) {
    if( state->image_state[ i ].valid ) {
      auto temp = nlohmann::json::object();
      if( state->image_state[ i ].write_request_index ) {
        temp[ "write_request_index" ] = *state->image_state[ i ].write_request_index;
      }
      if( state->image_state[ i ].image ) {
        temp[ "image" ] = *state->image_state[ i ].image;
      }
      dest[ "sampler_state" ][ std::to_string( i ) ] = temp;
    }
  }
  dest[ "write_request_buffer" ] = nlohmann::json::array();
  for( const auto &w: state->write_request_list ) {
    auto temp = nlohmann::json::object();
    temp[ "index" ] = w.index;
    if( w.staging_buffer ) {
      temp[ "staging_buffer" ] = *w.staging_buffer;
    }
    if( w.final_image ) {
      temp[ "final_image" ] = *w.final_image;
    }
    dest[ "write_request_buffer" ].push_back( temp );
  }
  dest[ "execution_pending" ] = state->execution_pending;
}


void to_json( nlohmann::json &dest, const image_pool &src ) {
  src.to_json( dest );
}
}
