#ifndef GCT_HBAO2_HPP
#define GCT_HBAO2_HPP
#include <memory>
#include <vector>
#include <gct/hbao2_create_info.hpp>
#include <gct/property.hpp>
#include <gct/shader_graph.hpp>

namespace gct {
  class image_view_t;
  class shader_graph_builder;
  class hbao2 :
    public property< hbao2_create_info > {
  public:
    hbao2(
      const hbao2_create_info &ci
    );
    shader_graph_vertex::subresult_type operator()(
      shader_graph_builder &b,
      const shader_graph_vertex::subresult_type &p
    ) const;
  private:
    std::vector< std::shared_ptr< image_view_t > > temp;
    std::vector< compute > comp;
  };
}

#endif

