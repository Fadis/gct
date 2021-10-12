#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>
#include <filesystem>
#include <optional>
#include <boost/program_options.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/phoenix.hpp>

template< typename Iterator >
class include_rule : public boost::spirit::qi::grammar< Iterator, std::pair< std::filesystem::path, bool > > {
    using result_t = std::pair< std::filesystem::path, bool >;
  public:
    include_rule() : include_rule::base_type( root ) {
      namespace qi = boost::spirit::qi;
      namespace phx = boost::phoenix;
      type1 = ( "<" >> *( qi::standard::char_ - '>' ) >> ">" )[
        qi::_pass = phx::bind( &include_rule::create_path, qi::_val, qi::_1, false )
      ];
      type2 = ( "\"" >> *( qi::standard::char_ - '"' ) >> "\"" )[
        qi::_pass = phx::bind( &include_rule::create_path, qi::_val, qi::_1, true )
      ];
      root = qi::omit[ *qi::standard::blank ] >> qi::lit( "#" ) >> qi::omit[ *qi::standard::blank ] >> "include" >> qi::omit[ *qi::standard::blank ] >> ( type1 | type2 );
    }
  private:
    static bool create_path( result_t &dest, const std::vector< char > &src, bool order ) {
      dest = std::make_pair(
        std::filesystem::path( std::string( src.begin(), src.end() ) ),
        order
      );
      return true;
    }
    boost::spirit::qi::rule< Iterator, std::string > type1_str;
    boost::spirit::qi::rule< Iterator, std::string > type2_str;
    boost::spirit::qi::rule< Iterator, result_t > type1;
    boost::spirit::qi::rule< Iterator, result_t > type2;
    boost::spirit::qi::rule< Iterator, result_t > root;
};

std::optional< std::filesystem::path > find_header(
  const std::vector< std::filesystem::path > &path,
  const std::filesystem::path &cd,
  const std::filesystem::path &filename,
  bool order
) {
  if( order ) {
    if( std::filesystem::exists( cd / filename ) )
      return cd / filename;
    for( auto &dir: path ) {
      if( std::filesystem::exists( dir / filename ) )
        return dir / filename;
    }
  }
  else {
    for( auto &dir: path ) {
      if( std::filesystem::exists( dir / filename ) )
        return dir / filename;
    }
    if( std::filesystem::exists( cd / filename ) )
      return cd / filename;
  }
  return std::nullopt;
}

void dump(
  const std::vector< std::filesystem::path > &path,
  const std::filesystem::path &filename
) {
  auto fd = std::ifstream( filename.c_str() );
  static const include_rule< std::string::const_iterator > rule;
  while( !fd.eof() ) {
    std::string line;
    std::getline( fd, line );
    auto begin = line.cbegin();
    auto end = line.cend();
    std::pair< std::filesystem::path, bool > parsed;
    if( boost::spirit::qi::parse( begin, end, rule, parsed ) ) {
      auto cd = filename.parent_path();
      auto header = find_header( path, cd, parsed.first, parsed.second );
      if( header ) dump( path, *header );
    }
    else std::cout << line << std::endl;
  }
}

int main( int argc, const char *argv[] ) {
  namespace po = boost::program_options;
  po::options_description desc( "Options" );
  desc.add_options()
    ( "help,h", "show this message" )
    ( "include,I", po::value< std::vector< std::string > >()->multitoken(), "include path" );
  po::variables_map vm;
  po::store( po::parse_command_line( argc, argv, desc ), vm );
  po::notify( vm );
  if( vm.count( "help" ) ) {
    std::cout << desc << std::endl;
    return 0;
  }
  std::vector< std::string > path_as_string = vm.count( "include" ) ? vm[ "include" ].as< std::vector< std::string > >() : std::vector< std::string >();
  std::vector< std::filesystem::path > path;
  path.reserve( path_as_string.size() );
  std::transform( path_as_string.begin(), path_as_string.end(), std::back_inserter( path ), []( const auto &v ) { return std::filesystem::path( v ); } );
  static const include_rule< std::string::const_iterator > rule;
  while( !std::cin.eof() ) {
    std::string line;
    std::getline( std::cin, line );
    auto begin = line.cbegin();
    auto end = line.cend();
    std::pair< std::filesystem::path, bool > parsed;
    if( boost::spirit::qi::parse( begin, end, rule, parsed ) ) {
      auto cd = std::filesystem::current_path();
      auto header = find_header( path, cd, parsed.first, parsed.second );
      if( header ) {
        dump( path, *header );
      }
    }
    else {
      std::cout << line << std::endl;
    }
  }
}
