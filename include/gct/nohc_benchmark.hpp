#ifndef GCT_NOHC_BENCHMARK_HPP
#define GCT_NOHC_BENCHMARK_HPP
#include <chrono>
#include <memory>
#include <glm/vec4.hpp>
#include <gct/setter.hpp>
#include <gct/pool.hpp>
#include <gct/nohc_benchmark_create_info.hpp>
#include <gct/instance_list.hpp>
#include <gct/occlusion_query.hpp>
#include <gct/kdtree.hpp>
#include <gct/nohc_parameter.hpp>

namespace gct {

class image_view_t;
class compute;
class bound_command_buffer_t;
class command_buffer_recorder_t;
class gbuffer;
class descriptor_set_t;
class mappable_buffer_t;
class sampler_t;
namespace scene_graph {
  class scene_graph;
  class compiled_scene_graph;
  class instance_list;
  class instance;
}

class nohc_benchmark {
public:
  nohc_benchmark(
    const nohc_benchmark_create_info&
  );
  void operator()(
    std::shared_ptr< bound_command_buffer_t > &command_buffer
  );
  [[nodiscard]] nohc_parameter get() const {
    return nohc_parameter()
      .set_tos( tos )
      .set_top( top )
      .set_trs( trs )
      .set_trv( trv )
      .set_trf( trf );
  }
private:
  void draw(
    command_buffer_recorder_t &rec,
    float scale,
    bool high,
    const std::shared_ptr< gct::scene_graph::instance_list >&
  );
  [[nodiscard]] std::shared_ptr< gct::scene_graph::instance_list > setup(
    command_buffer_recorder_t &rec,
    float scale,
    bool high
  );
  nohc_benchmark_create_info props;
  std::chrono::nanoseconds highv_lowp;
  std::chrono::nanoseconds lowv_lowp;
  std::chrono::nanoseconds lowv_highp;
  std::chrono::nanoseconds oq1_lowp;
  std::chrono::nanoseconds oq1_highp;
  std::chrono::nanoseconds oq1000;
  float tos = 0.f;
  float top = 0.f;
  float trs = 0.f;
  float trv = 0.f;
  float trf = 0.f;
  std::shared_ptr< gbuffer > gbuf;
  std::shared_ptr< gbuffer > depth_gbuf;
  std::shared_ptr< gbuffer > shadow;
  std::shared_ptr< scene_graph::scene_graph > sg;
  std::shared_ptr< scene_graph::compiled_scene_graph > csg;
  std::shared_ptr< scene_graph::instance_list > il;
  pool< std::shared_ptr< scene_graph::instance > >::descriptor high_i_desc;
  pool< std::shared_ptr< scene_graph::instance > >::descriptor low_i_desc;
  std::shared_ptr< mappable_buffer_t > global_uniform;
  std::shared_ptr< descriptor_set_t > global_descriptor_set;
  std::shared_ptr< occlusion_query< unsigned int > > oq;
  std::shared_ptr< sampler_t > cubemap_sampler;
};

}

#endif

