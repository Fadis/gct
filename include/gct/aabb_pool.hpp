#ifndef GCT_AABB_POOL_HPP
#define GCT_AABB_POOL_HPP
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
#include <gct/aabb_pool_create_info.hpp>
#include <gct/linear_allocator.hpp>
#include <gct/index_range.hpp>
#include <gct/named_resource.hpp>
#include <gct/handler.hpp>

namespace gct {

class aabb_pool {
public:
  using aabb_level_t = std::uint32_t;
  using aabb_index_t = std::uint32_t;
  using request_index_t = std::uint32_t;
  using aabb_descriptor = handler< aabb_index_t, aabb_pool >;
  using weak_aabb_descriptor = aabb_descriptor::weak_type;
private:
  struct aabb_state_type {
    LIBGCT_SETTER( valid )
    LIBGCT_SETTER( staging_index )
    LIBGCT_SETTER( write_request_index )
    LIBGCT_SETTER( read_request_index )
    LIBGCT_SETTER( update_request_index )
    LIBGCT_SETTER( matrix )
    LIBGCT_SETTER( local )
    LIBGCT_SETTER( self )
    bool valid = false;
    std::optional< aabb_index_t > staging_index;
    std::optional< request_index_t > write_request_index;
    std::optional< request_index_t > read_request_index;
    std::optional< request_index_t > update_request_index;
    matrix_pool::matrix_descriptor matrix;
    aabb_descriptor local;
    weak_aabb_descriptor self;
  };
  using aabb_type = aabb4;
  struct write_request {
    LIBGCT_SETTER( staging )
    LIBGCT_SETTER( destination )
    aabb_index_t staging = 0u;
    aabb_index_t destination = 0u;
  };
  struct read_request {
    LIBGCT_SETTER( source )
    LIBGCT_SETTER( staging )
    aabb_index_t source = 0u;
    aabb_index_t staging = 0u;
  };
  struct update_request {
    LIBGCT_SETTER( matrix )
    LIBGCT_SETTER( local )
    LIBGCT_SETTER( world )
    matrix_pool::matrix_index_t matrix = 0u;
    aabb_index_t local = 0u;
    aabb_index_t world = 0u;
    aabb_index_t reserved = 0u;
  };
  using request_range = index_range;
public:
  aabb_pool( const aabb_pool_create_info & );
  [[nodiscard]] aabb_descriptor allocate( const aabb_type& );
  [[nodiscard]] aabb_descriptor allocate( const aabb_descriptor&, const matrix_pool::matrix_descriptor& );
  [[nodiscard]] aabb_descriptor get_local( const aabb_descriptor& );
  void touch( const aabb_descriptor& );
  void set( const aabb_descriptor&, const aabb_type& );
  void get( const aabb_descriptor&, const std::function< void( vk::Result, const aabb_type& ) >& );
  [[nodiscard]] const aabb_pool_create_info &get_props() const { return state->props; }
  void operator()( command_buffer_recorder_t& );
  [[nodiscard]] std::shared_ptr< buffer_t > get_buffer() const {
    return state->aabb;
  }
  void to_json( nlohmann::json& ) const;
private:
  struct state_type : std::enable_shared_from_this< state_type > {
    state_type( const aabb_pool_create_info & );
    [[nodiscard]] aabb_index_t allocate_index();
    void release_index( aabb_index_t );
    [[nodiscard]] aabb_descriptor allocate( const aabb_type& ); // standalone aabb
    [[nodiscard]] aabb_descriptor allocate( const aabb_descriptor&, const matrix_pool::matrix_descriptor& );
    //////aabb_descriptor allocate( const aabb_descriptor&, const aabb4& ); // chained aabb
    void release( aabb_index_t );
    void touch( const aabb_descriptor& );
    void touch( aabb_index_t );
    void set( const aabb_descriptor&, const aabb_type& );
    void get( const aabb_descriptor&, const std::function< void( vk::Result, const aabb_type& ) >& );
    void flush( command_buffer_recorder_t& );
    [[nodiscard]] aabb_descriptor get_local( const aabb_descriptor& );
    aabb_pool_create_info props;
    std::vector< aabb_state_type > aabb_state;
    linear_allocator index_allocator;
    std::unordered_multimap< aabb_index_t, aabb_index_t > edge; // p -> c
    std::shared_ptr< buffer_t > staging_aabb; // mat4[]
    std::shared_ptr< buffer_t > aabb; // mat4[]
    std::shared_ptr< buffer_t > write_request_buffer; // write_request[] destination
    std::shared_ptr< buffer_t > read_request_buffer; // read_request[] source
    std::shared_ptr< buffer_t > update_request_buffer; // update_request_buffer[] target
    reduced_linear_allocator staging_index_allocator;
    reduced_linear_allocator write_request_index_allocator;
    reduced_linear_allocator read_request_index_allocator;
    reduced_linear_allocator update_request_index_allocator;
    std::vector< aabb_descriptor > used_on_gpu;
    std::unordered_multimap< aabb_index_t, std::function< void( vk::Result, const aabb_type& ) > > cbs;
    std::shared_ptr< compute > write;
    std::shared_ptr< compute > read;
    std::shared_ptr< compute > update;
    bool execution_pending = false;
    std::mutex guard;
  };
  std::shared_ptr< state_type > state;
};
void to_json( nlohmann::json&, const aabb_pool& );
void test_aabb_pool(
  matrix_pool &matrix,
  aabb_pool &aabb,
  queue_t &queue
);
}

#endif

