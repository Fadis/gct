#ifndef GCT_HBAO2_HPP
#define GCT_HBAO2_HPP
#include <memory>
#include <gct/hbao2_create_info.hpp>
#include <gct/property.hpp>
#include <gct/shader_graph.hpp>

namespace gct {
  class image_view_t;
  class hbao2 :
    public property< hbao2_create_info > {
  public:
    hbao2(
      const hbao2_create_info &ci
    );
    shader_graph::vertex::combined_result_type operator()(
      shader_graph::builder &b,
      const shader_graph::vertex::subresult_type &p
    ) const;
  private:
    std::shared_ptr< compute > p0;
    std::shared_ptr< compute > p45;
    std::shared_ptr< compute > p90;
    std::shared_ptr< compute > p135;
    std::shared_ptr< compute > mix;
  };
}

#endif

