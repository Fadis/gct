#include <gct/device.hpp>
#include <gct/command_buffer_allocate_info.hpp>
#include <gct/command_pool.hpp>

namespace gct {
  command_pool_t::command_pool_t(
    const std::shared_ptr< device_t > &device,
    const activated_queue_family_count_t &f
  ) : 
    created_from< device_t >( device ),
    handle(
      (*get_factory())->createCommandPoolUnique(
        vk::CommandPoolCreateInfo()
          .setQueueFamilyIndex( f.available_queue_family_index )
          .setFlags( f.command_pool_create_flags )
      )
    ),
    queue_index( f ) {}
  std::shared_ptr< command_buffer_t > command_pool_t::allocate(
    const command_buffer_allocate_info_t &create_info
  ) {
    return std::shared_ptr< command_buffer_t >(
      new command_buffer_t(
        shared_from_this(),
        create_info
      )
    );
  }
  std::shared_ptr< command_buffer_t > command_pool_t::allocate() {
    return allocate(
      command_buffer_allocate_info_t()
        .set_basic(
          vk::CommandBufferAllocateInfo()
            .setLevel( vk::CommandBufferLevel::ePrimary )
        )
    );
  }
  bound_command_pool_t::bound_command_pool_t(
    const std::shared_ptr< queue_t > &q,
    const std::shared_ptr< command_pool_t > &p
  ) :
    created_from< queue_t >( q ),
    pool( p ) {}
  std::shared_ptr< bound_command_buffer_t > bound_command_pool_t::allocate(
    const command_buffer_allocate_info_t &create_info
  ) {
    return std::shared_ptr< bound_command_buffer_t >(
      new bound_command_buffer_t(
        shared_from_this(),
        pool->allocate( create_info )
      )
    );
  }
  std::shared_ptr< bound_command_buffer_t > bound_command_pool_t::allocate() {
    return std::shared_ptr< bound_command_buffer_t >(
      new bound_command_buffer_t(
        shared_from_this(),
        pool->allocate()
      )
    );
  }
}

