#include <gct/format.hpp>
#include <gct/dds.hpp>

namespace gct::dds {

vk::Format ddsformat_to_vk_format( const pixel_format &f ) {
  if( int( f.flags ) & int( pixel_format_flag::DDPF_FOURCC ) ) {
    if( f.four_cc == pixel_format_four_cc::DXT1 ) {
      return vk::Format::eBc1RgbaSrgbBlock;
    }
    else if( f.four_cc == pixel_format_four_cc::DXT2 ) {
      throw std::runtime_error( "DXT2 is not supported." );
    }
    else if( f.four_cc == pixel_format_four_cc::DXT3 ) {
      return vk::Format::eBc2SrgbBlock;
    }
    else if( f.four_cc == pixel_format_four_cc::DXT4 ) {
      throw std::runtime_error( "DXT4 is not supported." );
    }
    else if( f.four_cc == pixel_format_four_cc::DXT5 ) {
      return vk::Format::eBc3SrgbBlock;
    }
    else if( f.four_cc == pixel_format_four_cc::DX10 ) {
      return vk::Format::eUndefined;
    }
  }
  else if( int( f.flags ) & int( pixel_format_flag::DDPF_RGB ) ) {
    if( int( f.flags ) & int( pixel_format_flag::DDPF_ALPHAPIXELS ) ) {
      if( f.rgb_bit_count == 32 ) {
        if( f.r_bit_mask == 0xFF000000u && f.g_bit_mask == 0x00FF0000u && f.b_bit_mask == 0x0000FF00u && f.a_bit_mask == 0x000000FFu ) {
          return vk::Format::eR8G8B8A8Srgb;
        }
        else if( f.r_bit_mask == 0x0000FF00u && f.g_bit_mask == 0x00FF0000u && f.b_bit_mask == 0xFF000000u && f.a_bit_mask == 0x000000FFu ) {
          return vk::Format::eB8G8R8A8Srgb;
        }
      }
    }
    else {
      if( f.rgb_bit_count == 24 ) {
        if( f.r_bit_mask == 0xFF0000u && f.g_bit_mask == 0x00FF00u && f.b_bit_mask == 0x0000FFu ) {
          return vk::Format::eR8G8B8Srgb;
        }
        else if( f.r_bit_mask == 0x0000FFu && f.g_bit_mask == 0x00FF00u && f.b_bit_mask == 0xFF0000u ) {
          return vk::Format::eB8G8R8Srgb;
        }
      }
    }
  }
  return vk::Format::eUndefined;
}

vk::Format ddsformat_to_vk_format( dxgi_format f ) {
  if( f == dxgi_format::DXGI_FORMAT_UNKNOWN ) {
    return vk::Format::eUndefined;
  }
  if( f == dxgi_format::DXGI_FORMAT_R32G32B32A32_TYPELESS ) {
    return vk::Format::eUndefined;
  }
  if( f == dxgi_format::DXGI_FORMAT_R32G32B32A32_FLOAT ) {
    return vk::Format::eR32G32B32A32Sfloat;
  }
  if( f == dxgi_format::DXGI_FORMAT_R32G32B32A32_UINT ) {
    return vk::Format::eR32G32B32A32Uint;
  }
  if( f == dxgi_format::DXGI_FORMAT_R32G32B32A32_SINT ) {
    return vk::Format::eR32G32B32A32Sint;
  }
  if( f == dxgi_format::DXGI_FORMAT_R32G32B32_TYPELESS ) {
    return vk::Format::eUndefined;
  }
  if( f == dxgi_format::DXGI_FORMAT_R32G32B32_FLOAT ) {
    return vk::Format::eR32G32B32Sfloat;
  }
  if( f == dxgi_format::DXGI_FORMAT_R32G32B32_UINT ) {
    return vk::Format::eR32G32B32Uint;
  }
  if( f == dxgi_format::DXGI_FORMAT_R32G32B32_SINT ) {
    return vk::Format::eR32G32B32Sint;
  }
  if( f == dxgi_format::DXGI_FORMAT_R16G16B16A16_TYPELESS ) {
    return vk::Format::eUndefined;
  }
  if( f == dxgi_format::DXGI_FORMAT_R16G16B16A16_FLOAT ) {
    return vk::Format::eR16G16B16A16Sfloat;
  }
  if( f == dxgi_format::DXGI_FORMAT_R16G16B16A16_UNORM ) {
    return vk::Format::eR16G16B16A16Unorm;
  }
  if( f == dxgi_format::DXGI_FORMAT_R16G16B16A16_UINT ) {
    return vk::Format::eR16G16B16A16Uint;
  }
  if( f == dxgi_format::DXGI_FORMAT_R16G16B16A16_SNORM ) {
    return vk::Format::eR16G16B16A16Snorm;
  }
  if( f == dxgi_format::DXGI_FORMAT_R16G16B16A16_SINT ) {
    return vk::Format::eR16G16B16A16Sint;
  }
  if( f == dxgi_format::DXGI_FORMAT_R32G32_TYPELESS ) {
    return vk::Format::eUndefined;
  }
  if( f == dxgi_format::DXGI_FORMAT_R32G32_FLOAT ) {
    return vk::Format::eR32G32Sfloat;
  }
  if( f == dxgi_format::DXGI_FORMAT_R32G32_UINT ) {
    return vk::Format::eR32G32Uint;
  }
  if( f == dxgi_format::DXGI_FORMAT_R32G32_SINT ) {
    return vk::Format::eR32G32Sint;
  }
  if( f == dxgi_format::DXGI_FORMAT_R32G8X24_TYPELESS ) {
    return vk::Format::eUndefined;
  }
  if( f == dxgi_format::DXGI_FORMAT_D32_FLOAT_S8X24_UINT ) {
    return vk::Format::eD32SfloatS8Uint;
  }
  if( f == dxgi_format::DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS ) {
    return vk::Format::eUndefined;
  }
  if( f == dxgi_format::DXGI_FORMAT_X32_TYPELESS_G8X24_UINT ) {
    return vk::Format::eUndefined;
  }
  if( f == dxgi_format::DXGI_FORMAT_R10G10B10A2_TYPELESS ) {
    return vk::Format::eUndefined;
  }
  if( f == dxgi_format::DXGI_FORMAT_R10G10B10A2_UNORM ) {
    return vk::Format::eA2B10G10R10UnormPack32;
  }
  if( f == dxgi_format::DXGI_FORMAT_R10G10B10A2_UINT ) {
    return vk::Format::eA2B10G10R10UintPack32;
  }
  if( f == dxgi_format::DXGI_FORMAT_R11G11B10_FLOAT ) {
    return vk::Format::eB10G11R11UfloatPack32;
  }
  if( f == dxgi_format::DXGI_FORMAT_R8G8B8A8_TYPELESS ) {
    return vk::Format::eUndefined;
  }
  if( f == dxgi_format::DXGI_FORMAT_R8G8B8A8_UNORM ) {
    return vk::Format::eR8G8B8A8Unorm;
  }
  if( f == dxgi_format::DXGI_FORMAT_R8G8B8A8_UNORM_SRGB ) {
    return vk::Format::eR8G8B8A8Srgb;
  }
  if( f == dxgi_format::DXGI_FORMAT_R8G8B8A8_UINT ) {
    return vk::Format::eR8G8B8A8Uint;
  }
  if( f == dxgi_format::DXGI_FORMAT_R8G8B8A8_SNORM ) {
    return vk::Format::eR8G8B8A8Snorm;
  }
  if( f == dxgi_format::DXGI_FORMAT_R8G8B8A8_SINT ) {
    return vk::Format::eR8G8B8A8Sint;
  }
  if( f == dxgi_format::DXGI_FORMAT_R16G16_TYPELESS ) {
    return vk::Format::eUndefined;
  }
  if( f == dxgi_format::DXGI_FORMAT_R16G16_FLOAT ) {
    return vk::Format::eR16G16Sfloat;
  }
  if( f == dxgi_format::DXGI_FORMAT_R16G16_UNORM ) {
    return vk::Format::eR16G16Unorm;
  }
  if( f == dxgi_format::DXGI_FORMAT_R16G16_UINT ) {
    return vk::Format::eR16G16Uint;
  }
  if( f == dxgi_format::DXGI_FORMAT_R16G16_SNORM ) {
    return vk::Format::eR16G16Snorm;
  }
  if( f == dxgi_format::DXGI_FORMAT_R16G16_SINT ) {
    return vk::Format::eR16G16Sint;
  }
  if( f == dxgi_format::DXGI_FORMAT_R32_TYPELESS ) {
    return vk::Format::eUndefined;
  }
  if( f == dxgi_format::DXGI_FORMAT_D32_FLOAT ) {
    return vk::Format::eD32Sfloat;
  }
  if( f == dxgi_format::DXGI_FORMAT_R32_FLOAT ) {
    return vk::Format::eR32Sfloat;
  }
  if( f == dxgi_format::DXGI_FORMAT_R32_UINT ) {
    return vk::Format::eR32Uint;
  }
  if( f == dxgi_format::DXGI_FORMAT_R32_SINT ) {
    return vk::Format::eR8G8B8A8Sint;
  }
  if( f == dxgi_format::DXGI_FORMAT_R24G8_TYPELESS ) {
    return vk::Format::eUndefined;
  }
  if( f == dxgi_format::DXGI_FORMAT_D24_UNORM_S8_UINT ) {
    return vk::Format::eD24UnormS8Uint;
  }
  if( f == dxgi_format::DXGI_FORMAT_R24_UNORM_X8_TYPELESS ) {
    return vk::Format::eUndefined;
  }
  if( f == dxgi_format::DXGI_FORMAT_X24_TYPELESS_G8_UINT ) {
    return vk::Format::eUndefined;
  }
  if( f == dxgi_format::DXGI_FORMAT_R8G8_TYPELESS ) {
    return vk::Format::eUndefined;
  }
  if( f == dxgi_format::DXGI_FORMAT_R8G8_UNORM ) {
    return vk::Format::eR8G8Unorm;
  }
  if( f == dxgi_format::DXGI_FORMAT_R8G8_UINT ) {
    return vk::Format::eR8G8Uint;
  }
  if( f == dxgi_format::DXGI_FORMAT_R8G8_SNORM ) {
    return vk::Format::eR8G8Snorm;
  }
  if( f == dxgi_format::DXGI_FORMAT_R8G8_SINT ) {
    return vk::Format::eR8G8Sint;
  }
  if( f == dxgi_format::DXGI_FORMAT_R16_TYPELESS ) {
    return vk::Format::eUndefined;
  }
  if( f == dxgi_format::DXGI_FORMAT_R16_FLOAT ) {
    return vk::Format::eR16Sfloat;
  }
  if( f == dxgi_format::DXGI_FORMAT_D16_UNORM ) {
    return vk::Format::eR16Unorm;
  }
  if( f == dxgi_format::DXGI_FORMAT_R16_UNORM ) {
    return vk::Format::eR16Unorm;
  }
  if( f == dxgi_format::DXGI_FORMAT_R16_UINT ) {
    return vk::Format::eR16Uint;
  }
  if( f == dxgi_format::DXGI_FORMAT_R16_SNORM ) {
    return vk::Format::eR16Snorm;
  }
  if( f == dxgi_format::DXGI_FORMAT_R16_SINT ) {
    return vk::Format::eR16Sint;
  }
  if( f == dxgi_format::DXGI_FORMAT_R8_TYPELESS ) {
    return vk::Format::eUndefined;
  }
  if( f == dxgi_format::DXGI_FORMAT_R8_UNORM ) {
    return vk::Format::eR8Unorm;
  }
  if( f == dxgi_format::DXGI_FORMAT_R8_UINT ) {
    return vk::Format::eR8Uint;
  }
  if( f == dxgi_format::DXGI_FORMAT_R8_SNORM ) {
    return vk::Format::eR8Snorm;
  }
  if( f == dxgi_format::DXGI_FORMAT_R8_SINT ) {
    return vk::Format::eR8Sint;
  }
  if( f == dxgi_format::DXGI_FORMAT_A8_UNORM ) {
    return vk::Format::eR8Unorm;
  }
  if( f == dxgi_format::DXGI_FORMAT_R1_UNORM ) {
    return vk::Format::eUndefined;
  }
  if( f == dxgi_format::DXGI_FORMAT_R9G9B9E5_SHAREDEXP ) {
    return vk::Format::eUndefined;
  }
  if( f == dxgi_format::DXGI_FORMAT_R8G8_B8G8_UNORM ) {
    return vk::Format::eUndefined;
  }
  if( f == dxgi_format::DXGI_FORMAT_G8R8_G8B8_UNORM ) {
    return vk::Format::eUndefined;
  }
  if( f == dxgi_format::DXGI_FORMAT_BC1_TYPELESS ) {
    return vk::Format::eUndefined;
  }
  if( f == dxgi_format::DXGI_FORMAT_BC1_UNORM ) {
    return vk::Format::eBc1RgbaUnormBlock;
  }
  if( f == dxgi_format::DXGI_FORMAT_BC1_UNORM_SRGB ) {
    return vk::Format::eBc1RgbaSrgbBlock;
  }
  if( f == dxgi_format::DXGI_FORMAT_BC2_TYPELESS ) {
    return vk::Format::eUndefined;
  }
  if( f == dxgi_format::DXGI_FORMAT_BC2_UNORM ) {
    return vk::Format::eBc2UnormBlock;
  }
  if( f == dxgi_format::DXGI_FORMAT_BC2_UNORM_SRGB ) {
    return vk::Format::eBc2SrgbBlock;
  }
  if( f == dxgi_format::DXGI_FORMAT_BC3_TYPELESS ) {
    return vk::Format::eUndefined;
  }
  if( f == dxgi_format::DXGI_FORMAT_BC3_UNORM ) {
    return vk::Format::eBc3UnormBlock;
  }
  if( f == dxgi_format::DXGI_FORMAT_BC3_UNORM_SRGB ) {
    return vk::Format::eBc3SrgbBlock;
  }
  if( f == dxgi_format::DXGI_FORMAT_BC4_TYPELESS ) {
    return vk::Format::eUndefined;
  }
  if( f == dxgi_format::DXGI_FORMAT_BC4_UNORM ) {
    return vk::Format::eBc4UnormBlock;
  }
  if( f == dxgi_format::DXGI_FORMAT_BC4_SNORM ) {
    return vk::Format::eBc4SnormBlock;
  }
  if( f == dxgi_format::DXGI_FORMAT_BC5_TYPELESS ) {
    return vk::Format::eUndefined;
  }
  if( f == dxgi_format::DXGI_FORMAT_BC5_UNORM ) {
    return vk::Format::eBc5UnormBlock;
  }
  if( f == dxgi_format::DXGI_FORMAT_BC5_SNORM ) {
    return vk::Format::eBc5SnormBlock;
  }
  if( f == dxgi_format::DXGI_FORMAT_B5G6R5_UNORM ) {
    return vk::Format::eR5G6B5UnormPack16;
  }
  if( f == dxgi_format::DXGI_FORMAT_B5G5R5A1_UNORM ) {
    return vk::Format::eR5G5B5A1UnormPack16;
  }
  if( f == dxgi_format::DXGI_FORMAT_B8G8R8A8_UNORM ) {
    return vk::Format::eB8G8R8A8Unorm;
  }
  if( f == dxgi_format::DXGI_FORMAT_B8G8R8X8_UNORM ) {
    return vk::Format::eUndefined;
  }
  if( f == dxgi_format::DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM ) {
    return vk::Format::eA2B10G10R10UnormPack32;
  }
  if( f == dxgi_format::DXGI_FORMAT_B8G8R8A8_TYPELESS ) {
    return vk::Format::eUndefined;
  }
  if( f == dxgi_format::DXGI_FORMAT_B8G8R8A8_UNORM_SRGB ) {
    return vk::Format::eUndefined;
  }
  if( f == dxgi_format::DXGI_FORMAT_B8G8R8X8_TYPELESS ) {
    return vk::Format::eUndefined;
  }
  if( f == dxgi_format::DXGI_FORMAT_B8G8R8X8_UNORM_SRGB ) {
    return vk::Format::eUndefined;
  }
  if( f == dxgi_format::DXGI_FORMAT_BC6H_TYPELESS ) {
    return vk::Format::eUndefined;
  }
  if( f == dxgi_format::DXGI_FORMAT_BC6H_UF16 ) {
    return vk::Format::eBc6HUfloatBlock;
  }
  if( f == dxgi_format::DXGI_FORMAT_BC6H_SF16 ) {
    return vk::Format::eBc6HSfloatBlock;
  }
  if( f == dxgi_format::DXGI_FORMAT_BC7_TYPELESS ) {
    return vk::Format::eUndefined;
  }
  if( f == dxgi_format::DXGI_FORMAT_BC7_UNORM ) {
    return vk::Format::eBc7UnormBlock;
  }
  if( f == dxgi_format::DXGI_FORMAT_BC7_UNORM_SRGB ) {
    return vk::Format::eBc7SrgbBlock;
  }
  if( f == dxgi_format::DXGI_FORMAT_AYUV ) {
    return vk::Format::eUndefined;
  }
  if( f == dxgi_format::DXGI_FORMAT_Y410 ) {
    return vk::Format::eUndefined;
  }
  if( f == dxgi_format::DXGI_FORMAT_Y416 ) {
    return vk::Format::eUndefined;
  }
  if( f == dxgi_format::DXGI_FORMAT_NV12 ) {
    return vk::Format::eUndefined;
  }
  if( f == dxgi_format::DXGI_FORMAT_P010 ) {
    return vk::Format::eUndefined;
  }
  if( f == dxgi_format::DXGI_FORMAT_P016 ) {
    return vk::Format::eUndefined;
  }
  if( f == dxgi_format::DXGI_FORMAT_420_OPAQUE ) {
    return vk::Format::eUndefined;
  }
  if( f == dxgi_format::DXGI_FORMAT_YUY2 ) {
    return vk::Format::eUndefined;
  }
  if( f == dxgi_format::DXGI_FORMAT_Y210 ) {
    return vk::Format::eUndefined;
  }
  if( f == dxgi_format::DXGI_FORMAT_Y216 ) {
    return vk::Format::eUndefined;
  }
  if( f == dxgi_format::DXGI_FORMAT_NV11 ) {
    return vk::Format::eUndefined;
  }
  if( f == dxgi_format::DXGI_FORMAT_AI44 ) {
    return vk::Format::eUndefined;
  }
  if( f == dxgi_format::DXGI_FORMAT_IA44 ) {
    return vk::Format::eUndefined;
  }
  if( f == dxgi_format::DXGI_FORMAT_P8 ) {
    return vk::Format::eUndefined;
  }
  if( f == dxgi_format::DXGI_FORMAT_A8P8 ) {
    return vk::Format::eUndefined;
  }
  if( f == dxgi_format::DXGI_FORMAT_B4G4R4A4_UNORM ) {
    return vk::Format::eR4G4B4A4UnormPack16;
  }
  if( f == dxgi_format::DXGI_FORMAT_P208 ) {
    return vk::Format::eUndefined;
  }
  if( f == dxgi_format::DXGI_FORMAT_V208 ) {
    return vk::Format::eUndefined;
  }
  if( f == dxgi_format::DXGI_FORMAT_V408 ) {
    return vk::Format::eUndefined;
  }
  if( f == dxgi_format::DXGI_FORMAT_SAMPLER_FEEDBACK_MIN_MIP_OPAQUE ) {
    return vk::Format::eUndefined;
  }
  if( f == dxgi_format::DXGI_FORMAT_SAMPLER_FEEDBACK_MIP_REGION_USED_OPAQUE ) {
    return vk::Format::eUndefined;
  }
  if( f == dxgi_format::DXGI_FORMAT_FORCE_UINT ) {
    return vk::Format::eUndefined;
  }
  return vk::Format::eUndefined;
}


vk::ImageType ddsdimension_to_vk_image_type( d3d10_resource_dimension d ) {
  if( d == d3d10_resource_dimension::D3D10_RESOURCE_DIMENSION_TEXTURE1D ) {
    return vk::ImageType::e1D;
  }
  if( d == d3d10_resource_dimension::D3D10_RESOURCE_DIMENSION_TEXTURE2D ) {
    return vk::ImageType::e2D;
  }
  if( d == d3d10_resource_dimension::D3D10_RESOURCE_DIMENSION_TEXTURE3D ) {
    return vk::ImageType::e3D;
  }
  return vk::ImageType::e2D;
}

dds_iterator &dds_iterator::operator++() {
  if( current_array_layer != array_layer_count ) {
    const auto current_image_bytes = ::gct::get_subimage_bytes( format, max_width, max_height, current_mip_level );
    body += current_image_bytes;
    ++current_mip_level;
    if( current_mip_level == mip_level_count ) {
      current_mip_level = 0u;
      ++current_array_layer;
    }
  }
  return *this;
}
dds_iterator &dds_iterator::operator--() {
  if( current_array_layer != 0u ) {
    if( current_mip_level == 0u ) {
      current_mip_level = mip_level_count - 1u;
      --current_array_layer;
    }
    else {
      --current_mip_level;
    }
    const auto current_image_bytes = ::gct::get_subimage_bytes( format, max_width, max_height, current_mip_level );
    body -= current_image_bytes;
  }
  return *this;
}

std::uint32_t dds_iterator::get_subimage_bytes() const {
  return ::gct::get_subimage_bytes( format, max_width, max_height, current_mip_level );
}

vk::ImageCreateInfo get_dds_image_create_info( const std::uint8_t *head ) {
  const auto *header_ = reinterpret_cast< const header* >( head );

  if( header_->magic != 0x20534444u ) {
    throw std::runtime_error( "Not a DDS file." );
  }

  const bool dx10 = header_->spf.four_cc == pixel_format_four_cc::DX10;
  const auto *header_dxt10_ = reinterpret_cast< const header_dxt10* >( std::next( head, sizeof( header ) ) );

  const auto format =
    dx10 ?
    ddsformat_to_vk_format( header_dxt10_->dxgi_format ):
    ddsformat_to_vk_format( header_->spf );

  if( format == vk::Format::eUndefined ) {
    throw std::runtime_error( "Unsupported format." );
  }

  std::vector< vk::Format > formats;
  formats.push_back( format );

  return vk::ImageCreateInfo()
    .setFlags(
      ( int( header_->caps2 ) &  int(  dds_caps2::DDSCAPS2_CUBEMAP ) ) ?
      vk::ImageCreateFlagBits::eCubeCompatible :
      vk::ImageCreateFlagBits( 0 )
    )
    .setImageType( dx10 ? ddsdimension_to_vk_image_type( header_dxt10_->resource_dimension ) : vk::ImageType::e2D )
    .setFormat( format )
    .setExtent(
      vk::Extent3D()
        .setWidth( header_->width )
        .setHeight( header_->height )
        .setDepth( ( int( header_->flags ) & int( dds_flag::DDSD_DEPTH ) ) ? header_->depth : 1u )
    )
    .setMipLevels( ( int( header_->flags ) & int( dds_flag::DDSD_MIPMAPCOUNT ) ) ? header_->mipmap_count : 1u )
    .setArrayLayers( dx10 ? header_dxt10_->array_size : 1u )
    .setSamples( vk::SampleCountFlagBits::e1 )
    .setTiling( vk::ImageTiling::eOptimal )
    .setSharingMode( vk::SharingMode::eExclusive )
    .setQueueFamilyIndexCount( 0 )
    .setPQueueFamilyIndices( nullptr )
    .setInitialLayout( vk::ImageLayout::eUndefined );
}

dds_iterator get_dds_data( const vk::ImageCreateInfo &ci, const std::uint8_t *head ) {
  const auto *header_ = reinterpret_cast< const header* >( head );
  const bool dx10 = header_->spf.four_cc == pixel_format_four_cc::DX10;
  const auto body = std::next( head, sizeof( header ) + ( dx10 ? sizeof( header_dxt10 ) : 0u ) );
  return dds_iterator(
    body,
    ci.format,
    ci.arrayLayers,
    ci.mipLevels,
    ci.extent.width,
    ci.extent.height
  );
}

}

