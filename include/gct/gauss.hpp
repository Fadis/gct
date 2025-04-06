#ifndef GCT_GAUSS_HPP
#define GCT_GAUSS_HPP
#include <memory>
#include <gct/gauss_create_info.hpp>
#include <gct/property.hpp>
#include <gct/shader_graph.hpp>

namespace gct {
  class image_view_t;
  class gauss :
    public property< gauss_create_info > {
  public:
    gauss(
      const gauss_create_info &ci
    );
    shader_graph::vertex::combined_result_type operator()(
      shader_graph::builder &b,
      const shader_graph::vertex::subresult_type &p
    ) const;
  private:
    std::shared_ptr< compute > horizontal;
    std::shared_ptr< compute > vertical;
  };
}

#endif

