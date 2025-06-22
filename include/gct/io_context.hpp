#ifndef GCT_IO_CONTEXT_HPP
#define GCT_IO_CONTEXT_HPP
#include <boost/version.hpp>
#include <boost/asio/io_context.hpp>
#if BOOST_VERSION >= 106600
#include <boost/asio/post.hpp>
#endif
#include <thread>
#include <vector>
#if BOOST_VERSION < 105800
#include <memory>
#endif
namespace gct {
  boost::asio::io_context &get_io_context();
  class thread_pool {
  public:
    static void init();
  private:
    thread_pool();
    ~thread_pool();
    std::vector< std::thread > threads;
#if BOOST_VERSION < 105800
    std::unique_ptr< boost::asio::io_context::work > keep;
#else
    boost::asio::executor_work_guard<boost::asio::io_context::executor_type> keep;
#endif
  };
  template< typename F >
  void async(
    unsigned int max,
    const F &func
  ) {
    const auto jobs = std::min( std::thread::hardware_concurrency(), max );
    for( unsigned int i = 0u; i != jobs; ++i )
#if BOOST_VERSION < 106600
      get_io_context().post( func );
#else
      boost::asio::post( get_io_context(), func );
#endif
  }
}
#endif

