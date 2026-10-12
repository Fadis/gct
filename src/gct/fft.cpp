#include <gct/fft.hpp>
#include <cmath>
#include <array>
#include <vector>
#include <gct/is_pot.hpp>
#include <gct/exception.hpp>
#include <kissfft/kiss_fftnd.h>


namespace gct {

void fft2d( const float *input, float *output, std::uint32_t width ) {
  if( !is_pot( width ) ) throw exception::invalid_argument( "fft2d : invalid image width", __FILE__, __LINE__ );
  std::vector< kiss_fft_cpx > complex_input( width * width );
  for( std::uint32_t y = 0u; y < width; ++y ) {
    for( std::uint32_t x = 0u; x < width; ++x ) {
      kiss_fft_cpx c;
      c.r = input[ x + y * width ];
      c.i = 0.0f;
      complex_input[ x + y * width ] = c;
    }
  }
  std::vector< kiss_fft_cpx > complex_output( width * width );
  std::array< int, 2u > nfft{ int(width), int(width) };
  kiss_fftnd_cfg config = kiss_fftnd_alloc( nfft.data(), nfft.size(), false, 0, 0 );
  kiss_fftnd( config, complex_input.data(), complex_output.data() );
  free( config );
  const std::uint32_t half = width / 2;
  for( std::uint32_t y = 0u; y < width; ++y ) {
    for( std::uint32_t x = 0u; x < width; ++x ) {
      kiss_fft_cpx c;
      c.r = input[ x + y * width ];
      c.i = 0.0f;
      output[ ( ( x + half ) % width ) + ( ( y + half ) % width ) * width ] =
        0.1 * std::log( std::sqrt( complex_output[ x + y * width ].r * complex_output[ x + y * width ].r + complex_output[ x + y * width ].i * complex_output[ x + y * width ].i ) + 1.0f );
    }
  }
}

}
