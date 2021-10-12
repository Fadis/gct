#ifndef LIBGCT_INCLUDE_GCT_GLTF_HPP
#define LIBGCT_INCLUDE_GCT_GLTF_HPP
#include <tuple>
#include <memory>
#include <cstdint>
#include <vector>
#include <filesystem>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <stamp/exception.h>
#include <stamp/setter.h>
#include <fx/gltf.h>
#include <vulkan/vulkan.hpp>
#include <gct/shader_flag.hpp>
#include <gct/descriptor_image_info.hpp>
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
  LIBSTAMP_EXCEPTION( runtime_error, invalid_gltf, "invalid glTF" )
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
    LIBSTAMP_SETTER( unorm )
    LIBSTAMP_SETTER( srgb )
    LIBSTAMP_SETTER( unorm_view )
    LIBSTAMP_SETTER( srgb_view )
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
    const std::filesystem::path cd
  );
  struct camera_t {
    LIBSTAMP_SETTER( projection_matrix )
    LIBSTAMP_SETTER( camera_pos )
    LIBSTAMP_SETTER( camera_direction )
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
    LIBSTAMP_SETTER( location )
    LIBSTAMP_SETTER( color )
    LIBSTAMP_SETTER( intensity )
    glm::vec3 location;
    glm::vec3 color;
    float intensity;
  };
  using point_lights_t = std::vector< point_light_t >;
  point_lights_t create_point_light(
    const fx::gltf::Document &doc
  );
  struct buffer_window_t {
    buffer_window_t() : index( 0 ), offset( 0 ) {}
    LIBSTAMP_SETTER( index )
    LIBSTAMP_SETTER( offset )
    std::uint32_t index;
    std::uint32_t offset;
  };
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
    LIBSTAMP_SETTER( unorm )
    LIBSTAMP_SETTER( srgb )
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
    LIBSTAMP_SETTER( pipeline )
    LIBSTAMP_SETTER( pipeline_layout )
    LIBSTAMP_SETTER( vertex_buffer )
    LIBSTAMP_SETTER( indexed )
    LIBSTAMP_SETTER( index_buffer )
    LIBSTAMP_SETTER( descriptor_set )
    LIBSTAMP_SETTER( count )
    LIBSTAMP_SETTER( index_buffer_type )
    LIBSTAMP_SETTER( min )
    LIBSTAMP_SETTER( max )
    LIBSTAMP_SETTER( uniform_buffer )
    std::vector< std::shared_ptr< graphics_pipeline_t > > pipeline;
    std::shared_ptr< pipeline_layout_t > pipeline_layout;
    std::unordered_map< std::uint32_t, buffer_window_t > vertex_buffer;
    bool indexed;
    buffer_window_t index_buffer;
    std::vector< std::shared_ptr< descriptor_set_t > > descriptor_set;
    uint32_t count;
    vk::IndexType index_buffer_type;
    glm::vec3 min;
    glm::vec3 max;
    std::shared_ptr< buffer_t > uniform_buffer;
  };
  using shader_t = std::unordered_map< shader_flag_t, std::shared_ptr< shader_module_t > >;
  struct mesh_t {
    LIBSTAMP_SETTER( primitive )
    LIBSTAMP_SETTER( min )
    LIBSTAMP_SETTER( max )
    std::vector< primitive_t > primitive;
    glm::vec3 min;
    glm::vec3 max;
  };
  using meshes_t = std::vector< mesh_t >;
  struct node_t {
    node_t() : has_mesh( false ), mesh( 0 ) {}
    LIBSTAMP_SETTER( matrix )
    LIBSTAMP_SETTER( children )
    LIBSTAMP_SETTER( gltf )
    LIBSTAMP_SETTER( has_mesh )
    LIBSTAMP_SETTER( mesh )
    LIBSTAMP_SETTER( min )
    LIBSTAMP_SETTER( max )
    LIBSTAMP_SETTER( has_light )
    LIBSTAMP_SETTER( light )
    LIBSTAMP_SETTER( has_camera )
    LIBSTAMP_SETTER( camera )
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
    LIBSTAMP_SETTER( shader )
    LIBSTAMP_SETTER( mesh )
    LIBSTAMP_SETTER( point_light )
    LIBSTAMP_SETTER( camera )
    LIBSTAMP_SETTER( buffer )
    LIBSTAMP_SETTER( sampler )
    LIBSTAMP_SETTER( default_sampler )
    LIBSTAMP_SETTER( image )
    LIBSTAMP_SETTER( texture )
    LIBSTAMP_SETTER( node )
    shader_t shader;
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
    const std::filesystem::path &shader_dir,
    std::uint32_t subpass,
    uint32_t swapchain_size,
    int shader_mask,
    const std::vector< std::shared_ptr< buffer_t > > &dynamic_uniform_buffer,
    float aspect_ratio
  );
  struct push_constants_t {
    LIBSTAMP_SETTER( world_matrix )
    LIBSTAMP_SETTER( fid )
    glm::mat4 world_matrix;
    int32_t fid;
  };
  struct uniforms_t {
    LIBSTAMP_SETTER( base_color )
    LIBSTAMP_SETTER( roughness )
    LIBSTAMP_SETTER( metalness )
    LIBSTAMP_SETTER( emissive )
    LIBSTAMP_SETTER( normal_scale )
    LIBSTAMP_SETTER( occlusion_strength )
    glm::vec4 base_color;
    glm::vec4 emissive;
    float roughness;
    float metalness;
    float normal_scale;
    float occlusion_strength;
  };
  struct dynamic_uniforms_t {
    LIBSTAMP_SETTER( projection_matrix )
    LIBSTAMP_SETTER( camera_matrix )
    LIBSTAMP_SETTER( light_vp_matrix0 )
    LIBSTAMP_SETTER( light_vp_matrix1 )
    LIBSTAMP_SETTER( light_vp_matrix2 )
    LIBSTAMP_SETTER( light_vp_matrix3 )
    LIBSTAMP_SETTER( eye_pos )
    LIBSTAMP_SETTER( light_pos )
    LIBSTAMP_SETTER( light_energy )
    LIBSTAMP_SETTER( light_z )
    LIBSTAMP_SETTER( light_frustum_width )
    LIBSTAMP_SETTER( light_size )
    LIBSTAMP_SETTER( split_bias )
    LIBSTAMP_SETTER( shadow_mode )
    glm::mat4 projection_matrix;
    glm::mat4 camera_matrix;
    glm::mat4 light_vp_matrix0;
    glm::mat4 light_vp_matrix1;
    glm::mat4 light_vp_matrix2;
    glm::mat4 light_vp_matrix3;
    glm::vec4 eye_pos;
    glm::vec4 light_pos;
    float light_energy;
    float light_frustum_width;
    float light_size;
    float split_bias;
    uint32_t shadow_mode;
    std::array< float, 5u > light_z;
  };
  std::tuple<
    std::shared_ptr< descriptor_set_layout_t >,
    std::shared_ptr< pipeline_layout_t >
  >
  create_pipeline_layout(
    const std::shared_ptr< device_t > &device
  );
  std::shared_ptr< graphics_pipeline_t > create_pipeline(
    const std::shared_ptr< pipeline_cache_t > &pipeline_cache,
    const std::shared_ptr< shader_module_t > &vs,
    const std::shared_ptr< shader_module_t > &fs,
    const std::shared_ptr< pipeline_layout_t > &pipeline_layout,
    const std::shared_ptr< render_pass_t > &render_pass,
    std::uint32_t subpass,
    const std::vector< vk::VertexInputBindingDescription > &vertex_input_binding,
    const std::vector< vk::VertexInputAttributeDescription > &vertex_input_attribute,
    bool cull,
    bool blend,
    bool back_side
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
    const shader_t &shader,
    const textures_t &textures,
    uint32_t swapchain_size,
    int shader_mask,
    const std::vector< std::shared_ptr< buffer_t > > &dynamic_uniform_buffer
  );
  meshes_t create_mesh(
    const fx::gltf::Document &doc,
    const std::shared_ptr< device_t > &device,
    command_buffer_recorder_t &command_buffer,
    const std::shared_ptr< allocator_t > &allocator,
    const std::shared_ptr< pipeline_layout_t > &pipeline_layout, 
    const std::shared_ptr< descriptor_pool_t > &descriptor_pool, 
    const std::vector< std::shared_ptr< render_pass_t > > &render_pass,
    const std::shared_ptr< descriptor_set_layout_t > &descriptor_set_layout,
    std::uint32_t subpass,
    const shader_t &shader,
    const textures_t &textures,
    uint32_t swapchain_size,
    int shader_mask,
    const std::vector< std::shared_ptr< buffer_t > > &dynamic_uniform_buffer
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
    const command_buffer_recorder_t &rec,
    const node_t &node,
    const meshes_t &meshes,
    const buffers_t &buffers,
    uint32_t current_frame,
    uint32_t pipeline_index
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

