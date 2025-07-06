#ifndef GCT_INTERVAL_HPP
#define GCT_INTERVAL_HPP
#include <boost/container/flat_map.hpp>
#include <utility>

namespace gct {

template< typename T >
class interval {
public:
  bool add( T begin, T count ) {
    if( count == T(0) ) return false;

    auto begin_iter = range.lower_bound( begin );
    if( begin_iter != range.end() ) {
      if( begin_iter->first != begin ) {
        if( begin_iter != range.begin() ) {
          auto prev = std::prev( begin_iter );
          if( prev->first + prev->second >= begin ) {
            begin_iter = prev;
          }
        }
      }
    }
    else {
      if( begin_iter != range.begin() ) {
        auto prev = std::prev( begin_iter );
        if( prev->first + prev->second >= begin ) {
          begin_iter = prev;
        }
      }
    }

    auto end_iter = range.lower_bound( begin + count );
    if( end_iter != range.end() ) {
      if( end_iter->first == begin + count ) {
        end_iter = std::next( end_iter );
      }
    }

    if( begin_iter != end_iter ) {
      const auto last = std::prev( end_iter );
      const auto end = std::max( count + begin, last->first + last->second );
      begin = std::min( begin, begin_iter->first );
      count = end - begin;
    }

    if( std::distance( begin_iter, end_iter ) == 1u ) {
      if( begin_iter->first == begin && begin_iter->second == count ) {
        return false;
      }
    }

    range.erase( begin_iter, end_iter );
    range.insert( std::make_pair( begin, count ) );
    return true;
  }
  const boost::container::flat_map< T, T > &get() const {
    return range;
  }
  void clear() {
    range.clear();
  }
private:
  boost::container::flat_map< T, T > range;
};
}

#endif

