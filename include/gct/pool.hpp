#ifndef GCT_POOL_HPP
#define GCT_POOL_HPP
#include <cstdint>
#include <memory>
#include <vector>
#include <mutex>
#include <gct/setter.hpp>
#include <gct/linear_allocator.hpp>
#include <gct/index_range.hpp>
#include <gct/exception.hpp>
#include <gct/handler.hpp>

namespace gct {

template< typename T >
class pool {
public:
  using index_t = std::uint32_t;
  using request_index_t = std::uint32_t;
  using descriptor = handler< index_t >;
  using weak_descriptor = descriptor::weak_type;
private:
  struct data_state_type {
    LIBGCT_SETTER( valid )
    LIBGCT_SETTER( data )
    LIBGCT_SETTER( self )
    bool valid = false;
    T data;
    weak_descriptor self;
  };
  using request_range = index_range;
public:
  pool( std::uint32_t size ) : state( new state_type( size ) ) {
  }
  descriptor allocate( const T &v ) {
    std::lock_guard< std::mutex > lock( state->guard );
    return state->allocate( v );
  }
  T get( const descriptor &desc ) const {
    std::lock_guard< std::mutex > lock( state->guard );
    return state->get( desc );
  }
  std::vector< descriptor > get_descriptor() const {
    std::lock_guard< std::mutex > lock( state->guard );
    std::vector< descriptor > desc;
    for( const auto &d : state->data_state ) {
      if( d.valid && !d.self.expired() ) {
        desc.push_back( descriptor( d.self.lock() ) );
      }
    }
    return desc;
  }
  std::vector< T > get_value() const {
    std::lock_guard< std::mutex > lock( state->guard );
    std::vector< T > v;
    for( const auto &d : state->data_state ) {
      if( d.valid && !d.self.expired() ) {
        v.push_back( d.data );
      }
    }
    return v;
  }
private:
  struct state_type : std::enable_shared_from_this< state_type > {
    state_type( std::uint32_t size ) :
      index_allocator( linear_allocator_create_info().set_max( size ) ) {
    }
    index_t allocate_index() {
      const auto index = index_allocator.allocate();
      if( index >= data_state.size() ) {
        data_state.resize( index + 1u );
      }
      return index; 
    }
    void release_index( index_t index ) {
      if( index >= data_state.size() || !data_state[ index ].valid ) {
        return;
      }
      data_state[ index ].valid = false;
      index_allocator.release( index );
    }
    descriptor allocate( const T &d ) {
      const index_t index = allocate_index();
      data_state[ index ]
        .set_valid( true )
        .set_data( d );
      descriptor desc(
        new index_t( index ),
        [self=std::enable_shared_from_this< state_type >::shared_from_this()]( const index_t *p ) {
          if( p ) {
            self->release( *p );
            delete p;
          }
        }
      );
      data_state[ index ]
        .set_self( desc.get_weak() );
      return desc;
    }
    T get( const descriptor &desc ) {
      if( data_state.size() <= *desc || !data_state[ *desc ].valid ) {
        throw exception::invalid_argument( "pool::get : No such sampler" );
      }
      return data_state[ *desc ].data;
    }
    void release( index_t index ) {
      std::lock_guard< std::mutex > lock( guard );
      if( data_state.size() <= index || !data_state[ index ].valid ) {
        return;
      }
      data_state[ index ] = data_state_type();
      release_index( index );
    }
    std::vector< data_state_type > data_state;
    linear_allocator index_allocator;
    std::mutex guard;
  };
  std::shared_ptr< state_type > state;
};

}

#endif

