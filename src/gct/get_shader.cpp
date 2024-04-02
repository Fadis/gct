#include <gct/get_shader.hpp>

namespace gct {
  std::shared_ptr< shader_module_t > get_suboptimal_shader(
    const std::unordered_map< shader_flag_t, std::shared_ptr< shader_module_t > > &shader,
    shader_flag_t flag,
    int depth
  ) {
    constexpr std::array< shader_flag_t, 6u > flags{
      shader_flag_t::emissive,
      shader_flag_t::occlusion,
      shader_flag_t::normal,
      shader_flag_t::metallic_roughness,
      shader_flag_t::base_color,
      shader_flag_t::tangent
    };
    for( auto &f: flags ) {
      if( int( flag ) & int( f ) ) {
        if( depth ) {
          auto s = get_suboptimal_shader(
            shader,
            shader_flag_t( int( flag ) ^ int( f ) ),
            depth - 1
          );
          if( s ) return s;
        }
        else {
          auto s = shader.find( shader_flag_t( int( flag ) ^ int( f ) ) );
          if( s != shader.end() ) return s->second;
        }
      }
    }
    return std::shared_ptr< shader_module_t >();
  }
  std::shared_ptr< shader_module_t > get_shader(
    const std::unordered_map< shader_flag_t, std::shared_ptr< shader_module_t > > &shader,
    shader_flag_t flag
  ) { 
    auto s = shader.find( flag );
    if( s != shader.end() ) return s->second;
    for( int i = 0; i != 6; ++i ){
      auto sub = get_suboptimal_shader( shader, flag, i );
      if( sub ) return sub;
    }
    return std::shared_ptr< shader_module_t >();
  }
  std::optional< std::filesystem::path > get_suboptimal_shader(
    const std::unordered_map< shader_flag_t, std::filesystem::path > &shader,
    shader_flag_t flag,
    int depth
  ) {
    constexpr std::array< shader_flag_t, 6u > flags{
      shader_flag_t::emissive,
      shader_flag_t::occlusion,
      shader_flag_t::normal,
      shader_flag_t::metallic_roughness,
      shader_flag_t::base_color,
      shader_flag_t::tangent
    };
    for( auto &f: flags ) {
      if( int( flag ) & int( f ) ) {
        if( depth ) {
          auto s = get_suboptimal_shader(
            shader,
            shader_flag_t( int( flag ) ^ int( f ) ),
            depth - 1
          );
          if( s ) return s;
        }
        else {
          auto s = shader.find( shader_flag_t( int( flag ) ^ int( f ) ) );
          if( s != shader.end() ) return s->second;
        }
      }
    }
    return std::nullopt;
  }
  std::optional< std::filesystem::path > get_shader(
    const std::unordered_map< shader_flag_t, std::filesystem::path > &shader,
    shader_flag_t flag
  ) {
    auto s = shader.find( flag );
    if( s != shader.end() ) return s->second;
    for( int i = 0; i != 6; ++i ){
      auto sub = get_suboptimal_shader( shader, flag, i );
      if( sub ) return sub;
    }
    return std::nullopt;
  }
  std::vector< std::shared_ptr< shader_module_t > > get_shader(
    const std::vector< std::unordered_map< shader_flag_t, std::shared_ptr< shader_module_t > > > &shader,
    shader_flag_t flag
  ) {
    std::vector< std::shared_ptr< shader_module_t > > temp;
    temp.reserve( shader.size() );
    std::transform(
      shader.begin(),
      shader.end(),
      std::back_inserter( temp ),
      [flag]( const std::unordered_map< shader_flag_t, std::shared_ptr< shader_module_t > > &s ) {
        return get_shader( s, flag );
      }
    );
    if( std::find(
      temp.begin(),
      temp.end(),
      std::shared_ptr< shader_module_t >()
    ) != temp.end() ) temp.clear();
    return temp;
  }
  std::vector< std::shared_ptr< shader_module_t > > get_shader_optional(
    const std::vector< std::unordered_map< shader_flag_t, std::shared_ptr< shader_module_t > > > &shader,
    shader_flag_t flag
  ) {
    std::vector< std::shared_ptr< shader_module_t > > temp;
    temp.reserve( shader.size() );
    std::transform(
      shader.begin(),
      shader.end(),
      std::back_inserter( temp ),
      [flag]( const std::unordered_map< shader_flag_t, std::shared_ptr< shader_module_t > > &s ) {
        return get_shader( s, flag );
      }
    );
    return temp;
  }

}

