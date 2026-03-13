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
#include <gct/image.hpp>
#include <gct/image_create_info.hpp>
#include <gct/simplify_buffer_copy.hpp>
#include <gct/format.hpp>

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
  return allocate(
    vk::ImageUsageFlagBits::eTransferSrc |
    vk::ImageUsageFlagBits::eTransferDst |
    vk::ImageUsageFlagBits::eSampled |
    vk::ImageUsageFlagBits::eStorage
  );
}

image_pool::views image_pool::state_type::allocate(
  vk::ImageUsageFlags usage
) {

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
          .setUsage(
            usage
          )
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

  if( metadata_member_pointer ) {
    const auto metadata_aligned_size = metadata_member_pointer->get_stride();
    if( normalized ) {
      const auto &color_prof = normalized->get_factory()->get_props().get_profile();
      const auto from_mat = props.csmat.from.find( color_prof.space )->second;
      const auto to_mat = props.csmat.to.find( color_prof.space )->second;
      std::vector< std::uint8_t > temp( metadata_aligned_size, 0u );
      temp.data()->*(*metadata_member_pointer)[ 0 ][ "color_profile" ][ "space" ] = std::uint32_t( color_prof.space );
      temp.data()->*(*metadata_member_pointer)[ 0 ][ "color_profile" ][ "gamma" ] = std::uint32_t( color_prof.gamma );
      temp.data()->*(*metadata_member_pointer)[ 0 ][ "color_profile" ][ "max_intensity" ] = 1.0f;
      temp.data()->*(*metadata_member_pointer)[ 0 ][ "color_space_matrix" ][ "from" ] = *from_mat;
      temp.data()->*(*metadata_member_pointer)[ 0 ][ "color_space_matrix" ][ "to" ] = *to_mat;
      const auto staging_index = staging_index_allocator.allocate( 1u );
      {
        auto mapped = staging_metadata_buffer->map< std::uint8_t >();
        std::copy( temp.begin(), temp.end(), std::next( mapped.begin(), staging_index * metadata_aligned_size ) );
      }
      metadata_write_region.push_back(
        vk::BufferCopy()
          .setSrcOffset( staging_index * metadata_aligned_size )
          .setDstOffset( normalized_index * metadata_aligned_size )
          .setSize( metadata_aligned_size )
      );
    }
    if( srgb )  {
      const auto &color_prof = srgb->get_factory()->get_props().get_profile();
      const auto from_mat = props.csmat.from.find( color_prof.space )->second;
      const auto to_mat = props.csmat.to.find( color_prof.space )->second;
      std::vector< std::uint8_t > temp( metadata_aligned_size, 0u );
      temp.data()->*(*metadata_member_pointer)[ 0 ][ "color_profile" ][ "space" ] = std::uint32_t( color_prof.space );
      temp.data()->*(*metadata_member_pointer)[ 0 ][ "color_profile" ][ "gamma" ] = std::uint32_t( color_prof.gamma );
      temp.data()->*(*metadata_member_pointer)[ 0 ][ "color_profile" ][ "max_intensity" ] = 1.0f;
      temp.data()->*(*metadata_member_pointer)[ 0 ][ "color_space_matrix" ][ "from" ] = *from_mat;
      temp.data()->*(*metadata_member_pointer)[ 0 ][ "color_space_matrix" ][ "to" ] = *to_mat;
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

  const auto cis = create_image_from_file(
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

  const auto normalized_format = remove_srgb( cis.nonlinear_image->get_props().get_basic().format );
  const auto srgb_format = add_srgb( cis.nonlinear_image->get_props().get_basic().format );
  const auto formats = cis.nonlinear_image->get_props().get_format();

  if( std::find( formats.begin(), formats.end(), normalized_format ) != formats.end() ) {
    const auto fp = device.get_format_properties( normalized_format );
    const bool compatible = is_available_for(
      cis.nonlinear_image->get_props().get_basic().usage,
      cis.nonlinear_image->get_props().get_basic().tiling == vk::ImageTiling::eOptimal ?
      fp.get_basic().optimalTilingFeatures :
      fp.get_basic().linearTilingFeatures
    );
    if( compatible ) {
      normalized = cis.nonlinear_image->get_view(
        image_view_create_info_t()
          .set_basic(
            vk::ImageViewCreateInfo()
              .setSubresourceRange(
                vk::ImageSubresourceRange()
                  .setAspectMask( vk::ImageAspectFlagBits::eColor )
                  .setBaseMipLevel( 0 )
                  .setLevelCount( cis.nonlinear_image->get_props().get_basic().mipLevels )
                  .setBaseArrayLayer( 0 )
                  .setLayerCount( cis.nonlinear_image->get_props().get_basic().arrayLayers )
              )
              .setViewType( to_image_view_type( cis.nonlinear_image->get_props().get_basic().imageType, cis.nonlinear_image->get_props().get_basic().arrayLayers, false ) )
              .setFormat( normalized_format )
          )
          .rebuild_chain()
      );
    }
  }
  if( normalized_format != srgb_format && std::find( formats.begin(), formats.end(), srgb_format ) != formats.end() ) {
    const auto fp = device.get_format_properties( srgb_format );
    const bool compatible = is_available_for(
      cis.nonlinear_image->get_props().get_basic().usage,
      cis.nonlinear_image->get_props().get_basic().tiling == vk::ImageTiling::eOptimal ?
      fp.get_basic().optimalTilingFeatures :
      fp.get_basic().linearTilingFeatures
    );
    if( compatible ) {
      srgb = cis.nonlinear_image->get_view(
        image_view_create_info_t()
          .set_basic(
            vk::ImageViewCreateInfo()
              .setSubresourceRange(
                vk::ImageSubresourceRange()
                  .setAspectMask( vk::ImageAspectFlagBits::eColor )
                  .setBaseMipLevel( 0 )
                  .setLevelCount( cis.nonlinear_image->get_props().get_basic().mipLevels )
                  .setBaseArrayLayer( 0 )
                  .setLayerCount( cis.nonlinear_image->get_props().get_basic().arrayLayers )
              )
              .setViewType( to_image_view_type( cis.nonlinear_image->get_props().get_basic().imageType, cis.nonlinear_image->get_props().get_basic().arrayLayers, false ) )
              .setFormat( srgb_format )
          )
          .rebuild_chain()
      );
    }
  }

  if( !normalized ) {
    throw exception::runtime_error( "image_pool::state_type::allocate : Specified usage is not compatible to the format", __FILE__, __LINE__ );
  }

  const unsigned int channels = format_to_channels( cis.nonlinear_image->get_props().get_basic().format );
  const bool enable_linear =
    props.enable_linear &&
    ci.enable_linear &&
    cis.nonlinear_image->get_props().get_basic().arrayLayers == 1 &&
    ( channels == 3u || channels == 4u );

  if( enable_linear && cis.linear_image ) {
    linear_index = allocate_index();
    linear =
      cis.linear_image->get_view(
        image_view_create_info_t()
          .set_basic(
            vk::ImageViewCreateInfo()
              .setSubresourceRange(
                vk::ImageSubresourceRange()
                  .setAspectMask( vk::ImageAspectFlagBits::eColor )
                  .setBaseMipLevel( 0 )
                  .setLevelCount( cis.linear_image->get_props().get_basic().mipLevels )
                  .setBaseArrayLayer( 0 )
                  .setLayerCount( cis.linear_image->get_props().get_basic().arrayLayers )
              )
              .setViewType( to_image_view_type( cis.linear_image->get_props().get_basic().imageType, cis.linear_image->get_props().get_basic().arrayLayers, false ) )
              .setFormat( cis.linear_image->get_props().get_basic().format )
          )
          .rebuild_chain()
      );
  }

  write_request_list.push_back(
    write_request()
      .set_index( normalized_index )
      .set_mipmap( ci.mipmap && cis.generate_mipmap )
      .set_staging_buffer( cis.staging_buffer )
      .set_final_image( normalized )
      .set_range( cis.copy_range )
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
  if( enable_linear && cis.linear_image ) {
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
  else if(
    cis.nonlinear_image->get_props().get_profile().gamma == gct::color_gamma::linear &&
    cis.nonlinear_image->get_props().get_profile().space == gct::color_space::cie_xyz
  ) {
    linear_desc = normalized_desc;
  }

  if( metadata_member_pointer ) {
    const auto metadata_aligned_size = metadata_member_pointer->get_stride();
    if( srgb ) {
      const auto &color_prof = srgb->get_factory()->get_props().get_profile();
      const auto from_mat = props.csmat.from.find( color_prof.space )->second;
      const auto to_mat = props.csmat.to.find( color_prof.space )->second;
      std::vector< std::uint8_t > temp( metadata_aligned_size, 0u );
      temp.data()->*(*metadata_member_pointer)[ 0 ][ "color_profile" ][ "space" ] = std::uint32_t( color_prof.space );
      temp.data()->*(*metadata_member_pointer)[ 0 ][ "color_profile" ][ "gamma" ] = std::uint32_t( color_prof.gamma );
      temp.data()->*(*metadata_member_pointer)[ 0 ][ "color_profile" ][ "max_intensity" ] = 1.0f;
      temp.data()->*(*metadata_member_pointer)[ 0 ][ "color_space_matrix" ][ "from" ] = *from_mat;
      temp.data()->*(*metadata_member_pointer)[ 0 ][ "color_space_matrix" ][ "to" ] = *to_mat;
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
      const auto from_mat = props.csmat.from.find( color_prof.space )->second;
      const auto to_mat = props.csmat.to.find( color_prof.space )->second;
      std::vector< std::uint8_t > temp( metadata_aligned_size, 0u );
      temp.data()->*(*metadata_member_pointer)[ 0 ][ "color_profile" ][ "space" ] = std::uint32_t( color_prof.space );
      temp.data()->*(*metadata_member_pointer)[ 0 ][ "color_profile" ][ "gamma" ] = std::uint32_t( color_prof.gamma );
      temp.data()->*(*metadata_member_pointer)[ 0 ][ "color_profile" ][ "max_intensity" ] = 1.0f;
      temp.data()->*(*metadata_member_pointer)[ 0 ][ "color_space_matrix" ][ "from" ] = *from_mat;
      temp.data()->*(*metadata_member_pointer)[ 0 ][ "color_space_matrix" ][ "to" ] = *to_mat;
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

  const std::shared_ptr< image_view_t > linear = i->get_view(
    alloc.range ? *alloc.range :
    subview_range()
      .set_mip_offset( 0u )
      .set_mip_count( alloc.create_info.get_basic().mipLevels )
      .set_layer_offset( 0u )
      .set_layer_count( alloc.create_info.get_basic().arrayLayers )
  );

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

  if( metadata_member_pointer ) {
    const auto metadata_aligned_size = metadata_member_pointer->get_stride();
    if( linear )  {
      const auto &color_prof = linear->get_factory()->get_props().get_profile();
      const auto from_mat = props.csmat.from.find( color_prof.space )->second;
      const auto to_mat = props.csmat.to.find( color_prof.space )->second;
      std::vector< std::uint8_t > temp( metadata_aligned_size, 0u );
      temp.data()->*(*metadata_member_pointer)[ 0 ][ "color_profile" ][ "space" ] = std::uint32_t( color_prof.space );
      temp.data()->*(*metadata_member_pointer)[ 0 ][ "color_profile" ][ "gamma" ] = std::uint32_t( color_prof.gamma );
      temp.data()->*(*metadata_member_pointer)[ 0 ][ "color_profile" ][ "max_intensity" ] = 1.0f;
      temp.data()->*(*metadata_member_pointer)[ 0 ][ "color_space_matrix" ][ "from" ] = *from_mat;
      temp.data()->*(*metadata_member_pointer)[ 0 ][ "color_space_matrix" ][ "to" ] = *to_mat;
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
    .set_linear( linear_desc );
}

image_pool::image_descriptor image_pool::state_type::allocate(
  const std::shared_ptr< image_view_t > &view
) {
  if( execution_pending ) {
    throw exception::runtime_error( "image_pool::state_type::allocate : last execution is not completed yet", __FILE__, __LINE__ );
  }
  auto &device = get_device( *props.allocator_set.allocator );
  const auto linear_index = allocate_index();
  image_descriptor linear_desc;
  image_state[ linear_index ] =
    image_state_type()
      .set_valid( true )
      .set_image( view );
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


  if( metadata_member_pointer ) {
    const auto metadata_aligned_size = metadata_member_pointer->get_stride();
    if( view )  {
      const auto &color_prof = view->get_factory()->get_props().get_profile();
      const auto from_mat = props.csmat.from.find( color_prof.space )->second;
      const auto to_mat = props.csmat.to.find( color_prof.space )->second;
      std::vector< std::uint8_t > temp( metadata_aligned_size, 0u );
      temp.data()->*(*metadata_member_pointer)[ 0 ][ "color_profile" ][ "space" ] = std::uint32_t( color_prof.space );
      temp.data()->*(*metadata_member_pointer)[ 0 ][ "color_profile" ][ "gamma" ] = std::uint32_t( color_prof.gamma );
      temp.data()->*(*metadata_member_pointer)[ 0 ][ "color_profile" ][ "max_intensity" ] = 1.0f;
      temp.data()->*(*metadata_member_pointer)[ 0 ][ "color_space_matrix" ][ "from" ] = *from_mat;
      temp.data()->*(*metadata_member_pointer)[ 0 ][ "color_space_matrix" ][ "to" ] = *to_mat;
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

  return linear_desc;
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
  for( const auto &req: write_request_list ) {
    if( req.range.empty() ) {
      rec.buffer_to_image( req.mipmap, req.staging_buffer, req.final_image->get_factory() );
    }
    else {
      rec.buffer_to_image( req.mipmap, req.staging_buffer, req.final_image->get_factory(), req.range );
    }
  }
  for( const auto &req: write_request_list ) {
    rec.convert_image( req.final_image->get_factory(), vk::ImageLayout::eGeneral );
  }
  if( props.enable_linear && props.external_descriptor_set.find( props.image_descriptor_set_id ) != props.external_descriptor_set.end() ) {
    std::vector< write_descriptor_set_t > updates;
    const auto target = (*props.external_descriptor_set[ props.image_descriptor_set_id ])[ props.descriptor_name ];
    for( const auto &req: write_request_list ) {
      if( req.final_image->get_factory()->get_props().get_basic().usage & vk::ImageUsageFlagBits::eStorage ) {
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
    }
    for( const auto &req: rgb_to_xyz_request_list ) {
      if( req.xyz_image->get_factory()->get_props().get_basic().usage & vk::ImageUsageFlagBits::eStorage ) {
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
    }
    for( const auto &req: convert_request_list ) {
      if( req.image->get_factory()->get_props().get_basic().usage & vk::ImageUsageFlagBits::eStorage ) {
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
    }
    props.external_descriptor_set[ props.image_descriptor_set_id ]->update(
      std::move( updates )
    );
  }
  for( const auto &req: rgb_to_xyz_request_list ) {
    rec.convert_image( req.xyz_image->get_factory(), vk::ImageLayout::eGeneral );
    rec.convert_image( req.rgb_image->get_factory(), vk::ImageLayout::eGeneral );
  }
  for( const auto &req: rgb_to_xyz_request_list ) {
    //rec.blit( req.rgb_image->get_factory(), req.xyz_image->get_factory() );
    rec.convert_image( req.xyz_image->get_factory(), vk::ImageLayout::eTransferDstOptimal );
    rec.fill( req.xyz_image->get_factory(), std::array< float, 4u >{ 1.0, 0.0, 0.0, 1.0 } );
    rec.convert_image( req.xyz_image->get_factory(), vk::ImageLayout::eGeneral );
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
        self->staging_index_allocator.reset();
        self->metadata_write_region.clear();
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
  staging_index_allocator( linear_allocator_create_info().set_max( ci.max_request_count ) ),
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
  if( std::filesystem::exists( props.rgba8_shader ) ) {
    reflection.reset(
      new shader_module_reflection_t( props.rgba8_shader )
    );
  }
  else if( std::filesystem::exists( props.rgba16_shader ) ) {
    reflection.reset(
      new shader_module_reflection_t( props.rgba16_shader )
    );
  }
  else if( std::filesystem::exists( props.rgba16f_shader ) ) {
    reflection.reset(
      new shader_module_reflection_t( props.rgba16f_shader )
    );
  }
  else if( std::filesystem::exists( props.rgba32f_shader ) ) {
    reflection.reset(
      new shader_module_reflection_t( props.rgba32f_shader )
    );
  }
  if( reflection ) {
    metadata_member_pointer = reflection->get_member_pointer_maybe( props.metadata_buffer_name, props.metadata_layout );
    if( metadata_member_pointer ) {
      const auto metadata_aligned_size = metadata_member_pointer->get_stride();
      metadata_buffer = props.allocator_set.allocator->create_buffer(
        metadata_aligned_size * props.max_image_count,
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

image_pool::views image_pool::allocate( vk::ImageUsageFlags usage ) {
  std::lock_guard< std::mutex > lock( state->guard );
  return state->allocate( usage );
}

image_pool::views image_pool::allocate(
  const image_allocate_info &ci
) {
  std::lock_guard< std::mutex > lock( state->guard );
  return state->allocate( ci );
}
image_pool::image_descriptor image_pool::allocate(
  const std::shared_ptr< image_view_t > &view
) {
  std::lock_guard< std::mutex > lock( state->guard );
  return state->allocate( view );
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
