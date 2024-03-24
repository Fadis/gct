#include <gct/device.hpp>
#include <gct/shader_module_reflection.hpp>
#include <gct/descriptor_pool_create_info.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/descriptor_set_layout_create_info.hpp>
#include <gct/descriptor_set.hpp>
#include <gct/sampler_create_info.hpp>
#include <gct/sampler.hpp>
#include <gct/allocator.hpp>
#include <gct/mappable_buffer.hpp>
#include <gct/get_device.hpp>
#include <gct/scene_graph.hpp>

namespace gct {
  scene_graph::scene_graph(
    const scene_graph_create_info &ci
  ) : props( new scene_graph_create_info( ci ) ), resource( new scene_graph_resource() ) {
    auto &device = get_device( *props->allocator );
    shader_module_reflection_t reflection( props->master_shader );
    resource->descriptor_set_layout = device.get_descriptor_set_layout( reflection );
    resource->descriptor_set = props->descriptor_pool->allocate( resource->descriptor_set_layout );
    resource->matrix.reset( new matrix_pool(
      matrix_pool_create_info( props->matrix )
        .set_allocator( props->allocator )
        .set_descriptor_pool( props->descriptor_pool )
        .set_pipeline_cache( props->pipeline_cache )
        .set_external_descriptor_set( resource->descriptor_set )
    ) );
    resource->aabb.reset( new aabb_pool(
      aabb_pool_create_info( props->aabb )
        .set_allocator( props->allocator )
        .set_descriptor_pool( props->descriptor_pool )
        .set_pipeline_cache( props->pipeline_cache )
        .set_external_descriptor_set( resource->descriptor_set )
    ) );
    resource->texture.reset( new texture_pool(
      texture_pool_create_info( props->texture )
        .set_allocator( props->allocator )
        .set_descriptor_pool( props->descriptor_pool )
        .set_descriptor_set( resource->descriptor_set )
    ) );
    resource->sampler.reset( new sampler_pool(
      sampler_pool_create_info( props->sampler )
        .set_allocator( props->allocator )
        .set_descriptor_pool( props->descriptor_pool )
        .set_descriptor_set( resource->descriptor_set )
    ) );
    resource->resource_index.reset( new buffer_pool(
      buffer_pool_create_info( props->resource_index )
        .set_allocator( props->allocator )
        .set_descriptor_pool( props->descriptor_pool )
        .set_pipeline_cache( props->pipeline_cache )
        .set_external_descriptor_set( resource->descriptor_set )
    ) );
    resource->visibility.reset( new buffer_pool(
      buffer_pool_create_info( props->visibility )
        .set_allocator( props->allocator )
        .set_descriptor_pool( props->descriptor_pool )
        .set_pipeline_cache( props->pipeline_cache )
        .set_external_descriptor_set( resource->descriptor_set )
    ) );
    root_node.reset( new node(
      props,
      resource,
      glm::mat4(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
      )
    ) );
  }
}

