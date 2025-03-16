#include <nlohmann/json.hpp>
#include <gct/buffer.hpp>
#include <gct/mappable_buffer.hpp>
#include <gct/image.hpp>
#include <gct/image_view.hpp>
#include <gct/syncable.hpp>

namespace gct {
syncable::syncable( const std::shared_ptr< mappable_buffer_t > &b ) : buffer( { b->get_buffer() } ) {}
syncable &syncable::add( const std::shared_ptr< mappable_buffer_t > &b ) {
  buffer.push_back( b->get_buffer() );
  return *this;
}
void to_json( nlohmann::json &dest, const syncable &src ) {
  dest = nlohmann::json::object();
  dest[ "buffer" ] = nlohmann::json::array();
  for( const auto &b: src.buffer ) {
    dest[ "buffer" ].push_back( *b );
  }
  dest[ "image" ] = nlohmann::json::array();
  for( const auto &i: src.image ) {
    dest[ "image" ].push_back( *i );
  }
  dest[ "image_view" ] = nlohmann::json::array();
  for( const auto &i: src.image_view ) {
    dest[ "image_view" ].push_back( *i );
  }
}
}

