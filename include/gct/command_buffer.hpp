#ifndef GCT_COMMAND_BUFFER_HPP
#define GCT_COMMAND_BUFFER_HPP
#include <memory>
#include <vector>
#include <any>
#include <vulkan/vulkan.hpp>
#include <gct/created_from.hpp>
#include <gct/command_buffer_allocate_info.hpp>
#include <gct/command_buffer_begin_info.hpp>
#include <gct/command_buffer_recorder.hpp>

namespace gct {
  struct command_pool_t;
  struct bound_command_pool_t;
  class fence_t;
  class submit_info_t;
  class bound_command_buffer_t;
  class command_buffer_t : public created_from< command_pool_t >, public std::enable_shared_from_this< command_buffer_t > {
  public:
    friend command_buffer_recorder_t;
    friend bound_command_buffer_t;
    command_buffer_t(
      const std::shared_ptr< command_pool_t >&,
      const command_buffer_allocate_info_t&
    );
    command_buffer_t( const command_buffer_t& ) = delete;
    command_buffer_t( command_buffer_t&& ) = default;
    command_buffer_t &operator=( const command_buffer_t& ) = delete;
    command_buffer_t &operator=( command_buffer_t&& ) = default;
    const command_buffer_allocate_info_t &get_props() const { return props; }
    vk::CommandBuffer &operator*() {
      return *handle;
    }
    const vk::CommandBuffer &operator*() const {
      return *handle;
    }
    vk::CommandBuffer *operator->() {
      return &handle.get();
    }
    const vk::CommandBuffer *operator->() const {
      return &handle.get();
    }
    void reset();
    void execute(
      const submit_info_t&
    );
    bool wait_for_executed();
    bool wait_for_executed( std::uint64_t );
  private:
    void on_executed( vk::Result );
    command_buffer_allocate_info_t props;
    vk::UniqueHandle< vk::CommandBuffer, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE > handle;
    std::vector< std::any > keep;
    std::vector< std::function< void( vk::Result ) > > cbs;
  };
  class bound_command_buffer_t : public created_from< bound_command_pool_t >, public std::enable_shared_from_this< bound_command_buffer_t > {
  public:
    friend command_buffer_recorder_t;
    bound_command_buffer_t(
      const std::shared_ptr< bound_command_pool_t >&,
      const std::shared_ptr< command_buffer_t >&
    );
    bound_command_buffer_t( const bound_command_buffer_t& ) = delete;
    bound_command_buffer_t( bound_command_buffer_t&& ) = default;
    bound_command_buffer_t &operator=( const bound_command_buffer_t& ) = delete;
    bound_command_buffer_t &operator=( bound_command_buffer_t&& ) = default;
    const command_buffer_allocate_info_t &get_props() const { return buffer->get_props(); }
    vk::CommandBuffer &operator*() {
      return **buffer;
    }
    const vk::CommandBuffer &operator*() const {
      return **buffer;
    }
    vk::CommandBuffer *operator->() {
      return &**buffer;
    }
    const vk::CommandBuffer *operator->() const {
      return &**buffer;
    }
    void reset() {
      buffer->reset();
    }
    const std::shared_ptr< command_buffer_t > unbound() const { return buffer; }
    command_buffer_recorder_t begin(
      const command_buffer_begin_info_t&
    );
    command_buffer_recorder_t begin(
      vk::CommandBufferUsageFlags
    );
    command_buffer_recorder_t begin();
    void execute(
      const submit_info_t&
    );
    void wait_for_executed();
    bool wait_for_executed( std::uint64_t );
    void execute_and_wait();
  private:
    std::shared_ptr< command_buffer_t > buffer;
    std::shared_ptr< fence_t > fence;
    bool executing = false;
  };
}

#endif

