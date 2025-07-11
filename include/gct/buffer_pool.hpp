#ifndef GCT_BUFFER_POOL_HPP
#define GCT_BUFFER_POOL_HPP
#include <glm/mat4x4.hpp>
#include <cstdint>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <vector>
#include <optional>
#include <functional>
#include <mutex>
#include <boost/container/flat_map.hpp>
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>
#include <gct/named_resource.hpp>
#include <gct/handler.hpp>
#include <gct/buffer_pool_create_info.hpp>
#include <gct/linear_allocator.hpp>
#include <gct/index_range.hpp>
#include <gct/spv_member_pointer.hpp>
#include <gct/interval.hpp>

namespace gct {

class buffer_pool {
public:
  using buffer_level_t = std::uint32_t;
  using buffer_index_t = std::uint32_t;
  using request_index_t = std::uint32_t;
  using buffer_descriptor = handler< buffer_index_t, buffer_pool >;
  using weak_buffer_descriptor = buffer_descriptor::weak_type;
private:
  struct buffer_state_type {
    LIBGCT_SETTER( self )
    weak_buffer_descriptor self;
  };
  struct write_request {
    LIBGCT_SETTER( staging )
    LIBGCT_SETTER( destination )
    buffer_index_t staging = 0u;
    buffer_index_t destination = 0u;
  };
  struct read_request {
    LIBGCT_SETTER( source )
    LIBGCT_SETTER( staging )
    buffer_index_t source = 0u;
    buffer_index_t staging = 0u;
  };
  using request_range = index_range;
public:
  buffer_pool( const buffer_pool_create_info & );
  buffer_descriptor allocate( const std::uint8_t *begin, const std::uint8_t *end ); // standalone
  buffer_descriptor allocate( std::uint32_t count, const std::uint8_t *begin, const std::uint8_t *end );
  buffer_descriptor allocate( std::uint32_t count = 1u );
  void set( const buffer_descriptor&, const std::uint8_t *begin, const std::uint8_t *end );
  void set( const buffer_descriptor&, std::uint32_t index, const std::uint8_t *begin, const std::uint8_t *end );
  void clear( const buffer_descriptor&, std::uint32_t index, std::uint32_t count = 1u );
  void clear( const buffer_descriptor& );
  void clear();
  void get( const buffer_descriptor&, const std::function< void( vk::Result, std::vector< std::uint8_t >&& ) >& );
  void get( const buffer_descriptor&, std::uint32_t index, const std::function< void( vk::Result, std::vector< std::uint8_t >&& ) >& );
  bool is_valid( const buffer_descriptor& ) const;
  [[nodiscard]] const buffer_pool_create_info &get_props() const { return state->props; }
  void operator()( command_buffer_recorder_t& );
  void to_json( nlohmann::json& ) const;
  [[nodiscard]] std::shared_ptr< buffer_t > get_buffer() const {
    return state->buffer;
  }
  [[nodiscard]] spv_member_pointer get_member_pointer() const;
  [[nodiscard]] std::uint32_t size() const;
private:
  struct state_type : std::enable_shared_from_this< state_type > {
    state_type( const buffer_pool_create_info& );
    [[nodiscard]] buffer_index_t allocate_index( std::uint32_t count );
    void release_index( buffer_index_t );
    [[nodiscard]] buffer_descriptor allocate( std::uint32_t count, const std::uint8_t *begin, const std::uint8_t *end ); // standalone
    [[nodiscard]] buffer_descriptor allocate( std::uint32_t count );
    void release( buffer_index_t );
    void set( const buffer_descriptor&, std::uint32_t index, const std::uint8_t *begin, const std::uint8_t *end );
    void clear( const buffer_descriptor&, std::uint32_t, std::uint32_t );
    void clear();
    void get( const buffer_descriptor&, std::uint32_t index, const std::function< void( vk::Result, std::vector< std::uint8_t >&& ) >& );
    [[nodiscard]] bool is_valid( const buffer_descriptor& ) const;
    void flush( command_buffer_recorder_t& );
    [[nodiscard]] std::uint32_t size() const;
    [[nodiscard]] std::vector< request_range > build_update_request_range();
    void fill( const buffer_descriptor&, std::uint32_t count );
    buffer_pool_create_info props;
    std::unordered_map< std::uint32_t, buffer_state_type > buffer_state;
    sized_linear_allocator index_allocator;
    std::shared_ptr< buffer_t > staging_buffer;
    std::shared_ptr< buffer_t > buffer;
    std::shared_ptr< buffer_t > write_request_buffer; // write_request[] destination
    std::shared_ptr< buffer_t > read_request_buffer; // read_request[] source
    interval< std::uint32_t > fill_requests;
    reduced_linear_allocator staging_index_allocator;
    reduced_linear_allocator write_request_index_allocator;
    reduced_linear_allocator read_request_index_allocator;
    std::vector< buffer_descriptor > used_on_gpu;
    std::unordered_set< buffer_index_t > modified;
    std::unordered_multimap< buffer_index_t, std::function< void( vk::Result, std::vector< std::uint8_t >&& ) > > cbs;
    std::shared_ptr< compute > write;
    std::shared_ptr< compute > read;
    std::size_t aligned_size;
    bool execution_pending = false;
    boost::container::flat_map< std::uint32_t, buffer_index_t > staging_index;
    boost::container::flat_map< std::uint32_t, request_index_t > write_request_index;
    boost::container::flat_map< std::uint32_t, request_index_t > read_request_index;
    std::mutex guard;
  };
  std::shared_ptr< state_type > state;
};

void to_json( nlohmann::json&, const buffer_pool& );

}

#endif

