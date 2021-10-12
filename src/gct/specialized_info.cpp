#include <gct/specialization_info.hpp>

namespace gct {
  basic_specialization_info_t &basic_specialization_info_t::rebuild_chain() {
    basic
      .setMapEntryCount( map.size() )
      .setPMapEntries( map.data() )
      .setDataSize( data_size )
      .setPData( data_head );
    chained = true;
    return *this;
  }
}

