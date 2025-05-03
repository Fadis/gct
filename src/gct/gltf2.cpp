#include "gct/exception.hpp"
#include <fx/gltf.h>
#include <iterator>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <gct/device.hpp>
#include <gct/shader_module_reflection.hpp>
#include <gct/descriptor_pool_create_info.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/descriptor_set_layout_create_info.hpp>
#include <gct/descriptor_set.hpp>
#include <gct/sampler_create_info.hpp>
#include <gct/sampler.hpp>
#include <gct/allocator.hpp>
#include <gct/mappable_buffer.hpp>
#include <gct/get_device.hpp>
#include <gct/gltf2.hpp>
#include <gct/gltf.hpp>
#include <gct/to_matrix.hpp>
#include <gct/image.hpp>
#include <gct/scene_graph_accessor.hpp>
#include <gct/numeric_types.hpp>
#include <vulkan/vulkan_enums.hpp>
#include <vulkan/vulkan_structs.hpp>
namespace gct::gltf {

vk::PrimitiveTopology gltf_topology_to_vulkan_topology(
  fx::gltf::Primitive::Mode t
) {
  if( t == fx::gltf::Primitive::Mode::Points ) {
    return vk::PrimitiveTopology::ePointList;
  }
  else if( t == fx::gltf::Primitive::Mode::Lines ) {
    return vk::PrimitiveTopology::eLineList;
  }
  else if( t == fx::gltf::Primitive::Mode::LineStrip ) {
    return vk::PrimitiveTopology::eLineStrip;
  }
  else if( t == fx::gltf::Primitive::Mode::Triangles ) {
    return vk::PrimitiveTopology::eTriangleList;
  }
  else if( t == fx::gltf::Primitive::Mode::TriangleStrip ) {
    return vk::PrimitiveTopology::eTriangleStrip;
  }
  else if( t == fx::gltf::Primitive::Mode::TriangleFan ) {
    return vk::PrimitiveTopology::eTriangleFan;
  }
  else {
    throw exception::invalid_argument( "gltf_topology_to_vulkan_topology : Unsupported topology", __FILE__, __LINE__ );
  }
}

scene_graph::mesh_topology_id vulkan_topology_to_topology_id(
  vk::PrimitiveTopology t
) {
  if( t == vk::PrimitiveTopology::ePointList ) {
    return scene_graph::mesh_topology_id::point;
  }
  else if( t == vk::PrimitiveTopology::eLineList ) {
    return scene_graph::mesh_topology_id::line;
  }
  else if( t == vk::PrimitiveTopology::eTriangleList ) {
    return scene_graph::mesh_topology_id::triangle;
  }
  else {
    throw exception::invalid_argument( "vulkan_topology_to_topology_id : Unsupported topology", __FILE__, __LINE__ );
  }
}

numeric_type_t to_numeric_type(
  fx::gltf::Accessor::ComponentType componentType,
  fx::gltf::Accessor::Type type,
  bool normalize
) {
  numeric_type_t n;
  n.sign = true;
  n.rows = 1u;
  if( componentType == fx::gltf::Accessor::ComponentType::Byte ) {
    n.component = numeric_component_type_t::int_;
    n.depth = 8u;
  }
  else if( componentType == fx::gltf::Accessor::ComponentType::UnsignedByte ) {
    n.component = numeric_component_type_t::int_;
    n.depth = 8u;
    n.sign = false;
  }
  else if( componentType == fx::gltf::Accessor::ComponentType::Short ) {
    n.component = numeric_component_type_t::int_;
    n.depth = 16u;
  }
  else if( componentType == fx::gltf::Accessor::ComponentType::UnsignedShort ) {
    n.component = numeric_component_type_t::int_;
    n.depth = 16u;
    n.sign = false;
  }
  else if( componentType == fx::gltf::Accessor::ComponentType::UnsignedInt ) {
    n.component = numeric_component_type_t::int_;
    n.depth = 32u;
    n.sign = false;
  }
  else if( componentType == fx::gltf::Accessor::ComponentType::Float ) {
    n.component = numeric_component_type_t::float_;
    n.depth = 32u;
  }
  else {
    throw exception::invalid_argument( "to_numeric_type : 使用できないアクセサの型", __FILE__, __LINE__ );
  }

  if( type == fx::gltf::Accessor::Type::Scalar ) {
    n.composite = numeric_composite_type_t::scalar;
  }
  else if( type == fx::gltf::Accessor::Type::Vec2 ) {
    n.composite = numeric_composite_type_t::vector;
    n.rows = 2u;
  }
  else if( type == fx::gltf::Accessor::Type::Vec3 ) {
    n.composite = numeric_composite_type_t::vector;
    n.rows = 3u;
  }
  else if( type == fx::gltf::Accessor::Type::Vec4 ) {
    n.composite = numeric_composite_type_t::vector;
    n.rows = 4u;
  }
  else {
    throw exception::invalid_argument( "to_numeric_type : 使用できないアクセサの型", __FILE__, __LINE__ );
  }
  if( normalize ) {
    n.attr = integer_attribute_t::scaled;
  }
  return n;
}

gltf2::gltf2(
  const gltf2_create_info &ci
) : props( ci ) {
  if( ci.vertex_attribute_map.empty() ) {
    props.vertex_attribute_map[ "INDEX" ] = 0;
    props.vertex_attribute_map[ "POSITION" ] = 1;
    props.vertex_attribute_map[ "NORMAL" ] = 2;
    props.vertex_attribute_map[ "TANGENT" ] = 3;
    props.vertex_attribute_map[ "TEXCOORD_0" ] = 4;
    props.vertex_attribute_map[ "COLOR_0" ] = 5;
    props.vertex_attribute_map[ "JOINT_0" ] = 6;
    props.vertex_attribute_map[ "WEIGHT_0" ] = 7;
  }
  for( const auto &v: props.vertex_attribute_map ) {
    accessor_count = std::max( accessor_count, std::uint32_t( v.second + 1 ) );
  }
  fx::gltf::ReadQuotas quota;
  quota.MaxBufferByteLength = 1024 * 1024 * 1024;
  fx::gltf::Document doc = fx::gltf::LoadFromText( props.filename.string(), quota );
  cd = props.filename.parent_path();
  load_buffer( doc );
  load_sampler( doc );
  load_image( doc );
  load_texture( doc );
  load_mesh( doc );
  load_camera( doc );
  load_node( doc );
}

void gltf2::load_buffer(
  const fx::gltf::Document &doc
) {
  const auto allocator = props.graph->get_props().allocator_set.allocator;
  for( const auto &b: doc.buffers ) {
    auto buffer_path = std::filesystem::path( b.uri );
    if( buffer_path.is_relative() ) buffer_path = cd / buffer_path;
    buffer.push_back( props.graph->get_resource()->vertex->allocate(
      buffer_path.string()
    ) );
  }
}

/*std::unordered_map< shader_flag_t, std::shared_ptr< shader_module_t > > gltf2::load_shader(
  const fx::gltf::Document &doc,
  const std::filesystem::path &shader_dir
) {
  const auto allocator = props.graph->get_props().allocator;
  std::unordered_map< shader_flag_t, std::shared_ptr< shader_module_t > > shader;
  for( auto &path: std::filesystem::directory_iterator( shader_dir ) ) {
    auto flag = get_shader_flag( path.path() );
    if( flag ) {
      shader.emplace(
        *flag,
        get_device( *allocator ).get_shader_module(
          path.path().string()
        )
      );
    }
  }
  return shader;
}*/

void gltf2::load_sampler(
  const fx::gltf::Document &doc
) {
  default_sampler = 
    props.graph->get_resource()->sampler->allocate(
      get_basic_linear_sampler_create_info()
    );
  for( unsigned int i = 0u; i != doc.samplers.size(); ++i ) {
    const auto &s = doc.samplers[ i ];
    sampler.push_back(
      props.graph->get_resource()->sampler->allocate(
        sampler_create_info_t()
          .set_basic(
            vk::SamplerCreateInfo()
              .setMagFilter( to_vulkan_mag_filter( s.magFilter ) )
              .setMinFilter( to_vulkan_min_filter( s.minFilter ) )
              .setMipmapMode( to_vulkan_mipmap_mode( s.minFilter ) )
              .setAddressModeU( to_vulkan_address_mode( s.wrapS ) )
              .setAddressModeV( to_vulkan_address_mode( s.wrapT ) )
              .setAddressModeW( to_vulkan_address_mode( s.wrapT ) )
              .setAnisotropyEnable( false )
              .setCompareEnable( false )
              .setMipLodBias( 0.f )
              .setMinLod( 0.f )
              .setMaxLod( 32.f )
              .setBorderColor( vk::BorderColor::eFloatTransparentBlack )
              .setUnnormalizedCoordinates( false )
          )
      )
    );
  }
}

void gltf2::load_image(
  const fx::gltf::Document &doc
) {
  {
    default_image = 
      props.graph->get_resource()->image->allocate();
  }
  for( unsigned int i = 0u; i != doc.images.size(); ++i ) {
    const auto &e = doc.images[ i ];
    auto image_path = std::filesystem::path( e.uri );
    if( image_path.is_relative() ) image_path = cd / image_path;
    image.push_back(
      props.graph->get_resource()->image->allocate(
        image_load_info()
          .set_filename( image_path )
          .set_usage( vk::ImageUsageFlagBits::eSampled )
          .set_enable_linear( props.graph->get_image()->get_props().enable_linear )
      )
    );
  }
}

void gltf2::load_texture(
  const fx::gltf::Document &doc
) {
  for( unsigned int i = 0u; i != doc.textures.size(); ++i ) {
    const auto &e = doc.textures[ i ];
    auto sampler_ = ( e.sampler >= 0u && e.sampler <= sampler.size() ) ?
      sampler[ e.sampler ] :
      default_sampler;
    auto image_ = ( e.source >= 0u && e.source <= image.size() ) ?
      image[ e.source ] :
      default_image;
    texture.push_back(
      props.graph->get_resource()->texture->allocate(
        sampler_,
        image_
      )
    );
  }
}


graphics_pipeline_create_info_t gltf2::create_pipeline(
  const fx::gltf::Document &doc,
  const fx::gltf::Primitive &doc_primitive,
  const scene_graph::primitive &primitive_
) {
  const auto stencil_op = vk::StencilOpState()
    .setFailOp( vk::StencilOp::eKeep )
    .setPassOp( vk::StencilOp::eKeep )
    .setCompareOp( vk::CompareOp::eAlways );
  
  auto vertex_input =
    pipeline_vertex_input_state_create_info_t();
  for( auto &attr: primitive_.vertex_input_attribute )
    vertex_input
      .add_vertex_input_attribute_description( attr );
  for( auto &binding: primitive_.vertex_input_binding )
    vertex_input
      .add_vertex_input_binding_description( binding );
  auto dsci = pipeline_dynamic_state_create_info_t()
    .add_dynamic_state( vk::DynamicState::eViewport )
    .add_dynamic_state( vk::DynamicState::eScissor );
  return graphics_pipeline_create_info_t()
    .set_vertex_input( vertex_input )
    .set_input_assembly(
      pipeline_input_assembly_state_create_info_t()
        .set_basic(
          vk::PipelineInputAssemblyStateCreateInfo()
            .setTopology( gltf_topology_to_vulkan_topology( doc_primitive.mode ) )
        )
    )
    .set_viewport(
      pipeline_viewport_state_create_info_t()
        .set_basic(
          vk::PipelineViewportStateCreateInfo()
            .setViewportCount( 1 )
            .setScissorCount( 1 )
        )
    )
    .set_rasterization(
      pipeline_rasterization_state_create_info_t()
        .set_basic(
          vk::PipelineRasterizationStateCreateInfo()
            .setDepthClampEnable( VK_FALSE )
            .setRasterizerDiscardEnable( VK_FALSE )
            .setPolygonMode( vk::PolygonMode::eFill )
            .setCullMode(
              primitive_.cull ?
              vk::CullModeFlagBits::eBack :
              vk::CullModeFlagBits::eNone
            )
            .setFrontFace( vk::FrontFace::eCounterClockwise )
            .setDepthBiasEnable( VK_FALSE )
            .setLineWidth( 1.0f )
        )
    )
    .set_multisample(
      pipeline_multisample_state_create_info_t()
        .set_basic(
          vk::PipelineMultisampleStateCreateInfo()
        )
    )
    //.set_depth_stencil( std::move( dssci ) )
    //.set_color_blend( std::move( cbsci ) )
    .set_dynamic( std::move( dsci ) )
    .set_layout( props.graph->get_resource()->pipeline_layout )
    //.set_render_pass( render_pass, subpass )
    .rebuild_chain();
}

scene_graph::primitive gltf2::create_primitive(
  const fx::gltf::Document &doc,
  const fx::gltf::Primitive &primitive_
) {
  if( primitive_.material < 0 || doc.materials.size() <= size_t( primitive_.material ) ) throw invalid_gltf( "参照されたmaterialが存在しない", __FILE__, __LINE__ );
  const auto &material = doc.materials[ primitive_.material ];
  std::unordered_map< uint32_t, scene_graph::buffer_offset > vertex_buffer;
  std::vector< vk::VertexInputBindingDescription > vertex_input_binding;
  std::vector< vk::VertexInputAttributeDescription > vertex_input_attribute;
  uint32_t vertex_count = std::numeric_limits< uint32_t >::max();
  bool rigged = false;
  bool has_tangent = false;
  glm::vec3 min( -1, -1, -1 );
  glm::vec3 max( 1, 1, 1 );
  scene_graph::lod_t l;
  for( const auto &[target,index]: primitive_.attributes ) {
    if( target == "WEIGHTS_0" ) rigged = true;
    if( target == "TANGENT" ) has_tangent = true;
    if( doc.accessors.size() <= size_t( index ) ) throw invalid_gltf( "参照されたaccessorsが存在しない", __FILE__, __LINE__ );
    const auto &accessor = doc.accessors[ index ];
    if( accessor.bufferView < 0 || doc.bufferViews.size() <= size_t( accessor.bufferView ) ) throw invalid_gltf( "参照されたbufferViewが存在しない", __FILE__, __LINE__ );
    const auto &view = doc.bufferViews[ accessor.bufferView ];
    const uint32_t default_stride = to_size( accessor.componentType, accessor.type );
    const uint32_t stride = view.byteStride ? view.byteStride : default_stride;
    const uint32_t max_count = ( view.byteLength - ( accessor.byteOffset ) ) / stride;
    if( view.buffer < 0 || doc.buffers.size() <= size_t( view.buffer ) ) throw invalid_gltf( "参照されたbufferが存在しない", __FILE__, __LINE__ );
    if( accessor.count > max_count ) throw invalid_gltf( "指定された要素数に対してbufferViewが小さすぎる" );
    vertex_count = std::min( vertex_count, accessor.count );
    const uint32_t offset = accessor.byteOffset + view.byteOffset;
    const vk::Format vertex_format = to_vulkan_format( accessor.componentType, accessor.type, accessor.normalized );
    if( target == "POSITION" ) {
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

    auto binding = props.graph->get_resource()->attr2index.find( target );
    if( binding != props.graph->get_resource()->attr2index.end() ) {
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
          .setFormat( vertex_format )
      );
      vertex_buffer.insert( std::make_pair( binding->second, scene_graph::buffer_offset().set_buffer( buffer[ view.buffer ] ).set_offset( offset ) ) );
    }
    const auto attribute_index = props.vertex_attribute_map.find( target );
    if( attribute_index != props.vertex_attribute_map.end() ) {
      if( l.level.empty() ) {
        l.level.push_back(
          std::make_pair(
            scene_graph::mesh_t()
              .set_topology( gltf_topology_to_vulkan_topology( primitive_.mode ) )
              .set_vertex_count( vertex_count ),
            0.0f // occupancy
          )
        );
      }
      else {
        l.level[ 0 ].first.set_vertex_count( vertex_count );
      }
      l.level[ 0 ].first.attribute.insert(
        std::make_pair(
          attribute_index->second,
          scene_graph::accessor_t()
            .set_buffer( buffer[ view.buffer ] )
            .set_type(
              to_numeric_type( accessor.componentType, accessor.type, accessor.normalized )
            )
            .set_stride( stride )
            .set_offset( offset )
        )
      );
    }
  }
  constexpr static const float eps = 0.01f;
  if( min[ 0 ] == max[ 0 ] ) {
    max[ 0 ] += eps;
    min[ 0 ] -= eps;
  }
  if( min[ 1 ] == max[ 1 ] ) {
    max[ 1 ] += eps;
    min[ 1 ] -= eps;
  }
  if( min[ 2 ] == max[ 2 ] ) {
    max[ 2 ] += eps;
    min[ 2 ] -= eps;
  }
  if( vertex_count == std::numeric_limits< uint32_t >::max() )
    throw invalid_gltf( "頂点属性がない", __FILE__, __LINE__ );
  if( vertex_count == 0 )
    throw invalid_gltf( "頂点属性がない", __FILE__, __LINE__ );
  scene_graph::primitive p;

  auto vs_flag = shader_flag_t::vertex;
  if( rigged ) vs_flag = shader_flag_t( int( vs_flag )|int( shader_flag_t::skin ) );
  if( has_tangent ) vs_flag = shader_flag_t( int( vs_flag )|int( shader_flag_t::tangent ) );
  p.set_vs_flag( vs_flag );

  auto gs_flag = shader_flag_t::geometry;
  if( rigged ) gs_flag = shader_flag_t( int( gs_flag )|int( shader_flag_t::skin ) );
  if( has_tangent ) gs_flag = shader_flag_t( int( gs_flag )|int( shader_flag_t::tangent ) );
  p.set_gs_flag( gs_flag );

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
  //if( shader_mask ) fs_flag = shader_flag_t( shader_mask );
  p.set_fs_flag( fs_flag );

  p.set_cull( !material.doubleSided );
  p.set_blend( material.alphaMode == fx::gltf::Material::AlphaMode::Blend );
  p.set_vertex_input_binding( vertex_input_binding );
  p.set_vertex_input_attribute( vertex_input_attribute );
  p.set_vertex_buffer( vertex_buffer );
  if( primitive_.indices >= 0 ) {
    if( doc.accessors.size() <= size_t( primitive_.indices ) ) throw invalid_gltf( "参照されたaccessorsが存在しない", __FILE__, __LINE__ );
    const auto &accessor = doc.accessors[ primitive_.indices ];
    if( accessor.bufferView < 0 || doc.bufferViews.size() <= size_t( accessor.bufferView ) ) throw invalid_gltf( "参照されたbufferViewが存在しない", __FILE__, __LINE__ );
    const auto &view = doc.bufferViews[ accessor.bufferView ];
    if( view.buffer < 0 || doc.buffers.size() <= size_t( view.buffer ) ) throw invalid_gltf( "参照されたbufferが存在しない", __FILE__, __LINE__ );
    const uint32_t offset = accessor.byteOffset + view.byteOffset;
    p.set_indexed( true );
    p.set_index_buffer( scene_graph::buffer_offset().set_buffer( buffer[ view.buffer ] ).set_offset( offset ) );
    p.set_index_buffer_type( to_vulkan_index_type( accessor.componentType ) );
    p.set_count( accessor.count );
    const auto accessor_index = props.vertex_attribute_map.find( "INDEX" );
    if( accessor_index != props.vertex_attribute_map.end() ) {
      if( l.level.empty() ) {
        throw invalid_gltf( "At least one vertex attribute is required", __FILE__, __LINE__ );
      }
      vertex_count = accessor.count;
      l.level[ 0 ].first.set_vertex_count( accessor.count );
      l.level[ 0 ].first.attribute.insert(
        std::make_pair(
          accessor_index->second,
          scene_graph::accessor_t()
            .set_buffer( buffer[ view.buffer ] )
            .set_type(
              to_numeric_type( accessor.componentType, accessor.type, accessor.normalized )
            )
            .set_offset( offset )
        )
      );
    }
  }
  else {
    p.set_indexed( false );
    p.set_count( vertex_count );
  }
 
  auto rimp = props.graph->get_resource()->primitive_resource_index->get_member_pointer();
  std::vector< std::uint8_t > ri( rimp.get_aligned_size() );

  p.set_roughness( material.pbrMetallicRoughness.roughnessFactor );
  ri.data()->*rimp[ "roughness" ] = p.roughness;
  p.set_metalness( material.pbrMetallicRoughness.metallicFactor );
  ri.data()->*rimp[ "metallic" ] = p.metalness;
  p.emplace_emissive(
    std::pow( material.emissiveFactor[ 0 ], 2.2 ),
    std::pow( material.emissiveFactor[ 1 ], 2.2 ),
    std::pow( material.emissiveFactor[ 2 ], 2.2 ),
    material.emissiveFactor[ 3 ]
  );
  if( props.graph->get_image()->get_props().enable_linear ) {
    p.emissive = get_rgb_to_xyz( color_space::bt709 ) * p.emissive;
  }
  ri.data()->*rimp[ "emissive" ] = p.emissive;
  p.emplace_base_color(
    std::pow( material.pbrMetallicRoughness.baseColorFactor[ 0 ], 2.2 ),
    std::pow( material.pbrMetallicRoughness.baseColorFactor[ 1 ], 2.2 ),
    std::pow( material.pbrMetallicRoughness.baseColorFactor[ 2 ], 2.2 ),
    material.pbrMetallicRoughness.baseColorFactor[ 3 ]
  );
  if( props.graph->get_image()->get_props().enable_linear ) {
    p.base_color = get_rgb_to_xyz( color_space::bt709 ) * p.base_color;
  }
  ri.data()->*rimp[ "base_color" ] = p.base_color;
  p.set_normal_scale( material.normalTexture.scale );
  ri.data()->*rimp[ "normal_scale" ] = p.normal_scale;
  p.set_occlusion_strength( material.occlusionTexture.strength );
  ri.data()->*rimp[ "occlusion_strength" ] = p.occlusion_strength;

  const auto bct = material.pbrMetallicRoughness.baseColorTexture.index;
  if( bct >= 0 ) {
    if( texture.size() <= size_t( bct ) ) throw invalid_gltf( "参照されたtextureが存在しない", __FILE__, __LINE__ );
    auto view = props.graph->get_resource()->texture->get( texture[ bct ].normalized ).first;
    const auto &profile = view->get_factory()->get_props().get_profile();
    if( profile.gamma == color_gamma::srgb && texture[ bct ].srgb ) {
      p.descriptor.set_base_color_texture( texture[ bct ].srgb );
      ri.data()->*rimp[ "base_color_texture" ] = *texture[ bct ].srgb;
    }
    else if( texture[ bct ].linear ) {
      p.descriptor.set_base_color_texture( texture[ bct ].linear );
      ri.data()->*rimp[ "base_color_texture" ] = *texture[ bct ].linear;
    }
    else {
      p.descriptor.set_base_color_texture( texture[ bct ].normalized );
      ri.data()->*rimp[ "base_color_texture" ] = *texture[ bct ].normalized;
    }
    if( rimp.has( "base_color_gamma" ) ) {
      ri.data()->*rimp[ "base_color_gamma" ] = std::uint32_t( profile.gamma );
    }
    if( rimp.has( "base_color_max_intensity" ) ) {
      ri.data()->*rimp[ "base_color_max_intensity" ] = profile.max_intensity;
    }
    const auto rgb_to_xyz = props.graph->get_resource()->csmat.from.find( profile.space );
    if( rgb_to_xyz == props.graph->get_resource()->csmat.from.end() ) {
      throw -1;
    }
    if( rimp.has( "base_color_rgb_to_xyz" ) ) { //////
      ri.data()->*rimp[ "base_color_rgb_to_xyz" ] = std::uint32_t( *rgb_to_xyz->second );
    }
  }
  else {
    ri.data()->*rimp[ "base_color_texture" ] = 0u;
    if( rimp.has( "base_color_gamma" ) ) {
      ri.data()->*rimp[ "base_color_gamma" ] = std::uint32_t( color_gamma::srgb );
    }
    if( rimp.has( "base_color_max_intensity" ) ) {
      ri.data()->*rimp[ "base_color_max_intensity" ] = 100.0f;
    }
    const auto rgb_to_xyz = props.graph->get_resource()->csmat.from.find( color_space::bt709 );
    if( rgb_to_xyz == props.graph->get_resource()->csmat.from.end() ) {
      throw -1;
    }
    if( rimp.has( "base_color_rgb_to_xyz" ) ) { //////
      ri.data()->*rimp[ "base_color_rgb_to_xyz" ] = std::uint32_t( *rgb_to_xyz->second );
    }
  }
  const auto mrt = material.pbrMetallicRoughness.metallicRoughnessTexture.index;
  if( mrt >= 0 ) {
    if( texture.size() <= size_t( mrt ) ) throw invalid_gltf( "参照されたtextureが存在しない", __FILE__, __LINE__ );
    p.descriptor.set_metallic_roughness_texture( texture[ mrt ].normalized );
    ri.data()->*rimp[ "metallic_roughness_texture" ] = *texture[ mrt ].normalized;
  }
  else {
    ri.data()->*rimp[ "metallic_roughness_texture" ] = 0u;
  }
  const auto nt = material.normalTexture.index;
  if( nt >= 0 ) {
    if( texture.size() <= size_t( nt ) ) throw invalid_gltf( "参照されたtextureが存在しない", __FILE__, __LINE__ );
    p.descriptor.set_normal_texture( texture[ nt ].normalized );
    ri.data()->*rimp[ "normal_texture" ] = *texture[ nt ].normalized;
  }
  else {
    ri.data()->*rimp[ "normal_texture" ] = 0u;
  }
  const auto oct = material.occlusionTexture.index;
  if( oct >= 0 ) {
    if( texture.size() <= size_t( oct ) ) throw invalid_gltf( "参照されたtextureが存在しない", __FILE__, __LINE__ );
    p.descriptor.set_occlusion_texture( texture[ oct ].normalized );
    ri.data()->*rimp[ "occlusion_texture" ] = *texture[ oct ].normalized;
  }
  else {
    ri.data()->*rimp[ "occlusion_texture" ] = 0u;
  }
  const auto emt = material.emissiveTexture.index;
  if( emt >= 0 ) {
    if( texture.size() <= size_t( emt ) ) throw invalid_gltf( "参照されたtextureが存在しない", __FILE__, __LINE__ );
    auto view = props.graph->get_resource()->texture->get( texture[ emt ].normalized ).first;
    const auto &profile = view->get_factory()->get_props().get_profile();
    if( profile.gamma == color_gamma::srgb && texture[ emt ].srgb ) {
      p.descriptor.set_emissive_texture( texture[ emt ].srgb );
      ri.data()->*rimp[ "emissive_texture" ] = *texture[ emt ].srgb;
    }
    else if( texture[ emt ].linear ) {
      p.descriptor.set_emissive_texture( texture[ emt ].linear );
      ri.data()->*rimp[ "emissive_texture" ] = *texture[ emt ].linear;
    }
    else {
      p.descriptor.set_emissive_texture( texture[ emt ].normalized );
      ri.data()->*rimp[ "emissive_texture" ] = *texture[ emt ].normalized;
    }
    if( rimp.has( "emissive_gamma" ) ) {
      ri.data()->*rimp[ "emissive_gamma" ] = std::uint32_t( profile.gamma );
    }
    if( rimp.has( "emissive_max_intensity" ) ) {
      ri.data()->*rimp[ "emissive_max_intensity" ] = profile.max_intensity;
    }
    const auto rgb_to_xyz = props.graph->get_resource()->csmat.from.find( profile.space );
    if( rgb_to_xyz == props.graph->get_resource()->csmat.from.end() ) {
      throw -1;
    }
    if( rimp.has( "emissive_rgb_to_xyz" ) ) { //////
      ri.data()->*rimp[ "emissive_rgb_to_xyz" ] = std::uint32_t( *rgb_to_xyz->second );
    }
  }
  else {
    ri.data()->*rimp[ "emissive_texture" ] = 0u;
    if( rimp.has( "emissive_gamma" ) ) {
      ri.data()->*rimp[ "emissive_gamma" ] = std::uint32_t( color_gamma::srgb );
    }
    if( rimp.has( "emissive_max_intensity" ) ) {
      ri.data()->*rimp[ "emissive_max_intensity" ] = 100.0f;
    }
    const auto rgb_to_xyz = props.graph->get_resource()->csmat.from.find( color_space::bt709 );
    if( rgb_to_xyz == props.graph->get_resource()->csmat.from.end() ) {
      throw -1;
    }
    if( rimp.has( "emissive_rgb_to_xyz" ) ) { //////
      ri.data()->*rimp[ "emissive_rgb_to_xyz" ] = std::uint32_t( *rgb_to_xyz->second );
    }
    if( rimp.has( "cull" ) ) {
      ri.data()->*rimp[ "cull" ] = p.cull ? 1u : 0u;
    }
  }
  p.aabb.set_min( glm::vec4(
    min[ 0 ],
    min[ 1 ],
    min[ 2 ],
    1.f
  ) );
  p.aabb.set_max( glm::vec4(
    max[ 0 ],
    max[ 1 ],
    max[ 2 ],
    1.f
  ) );
  p.lod = l;
  const bool enable_mesh_shader =
    props.graph->get_resource()->accessor &&
    props.graph->get_resource()->mesh;
  if( enable_mesh_shader ) {
    auto mmp = props.graph->get_resource()->mesh->get_member_pointer();
    std::vector< std::uint8_t > mesh( mmp.get_aligned_size(), 0u );
    auto amp = props.graph->get_resource()->accessor->get_member_pointer();
    std::vector< std::uint8_t > accessor( amp.get_aligned_size(), 0u );
    const auto mesh_desc = props.graph->get_resource()->mesh->allocate( l.level.size() );
    p.descriptor.mesh = mesh_desc;
    if( rimp.has( "mesh" ) ) {
      ri.data()->*rimp[ "mesh" ] = *mesh_desc;
    }
    for( std::uint32_t lod_id = 0u; lod_id != l.level.size(); ++lod_id ) {
      const auto accessor_desc = props.graph->get_resource()->accessor->allocate( accessor_count );
      p.descriptor.accessor.push_back( accessor_desc );
      {
        for( std::uint32_t attr_id = 0u; attr_id != accessor_count; ++attr_id ) {
          const auto attr = l.level[ lod_id ].first.attribute.find( attr_id );
          if( attr != l.level[ lod_id ].first.attribute.end() ) {
            if( amp.has( "enabled" ) ) {
              accessor.data()->*amp[ "enabled" ] = 1u;
            }
            if( amp.has( "vertex_buffer" ) ) {
              accessor.data()->*amp[ "vertex_buffer" ] = *attr->second.buffer;
            }
            else throw -1;
            if( amp.has( "type" ) ) {
              accessor.data()->*amp[ "type" ] = std::uint32_t( scene_graph::to_type_id( attr->second.type ) );
            }
            else throw -1;
            if( amp.has( "normalized" ) ) {
              accessor.data()->*amp[ "normalized" ] = ( attr->second.type.attr == integer_attribute_t::scaled ) ? 1u : 0u;
            }
            else throw -1;
            if( amp.has( "component_count" ) ) {
              accessor.data()->*amp[ "component_count" ] = std::uint32_t( attr->second.type.rows );
            }
            else throw -1;
            if( amp.has( "offset" ) ) {
              accessor.data()->*amp[ "offset" ] = std::uint32_t( attr->second.offset / ( attr->second.type.depth / 8u ) );
            }
            else throw -1;
            if( amp.has( "stride" ) ) {
              if( attr->second.stride == 0u ) {
                accessor.data()->*amp[ "stride" ] = std::uint32_t( attr->second.type.rows );
              }
              else {
                accessor.data()->*amp[ "stride" ] = std::uint32_t( attr->second.stride / ( attr->second.type.depth / 8u ) );
              }
            }
            else throw -1;
            props.graph->get_resource()->accessor->set( accessor_desc, attr_id, accessor.data(), std::next( accessor.data(), accessor.size() ) );
          }
        }
      }
      if( mmp.has( "accessor" ) ) {
        mesh.data()->*mmp[ "accessor" ] = *accessor_desc;
      }
      if( mmp.has( "vertex_count" ) ) {
        mesh.data()->*mmp[ "vertex_count" ] = std::uint32_t( vertex_count );
      }
      if( mmp.has( "topology" ) ) {
        mesh.data()->*mmp[ "topology" ] = std::uint32_t( vulkan_topology_to_topology_id( l.level[ lod_id ].first.topology ) );
      }
      if( mmp.has( "occupancy" ) ) {
        mesh.data()->*mmp[ "occupancy" ] = l.level[ lod_id ].second;
      }
      props.graph->get_resource()->mesh->set( mesh_desc, lod_id, mesh.data(), std::next( mesh.data(), mesh.size() ) );
    }
  }
  p.descriptor.resource_index = props.graph->get_resource()->primitive_resource_index->allocate( ri.data(), std::next( ri.data(), ri.size() ) );
  p.descriptor.aabb = props.graph->get_resource()->aabb->allocate( p.aabb );
  p.set_pipeline_create_info( create_pipeline( doc, primitive_, p ) );
  return p;
}

std::shared_ptr< mesh > gltf2::create_mesh(
  const fx::gltf::Document &doc,
  const fx::gltf::Mesh &doc_mesh
) {
  std::shared_ptr< mesh > m( new mesh() );
  unsigned int i = 0u;
  for( const auto &primitive_: doc_mesh.primitives ) {
    m->prim.push_back(
      props.graph->get_resource()->prim.allocate(
        std::make_shared< scene_graph::primitive >(
          create_primitive(
            doc,
            primitive_
          )
        )
      )
    );
    ++i;
    /*if( m->prim.size() == 1u ) {
      m->aabb = props.graph->get_resource()->prim.get( m->prim.back() )->aabb;
    }
    else {
      m->aabb = m->aabb | props.graph->get_resource()->prim.get( m->prim.back() )->aabb;
    }*/
  }
  return m;
}

void gltf2::load_mesh(
  const fx::gltf::Document &doc
) {
  for( const auto &m: doc.meshes ) {
    mesh_.push_back(
      create_mesh(
        doc,
        m
      )
    );
  }
}

void gltf2::load_camera(
  const fx::gltf::Document &doc
) {
  static const auto lhrh = glm::mat4(-1,0,0,0,0,-1,0,0,0,0,1,0,0,0,0,1);
  for( const auto &c : doc.cameras ) {
    if( c.type == fx::gltf::Camera::Type::Perspective ) {
      camera.emplace_back(
        props.graph->get_resource()->matrix->allocate(
          lhrh *
          glm::perspective(
            c.perspective.yfov,
            ( props.aspect_ratio == 0.f ) ? c.perspective.aspectRatio : props.aspect_ratio,
            c.perspective.znear,
            c.perspective.zfar
          )
        )
      );
    }
    else if( c.type == fx::gltf::Camera::Type::Orthographic ) {
      camera.emplace_back(
        props.graph->get_resource()->matrix->allocate(
          lhrh *
          glm::ortho(
            -c.orthographic.xmag,
            c.orthographic.xmag,
            -c.orthographic.ymag,
            c.orthographic.ymag,
            -c.orthographic.znear,
            c.orthographic.zfar
          )
        )
      );
    }
  }
}

void gltf2::load_node(
  const std::shared_ptr< scene_graph::node > &parent,
  const fx::gltf::Document &doc,
  int32_t index
) {
  if( index < 0 || doc.nodes.size() <= size_t( index ) ) throw invalid_gltf( "参照されたnodeが存在しない", __FILE__, __LINE__ );
  const auto &doc_node = doc.nodes[ index ];
  std::shared_ptr< scene_graph::node > cur;
  if( doc_node.matrix != fx::gltf::defaults::IdentityMatrix ) {
    cur = parent->add_child( to_matrix(
      doc_node.matrix
    ) );
  }
  else {
    cur = parent->add_child( to_matrix(
      doc_node.translation, doc_node.rotation, doc_node.scale
    ) );
  }
  if( doc_node.mesh >= 0 ) {
    if( mesh_.size() <= size_t( doc_node.mesh ) ) throw invalid_gltf( "参照されたmeshが存在しない", __FILE__, __LINE__ );
    aabb4 aabb;
    bool first = true;
    for( const auto &p: mesh_[ doc_node.mesh ]->prim ) {
      auto i = std::make_shared< scene_graph::instance >();
      i->descriptor.set_prim( p );
      i->descriptor.set_matrix( cur->matrix );
      const auto prim = props.graph->get_resource()->prim.get( p );
      i->descriptor.set_aabb( props.graph->get_resource()->aabb->allocate( prim->descriptor.aabb, cur->matrix ) );
      const std::uint32_t vis = 0u;
      i->descriptor.set_visibility( props.graph->get_resource()->visibility->allocate() );
      auto rimp = props.graph->get_resource()->instance_resource_index->get_member_pointer();
      std::vector< std::uint8_t > ri( rimp.get_aligned_size() );
   
      i->initial_world_matrix = cur->initial_world_matrix;
      ri.data()->*rimp[ "world_matrix" ] = *i->descriptor.matrix;
      if( rimp.has( "previous_world_matrix" ) ) {
        if( props.graph->get_resource()->matrix->copy_enabled() ) {
          ri.data()->*rimp[ "previous_world_matrix" ] = *props.graph->get_resource()->matrix->get_history( i->descriptor.matrix );
          //ri.data()->*rimp[ "previous_world_matrix" ] = *i->descriptor.matrix;
        }
        else {
          ri.data()->*rimp[ "previous_world_matrix" ] = *i->descriptor.matrix;
        }
      }
      i->initial_world_aabb = cur->initial_world_matrix * prim->aabb;
      if( first ) {
        aabb = i->initial_world_aabb;
        first = false;
      }
      else {
        aabb = aabb | i->initial_world_aabb;
      }
      ri.data()->*rimp[ "aabb" ] = *i->descriptor.aabb;
      ri.data()->*rimp[ "visibility" ] = *i->descriptor.visibility;
      i->descriptor.resource_index = props.graph->get_resource()->instance_resource_index->allocate( ri.data(), std::next( ri.data(), ri.size() ) );
      i->descriptor.prim = p;
      cur->inst.push_back( props.graph->get_resource()->inst.allocate( i ) );
    }
    cur->prim_aabb = aabb;
  }
  for( const auto &c: doc_node.children ) {
    load_node( cur, doc, c );
  }
}

void gltf2::load_node(
  const fx::gltf::Document &doc
) {
  int32_t index = doc.scene;
  if( index < 0 || doc.scenes.size() <= size_t( index ) ) throw invalid_gltf( "参照されたsceneが存在しない", __FILE__, __LINE__ );
  const auto &scene = doc.scenes[ index ];
  for( const auto c: scene.nodes ) {
    load_node( props.root, doc, c );
  }
}

void to_json( nlohmann::json&, const gltf2& ) {
}

}

