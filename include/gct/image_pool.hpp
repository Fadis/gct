#ifndef GCT_IMAGE_POOL_HPP
#define GCT_IMAGE_POOL_HPP
#include <glm/mat4x4.hpp>
#include <cstdint>
#include <memory>
#include <vector>
#include <cstdint>
#include <optional>
#include <mutex>
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>
#include <gct/matrix_pool.hpp>
#include <gct/image_pool_create_info.hpp>
#include <gct/numeric_types.hpp>
#include <gct/linear_allocator.hpp>
#include <gct/index_range.hpp>
#include <gct/named_resource.hpp>
#include <gct/handler.hpp>
#include <gct/color_space.hpp>
#include <gct/image_create_info.hpp>
#include <gct/subview_range.hpp>
#include <gct/image_allocate_info.hpp>

namespace gct {
class image_t;
class buffer_t;
struct image_load_info {
  LIBGCT_SETTER( filename )
  LIBGCT_SETTER( usage )
  LIBGCT_SETTER( mipmap )
  LIBGCT_SETTER( attr )
  LIBGCT_SETTER( max_channels_per_layer )
  LIBGCT_SETTER( enable_linear )
  LIBGCT_SETTER( layout )
  std::string filename;
  vk::ImageUsageFlagBits usage;
  bool mipmap = true;
  integer_attribute_t attr = integer_attribute_t::normalized;
  unsigned int max_channels_per_layer = 4u;
  bool enable_linear = false;
  std::optional< vk::ImageLayout > layout;
};

struct image_dump_info {
  LIBGCT_SETTER( filename )
  LIBGCT_SETTER( mipmap )
  LIBGCT_SETTER( layer )
  LIBGCT_SETTER( depth )
  std::string filename = "output.png";
  unsigned int mipmap = 0u;
  unsigned int layer = 0u;
  unsigned int depth = 0u;
};

class image_pool {
public:
  using image_index_t = std::uint32_t;
  using request_index_t = std::uint32_t;
  using image_descriptor = handler< image_index_t, image_pool >;
  using weak_image_descriptor = image_descriptor::weak_type;
  struct views {
    LIBGCT_SETTER( normalized )
    LIBGCT_SETTER( srgb )
    LIBGCT_SETTER( linear )
    image_descriptor normalized;
    image_descriptor srgb;
    image_descriptor linear;
  };
private:
  struct image_state_type {
    LIBGCT_SETTER( valid )
    LIBGCT_SETTER( write_request_index )
    LIBGCT_SETTER( image )
    bool valid = false;
    std::optional< request_index_t > write_request_index;
    std::shared_ptr< image_view_t > image;
  };
  struct write_request {
    LIBGCT_SETTER( index )
    LIBGCT_SETTER( mipmap )
    LIBGCT_SETTER( staging_buffer )
    LIBGCT_SETTER( final_image )
    LIBGCT_SETTER( layout )
    image_index_t index;
    bool mipmap = true;
    std::shared_ptr< buffer_t > staging_buffer;
    std::shared_ptr< image_view_t > final_image;
    vk::ImageLayout layout;
  };
  struct convert_request {
    LIBGCT_SETTER( index )
    LIBGCT_SETTER( image )
    LIBGCT_SETTER( layout )
    image_index_t index;
    std::shared_ptr< image_view_t > image;
    vk::ImageLayout layout;
  };
  struct rgb_to_xyz_request {
    LIBGCT_SETTER( rgb )
    LIBGCT_SETTER( xyz )
    LIBGCT_SETTER( rgb_image )
    LIBGCT_SETTER( xyz_image )
    LIBGCT_SETTER( layout )
    image_index_t rgb;
    image_index_t xyz;
    std::shared_ptr< image_view_t > rgb_image;
    std::shared_ptr< image_view_t > xyz_image;
    vk::ImageLayout layout;
  };
  using request_range = index_range;
public:
  image_pool( const image_pool_create_info & );
  [[nodiscard]] views allocate( const image_load_info& );
  [[nodiscard]] views allocate();
  [[nodiscard]] views allocate( const image_allocate_info& );
  [[nodiscard]] image_descriptor allocate( const std::shared_ptr< image_view_t >& );
  [[nodiscard]] const image_pool_create_info &get_props() const { return state->props; }
  [[nodiscard]] std::shared_ptr< image_view_t > get( const image_descriptor& ) const;
  void dump( const image_descriptor&, const image_dump_info& );
  void operator()( command_buffer_recorder_t& );
  void to_json( nlohmann::json& ) const;
private:
  struct state_type : std::enable_shared_from_this< state_type > {
    state_type( const image_pool_create_info & );
    [[nodiscard]] image_index_t allocate_index();
    void release_index( image_index_t );
    [[nodiscard]] views allocate( const image_load_info& );
    [[nodiscard]] views allocate();
    [[nodiscard]] views allocate( const image_allocate_info& );
    [[nodiscard]] image_descriptor allocate( const std::shared_ptr< image_view_t >& );
    [[nodiscard]] std::shared_ptr< image_view_t > get( const image_descriptor& ) const;
    void dump( const image_descriptor&, const image_dump_info& );
    void release( image_index_t );
    void flush( command_buffer_recorder_t& );
    image_pool_create_info props;
    std::vector< image_state_type > image_state;
    linear_allocator index_allocator;
    std::vector< write_request > write_request_list;
    std::vector< std::pair< std::shared_ptr< image_t >, image_dump_info > > dump_request_list;
    std::vector< rgb_to_xyz_request > rgb_to_xyz_request_list;
    std::vector< convert_request > convert_request_list;
    std::vector< image_descriptor > used_on_gpu;
    bool execution_pending = false;
    std::mutex guard;
    bool enable_color_space = false;
    std::shared_ptr< compute > rgba8;
    std::shared_ptr< compute > rgba16;
    std::shared_ptr< compute > rgba16f;
    std::shared_ptr< compute > rgba32f;
  };
  std::shared_ptr< state_type > state;
};
void to_json( nlohmann::json&, const image_pool& );
}

#endif

