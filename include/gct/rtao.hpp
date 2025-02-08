#ifndef GCT_RTAO_HPP
#define GCT_RTAO_HPP
#include <memory>
#include <gct/compute.hpp>
#include <gct/rtao_create_info.hpp>
#include <gct/property.hpp>

namespace gct {
  class image_view_t;
  class command_buffer_recorder_t;
  class rtao :
    public property< rtao_create_info > {
  public:
    rtao(
      const rtao_create_info &ci
    );
    void operator()(
      command_buffer_recorder_t &rec
    ) const;
    [[nodiscard]] const std::shared_ptr< image_view_t > &get_output() const {
      return props.output;
    }
  private:
    unsigned int width;
    unsigned int height;
    std::shared_ptr< image_view_t > previous_ao;
    std::shared_ptr< image_view_t > previous_history;
    std::shared_ptr< image_view_t > history;
    std::shared_ptr< compute > generate_ao;
    std::shared_ptr< compute > hgauss;
    std::shared_ptr< compute > vgauss;
  };
}

#endif

