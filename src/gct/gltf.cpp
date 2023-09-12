#include <iostream>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <gct/allocator.hpp>
#include <gct/image.hpp>
#include <gct/image_view.hpp>
#include <gct/sampler.hpp>
#include <gct/device.hpp>
#include <gct/render_pass.hpp>
#include <gct/format.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/pipeline_layout_create_info.hpp>
#include <gct/pipeline_shader_stage_create_info.hpp>
#include <gct/pipeline_input_assembly_state_create_info.hpp>
#include <gct/pipeline_viewport_state_create_info.hpp>
#include <gct/pipeline_rasterization_state_create_info.hpp>
#include <gct/pipeline_multisample_state_create_info.hpp>
#include <gct/pipeline_depth_stencil_state_create_info.hpp>
#include <gct/pipeline_color_blend_state_create_info.hpp>
#include <gct/pipeline_dynamic_state_create_info.hpp>
#include <gct//pipeline_vertex_input_state_create_info.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/graphics_pipeline_create_info.hpp>
#include <gct/graphics_pipeline.hpp>
#include <gct/shader_module.hpp>
#include <gct/gltf.hpp>
#include <gct/device.hpp>
#include <gct/queue.hpp>
#include <gct/command_pool.hpp>
#include <gct/command_buffer.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/get_device.hpp>

namespace gct::gltf {
  vk::Filter to_vulkan_mag_filter( fx::gltf::Sampler::MagFilter v ) {
    if( v == fx::gltf::Sampler::MagFilter::None ) return vk::Filter::eNearest;
    else if( v == fx::gltf::Sampler::MagFilter::Nearest ) return vk::Filter::eNearest;
    else if( v == fx::gltf::Sampler::MagFilter::Linear ) return vk::Filter::eLinear;
    else throw invalid_gltf( "不正なmag_filter", __FILE__, __LINE__ );
  }
  vk::Filter to_vulkan_min_filter( fx::gltf::Sampler::MinFilter v ) {
    if( v == fx::gltf::Sampler::MinFilter::None ) return vk::Filter::eNearest;
    else if( v == fx::gltf::Sampler::MinFilter::Nearest ) return vk::Filter::eNearest;
    else if( v == fx::gltf::Sampler::MinFilter::Linear ) return vk::Filter::eLinear;
    else if( v == fx::gltf::Sampler::MinFilter::NearestMipMapNearest ) return vk::Filter::eNearest;
    else if( v == fx::gltf::Sampler::MinFilter::LinearMipMapNearest ) return vk::Filter::eLinear;
    else if( v == fx::gltf::Sampler::MinFilter::NearestMipMapLinear ) return vk::Filter::eNearest;
    else if( v == fx::gltf::Sampler::MinFilter::LinearMipMapLinear ) return vk::Filter::eLinear;
    else throw invalid_gltf( "不正なmin_filter", __FILE__, __LINE__ );
  }
  vk::SamplerMipmapMode to_vulkan_mipmap_mode( fx::gltf::Sampler::MinFilter v ) {
    if( v == fx::gltf::Sampler::MinFilter::None ) return vk::SamplerMipmapMode::eNearest;
    else if( v == fx::gltf::Sampler::MinFilter::Nearest ) return vk::SamplerMipmapMode::eNearest;
    else if( v == fx::gltf::Sampler::MinFilter::Linear ) return vk::SamplerMipmapMode::eNearest;
    else if( v == fx::gltf::Sampler::MinFilter::NearestMipMapNearest ) return vk::SamplerMipmapMode::eNearest;
    else if( v == fx::gltf::Sampler::MinFilter::LinearMipMapNearest ) return vk::SamplerMipmapMode::eNearest;
    else if( v == fx::gltf::Sampler::MinFilter::NearestMipMapLinear ) return vk::SamplerMipmapMode::eLinear;
    else if( v == fx::gltf::Sampler::MinFilter::LinearMipMapLinear ) return vk::SamplerMipmapMode::eLinear;
    else throw invalid_gltf( "不正なmin_filter", __FILE__, __LINE__ );
  }
  vk::SamplerAddressMode to_vulkan_address_mode( fx::gltf::Sampler::WrappingMode v ) {
    if( v == fx::gltf::Sampler::WrappingMode::ClampToEdge ) return vk::SamplerAddressMode::eClampToEdge;
    else if( v == fx::gltf::Sampler::WrappingMode::MirroredRepeat ) return vk::SamplerAddressMode::eMirroredRepeat;
    else if( v == fx::gltf::Sampler::WrappingMode::Repeat ) return vk::SamplerAddressMode::eRepeat;
    else throw invalid_gltf( "不正なaddress_mode", __FILE__, __LINE__ );
  }

  std::uint32_t to_size( fx::gltf::Accessor::ComponentType type ) {
    if( type == fx::gltf::Accessor::ComponentType::None ) return 0u;
    if( type == fx::gltf::Accessor::ComponentType::Byte ) return 1u;
    if( type == fx::gltf::Accessor::ComponentType::UnsignedByte ) return 1u;
    if( type == fx::gltf::Accessor::ComponentType::Short ) return 2u;
    if( type == fx::gltf::Accessor::ComponentType::UnsignedShort ) return 2u;
    if( type == fx::gltf::Accessor::ComponentType::UnsignedInt ) return 4u;
    if( type == fx::gltf::Accessor::ComponentType::Float ) return 4u;
    return 0u;
  }
  std::uint32_t to_size( fx::gltf::Accessor::Type type ) {
    if( type == fx::gltf::Accessor::Type::None ) return 0u;
    if( type == fx::gltf::Accessor::Type::Scalar ) return 1u;
    if( type == fx::gltf::Accessor::Type::Vec2 ) return 2u;
    if( type == fx::gltf::Accessor::Type::Vec3 ) return 3u;
    if( type == fx::gltf::Accessor::Type::Vec4 ) return 4u;
    if( type == fx::gltf::Accessor::Type::Mat2 ) return 4u;
    if( type == fx::gltf::Accessor::Type::Mat3 ) return 9u;
    if( type == fx::gltf::Accessor::Type::Mat4 ) return 16u;
    return 0u;
  }
  std::uint32_t to_size(
    fx::gltf::Accessor::ComponentType componentType,
    fx::gltf::Accessor::Type type
  ) {
    return to_size( componentType ) * to_size( type );
  }
  vk::Format to_vulkan_format(
    fx::gltf::Accessor::ComponentType componentType,
    fx::gltf::Accessor::Type type,
    bool normalize
  ) {
    if( componentType == fx::gltf::Accessor::ComponentType::Byte ) {
      if( type == fx::gltf::Accessor::Type::Scalar ) {
        if( normalize ) return vk::Format::eR8Snorm;
        else return vk::Format::eR8Sscaled;
      }
      else if( type == fx::gltf::Accessor::Type::Vec2 ) {
        if( normalize ) return vk::Format::eR8G8Snorm;
        else return vk::Format::eR8G8Sscaled;
      }
      else if( type == fx::gltf::Accessor::Type::Vec3 ) {
        if( normalize ) return vk::Format::eR8G8B8Snorm;
        else return vk::Format::eR8G8B8Sscaled;
      }
      else if( type == fx::gltf::Accessor::Type::Vec4 ) {
        if( normalize ) return vk::Format::eR8G8B8A8Snorm;
        else return vk::Format::eR8G8B8A8Sscaled;
      }
      else throw invalid_gltf( "使用できないアクセサの型", __FILE__, __LINE__ );
    }
    else if( componentType == fx::gltf::Accessor::ComponentType::UnsignedByte ) {
      if( type == fx::gltf::Accessor::Type::Scalar ) {
        if( normalize ) return vk::Format::eR8Unorm;
        else return vk::Format::eR8Uscaled;
      }
      else if( type == fx::gltf::Accessor::Type::Vec2 ) {
        if( normalize ) return vk::Format::eR8G8Unorm;
        else return vk::Format::eR8G8Uscaled;
      }
      else if( type == fx::gltf::Accessor::Type::Vec3 ) {
        if( normalize ) return vk::Format::eR8G8B8Unorm;
        else return vk::Format::eR8G8B8Uscaled;
      }
      else if( type == fx::gltf::Accessor::Type::Vec4 ) {
        if( normalize ) return vk::Format::eR8G8B8A8Unorm;
        else return vk::Format::eR8G8B8A8Uscaled;
      }
      else throw invalid_gltf( "使用できないアクセサの型", __FILE__, __LINE__ );
    }
    else if( componentType == fx::gltf::Accessor::ComponentType::Short ) {
      if( type == fx::gltf::Accessor::Type::Scalar ) {
        if( normalize ) return vk::Format::eR16Snorm;
        else return vk::Format::eR16Sscaled;
      }
      else if( type == fx::gltf::Accessor::Type::Vec2 ) {
        if( normalize ) return vk::Format::eR16G16Snorm;
        else return vk::Format::eR8G8Sscaled;
      }
      else if( type == fx::gltf::Accessor::Type::Vec3 ) {
        if( normalize ) return vk::Format::eR16G16B16Snorm;
        else return vk::Format::eR16G16B16Sscaled;
      }
      else if( type == fx::gltf::Accessor::Type::Vec4 ) {
        if( normalize ) return vk::Format::eR16G16B16A16Snorm;
        else return vk::Format::eR16G16B16A16Sscaled;
      }
      else throw invalid_gltf( "使用できないアクセサの型", __FILE__, __LINE__ );
    }
    else if( componentType == fx::gltf::Accessor::ComponentType::UnsignedShort ) {
      if( type == fx::gltf::Accessor::Type::Scalar ) {
        if( normalize ) return vk::Format::eR16Unorm;
        else return vk::Format::eR16Uscaled;
      }
      else if( type == fx::gltf::Accessor::Type::Vec2 ) {
        if( normalize ) return vk::Format::eR16G16Unorm;
        else return vk::Format::eR16G16Uscaled;
      }
      else if( type == fx::gltf::Accessor::Type::Vec3 ) {
        if( normalize ) return vk::Format::eR16G16B16Unorm;
        else return vk::Format::eR16G16B16Uscaled;
      }
      else if( type == fx::gltf::Accessor::Type::Vec4 ) {
        if( normalize ) return vk::Format::eR16G16B16A16Unorm;
        else return vk::Format::eR16G16B16A16Uscaled;
      }
      else throw invalid_gltf( "使用できないアクセサの型", __FILE__, __LINE__ );
    }
    else if( componentType == fx::gltf::Accessor::ComponentType::UnsignedInt ) {
      if( type == fx::gltf::Accessor::Type::Scalar ) {
        if( normalize ) throw invalid_gltf( "使用できないアクセサの型", __FILE__, __LINE__ );
        else return vk::Format::eR32Uint;
      }
      else throw invalid_gltf( "使用できないアクセサの型", __FILE__, __LINE__ );
    }
    else if( componentType == fx::gltf::Accessor::ComponentType::Float ) {
      if( type == fx::gltf::Accessor::Type::Scalar ) {
        return vk::Format::eR32Sfloat;
      }
      else if( type == fx::gltf::Accessor::Type::Vec2 ) {
        return vk::Format::eR32G32Sfloat;
      }
      else if( type == fx::gltf::Accessor::Type::Vec3 ) {
        return vk::Format::eR32G32B32Sfloat;
      }
      else if( type == fx::gltf::Accessor::Type::Vec4 ) {
        return vk::Format::eR32G32B32A32Sfloat;
      }
      else throw invalid_gltf( "使用できないアクセサの型", __FILE__, __LINE__ );
    }
    else throw invalid_gltf( "使用できないアクセサの型", __FILE__, __LINE__ );
  }
  vk::IndexType to_vulkan_index_type( fx::gltf::Accessor::ComponentType type ) {
    if( type == fx::gltf::Accessor::ComponentType::UnsignedShort )
      return vk::IndexType::eUint16;
    else if( type == fx::gltf::Accessor::ComponentType::UnsignedInt )
      return vk::IndexType::eUint32;
    else throw invalid_gltf( "使用できないインデックスの型", __FILE__, __LINE__ );
  }
  images_t create_image(
    const fx::gltf::Document &doc,
    command_buffer_recorder_t &command_buffer,
    const std::shared_ptr< allocator_t > &allocator,
    const std::filesystem::path cd
  ) {
    images_t images;
    unsigned int cur = 1u;
    for( const auto &image: doc.images ) {
      auto image_path = std::filesystem::path( image.uri );
      if( image_path.is_relative() ) image_path = cd / image_path;
      images.push_back( image_pair_t() );
      std::cout << "[" << cur << "/" << doc.images.size() <<  "] " << image_path.string() << " をロード中..." << std::flush;
      const vk::UniqueHandle< vk::Semaphore, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE > null_semaphore;
      const auto api_version = get_device( command_buffer ).get_api_version();
      const auto &exts = get_device( command_buffer ).get_activated_extensions();
      const bool use_format_list = api_version >= VK_API_VERSION_1_2 || exts.find( "VK_KHR_image_format_list" ) != exts.end();
      images.back().set_unorm(
        command_buffer.load_image(
          allocator,
          image_path.string(),
          vk::ImageUsageFlagBits::eSampled,
          true,
          integer_attribute_t::normalized
        )
      );
      if( !use_format_list ) {
        images.back().set_unorm_view(
          images.back().unorm->get_view( vk::ImageAspectFlagBits::eColor )
        );
        images.back().set_srgb(
          command_buffer.load_image(
            allocator,
            image_path.string(),
            vk::ImageUsageFlagBits::eSampled,
            true,
            integer_attribute_t::srgb
          )
        );
        images.back().set_srgb_view(
          images.back().srgb->get_view( vk::ImageAspectFlagBits::eColor )
        );
      }
      else {
        images.back().set_unorm_view(
          images.back().unorm->get_view(
            image_view_create_info_t()
              .set_basic(
                vk::ImageViewCreateInfo()
                  .setSubresourceRange(
                    vk::ImageSubresourceRange()
                      .setAspectMask( vk::ImageAspectFlagBits::eColor )
                      .setBaseMipLevel( 0 )
                      .setLevelCount( images.back().unorm->get_props().get_basic().mipLevels )
                      .setBaseArrayLayer( 0 )
                      .setLayerCount( images.back().unorm->get_props().get_basic().arrayLayers )
                  )
                  .setViewType( to_image_view_type( images.back().unorm->get_props().get_basic().imageType, images.back().unorm->get_props().get_basic().arrayLayers ) )
                  .setFormat( images.back().unorm->get_props().get_format_list_formats().front() )
              )
              .rebuild_chain()
          )
        );
        images.back().set_srgb_view(
          images.back().unorm->get_view(
            image_view_create_info_t()
              .set_basic(
                vk::ImageViewCreateInfo()
                  .setSubresourceRange(
                    vk::ImageSubresourceRange()
                      .setAspectMask( vk::ImageAspectFlagBits::eColor )
                      .setBaseMipLevel( 0 )
                      .setLevelCount( images.back().unorm->get_props().get_basic().mipLevels )
                      .setBaseArrayLayer( 0 )
                      .setLayerCount( images.back().unorm->get_props().get_basic().arrayLayers )
                  )
                  .setViewType( to_image_view_type( images.back().unorm->get_props().get_basic().imageType, images.back().unorm->get_props().get_basic().arrayLayers ) )
                  .setFormat( images.back().unorm->get_props().get_format_list_formats().back() )
              )
              .rebuild_chain()

          )
        );
      }
      std::cout << " OK" << std::endl;
      ++cur;
    }
    return images;
  }
  buffers_t create_buffer(
    const fx::gltf::Document &doc,
    command_buffer_recorder_t &command_buffer,
    const std::shared_ptr< allocator_t > &allocator,
    const std::filesystem::path cd,
    bool ray_trace
  ) {
    buffers_t buffers;
    unsigned int cur = 1u;
    for( const auto &buffer: doc.buffers ) {
      auto buffer_path = std::filesystem::path( buffer.uri );
      if( buffer_path.is_relative() ) buffer_path = cd / buffer_path;
      std::cout << "[" << cur << "/" << doc.buffers.size() <<  "] " << buffer_path.string() << " をロード中..." << std::flush;
      buffers.push_back(
        command_buffer.load_buffer_from_file(
          allocator,
          buffer_path.string(),
	  ray_trace ?
	  (
            vk::BufferUsageFlagBits::eVertexBuffer|vk::BufferUsageFlagBits::eIndexBuffer|
            vk::BufferUsageFlagBits::eAccelerationStructureBuildInputReadOnlyKHR|
            vk::BufferUsageFlagBits::eShaderDeviceAddress
	  ) :
	  (
            vk::BufferUsageFlagBits::eVertexBuffer|vk::BufferUsageFlagBits::eIndexBuffer
	  )
        )
      );
      std::cout << " OK" << std::endl;
      ++cur;
    }
    return buffers;
  }
  cameras_t create_camera(
    const fx::gltf::Document &doc,
    float aspect_ratio
  ) {
    cameras_t cameras;
    static const auto lhrh = glm::mat4(-1,0,0,0,0,-1,0,0,0,0,1,0,0,0,0,1);
    for( const auto &c : doc.cameras ) {
      if( c.type == fx::gltf::Camera::Type::Perspective ) {
        cameras.emplace_back(
          camera_t()
            .set_projection_matrix(
              lhrh *
              glm::perspective(
                c.perspective.yfov,
                ( aspect_ratio == 0.f ) ? c.perspective.aspectRatio : aspect_ratio,
                c.perspective.znear,
                c.perspective.zfar
              )
            )
        );
      }
      else if( c.type == fx::gltf::Camera::Type::Orthographic ) {
        cameras.emplace_back(
          camera_t()
            .set_projection_matrix(
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
    return cameras;
  }
  point_lights_t create_point_light(
    const fx::gltf::Document &doc
  ) {
    point_lights_t lights;
    if( doc.extensionsAndExtras.is_null() ) return point_lights_t();
    if( doc.extensionsAndExtras.find( "extensions" ) == doc.extensionsAndExtras.end() ) return point_lights_t();
    auto &exts = doc.extensionsAndExtras[ "extensions" ];
    if( exts.find( "KHR_lights_punctual" ) == exts.end() ) return point_lights_t();
    auto &ext = exts[ "KHR_lights_punctual" ];
    if( ext.find( "lights" ) == ext.end() ) return point_lights_t();
    for( const auto &l : ext[ "lights" ] ) {
      if( l.find( "type" ) != l.end() && l[ "type" ] == "point" ) {
        point_light_t light;
        if(
          l.find( "color" ) != l.end() &&
          l[ "color" ].is_array() &&
          l[ "color" ].size() == 3u
        ) light.set_color( glm::vec3( l[ "color" ][ 0 ], l[ "color" ][ 1 ], l[ "color" ][ 2 ] ) );
        else light.set_color( glm::vec3( 1.f, 1.f, 1.f ) );
        if(
          l.find( "intensity" ) != l.end() &&
          l[ "intensity" ].is_number()
        ) light.set_intensity( float( l[ "intensity" ] ) );
        else light.set_intensity( 5.f );
        light.set_location( glm::vec3( 0.f, 0.f, 0.f ) );
        lights.emplace_back( std::move( light ) );
      }
    }
    return lights;
  }
  std::shared_ptr< sampler_t > create_sampler(
    const fx::gltf::Document &doc,
    int32_t index,
    const std::shared_ptr< device_t > &device
  ) {
    if( index < 0 || doc.samplers.size() <= size_t( index ) ) throw invalid_gltf( "参照されたsamplerが存在しない", __FILE__, __LINE__ );
    const auto &sampler = doc.samplers[ index ];
    return device->get_sampler(
      sampler_create_info_t()
        .set_basic(
          vk::SamplerCreateInfo()
            .setMagFilter( to_vulkan_mag_filter( sampler.magFilter ) )
            .setMinFilter( to_vulkan_min_filter( sampler.minFilter ) )
            .setMipmapMode( to_vulkan_mipmap_mode( sampler.minFilter ) )
            .setAddressModeU( to_vulkan_address_mode( sampler.wrapS ) )
            .setAddressModeV( to_vulkan_address_mode( sampler.wrapT ) )
            .setAddressModeW( to_vulkan_address_mode( sampler.wrapT ) )
            .setAnisotropyEnable( false )
            .setCompareEnable( false )
            .setMipLodBias( 0.f )
            .setMinLod( 0.f )
            .setMaxLod( 32.f )
            .setBorderColor( vk::BorderColor::eFloatTransparentBlack )
            .setUnnormalizedCoordinates( false )
        )
    );
  }
  samplers_t create_sampler(
    const fx::gltf::Document &doc,
    const std::shared_ptr< device_t > &device
  ) {
    samplers_t samplers;
    for( uint32_t i = 0u; i != doc.samplers.size(); ++i ) {
      samplers.push_back(
        create_sampler(
          doc,
          i,
          device
        )
      );
    }
    return samplers;
  }
  std::shared_ptr< sampler_t > create_default_sampler(
    const std::shared_ptr< device_t > &device
  ) {
    return device->get_sampler(
      sampler_create_info_t()
        .set_basic(
          vk::SamplerCreateInfo()
            .setMagFilter( to_vulkan_mag_filter( fx::gltf::Sampler::MagFilter::Linear ) )
            .setMinFilter( to_vulkan_min_filter( fx::gltf::Sampler::MinFilter::LinearMipMapLinear ) )
            .setMipmapMode( to_vulkan_mipmap_mode( fx::gltf::Sampler::MinFilter::LinearMipMapLinear ) )
            .setAddressModeU( to_vulkan_address_mode( fx::gltf::Sampler::WrappingMode::Repeat ) )
            .setAddressModeV( to_vulkan_address_mode( fx::gltf::Sampler::WrappingMode::Repeat ) )
            .setAddressModeW( to_vulkan_address_mode( fx::gltf::Sampler::WrappingMode::Repeat ) )
            .setAnisotropyEnable( false )
            .setCompareEnable( false )
            .setMipLodBias( 0.f )
            .setMinLod( 0.f )
            .setMaxLod( 32.f )
            .setBorderColor( vk::BorderColor::eFloatTransparentBlack )
            .setUnnormalizedCoordinates( false )
        )
    );
  }
  std::shared_ptr< sampler_t > create_nomip_sampler(
    const std::shared_ptr< device_t > &device
  ) {
    return device->get_sampler(
      sampler_create_info_t()
        .set_basic(
          vk::SamplerCreateInfo()
            .setMagFilter( to_vulkan_mag_filter( fx::gltf::Sampler::MagFilter::Linear ) )
            .setMinFilter( to_vulkan_min_filter( fx::gltf::Sampler::MinFilter::LinearMipMapLinear ) )
            .setMipmapMode( to_vulkan_mipmap_mode( fx::gltf::Sampler::MinFilter::LinearMipMapLinear ) )
            .setAddressModeU( to_vulkan_address_mode( fx::gltf::Sampler::WrappingMode::Repeat ) )
            .setAddressModeV( to_vulkan_address_mode( fx::gltf::Sampler::WrappingMode::Repeat ) )
            .setAddressModeW( to_vulkan_address_mode( fx::gltf::Sampler::WrappingMode::Repeat ) )
            .setAnisotropyEnable( false )
            .setCompareEnable( false )
            .setMipLodBias( 0.f )
            .setMinLod( 0.f )
            .setMaxLod( 0.f )
            .setBorderColor( vk::BorderColor::eFloatTransparentBlack )
            .setUnnormalizedCoordinates( false )
        )
    );
  }
  texture_t create_texture(
    const fx::gltf::Document &doc,
    int32_t index,
    const images_t &images,
    const samplers_t &samplers,
    const std::shared_ptr< sampler_t > &default_sampler
  ) {
    if( index < 0 || doc.textures.size() <= size_t( index ) ) throw invalid_gltf( "参照されたtextureが存在しない", __FILE__, __LINE__ );
    const auto &texture = doc.textures[ index ];
    const std::shared_ptr< sampler_t > &sampler =
      ( texture.sampler < 0 || samplers.size() <= size_t( texture.sampler ) ) ?
      default_sampler :
      samplers[ texture.sampler ];
    if( texture.source < 0 || images.size() <= size_t( texture.source ) ) throw invalid_gltf( "参照されたimageが存在しない", __FILE__, __LINE__ );
    const auto &image = images[ texture.source ];
    texture_t texture_;
    texture_.set_unorm(
      descriptor_image_info_t()
        .set_basic(
          vk::DescriptorImageInfo()
            .setImageLayout( vk::ImageLayout::eShaderReadOnlyOptimal )
        )
        .set_image_view( image.unorm_view )
        .set_sampler( sampler )
    );
    texture_.set_srgb(
      descriptor_image_info_t()
        .set_basic(
          vk::DescriptorImageInfo()
            .setImageLayout( vk::ImageLayout::eShaderReadOnlyOptimal )
        )
        .set_image_view( image.srgb_view )
        .set_sampler( sampler )
    );
    return texture_;
  }
  texture_t create_texture(
    const std::shared_ptr< image_view_t > &image_view,
    const std::shared_ptr< sampler_t > &sampler
  ) {
    texture_t texture_;
    texture_.set_unorm(
      vk::DescriptorImageInfo()
        .setImageLayout( vk::ImageLayout::eShaderReadOnlyOptimal )
        .setImageView( **image_view )
        .setSampler( **sampler )
    );
    return texture_;
  }
  textures_t create_texture(
    const fx::gltf::Document &doc,
    const images_t &images,
    const samplers_t &samplers,
    const std::shared_ptr< sampler_t > &default_sampler
  ) {
    textures_t textures;
    for( uint32_t i = 0u; i != doc.textures.size(); ++i ) {
      textures.push_back(
        create_texture(
          doc,
          i,
          images,
          samplers,
          default_sampler
        )
      );
    }
    return textures;
  }
  std::shared_ptr< graphics_pipeline_t > create_pipeline(
    const std::shared_ptr< pipeline_cache_t > &pipeline_cache,
    const std::shared_ptr< shader_module_t > &vs,
    const std::shared_ptr< shader_module_t > &gs,
    const std::shared_ptr< shader_module_t > &fs,
    const std::shared_ptr< pipeline_layout_t > &pipeline_layout,
    const std::shared_ptr< render_pass_t > &render_pass,
    std::uint32_t subpass,
    const std::vector< vk::VertexInputBindingDescription > &vertex_input_binding,
    const std::vector< vk::VertexInputAttributeDescription > &vertex_input_attribute,
    bool cull,
    bool blend,
    bool back_side,
    bool dynamic_cull_mode
  ) {
    const auto stencil_op = vk::StencilOpState()
      .setFailOp( vk::StencilOp::eKeep )
      .setPassOp( vk::StencilOp::eKeep )
      .setCompareOp( vk::CompareOp::eAlways );

    auto vertex_input =
      pipeline_vertex_input_state_create_info_t();
    for( auto &attr: vertex_input_attribute )
      vertex_input
        .add_vertex_input_attribute_description(
          attr
        );
    for( auto &binding: vertex_input_binding )
      vertex_input
        .add_vertex_input_binding_description(
          binding
        );
    auto cbsci = pipeline_color_blend_state_create_info_t();
    const auto &attachments = render_pass->get_props().get_attachment();
    for( const auto &attachment: attachments ) {
      if( format_to_aspect( attachment.format ) == vk::ImageAspectFlagBits::eColor ) {
        cbsci
          .add_attachment(
            vk::PipelineColorBlendAttachmentState()
              .setBlendEnable( blend )
              .setColorWriteMask(
                vk::ColorComponentFlagBits::eR |
                vk::ColorComponentFlagBits::eG |
                vk::ColorComponentFlagBits::eB |
                vk::ColorComponentFlagBits::eA
              )
          );
      }
    }
    pipeline_depth_stencil_state_create_info_t dssci;
    if( std::find_if(
      attachments.begin(),
      attachments.end(),
      []( const auto &attachment ) {
        return format_to_aspect( attachment.format ) & vk::ImageAspectFlagBits::eDepth;
      }
    ) != attachments.end() ) {
      dssci
        .set_basic(
          vk::PipelineDepthStencilStateCreateInfo()
            .setDepthTestEnable( VK_TRUE )
            .setDepthWriteEnable( VK_TRUE )
            .setDepthCompareOp( vk::CompareOp::eLessOrEqual )
            .setDepthBoundsTestEnable( VK_FALSE )
            .setStencilTestEnable( VK_FALSE )
            .setFront( stencil_op )
            .setBack( stencil_op )
        );
    }
    else {
      dssci
        .set_basic(
          vk::PipelineDepthStencilStateCreateInfo()
            .setDepthTestEnable( VK_FALSE )
            .setDepthWriteEnable( VK_FALSE )
            .setDepthCompareOp( vk::CompareOp::eLessOrEqual )
            .setDepthBoundsTestEnable( VK_FALSE )
            .setStencilTestEnable( VK_FALSE )
            .setFront( stencil_op )
            .setBack( stencil_op )
        );
    }
    auto dsci = pipeline_dynamic_state_create_info_t()
      .add_dynamic_state( vk::DynamicState::eViewport )
      .add_dynamic_state( vk::DynamicState::eScissor );
    if( dynamic_cull_mode ) {
#if defined(VK_VERSION_1_3)
      dsci
        .add_dynamic_state( vk::DynamicState::eCullMode )
        .add_dynamic_state( vk::DynamicState::eDepthCompareOp );
#elif defined(VK_KHR_VULKAN_EXTENDED_DYNAMIC_EXTENSION_NAME)
      dsci
        .add_dynamic_state( vk::DynamicState::eCullModeExt )
        .add_dynamic_state( vk::DynamicState::eDepthCompareOpExt );
#endif
    }

    return pipeline_cache->get_pipeline(
      graphics_pipeline_create_info_t()
        .add_stage( vs )
        .add_stage( gs )
        .add_stage( fs )
        .set_vertex_input( vertex_input )
        .set_input_assembly(
          pipeline_input_assembly_state_create_info_t()
            .set_basic(
              vk::PipelineInputAssemblyStateCreateInfo()
                .setTopology( vk::PrimitiveTopology::eTriangleList )
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
                  cull ?
                  ( back_side ? vk::CullModeFlagBits::eFront : vk::CullModeFlagBits::eBack ) :
                  vk::CullModeFlagBits::eNone
                )
                .setFrontFace( vk::FrontFace::eCounterClockwise )
                .setDepthBiasEnable( back_side ? VK_TRUE : VK_FALSE )
                .setLineWidth( 1.0f )
            )
        )
        .set_multisample(
          pipeline_multisample_state_create_info_t()
            .set_basic(
              vk::PipelineMultisampleStateCreateInfo()
            )
        )
        .set_depth_stencil( std::move( dssci ) )
        .set_color_blend( std::move( cbsci ) )
        .set_dynamic( std::move( dsci ) )
        .set_layout( pipeline_layout )
        .set_render_pass( render_pass, subpass )
        .rebuild_chain()
    );
  }
}

