#ifndef LIBGCT_INCLUDE_GCT_GLTF_HPP
#define LIBGCT_INCLUDE_GCT_GLTF_HPP
#include <tuple>
#include <memory>
#include <cstdint>
#include <vector>
#include <filesystem>
#include <unordered_map>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <gct/exception.hpp>
#include <gct/setter.hpp>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnarrowing"
#include <fx/gltf.h>
#pragma GCC diagnostic pop
#include <vulkan/vulkan.hpp>
#include <gct/shader_flag.hpp>
#include <gct/descriptor_image_info.hpp>
#include <gct/buffer_window.hpp>
namespace gct {
  class image_t;
  class image_view_t;
  class command_buffer_recorder_t;
  class allocator_t;
  class buffer_t;
  class sampler_t;
  class device_t;
  class pipeline_cache_t;
  class pipeline_layout_t;
  class pipeline_t;
  class descriptor_pool_t;
  class descriptor_set_layout_t;
  class descriptor_set_t;
  class render_pass_t;
  class graphics_pipeline_t;
}
namespace gct::gltf {
  LIBGCT_EXCEPTION( runtime_error, invalid_gltf, "invalid glTF" )
  vk::Filter to_vulkan_mag_filter( fx::gltf::Sampler::MagFilter v );
  vk::Filter to_vulkan_min_filter( fx::gltf::Sampler::MinFilter v );
  vk::SamplerMipmapMode to_vulkan_mipmap_mode( fx::gltf::Sampler::MinFilter v );
  vk::SamplerAddressMode to_vulkan_address_mode( fx::gltf::Sampler::WrappingMode v );
  std::uint32_t to_size( fx::gltf::Accessor::ComponentType type );
  std::uint32_t to_size( fx::gltf::Accessor::Type type );
  std::uint32_t to_size(
    fx::gltf::Accessor::ComponentType componentType,
    fx::gltf::Accessor::Type type
  );
  vk::Format to_vulkan_format(
    fx::gltf::Accessor::ComponentType componentType,
    fx::gltf::Accessor::Type type,
    bool normalize
  );
  vk::IndexType to_vulkan_index_type( fx::gltf::Accessor::ComponentType v );
  struct image_pair_t {
    LIBGCT_SETTER( unorm )
    LIBGCT_SETTER( srgb )
    LIBGCT_SETTER( unorm_view )
    LIBGCT_SETTER( srgb_view )
    std::shared_ptr< image_t > unorm;
    std::shared_ptr< image_t > srgb;
    std::shared_ptr< image_view_t > unorm_view;
    std::shared_ptr< image_view_t > srgb_view;
  };
  using images_t = std::vector< image_pair_t >;
  images_t create_image(
    const fx::gltf::Document &doc,
    command_buffer_recorder_t &command_buffer,
    const std::shared_ptr< allocator_t > &allocator,
    const std::filesystem::path cd
  );
  using buffers_t = std::vector< std::shared_ptr< buffer_t > >;
  buffers_t create_buffer(
    const fx::gltf::Document &doc,
    command_buffer_recorder_t &command_buffer,
    const std::shared_ptr< allocator_t > &allocator,
    const std::filesystem::path cd,
    bool ray_trace
  );
  struct camera_t {
    LIBGCT_SETTER( projection_matrix )
    LIBGCT_SETTER( camera_pos )
    LIBGCT_SETTER( camera_direction )
    glm::vec3 camera_pos;
    glm::vec3 camera_direction;
    glm::mat4 projection_matrix;
  };
  using cameras_t = std::vector< camera_t >;
  cameras_t create_camera(
    const fx::gltf::Document &doc,
    float
  );
  struct point_light_t {
    LIBGCT_SETTER( location )
    LIBGCT_SETTER( color )
    LIBGCT_SETTER( intensity )
    glm::vec3 location;
    glm::vec3 color;
    float intensity;
  };
  using point_lights_t = std::vector< point_light_t >;
  point_lights_t create_point_light(
    const fx::gltf::Document &doc
  );
  std::shared_ptr< sampler_t > create_sampler(
    const fx::gltf::Document &doc,
    int32_t index,
    const std::shared_ptr< device_t > &device
  );
  using samplers_t = std::vector< std::shared_ptr< sampler_t > >;
  samplers_t create_sampler(
    const fx::gltf::Document &doc,
    const std::shared_ptr< device_t > &device
  );
  std::shared_ptr< sampler_t > create_default_sampler(
    const std::shared_ptr< device_t > &device
  );
  std::shared_ptr< sampler_t > create_nomip_sampler(
    const std::shared_ptr< device_t > &device
  );


  struct texture_t {
    LIBGCT_SETTER( unorm )
    LIBGCT_SETTER( srgb )
    descriptor_image_info_t unorm;
    descriptor_image_info_t srgb;
  };
  //
  using textures_t = std::vector< texture_t >;
  texture_t create_texture(
    const fx::gltf::Document &doc,
    int32_t index,
    const images_t &images,
    const samplers_t &samplers,
    const std::shared_ptr< sampler_t > &default_samplers
  );
  std::shared_ptr< texture_t > create_texture(
    int32_t index,
    const image_t &image,
    const samplers_t &samplers,
    const std::shared_ptr< sampler_t > &default_samplers
  );
  texture_t create_texture(
    const std::shared_ptr< image_view_t > &image,
    const std::shared_ptr< sampler_t > &sampler
  );
  textures_t create_texture(
    const fx::gltf::Document &doc,
    const images_t &images,
    const samplers_t &samplers,
    const std::shared_ptr< sampler_t > &default_samplers
  );
  struct primitive_t {
    primitive_t() : indexed( false ), count( 0 ) {}
    LIBGCT_SETTER( pipeline )
    LIBGCT_SETTER( pipeline_layout )
    LIBGCT_SETTER( vertex_buffer )
    LIBGCT_SETTER( indexed )
    LIBGCT_SETTER( index_buffer )
    LIBGCT_SETTER( descriptor_set )
    LIBGCT_SETTER( count )
    LIBGCT_SETTER( index_buffer_type )
    LIBGCT_SETTER( min )
    LIBGCT_SETTER( max )
    LIBGCT_SETTER( uniform_buffer )
    LIBGCT_SETTER( vertex_input_binding )
    LIBGCT_SETTER( vertex_input_attribute )
    std::vector< std::shared_ptr< graphics_pipeline_t > > pipeline;
    std::shared_ptr< pipeline_layout_t > pipeline_layout;
    std::unordered_map< std::uint32_t, buffer_window_t > vertex_buffer;
    bool indexed;
    buffer_window_t index_buffer;
    std::shared_ptr< descriptor_set_t > descriptor_set;
    uint32_t count;
    vk::IndexType index_buffer_type;
    glm::vec3 min;
    glm::vec3 max;
    std::shared_ptr< buffer_t > uniform_buffer;
    std::vector< vk::VertexInputBindingDescription > vertex_input_binding;
    std::vector< vk::VertexInputAttributeDescription > vertex_input_attribute;
  };
  using shader_t = std::unordered_map< shader_flag_t, std::shared_ptr< shader_module_t > >;
  struct mesh_t {
    LIBGCT_SETTER( primitive )
    LIBGCT_SETTER( min )
    LIBGCT_SETTER( max )
    std::vector< primitive_t > primitive;
    glm::vec3 min;
    glm::vec3 max;
  };
  using meshes_t = std::vector< mesh_t >;
  struct node_t {
    node_t() : has_mesh( false ), mesh( 0 ) {}
    LIBGCT_SETTER( matrix )
    LIBGCT_SETTER( children )
    LIBGCT_SETTER( gltf )
    LIBGCT_SETTER( has_mesh )
    LIBGCT_SETTER( mesh )
    LIBGCT_SETTER( min )
    LIBGCT_SETTER( max )
    LIBGCT_SETTER( has_light )
    LIBGCT_SETTER( light )
    LIBGCT_SETTER( has_camera )
    LIBGCT_SETTER( camera )
    glm::mat4 matrix;
    std::vector< node_t > children;
    fx::gltf::Node gltf;
    bool has_mesh;
    int32_t mesh;
    bool has_light;
    int32_t light;
    bool has_camera;
    int32_t camera;
    glm::vec3 min;
    glm::vec3 max;
  };
  struct document_t {
    LIBGCT_SETTER( shader )
    LIBGCT_SETTER( mesh )
    LIBGCT_SETTER( point_light )
    LIBGCT_SETTER( camera )
    LIBGCT_SETTER( buffer )
    LIBGCT_SETTER( sampler )
    LIBGCT_SETTER( default_sampler )
    LIBGCT_SETTER( image )
    LIBGCT_SETTER( texture )
    LIBGCT_SETTER( node )
    std::vector< shader_t > shader;
    meshes_t mesh;
    point_lights_t point_light;
    cameras_t camera;
    buffers_t buffer;
    samplers_t sampler;
    std::shared_ptr< sampler_t > default_sampler;
    images_t image;
    textures_t texture;
    node_t node;
  };
  document_t load_gltf(
    std::filesystem::path path,
    const std::shared_ptr< device_t > &device,
    command_buffer_recorder_t &command_buffer,
    const std::shared_ptr< allocator_t > &allocator,
    const std::shared_ptr< descriptor_pool_t > &descriptor_pool,
    const std::vector< std::shared_ptr< render_pass_t > > &render_pass,
    const std::vector< std::filesystem::path > &shader_dir,
    std::uint32_t subpass,
    uint32_t swapchain_size,
    int shader_mask,
    float aspect_ratio,
    bool ray_trace,
    const std::vector< std::shared_ptr< descriptor_set_layout_t > > &env_descriptor_set_layout,
    bool dynamic_cull_mode = false
  );
  struct push_constants_t {
    LIBGCT_SETTER( world_matrix )
    LIBGCT_SETTER( fid )
    glm::mat4 world_matrix;
    int32_t fid;
  };
  struct uniforms_t {
    LIBGCT_SETTER( base_color )
    LIBGCT_SETTER( roughness )
    LIBGCT_SETTER( metalness )
    LIBGCT_SETTER( emissive )
    LIBGCT_SETTER( normal_scale )
    LIBGCT_SETTER( occlusion_strength )
    glm::vec4 base_color = glm::vec4( 0.5f, 0.5f, 0.5f, 1.f );
    glm::vec4 emissive = glm::vec4( 0.f, 0.f, 0.f, 1.f );
    float roughness = 0.3f;
    float metalness = 0.f;
    float normal_scale = 1.f;
    float occlusion_strength = 1.f;
  };
  struct dynamic_uniforms_t {
    LIBGCT_SETTER( projection_matrix )
    LIBGCT_SETTER( camera_matrix )
    LIBGCT_SETTER( light_vp_matrix0 )
    LIBGCT_SETTER( light_vp_matrix1 )
    LIBGCT_SETTER( light_vp_matrix2 )
    LIBGCT_SETTER( light_vp_matrix3 )
    LIBGCT_SETTER( voxel )
    LIBGCT_SETTER( inversed_voxel )
    LIBGCT_SETTER( eye_pos )
    LIBGCT_SETTER( light_pos )
    LIBGCT_SETTER( light_energy )
    LIBGCT_SETTER( light_z )
    LIBGCT_SETTER( light_frustum_width )
    LIBGCT_SETTER( light_size )
    LIBGCT_SETTER( split_bias )
    LIBGCT_SETTER( shadow_mode )
    LIBGCT_SETTER( frame_counter )
    LIBGCT_SETTER( ambient )
    glm::mat4 projection_matrix;
    glm::mat4 camera_matrix;
    glm::mat4 light_vp_matrix0;
    glm::mat4 light_vp_matrix1;
    glm::mat4 light_vp_matrix2;
    glm::mat4 light_vp_matrix3;
    glm::mat4 voxel;
    glm::mat4 inversed_voxel;
    glm::vec4 eye_pos;
    glm::vec4 light_pos;
    float light_energy;
    float light_frustum_width;
    float light_size;
    float split_bias;
    uint32_t shadow_mode;
    uint32_t frame_counter;
    float ambient;
    std::array< float, 5u > light_z;
  };
  std::tuple<
    std::shared_ptr< descriptor_set_layout_t >,
    std::shared_ptr< pipeline_layout_t >
  >
  create_pipeline_layout(
    const std::shared_ptr< device_t > &device,
    const std::vector< shader_t > &
  );
  std::shared_ptr< graphics_pipeline_t > create_pipeline(
    const std::shared_ptr< pipeline_cache_t > &pipeline_cache,
    const std::shared_ptr< shader_module_t > &vs,
    const std::shared_ptr< shader_module_t > &gs,
    const std::shared_ptr< shader_module_t > &fs,
    const std::shared_ptr< pipeline_layout_t > &pipeline_layout,
    const std::shared_ptr< render_pass_t > &render_pass,
    std::uint32_t subpass,
    const std::vector< vk::VertexInputBindingDescription > &vertex_input_binding,
    const std::vector< vk::VertexInputAttributeDescription > &vertex_input_attribute,
    bool cull,
    bool blend,
    bool back_side,
    bool dynamic_cull_mode
  );
  primitive_t create_primitive(
    const fx::gltf::Document &doc,
    const fx::gltf::Primitive &primitive,
    const std::shared_ptr< device_t > &device,
    command_buffer_recorder_t &command_buffer,
    const std::shared_ptr< allocator_t > &allocator,
    const std::shared_ptr< pipeline_cache_t > &pipeline_cache, 
    const std::shared_ptr< pipeline_layout_t > &pipeline_layout, 
    const std::shared_ptr< descriptor_pool_t > &descriptor_pool, 
    const std::vector< std::shared_ptr< render_pass_t > > &render_pass,
    const std::shared_ptr< descriptor_set_layout_t > &descriptor_set_layout,
    std::uint32_t subpass,
    uint32_t push_constant_size,
    const std::vector< shader_t > &shader,
    const textures_t &textures,
    uint32_t swapchain_size,
    int shader_mask,
    bool dynamic_cull_mode
  );
  meshes_t create_mesh(
    const fx::gltf::Document &doc,
    const std::shared_ptr< device_t > &device,
    command_buffer_recorder_t &command_buffer,
    const std::shared_ptr< allocator_t > &allocator,
    const std::shared_ptr< descriptor_pool_t > &descriptor_pool, 
    const std::vector< std::shared_ptr< render_pass_t > > &render_pass,
    std::uint32_t subpass,
    const std::vector< shader_t > &shader,
    const textures_t &textures,
    uint32_t swapchain_size,
    int shader_mask,
    const std::vector< std::shared_ptr< descriptor_set_layout_t > > &env_descriptor_set_layout,
    bool dynamic_cull_mode
  );
  node_t create_node(
    const fx::gltf::Document &doc,
    int32_t index,
    const glm::mat4 &upper,
    const meshes_t &meshes
  );
  node_t create_node(
    const fx::gltf::Document &doc,
    const meshes_t &mesh
  );
  point_lights_t get_point_lights(
    const node_t &node,
    const point_lights_t &lights
  );
  cameras_t get_cameras(
    const node_t &node,
    const cameras_t &cameras
  );
  void draw_node(
    command_buffer_recorder_t &rec,
    const node_t &node,
    const meshes_t &meshes,
    const buffers_t &buffers,
    uint32_t pipeline_index,
    const std::vector< std::shared_ptr< descriptor_set_t > > &env_descriptor_set
  );
  /*mesh_t create_mesh(
    const fx::gltf::Document &doc,
    int32_t index,
    const vw::context_t &context,
    const std::vector< vw::render_pass_t > &render_pass,
    uint32_t push_constant_size,
    const shader_t &shader,
    const textures_t &textures,
    uint32_t swapchain_size,
    int shader_mask,
    const std::vector< std::vector< viewer::texture_t > >&,
    const std::vector< buffer_t > &dynamic_uniform_buffer
  );*/
}

#endif

