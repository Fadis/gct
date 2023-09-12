#include <gct/allocator.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/image.hpp>
#include <gct/image_view.hpp>
#include <gct/buffer.hpp>
#include <gct/compute.hpp>
#include <gct/tone_mapping.hpp>

namespace gct {

tone_mapping::tone_mapping(
  const tone_mapping_create_info &ci
) :
  props( ci ),
  tone_scale( 1.f, 10, 60 )
{
  for( std::size_t i = 0u; i != props.input.size(); ++i ) {
    tone.push_back(
      props.allocator->create_buffer(
        sizeof( tone_state_t ),
        vk::BufferUsageFlagBits::eStorageBuffer|
        vk::BufferUsageFlagBits::eTransferDst|
        vk::BufferUsageFlagBits::eTransferSrc,
        VMA_MEMORY_USAGE_GPU_ONLY
      )
    );
    tone_staging.push_back(
      props.allocator->create_buffer(
        sizeof( tone_state_t ),
        vk::BufferUsageFlagBits::eTransferDst|
        vk::BufferUsageFlagBits::eTransferSrc,
        VMA_MEMORY_USAGE_CPU_TO_GPU
      )
    );
  }
  calc_tone.reset( new gct::compute(
    gct::compute_create_info()
      .set_allocator( props.allocator )
      .set_descriptor_pool( props.descriptor_pool )
      .set_pipeline_cache( props.pipeline_cache )
      .set_shader( props.shader )
      .set_swapchain_image_count( props.input.size() )
      .set_resources( props.resources )
      .add_resource( { props.input_name, props.input } )
      .add_resource( { props.output_name, tone } )
  ) );

}

void tone_mapping::set(
  command_buffer_recorder_t &rec,
  unsigned int image_index
) const {
  {
    auto mapped = tone_staging[ image_index ]->map< tone_state_t >();
    tone_scale.set( mapped.begin()->max / 65536.f );
    mapped.begin()->max = 0u;
    mapped.begin()->scale = tone_scale.get();
  }
  rec.copy(
    tone_staging[ image_index ],
    tone[ image_index ]
  );
  rec.transfer_to_compute_barrier(
    { tone[ image_index ] },
    {}
  );
}

void tone_mapping::get(
  command_buffer_recorder_t &rec,
  unsigned int image_index
) const {
  rec.compute_barrier(
    {},
    { props.input[ image_index ]->get_factory() }
  );
  (*calc_tone)(
    rec,
    image_index,
    props.input[ image_index ]->get_factory()->get_props().get_basic().extent.width,
    props.input[ image_index ]->get_factory()->get_props().get_basic().extent.height,
    1u
  );
  rec.compute_to_transfer_barrier(
    { tone[ image_index ] },
    {}
  );
  rec.copy(
    tone[ image_index ],
    tone_staging[ image_index ]
  );
}

}

