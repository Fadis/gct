#include <gct/command_buffer_recorder.hpp>
#include <gct/query_pool.hpp>
#include <gct/command_buffer.hpp>
namespace gct {
std::shared_ptr< void > command_buffer_recorder_t::begin(
  const std::shared_ptr< query_pool_t > &query_pool,
  std::uint32_t query,
  vk::QueryControlFlags flags
) {
  (*get_factory())->beginQuery(
    **query_pool, query, flags
  );
  get_factory()->unbound()->keep.push_back( query_pool );
  return std::shared_ptr< void >(
    nullptr,
    [command_buffer=get_factory(),query_pool,query]( void* ) {
      (*command_buffer)->endQuery( **query_pool, query );
    }
  );
}
#ifdef VK_EXT_TRANSFORM_FEEDBACK_EXTENSION_NAME
std::shared_ptr< void > command_buffer_recorder_t::begin(
  const std::shared_ptr< query_pool_t > &query_pool,
  std::uint32_t query,
  vk::QueryControlFlags flags,
  std::uint32_t index
) {
  (*get_factory())->beginQueryIndexedEXT(
    **query_pool, query, flags, index
  );
  get_factory()->unbound()->keep.push_back( query_pool );
  return std::shared_ptr< void >(
    nullptr,
    [command_buffer=get_factory(),query_pool,query,index]( void* ) {
      (*command_buffer)->endQueryIndexedEXT( **query_pool, query, index );
    }
  );
}
#endif
}

