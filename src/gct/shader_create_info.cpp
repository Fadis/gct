#include <vulkan/vulkan.hpp>
#if defined(VK_VERSION_1_3) || defined(VK_EXT_SHADER_OBJECT_EXTENSION_NAME)
#include <fstream>
#include <iterator>
#include <nlohmann/json.hpp>
#include <vulkan2json/ShaderCreateInfoEXT.hpp>
#include <vulkan2json/PushConstantRange.hpp>
#if defined(VK_VERSION_1_3) || defined(VK_EXT_SUBGROUP_SIZE_CONTROL_EXTENSION_NAME)
#include <vulkan2json/PipelineShaderStageRequiredSubgroupSizeCreateInfo.hpp>
#endif
#include <gct/descriptor_set_layout.hpp>
#include <gct/shader_create_info.hpp>
#include <gct/shader_module_reflection.hpp>
#include <gct/spv2json.hpp>
#include <gct/spirv_reflect.h>

namespace gct {
  void to_json( nlohmann::json &root, const shader_create_info_t &v ) {
    root = nlohmann::json::object();
    root[ "basic" ] = v.get_basic();
    LIBGCT_ARRAY_OF_TO_JSON_WRAPPED( basic, pSetLayouts, descriptor_set_layout )
    LIBGCT_ARRAY_OF_TO_JSON( basic, pPushConstantRanges, push_constant_range )
#if defined(VK_VERSION_1_3) || defined(VK_EXT_SUBGROUP_SIZE_CONTROL_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( subgroup_size )
#endif
    if( !v.get_code().empty() )
      root[ "code" ] = v.get_code();
    if( v.has_reflection() )
      root[ "reflection" ] = *v.get_reflection();
  }
  void from_json( const nlohmann::json &root, shader_create_info_t &v ) {
    if( !root.is_object() ) throw incompatible_json( "The JSON is incompatible to shader_create_info_t", __FILE__, __LINE__ );
    LIBGCT_EXTENSION_FROM_JSON( basic )
    LIBGCT_ARRAY_OF_FROM_JSON( basic, pPushConstantRanges, push_constant_range )
    if( root.find( "code" ) != root.end() ) {
      v.load_from_memory( std::vector< std::uint8_t >( root[ "code" ] ) );
    }
#if defined(VK_VERSION_1_3) || defined(VK_EXT_SUBGROUP_SIZE_CONTROL_EXTENSION_NAME)
    LIBGCT_EXTENSION_FROM_JSON( subgroup_size )
#endif
  }
  shader_create_info_t &shader_create_info_t::rebuild_chain() {
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
    if( !code.empty() ) {
      basic.setCodeSize( code.size() );
      basic.setPCode( reinterpret_cast< const uint32_t* >( code.data() ) );
    }
    LIBGCT_ARRAY_OF_REBUILD_CHAIN_WRAPPED( basic, SetLayoutCount, PSetLayouts, descriptor_set_layout )
    LIBGCT_ARRAY_OF_REBUILD_CHAIN( basic, PushConstantRangeCount, PPushConstantRanges, push_constant_range )
#if defined(VK_VERSION_1_3) || defined(VK_EXT_SUBGROUP_SIZE_CONTROL_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( subgroup_size )
#endif
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
  shader_create_info_t &shader_create_info_t::load( const std::string &filename ) {
    std::fstream file( filename, std::ios::in|std::ios::binary );
    if( !file.good() ) throw -1;
    std::vector< std::uint8_t > temp( ( std::istreambuf_iterator< char >( file ) ), std::istreambuf_iterator<char>() );
    auto raw = new shader_module_reflection_t( temp );
    chained = false;
    reflect.reset( raw );
    code = std::move( temp );
    return *this;
  }
  shader_create_info_t &shader_create_info_t::load_from_memory( const std::vector< std::uint8_t > &v ) {
    std::vector< std::uint8_t > temp = v;
    auto raw = new shader_module_reflection_t( temp );
    chained = false;
    reflect.reset( raw );
    code = std::move( temp );
    return *this;
  }
  const shader_module_reflection_t &shader_create_info_t::get_reflection() const {
    if( !reflect ) throw exception::invalid_argument( "Reflection is not available", __FILE__, __LINE__ );
    return *reflect;
  }
  pipeline_layout_create_info_t shader_create_info_t::get_pipeline_layout_create_info() const {
    return
      pipeline_layout_create_info_t()
        .add_descriptor_set_layout( descriptor_set_layout.begin(), descriptor_set_layout.end() )
        .add_push_constant_range( push_constant_range.begin(), push_constant_range.end() );
  }
}

#endif

