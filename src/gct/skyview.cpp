#include <gct/allocator.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/image.hpp>
#include <gct/image_view.hpp>
#include <gct/compute.hpp>
#include <gct/get_device.hpp>
#include <gct/device.hpp>
#include <gct/sampler_create_info.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/compute_pipeline.hpp>
#include <gct/pipeline_layout.hpp>
#include <gct/skyview.hpp>
#include <gct/color_space.hpp>

namespace gct {

skyview::skyview(
  const skyview_create_info &ci
) : props( ci ) {
  auto linear_sampler = get_device( *props.allocator_set.allocator ).get_sampler(
    gct::get_basic_linear_sampler_create_info()
  );
  transmittance = props.allocator_set.allocator->create_image_view(
    gct::image_create_info_t()
      .set_basic(
        gct::basic_2d_image( props.transmittance_width, props.transmittance_height )
          .setUsage(
            vk::ImageUsageFlagBits::eStorage|
            vk::ImageUsageFlagBits::eSampled|
            vk::ImageUsageFlagBits::eTransferSrc
          )
          .setFormat( props.format )
      ),
    VMA_MEMORY_USAGE_GPU_ONLY
  );
  multiscat = props.allocator_set.allocator->create_image_view(
    gct::image_create_info_t()
      .set_basic(
        gct::basic_2d_image( props.multiscat_size, props.multiscat_size )
          .setUsage(
            vk::ImageUsageFlagBits::eStorage|
            vk::ImageUsageFlagBits::eSampled|
            vk::ImageUsageFlagBits::eTransferSrc
          )
          .setFormat( props.format )
      ),
    VMA_MEMORY_USAGE_GPU_ONLY
  );
  output = props.allocator_set.allocator->create_image_view(
    gct::image_create_info_t()
      .set_basic(
        gct::basic_2d_image( props.skyview_width, props.skyview_height )
          .setUsage(
            vk::ImageUsageFlagBits::eStorage|
            vk::ImageUsageFlagBits::eSampled|
            vk::ImageUsageFlagBits::eTransferSrc
          )
          .setFormat( props.format )
      ),
    VMA_MEMORY_USAGE_GPU_ONLY
  );
  
  generate_transmittance = std::make_shared< gct::compute >(
    gct::compute_create_info()
      .set_allocator_set( props.allocator_set )
      .set_shader( props.transmittance_shader )
      .set_swapchain_image_count( 1 )
      .add_resource( { "dest_image", transmittance, vk::ImageLayout::eGeneral } )
  );
  generate_multiscat = std::make_shared< gct::compute >(
    gct::compute_create_info()
      .set_allocator_set( props.allocator_set )
      .set_shader( props.multiscat_shader )
      .set_swapchain_image_count( 1 )
      .add_resource( { "transmittance", linear_sampler, transmittance, vk::ImageLayout::eShaderReadOnlyOptimal } )
      .add_resource( { "dest_image", multiscat, vk::ImageLayout::eGeneral } )
  );
  generate_skyview = std::make_shared< gct::compute >(
    gct::compute_create_info()
      .set_allocator_set( props.allocator_set )
      .set_shader( props.skyview_shader )
      .set_swapchain_image_count( 1 )
      .add_resource( { "transmittance", linear_sampler, transmittance, vk::ImageLayout::eShaderReadOnlyOptimal } )
      .add_resource( { "multiscattering", linear_sampler, multiscat, vk::ImageLayout::eShaderReadOnlyOptimal } )
      .add_resource( { "dest_image", output, vk::ImageLayout::eGeneral } )
  );
}
void skyview::operator()(
  command_buffer_recorder_t &rec,
  const skyview_parameter &param
) const {
  const glm::vec4 light_pos = glm::vec4( float( std::cos( param.sun_angle ) ), float( std::sin( param.sun_angle ) ), 0.f, 0.f ) * 14960000000.0f;
  rec.set_image_layout( transmittance, vk::ImageLayout::eGeneral );
  rec.set_image_layout( multiscat, vk::ImageLayout::eGeneral );
  rec.set_image_layout( output, vk::ImageLayout::eGeneral );
  const auto rgb_to_xyz = get_rgb_to_xyz( param.space );
  {
    auto params = transmittance_pc()
      .set_sigma_ma( param.convert_to_xyz ? rgb_to_xyz * param.sigma_ma : param.sigma_ma )
      .set_sigma_oa( param.convert_to_xyz ? rgb_to_xyz * param.sigma_oa : param.sigma_oa )
      .set_sigma_rs( param.convert_to_xyz ? rgb_to_xyz * param.sigma_rs : param.sigma_rs )
      .set_sigma_ms( param.convert_to_xyz ? rgb_to_xyz * param.sigma_ms : param.sigma_ms )
      .set_ground_radius( param.ground_radius )
      .set_top_radius( param.top_radius );
    rec->pushConstants(
      **generate_transmittance->get_pipeline()->get_props().get_layout(),
      vk::ShaderStageFlagBits::eCompute,
      0u,
      sizeof( transmittance_pc ),
      reinterpret_cast< void* >( &params )
    );
  }
  (*generate_transmittance)( rec, 0u, props.transmittance_width, props.transmittance_height, 1u );
  rec.convert_image(
    transmittance->get_factory(),
    vk::ImageLayout::eShaderReadOnlyOptimal
  );
  {
    auto params = multiscat_pc()
      .set_sigma_ma( param.convert_to_xyz ? rgb_to_xyz * param.sigma_ma : param.sigma_ma )
      .set_sigma_oa( param.convert_to_xyz ? rgb_to_xyz * param.sigma_oa : param.sigma_oa )
      .set_sigma_rs( param.convert_to_xyz ? rgb_to_xyz * param.sigma_rs : param.sigma_rs )
      .set_sigma_ms( param.convert_to_xyz ? rgb_to_xyz * param.sigma_ms : param.sigma_ms )
      .set_ground_radius( param.ground_radius )
      .set_top_radius( param.top_radius )
      .set_g( param.g )
      .set_light_energy( param.light_energy );
    rec->pushConstants(
      **generate_multiscat->get_pipeline()->get_props().get_layout(),
      vk::ShaderStageFlagBits::eCompute,
      0u,
      sizeof( multiscat_pc ),
      reinterpret_cast< void* >( &params )
    );
  }
  (*generate_multiscat)( rec, 0u, props.multiscat_size, props.multiscat_size, 1u );
  rec.convert_image(
    multiscat->get_factory(),
    vk::ImageLayout::eShaderReadOnlyOptimal
  );
  {
    auto params = skyview_pc()
      .set_sigma_ma( param.convert_to_xyz ? rgb_to_xyz * param.sigma_ma : param.sigma_ma )
      .set_sigma_oa( param.convert_to_xyz ? rgb_to_xyz * param.sigma_oa : param.sigma_oa )
      .set_sigma_rs( param.convert_to_xyz ? rgb_to_xyz * param.sigma_rs : param.sigma_rs )
      .set_sigma_ms( param.convert_to_xyz ? rgb_to_xyz * param.sigma_ms : param.sigma_ms )
      .set_light_pos( light_pos )
      .set_ground_radius( param.ground_radius )
      .set_top_radius( param.top_radius )
      .set_g( param.g )
      .set_altitude( param.ground_radius + param.altitude )
      .set_light_energy( param.light_energy );
    rec->pushConstants(
      **generate_skyview->get_pipeline()->get_props().get_layout(),
      vk::ShaderStageFlagBits::eCompute,
      0u,
      sizeof( skyview_pc ),
      reinterpret_cast< void* >( &params )
    );
  }
  (*generate_skyview)( rec, 0u, props.skyview_width, props.skyview_height, 1u );
  rec.convert_image(
    output->get_factory(),
    vk::ImageLayout::eShaderReadOnlyOptimal
  );
}

}

