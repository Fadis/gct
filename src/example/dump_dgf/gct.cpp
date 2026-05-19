#include <boost/program_options.hpp>
#include <nlohmann/json.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <gct/get_extensions.hpp>
#include <gct/instance.hpp>
#include <gct/glfw.hpp>
#include <gct/queue.hpp>
#include <gct/device.hpp>
#include <gct/allocator.hpp>
#include <gct/device_create_info.hpp>
#include <gct/buffer.hpp>
#include <gct/mappable_buffer.hpp>
#include <gct/image_create_info.hpp>
#include <gct/image.hpp>
#include <gct/swapchain.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/descriptor_set_layout.hpp>
#include <gct/sampler_create_info.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/pipeline_layout_create_info.hpp>
#include <gct/pipeline_layout.hpp>
#include <gct/buffer_view_create_info.hpp>
#include <gct/compute_pipeline_create_info.hpp>
#include <gct/compute_pipeline.hpp>
#include <gct/render_pass_begin_info.hpp>
#include <gct/submit_info.hpp>
#include <gct/shader_module.hpp>
#include <gct/fence.hpp>
#include <gct/wait_for_sync.hpp>
#include <gct/present_info.hpp>
#include <gct/gltf.hpp>
#include <gct/color.hpp>
#include <gct/timer.hpp>
#include <gct/cubemap.hpp>
#include <gct/gbuffer.hpp>
#include <gct/hysteresis.hpp>
#include <gct/command_buffer.hpp>
#include <gct/command_pool.hpp>
#include <gct/framebuffer.hpp>
#include <gct/render_pass.hpp>
#include <gct/cubemap_matrix_generator2.hpp>
#include <gct/image_filter.hpp>
#include <gct/named_resource.hpp>
#include <gct/compute.hpp>
#include <gct/common_sample_setup.hpp>
#include <gct/tone_mapping.hpp>
#include <gct/hbao2.hpp>
#include <gct/gauss.hpp>
#include <gct/scene_graph.hpp>
#include <gct/gltf2.hpp>
#include <gct/gltf2_create_info.hpp>
#include <gct/image_pool.hpp>
#include <gct/compiled_scene_graph.hpp>
#include <gct/instance_list.hpp>
#include <gct/kdtree.hpp>
#include <gct/skyview.hpp>
#include <gct/skyview_froxel2.hpp>
#include <gct/skyview_froxel2_create_info.hpp>
#include <gct/skyview_froxel2_param.hpp>
#include <gct/lrlf_dof.hpp>
#include <gct/lens_flare.hpp>
#include <gct/af_state.hpp>
#include <gct/image_io.hpp>
#include <gct/get_library_path.hpp>
#include <gct/color_attachment_name.hpp>
#include <gct/gbuffer_format.hpp>

int main( int argc, const char *argv[] ) {
  const gct::common_sample_setup res(
    argc, argv,
    {
      VK_KHR_SWAPCHAIN_EXTENSION_NAME,
      VK_KHR_SWAPCHAIN_MUTABLE_FORMAT_EXTENSION_NAME,
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
      .add_master_shader( gct::get_system_shader_path() / "dump_dgf" / "1.0" )
      .set_enable_linear( true )
  );

  gct::gltf::gltf2 doc(
    gct::gltf::gltf2_create_info()
      .set_filename( res.model_filename )
      .set_graph( sg )
      .set_root( sg->get_root_node() )
      .set_aspect_ratio( float( res.width )/float( res.height ) )
  );

  const auto il = std::make_shared< gct::scene_graph::instance_list >(
    gct::scene_graph::instance_list_create_info()
      .set_parallel_mode3( true ),
    *sg,
    doc.get_descriptor(),
    res.primitive_filter
  );

  auto command_buffer = res.queue->get_command_pool()->allocate();
  {
    {
      auto recorder = command_buffer->begin();
      (*sg)( recorder );
    }
    command_buffer->execute_and_wait();
  }

  const auto rp = il->get_draw_list();
  if( rp.empty() ) return 0;

  const auto prim = sg->get_resource()->prim.get( rp[ 0 ].prim );

  const auto unique_vertex_count = prim->mesh.unique_vertex_count;
  const auto meshlet_count = prim->mesh.meshlet_count;

  std::shared_ptr< gct::mappable_buffer_t > dest =
    res.allocator->create_mappable_buffer(
       unique_vertex_count * 3u * sizeof( float ),
      vk::BufferUsageFlagBits::eStorageBuffer
    );
  
  auto dump_dgf = gct::compute(
    gct::compute_create_info()
      .set_allocator_set( res.allocator_set )
      .set_shader( gct::get_system_shader_path() / "dump_dgf" / "1.0" / "dump_dgf.comp.spv" )
      .set_swapchain_image_count( 1u )
      .set_scene_graph( sg->get_resource() )
      .add_resource( { "dest", dest } )
  );
  std::cout << "offset : " << il->get_shape().offset << std::endl;
  std::cout << "unique vertex count : " << unique_vertex_count << std::endl;
  std::cout << "meshlet count : " << meshlet_count << std::endl;
  dump_dgf.set_push_constant( "offset", il->get_shape().offset );
  dump_dgf.set_push_constant( "meshlet_id", 0u );

  {
    auto command_buffer = res.queue->get_command_pool()->allocate();
    {
      auto recorder = command_buffer->begin();
      il->setup_resource_pair_buffer( recorder );
      dump_dgf( recorder, 0u, meshlet_count *32u, 1u, 1u );
      recorder.barrier(
        gct::syncable()
          .add( dest )
      );
      recorder.sync_to_host( dest );
    }
    command_buffer->execute_and_wait();
  }
  
  std::vector< float > result;
  {
    auto mapped = dest->map< float >();
    std::copy( mapped.begin(), mapped.end(), std::back_inserter( result ) );
  }
  for( unsigned int i = 0u; i != result.size(); i += 3 ) {
    std::cout << result[ i ] << ", " << result[ i + 1 ] << ", " << result[ i + 2 ] << std::endl;
  }

}

