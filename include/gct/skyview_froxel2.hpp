#ifndef GCT_SKYVIEW_FROXEL2_HPP
#define GCT_SKYVIEW_FROXEL2_HPP
#include <memory>
#include <gct/skyview_froxel2_create_info.hpp>
#include <gct/property.hpp>
#include <gct/shader_graph.hpp>
#include <gct/buffer_pool.hpp>
#include <gct/gbuffer_format.hpp>

namespace gct {
  class image_view_t;
  class skyview_froxel2 :
    public property< skyview_froxel2_create_info > {
  public:
    skyview_froxel2(
      const skyview_froxel2_create_info &ci
    );
    shader_graph::vertex::combined_result_type operator()(
      shader_graph::builder &b,
      const shader_graph::vertex::subresult_type &gbuffer,
      const shader_graph::vertex::subresult_type &depth,
      const shader_graph::vertex::subresult_type &transmittance
    ) const;
    shader_graph::vertex::combined_result_type operator()(
      shader_graph::builder &b,
      const shader_graph::vertex::subresult_type &gbuffer,
      const shader_graph::vertex::subresult_type &position,
      const shader_graph::vertex::subresult_type &start,
      const shader_graph::vertex::subresult_type &next,
      const shader_graph::vertex::subresult_type &transmittance,
      const buffer_pool::buffer_descriptor &ppll_state_id
    ) const;
    shader_graph::vertex::combined_result_type operator()(
      shader_graph::builder &b,
      const shader_graph::vertex::subresult_type &gbuffer,
      const shader_graph::vertex::subresult_type &depth,
      const shader_graph::vertex::subresult_type &transmittance,
      gbuffer_format
    ) const;
    shader_graph::vertex::combined_result_type operator()(
      shader_graph::builder &b,
      const shader_graph::vertex::subresult_type &gbuffer,
      const shader_graph::vertex::subresult_type &position,
      const shader_graph::vertex::subresult_type &start,
      const shader_graph::vertex::subresult_type &next,
      const shader_graph::vertex::subresult_type &transmittance,
      const buffer_pool::buffer_descriptor &ppll_state_id,
      gbuffer_format
    ) const;
  private:
    std::shared_ptr< compute > generate;
    std::shared_ptr< compute > render;
  };
}

#endif

