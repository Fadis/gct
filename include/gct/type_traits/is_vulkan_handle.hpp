#ifndef GCT_TYPE_TRAITS_IS_VULKAN_HANDLE_HPP
#define GCT_TYPE_TRAITS_IS_VULKAN_HANDLE_HPP

#include <cstdint>
#include <type_traits>
#include <gct/voider.hpp>

namespace gct::type_traits {

template< typename T, typename Enable = void >
struct is_vulkan_handle : public std::false_type {};
template< typename T >
struct is_vulkan_handle<
  T,
  std::enable_if_t<
#if (VK_USE_64_BIT_PTR_DEFINES==1)
    std::is_pointer_v< std::remove_cvref_t< T > >
#else
    std::is_same_v< std::remove_cvref_t< T >, std::uint64_t >
#endif
  >
> : public std::true_type {};
template< typename T >
constexpr bool is_vulkan_handle_v = is_vulkan_handle< T >::value;

template< typename T, typename Enable = void >
struct is_vulkan_hpp_handle : public std::false_type {};
template< typename T >
struct is_vulkan_hpp_handle<
  T,
  std::enable_if_t<
    is_vulkan_handle_v< typename T::NativeType >
  >
> : public std::true_type {};
template< typename T >
constexpr bool is_vulkan_hpp_handle_v = is_vulkan_hpp_handle< T >::value;

}

#endif

