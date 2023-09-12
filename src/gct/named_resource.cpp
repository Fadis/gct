#include <gct/buffer.hpp>
#include <gct/mappable_buffer.hpp>
#include <gct/image_view.hpp>
#include <gct/sampler.hpp>
#include <gct/named_resource.hpp>

namespace gct {

named_resource::named_resource(
  const std::string &n,
  const std::shared_ptr< buffer_t > &r
) {
  set_name( n );
  set_resource( r );
}
named_resource::named_resource(
  const std::string &n,
  std::uint32_t i,
  const std::shared_ptr< buffer_t > &r
) {
  set_name( n );
  set_index( i );
  set_resource( r );
}
named_resource::named_resource(
  const std::string &n,
  const std::shared_ptr< mappable_buffer_t > &r
) {
  set_name( n );
  set_resource( r );
}
named_resource::named_resource(
  const std::string &n,
  std::uint32_t i,
  const std::shared_ptr< mappable_buffer_t > &r
) {
  set_name( n );
  set_index( i );
  set_resource( r );
}
named_resource::named_resource(
  const std::string &n,
  const std::shared_ptr< image_view_t > &r
) {
  set_name( n );
  set_resource( r );
}
named_resource::named_resource(
  const std::string &n,
  std::uint32_t i,
  const std::shared_ptr< image_view_t > &r
) {
  set_name( n );
  set_index( i );
  set_resource( r );
}
named_resource::named_resource(
  const std::string &n,
  const std::shared_ptr< sampler_t > &s,
  const std::shared_ptr< image_view_t > &v
) {
  set_name( n );
  set_resource( s, v );
}
named_resource::named_resource(
  const std::string &n,
  std::uint32_t i,
  const std::shared_ptr< sampler_t > &s,
  const std::shared_ptr< image_view_t > &v
) {
  set_name( n );
  set_index( i );
  set_resource( s, v );
}
named_resource::named_resource(
  const std::string &n,
  const std::shared_ptr< image_view_t > &v,
  vk::ImageLayout l
) {
  set_name( n );
  set_resource( v, l );
}
named_resource::named_resource(
  const std::string &n,
  std::uint32_t i,
  const std::shared_ptr< image_view_t > &v,
  vk::ImageLayout l
) {
  set_name( n );
  set_index( i );
  set_resource( v, l );
}
named_resource::named_resource(
  const std::string &n,
  const std::shared_ptr< sampler_t > &s,
  const std::shared_ptr< image_view_t > &v,
  vk::ImageLayout l
) {
  set_name( n );
  set_resource( s, v, l );
}
named_resource::named_resource(
  const std::string &n,
  std::uint32_t i,
  const std::shared_ptr< sampler_t > &s,
  const std::shared_ptr< image_view_t > &v,
  vk::ImageLayout l
) {
  set_name( n );
  set_index( i );
  set_resource( s, v, l );
}
named_resource::named_resource(
  const std::string &n,
  const std::vector< std::shared_ptr< buffer_t > > &r
) {
  set_name( n );
  set_resource( r );
}
named_resource::named_resource(
  const std::string &n,
  std::uint32_t i,
  const std::vector< std::shared_ptr< buffer_t > > &r
) {
  set_name( n );
  set_index( i );
  set_resource( r );
}
named_resource::named_resource(
  const std::string &n,
  const std::vector< std::shared_ptr< mappable_buffer_t > > &r
) {
  set_name( n );
  set_resource( r );
}
named_resource::named_resource(
  const std::string &n,
  std::uint32_t i,
  const std::vector< std::shared_ptr< mappable_buffer_t > > &r
) {
  set_name( n );
  set_index( i );
  set_resource( r );
}
named_resource::named_resource(
  const std::string &n,
  const std::vector< std::shared_ptr< image_view_t > > &r
) {
  set_name( n );
  set_resource( r );
}
named_resource::named_resource(
  const std::string &n,
  std::uint32_t i,
  const std::vector< std::shared_ptr< image_view_t > > &r
) {
  set_name( n );
  set_index( i );
  set_resource( r );
}
named_resource::named_resource(
  const std::string &n,
  const std::vector< std::shared_ptr< image_view_t > > &v,
  vk::ImageLayout l
) {
  set_name( n );
  set_resource( v, l );
}
named_resource::named_resource(
  const std::string &n,
  std::uint32_t i,
  const std::vector< std::shared_ptr< image_view_t > > &v,
  vk::ImageLayout l
) {
  set_name( n );
  set_index( i );
  set_resource( v, l );
}
named_resource::named_resource(
  const std::string &n,
  const std::shared_ptr< sampler_t > &s,
  const std::vector< std::shared_ptr< image_view_t > > &i
) {
  set_name( n );
  set_resource( s, i );
}
named_resource::named_resource(
  const std::string &n,
  std::uint32_t i,
  const std::shared_ptr< sampler_t > &s,
  const std::vector< std::shared_ptr< image_view_t > > &v
) {
  set_name( n );
  set_index( i );
  set_resource( s, v );
}
named_resource::named_resource(
  const std::string &n,
  const std::vector< std::tuple<
    std::shared_ptr< sampler_t >,
    std::shared_ptr< image_view_t >
  > > &r
) {
  set_name( n );
  set_resource( r );
}
named_resource::named_resource(
  const std::string &n,
  std::uint32_t i,
  const std::vector< std::tuple<
    std::shared_ptr< sampler_t >,
    std::shared_ptr< image_view_t >
  > > &r
) {
  set_name( n );
  set_index( i );
  set_resource( r );
}
named_resource::named_resource(
  const std::string &n,
  const std::shared_ptr< sampler_t > &s,
  const std::vector< std::shared_ptr< image_view_t > > &i,
  vk::ImageLayout l
) {
  set_name( n );
  set_resource( s, i, l );
}
named_resource::named_resource(
  const std::string &n,
  std::uint32_t i,
  const std::shared_ptr< sampler_t > &s,
  const std::vector< std::shared_ptr< image_view_t > > &v,
  vk::ImageLayout l
) {
  set_name( n );
  set_index( i );
  set_resource( s, v, l );
}
named_resource::named_resource(
  const std::string &n,
  const std::vector< std::tuple<
    std::shared_ptr< sampler_t >,
    std::shared_ptr< image_view_t >,
    vk::ImageLayout
  > > &r
) {
  set_name( n );
  set_resource( r );
}
named_resource::named_resource(
  const std::string &n,
  std::uint32_t i,
  const std::vector< std::tuple<
    std::shared_ptr< sampler_t >,
    std::shared_ptr< image_view_t >,
    vk::ImageLayout
  > > &r
) {
  set_name( n );
  set_index( i );
  set_resource( r );
}
named_resource &named_resource::set_name( const std::string &n ) {
  name = n;
  return *this;
}
named_resource &named_resource::set_name( std::string &&n ) {
  name = std::move( n );
  return *this;
}
named_resource &named_resource::set_index( std::uint32_t i ) {
  index = i;
  return *this;
}
named_resource &named_resource::set_resource( const std::shared_ptr< buffer_t > &r ) {
  resource = std::vector< std::shared_ptr< buffer_t > >{ r };
  return *this;
}
named_resource &named_resource::set_resource( const std::shared_ptr< mappable_buffer_t > &r ) {
  resource = std::vector< std::shared_ptr< buffer_t > >{ r->get_buffer() };
  return *this;
}
named_resource &named_resource::set_resource( const std::shared_ptr< image_view_t > &r ) {
  resource = std::vector< std::shared_ptr< image_view_t > >{ r };
  return *this;
}
named_resource &named_resource::set_resource(
  const std::shared_ptr< sampler_t > &s,
  const std::shared_ptr< image_view_t > &v
) {
  resource = std::vector< std::tuple< std::shared_ptr< sampler_t >, std::shared_ptr< image_view_t > > >{
    std::tuple< std::shared_ptr< sampler_t >, std::shared_ptr< image_view_t > >{
      s, v
    }
  };
  return *this;
}
named_resource &named_resource::set_resource(
  const std::shared_ptr< image_view_t > &v,
  const vk::ImageLayout l
) {
  resource = std::vector< std::tuple< std::shared_ptr< image_view_t >, vk::ImageLayout > >{
    std::tuple< std::shared_ptr< image_view_t >, vk::ImageLayout >{
      v, l
    }
  };
  return *this;
}
named_resource &named_resource::set_resource(
  const std::shared_ptr< sampler_t > &s,
  const std::shared_ptr< image_view_t > &v,
  const vk::ImageLayout l
) {
  resource = std::vector< std::tuple< std::shared_ptr< sampler_t >, std::shared_ptr< image_view_t >, vk::ImageLayout > >{
    std::tuple< std::shared_ptr< sampler_t >, std::shared_ptr< image_view_t >, vk::ImageLayout >{
      s, v, l
    }
  };
  return *this;
}
named_resource &named_resource::set_resource( const std::vector< std::shared_ptr< buffer_t > > &r ) {
  resource = r;
  return *this;
}
named_resource &named_resource::set_resource( const std::vector< std::shared_ptr< mappable_buffer_t > > &r ) {
  std::vector< std::shared_ptr< buffer_t > > temp;
  temp.reserve( r.size() );
  std::transform(
    r.begin(), r.end(), std::back_inserter( temp ),
    []( const auto &v ) {
      return v->get_buffer();
    }
  );
  resource = std::move( temp );
  return *this;
}
named_resource &named_resource::set_resource( const std::vector< std::shared_ptr< image_view_t > > &r ) {
  resource = r;
  return *this;
}
named_resource &named_resource::set_resource(
  const std::shared_ptr< sampler_t > &s,
  const std::vector< std::shared_ptr< image_view_t > > &v
) {
  std::vector< std::tuple< std::shared_ptr< sampler_t >, std::shared_ptr< image_view_t > > > temp;
  temp.reserve( v.size() );
  std::transform(
    v.begin(), v.end(),
    std::back_inserter( temp ),
    [s]( const auto &v ) {
      return std::make_tuple( s, v );
    }
  );
  resource = std::move( temp );
  return *this;
}
named_resource &named_resource::set_resource( const std::vector< std::tuple< std::shared_ptr< sampler_t >, std::shared_ptr< image_view_t > > > &r ) {
  resource = r;
  return *this;
}
named_resource &named_resource::set_resource(
  const std::vector< std::shared_ptr< image_view_t > > &v,
  vk::ImageLayout l
) {
  std::vector< std::tuple< std::shared_ptr< image_view_t >, vk::ImageLayout > > temp;
  temp.reserve( v.size() );
  std::transform(
    v.begin(), v.end(),
    std::back_inserter( temp ),
    [l]( const auto &v ) {
      return std::make_tuple( v, l );
    }
  );
  resource = std::move( temp );
  return *this;
}
named_resource &named_resource::set_resource(
  const std::shared_ptr< sampler_t > &s,
  const std::vector< std::shared_ptr< image_view_t > > &v,
  vk::ImageLayout l
) {
  std::vector< std::tuple< std::shared_ptr< sampler_t >, std::shared_ptr< image_view_t >, vk::ImageLayout > > temp;
  temp.reserve( v.size() );
  std::transform(
    v.begin(), v.end(),
    std::back_inserter( temp ),
    [s,l]( const auto &v ) {
      return std::make_tuple( s, v, l );
    }
  );
  resource = std::move( temp );
  return *this;
}
named_resource &named_resource::set_resource( const std::vector< std::tuple< std::shared_ptr< sampler_t >, std::shared_ptr< image_view_t >, vk::ImageLayout > > &r ) {
  resource = r;
  return *this;
}
bool named_resource::is_uniform() const {
  return std::visit(
    []( const auto &v ) -> bool {
      return v.size() == 1u;
    },
    resource
  );
}

}

