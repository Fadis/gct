#include <gct/allocator.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/image.hpp>
#include <gct/image_view.hpp>
#include <gct/gbuffer.hpp>
#include <gct/graphics.hpp>
#include <gct/compute.hpp>
#include <gct/get_device.hpp>
#include <gct/device.hpp>
#include <gct/sampler_create_info.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/compute_pipeline.hpp>
#include <gct/pipeline_layout.hpp>
#include <gct/mappable_buffer.hpp>
#include <gct/load_obj_particle.hpp>
#include <gct/particle_buffer.hpp>

namespace gct {

particle_buffer::particle_buffer(
  const particle_buffer_create_info &ci
) : props( ci ) {
  std::tie(host_vertex_buffer,particle_count,aabb_) = load_obj_particle(
    props.filename,
    props.vamap,
    props.stride
  );
    
}

void particle_buffer::init(
  command_buffer_recorder_t &rec
) {
  vertex_buffer = rec.load_buffer(
    props.allocator,
    host_vertex_buffer.data(),
    host_vertex_buffer.size(),
    vk::BufferUsageFlagBits::eVertexBuffer
  );
  rec.barrier(
    vk::AccessFlagBits::eTransferWrite,
    vk::AccessFlagBits::eVertexAttributeRead,
    vk::PipelineStageFlagBits::eTransfer,
    vk::PipelineStageFlagBits::eVertexInput,
    vk::DependencyFlagBits( 0 ),
    {
      vertex_buffer
    },
    {}
  );
  host_vertex_buffer.clear();
}

}

