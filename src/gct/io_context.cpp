#include <boost/asio/executor_work_guard.hpp>
#include <gct/io_context.hpp>
#include <gct/deferred_operation.hpp>
#include <gct/device.hpp>
#include <gct/get_device.hpp>
namespace gct {
  boost::asio::io_context &get_io_context() {
    static boost::asio::io_context io_context;
    return io_context;
  }
  thread_pool::thread_pool() :
#if BOOST_VERSION < 105800
    keep( new boost::asio::io_context::work( get_io_context() ) )
#else
    keep( boost::asio::make_work_guard( get_io_context() ) )
#endif
  {
    for( unsigned int i = 0u; i != std::thread::hardware_concurrency(); ++i ) {
      threads.push_back( std::thread( []() {
        get_io_context().run();
      } ) );
    }
  }
  thread_pool::~thread_pool() {
    keep.reset();
    for( auto &t: threads )
      t.join();
  }
  void thread_pool::init() {
    static thread_pool instance;
  }
}

