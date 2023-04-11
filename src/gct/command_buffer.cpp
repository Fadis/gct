#include <gct/buffer.hpp>
#include <gct/allocator.hpp>
#include <gct/device.hpp>
#include <gct/command_pool.hpp>
#include <OpenImageIO/imageio.h>
#include <OpenImageIO/version.h>
#include <gct/device.hpp>
#include <gct/queue.hpp>
#include <gct/fence.hpp>
#include <gct/submit_info.hpp>
#include <gct/command_pool.hpp>
#include <gct/command_buffer.hpp>

namespace gct {
  command_buffer_t::command_buffer_t(
    const std::shared_ptr< command_pool_t > &pool,
    const command_buffer_allocate_info_t &create_info
  ) :
    created_from< command_pool_t >( pool ),
    props( create_info ) {
    auto basic = props.get_basic();
    basic
      .setCommandPool( **pool )
      .setCommandBufferCount( 1 );
    props
      .set_basic( std::move( basic ) )
      .rebuild_chain();
    handle = std::move( (*pool->get_factory())->allocateCommandBuffersUnique( props.get_basic() )[ 0 ] );
  }
  void command_buffer_t::reset() {
    handle->reset( vk::CommandBufferResetFlags( 0 ) );
    keep.clear();
    cbs.clear();
  }
  void command_buffer_t::on_executed( vk::Result result ) {
    for( auto &cb: cbs ) {
      cb( result );        
    }
    reset();
  }
  bound_command_buffer_t::bound_command_buffer_t(
    const std::shared_ptr< bound_command_pool_t > &pool,
    const std::shared_ptr< command_buffer_t > &buffer_
  ) :
    created_from< bound_command_pool_t >( pool ),
    buffer( buffer_ ) {
    fence = pool->get_factory()->get_factory()->get_fence();
  }
  void bound_command_buffer_t::execute(
    const submit_info_t &submit_info_
  ) {
    submit_info_t submit_info = submit_info_;
    auto basic = submit_info.get_basic();
    basic
      .setCommandBufferCount( 1 )
      .setPCommandBuffers( &**buffer );
    submit_info
      .set_basic( basic )
      .rebuild_chain();
    (*get_factory()->get_factory())->submit(
      submit_info.get_basic(),
      **fence
    );
    executing = true;
  }
  void bound_command_buffer_t::wait_for_executed() {
    wait_for_executed( UINT64_MAX );
  }
  bool bound_command_buffer_t::wait_for_executed( std::uint64_t timeout ) {
    if( !executing ) return true;
    auto result = (*get_factory()->get_factory()->get_factory())->waitForFences( 1, &**fence, true, timeout );
    if( result == vk::Result::eSuccess ) {
      executing = false;
      auto reset_result = (*get_factory()->get_factory()->get_factory())->resetFences( 1, &**fence );
      if( reset_result != vk::Result::eSuccess ) throw -1;
      unbound()->on_executed( result );
      return true;
    }
    if( result == vk::Result::eTimeout ) return false;
    else {
      executing = false;
      unbound()->on_executed( result );
      vk::throwResultException( vk::Result( result ), "wait_for_executed failed." );
    }
  }
  void bound_command_buffer_t::execute_and_wait() {
    execute(
      gct::submit_info_t()
    );
    wait_for_executed();
  }
  command_buffer_recorder_t bound_command_buffer_t::begin(
    const command_buffer_begin_info_t &begin_info
  ) {
    return command_buffer_recorder_t(
      shared_from_this(),
      begin_info
    );
  }
  command_buffer_recorder_t bound_command_buffer_t::begin(
    vk::CommandBufferUsageFlags usage
  ) {
    return begin(
      command_buffer_begin_info_t()
        .set_basic(
          vk::CommandBufferBeginInfo()
            .setFlags( usage )
        )
        .rebuild_chain()
    );
  }
  command_buffer_recorder_t bound_command_buffer_t::begin() {
    return begin( vk::CommandBufferUsageFlagBits::eOneTimeSubmit );
  }
}

