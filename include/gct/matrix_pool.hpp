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
#include <gct/setter.hpp>
#include <gct/named_resource.hpp>

namespace gct {

using matrix_level_t = std::uint32_t;
using matrix_index_t = std::uint32_t;
using request_index_t = std::uint32_t;

class matrix_descriptor;


using weak_matrix_descriptor = std::weak_ptr< matrix_index_t >;

class matrix_descriptor {
public:
  template< typename ... Args >
  matrix_descriptor(
    Args&& ... v
  ) : value( std::forward< Args >( v )... ) {}
  matrix_index_t operator*() const {
    return *value;
  }
  weak_matrix_descriptor get_weak() const {
    return value;
  }
  operator bool() const {
    return bool( value );
  }
private:
  std::shared_ptr< matrix_index_t > value;
};

class allocator_t;
class descriptor_pool_t;
class pipeline_cache_t;
class compute;
class command_buffer_recorder_t;
struct matrix_pool_create_info {
  LIBGCT_SETTER( allocator )
  LIBGCT_SETTER( descriptor_pool )
  LIBGCT_SETTER( pipeline_cache )
  LIBGCT_SETTER( write_shader )
  LIBGCT_SETTER( read_shader )
  LIBGCT_SETTER( update_shader )
  LIBGCT_SETTER( max_matrix_count )
  LIBGCT_SETTER( staging_matrix_buffer_name )
  LIBGCT_SETTER( matrix_buffer_name )
  LIBGCT_SETTER( write_request_buffer_name )
  LIBGCT_SETTER( read_request_buffer_name )
  LIBGCT_SETTER( update_request_buffer_name )
  LIBGCT_SETTER( resources )
  std::shared_ptr< allocator_t > allocator;
  std::shared_ptr< descriptor_pool_t > descriptor_pool;
  std::shared_ptr< pipeline_cache_t > pipeline_cache;
  std::filesystem::path write_shader;
  std::filesystem::path read_shader;
  std::filesystem::path update_shader;
  std::uint32_t max_matrix_count = 65536u;
  std::string staging_matrix_buffer_name = "staging";
  std::string matrix_buffer_name = "matrix";
  std::string write_request_buffer_name = "request";
  std::string read_request_buffer_name = "request";
  std::string update_request_buffer_name = "request";
  std::vector< named_resource > resources;
};

class matrix_pool : public std::enable_shared_from_this< matrix_pool > {
  struct matrix_state_type {
    LIBGCT_SETTER( valid )
    LIBGCT_SETTER( auto_touch )
    LIBGCT_SETTER( staging_index )
    LIBGCT_SETTER( write_request_index )
    LIBGCT_SETTER( read_request_index )
    LIBGCT_SETTER( update_request_index )
    LIBGCT_SETTER( level )
    LIBGCT_SETTER( local )
    LIBGCT_SETTER( parent )
    LIBGCT_SETTER( self )
    bool valid = false;
    bool auto_touch = false;
    std::optional< matrix_index_t > staging_index;
    std::optional< request_index_t > write_request_index;
    std::optional< request_index_t > read_request_index;
    std::optional< request_index_t > update_request_index;
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
  struct request_range {
    LIBGCT_SETTER( offset )
    LIBGCT_SETTER( count )
    request_index_t offset = 0u;
    request_index_t count = 0u;
  };
public:
  matrix_pool( const matrix_pool_create_info & );
  const matrix_descriptor &root();
  matrix_descriptor allocate( const glm::mat4&, bool ); // standalone matrix
  matrix_descriptor allocate( const matrix_descriptor&, const glm::mat4&, bool ); // chained matrix
  matrix_descriptor get_local( const matrix_descriptor& );
  void touch( const matrix_descriptor& );
  void set( const matrix_descriptor&, const glm::mat4& );
  void get( const matrix_descriptor&, const std::function< void( const glm::mat4& ) >& );
  const matrix_pool_create_info &get_props() const { return state->props; }
  void operator()( command_buffer_recorder_t& );
private:
  struct state_type : std::enable_shared_from_this< state_type > {
    state_type( const matrix_pool_create_info & );
    matrix_index_t allocate_index();
    matrix_index_t allocate_staging_index();
    request_index_t allocate_write_request_index();
    request_index_t allocate_read_request_index();
    void release_index( matrix_index_t );
    matrix_descriptor allocate( const glm::mat4&, bool ); // standalone matrix
    matrix_descriptor allocate( const matrix_descriptor&, const glm::mat4&, bool ); // chained matrix
    void release( matrix_index_t );
    void touch( const matrix_descriptor& );
    void touch( matrix_index_t );
    void set( const matrix_descriptor&, const glm::mat4& );
    void get( const matrix_descriptor&, const std::function< void( const glm::mat4& ) >& );
    void flush( command_buffer_recorder_t& );
    matrix_descriptor get_local( const matrix_descriptor& );
    std::vector< request_range > build_update_request_range();
    matrix_pool_create_info props;
    std::vector< matrix_state_type > matrix_state;
    std::vector< matrix_index_t > deallocated;
    std::unordered_multimap< matrix_index_t, matrix_index_t > edge; // p -> c
    std::shared_ptr< buffer_t > staging_matrix; // mat4[]
    std::shared_ptr< buffer_t > matrix; // mat4[]
    std::shared_ptr< buffer_t > write_request_buffer; // write_request[] destination
    std::shared_ptr< buffer_t > read_request_buffer; // read_request[] source
    std::shared_ptr< buffer_t > update_request_buffer; // update_request_buffer[] target
    std::vector< std::vector< update_request > > update_request_list;
    std::unordered_set< matrix_index_t > update_requested;
    matrix_index_t staging_tail = 0u;
    request_index_t write_request_tail = 0u;
    request_index_t read_request_tail = 0u;
    std::vector< matrix_descriptor > used_on_gpu;
    std::unordered_set< matrix_index_t > modified;
    std::unordered_multimap< matrix_index_t, std::function< void( const glm::mat4& ) > > cbs;
    std::shared_ptr< compute > write;
    std::shared_ptr< compute > read;
    std::shared_ptr< compute > update;
    std::mutex guard;
  };
  std::shared_ptr< state_type > state;
};
}

#endif

