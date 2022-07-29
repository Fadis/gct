#include <vulkan/vulkan.hpp>
#include <gct/image_layout.hpp>
#include <gct/exception.hpp>
#include <nlohmann/json.hpp>
#include <hdmap/json.hpp>
#include <vulkan2json/ImageMemoryBarrier.hpp>

namespace gct {
  image_layout_t::image_layout_t(
    std::uint32_t mip_levels_,
    std::uint32_t array_layers_,
    vk::ImageLayout initial_layout
  ) :
    mip_levels( mip_levels_ ),
    array_layers( array_layers_ ) {
    layouts.update( layouts.rect( layouts.enc( 0u, 0u ), layouts.enc( mip_levels, array_layers ) ), vk::ImageLayout( initial_layout ) );
  }
  std::vector< vk::ImageMemoryBarrier >
  image_layout_t::set_layout(
    std::uint32_t mip_begin,
    std::uint32_t mip_count,
    std::uint32_t array_begin,
    std::uint32_t array_count,
    vk::ImageLayout layout
  ) {
    const auto mip_end = mip_begin + mip_count;
    if( mip_end > mip_levels )
      exception::out_of_range(
        "image_layout_t::set_layout : Specified mipmap levels are out of range.",
        __FILE__,
        __LINE__
      );
    const auto array_end = array_begin + array_count;
    if( array_end > array_layers )
      exception::out_of_range(
        "image_layout_t::set_layout : Specified array levels are out of range.",
        __FILE__,
        __LINE__
      );
    const auto range = layouts.rect( layouts.enc( mip_begin, array_begin ), layouts.enc( mip_end, array_end ) );
    std::vector< value_t > affected;
    layouts.find( range, affected );
    layouts.update( range, vk::ImageLayout( layout ) );
    std::vector< vk::ImageMemoryBarrier > barriers;
    std::transform(
      affected.begin(), affected.end(),
      std::back_inserter( barriers ),
      [&]( auto &v ) {
        auto left_top = layouts.dec( v.first.left_top );
        auto right_bottom = layouts.dec( v.first.right_bottom );
        return
          vk::ImageMemoryBarrier()
            .setOldLayout( v.second )
            .setNewLayout( layout )
            .setSubresourceRange(
              vk::ImageSubresourceRange()
                .setBaseMipLevel( left_top[ 0 ] )
                .setLevelCount( right_bottom[ 0 ] - left_top[ 0 ] )
                .setBaseArrayLayer( left_top[ 1 ] )
                .setLayerCount( right_bottom[ 1 ] - left_top[ 1 ] )
            );
      }
    );
    return barriers;
  }
  std::vector< vk::ImageMemoryBarrier >
  image_layout_t::get_layout(
    std::uint32_t mip_begin,
    std::uint32_t mip_count,
    std::uint32_t array_begin,
    std::uint32_t array_count
  ) const {
    const auto mip_end = mip_begin + mip_count;
    if( mip_end > mip_levels )
      exception::out_of_range(
        "image_layout_t::set_layout : Specified mipmap levels are out of range.",
        __FILE__,
        __LINE__
      );
    const auto array_end = array_begin + array_count;
    if( array_end > array_layers )
      exception::out_of_range(
        "image_layout_t::set_layout : Specified array levels are out of range.",
        __FILE__,
        __LINE__
      );
    const auto range = layouts.rect( layouts.enc( mip_begin, array_begin ), layouts.enc( mip_end, array_end ) );
    std::vector< value_t > temp;
    layouts.find( range, temp );
    std::vector< vk::ImageMemoryBarrier > barriers;
    std::transform(
      temp.begin(), temp.end(),
      std::back_inserter( barriers ),
      [&]( auto &v ) {
        auto left_top = layouts.dec( v.first.left_top );
        auto right_bottom = layouts.dec( v.first.right_bottom );
        return
          vk::ImageMemoryBarrier()
            .setOldLayout( v.second )
            .setNewLayout( v.second )
            .setSubresourceRange(
              vk::ImageSubresourceRange()
                .setBaseMipLevel( left_top[ 0 ] )
                .setLevelCount( right_bottom[ 0 ] - left_top[ 0 ] )
                .setBaseArrayLayer( left_top[ 1 ] )
                .setLayerCount( right_bottom[ 1 ] - left_top[ 1 ] )
            );
      }
    );
    return barriers;
  }
  std::vector< vk::ImageMemoryBarrier >
  image_layout_t::get_layout() const {
    return get_layout( 0u, mip_levels, 0u, array_layers );
  }
  bool image_layout_t::is_uniform(
    std::uint32_t mip_begin,
    std::uint32_t mip_count,
    std::uint32_t array_begin,
    std::uint32_t array_count
  ) const {
    const auto mip_end = mip_begin + mip_count;
    if( mip_end > mip_levels )
      exception::out_of_range(
        "image_layout_t::set_layout : Specified mipmap levels are out of range.",
        __FILE__,
        __LINE__
      );
    const auto array_end = array_begin + array_count;
    if( array_end > array_layers )
      exception::out_of_range(
        "image_layout_t::set_layout : Specified array levels are out of range.",
        __FILE__,
        __LINE__
      );
    const auto range = layouts.rect( layouts.enc( mip_begin, array_begin ), layouts.enc( mip_end, array_end ) );
    std::vector< value_t > temp;
    layouts.find( range, temp );
    return temp.size() == 1u;
  }
  bool image_layout_t::is_uniform() const {
    return is_uniform( 0u, mip_levels, 0u, array_layers );
  }
  void image_layout_t::to_json( nlohmann::json &root ) const {
    root = get_layout();
  }
  void to_json( nlohmann::json &root, const image_layout_t &v ) {
    v.to_json( root );
  }
}

