#ifndef GCT_HIERARCHICAL_RASTER_OCCLUSION_CULLING_HPP
#define GCT_HIERARCHICAL_RASTER_OCCLUSION_CULLING_HPP
#include <memory>
#include <glm/mat4x4.hpp>
#include <gct/setter.hpp>
#include <gct/aabb.hpp>
#include <gct/hierarchical_raster_occlusion_culling_create_info.hpp>
#include <gct/instance_list.hpp>
#include <gct/index_range.hpp>

namespace gct {

class image_view_t;
class graphics;
class compute;
class gbuffer;
class onesweep;
class command_buffer_recorder_t;
class query_pool_t;
class buffer_t;

namespace scene_graph {
using hierarchical_raster_occlusion_culling_id_t = std::uint32_t;
class hierarchical_raster_occlusion_culling {
  struct push_constant_type {
    LIBGCT_SETTER( src_count )
    LIBGCT_SETTER( src_offset )
    LIBGCT_SETTER( dest_count )
    LIBGCT_SETTER( dest_offset )
    std::uint32_t src_count = 0u; 
    std::uint32_t src_offset = 0u;
    std::uint32_t dest_count = 0u;
    std::uint32_t dest_offset = 0u;
  };
  struct global_uniform_type {
    LIBGCT_SETTER( projection )
    LIBGCT_SETTER( camera )
    LIBGCT_SETTER( eye_pos )
    glm::mat4 projection;
    glm::mat4 camera;
    glm::vec4 eye_pos;
  };
public:
  struct node_type {
    LIBGCT_SETTER( aabb )
    LIBGCT_SETTER( parent )
    LIBGCT_SETTER( left )
    LIBGCT_SETTER( right )
    LIBGCT_SETTER( leaf )
    aabb4 aabb;
    std::uint32_t parent = 0u;
    std::uint32_t left = 0u;
    std::uint32_t right = 0u;
    std::uint32_t leaf = 0u; // instance id
  };
  hierarchical_raster_occlusion_culling(
    const hierarchical_raster_occlusion_culling_create_info&,
    const instance_list &il
  );
  void setup(
    command_buffer_recorder_t&
  );
  void operator()(
    command_buffer_recorder_t&,
    const glm::mat4 &projection,
    const glm::mat4 &camera,
    const glm::vec4 &eye_pos
  );
  [[nodiscard]] const std::shared_ptr< mappable_buffer_t > &get_sort_input() const {
    return sort_input;
  }
  [[nodiscard]] const std::shared_ptr< mappable_buffer_t > &get_sort_output() const {
    return sort_output;
  }
  [[nodiscard]] const std::shared_ptr< mappable_buffer_t > &get_bvh() const {
    return bvh;
  }
  [[nodiscard]] const std::shared_ptr< mappable_buffer_t > &get_visibility() const {
    return visibility;
  }
private:
  hierarchical_raster_occlusion_culling_create_info props;
  std::shared_ptr< scene_graph_resource > resource;
  std::shared_ptr< gbuffer > output;
  std::shared_ptr< compute > setup_sort;
  std::shared_ptr< onesweep > sort;
  std::shared_ptr< compute > setup_leaf;
  std::shared_ptr< compute > setup_node;
  std::shared_ptr< compute > mark_leaf;
  std::shared_ptr< compute > mark_node;
  std::shared_ptr< graphics > evaluate;
  std::shared_ptr< mappable_buffer_t > vertex_buffer;
  std::shared_ptr< mappable_buffer_t > sort_input;
  std::shared_ptr< mappable_buffer_t > sort_output;
  std::shared_ptr< mappable_buffer_t > bvh;
  std::shared_ptr< mappable_buffer_t > visibility;
  std::shared_ptr< mappable_buffer_t > global_uniform;
  std::vector< index_range > node_offset;
  std::uint32_t node_count = 0u;
  std::uint32_t leaf_count = 0u;
};

}

}

#endif

