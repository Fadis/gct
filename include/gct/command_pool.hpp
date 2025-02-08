#ifndef GCT_COMMAND_POOL_HPP
#define GCT_COMMAND_POOL_HPP
#include <memory>
#include <mutex>
#include <vulkan/vulkan.hpp>
#include <gct/queue.hpp>
#include <gct/created_from.hpp>
#include <gct/command_buffer.hpp>
#include <gct/vulkan_handle.hpp>

namespace gct {
  struct device_t;
  class command_buffer_allocate_info_t;
  class command_pool_t :
    public vulkan_handle< vk::CommandPool >,
    public created_from< device_t >,
    public std::enable_shared_from_this< command_pool_t > {
  public:
    command_pool_t(
      const std::shared_ptr< device_t > &queue,
      const activated_queue_family_count_t&
    );
    [[nodiscard]] std::shared_ptr< command_buffer_t > allocate(
      const command_buffer_allocate_info_t&
    );
    [[nodiscard]] std::shared_ptr< command_buffer_t > allocate();
  private:
    activated_queue_family_count_t queue_index;
  };
  class bound_command_pool_t :
    public created_from< queue_t >,
    public std::enable_shared_from_this< bound_command_pool_t > {
  public:
    bound_command_pool_t(
      const std::shared_ptr< queue_t >&,
      const std::shared_ptr< command_pool_t >&
    );
    [[nodiscard]] vk::CommandPool &operator*() {
      return **pool;
    }
    [[nodiscard]] const vk::CommandPool &operator*() const {
      return **pool;
    }
    [[nodiscard]] vk::CommandPool *operator->() {
      return &**pool;
    }
    [[nodiscard]] const vk::CommandPool *operator->() const {
      return &**pool;
    }
    [[nodiscard]] std::shared_ptr< bound_command_buffer_t > allocate(
      const command_buffer_allocate_info_t&
    );
    [[nodiscard]] std::shared_ptr< bound_command_buffer_t > allocate();
  private:
    std::shared_ptr< command_pool_t > pool;
  };
}

#endif

