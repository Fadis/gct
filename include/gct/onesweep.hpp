#ifndef GCT_ONESWEEP_HPP
#define GCT_ONESWEEP_HPP
#include <memory>
#include <gct/onesweep_create_info.hpp>
#include <gct/property.hpp>

namespace gct {

class buffer_t;
class compute;
class command_buffer_recorder_t;
class onesweep :
  public property< onesweep_create_info > {
public:
  onesweep(
    const onesweep_create_info&
  );
  void operator()(
    command_buffer_recorder_t&,
    std::uint32_t
  ) const;
private:
  std::uint32_t max_size = 0u;
  std::shared_ptr< buffer_t > histgram_buffer;
  std::shared_ptr< buffer_t > local_offset;
  std::shared_ptr< buffer_t > workgroup_offset;
  std::shared_ptr< compute > histgram;
  std::shared_ptr< compute > histgram_sum;
  std::shared_ptr< compute > local_sum;
  std::shared_ptr< compute > global_sum;
  std::shared_ptr< compute > sort;
  std::shared_ptr< compute > small_sort;
};

}

#endif

