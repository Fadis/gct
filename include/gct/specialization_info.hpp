#ifndef GCT_SPECIALIZATION_INFO_HPP
#define GCT_SPECIALIZATION_INFO_HPP
#include <memory>
#include <optional>
#include <vulkan/vulkan.hpp>
#include <gct/extension.hpp>

namespace gct {
  class basic_specialization_info_t {
  public:
    using self_type = basic_specialization_info_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
    LIBGCT_BASIC_SETTER( vk::SpecializationInfo )
  protected:
    std::vector< vk::SpecializationMapEntry > map;
    size_t data_size = 0u;
    void *data_head = nullptr;
    void set_unchained() { chained = false; }
  };
  template< typename T >
  class specialization_info_t : public basic_specialization_info_t {
  public:
    specialization_info_t() {
      data_size = sizeof( T );
    }
    specialization_info_t &rebuild_chain() {
      basic_specialization_info_t::rebuild_chain();
      return *this;
    }
    using self_type = specialization_info_t;
  private:
    std::shared_ptr< T > data;
  public:
    specialization_info_t &set_data( const T &&v ) {
      data.reset( new T( v ) );
      data_head = &data.get();
      set_unchained();
      return *this;
    }
    specialization_info_t &set_data( T &&v ) {
      data.reset( new T( std::move( v ) ) );
      data_head = data.get();
      set_unchained();
      return *this;
    }
    template< typename U >
    specialization_info_t &add_map( std::uint32_t id, std::uint32_t offset ) {
      map.push_back(
        vk::SpecializationMapEntry()
          .setConstantID( id )
          .setOffset( offset )
          .setSize( sizeof( U ) )
      );
      set_unchained();
      return *this;
    }
    specialization_info_t &clear_map() {
      map.clear();
      set_unchained();
      return *this;
    }
  };
}

#endif

