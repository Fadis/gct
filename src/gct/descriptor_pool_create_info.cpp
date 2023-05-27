#include <nlohmann/json.hpp>
#include <gct/surface.hpp>
#include <gct/descriptor_pool_create_info.hpp>
#include <vulkan2json/DescriptorPoolCreateInfo.hpp>
#ifdef VK_EXT_INLINE_UNIFORM_BLOCK_EXTENSION_NAME
#include <vulkan2json/DescriptorPoolInlineUniformBlockCreateInfoEXT.hpp>
#endif
#ifdef VK_VALVE_MUTABLE_DESCRIPTOR_TYPE_EXTENSION_NAME
#include <vulkan2json/MutableDescriptorTypeCreateInfoVALVE.hpp>
#endif

namespace gct {
  void to_json( nlohmann::json &root, const descriptor_pool_create_info_t &v ) {
    root = nlohmann::json::object();
    root[ "basic" ] = v.get_basic();
#ifdef VK_EXT_INLINE_UNIFORM_BLOCK_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( inline_uniform_block )
#endif
#ifdef VK_VALVE_MUTABLE_DESCRIPTOR_TYPE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( mutable_descriptor_type )
#endif
  }
  void from_json( const nlohmann::json &root, descriptor_pool_create_info_t &v ) {
    if( !root.is_object() ) throw incompatible_json( "The JSON is incompatible to descriptor_pool_create_info_t", __FILE__, __LINE__ );
    LIBGCT_EXTENSION_FROM_JSON( basic )
#ifdef VK_EXT_INLINE_UNIFORM_BLOCK_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( inline_uniform_block )
#endif
#ifdef VK_VALVE_MUTABLE_DESCRIPTOR_TYPE_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( mutable_descriptor_type )
#endif
  }
  descriptor_pool_create_info_t &descriptor_pool_create_info_t::rebuild_chain() {
    if( chained ) return *this;
    sequential.clear();
    sequential.reserve( unique_types.size() );
    std::transform(
      unique_types.begin(),
      unique_types.end(),
      std::back_inserter( sequential ),
      []( const auto &v ) {
        return v.second;
      }
    );
    basic
      .setPoolSizeCount( sequential.size() )
      .setPPoolSizes( sequential.data() );
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
#ifdef VK_EXT_INLINE_UNIFORM_BLOCK_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( inline_uniform_block )
#endif
#ifdef VK_VALVE_MUTABLE_DESCRIPTOR_TYPE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( mutable_descriptor_type )
#endif
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
  descriptor_pool_create_info_t &descriptor_pool_create_info_t::set_descriptor_pool_size( const vk::DescriptorPoolSize &v ) {
    const auto existing = unique_types.find( v.type );
    if( existing == unique_types.end() )
      unique_types.insert( std::make_pair( v.type, v ) );
    else existing->second.descriptorCount = v.descriptorCount;
    chained = false;
    return *this;
  }
  descriptor_pool_create_info_t &descriptor_pool_create_info_t::set_descriptor_pool_size( vk::DescriptorType type, std::uint32_t count ) {
    return set_descriptor_pool_size(
      vk::DescriptorPoolSize()
        .setType( type )
        .setDescriptorCount( count )
    );
  }
  descriptor_pool_create_info_t &descriptor_pool_create_info_t::clear_descriptor_pool_size() {
    unique_types.clear();
    chained = false;
    return *this;
  }
}

