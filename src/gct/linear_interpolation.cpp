#include <gct/linear_interpolation.hpp>
#include <algorithm>
#include <glm/common.hpp>

namespace gct {

float linear_interpolation( const host_image &image, std::uint32_t channel, const glm::vec2 p ) {
  const auto unnorm_pos = glm::vec2(
    std::max( std::min( p.x, 1.0f ), 0.0f ) * ( image.width - 1 ),
    std::max( std::min( p.y, 1.0f ), 0.0f ) * ( image.height - 1 )
  );
  const auto left_top = glm::ivec2( glm::trunc( unnorm_pos ) );
  const auto right_bottom = glm::ivec2(
    std::min( left_top.x, int32_t( image.width ) - 1 ),
    std::min( left_top.y, int32_t( image.height ) - 1 )
  );
  const auto offset = unnorm_pos - glm::trunc( unnorm_pos );
  const float i0 =
    image.image[ channel + ( left_top.x + left_top.y * image.width ) * image.channels ] * ( 1.0f - offset.y ) +
    image.image[ channel + ( left_top.x + right_bottom.y * image.width ) * image.channels ] * offset.y;
  const float i1 =
    image.image[ channel + ( right_bottom.x + left_top.y * image.width ) * image.channels ] * ( 1.0f - offset.y ) +
    image.image[ channel + ( right_bottom.x + right_bottom.y * image.width ) * image.channels ] * offset.y;
  return i0 * ( 1.0f - offset.x ) + i1 * offset.x;
}

}
