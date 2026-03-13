#ifndef GCT_DDS_HPP
#define GCT_DDS_HPP

#include <cstdint>
#include <vulkan/vulkan.hpp>

namespace gct::dds {

enum class pixel_format_flag {
  DDPF_ALPHAPIXELS = 0x1,
  DDPF_ALPHA = 0x2,
  DDPF_FOURCC = 0x4,
  DDPF_RGB = 0x40,
  DDPF_YUV = 0x200,
  DDPF_LUMINANCE = 0x20000
};

enum class pixel_format_four_cc : std::uint32_t {
  DXT1 = 0x31545844,
  DXT2 = 0x32545844,
  DXT3 = 0x33545844,
  DXT4 = 0x34545844,
  DXT5 = 0x35545844,
  DX10 = 0x30315844
};

struct pixel_format {
  std::uint32_t size = 0u;
  pixel_format_flag flags = pixel_format_flag( 0 );
  pixel_format_four_cc four_cc = pixel_format_four_cc( 0 );
  std::uint32_t rgb_bit_count = 0u;
  std::uint32_t r_bit_mask = 0u;
  std::uint32_t g_bit_mask = 0u;
  std::uint32_t b_bit_mask = 0u;
  std::uint32_t a_bit_mask = 0u;
};

enum class dds_flag {
  DDSD_CAPS = 0x1,
  DDSD_HEIGHT = 0x2,
  DDSD_WIDTH = 0x4,
  DDSD_PITCH = 0x8,
  DDSD_PIXELFORMAT = 0x1000,
  DDSD_MIPMAPCOUNT = 0x20000,
  DDSD_LINEARSIZE = 0x80000,
  DDSD_DEPTH = 0x800000
};

enum class dds_caps {
  DDSCAPS_COMPLEX = 0x8,
  DDSCAPS_MIPMAP = 0x400000,
  DDSCAPS_TEXTURE = 0x1000
};

enum class dds_caps2 {
  DDSCAPS2_CUBEMAP = 0x200,
  DDSCAPS2_CUBEMAP_POSITIVEX = 0x400,
  DDSCAPS2_CUBEMAP_NEGATIVEX = 0x800,
  DDSCAPS2_CUBEMAP_POSITIVEY = 0x1000,
  DDSCAPS2_CUBEMAP_NEGATIVEY = 0x2000,
  DDSCAPS2_CUBEMAP_POSITIVEZ = 0x4000,
  DDSCAPS2_CUBEMAP_NEGATIVEZ = 0x8000,
  DDSCAPS2_VOLUME = 0x200000
};


struct header {
  std::uint32_t magic = 0x20534444u;
  std::uint32_t size = 0u;
  dds_flag flags = dds_flag( int( dds_flag::DDSD_CAPS ) | int( dds_flag::DDSD_HEIGHT ) | int( dds_flag::DDSD_WIDTH ) | int( dds_flag::DDSD_PIXELFORMAT ) );
  std::uint32_t height = 0u;
  std::uint32_t width = 0u;
  std::uint32_t pitch_or_linear_size = 0u;
  std::uint32_t depth = 0u;
  std::uint32_t mipmap_count = 0u;
  std::uint32_t reserved1[11] = { 0u };
  pixel_format  spf;
  dds_caps caps = dds_caps( dds_caps::DDSCAPS_TEXTURE );
  dds_caps2 caps2 = dds_caps2( 0 );
  std::uint32_t caps3 = 0u;
  std::uint32_t caps4 = 0u;
  std::uint32_t reserved2 = 0u;
};

enum class dxgi_format : std::uint32_t {
  DXGI_FORMAT_UNKNOWN = 0,
  DXGI_FORMAT_R32G32B32A32_TYPELESS = 1,
  DXGI_FORMAT_R32G32B32A32_FLOAT = 2,
  DXGI_FORMAT_R32G32B32A32_UINT = 3,
  DXGI_FORMAT_R32G32B32A32_SINT = 4,
  DXGI_FORMAT_R32G32B32_TYPELESS = 5,
  DXGI_FORMAT_R32G32B32_FLOAT = 6,
  DXGI_FORMAT_R32G32B32_UINT = 7,
  DXGI_FORMAT_R32G32B32_SINT = 8,
  DXGI_FORMAT_R16G16B16A16_TYPELESS = 9,
  DXGI_FORMAT_R16G16B16A16_FLOAT = 10,
  DXGI_FORMAT_R16G16B16A16_UNORM = 11,
  DXGI_FORMAT_R16G16B16A16_UINT = 12,
  DXGI_FORMAT_R16G16B16A16_SNORM = 13,
  DXGI_FORMAT_R16G16B16A16_SINT = 14,
  DXGI_FORMAT_R32G32_TYPELESS = 15,
  DXGI_FORMAT_R32G32_FLOAT = 16,
  DXGI_FORMAT_R32G32_UINT = 17,
  DXGI_FORMAT_R32G32_SINT = 18,
  DXGI_FORMAT_R32G8X24_TYPELESS = 19,
  DXGI_FORMAT_D32_FLOAT_S8X24_UINT = 20,
  DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS = 21,
  DXGI_FORMAT_X32_TYPELESS_G8X24_UINT = 22,
  DXGI_FORMAT_R10G10B10A2_TYPELESS = 23,
  DXGI_FORMAT_R10G10B10A2_UNORM = 24,
  DXGI_FORMAT_R10G10B10A2_UINT = 25,
  DXGI_FORMAT_R11G11B10_FLOAT = 26,
  DXGI_FORMAT_R8G8B8A8_TYPELESS = 27,
  DXGI_FORMAT_R8G8B8A8_UNORM = 28,
  DXGI_FORMAT_R8G8B8A8_UNORM_SRGB = 29,
  DXGI_FORMAT_R8G8B8A8_UINT = 30,
  DXGI_FORMAT_R8G8B8A8_SNORM = 31,
  DXGI_FORMAT_R8G8B8A8_SINT = 32,
  DXGI_FORMAT_R16G16_TYPELESS = 33,
  DXGI_FORMAT_R16G16_FLOAT = 34,
  DXGI_FORMAT_R16G16_UNORM = 35,
  DXGI_FORMAT_R16G16_UINT = 36,
  DXGI_FORMAT_R16G16_SNORM = 37,
  DXGI_FORMAT_R16G16_SINT = 38,
  DXGI_FORMAT_R32_TYPELESS = 39,
  DXGI_FORMAT_D32_FLOAT = 40,
  DXGI_FORMAT_R32_FLOAT = 41,
  DXGI_FORMAT_R32_UINT = 42,
  DXGI_FORMAT_R32_SINT = 43,
  DXGI_FORMAT_R24G8_TYPELESS = 44,
  DXGI_FORMAT_D24_UNORM_S8_UINT = 45,
  DXGI_FORMAT_R24_UNORM_X8_TYPELESS = 46,
  DXGI_FORMAT_X24_TYPELESS_G8_UINT = 47,
  DXGI_FORMAT_R8G8_TYPELESS = 48,
  DXGI_FORMAT_R8G8_UNORM = 49,
  DXGI_FORMAT_R8G8_UINT = 50,
  DXGI_FORMAT_R8G8_SNORM = 51,
  DXGI_FORMAT_R8G8_SINT = 52,
  DXGI_FORMAT_R16_TYPELESS = 53,
  DXGI_FORMAT_R16_FLOAT = 54,
  DXGI_FORMAT_D16_UNORM = 55,
  DXGI_FORMAT_R16_UNORM = 56,
  DXGI_FORMAT_R16_UINT = 57,
  DXGI_FORMAT_R16_SNORM = 58,
  DXGI_FORMAT_R16_SINT = 59,
  DXGI_FORMAT_R8_TYPELESS = 60,
  DXGI_FORMAT_R8_UNORM = 61,
  DXGI_FORMAT_R8_UINT = 62,
  DXGI_FORMAT_R8_SNORM = 63,
  DXGI_FORMAT_R8_SINT = 64,
  DXGI_FORMAT_A8_UNORM = 65,
  DXGI_FORMAT_R1_UNORM = 66,
  DXGI_FORMAT_R9G9B9E5_SHAREDEXP = 67,
  DXGI_FORMAT_R8G8_B8G8_UNORM = 68,
  DXGI_FORMAT_G8R8_G8B8_UNORM = 69,
  DXGI_FORMAT_BC1_TYPELESS = 70,
  DXGI_FORMAT_BC1_UNORM = 71,
  DXGI_FORMAT_BC1_UNORM_SRGB = 72,
  DXGI_FORMAT_BC2_TYPELESS = 73,
  DXGI_FORMAT_BC2_UNORM = 74,
  DXGI_FORMAT_BC2_UNORM_SRGB = 75,
  DXGI_FORMAT_BC3_TYPELESS = 76,
  DXGI_FORMAT_BC3_UNORM = 77,
  DXGI_FORMAT_BC3_UNORM_SRGB = 78,
  DXGI_FORMAT_BC4_TYPELESS = 79,
  DXGI_FORMAT_BC4_UNORM = 80,
  DXGI_FORMAT_BC4_SNORM = 81,
  DXGI_FORMAT_BC5_TYPELESS = 82,
  DXGI_FORMAT_BC5_UNORM = 83,
  DXGI_FORMAT_BC5_SNORM = 84,
  DXGI_FORMAT_B5G6R5_UNORM = 85,
  DXGI_FORMAT_B5G5R5A1_UNORM = 86,
  DXGI_FORMAT_B8G8R8A8_UNORM = 87,
  DXGI_FORMAT_B8G8R8X8_UNORM = 88,
  DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM = 89,
  DXGI_FORMAT_B8G8R8A8_TYPELESS = 90,
  DXGI_FORMAT_B8G8R8A8_UNORM_SRGB = 91,
  DXGI_FORMAT_B8G8R8X8_TYPELESS = 92,
  DXGI_FORMAT_B8G8R8X8_UNORM_SRGB = 93,
  DXGI_FORMAT_BC6H_TYPELESS = 94,
  DXGI_FORMAT_BC6H_UF16 = 95,
  DXGI_FORMAT_BC6H_SF16 = 96,
  DXGI_FORMAT_BC7_TYPELESS = 97,
  DXGI_FORMAT_BC7_UNORM = 98,
  DXGI_FORMAT_BC7_UNORM_SRGB = 99,
  DXGI_FORMAT_AYUV = 100,
  DXGI_FORMAT_Y410 = 101,
  DXGI_FORMAT_Y416 = 102,
  DXGI_FORMAT_NV12 = 103,
  DXGI_FORMAT_P010 = 104,
  DXGI_FORMAT_P016 = 105,
  DXGI_FORMAT_420_OPAQUE = 106,
  DXGI_FORMAT_YUY2 = 107,
  DXGI_FORMAT_Y210 = 108,
  DXGI_FORMAT_Y216 = 109,
  DXGI_FORMAT_NV11 = 110,
  DXGI_FORMAT_AI44 = 111,
  DXGI_FORMAT_IA44 = 112,
  DXGI_FORMAT_P8 = 113,
  DXGI_FORMAT_A8P8 = 114,
  DXGI_FORMAT_B4G4R4A4_UNORM = 115,
  DXGI_FORMAT_P208 = 130,
  DXGI_FORMAT_V208 = 131,
  DXGI_FORMAT_V408 = 132,
  DXGI_FORMAT_SAMPLER_FEEDBACK_MIN_MIP_OPAQUE = 189,
  DXGI_FORMAT_SAMPLER_FEEDBACK_MIP_REGION_USED_OPAQUE = 190,
  DXGI_FORMAT_FORCE_UINT = 0xffffffff
};

enum class d3d10_resource_dimension {
  D3D10_RESOURCE_DIMENSION_UNKNOWN = 0,
  D3D10_RESOURCE_DIMENSION_BUFFER = 1,
  D3D10_RESOURCE_DIMENSION_TEXTURE1D = 2,
  D3D10_RESOURCE_DIMENSION_TEXTURE2D = 3,
  D3D10_RESOURCE_DIMENSION_TEXTURE3D = 4
};

enum class d3d10_resource_misc_flag {
  D3D10_RESOURCE_MISC_GENERATE_MIPS = 0x1L,
  D3D10_RESOURCE_MISC_SHARED = 0x2L,
  D3D10_RESOURCE_MISC_TEXTURECUBE = 0x4L,
  D3D10_RESOURCE_MISC_SHARED_KEYEDMUTEX = 0x10L,
  D3D10_RESOURCE_MISC_GDI_COMPATIBLE = 0x20L
};

enum class d3d10_resource_misc2_flags {
  DDS_ALPHA_MODE_UNKNOWN = 0,
  DDS_ALPHA_MODE_STRAIGHT = 1,
  DDS_ALPHA_MODE_PREMULTIPLIED = 2,
  DDS_ALPHA_MODE_OPAQUE = 3,
  DDS_ALPHA_MODE_CUSTOM = 4
};

struct header_dxt10 {
  dxgi_format dxgi_format = dxgi_format::DXGI_FORMAT_UNKNOWN;
  d3d10_resource_dimension resource_dimension = d3d10_resource_dimension::D3D10_RESOURCE_DIMENSION_UNKNOWN;
  d3d10_resource_misc_flag misc_flag = d3d10_resource_misc_flag( 0 );
  std::uint32_t array_size = 0;
  d3d10_resource_misc2_flags misc_flags2 = d3d10_resource_misc2_flags( 0 );
};

class dds_iterator {
public:
  dds_iterator() {}
  dds_iterator(
    const std::uint8_t *b,
    vk::Format f,
    std::uint32_t alc,
    std::uint32_t mlc,
    std::uint32_t w,
    std::uint32_t h
  ) :
    body( b ),
    format( f ),
    array_layer_count( alc ),
    mip_level_count( mlc ),
    max_width( w ),
    max_height( h ) {}
  const std::uint8_t *operator*() const {
    return body;
  }
  bool operator==( const dds_iterator &r ) const {
    if( body == nullptr ) {
      if( r.body == nullptr ) return true;
      return r.current_array_layer == r.array_layer_count;
    }
    else if( r.body == nullptr ) {
      if( body == nullptr ) return true;
      return current_array_layer == array_layer_count;
    }
    else return body == r.body;
  }
  bool operator!=( const dds_iterator &r ) const {
    return !( *this == r );
  }
  bool operator<( const dds_iterator &r ) const {
    if( body == nullptr ) {
      if( r.body == nullptr ) return false;
      return false;
    }
    else if( r.body == nullptr ) {
      if( body == nullptr ) return false;
      return current_array_layer != array_layer_count;
    }
    else return body < r.body;
  }
  bool operator<=( const dds_iterator &r ) const {
    if( body == nullptr ) {
      if( r.body == nullptr ) return true;
      return false;
    }
    else if( r.body == nullptr ) {
      if( body == nullptr ) return true;
      return current_array_layer != array_layer_count;
    }
    else return body <= r.body;
  }
  bool operator>( const dds_iterator &r ) const {
    if( body == nullptr ) {
      if( r.body == nullptr ) return false;
      return r.current_array_layer != r.array_layer_count;
    }
    else if( r.body == nullptr ) {
      if( body == nullptr ) return false;
      return false;
    }
    else return body > r.body;
  }
  bool operator>=( const dds_iterator &r ) const {
    if( body == nullptr ) {
      if( r.body == nullptr ) return true;
      return r.current_array_layer != r.array_layer_count;
    }
    else if( r.body == nullptr ) {
      if( body == nullptr ) return true;
      return false;
    }
    else return body >= r.body;
  }
  dds_iterator &operator++();
  dds_iterator &operator--();
  std::uint32_t get_subimage_bytes() const;
  std::uint32_t get_mip_level() const {
    return current_mip_level;
  }
  std::uint32_t get_array_layer() const {
    return current_array_layer;
  }
private:
  const std::uint8_t *body = nullptr;
  vk::Format format = vk::Format::eUndefined;
  std::uint32_t array_layer_count = 0u;
  std::uint32_t mip_level_count = 0u;
  std::uint32_t max_width = 0u;
  std::uint32_t max_height = 0u;
  std::uint32_t current_array_layer = 0u;
  std::uint32_t current_mip_level = 0u;
  std::uint32_t current_width = 0u;
  std::uint32_t current_height = 0u;
};

vk::Format ddsformat_to_vk_format( const pixel_format& );
vk::Format ddsformat_to_vk_format( dxgi_format );
vk::ImageType ddsdimension_to_vk_image_type( d3d10_resource_dimension );
vk::ImageCreateInfo get_dds_image_create_info( const std::uint8_t *head );
dds_iterator get_dds_data( const vk::ImageCreateInfo&, const std::uint8_t *head );

}

#endif

