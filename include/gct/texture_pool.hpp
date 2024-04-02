#ifndef GCT_TEXTURE_POOL_HPP
#define GCT_TEXTURE_POOL_HPP
#include <glm/mat4x4.hpp>
#include <cstdint>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <vector>
#include <optional>
#include <functional>
#include <mutex>
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>
#include <gct/matrix_pool.hpp>
#include <gct/texture_pool_create_info.hpp>
#include <gct/numeric_types.hpp>
#include <gct/image_view.hpp>
#include <gct/linear_allocator.hpp>
#include <gct/index_range.hpp>
#include <gct/named_resource.hpp>
#include <gct/handler.hpp>
#include <gct/sampler_pool.hpp>
#include <gct/image_pool.hpp>

namespace gct {

struct texture_create_info {
  LIBGCT_SETTER( filename )
  LIBGCT_SETTER( mipmap )
  LIBGCT_SETTER( attr )
  LIBGCT_SETTER( max_channels_per_layer )
  LIBGCT_SETTER( sampler )
  std::string filename;
  bool mipmap = true;
  integer_attribute_t attr = integer_attribute_t::normalized;
  unsigned int max_channels_per_layer = 4u;
  sampler_pool::sampler_descriptor sampler;
};
void to_json( nlohmann::json&, const texture_create_info& );

class texture_pool {
public:
  using texture_index_t = std::uint32_t;
  using request_index_t = std::uint32_t;
  using texture_descriptor = handler< texture_index_t >;
  using weak_texture_descriptor = texture_descriptor::weak_type;
  struct views {
    LIBGCT_SETTER( normalized )
    LIBGCT_SETTER( srgb )
    texture_descriptor normalized;
    texture_descriptor srgb;
  };
private:
  struct texture_state_type {
    LIBGCT_SETTER( valid )
    LIBGCT_SETTER( write_request_index )
    LIBGCT_SETTER( view )
    LIBGCT_SETTER( sampler )
    bool valid = false;
    std::optional< request_index_t > write_request_index;
    std::shared_ptr< image_view_t > view;
    std::shared_ptr< sampler_t > sampler;
  };
  struct write_request {
    LIBGCT_SETTER( index )
    LIBGCT_SETTER( view )
    LIBGCT_SETTER( sampler )
    texture_index_t index;
    std::shared_ptr< image_view_t > view;
    std::shared_ptr< sampler_t > sampler;
  };
  using request_range = index_range;
public:
  texture_pool( const texture_pool_create_info & );
  views allocate(
    const sampler_pool::sampler_descriptor &s,
    const image_pool::views &i
  );
  std::pair< std::shared_ptr< image_view_t >, std::shared_ptr< sampler_t > > get(
    const texture_descriptor&
  ) const;
  const texture_pool_create_info &get_props() const { return state->props; }
  void operator()( command_buffer_recorder_t& );
  void to_json( nlohmann::json& ) const;
private:
  struct state_type : std::enable_shared_from_this< state_type > {
    state_type( const texture_pool_create_info & );
    texture_index_t allocate_index();
    void release_index( texture_index_t );
    views allocate(
      const sampler_pool::sampler_descriptor &s,
      const image_pool::views &i
    );
    std::pair< std::shared_ptr< image_view_t >, std::shared_ptr< sampler_t > > get(
      const texture_descriptor&
    ) const;
    void release( texture_index_t );
    void flush( command_buffer_recorder_t& );
    texture_pool_create_info props;
    std::vector< texture_state_type > texture_state;
    linear_allocator index_allocator;
    std::vector< write_request > write_request_list;
    std::vector< texture_descriptor > used_on_gpu;
    bool execution_pending = false;
    std::mutex guard;
  };
  std::shared_ptr< state_type > state;
};
void to_json( nlohmann::json&, const texture_pool& );
}

#endif

