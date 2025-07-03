#include <iostream>
#include <cstdlib>
#include <limits>
#include <nlohmann/json.hpp>
#include <gct/exception.hpp>
#include <gct/linear_allocator.hpp>

namespace gct {

sized_linear_allocator::sized_linear_allocator(
  const linear_allocator_create_info &ci
) : property_type( ci ) {}

std::uint32_t sized_linear_allocator::allocate( std::uint32_t size ) {
  if( deallocated.empty() ) {
    if( ( tail + size ) > props.max ) {
      throw exception::out_of_range( "sized_linear_allocator::allocate : no space left", __FILE__, __LINE__ );
    }
    const std::uint32_t allocated_head = tail;
    tail += size;
    if( size != 1u ) {
      contiguous.insert( std::make_pair( allocated_head, size ) );
    }
    return allocated_head;
  }
  else {
    std::uint32_t best_id = std::numeric_limits< std::uint32_t >().max();
    std::uint32_t size_diff = std::numeric_limits< std::uint32_t >().max();
    auto best_iter = deallocated.begin();
    boost::container::flat_set< std::uint32_t >::iterator iter;
    for( iter = deallocated.begin(); iter != deallocated.end(); ++iter ) {
      std::uint32_t s = 1u;
      const auto c = contiguous.find( *iter );
      if( c != contiguous.end() ) {
        s = c->second;
      }
      if( size <= s && ( s - size ) < size_diff ) {
        best_id = *iter;
        size_diff = s - size;
        best_iter = iter;
        if( size_diff == 0u ) {
          break;
        }
      }
    }
    if( best_id != std::numeric_limits< std::uint32_t >().max() ) {
      std::uint32_t allocated_head = *best_iter;
      deallocated.erase( best_iter );
      if( size != 1u ) {
        contiguous[ allocated_head ] = size;
        contiguous.insert( std::make_pair( allocated_head, size ) );
      }
      if( size_diff != 0u ) {
        deallocated.insert( allocated_head + size );
        if( size_diff != 1u ) {
          contiguous[ allocated_head + size ] = size_diff;
        }
        if( size != 1u ) {
          contiguous[ allocated_head ] = size;
        }
        else {
          contiguous.erase( allocated_head );
        }
      }
      return allocated_head;
    }
    else {
      if( ( tail + size ) > props.max ) {
        throw exception::out_of_range( "sized_linear_allocator::allocate : no space left", __FILE__, __LINE__ );
      }
      const std::uint32_t allocated_head = tail;
      tail += size;
      if( size != 1u ) {
        contiguous.insert( std::make_pair( allocated_head, size ) );
      }
      return allocated_head;
    }
  }
}

std::uint32_t sized_linear_allocator::get_size( std::uint32_t i ) const {
  std::uint32_t current_size = 1u;
  {
    const auto c = contiguous.find( i );
    if( c != contiguous.end() ) {
      current_size = c->second;
    }
  }
  return current_size;
}

void sized_linear_allocator::release( std::uint32_t i ) {
  std::uint32_t current_size = get_size( i );
  {
    const auto c = contiguous.find( i );
    if( c != contiguous.end() ) {
      current_size = c->second;
    }
  }
  if( i == tail - current_size ) {
    tail -= current_size;
    if( current_size > 1u ) {
      contiguous.erase( i );
    }
    if( !deallocated.empty() ) {
      auto back = std::prev( deallocated.end() );
      std::uint32_t previous_size = 1u;
      {
        const auto c = contiguous.find( *back );
        if( c != contiguous.end() ) {
          previous_size = c->second;
        }
      }
      if( *back == tail - previous_size ) {
        tail -= previous_size;
        if( previous_size != 1u ) {
          contiguous.erase( *back );
        }
      }
      deallocated.erase( back, deallocated.end() );
    }
  }
  else if( i < tail - current_size ) {
    const auto insert_at = deallocated.lower_bound( i );
    bool solved = false;
    if( insert_at != deallocated.begin() ) {
      const std::uint32_t previous_id = *std::prev( insert_at );
      std::uint32_t previous_size = 1u;
      {
        const auto c = contiguous.find( previous_id );
        if( c != contiguous.end() ) {
          previous_size = c->second;
        }
      }
      if( previous_id + previous_size == i ) {
        if( insert_at != deallocated.end() ) {
          const std::uint32_t next_id = *insert_at;
          if( previous_id + previous_size + current_size == next_id ) {
            std::uint32_t next_size = 1u;
            {
              const auto c = contiguous.find( next_id );
              if( c != contiguous.end() ) {
                next_size = c->second;
              }
            }
            contiguous[ previous_id ] = previous_size + current_size + next_size;
            contiguous.erase( next_id );
            deallocated.erase( next_id );
          }
          else {
            contiguous[ previous_id ] = previous_size + current_size;
          }
        }
        else {
          contiguous[ previous_id ] = previous_size + current_size;
        }
        contiguous.erase( i );
        solved = true;
      }
    }
    if( !solved && insert_at != deallocated.end() ) {
      const std::uint32_t next_id = *insert_at;
      if( i + current_size == next_id ) {
        std::uint32_t next_size = 1u;
        {
          const auto c = contiguous.find( next_id );
          if( c != contiguous.end() ) {
            next_size = c->second;
          }
        }
        contiguous.erase( i );
        contiguous.insert( std::make_pair( i, current_size + next_size ) );
        contiguous.erase( next_id );
        deallocated.erase( next_id );
        deallocated.insert( i );
        solved = true;
      }
    }
    if( !solved ) {
      deallocated.insert( i );
    }
  }
}

void sized_linear_allocator::check() {
  std::uint32_t id = 0;
  std::uint32_t last_id = 0;
  while( id < tail ) {
    const auto d = deallocated.find( id );
    if( d != deallocated.end() ) {
      const auto c = contiguous.find( id );
      if( c != contiguous.end() ) {
        id += c->second;
      }
      else {
        id += 1;
      }
    }
    else {
      const auto c = contiguous.find( id );
      if( c != contiguous.end() ) {
        id += c->second;
      }
      else {
        id += 1;
      }
    }
    for( std::uint32_t j = last_id + 1; j != id; ++j ) {
      if( deallocated.find( j ) != deallocated.end() ) {
        std::cout << "inconsitent id in deallocated : " << j << std::endl;
        std::cout << nlohmann::json( *this ).dump( 2 ) << std::endl;
        std::abort();
      }
      if( contiguous.find( j ) != contiguous.end() ) {
        std::cout << "inconsitent id in contiguous : " << j << std::endl;
        std::cout << nlohmann::json( *this ).dump( 2 ) << std::endl;
        std::abort();
      }
    }
    last_id = id;
  }
  if( id != tail ) {
    std::cout << "inconsitent tail : expected = " << tail << " calculated = " << id << std::endl;
    std::cout << nlohmann::json( *this ).dump( 2 ) << std::endl;
    std::abort();
  }
}

void sized_linear_allocator::to_json( nlohmann::json &dest ) const {
  dest = nlohmann::json::object();
  dest[ "props" ] = get_props();
  dest[ "tail" ] = tail;
  dest[ "deallocated" ] = nlohmann::json::array();
  for( auto e: deallocated ) {
    dest[ "deallocated" ].push_back( e );
  }
  dest[ "contiguous" ] = nlohmann::json::array();
  for( auto e: contiguous ) {
    auto temp = nlohmann::json::array();
    temp.push_back( e.first );
    temp.push_back( e.second );
    dest[ "contiguous" ].push_back( temp );
  }
}
void to_json( nlohmann::json &dest, const sized_linear_allocator &src ) {
  src.to_json( dest );
}


linear_allocator::linear_allocator(
  const linear_allocator_create_info &ci
) : property_type( ci ) {}
std::uint32_t linear_allocator::allocate() {
  if( deallocated.empty() ) {
    if( tail == props.max ) {
      throw exception::out_of_range( "linear_allocator::allocate : no space left", __FILE__, __LINE__ );
    }
    return tail++;
  }
  else {
    auto back = std::prev( deallocated.end() );
    std::uint32_t temp = *back;
    deallocated.erase( back );
    return temp;
  }
}
void linear_allocator::release( std::uint32_t i ) {
  if( i == tail - 1u ) {
    --tail;
    if( !deallocated.empty() ) {
      auto back = std::prev( deallocated.end() );
      while( *back == tail - 1u ) {
        --tail;
        if( back == deallocated.begin() ) {
          deallocated.clear();
          return;
        }
        --back;
      }
      deallocated.erase( std::next( back ), deallocated.end() );
    }
  }
  else if( i < tail - 1u ) {
    deallocated.insert( i );
  }
}
void linear_allocator::reset() {
  deallocated.clear();
  tail = 0u;
}
void linear_allocator::to_json( nlohmann::json &dest ) const {
  dest = nlohmann::json::object();
  dest[ "props" ] = get_props();
  dest[ "tail" ] = tail;
  dest[ "deallocated" ] = nlohmann::json::array();
  for( auto e: deallocated ) {
    dest[ "deallocated" ].push_back( e );
  }
}
void to_json( nlohmann::json &dest, const linear_allocator &src ) {
  src.to_json( dest );
}

reduced_linear_allocator::reduced_linear_allocator(
  const linear_allocator_create_info &ci
) : property_type( ci ) {}
std::uint32_t reduced_linear_allocator::allocate( std::uint32_t count ) {
  if( ( tail + count ) > props.max ) {
    throw exception::out_of_range( "reduced_linear_allocator::allocate : no space left", __FILE__, __LINE__ );
  }
  const std::uint32_t allocate_head = tail;
  tail += count;
  return allocate_head;
}
void reduced_linear_allocator::reset() {
  tail = 0u;
}
void reduced_linear_allocator::to_json( nlohmann::json &dest ) const {
  dest = nlohmann::json::object();
  dest[ "props" ] = get_props();
  dest[ "tail" ] = tail;
}
void to_json( nlohmann::json &dest, const reduced_linear_allocator &src ) {
  src.to_json( dest );
}

}

