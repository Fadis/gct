#ifndef GCT_LIGHT_POOL_HPP
#define GCT_LIGHT_POOL_HPP
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
#include <gct/aabb.hpp>
#include <gct/light_pool_create_info.hpp>
#include <gct/linear_allocator.hpp>
#include <gct/index_range.hpp>
#include <gct/named_resource.hpp>
#include <gct/handler.hpp>
#include <gct/punctual_light_type.hpp>

namespace gct {

struct punctual_light_parameter {
  LIBGCT_SETTER( local_position )
  LIBGCT_SETTER( world_position )
  LIBGCT_SETTER( local_direction )
  LIBGCT_SETTER( world_direction )
  LIBGCT_SETTER( energy )
  LIBGCT_SETTER( type )
  LIBGCT_SETTER( light_size )
  LIBGCT_SETTER( theta )
  LIBGCT_SETTER( phi )
  LIBGCT_SETTER( falloff )
  LIBGCT_SETTER( matrix )
  LIBGCT_SETTER( shadow_map )
  glm::vec4 local_position = glm::vec4( 0.f, 0.f, 0.f, 1.f );
  glm::vec4 world_position = glm::vec4( 0.f, 0.f, 0.f, 1.f );
  glm::vec4 local_direction = glm::vec4( 0.f, 1.f, 0.f, 1.f );
  glm::vec4 world_direction = glm::vec4( 0.f, 1.f, 0.f, 1.f );
  glm::vec4 energy = glm::vec4( 0.f, 0.f, 0.f, 1.f );
  punctual_light_type type = punctual_light_type::point;
  float light_size = 1.f;
  float theta = 0.f;
  float phi = 0.f;
  float falloff = 0.f;
  std::uint32_t matrix = 0u;
  std::int32_t shadow_map = 0u;
  std::uint32_t reserved;
};

void to_json( nlohmann::json &dest, const punctual_light_parameter &src );
void from_json( const nlohmann::json &dest, punctual_light_parameter &src );

class light_pool {
public:
  using light_level_t = std::uint32_t;
  using light_index_t = std::uint32_t;
  using request_index_t = std::uint32_t;
  using light_descriptor = handler< light_index_t, light_pool >;
  using weak_aabb_descriptor = light_descriptor::weak_type;
private:
  struct light_state_type {
    LIBGCT_SETTER( valid )
    LIBGCT_SETTER( staging_index )
    LIBGCT_SETTER( write_request_index )
    LIBGCT_SETTER( read_request_index )
    LIBGCT_SETTER( update_request_index )
    LIBGCT_SETTER( matrix )
    LIBGCT_SETTER( self )
    bool valid = false;
    std::optional< light_index_t > staging_index;
    std::optional< request_index_t > write_request_index;
    std::optional< request_index_t > read_request_index;
    std::optional< request_index_t > update_request_index;
    matrix_pool::matrix_descriptor matrix;
    weak_aabb_descriptor self;
  };
  struct write_request {
    LIBGCT_SETTER( staging )
    LIBGCT_SETTER( destination )
    light_index_t staging = 0u;
    light_index_t destination = 0u;
  };
  struct read_request {
    LIBGCT_SETTER( source )
    LIBGCT_SETTER( staging )
    light_index_t source = 0u;
    light_index_t staging = 0u;
  };
  struct update_request {
    LIBGCT_SETTER( index )
    light_index_t index = 0u;
  };
  using request_range = index_range;
public:
  using light_type = punctual_light_parameter;
  light_pool( const light_pool_create_info & );
  light_descriptor allocate( const matrix_pool::matrix_descriptor &matrix, const light_type& );
  void touch( const light_descriptor& );
  void set( const light_descriptor&, const light_type& );
  void get( const light_descriptor&, const std::function< void( vk::Result, const light_type& ) >& );
  [[nodiscard]] const light_pool_create_info &get_props() const { return state->props; }
  void operator()( command_buffer_recorder_t& );
  [[nodiscard]] std::shared_ptr< buffer_t > get_buffer() const {
    return state->light;
  }
  [[nodiscard]] std::shared_ptr< buffer_t > get_active_light_buffer() const;
  [[nodiscard]] std::uint32_t get_active_light_count() const {
    return state->get_active_light_count();
  }
  void to_json( nlohmann::json& ) const;
private:
  struct state_type : std::enable_shared_from_this< state_type > {
    state_type( const light_pool_create_info & );
    [[nodiscard]] light_index_t allocate_index();
    void release_index( light_index_t );
    [[nodiscard]] light_descriptor allocate( const matrix_pool::matrix_descriptor &matrix, const light_type& );
    void release( light_index_t );
    void touch( const light_descriptor& );
    void touch( light_index_t );
    void set( const light_descriptor&, const light_type& );
    void get( const light_descriptor&, const std::function< void( vk::Result, const light_type& ) >& );
    void flush( command_buffer_recorder_t& );
    [[nodiscard]] std::uint32_t get_active_light_count() const;
    light_pool_create_info props;
    std::vector< light_state_type > light_state;
    linear_allocator index_allocator;
    std::unordered_multimap< light_index_t, light_index_t > edge; // p -> c
    std::shared_ptr< buffer_t > staging_light;
    std::shared_ptr< buffer_t > light;
    std::shared_ptr< buffer_t > write_request_buffer;
    std::shared_ptr< buffer_t > read_request_buffer;
    std::shared_ptr< buffer_t > update_request_buffer;
    std::shared_ptr< mappable_buffer_t > active_light;
    reduced_linear_allocator staging_index_allocator;
    reduced_linear_allocator write_request_index_allocator;
    reduced_linear_allocator read_request_index_allocator;
    reduced_linear_allocator update_request_index_allocator;
    std::vector< light_descriptor > used_on_gpu;
    std::unordered_multimap< light_index_t, std::function< void( vk::Result, const light_type& ) > > cbs;
    std::shared_ptr< compute > write;
    std::shared_ptr< compute > read;
    std::shared_ptr< compute > update;
    std::unordered_set< std::uint32_t > host_active_light;
    bool active_light_modified = false;
    bool execution_pending = false;
    std::mutex guard;
  };
  std::shared_ptr< state_type > state;
};
void to_json( nlohmann::json&, const light_pool& );
std::pair<
  std::vector< light_pool::light_descriptor >,
  std::vector< punctual_light_parameter >
> generate_random_light(
  light_pool &pool,
  const matrix_pool::matrix_descriptor &matrix,
  const aabb3 &range,
  const aabb3 &energy,
  float min_energy,
  float max_energy,
  std::uint32_t count
);
std::vector< light_pool::light_descriptor >
restore_light(
  light_pool &pool,
  const matrix_pool::matrix_descriptor &matrix,
  const std::vector< punctual_light_parameter >&
);

}

#endif

