#ifndef GCT_SYNCABLE_HPP
#define GCT_SYNCABLE_HPP
#include <memory>
#include <vector>
#include <gct/setter.hpp>
#include <nlohmann/json_fwd.hpp>

namespace gct {

class buffer_t;
class mappable_buffer_t;
class image_t;
class image_view_t;

struct syncable {
  syncable() {}
  syncable( const std::shared_ptr< buffer_t > &b ) : buffer( { b } ) {}
  syncable( const std::shared_ptr< mappable_buffer_t > &b );
  syncable( const std::shared_ptr< image_t > &i ) : image( { i } ) {}
  syncable( const std::shared_ptr< image_view_t > &i ) : image_view( { i } ) {}
  LIBGCT_SETTER( buffer )
  LIBGCT_SETTER( image )
  LIBGCT_SETTER( image_view )
  syncable &add( const std::shared_ptr< buffer_t > &b ) {
    buffer.push_back( b );
    return *this;
  }
  syncable &add( const std::shared_ptr< mappable_buffer_t > &b );
  syncable &add( const std::shared_ptr< image_t > &i ) {
    image.push_back( i );
    return *this;
  }
  syncable &add( const std::shared_ptr< image_view_t > &i ) {
    image_view.push_back( i );
    return *this;
  }
  [[nodiscard]] bool empty() const {
    return buffer.empty() && image.empty() && image_view.empty();
  }
  std::vector< std::shared_ptr< buffer_t > > buffer;
  std::vector< std::shared_ptr< image_t > > image;
  std::vector< std::shared_ptr< image_view_t > > image_view;
};

void to_json( nlohmann::json &dest, const syncable &src );

}

#endif

