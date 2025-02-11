#ifndef GCT_ALLOCATOR_SET_HPP
#define GCT_ALLOCATOR_SET_HPP
#include <memory>
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>

namespace gct {

class allocator_t;
class descriptor_pool_t;
class pipeline_cache_t;

struct allocator_set_t {
  LIBGCT_SETTER( allocator )
  LIBGCT_SETTER( descriptor_pool )
  LIBGCT_SETTER( pipeline_cache )
  std::shared_ptr< allocator_t > allocator;
  std::shared_ptr< descriptor_pool_t > descriptor_pool;
  std::shared_ptr< pipeline_cache_t > pipeline_cache;
};

void to_json( nlohmann::json &, const allocator_set_t& );

#define LIBGCT_ALLOCATOR_SET_LEGACY_SETTER( name ) \
   decltype(auto) set_allocator( const std::shared_ptr< allocator_t > &v ) { \
     ( name ) .set_allocator( v ); \
     return *this; \
   } \
   decltype(auto) set_allocator( std::shared_ptr< allocator_t > &&v ) { \
     ( name ) .set_allocator( std::move( v ) ); \
     return *this; \
   } \
   decltype(auto) set_descriptor_pool( const std::shared_ptr< descriptor_pool_t > &v ) { \
     ( name ) .set_descriptor_pool( v ); \
     return *this; \
   } \
   decltype(auto) set_descriptor_pool( std::shared_ptr< descriptor_pool_t > &&v ) { \
     ( name ) .set_descriptor_pool( std::move( v ) ); \
     return *this; \
   } \
   decltype(auto) set_pipeline_cache( const std::shared_ptr< pipeline_cache_t > &v ) { \
     ( name ) .set_pipeline_cache( v ); \
     return *this; \
   } \
   decltype(auto) set_pipeline_cache( std::shared_ptr< pipeline_cache_t > &&v ) { \
     ( name ) .set_pipeline_cache( std::move( v ) ); \
     return *this; \
   }

}

#endif

