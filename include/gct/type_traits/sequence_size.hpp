#ifndef GCT_TYPE_TRAITS_SEQUENCE_SIZE_HPP
#define GCT_TYPE_TRAITS_SEQUENCE_SIZE_HPP

namespace gct::type_traits {

template< typename T >
struct sequence_size {};
template< template< typename... > typename L, typename ...T >
struct sequence_size< L< T... > > : public std::integral_constant< unsigned int, sizeof...( T ) > {};
template< typename T >
constexpr unsigned int sequence_size_v = sequence_size< T >::value;

}

#endif


