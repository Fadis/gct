#ifndef GCT_QUERY_POOL_HPP
#define GCT_QUERY_POOL_HPP
#include <memory>
#include <vulkan/vulkan.hpp>
#include <gct/created_from.hpp>
#include <gct/query_pool_create_info.hpp>
#include <vulkan/vulkan_enums.hpp>
namespace gct {
  struct device_t;
  class query_pool_t : public created_from< device_t >, public std::enable_shared_from_this< query_pool_t > {
  public:
    query_pool_t(
      const std::shared_ptr< device_t >&,
      const query_pool_create_info_t&
    );
    vk::QueryPool &operator*() {
      return *handle;
    }
    const vk::QueryPool &operator*() const {
      return *handle;
    }
    vk::QueryPool* operator->() {
      return &handle.get();
    }
    const vk::QueryPool* operator->() const {
      return &handle.get();
    }
    const query_pool_create_info_t &get_props() const {
      return props;
    }
    void get_result(
      std::uint32_t offset,
      std::uint32_t count,
      std::uint8_t *begin,
      std::uint8_t *end,
      vk::DeviceSize stride,
      vk::QueryResultFlags flags
    ) const;
    template< typename T >
    std::vector< T > get_result(
      std::uint32_t offset,
      std::uint32_t count,
      vk::QueryResultFlags flags
    ) const {
      std::vector< T > temp( count );
      get_result(
        offset,
        count,
        reinterpret_cast< std::uint8_t* >( temp.data() ),
        reinterpret_cast< std::uint8_t* >( std::next( temp.data(), temp.size() ) ),
        sizeof( T ),
        flags
      );
      return temp;
    }
    template< typename T >
    std::vector< T > get_result(
      vk::QueryResultFlags flags
    ) const {
      return get_result< T >(
        0u,
        props.get_basic().queryCount,
        flags
      );
    }
    template< typename T >
    std::vector< T > get_result() const {
      return get_result< T >(
        vk::QueryResultFlags( 0 )
      );
    }
  private:
    query_pool_create_info_t props;
    vk::UniqueHandle< vk::QueryPool, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE > handle;
  };
  void to_json( nlohmann::json &dest, const query_pool_t &src );
}

#endif


