#ifndef GCT_IO_CONTEXT_HPP
#define GCT_IO_CONTEXT_HPP
#include <boost/asio/io_context.hpp>
#include <thread>
#include <vector>
#include <memory>
namespace gct {
  boost::asio::io_context &get_io_context();
  class thread_pool {
  public:
    static void init();
  private:
    thread_pool();
    ~thread_pool();
    std::vector< std::thread > threads;
    std::unique_ptr< boost::asio::io_context::work > keep;
  };
  template< typename F >
  void async(
    unsigned int max,
    const F &func
  ) {
    const auto jobs = std::min( std::thread::hardware_concurrency(), max );
    for( unsigned int i = 0u; i != jobs; ++i )
      get_io_context().post( func );
  }
}
#endif

