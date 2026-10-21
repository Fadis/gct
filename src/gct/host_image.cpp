#include <OpenImageIO/imageio.h>
#include <OpenImageIO/version.h>
#include <gct/host_image.hpp>
#include <gct/gamma.hpp>
#include <gct/color_space.hpp>
namespace gct {


host_image
create_host_image_from_file(
  const std::string &filename,
  bool is_srgb,
  bool degamma,
  bool grayscale
) {
  host_image state;

#if OIIO_VERSION_MAJOR >= 4 || ( OIIO_VERSION_MAJOR == 3 && OIIO_VERSION_MINOR >= 1 )
    using namespace OIIO_CURRENT_NAMESPACE;
#else
    using namespace OIIO_NAMESPACE;
#endif
#if OIIO_VERSION_MAJOR >= 2 
  auto file = ImageInput::open( filename );
#else
  std::shared_ptr< ImageInput > file(
    ImageInput::open( filename ),
    []( auto p ) { if( p ) ImageInput::destroy( p ); }
  );
#endif
  if( !file ) {
    return host_image();
  }
  const ImageSpec &spec = file->spec();

  if( spec.nchannels == 0 ) {
    return host_image();
  }
    
  if( spec.nchannels == 2 && grayscale ) {
    return host_image();
  }

  state.width = spec.width;
  state.height = spec.height;
  state.channels = grayscale ? 1u : spec.nchannels;
  state.image.resize( state.width * state.height * spec.nchannels );

  file->read_image(
#if OIIO_VERSION_MAJOR >= 2 
    0u, 0u,
#endif
    0u, spec.nchannels,
    TypeDesc::FLOAT,
    state.image.data()
  );
  if( degamma ) {
    for( auto &v: state.image )  {
      v = srgb_degamma( v );
    }
  }
  if( is_srgb && spec.nchannels >= 3u ) {
    const auto csmat = get_rgb_to_xyz( color_space::bt709 );
    for( std::uint32_t y = 0u; y != state.height; ++y ) {
      for( std::uint32_t x = 0u; x != state.width; ++x ) {
        auto converted = csmat * glm::vec4{
          state.image[ 0u + x * spec.nchannels + y * state.width * spec.nchannels ],
          state.image[ 1u + x * spec.nchannels + y * state.width * spec.nchannels ],
          state.image[ 2u + x * spec.nchannels + y * state.width * spec.nchannels ],
          1.0f
        };
        converted /= converted.w;
        state.image[ 0u + x * spec.nchannels + y * state.width * spec.nchannels ] = converted.x;
        state.image[ 1u + x * spec.nchannels + y * state.width * spec.nchannels ] = converted.y;
        state.image[ 2u + x * spec.nchannels + y * state.width * spec.nchannels ] = converted.z;
      }
    }
  }
  if( grayscale && spec.nchannels >= 3u ) {
    for( std::uint32_t y = 0u; y != state.height; ++y ) {
      for( std::uint32_t x = 0u; x != state.width; ++x ) {
        state.image[ x + y * state.width ] =
          state.image[ 1u + x * spec.nchannels + y * state.width * spec.nchannels ];
      }
    }
    state.channels = 1u;
    state.image.resize( state.width * state.height );
  }
  state.loaded = true;
  return state;
}

host_image
create_host_image(
  std::uint32_t width,
  std::uint32_t height,
  std::uint32_t channels
) {
  host_image state;

  state.loaded = true;
  state.width = width;
  state.height = height;
  state.channels = channels;
  state.image.resize( state.width * state.height * state.channels, 0.0f );
  return state;
}

void save_host_image(
  const host_image &image,
  const std::string &filename
) {
#if OIIO_VERSION_MAJOR >= 4 || ( OIIO_VERSION_MAJOR == 3 && OIIO_VERSION_MINOR >= 1 )
  using namespace OIIO_CURRENT_NAMESPACE;
#else
  using namespace OIIO_NAMESPACE;
#endif
  auto out = ImageOutput::create( filename );
  if( !out ) throw -1;
  ImageSpec spec( image.width, image.height, image.channels, filename.substr( filename.size() - 4u ) == ".exr" ? TypeDesc::HALF : TypeDesc::UINT8 );
  spec.attribute ("oiio:ColorSpace", "cie_xyz_d65_interchange");
  out->open( filename, spec );
  out->write_image( TypeDesc::FLOAT, image.image.data() );
  out->close();
}

}

