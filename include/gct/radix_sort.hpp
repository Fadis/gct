#ifndef GCT_RADIX_SORT_HPP
#define GCT_RADIX_SORT_HPP
#include <memory>
#include <gct/radix_sort_create_info.hpp>
#include <gct/spv_member_pointer.hpp>

namespace gct {

class buffer_t;
class compute;
class command_buffer_recorder_t;
class radix_sort :
  public property< radix_sort_create_info > {
public:
  radix_sort(
    const radix_sort_create_info&
  );
  void operator()(
    command_buffer_recorder_t&,
    std::uint32_t
  ) const;
private:
  std::uint32_t max_size = 0u;
  std::shared_ptr< buffer_t > local_offset;
  std::shared_ptr< buffer_t > workgroup_offset;
  std::shared_ptr< compute > local_sum;
  std::shared_ptr< compute > global_sum;
  std::shared_ptr< compute > sort;
  std::shared_ptr< compute > small_sort;
};

}

#endif

