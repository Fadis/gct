#ifndef GCT_QUERY_POOL_HPP
#define GCT_QUERY_POOL_HPP
#include <memory>
#include <vulkan/vulkan.hpp>
#include <gct/created_from.hpp>
#include <gct/query_pool_create_info.hpp>
#include <vulkan/vulkan_enums.hpp>
#include <gct/vulkan_handle.hpp>
#include <gct/property.hpp>
namespace gct {
  struct device_t;
  class query_pool_t :
    public vulkan_handle< vk::QueryPool >,
    public property< query_pool_create_info_t >,
    public created_from< device_t >,
    public std::enable_shared_from_this< query_pool_t > {
  public:
    query_pool_t(
      const std::shared_ptr< device_t >&,
      const query_pool_create_info_t&
    );
    void get_result(
      std::uint32_t offset,
      std::uint32_t count,
      std::uint8_t *begin,
      std::uint8_t *end,
      vk::DeviceSize stride,
      vk::QueryResultFlags flags
    ) const;
    template< typename T >
    [[nodiscard]] std::vector< T > get_result(
      std::uint32_t offset,
      std::uint32_t count,
      vk::QueryResultFlags flags
    ) const {
      std::vector< T > temp( count, 1 );
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
    void reset() const;
    void reset( std::uint32_t, std::uint32_t ) const;
    template< typename T >
    [[nodiscard]] std::vector< T > get_result(
      vk::QueryResultFlags flags
    ) const {
      return get_result< T >(
        0u,
        props.get_basic().queryCount,
        flags
      );
    }
    template< typename T >
    [[nodiscard]] std::vector< T > get_result() const {
      return get_result< T >(
        vk::QueryResultFlags( vk::QueryResultFlagBits::eWait | vk::QueryResultFlagBits::ePartial )
      );
    }
  };
  void to_json( nlohmann::json &dest, const query_pool_t &src );
}

#endif


