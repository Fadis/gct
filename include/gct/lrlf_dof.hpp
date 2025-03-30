#ifndef GCT_LRLF_DOF_HPP
#define GCT_LRLF_DOF_HPP
#include <memory>
#include <gct/lrlf_dof_create_info.hpp>
#include <gct/property.hpp>
#include <gct/shader_graph.hpp>

namespace gct {
  class image_view_t;
  class shader_graph_builder;
  class lrlf_dof :
    public property< lrlf_dof_create_info > {
  public:
    lrlf_dof(
      const lrlf_dof_create_info &ci
    );
    shader_graph_vertex::subresult_type operator()(
      shader_graph_builder &b,
      const shader_graph_vertex::subresult_type &input,
      const shader_graph_vertex::subresult_type &coc
    ) const;
  private:
    std::shared_ptr< compute > h0;
    std::shared_ptr< compute > h1;
    std::shared_ptr< compute > h2;
    std::shared_ptr< compute > h3;
    std::shared_ptr< compute > h4;
    std::shared_ptr< compute > v0;
    std::shared_ptr< compute > v1;
    std::shared_ptr< compute > v2;
    std::shared_ptr< compute > v3;
    std::shared_ptr< compute > v4;
    std::shared_ptr< compute > mix;
  };
}

#endif

