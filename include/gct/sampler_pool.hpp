#ifndef GCT_SAMPLER_POOL_HPP
#define GCT_SAMPLER_POOL_HPP
#include <glm/mat4x4.hpp>
#include <cstdint>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <vector>
#include <cstdint>
#include <optional>
#include <functional>
#include <mutex>
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>
#include <gct/matrix_pool.hpp>
#include <gct/sampler_pool_create_info.hpp>
#include <gct/numeric_types.hpp>
#include <gct/sampler.hpp>
#include <gct/linear_allocator.hpp>
#include <gct/index_range.hpp>
#include <gct/named_resource.hpp>
#include <gct/handler.hpp>

namespace gct {

class sampler_pool {
public:
  using sampler_index_t = std::uint32_t;
  using request_index_t = std::uint32_t;
  using sampler_descriptor = handler< sampler_index_t >;
  using weak_sampler_descriptor = sampler_descriptor::weak_type;
private:
  struct sampler_state_type {
    LIBGCT_SETTER( valid )
    LIBGCT_SETTER( write_request_index )
    LIBGCT_SETTER( sampler )
    bool valid = false;
    std::optional< request_index_t > write_request_index;
    std::shared_ptr< sampler_t > sampler;
  };
  struct write_request {
    LIBGCT_SETTER( index )
    LIBGCT_SETTER( sampler )
    sampler_index_t index;
    std::shared_ptr< sampler_t > sampler;
  };
  using request_range = index_range;
public:
  sampler_pool( const sampler_pool_create_info & );
  sampler_descriptor allocate( const sampler_create_info_t& );
  const sampler_pool_create_info &get_props() const { return state->props; }
  void operator()();
  void to_json( nlohmann::json& ) const;
private:
  struct state_type : std::enable_shared_from_this< state_type > {
    state_type( const sampler_pool_create_info & );
    sampler_index_t allocate_index();
    void release_index( sampler_index_t );
    sampler_descriptor allocate( const sampler_create_info_t& );
    void release( sampler_index_t );
    void flush();
    sampler_pool_create_info props;
    std::vector< sampler_state_type > sampler_state;
    linear_allocator index_allocator;
    std::vector< write_request > write_request_list;
    std::vector< sampler_descriptor > used_on_gpu;
    std::shared_ptr< sampler_t > null_sampler; //////
    bool execution_pending = false;
    std::mutex guard;
  };
  std::shared_ptr< state_type > state;
};
void to_json( nlohmann::json&, const sampler_pool& );
}

#endif

