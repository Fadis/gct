#include <boost/program_options.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <gct/get_extensions.hpp>
#include <gct/instance.hpp>
#include <gct/queue.hpp>
#include <gct/device.hpp>
#include <gct/allocator.hpp>
#include <gct/buffer.hpp>
#include <gct/mappable_buffer.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/descriptor_set_layout.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/pipeline_layout_create_info.hpp>
#include <gct/pipeline_layout.hpp>
#include <gct/compute_pipeline_create_info.hpp>
#include <gct/compute_pipeline.hpp>
#include <gct/submit_info.hpp>
#include <gct/shader_module.hpp>
#include <gct/fence.hpp>
#include <gct/wait_for_sync.hpp>
#include <gct/command_buffer.hpp>
#include <gct/command_pool.hpp>
#include <gct/named_resource.hpp>
#include <gct/compute.hpp>
#include <gct/shader_module_reflection.hpp>
#include <gct/spv_member_pointer.hpp>
#include <gct/key_value.hpp>
#include <gct/radix_sort.hpp>
#include <gct/common_sample_setup.hpp>
#include <gct/scene_graph.hpp>
#include <gct/get_library_path.hpp>
#include <gct/shader_graph.hpp>

int main( int argc, const char *argv[] ) {
  const gct::filter_setup res(
    argc, argv,
    {
      VK_KHR_IMAGE_FORMAT_LIST_EXTENSION_NAME,
      VK_KHR_MAINTENANCE1_EXTENSION_NAME,
      VK_EXT_PIPELINE_CREATION_FEEDBACK_EXTENSION_NAME,
      VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME,
      VK_EXT_CONDITIONAL_RENDERING_EXTENSION_NAME,
      VK_EXT_FRAGMENT_SHADER_INTERLOCK_EXTENSION_NAME,
      VK_NV_REPRESENTATIVE_FRAGMENT_TEST_EXTENSION_NAME,
      VK_KHR_MULTIVIEW_EXTENSION_NAME,
      VK_KHR_FRAGMENT_SHADING_RATE_EXTENSION_NAME,
      VK_KHR_MAINTENANCE_4_EXTENSION_NAME,
      VK_EXT_MESH_SHADER_EXTENSION_NAME//,
    },
    gct::descriptor_pool_create_info_t()
      .set_basic(
        vk::DescriptorPoolCreateInfo()
          .setFlags(
            vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet|
            vk::DescriptorPoolCreateFlagBits::eUpdateAfterBind
          )
          .setMaxSets( 65536 * 10 )
      )
      .set_descriptor_pool_size( vk::DescriptorType::eUniformBuffer, 16 )
      .set_descriptor_pool_size( vk::DescriptorType::eStorageBuffer, 65536*5 )
      .set_descriptor_pool_size( vk::DescriptorType::eCombinedImageSampler, 65536*2 )
      .set_descriptor_pool_size( vk::DescriptorType::eStorageImage, 65536*2 )
      .rebuild_chain()
  );

  const auto sg = std::make_shared< gct::scene_graph::scene_graph >(
    gct::scene_graph::scene_graph_create_info()
      .set_allocator_set( res.allocator_set )
      .add_master_shader( gct::get_system_shader_path() / "scene_graph" / "dummy" )
      .add_master_shader( gct::get_system_shader_path() / "occlusion_culling" / "roc" / "1.0" )
      .set_enable_linear( true )
  );

  gct::shader_graph::builder builder( sg->get_resource() );
  const auto height_desc = builder.call(
    builder.get_image_io_create_info(
      std::make_shared< gct::compute >(
        gct::compute_create_info()
          .set_allocator_set( res.allocator_set )
          .set_shader( gct::get_system_shader_path() / "terrain_noise" / "1.0" / "terrain_noise.comp.spv" )
          .set_scene_graph( sg->get_resource() )
      ),
      gct::image_io_plan()
        .add_output( "dest", 1024, 1024, vk::Format::eR32G32B32A32Sfloat )
        .set_dim( 1024, 1024, 1 )
        .set_node_name( "terrain_noise" )
    )
  )();
  auto scale_desc = builder.call(
    builder.get_image_io_create_info(
      std::make_shared< gct::compute >(
        gct::compute_create_info()
          .set_allocator_set( res.allocator_set )
          .set_shader( gct::get_system_shader_path() / "shift" / "1.0" / "shift.comp.spv" )
          .set_scene_graph( sg->get_resource() )
      ),
      gct::image_io_plan()
        .add_input( "src" )
        .add_output( "dest", "src", { 1.f, -1.f }, vk::Format::eR32Sfloat )
        .set_dim( "src", { 1.f, -1.f } )
        .set_node_name( "terrain_erosion.shift" )
    )
    .set_push_constant( "scale", 0.5f )
    .set_push_constant( "shift", 0.5f )
  )(
    gct::shader_graph::vertex::combined_result_type()
      .add( "src", height_desc )
  );
  builder.output( scale_desc );
  const auto compiled = builder();
  const auto modified_view = compiled.get_view( scale_desc );
  
  auto command_buffer = res.queue->get_command_pool()->allocate();
  {
    {
      auto recorder = command_buffer->begin();
      (*sg)( recorder );
    }
    command_buffer->execute_and_wait();
  }


  {
    {
      auto rec = command_buffer->begin();
      compiled( rec );
      rec.compute_to_transfer_barrier(
        gct::syncable()
          .add( modified_view )
      );
      rec.dump_image(
        res.allocator,
        modified_view->get_factory(),
        res.output,
        0
      );
    }
    command_buffer->execute_and_wait();
  }
}

