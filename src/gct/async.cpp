#include <gct/async.hpp>
#include <gct/io_context.hpp>
#include <gct/deferred_operation.hpp>
#include <gct/device.hpp>
#include <gct/get_device.hpp>
namespace gct {
  std::future< vk::Result > async(
    const std::shared_ptr< deferred_operation_t > &deferred_operation
  ) {
    const auto max = get_device( *deferred_operation )->getDeferredOperationMaxConcurrencyKHR( **deferred_operation );
    std::shared_ptr< std::promise< vk::Result > > p( new std::promise< vk::Result > () );
    auto f = p->get_future();
    async(
      max,
      [p,deferred_operation]() {
        auto &device = get_device( *deferred_operation );
        auto result = device->deferredOperationJoinKHR( **deferred_operation );
        if( result == vk::Result::eSuccess ) {
          p->set_value( device->getDeferredOperationResultKHR( **deferred_operation ) );
        }
      }
    );
    return f;
  }
  void async(
    const std::shared_ptr< deferred_operation_t > &deferred_operation,
    std::function< void( vk::Result ) > &cb
  ) {
    const auto max = get_device( *deferred_operation )->getDeferredOperationMaxConcurrencyKHR( **deferred_operation );
    async(
      max,
      [cb,deferred_operation]() {
        auto &device = get_device( *deferred_operation );
        auto result = device->deferredOperationJoinKHR( **deferred_operation );
        if( result == vk::Result::eSuccess ) {
          cb( device->getDeferredOperationResultKHR( **deferred_operation ) );
        }
      }
    );
  }
}
