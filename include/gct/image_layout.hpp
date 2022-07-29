#ifndef GCT_IMAGE_LAYOUT_HPP
#define GCT_IMAGE_LAYOUT_HPP
#include <cstdint>
#include <vector>
#include <vulkan/vulkan.hpp>
#include <nlohmann/json_fwd.hpp>
#include <hdmap/hdmap.hpp>

namespace gct {
  class image_layout_t {
    using map_t = hdmap::hdmap< std::uint32_t, vk::ImageLayout, 2u >;
    using rect_t = map_t::rect_type;
    using key_t = map_t::key_type;
    using value_t = map_t::value_type;
  public:
    image_layout_t(
      std::uint32_t mip_levels_,
      std::uint32_t array_layers_,
      vk::ImageLayout initial_layout = vk::ImageLayout::eUndefined
    );
    std::vector< vk::ImageMemoryBarrier >
    set_layout(
      std::uint32_t mip_begin,
      std::uint32_t mip_count,
      std::uint32_t array_begin,
      std::uint32_t array_count,
      vk::ImageLayout layout
    );
    std::vector< vk::ImageMemoryBarrier >
    get_layout(
      std::uint32_t mip_begin,
      std::uint32_t mip_count,
      std::uint32_t array_begin,
      std::uint32_t array_count
    ) const;
    std::vector< vk::ImageMemoryBarrier >
    get_layout() const;
    bool is_uniform(
      std::uint32_t mip_begin,
      std::uint32_t mip_count,
      std::uint32_t array_begin,
      std::uint32_t array_count
    ) const;
    bool is_uniform() const;
    void to_json( nlohmann::json &root ) const;
  private:
    std::uint32_t mip_levels;
    std::uint32_t array_layers;
    mutable map_t layouts;
  };
  void to_json( nlohmann::json &root, const image_layout_t &v );
}

#endif

