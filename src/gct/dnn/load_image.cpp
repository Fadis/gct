#include <OpenImageIO/imageio.h>
#include <OpenImageIO/version.h>
#include <gct/allocator.hpp>
#include <gct/nnef_data.hpp>
#include <gct/numeric_type_to_oiio_type.hpp>
#include <gct/buffer.hpp>
#include <gct/dnn/nnef.hpp>
#include <gct/dnn/load_image.hpp>

namespace gct::dnn {
  void load_image(
    const std::shared_ptr< allocator_t > &allocator,
    const std::filesystem::path &filename,
    const nnef_data_t &dest,
    command_buffer_recorder_t &rec
  ) {
    using namespace OIIO_NAMESPACE;
#if OIIO_VERSION_MAJOR >= 2 
    auto file = ImageInput::open( filename.string() );
#else
    std::shared_ptr< ImageInput > file(
      ImageInput::open( filename.string() ),
      []( auto p ) { if( p ) ImageInput::destroy( p ); }
    );
#endif
    if( !file ) throw -1;
    const ImageSpec &spec = file->spec();
    if( spec.nchannels == 0 ) throw -1;
    const auto type = numeric_type_to_oiio_type( dest.type );
    if( dest.dim.size() < 2u ) throw -1;
    if( spec.width != dest.dim[ dest.dim.size() - 1u ] ) throw -1;
    if( spec.height != dest.dim[ dest.dim.size() - 2u ] ) throw -1;
    std::vector< std::uint8_t > temp( dest.buffer->get_props().get_basic().size, 0u );
    std::unordered_map< std::string, int > channel_order{
      { "R", 2 },
      { "G", 1 },
      { "B", 0 }
    };
    for( int c = 0; c != spec.nchannels; ++c ) {
      const auto order = channel_order.find( spec.channelnames[ c ] );
      if( order != channel_order.end() ) {
        file->read_image(
          c,
          c + 1u,
          type,
          reinterpret_cast< std::uint8_t* >( std::next( temp.data(), spec.width * spec.height * dest.type.depth/8u * order->second ) )
        );
      }
    }
    constexpr std::array< float, 3u > mean{ 123.68f, 116.779f, 103.939f };
    for( int c = 0; c != spec.nchannels; ++c ) {
      for( unsigned int y = 0; y != spec.height; ++y ) {
        for( unsigned int x = 0; x != spec.width; ++x ) {
          const auto index = x + y * spec.width + c * spec.width * spec.height;
          reinterpret_cast< float* >( temp.data() )[ index ] =
            reinterpret_cast< float* >( temp.data() )[ index ] * 255.0f - mean[ c ];
        }
      }
    }
    rec.load_buffer(
      allocator,
      temp,
      dest.buffer
    );
  }
}

