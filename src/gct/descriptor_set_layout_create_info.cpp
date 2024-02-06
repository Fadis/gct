#include <nlohmann/json.hpp>
#include <cstring>
#include <gct/descriptor_set_layout_create_info.hpp>
#include <gct/shader_module_reflection.hpp>
#include <gct/spv2vk.hpp>
#include <vulkan2json/DescriptorSetLayoutCreateInfo.hpp>
#include <vulkan2json/DescriptorSetLayoutBinding.hpp>
#ifdef VK_VERSION_1_2
#include <vulkan2json/DescriptorSetLayoutBindingFlagsCreateInfo.hpp>
#elif defined(VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME)
#include <vulkan2json/DescriptorSetLayoutBindingFlagsCreateInfoEXT.hpp>
#endif
#ifdef VK_VALVE_MUTABLE_DESCRIPTOR_TYPE_EXTENSION_NAME
#include <vulkan2json/MutableDescriptorTypeCreateInfoVALVE.hpp>
#endif

namespace gct {
  void to_json( nlohmann::json &root, const descriptor_set_layout_create_info_t &v ) {
    root = nlohmann::json::object();
    root[ "basic" ] = v.get_basic();
    root[ "basic" ][ "pBindings" ] = v.get_binding();
#if defined(VK_VERSION_1_2) || defined(VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( binding_flags )
#endif
#ifdef VK_VALVE_MUTABLE_DESCRIPTOR_TYPE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( mutable_descriptor_type )
#endif
    root[ "named_binding" ] = v.get_name_to_binding();
  }
  void from_json( const nlohmann::json &root, descriptor_set_layout_create_info_t &v ) {
    if( !root.is_object() ) throw incompatible_json( "The JSON is incompatible to descriptor_set_layout_create_info_t", __FILE__, __LINE__ );
    LIBGCT_EXTENSION_FROM_JSON( basic )
#if defined(VK_VERSION_1_2) || defined(VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME)
    LIBGCT_EXTENSION_FROM_JSON( binding_flags )
#endif
#ifdef VK_VALVE_MUTABLE_DESCRIPTOR_TYPE_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( mutable_descriptor_type )
#endif
  }
  descriptor_set_layout_create_info_t &descriptor_set_layout_create_info_t::rebuild_chain() {
    if( chained ) return *this;
    basic
      .setBindingCount( binding.size() )
      .setPBindings( binding.empty() ? nullptr : binding.data() );
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
#if defined(VK_VERSION_1_2) || defined(VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( binding_flags )
#endif
#ifdef VK_VALVE_MUTABLE_DESCRIPTOR_TYPE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( mutable_descriptor_type )
#endif
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
  descriptor_set_layout_create_info_t &descriptor_set_layout_create_info_t::add_binding( const vk::DescriptorSetLayoutBinding &v ) {
    auto existing = std::lower_bound( binding.begin(), binding.end(), v, []( const auto &l, const auto &r ) { return l.binding < r.binding; } );
    const auto dup = existing != binding.end() && existing->binding == v.binding;
    if( !dup ) {
      binding.insert( existing, v );
    }
    else {
      if( existing->descriptorType != v.descriptorType )
        throw exception::invalid_argument( "Incompatible descriptor type", __FILE__, __LINE__ );
      if( existing->pImmutableSamplers != v.pImmutableSamplers )
        throw exception::invalid_argument( "Incompatible immutable sampler", __FILE__, __LINE__ );
      existing->descriptorCount = std::max( existing->descriptorCount, v.descriptorCount );
      existing->stageFlags |= v.stageFlags;
    }
    chained = false;
    return *this;
  }
  descriptor_set_layout_create_info_t &descriptor_set_layout_create_info_t::add_binding( const shader_module_reflection_t &v, std::uint32_t set_id ) {
    for( unsigned int i = 0u; i != v->descriptor_binding_count; ++i ) {
      auto &spv_binding = v->descriptor_bindings[ i ];
      if( spv_binding.set == set_id ) {
        add_binding(
          vk::DescriptorSetLayoutBinding()
            .setBinding( spv_binding.binding )
            .setDescriptorType( spv2vk( spv_binding.descriptor_type ) )
            .setDescriptorCount( spv_binding.count )
            .setStageFlags( spv2vk( v->shader_stage ) )
        );
        const auto spirv_id = spv_binding.spirv_id;
        const auto iv_begin = v->interface_variables;
        const auto iv_end = std::next( v->interface_variables, v->interface_variable_count );
        const auto iv = std::find_if(
          iv_begin,
          iv_end,
          [&spirv_id]( const auto &v ) {
            return v.spirv_id == spirv_id;
          }
        );
        if( iv != iv_end ) {
          if( iv->name && std::strlen( iv->name ) != 0u ) {
            std::string name = iv->name;
            if( non_unique_name.find( name ) == non_unique_name.end() ) {
              const auto existing = name_to_binding.find( name );
              if( existing == name_to_binding.end() ) {
                name_to_binding.insert( std::make_pair( name, spv_binding.binding ) );
              }
              else if( existing->second != spv_binding.binding ) {
                name_to_binding.erase( existing );
                non_unique_name.insert( name );
              }
            }
          }
          else if( iv->type_description && iv->type_description->type_name && std::strlen( iv->type_description->type_name ) != 0u ) {
            std::string name = iv->type_description->type_name;
            if( non_unique_name.find( name ) == non_unique_name.end() ) {
              const auto existing = name_to_binding.find( name );
              if( existing == name_to_binding.end() ) {
                name_to_binding.insert( std::make_pair( name, spv_binding.binding ) );
              }
              else if( existing->second != spv_binding.binding ) {
                name_to_binding.erase( existing );
                non_unique_name.insert( name );
              }
            }
          }
        }
      }
    }
    return *this;
  }
  descriptor_set_layout_create_info_t &descriptor_set_layout_create_info_t::clear_binding() {
    binding.clear();
    name_to_binding.clear();
    non_unique_name.clear();
    chained = false;
    return *this;
  }
}

