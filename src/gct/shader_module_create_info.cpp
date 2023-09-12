#include <fstream>
#include <iterator>
#include <nlohmann/json.hpp>
#include <gct/shader_module_create_info.hpp>
#include <vulkan2json/ShaderModuleCreateInfo.hpp>
#ifdef VK_EXT_VALIDATION_CACHE_EXTENSION_NAME
#include <vulkan2json/ShaderModuleValidationCacheCreateInfoEXT.hpp>
#endif
#include <gct/spv2json.hpp>
#include <gct/spirv_reflect.h>

namespace gct {
  shader_module_reflection_t::shader_module_reflection_t(
    const std::vector< std::uint8_t > &code
  ) {
    const SpvReflectResult reflect_result = spvReflectCreateShaderModule(
      code.size(),
      code.data(),
      &reflect
    );
    if( reflect_result != SPV_REFLECT_RESULT_SUCCESS ) {
      throw exception::invalid_argument( "spvReflectCreateShaderModule failed", __FILE__, __LINE__ );
    }
  }
  shader_module_reflection_t::~shader_module_reflection_t() {
    spvReflectDestroyShaderModule( &reflect );
  }
  spv_member_pointer shader_module_reflection_t::get_member_pointer( const std::string &name, memory_layout layout ) const {
    for( std::uint32_t i = 0u; i != reflect.descriptor_binding_count; ++i ) {
      if( reflect.descriptor_bindings[ i ].name == name ) {
        const auto descriptor_type = reflect.descriptor_bindings[ i ].descriptor_type;
        if(
          descriptor_type == SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_BUFFER ||
          descriptor_type == SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_BUFFER ||
          descriptor_type == SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC ||
          descriptor_type == SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC
        ) {
          if( reflect.descriptor_bindings[ i ].type_description ) {
            return spv_member_pointer(
              0u,
              *reflect.descriptor_bindings[ i ].type_description,
              layout
            );
          }
          else {
            throw exception::invalid_argument( std::string( "shader_module_reflection_t::get_member_pointer : Descriptor " ) + name + " has no type information.", __FILE__, __LINE__ );
          }
        }
        else {
          throw exception::invalid_argument( std::string( "shader_module_reflection_t::get_member_pointer : Descriptor " ) + name + " is not an uniform buffer or storage buffer.", __FILE__, __LINE__ );
        }
      }
    }
    throw exception::invalid_argument( std::string( "shader_module_reflection_t::get_member_pointer : Descriptor " ) + name + " does not exist.", __FILE__, __LINE__ );
  }
  spv_member_pointer shader_module_reflection_t::get_push_constant_member_pointer( const std::string &name ) const {
    for( std::uint32_t i = 0u; i != reflect.push_constant_block_count; ++i ) {
      if( reflect.push_constant_blocks[ i ].name == name ) {
        if( reflect.push_constant_blocks[ i ].type_description ) {
          auto mp = spv_member_pointer(
            reflect.push_constant_blocks[ i ].absolute_offset,
            *reflect.push_constant_blocks[ i ].type_description,
            memory_layout::std140
          );
          if( mp.get_aligned_size() > reflect.push_constant_blocks[ i ].padded_size ) {
            throw exception::logic_error( std::string( "shader_module_reflection_t::get_push_constant_member_pointer : Push constant " ) + name + " doesn't have enough space to store members.", __FILE__, __LINE__ );
          }
          return mp;
        }
        else {
          throw exception::invalid_argument( std::string( "shader_module_reflection_t::get_push_constant_member_pointer : Push constant " ) + name + " has no type information.", __FILE__, __LINE__ );
        }
      }
    }
    throw exception::invalid_argument( std::string( "shader_module_reflection_t::get_push_constant_member_pointer : Push constant " ) + name + " does not exist.", __FILE__, __LINE__ );
  }


  void to_json( nlohmann::json &root, const shader_module_create_info_t &v ) {
    root = nlohmann::json::object();
    root[ "basic" ] = v.get_basic();
#ifdef VK_EXT_VALIDATION_CACHE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( validation_cache ) 
#endif
    if( !v.get_code().empty() )
      root[ "code" ] = v.get_code();
    if( v.has_reflection() )
      root[ "reflection" ] = *v.get_reflection();
  }
  void from_json( const nlohmann::json &root, shader_module_create_info_t &v ) {
    if( !root.is_object() ) throw incompatible_json( "The JSON is incompatible to shader_module_create_info_t", __FILE__, __LINE__ );
    if( root.find( "code" ) == root.end() ) {
      LIBGCT_EXTENSION_FROM_JSON( basic )
    }
    else {
      v.load_from_memory( std::vector< std::uint8_t >( root[ "code" ] ) );
    }
#ifdef VK_EXT_VALIDATION_CACHE_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( validation_cache ) 
#endif
  }
  shader_module_create_info_t &shader_module_create_info_t::rebuild_chain() {
    if( chained ) return *this;
    if( !code.empty() ) {
      basic.setCodeSize( code.size() );
      basic.setPCode( reinterpret_cast< const uint32_t* >( code.data() ) );
    }
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
#ifdef VK_EXT_VALIDATION_CACHE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( validation_cache ) 
#endif
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
  shader_module_create_info_t &shader_module_create_info_t::load( const std::string &filename ) {
    std::fstream file( filename, std::ios::in|std::ios::binary );
    if( !file.good() ) throw -1;
    std::vector< std::uint8_t > temp( ( std::istreambuf_iterator< char >( file ) ), std::istreambuf_iterator<char>() );
    auto raw = new shader_module_reflection_t( temp );
    chained = false;
    reflect.reset( raw );
    code = std::move( temp );
    return *this;
  }
  shader_module_create_info_t &shader_module_create_info_t::load_from_memory( const std::vector< std::uint8_t > &v ) {
    std::vector< std::uint8_t > temp = v;
    auto raw = new shader_module_reflection_t( temp );
    chained = false;
    reflect.reset( raw );
    code = std::move( temp );
    return *this;
  }
  const shader_module_reflection_t &shader_module_create_info_t::get_reflection() const {
    if( !reflect ) throw exception::invalid_argument( "Reflection is not available", __FILE__, __LINE__ );
    return *reflect;
  }
}

