#ifndef GCT_COMMON_SAMPLE_SETUP_HPP
#define GCT_COMMON_SAMPLE_SETUP_HPP
#include <string>
#include <vector>
#include <memory>
#include <gct/allocator_set.hpp>

namespace gct {

class instance_t;
class surface_t;
class device_t;
class queue_t;
class swapchain_t;
class image_t;
class image_view_t;
class descriptor_pool_t;
class descriptor_pool_create_info_t;
class pipeline_cache_t;
class allocator_t;
class glfw_window;
struct common_sample_setup {
  common_sample_setup(
    int argc,
    const char *argv[],
    const std::vector< const char* > &device_extensions,
    const descriptor_pool_create_info_t &dpci,
    bool enable_glfw = true,
    bool enable_device_address = false
  );
  std::string walk_state_filename;
  std::string model_filename;
  float ambient_level;
  std::shared_ptr< glfw_window > window;
  std::shared_ptr< instance_t > instance;
  std::uint32_t width;
  std::uint32_t height;
  std::uint32_t light_count;
  std::shared_ptr< surface_t > surface;
  std::shared_ptr< device_t > device;
  std::shared_ptr< queue_t > queue;
  std::shared_ptr< swapchain_t > swapchain;
  std::vector< std::shared_ptr< image_t > > swapchain_images;
  std::vector< std::shared_ptr< image_view_t > > swapchain_image_views;
  std::shared_ptr< descriptor_pool_t > descriptor_pool;
  std::shared_ptr< pipeline_cache_t > pipeline_cache;
  std::shared_ptr< allocator_t > allocator;
  allocator_set_t allocator_set;
  bool force_geometry = false;
};

}

#endif

