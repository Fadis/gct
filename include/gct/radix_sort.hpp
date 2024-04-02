#ifndef GCT_RADIX_SORT_HPP
#define GCT_RADIX_SORT_HPP
#include <memory>
#include <vector>
#include <gct/radix_sort_create_info.hpp>

namespace gct {

class buffer_t;
class compute;
class radix_sort {
public:
  radix_sort(
    const radix_sort_create_info&
  );
  void operator()(
    command_buffer_recorder_t&,
  ) const;
private:
  radix_sort_create_info props;
  std::shared_ptr< compute > sort;
};

}

#endif

