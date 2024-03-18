#ifndef GCT_MATRIX_POOL_HPP
#define GCT_MATRIX_POOL_HPP
#include <glm/mat4x4.hpp>
#include <cstdint>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <filesystem>
#include <vector>
#include <optional>
#include <functional>
#include <mutex>
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>
#include <gct/named_resource.hpp>
#include <gct/handler.hpp>
#include <gct/matrix_pool_create_info.hpp>
#include <gct/linear_allocator.hpp>
#include <gct/index_range.hpp>

namespace gct {

class matrix_pool : public std::enable_shared_from_this< matrix_pool > {
public:
  using matrix_level_t = std::uint32_t;
  using matrix_index_t = std::uint32_t;
  using request_index_t = std::uint32_t;
  using matrix_descriptor = handler< matrix_index_t >;
  using weak_matrix_descriptor = matrix_descriptor::weak_type;
private:
  struct matrix_state_type {
    LIBGCT_SETTER( valid )
    LIBGCT_SETTER( staging_index )
    LIBGCT_SETTER( write_request_index )
    LIBGCT_SETTER( read_request_index )
    LIBGCT_SETTER( level )
    LIBGCT_SETTER( local )
    LIBGCT_SETTER( parent )
    LIBGCT_SETTER( self )
    bool valid = false;
    std::optional< matrix_index_t > staging_index;
    std::optional< request_index_t > write_request_index;
    std::optional< request_index_t > read_request_index;
    matrix_level_t level = 0u;
    matrix_descriptor local;
    matrix_descriptor parent;
    weak_matrix_descriptor self;
  };
  struct write_request {
    LIBGCT_SETTER( staging )
    LIBGCT_SETTER( destination )
    matrix_index_t staging = 0u;
    matrix_index_t destination = 0u;
  };
  struct read_request {
    LIBGCT_SETTER( source )
    LIBGCT_SETTER( staging )
    matrix_index_t source = 0u;
    matrix_index_t staging = 0u;
  };
  struct update_request {
    LIBGCT_SETTER( parent )
    LIBGCT_SETTER( local )
    LIBGCT_SETTER( world )
    matrix_index_t parent = 0u;
    matrix_index_t local = 0u;
    matrix_index_t world = 0u;
    matrix_index_t reserved = 0u;
  };
  using request_range = index_range;
public:
  matrix_pool( const matrix_pool_create_info & );
  matrix_descriptor allocate( const glm::mat4& ); // standalone matrix
  matrix_descriptor allocate( const matrix_descriptor&, const glm::mat4& ); // chained matrix
  matrix_descriptor get_local( const matrix_descriptor& );
  void touch( const matrix_descriptor& );
  void set( const matrix_descriptor&, const glm::mat4& );
  void get( const matrix_descriptor&, const std::function< void( vk::Result, const glm::mat4& ) >& );
  bool is_valid( const matrix_descriptor& ) const;
  const matrix_pool_create_info &get_props() const { return state->props; }
  void operator()( command_buffer_recorder_t& );
  void to_json( nlohmann::json& ) const;
  std::shared_ptr< buffer_t > get_buffer() const {
    return state->matrix;
  }
private:
  struct state_type : std::enable_shared_from_this< state_type > {
    state_type( const matrix_pool_create_info & );
    matrix_index_t allocate_index();
    void release_index( matrix_index_t );
    matrix_descriptor allocate( const glm::mat4& ); // standalone matrix
    matrix_descriptor allocate( const matrix_descriptor&, const glm::mat4& ); // chained matrix
    void release( matrix_index_t );
    void touch( const matrix_descriptor& );
    void touch( matrix_index_t );
    void set( const matrix_descriptor&, const glm::mat4& );
    void get( const matrix_descriptor&, const std::function< void( vk::Result, const glm::mat4& ) >& );
    bool is_valid( const matrix_descriptor& ) const;
    void flush( command_buffer_recorder_t& );
    matrix_descriptor get_local( const matrix_descriptor& );
    std::vector< request_range > build_update_request_range();
    matrix_pool_create_info props;
    std::vector< matrix_state_type > matrix_state;
    linear_allocator index_allocator;
    std::unordered_multimap< matrix_index_t, matrix_index_t > edge; // p -> c
    std::shared_ptr< buffer_t > staging_matrix; // mat4[]
    std::shared_ptr< buffer_t > matrix; // mat4[]
    std::shared_ptr< buffer_t > write_request_buffer; // write_request[] destination
    std::shared_ptr< buffer_t > read_request_buffer; // read_request[] source
    std::shared_ptr< buffer_t > update_request_buffer; // update_request_buffer[] target
    std::vector< std::vector< update_request > > update_request_list;
    std::unordered_set< matrix_index_t > update_requested;
    reduced_linear_allocator staging_index_allocator;
    reduced_linear_allocator write_request_index_allocator;
    reduced_linear_allocator read_request_index_allocator;
    std::vector< matrix_descriptor > used_on_gpu;
    std::unordered_set< matrix_index_t > modified;
    std::unordered_multimap< matrix_index_t, std::function< void( vk::Result, const glm::mat4& ) > > cbs;
    std::shared_ptr< compute > write;
    std::shared_ptr< compute > read;
    std::shared_ptr< compute > update;
    bool execution_pending = false;
    std::mutex guard;
  };
  std::shared_ptr< state_type > state;
};
void to_json( nlohmann::json&, const matrix_pool& );
}

#endif

