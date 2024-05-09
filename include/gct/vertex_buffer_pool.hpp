#ifndef GCT_VERTEX_BUFFER_POOL_HPP
#define GCT_VERTEX_BUFFER_POOL_HPP
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
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
#include <gct/vertex_buffer_pool_create_info.hpp>
#include <gct/numeric_types.hpp>
#include <gct/image_view.hpp>
#include <gct/linear_allocator.hpp>
#include <gct/index_range.hpp>
#include <gct/named_resource.hpp>
#include <gct/handler.hpp>

namespace gct {
class mappable_buffer_t;
class vertex_buffer_pool {
public:
  using vertex_buffer_index_t = std::uint32_t;
  using request_index_t = std::uint32_t;
  using vertex_buffer_descriptor = handler< vertex_buffer_index_t >;
  using weak_vertex_buffer_descriptor = vertex_buffer_descriptor::weak_type;
private:
  struct vertex_buffer_state_type {
    LIBGCT_SETTER( valid )
    LIBGCT_SETTER( write_request_index )
    LIBGCT_SETTER( buffer )
    bool valid = false;
    std::optional< request_index_t > write_request_index;
    std::shared_ptr< mappable_buffer_t > buffer;
  };
  struct write_request {
    LIBGCT_SETTER( index )
    LIBGCT_SETTER( buffer )
    vertex_buffer_index_t index;
    std::shared_ptr< mappable_buffer_t > buffer;
  };
  using request_range = index_range;
public:
  vertex_buffer_pool( const vertex_buffer_pool_create_info & );
  vertex_buffer_descriptor allocate( const std::string &filename );
  vertex_buffer_descriptor allocate( const std::vector< glm::vec4 > &data );
  vertex_buffer_descriptor allocate( const std::vector< std::uint8_t > &data );
  std::shared_ptr< buffer_t > get( const vertex_buffer_descriptor& );
  std::vector< std::shared_ptr< buffer_t > > get();
  const vertex_buffer_pool_create_info &get_props() const { return state->props; }
  void operator()( command_buffer_recorder_t& );
  void to_json( nlohmann::json& ) const;
private:
  struct state_type : std::enable_shared_from_this< state_type > {
    state_type( const vertex_buffer_pool_create_info & );
    vertex_buffer_index_t allocate_index();
    void release_index( vertex_buffer_index_t );
    vertex_buffer_descriptor allocate( const std::string &filename );
    vertex_buffer_descriptor allocate( const std::vector< glm::vec4 > &data );
    vertex_buffer_descriptor allocate( const std::vector< std::uint8_t > &data );
    std::shared_ptr< buffer_t > get( const vertex_buffer_descriptor& );
    std::vector< std::shared_ptr< buffer_t > > get();
    void release( vertex_buffer_index_t );
    void flush( command_buffer_recorder_t& );
    vertex_buffer_pool_create_info props;
    std::vector< vertex_buffer_state_type > vertex_buffer_state;
    linear_allocator index_allocator;
    std::vector< write_request > write_request_list;
    std::vector< vertex_buffer_descriptor > used_on_gpu;
    bool execution_pending = false;
    std::shared_ptr< buffer_t > null_buffer;
    std::mutex guard;
  };
  std::shared_ptr< state_type > state;
};
void to_json( nlohmann::json&, const vertex_buffer_pool& );
}

#endif

