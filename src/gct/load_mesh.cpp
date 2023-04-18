/*
 * Copyright (C) 2020 Naomasa Matsubayashi
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */
#include <iostream>
#include <vulkan/vulkan.hpp>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnarrowing"
#include <fx/gltf.h>
#pragma GCC diagnostic pop
#include <glm/mat4x4.hpp>
#include <glm/gtx/string_cast.hpp>
#include <gct/exception.hpp>
#include <gct/gltf.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/write_descriptor_set.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/pipeline_layout_create_info.hpp>
#include <gct/shader_module.hpp>
#include <gct/device.hpp>
namespace gct::gltf {
  std::shared_ptr< shader_module_t > get_suboptimal_shader(
    const shader_t &shader,
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
    const shader_t &shader,
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
  std::vector< std::shared_ptr< shader_module_t > > get_shader(
    const std::vector< shader_t > &shader,
    shader_flag_t flag
  ) {
    std::vector< std::shared_ptr< shader_module_t > > temp;
    temp.reserve( shader.size() );
    std::transform(
      shader.begin(),
      shader.end(),
      std::back_inserter( temp ),
      [flag]( const shader_t &s ) {
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
    const std::vector< shader_t > &shader,
    shader_flag_t flag
  ) {
    std::vector< std::shared_ptr< shader_module_t > > temp;
    temp.reserve( shader.size() );
    std::transform(
      shader.begin(),
      shader.end(),
      std::back_inserter( temp ),
      [flag]( const shader_t &s ) {
        return get_shader( s, flag );
      }
    );
    return temp;
  }

  primitive_t create_primitive(
    const fx::gltf::Document &doc,
    const fx::gltf::Primitive &primitive,
    const std::shared_ptr< device_t > &device,
    command_buffer_recorder_t &command_buffer,
    const std::shared_ptr< allocator_t > &allocator,
    const std::shared_ptr< pipeline_cache_t > &pipeline_cache, 
    const std::shared_ptr< descriptor_pool_t > &descriptor_pool, 
    const std::vector< std::shared_ptr< render_pass_t > > &render_pass,
    std::uint32_t subpass,
    const std::vector< shader_t > &shader,
    const textures_t &textures,
    uint32_t swapchain_size,
    int shader_mask,
    const std::vector< std::shared_ptr< descriptor_set_layout_t > > &env_descriptor_set_layout
  ) {
    if( primitive.material < 0 || doc.materials.size() <= size_t( primitive.material ) ) throw invalid_gltf( "参照されたmaterialが存在しない", __FILE__, __LINE__ );
    const auto &material = doc.materials[ primitive.material ];
    std::unordered_map< uint32_t, buffer_window_t > vertex_buffer;
    std::vector< vk::VertexInputBindingDescription > vertex_input_binding;
    std::vector< vk::VertexInputAttributeDescription > vertex_input_attribute;
    const std::unordered_map< std::string, uint32_t > attr2index{
      std::make_pair( std::string( "POSITION" ), 0 ),
      std::make_pair( std::string( "NORMAL" ), 1 ),
      std::make_pair( std::string( "TANGENT" ), 2 ),
      std::make_pair( std::string( "TEXCOORD_0" ), 3 ),
      std::make_pair( std::string( "TEXCOORD_1" ), 4 ),
      std::make_pair( std::string( "COLOR_0" ), 5 ),
      std::make_pair( std::string( "JOINTS_0" ), 6 ),
      std::make_pair( std::string( "WEIGHTS_0" ), 7 )
    };
    uint32_t vertex_count = std::numeric_limits< uint32_t >::max();
    bool rigged = false;
    bool has_tangent = false;
    glm::vec3 min( -1, -1, -1 );
    glm::vec3 max( 1, 1, 1 );
    for( const auto &[target,index]: primitive.attributes ) {
      auto binding = attr2index.find( target );
      if( binding != attr2index.end() ) {
        if( binding->second == 7 ) rigged = true;
        if( binding->second == 2 ) has_tangent = true;
        if( doc.accessors.size() <= size_t( index ) ) throw invalid_gltf( "参照されたaccessorsが存在しない", __FILE__, __LINE__ );
        const auto &accessor = doc.accessors[ index ];
        if( binding->second == 0 ) {
          if( accessor.min.size() >= 3 ) {
            min[ 0 ] = accessor.min[ 0 ];
            min[ 1 ] = accessor.min[ 1 ];
            min[ 2 ] = accessor.min[ 2 ];
          }
          if( accessor.max.size() >= 3 ) {
            max[ 0 ] = accessor.max[ 0 ];
            max[ 1 ] = accessor.max[ 1 ];
            max[ 2 ] = accessor.max[ 2 ];
          }
        } 
        if( accessor.bufferView < 0 || doc.bufferViews.size() <= size_t( accessor.bufferView ) ) throw invalid_gltf( "参照されたbufferViewが存在しない", __FILE__, __LINE__ );
        const auto &view = doc.bufferViews[ accessor.bufferView ];
        if( view.buffer < 0 || doc.buffers.size() <= size_t( view.buffer ) ) throw invalid_gltf( "参照されたbufferが存在しない", __FILE__, __LINE__ );
        const uint32_t default_stride = to_size( accessor.componentType, accessor.type );
        const uint32_t stride = view.byteStride ? view.byteStride : default_stride;
        const uint32_t max_count = ( view.byteLength - ( accessor.byteOffset ) ) / stride;
        if( accessor.count > max_count ) throw invalid_gltf( "指定された要素数に対してbufferViewが小さすぎる" );
        vertex_count = std::min( vertex_count, accessor.count );
        const uint32_t offset = accessor.byteOffset + view.byteOffset;
        vertex_input_binding.push_back(
          vk::VertexInputBindingDescription()
            .setBinding( binding->second )
            .setStride( stride )
            .setInputRate( vk::VertexInputRate::eVertex )
        );
        vertex_input_attribute.push_back(
          vk::VertexInputAttributeDescription()
            .setLocation( binding->second )
            .setBinding( binding->second )
            .setFormat( to_vulkan_format( accessor.componentType, accessor.type, accessor.normalized ) )
        );
        vertex_buffer.insert( std::make_pair( binding->second, buffer_window_t().set_index( view.buffer ).set_offset( offset ) ) );
      }
    }
    if( vertex_count == std::numeric_limits< uint32_t >::max() )
      throw invalid_gltf( "頂点属性がない", __FILE__, __LINE__ );
    if( vertex_count == 0 )
      throw invalid_gltf( "頂点属性がない", __FILE__, __LINE__ );
    primitive_t primitive_;
    auto vs_flag = shader_flag_t::vertex;
    if( rigged ) vs_flag = shader_flag_t( int( vs_flag )|int( shader_flag_t::skin ) );
    if( has_tangent ) vs_flag = shader_flag_t( int( vs_flag )|int( shader_flag_t::tangent ) );
    auto vs = get_shader( shader, vs_flag );
    if( vs.empty() ) throw invalid_gltf( "必要なシェーダがない", __FILE__, __LINE__ );


    auto gs_flag = shader_flag_t::geometry;
    if( rigged ) gs_flag = shader_flag_t( int( gs_flag )|int( shader_flag_t::skin ) );
    if( has_tangent ) gs_flag = shader_flag_t( int( gs_flag )|int( shader_flag_t::tangent ) );
    auto gs = get_shader_optional( shader, gs_flag );

    auto fs_flag = shader_flag_t::fragment;
    if( has_tangent ) fs_flag = shader_flag_t( int( fs_flag )|int( shader_flag_t::tangent ) );
    if( material.pbrMetallicRoughness.baseColorTexture.index != -1 )
      fs_flag = shader_flag_t( int( fs_flag )|int( shader_flag_t::base_color ) );
    if( material.pbrMetallicRoughness.metallicRoughnessTexture.index != -1 )
      fs_flag = shader_flag_t( int( fs_flag )|int( shader_flag_t::metallic_roughness ) );
    if( material.normalTexture.index != -1 )
      fs_flag = shader_flag_t( int( fs_flag )|int( shader_flag_t::normal ) );
    if( material.occlusionTexture.index != -1 )
      fs_flag = shader_flag_t( int( fs_flag )|int( shader_flag_t::occlusion ) );
    if( material.emissiveTexture.index != -1 )
      fs_flag = shader_flag_t( int( fs_flag )|int( shader_flag_t::emissive ) );
    if( shader_mask ) fs_flag = shader_flag_t( shader_mask );
    auto fs = get_shader( shader, fs_flag );
    if( fs.empty() ) throw invalid_gltf( "必要なシェーダがない", __FILE__, __LINE__ );

    gct::descriptor_set_layout_create_info_t descriptor_set_layout_create_info;
    for( const auto &s: vs ) {
      descriptor_set_layout_create_info
        .add_binding( s->get_props().get_reflection() );
    }
    for( const auto &s: gs ) {
      if( s ) {
        descriptor_set_layout_create_info
          .add_binding( s->get_props().get_reflection() );
      }
    }
    for( const auto &s: fs ) {
      descriptor_set_layout_create_info
        .add_binding( s->get_props().get_reflection() );
    }
    descriptor_set_layout_create_info
      .rebuild_chain();
    auto descriptor_set_layout = device->get_descriptor_set_layout(
      descriptor_set_layout_create_info
    );
    auto pipeline_layout_create_info = gct::pipeline_layout_create_info_t()
      .add_descriptor_set_layout( descriptor_set_layout )
      .add_push_constant_range(
        vk::PushConstantRange()
          .setStageFlags( vk::ShaderStageFlagBits::eVertex|vk::ShaderStageFlagBits::eFragment )
          .setOffset( 0 )
          .setSize( sizeof( gct::gltf::push_constants_t ) )
      );
    for( auto &e: env_descriptor_set_layout ) {
      pipeline_layout_create_info
        .add_descriptor_set_layout( e );
    }
    auto pipeline_layout = device->get_pipeline_layout(
      pipeline_layout_create_info
    );

    std::vector< std::shared_ptr< graphics_pipeline_t > > pipelines;
    unsigned int i = 0u;
    for( const auto &r: render_pass ) {
      pipelines.emplace_back(
        create_pipeline(
          pipeline_cache,
          vs[ i ],
          gs[ i ],
          fs[ i ],
          pipeline_layout,
          r,
          subpass,
          vertex_input_binding,
          vertex_input_attribute,
          !material.doubleSided,
          material.alphaMode == fx::gltf::Material::AlphaMode::Blend,
          false
        )
      );
      ++i;
    }
    primitive_.set_vertex_input_binding( vertex_input_binding );
    primitive_.set_vertex_input_attribute( vertex_input_attribute );
    primitive_.set_pipeline( std::move( pipelines ) );
    primitive_.set_pipeline_layout( pipeline_layout );
    primitive_.set_vertex_buffer( vertex_buffer );
    if( primitive.indices >= 0 ) {
      if( doc.accessors.size() <= size_t( primitive.indices ) ) throw invalid_gltf( "参照されたaccessorsが存在しない", __FILE__, __LINE__ );
      const auto &accessor = doc.accessors[ primitive.indices ];
      if( accessor.bufferView < 0 || doc.bufferViews.size() <= size_t( accessor.bufferView ) ) throw invalid_gltf( "参照されたbufferViewが存在しない", __FILE__, __LINE__ );
      const auto &view = doc.bufferViews[ accessor.bufferView ];
      if( view.buffer < 0 || doc.buffers.size() <= size_t( view.buffer ) ) throw invalid_gltf( "参照されたbufferが存在しない", __FILE__, __LINE__ );
      const uint32_t offset = accessor.byteOffset + view.byteOffset;
      primitive_.set_indexed( true );
      primitive_.set_index_buffer( buffer_window_t().set_index( view.buffer ).set_offset( offset ) );
      primitive_.set_index_buffer_type( to_vulkan_index_type( accessor.componentType ) );
      primitive_.set_count( accessor.count );
    }
    else {
      primitive_.set_indexed( false );
      primitive_.set_count( vertex_count );
    }
    uniforms_t uniforms;
    uniforms.set_roughness( material.pbrMetallicRoughness.roughnessFactor );
    uniforms.set_metalness( material.pbrMetallicRoughness.metallicFactor );
    uniforms.emplace_emissive(
      std::pow( material.emissiveFactor[ 0 ], 2.2 ),
      std::pow( material.emissiveFactor[ 1 ], 2.2 ),
      std::pow( material.emissiveFactor[ 2 ], 2.2 ),
      material.emissiveFactor[ 3 ]
    );
    uniforms.emplace_base_color(
      std::pow( material.pbrMetallicRoughness.baseColorFactor[ 0 ], 2.2 ),
      std::pow( material.pbrMetallicRoughness.baseColorFactor[ 1 ], 2.2 ),
      std::pow( material.pbrMetallicRoughness.baseColorFactor[ 2 ], 2.2 ),
      material.pbrMetallicRoughness.baseColorFactor[ 3 ]
    );
    uniforms.set_normal_scale( material.normalTexture.scale );
    uniforms.set_occlusion_strength( material.occlusionTexture.strength );
    auto uniform_bytes_begin = reinterpret_cast< uint8_t* >( reinterpret_cast< void* >( &uniforms ) );
    auto uniform_bytes_end = uniform_bytes_begin + sizeof( uniforms_t );
    auto uniform_buffer = command_buffer.load_buffer(
      allocator,
      std::vector< uint8_t >{ uniform_bytes_begin, uniform_bytes_end },
      vk::BufferUsageFlagBits::eUniformBuffer
    );
    std::shared_ptr< descriptor_set_t > descriptor_set =
      descriptor_pool->allocate(
        descriptor_set_layout
      );
    std::vector< write_descriptor_set_t > updates;
    updates.push_back(
      gct::write_descriptor_set_t()
        .set_basic(
          (*descriptor_set)[ "uniforms" ]
        )
        .add_buffer(
          gct::descriptor_buffer_info_t()
            .set_buffer( uniform_buffer )
            .set_basic(
              vk::DescriptorBufferInfo()
                .setOffset( 0 )
                .setRange( sizeof( uniforms_t ) )
            )
        )
    );

    const auto bct = material.pbrMetallicRoughness.baseColorTexture.index;
    if( bct >= 0 ) {
      if( descriptor_set->has( "base_color" ) ) {
        if( textures.size() <= size_t( bct ) ) throw invalid_gltf( "参照されたtextureが存在しない", __FILE__, __LINE__ );
        const auto &bct_texture = textures[ bct ];
        updates.push_back(
          gct::write_descriptor_set_t()
            .set_basic(
              (*descriptor_set)[ "base_color" ]
            )
            .add_image(
              bct_texture.srgb
            )
        );
      }
    }
    const auto mrt = material.pbrMetallicRoughness.metallicRoughnessTexture.index;
    if( mrt >= 0 ) {
      if( descriptor_set->has( "metallic_roughness" ) ) {
        if( textures.size() <= size_t( mrt ) ) throw invalid_gltf( "参照されたtextureが存在しない", __FILE__, __LINE__ );
        const auto &mrt_texture = textures[ mrt ];
        updates.push_back(
          gct::write_descriptor_set_t()
            .set_basic(
              (*descriptor_set)[ "metallic_roughness" ]
            )
            .add_image(
              mrt_texture.unorm
            )
        );
      }
    }
    const auto nt = material.normalTexture.index;
    if( nt >= 0 ) {
      if( descriptor_set->has( "normal_map" ) ) {
        if( textures.size() <= size_t( nt ) ) throw invalid_gltf( "参照されたtextureが存在しない", __FILE__, __LINE__ );
        const auto &nt_texture = textures[ nt ];
        updates.push_back(
          gct::write_descriptor_set_t()
            .set_basic(
              (*descriptor_set)[ "normal_map" ]
            )
            .add_image(
              nt_texture.unorm
            )
        );
      }
    }
    const auto oct = material.occlusionTexture.index;
    if( oct >= 0 ) {
      if( descriptor_set->has( "occlusion" ) ) {
        if( textures.size() <= size_t( oct ) ) throw invalid_gltf( "参照されたtextureが存在しない", __FILE__, __LINE__ );
        const auto &oct_texture = textures[ oct ];
        updates.push_back(
          gct::write_descriptor_set_t()
            .set_basic(
              (*descriptor_set)[ "occlusion" ]
            )
            .add_image(
              oct_texture.unorm
            )
        );
      }
    }
    const auto emt = material.emissiveTexture.index;
    if( emt >= 0 ) {
      if( descriptor_set->has( "emissive" ) ) {
        if( textures.size() <= size_t( emt ) ) throw invalid_gltf( "参照されたtextureが存在しない", __FILE__, __LINE__ );
        const auto &emt_texture = textures[ emt ];
        updates.push_back(
          gct::write_descriptor_set_t()
            .set_basic(
              (*descriptor_set)[ "emissive" ]
            )
            .add_image(
              emt_texture.srgb
            )
        );
      }
    }
    descriptor_set->update( updates );
    primitive_.set_descriptor_set( descriptor_set ); 
    primitive_.set_min( min );
    primitive_.set_max( max );
    primitive_.set_uniform_buffer(
      uniform_buffer
    );
    return primitive_;
  }
  mesh_t create_mesh(
    const fx::gltf::Document &doc,
    int32_t index,
    const std::shared_ptr< device_t > &device,
    command_buffer_recorder_t &command_buffer,
    const std::shared_ptr< allocator_t > &allocator,
    const std::shared_ptr< pipeline_cache_t > &pipeline_cache, 
    const std::shared_ptr< descriptor_pool_t > &descriptor_pool, 
    const std::vector< std::shared_ptr< render_pass_t > > &render_pass,
    std::uint32_t subpass,
    const std::vector< shader_t > &shader,
    const textures_t &textures,
    uint32_t swapchain_size,
    int shader_mask,
    const std::vector< std::shared_ptr< descriptor_set_layout_t > > &env_descriptor_set_layout
  ) {
    if( index < 0 || doc.meshes.size() <= size_t( index ) ) throw invalid_gltf( "参照されたmeshが存在しない", __FILE__, __LINE__ );
    const auto &mesh = doc.meshes[ index ];
    mesh_t mesh_;
    glm::vec3 min(
      std::numeric_limits< float >::max(),
      std::numeric_limits< float >::max(),
      std::numeric_limits< float >::max()
    );
    glm::vec3 max(
      std::numeric_limits< float >::min(),
      std::numeric_limits< float >::min(),
      std::numeric_limits< float >::min()
    );
    for( const auto &p: mesh.primitives ) {
      mesh_.primitive.push_back( create_primitive(
        doc,
        p,
        device,
        command_buffer,
        allocator,
        pipeline_cache,
        descriptor_pool,
        render_pass,
        subpass,
        shader,
        textures,
        swapchain_size,
        shader_mask,
        env_descriptor_set_layout
      ) );
      min[ 0 ] = std::min( min[ 0 ], mesh_.primitive.back().min[ 0 ] );
      min[ 1 ] = std::min( min[ 1 ], mesh_.primitive.back().min[ 1 ] );
      min[ 2 ] = std::min( min[ 2 ], mesh_.primitive.back().min[ 2 ] );
      max[ 0 ] = std::max( max[ 0 ], mesh_.primitive.back().max[ 0 ] );
      max[ 1 ] = std::max( max[ 1 ], mesh_.primitive.back().max[ 1 ] );
      max[ 2 ] = std::max( max[ 2 ], mesh_.primitive.back().max[ 2 ] );
    }
    mesh_.set_min( min );
    mesh_.set_max( max );
    return mesh_;
  }
  meshes_t create_mesh(
    const fx::gltf::Document &doc,
    const std::shared_ptr< device_t > &device,
    command_buffer_recorder_t &command_buffer,
    const std::shared_ptr< allocator_t > &allocator,
    const std::shared_ptr< descriptor_pool_t > &descriptor_pool, 
    const std::vector< std::shared_ptr< render_pass_t > > &render_pass,
    std::uint32_t subpass,
    const std::vector< shader_t > &shader,
    const textures_t &textures,
    uint32_t swapchain_size,
    int shader_mask,
    const std::vector< std::shared_ptr< descriptor_set_layout_t > > &env_descriptor_set_layout
  ) {
    meshes_t mesh;
    auto pipeline_cache = device->get_pipeline_cache();
    for( uint32_t i = 0; i != doc.meshes.size(); ++i )
      mesh.push_back(
        create_mesh(
          doc,
          i,
          device,
          command_buffer,
          allocator,
          pipeline_cache,
          descriptor_pool,
          render_pass,
          subpass,
          shader,
          textures,
          swapchain_size,
          shader_mask,
          env_descriptor_set_layout
        )
      );
    return mesh;
  }
}

