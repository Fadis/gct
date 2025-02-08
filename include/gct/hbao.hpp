#ifndef GCT_HBAO_HPP
#define GCT_HBAO_HPP
#include <memory>
#include <vector>
#include <gct/hbao_create_info.hpp>
#include <gct/property.hpp>

namespace gct {
  class image_view_t;
  class command_buffer_recorder_t;
  class hbao :
    public property< hbao_create_info > {
  public:
    hbao(
      const hbao_create_info &ci
    );
    void operator()(
      command_buffer_recorder_t &rec,
      unsigned int image_index = 0u
    ) const;
    [[nodiscard]] const std::vector< std::shared_ptr< image_view_t > > &get_output() const {
      return props.output;
    }
  private:
    unsigned int width;
    unsigned int height;
    std::vector< std::shared_ptr< image_view_t > > temp;
    std::vector< compute > comp;
  };
}

#endif

