#ifndef LIBGCT_INCLUDE_GCT_QUEUE_HPP
#define LIBGCT_INCLUDE_GCT_QUEUE_HPP
#include <optional>
#include <memory>
#include <unordered_map>
#include <nlohmann/json_fwd.hpp>
#include <vulkan/vulkan.hpp>
#include <gct/queue_requirement.hpp>
#include <gct/created_from.hpp>

namespace gct {
  void to_json( nlohmann::json &, const queue_requirement_t& );

  using queue_requirements_t =
    std::vector< queue_requirement_t >;

  class device_t;

  class command_pool_t;
  class bound_command_pool_t;
  class present_info_t;

  class queue_t : public created_from< device_t >, public std::enable_shared_from_this< queue_t > {
  public:
    queue_t(
      const std::shared_ptr< device_t > &d,
      vk::Queue q,
      const std::shared_ptr< command_pool_t > &p,
      std::uint32_t
    );
    std::shared_ptr< bound_command_pool_t > get_command_pool();
    vk::Queue &operator*() {
      return queue;
    }
    const vk::Queue &operator*() const {
      return queue;
    }
    vk::Queue *operator->() {
      return &queue;
    }
    const vk::Queue *operator->() const {
      return &queue;
    }
    std::uint32_t get_available_queue_family_index() const {
      return available_queue_family_index;
    }
#ifdef VK_KHR_SWAPCHAIN_EXTENSION_NAME
    void present( const present_info_t& );
#endif
  private:
    vk::Queue queue;
    std::shared_ptr< command_pool_t > command_pool;
    std::uint32_t available_queue_family_index;
  };

  using available_queue_family_index_t = std::uint32_t;
  using logical_queue_index_t = std::uint32_t;

  using available_queue_family_count_t = std::uint32_t;
  using available_queue_family_counts_t =
    std::unordered_map< available_queue_family_index_t, available_queue_family_count_t >;
  

  using activated_queue_family_index_t = std::uint32_t;

  struct activated_queue_family_count_t {
    available_queue_family_index_t available_queue_family_index = 0u;
    activated_queue_family_index_t activated_queue_family_index = 0u;
#ifdef VK_EXT_GLOBAL_PRIORITY_EXTENSION_NAME
    std::optional< vk::QueueGlobalPriorityEXT > global_priority;
#endif
    std::uint32_t count = 0u;
    std::vector< float > local_priority;
    vk::CommandPoolCreateFlagBits command_pool_create_flags = vk::CommandPoolCreateFlagBits{};
  };
  
  void to_json( nlohmann::json &, const activated_queue_family_count_t& );

  using activated_queue_family_counts_t =
    std::vector< activated_queue_family_count_t >;
  
  void to_json( nlohmann::json, const activated_queue_family_counts_t& );

  struct activated_queue_mapping_t {
    queue_requirement_t req;
    logical_queue_index_t order = 0u;
    std::uint32_t rarity = 0u;
    std::uint32_t available_queue_family_index = 0u;
    std::uint32_t activated_queue_family_index = 0u;
    std::uint32_t queue_count_index = 0u;
  };
  
  void to_json( nlohmann::json &, const activated_queue_mapping_t& );

  using activated_queue_mappings_t =
    std::vector< activated_queue_mapping_t >;

  void to_json( nlohmann::json&, const activated_queue_mappings_t& );

  struct physical_device_properties_t;
  std::tuple<
    activated_queue_mappings_t,
    activated_queue_family_counts_t
  >
  generate_queue_map(
    const vk::PhysicalDevice &pdev,
    const physical_device_properties_t &props,
    const queue_requirements_t &reqs_
  );
}

#endif

