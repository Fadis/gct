#include <iostream>
#include <fstream>
#include <cstdlib>
#include <unordered_map>
#include <unordered_set>
#include <boost/program_options.hpp>
#include <tqdm.hpp>
#include <nlohmann/json.hpp>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstringop-overread"
#include <ttf2mesh.h>
#pragma GCC diagnostic pop

float align( float v, std::uint32_t a ) {
  return float( int32_t( v * a ) )/a;
}

struct pair_hash {
  std::uint64_t operator()( const std::pair< float, float > &v ) const {
    return std::hash< float >()( v.first ) ^ ( std::hash< float >()( v.second ) * 7u );
  }
};

std::uint32_t get_index(
  std::unordered_map< std::pair< float, float >, std::uint32_t, pair_hash > &vhash,
  std::vector< float > &vertices,
  float x,
  float y,
  std::uint32_t a
) {
  x = align( x, a );
  y = align( y, a );
  auto existing = vhash.find( std::make_pair( x, y ) );
  if( existing != vhash.end() ) {
    return existing->second;
  }
  std::uint32_t i = vertices.size() / 3u;
  vertices.push_back( x );
  vertices.push_back( y );
  vertices.push_back( 0 );
  vhash.insert( std::make_pair( std::make_pair( x, y ), i ) );
  return i;
}

int main( int argc, char *argv[] ) {
  namespace po = boost::program_options;
  po::options_description desc( "Options" );
  desc.add_options()
    ( "help,h", "show this message" )
    ( "align,a", po::value< std::uint32_t >()->default_value( 512u ), "vertex alignment" )
    ( "input,i", po::value< std::string >()->required(), "input filename" )
    ( "format,f", po::value< std::string >()->default_value("json"), "output format" )
    ( "min", po::value< std::uint32_t >()->default_value( 0x000000 ), "minimum character code in Unicode" )
    ( "max", po::value< std::uint32_t >()->default_value( 0x10FFFF ), "maximum character code in Unicode" )
    ( "bin,b", po::value< std::string >()->default_value( "font.bin" ), "output binary filename" )
    ( "json,j", po::value< std::string >()->default_value( "font.gltf" ), "output json filename" );
  po::variables_map vm;
  po::store( po::parse_command_line( argc, argv, desc ), vm );
  po::notify( vm );
  if( vm.count( "help" ) ) {
    std::cout << desc << std::endl;
    return 0;
  }
  const std::uint32_t align = vm[ "align" ].as< std::uint32_t >();
  const std::string input = vm[ "input" ].as< std::string >();
  const std::string format = vm[ "format" ].as< std::string >();
  const std::string bin_filename = vm[ "bin" ].as< std::string >();
  const std::string json_filename = vm[ "json" ].as< std::string >();
  const std::uint32_t min_code = vm[ "min" ].as< std::uint32_t >();
  const std::uint32_t max_code = vm[ "max" ].as< std::uint32_t >();

  static const std::unordered_set< std::string > supported_format{
    "json", "bson", "cbor", "msgpack", "ubjson"
  };
  if( supported_format.find( format ) == supported_format.end() ) {
    std::cout << "unsupported format : " << format << std::endl;
    std::abort();
  }

  ttf_t *font = nullptr;
  ttf_load_from_file( input.c_str(), &font, false);

  std::vector< float > vertices;
  std::unordered_map< std::pair< float, float >, std::uint32_t, pair_hash > vhash;
  std::vector< std::uint32_t > faces;
  std::vector< std::tuple< unsigned int, std::uint32_t, std::uint32_t > > glyphs;
  std::pair< float, float > min = std::make_pair(
    std::numeric_limits< float >::max(),
    std::numeric_limits< float >::max()
  );
  std::pair< float, float > max = std::make_pair(
    std::numeric_limits< float >::min(),
    std::numeric_limits< float >::min()
  );
  for( unsigned int glyph_id : tq::trange( font->nglyphs ) ) {
    ttf_mesh_t *out;
    if( font->glyphs[ glyph_id ].index && font->glyphs[ glyph_id ].outline ) {
      if( font->glyphs[ glyph_id ].symbol >= min_code && font->glyphs[ glyph_id ].symbol <= max_code ) {
        auto result = ttf_glyph2mesh( &font->glyphs[ glyph_id ], &out, TTF_QUALITY_LOW, TTF_FEATURE_IGN_ERR );
        if( result == TTF_DONE) {
          std::vector< std::uint32_t > vertex_indices;
          for( unsigned int i = 0u; i != out->nvert; ++i ) {
            vertex_indices.push_back( get_index( vhash, vertices, out->vert[ i ].x, out->vert[ i ].y, align ) );
            min.first = std::min( min.first, vertices[ vertex_indices.back() * 3u ] );
            min.second = std::min( min.second, vertices[ vertex_indices.back() * 3u + 1u ] );
            max.first = std::max( max.first, vertices[ vertex_indices.back() * 3u ] );
            max.second = std::max( max.second, vertices[ vertex_indices.back() * 3u + 1u ] );
          }
          glyphs.push_back( std::make_tuple( glyph_id, faces.size(), 0u ) );
          for( unsigned int i = 0u; i != out->nfaces; ++i ) {
            faces.push_back( vertex_indices[ out->faces[ i ].v1 ] );
            faces.push_back( vertex_indices[ out->faces[ i ].v2 ] );
            faces.push_back( vertex_indices[ out->faces[ i ].v3 ] );
          }
          std::get< 2 >( glyphs.back() ) = faces.size() - std::get< 1 >( glyphs.back() );
          ttf_free_mesh( out );
        }
      }
    }
  }
  std::cout << "vertices : " << vertices.size()/3u << std::endl;
  std::cout << "faces : " << faces.size()/3u << std::endl;
  std::cout << "meshes : " << glyphs.size() << std::endl;
  nlohmann::json root;
  root[ "asset" ] = nlohmann::json::object({
    { "generator", "ttf_to_gltf" },
    { "version", "2.0" }
  });
  root[ "buffers" ] = nlohmann::json::array();
  root[ "buffers" ].push_back(
    nlohmann::json::object({
      { "uri", "font.bin" },
      { "byteLength", vertices.size() * sizeof( float ) + faces.size() * sizeof( std::uint32_t ) }
    })
  );
  root[ "bufferViews" ] = nlohmann::json::array();
  for( unsigned int i = 0u; i != glyphs.size(); ++i ) {
    const auto begin = std::get< 1 >( glyphs[ i ] );
    const auto size = std::get< 2 >( glyphs[ i ] );
    root[ "bufferViews" ].push_back( nlohmann::json::object({
      { "buffer", 0 },
      { "target", 34963 },
      { "byteOffset", vertices.size() * sizeof( float ) + begin * sizeof( std::uint32_t ) },
      { "byteLength", size * sizeof( std::uint32_t ) },
    }) );
  }
  root[ "bufferViews" ].push_back( nlohmann::json::object({
    { "buffer", 0 },
    { "target", 34962 },
    { "byteLength", vertices.size() * sizeof( float ) },
  }) );
  root[ "accessors" ] = nlohmann::json::array();
  for( unsigned int i = 0u; i != glyphs.size(); ++i ) {
    const auto begin = std::get< 1 >( glyphs[ i ] );
    const auto size = std::get< 2 >( glyphs[ i ] );
    root[ "accessors" ].push_back( nlohmann::json::object({
      { "bufferView", i },
      { "componentType", 5125 }, // 32bit int
      { "type", "SCALAR" },
      { "count", size }
    }) );
  }
  root[ "accessors" ].push_back( nlohmann::json::object({
    { "bufferView", glyphs.size() },
    { "componentType", 5126 }, // 32bit float
    { "type", "VEC3" },
    { "count", vertices.size() / 3u },
    { "min", nlohmann::json::array({
      min.first, min.second, 0.f
    })},
    { "max", nlohmann::json::array({
      max.first, max.second, 0.f
    })}
  }) );
  root[ "meshes" ] = nlohmann::json::array();
  for( unsigned int i = 0u; i != glyphs.size(); ++i ) {
    const auto glyph_id = std::get< 0 >( glyphs[ i ] );
    root[ "meshes" ].push_back( nlohmann::json::object({
      { "primitives", nlohmann::json::array({ nlohmann::json::object({
        { "attributes", nlohmann::json::object({
          { "POSITION", glyphs.size() }
        })},
        { "indices", i }
      })})},
      { "name", std::to_string( glyph_id ) }
    }) );
  }
  root[ "nodes" ] = nlohmann::json::array();
  for( unsigned int i = 0u; i != glyphs.size(); ++i ) {
    const auto glyph_id = std::get< 0 >( glyphs[ i ] );
    root[ "nodes" ].push_back( nlohmann::json::object({
      { "mesh", i },
      { "name", std::to_string( glyph_id ) }
    }) );
  }
  root[ "scene" ] = 0;
  root[ "scenes" ].push_back( nlohmann::json::object({
    { "nodes", nlohmann::json::array() }
  }));
  for( unsigned int i = 0u; i != glyphs.size(); ++i ) {
    root[ "scenes" ][ 0 ][ "nodes" ].push_back( i );
  }

  {
    std::fstream bin( bin_filename, std::ios::out|std::ios::binary );
    bin.write( reinterpret_cast< const char* >( vertices.data() ), vertices.size() * sizeof( float ) );
    bin.write( reinterpret_cast< const char* >( faces.data() ), faces.size() * sizeof( std::uint32_t ) );
  }
  {
    if( format == "json" ) {
      std::fstream json( json_filename, std::ios::out );
      json << root.dump( 2 ) << std::endl;
    }
    else if( format == "bson" ) {
      std::fstream json( json_filename, std::ios::out|std::ios::binary );
      auto serialized = nlohmann::json::to_bson( root );
      json.write( reinterpret_cast< const char* >( serialized.data() ), serialized.size() );
    }
    else if( format == "cbor" ) {
      std::fstream json( json_filename, std::ios::out|std::ios::binary );
      auto serialized = nlohmann::json::to_cbor( root );
      json.write( reinterpret_cast< const char* >( serialized.data() ), serialized.size() );
    }
    else if( format == "msgpack" ) {
      std::fstream json( json_filename, std::ios::out|std::ios::binary );
      auto serialized = nlohmann::json::to_msgpack( root );
      json.write( reinterpret_cast< const char* >( serialized.data() ), serialized.size() );
    }
    else if( format == "ubjson" ) {
      std::fstream json( json_filename, std::ios::out|std::ios::binary );
      auto serialized = nlohmann::json::to_ubjson( root );
      json.write( reinterpret_cast< const char* >( serialized.data() ), serialized.size() );
    }
  }

  ttf_free( font );
}

