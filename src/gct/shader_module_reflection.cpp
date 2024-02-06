#include <gct/shader_module_reflection.hpp>
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
}

