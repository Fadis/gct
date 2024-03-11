#ifndef GCT_LINEAR_ALLOCATOR_HPP
#define GCT_LINEAR_ALLOCATOR_HPP
#include <cstdint>
#include <boost/container/flat_set.hpp>
#include <gct/linear_allocator_create_info.hpp>

namespace gct {

class linear_allocator {
public:
  linear_allocator(
    const linear_allocator_create_info &ci
  );
  const linear_allocator_create_info &get_props() const {
    return props;
  }
  std::uint32_t allocate();
  void release( std::uint32_t i );
private:
  linear_allocator_create_info props;
  boost::container::flat_set< std::uint32_t > deallocated;
  std::uint32_t tail = 0u;
};

}

#endif

