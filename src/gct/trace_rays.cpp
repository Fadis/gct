#include <iostream>
#include <fstream>
#include <gct/buffer.hpp>
#include <gct/device.hpp>
#include <gct/command_pool.hpp>
#include <gct/command_buffer.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/strided_device_address_region.hpp>
#include <nlohmann/json.hpp>
#ifdef VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME
namespace gct {
  void command_buffer_recorder_t::trace_rays(
    const strided_device_address_region_t &raygen,
    const strided_device_address_region_t &miss,
    const strided_device_address_region_t &hit,
    const strided_device_address_region_t &callable,
    std::uint32_t width,
    std::uint32_t height,
    std::uint32_t depth
  ) {
    vk::StridedDeviceAddressRegionKHR null_;
    (*get_factory())->traceRaysKHR(
      raygen ? &*raygen : &null_,
      miss ? &*miss : &null_,
      hit ? &*hit : &null_,
      callable ? &*callable : &null_,
      width,
      height,
      depth
    );
    if( raygen )
      get_factory()->unbound()->keep.push_back( raygen );
    if( miss )
      get_factory()->unbound()->keep.push_back( miss );
    if( hit )
      get_factory()->unbound()->keep.push_back( hit );
    if( callable )
      get_factory()->unbound()->keep.push_back( callable );
  }
}
#endif
