#include <nlohmann/json.hpp>
#include <gct/dpi.hpp>
#include <gct/physical_device_properties.hpp>
namespace gct {
  display_dpi get_dpi(
    const display_properties_t &props
  ) {
    display_dpi dpi;
    dpi.xdpmm = boost::rational< int >( props.basic.physicalResolution.width, props.basic.physicalDimensions.width );
    dpi.ydpmm = boost::rational< int >( props.basic.physicalResolution.height, props.basic.physicalDimensions.height );
    constexpr auto ipmm = boost::rational< int >( 127, 5 );
    dpi.xdpi = dpi.xdpmm * ipmm;
    dpi.ydpi = dpi.ydpmm * ipmm;
    dpi.high_dpi = dpi.xdpi >= boost::rational< int >( 300, 1 );
    dpi.square = boost::rational_cast< int >( dpi.xdpi ) == boost::rational_cast< int >( dpi.ydpi );
    return dpi;
  }
  std::tuple< float, float > pt_to_px( const display_dpi &dpi, float pt ) {
    const auto mm = pt * 0.35278f;
    const auto xpx = mm * boost::rational_cast< float >( dpi.xdpmm );
    const auto ypx = dpi.square ? xpx : mm * boost::rational_cast< float >( dpi.ydpmm );
    return std::make_tuple( xpx, ypx );
  }
  void to_json( nlohmann::json &dest, const display_dpi &src ) {
    dest = nlohmann::json::object();
    dest[ "xdpmm" ] = nlohmann::json::array();
    dest[ "xdpmm" ][ 0 ] = src.xdpmm.numerator();
    dest[ "xdpmm" ][ 1 ] = src.xdpmm.denominator();
    dest[ "ydpmm" ] = nlohmann::json::array();
    dest[ "ydpmm" ][ 0 ] = src.ydpmm.numerator();
    dest[ "ydpmm" ][ 1 ] = src.ydpmm.denominator();
    dest[ "xdpi" ] = nlohmann::json::array();
    dest[ "xdpi" ][ 0 ] = src.xdpi.numerator();
    dest[ "xdpi" ][ 1 ] = src.xdpi.denominator();
    dest[ "ydpi" ] = nlohmann::json::array();
    dest[ "ydpi" ][ 0 ] = src.ydpi.numerator();
    dest[ "ydpi" ][ 1 ] = src.ydpi.denominator();
    dest[ "square" ] = src.square;
    dest[ "high_dpi" ] = src.high_dpi;
  }
  void from_json( const nlohmann::json &src, display_dpi &dest ) {
    dest = nlohmann::json::object();
    dest.xdpmm = boost::rational< int >( int( src[ "xdpmm" ][ 0 ] ), int( src[ "xdpmm" ][ 1 ] ) );
    dest.xdpmm = boost::rational< int >( int( src[ "ydpmm" ][ 0 ] ), int( src[ "ydpmm" ][ 1 ] ) );
    dest.square = dest.xdpmm == dest.ydpmm;
    constexpr auto ipmm = boost::rational< int >( 127, 5 );
    dest.xdpi = dest.xdpmm * ipmm;
    dest.ydpi = dest.ydpmm * ipmm;
    dest.high_dpi = dest.xdpi >= boost::rational< int >( 300, 1 );
  }
}

