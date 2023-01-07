#include <string>
#include <boost/program_options.hpp>
#include <gct/text/layout_engine.hpp>

int main( int argc, const char *argv[] ) {
  namespace po = boost::program_options;
  po::options_description desc( "Options" );
  desc.add_options()
    ( "help,h", "show this message" )
    ( "input,i", po::value< std::string >(), "input string" );
  po::variables_map vm;
  po::store( po::parse_command_line( argc, argv, desc ), vm );
  po::notify( vm );
  if( vm.count( "help" ) || !vm.count( "input" ) ) {
    std::cout << desc << std::endl;
    return 0;
  }
  std::string text = vm[ "input" ].as< std::string >();
  gct::text::layout_engine engine(
    "Nasu-Regular-20200227.ttf",
    183.167f, // 16pt on 137dpi
    1600.f,
    13.356f // 7pt on 137dpi
  );
  const auto line = engine.parse_lines( text.data(), text.length(), "he-IL" );
  const auto lines = engine.wrap_line( line );
  for( const auto &l: lines ) {
    std::cout << l.glyphs.size() << " : ";
    for( const auto &g: l.glyphs ) {
      std::cout << g.glyph_id << "(" << g.breakable << "," << g.is_rtl << ")" << " ";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
  for( const auto &g: engine.layout( lines ).glyphs ) {
    std::cout << g.glyph_id << "(" << g.x_pos << "," << g.y_pos << ")" << " ";
  }
  std::cout << std::endl;
}
