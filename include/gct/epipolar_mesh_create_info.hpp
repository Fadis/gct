#ifndef GCT_EPIPOLAR_MESH_CREATE_INFO_HPP
#define GCT_EPIPOLAR_MESH_CREATE_INFO_HPP

#include <cstdint>
#include <memory>
#include <vector>
#include <filesystem>
#include <unordered_map>
#include <gct/setter.hpp>
#include <gct/named_resource.hpp>
#include <gct/vertex_attributes.hpp>

namespace gct {

class allocator_t;
class descriptor_pool_t;
class pipeline_cache_t;
struct epipolar_mesh_create_info {
  LIBGCT_SETTER( allocator )
  LIBGCT_SETTER( descriptor_pool )
  LIBGCT_SETTER( pipeline_cache )
  LIBGCT_SETTER( generate2_shader )
  LIBGCT_SETTER( generate3_shader )
  LIBGCT_SETTER( generate4_shader )
  LIBGCT_SETTER( pole_count )
  LIBGCT_SETTER( vertex_count )
  LIBGCT_SETTER( attrs )
  LIBGCT_SETTER( stride )
  LIBGCT_SETTER( resources )
  LIBGCT_SETTER( swapchain_image_count )
  epipolar_mesh_create_info &add_resource(
    const named_resource &n
  );
  epipolar_mesh_create_info &clear_resource(
    const named_resource &n
  );
  std::shared_ptr< allocator_t > allocator;
  std::shared_ptr< descriptor_pool_t > descriptor_pool;
  std::shared_ptr< pipeline_cache_t > pipeline_cache;
  std::filesystem::path generate2_shader;
  std::filesystem::path generate3_shader;
  std::filesystem::path generate4_shader;
  std::uint32_t pole_count = 0u;
  std::uint32_t vertex_count = 0u;
  std::unordered_map< vertex_attribute_usage_t, vertex_input_detail_t > attrs;
  unsigned int stride = 0u;
  std::vector< named_resource > resources;
  unsigned int swapchain_image_count = 1u;
};

}

#endif

