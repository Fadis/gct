#include <nlohmann/json.hpp>
#include <gct/device.hpp>
#include <gct/query_pool.hpp>
#include <gct/get_device.hpp>

namespace gct {
query_pool_t::query_pool_t(
  const std::shared_ptr< device_t > &device,
  const query_pool_create_info_t &create_info
) :
  property_type( create_info ),
  created_from< device_t >( device ) {
  props.rebuild_chain();
  handle = (*device)->createQueryPoolUnique(
    props.get_basic()
  );
}
void query_pool_t::get_result(
  std::uint32_t offset,
  std::uint32_t count,
  std::uint8_t *begin,
  std::uint8_t *end,
  vk::DeviceSize stride,
  vk::QueryResultFlags flags
) const {
  const auto result = get_device( *this )->getQueryPoolResults(
    *handle,
    offset,
    count,
    std::distance( begin, end ),
    reinterpret_cast< void* >( begin ),
    stride,
    flags
  );
  if( result != vk::Result::eSuccess ) {
    vk::detail::throwResultException( vk::Result( result ), "query_pool_t::get_result : getQueryPoolResults failed" );
  }
}
void query_pool_t::reset(
  std::uint32_t offset,
  std::uint32_t count
) const {
  get_device( *this )->resetQueryPool( *handle, offset, count );
}
void query_pool_t::reset() const {
  reset( 0u, props.get_basic().queryCount );
}
void to_json( nlohmann::json &dest, const query_pool_t &src ) {
  dest = nlohmann::json::object();
  dest[ "props" ] = src.get_props();
}
}

