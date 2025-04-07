#ifndef GCT_LRLF_DOF_HPP
#define GCT_LRLF_DOF_HPP
#include <memory>
#include <gct/lrlf_dof_create_info.hpp>
#include <gct/property.hpp>
#include <gct/shader_graph.hpp>

namespace gct {
  class image_view_t;
  class lrlf_dof :
    public property< lrlf_dof_create_info > {
  public:
    lrlf_dof(
      const lrlf_dof_create_info &ci
    );
    shader_graph::vertex::subresult_type operator()(
      shader_graph::builder &b,
      const shader_graph::vertex::subresult_type &input,
      const shader_graph::vertex::subresult_type &coc
    ) const;
  private:
    std::shared_ptr< compute > h0;
    std::shared_ptr< compute > h1;
    std::shared_ptr< compute > h2;
    std::shared_ptr< compute > h3;
    std::shared_ptr< compute > h4;
    std::shared_ptr< compute > v;
    std::shared_ptr< compute > mix;
  };
}

#endif

