#include <iostream>
#include <gct/specialization_info.hpp>

namespace gct {
  basic_specialization_info_t &basic_specialization_info_t::rebuild_chain() {
    basic
      .setMapEntryCount( map.size() )
      .setPMapEntries( map.data() )
      .setDataSize( get_size() )
      .setPData( get_head() );
    chained = true;
    return *this;
  }
  encoded_specialization_info_t::encoded_specialization_info_t() {
  }
  encoded_specialization_info_t &encoded_specialization_info_t::rebuild_chain() {
    basic_specialization_info_t::rebuild_chain();
    return *this;
  }
  encoded_specialization_info_t &encoded_specialization_info_t::set_data( const std::vector< std::uint8_t > &v ) {
    data = v;
    set_unchained();
    return *this;
  }
  encoded_specialization_info_t &encoded_specialization_info_t::set_data( std::vector< std::uint8_t > &&v ) {
    data = std::move( v );
    set_unchained();
    return *this;
  }
  encoded_specialization_info_t &encoded_specialization_info_t::add_map( std::uint32_t id, std::uint32_t offset, std::uint32_t size ) {
    map.push_back(
      vk::SpecializationMapEntry()
        .setConstantID( id )
        .setOffset( offset )
        .setSize( size )
    );
    set_unchained();
    return *this;
  }
  encoded_specialization_info_t &encoded_specialization_info_t::clear_map() {
    map.clear();
    set_unchained();
    return *this;
  }
}

