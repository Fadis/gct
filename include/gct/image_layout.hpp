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
    const map_t &get_layout_map() const;
    bool is_uniform(
      std::uint32_t mip_begin,
      std::uint32_t mip_count,
      std::uint32_t array_begin,
      std::uint32_t array_count
    ) const;
    bool is_uniform() const;
    vk::ImageLayout get_uniform_layout() const;
    void to_json( nlohmann::json &root ) const;
    std::uint32_t get_mip_level_count() const {
      return mip_levels;
    }
    std::uint32_t get_array_layer_count() const {
      return array_layers;
    }
    bool is_copyable_source_layout() const;
    bool is_copyable_destination_layout() const;
  private:
    std::uint32_t mip_levels;
    std::uint32_t array_layers;
    mutable map_t layouts;
  };
  void to_json( nlohmann::json &root, const image_layout_t &v );
}

#endif

