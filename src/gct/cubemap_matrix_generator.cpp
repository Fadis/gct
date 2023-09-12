#include <glm/vec4.hpp>
#include <gct/allocator.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/descriptor_set_layout.hpp>
#include <gct/compute_pipeline.hpp>
#include <gct/pipeline_layout.hpp>
#include <gct/buffer.hpp>
#include <gct/write_descriptor_set.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/cubemap.hpp>
#include <gct/cubemap_matrix_generator.hpp>

namespace gct {
  cubemap_matrix_generator::cubemap_matrix_generator(
    const std::shared_ptr< allocator_t > &allocator,
    const std::shared_ptr< descriptor_pool_t > &descriptor_pool,
    const std::shared_ptr< pipeline_cache_t > &pipeline_cache,
    const std::filesystem::path &mat_shader,
    unsigned int buffer_count
  ) {
    std::tie(descriptor_set_layout,pipeline) = pipeline_cache->get_pipeline( mat_shader );

    for( unsigned int buffer_index = 0u; buffer_index != buffer_count; ++buffer_index ) {
      storage.push_back( allocator->create_buffer(
        sizeof( matrices_t ),
        vk::BufferUsageFlagBits::eStorageBuffer,
        VMA_MEMORY_USAGE_GPU_ONLY
      ) );
      descriptor_set.push_back( descriptor_pool->allocate(
        descriptor_set_layout
      ) );
      descriptor_set.back()->update({
        { "matrices", storage.back() }
      });
    }
    prev.resize( buffer_count );
  }
  void cubemap_matrix_generator::operator()(
    command_buffer_recorder_t &rec,
    const glm::vec3 &pos,
    float near,
    float far,
    unsigned int buffer_index
  ) const {
    const bool same_as_before =
      prev[ buffer_index ].pos == pos &&
      prev[ buffer_index ].near == near &&
      prev[ buffer_index ].far == far;
    if( same_as_before ) return;
    {
      auto cube_mat_pc = gct::cubemat_push_constant_t()
        .set_ioffset( glm::ivec4( 0, 0, 0, 0 ) )
        .set_offset( glm::vec4( pos, 1.f ) )
        .set_near( near )
        .set_far( far );
      rec->pushConstants(
        **pipeline->get_props().get_layout(),
        vk::ShaderStageFlagBits::eCompute,
        0u,
        sizeof( gct::cubemat_push_constant_t ),
        reinterpret_cast< void* >( &cube_mat_pc )
      );
    }
    rec.bind(
      pipeline,
      { descriptor_set[ buffer_index ] }
    );
    rec.dispatch_threads( 6, 1, 1 );
    rec.barrier(
      vk::AccessFlagBits::eShaderWrite,
      vk::AccessFlagBits::eShaderRead,
      vk::PipelineStageFlagBits::eComputeShader,
      vk::PipelineStageFlagBits::eGeometryShader,
      vk::DependencyFlagBits( 0 ),
      { storage[ buffer_index ] },
      {}
    );
    prev[ buffer_index ].pos = pos;
    prev[ buffer_index ].near = near;
    prev[ buffer_index ].far = far;
  }
}

