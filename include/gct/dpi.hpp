#ifndef LIBGCT_INCLUDE_GCT_DPI_HPP
#define LIBGCT_INCLUDE_GCT_DPI_HPP

#include <boost/rational.hpp>
#include <nlohmann/json_fwd.hpp>

namespace gct {
  struct display_dpi {
    bool square = false;
    bool high_dpi = false;
    boost::rational< int > xdpi = 0;
    boost::rational< int > ydpi = 0;
    boost::rational< int > xdpmm = 0;
    boost::rational< int > ydpmm = 0;
  };
  class display_properties_t;
  display_dpi get_dpi(
    const display_properties_t &props
  );
  std::tuple< float, float > pt_to_px( const display_dpi &dpi, float pt );
  void to_json( nlohmann::json &dest, const display_dpi &src );
  void from_json( const nlohmann::json &src, display_dpi &dest );
}

#endif

