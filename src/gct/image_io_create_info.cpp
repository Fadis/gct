#include <iostream>
#include <nlohmann/json.hpp>
#include <gct/image_io_create_info.hpp>
#include <gct/compute.hpp>
#include <gct/graphics.hpp>
#include <gct/image_pool.hpp>
#include <gct/exception.hpp>
#include <gct/scene_graph_resource.hpp>
#include <gct/image_view.hpp>
#include <gct/image.hpp>
#include <gct/color_attachment_name.hpp>
#include <gct/graphics_execution_shape.hpp>

namespace gct {

image_io_plan &image_io_plan::set_dim(
  const graphics_execution_shape &s
) {
  set_dim(
    image_io_dimension()
      .set_size_transform(
        glm::mat4x4(
          0.f, 0.f, 0.f, s.x,
          0.f, 0.f, 0.f, s.y,
          0.f, 0.f, 0.f, s.z,
          0.f, 0.f, 0.f, 1.f
        )
      )
      .set_preserve_layer_count( false )
  );
  set_shape( s );
  return *this;
}

void to_json( nlohmann::json &dest, const image_io_plan &src ) {
  dest = nlohmann::json::object();
  dest[ "input" ] = nlohmann::json::array();
  for( const auto &v: src.input ) {
    dest[ "input" ].push_back( v );
  }
  dest[ "output" ] = nlohmann::json::object();
  for( const auto &v: src.output ) {
    if( v.second.index() == 0u ) {
      dest[ "output" ][ v.first ] = *std::get< image_pool::image_descriptor >( v.second );
    }
    else if( v.second.index() == 1u ) {
      dest[ "output" ][ v.first ] = std::get< image_allocate_info >( v.second );
    }
    else if( v.second.index() == 2u ) {
      dest[ "output" ][ v.first ] = std::get< dynamic_size_image_allocate_info >( v.second );
    }
  }
  dest[ "inout" ] = nlohmann::json::array();
  for( const auto &v: src.inout ) {
    dest[ "inout" ].push_back( v );
  }
  dest[ "sampled" ] = nlohmann::json::object();
  for( const auto &v: src.sampled ) {
    if( v.second ) {
      dest[ "sampled" ][ v.first ] = *v.second;
    }
    else {
      dest[ "sampled" ][ v.first ] = nullptr;
    }
  }
  dest[ "dim" ] = src.dim;
  if( src.shape ) {
    dest[ "shape" ] = *src.shape;
  }
}

image_io_create_info::image_io_create_info(
  const std::shared_ptr< compute > &e,
  const std::shared_ptr< scene_graph::scene_graph_resource > &r,
  const image_io_plan &p
) : executable( e ), resource( r ), plan( p ) {
  if( !e ) {
    throw exception::invalid_argument( "image_io_create_info::image_io_create_info : The shader is null.", __FILE__, __LINE__ );
  }
  if( !r ) {
    throw exception::invalid_argument( "image_io_create_info::image_io_create_info : The scene graph resource is null.", __FILE__, __LINE__ );
  }
  const auto pcmp = executable->get_push_constant_member_pointer();
  if( !pcmp ) {
    std::cout << executable->get_props().shader << std::endl;
    throw exception::invalid_argument( "image_io_create_info::image_io_create_info : Push constants reflection is not available", __FILE__, __LINE__ );
  }
  std::unordered_set< std::string > duplicated_name;
  for( const auto &v: plan.input ) {
    if( duplicated_name.find( v ) != duplicated_name.end() ) {
      throw exception::invalid_argument( "image_io_create_info::image_io_create_info : Push constant value " + v + " is used multiple times", __FILE__, __LINE__ );
    }
    duplicated_name.insert( v );
    if( !pcmp->has( v ) ) {
      throw exception::invalid_argument( "image_io_create_info::image_io_create_info : The shader doesn't have push constant value " + v, __FILE__, __LINE__ );
    }
  }
  for( const auto &v: plan.output ) {
    if( duplicated_name.find( v.first ) != duplicated_name.end() ) {
      throw exception::invalid_argument( "image_io_create_info::image_io_create_info : Push constant value " + v.first + " is used multiple times", __FILE__, __LINE__ );
    }
    duplicated_name.insert( v.first );
    if( !pcmp->has( v.first ) ) {
      throw exception::invalid_argument( "image_io_create_info::image_io_create_info : The shader doesn't have push constant value " + v.first, __FILE__, __LINE__ );
    }
  }
  for( const auto &v: plan.inout ) {
    if( duplicated_name.find( v ) != duplicated_name.end() ) {
      throw exception::invalid_argument( "image_io_create_info::image_io_create_info : Push constant value " + v + " is used multiple times", __FILE__, __LINE__ );
    }
    duplicated_name.insert( v );
    if( !pcmp->has( v ) ) {
      throw exception::invalid_argument( "image_io_create_info::image_io_create_info : The shader doesn't have push constant value " + v, __FILE__, __LINE__ );
    }
  }
  push_constant.resize( pcmp->get_aligned_size() );
  bool size_specified = false;
  if( plan.dim.relative_to ) {
    bool found = false;
    if( plan.input.find( *plan.dim.relative_to ) != plan.input.end() ) found = true;
    else if( plan.output.find( *plan.dim.relative_to ) != plan.output.end() ) found = true;
    else if( plan.inout.find( *plan.dim.relative_to ) != plan.inout.end() ) found = true;
    if( !found ) {
      throw exception::invalid_argument( "image_io_create_info::image_io_create_info : Compute dimension is relative to size of unknown image " + *plan.dim.relative_to, __FILE__, __LINE__ );
    }
    const auto e = plan.output.find( *plan.dim.relative_to );
    if( e != plan.output.end() ) {
      if( e->second.index() == 0 ) {
        update_pc( e->first, std::get< image_pool::image_descriptor >( e->second ) );
        update_size( e->first, std::get< image_pool::image_descriptor >( e->second ) );
        size_specified = true;
      }
      else if( e->second.index() == 1 ) {
        const auto &basic = std::get< image_allocate_info >( e->second ).create_info.get_basic();
        const auto image_size = basic.extent;
        const auto &range = std::get< image_allocate_info >( e->second ).range;
        if( range && range->layer_offset + range->layer_count > basic.arrayLayers ) {
          throw exception::invalid_argument( "image_io_create_info::image_io_create_info : Broken image allocate info", __FILE__, __LINE__ );
        }
        auto layer_count = range ? range->layer_count : basic.arrayLayers;
        auto size =
          ( basic.imageType == vk::ImageType::e3D ) ?
          plan.dim.size_transform * glm::vec4( image_size.width, image_size.height, image_size.depth, 1.0f ) :
          (
            ( basic.imageType == vk::ImageType::e2D ) ?
            plan.dim.size_transform * glm::vec4( image_size.width, image_size.height, layer_count, 1.0f ) :
            plan.dim.size_transform * glm::vec4( image_size.width, layer_count, 1.0f, 1.0f )
          );
        size /= size.w;
        auto layer_count_vec = ( glm::vec2( layer_count, 1.0f ) * plan.dim.layer_transform );
        layer_count = std::max( layer_count_vec.x / layer_count_vec.y, 1.0f );
        if( plan.dim.preserve_layer_count && basic.imageType == vk::ImageType::e2D ) {
          size.z = layer_count;
        }
        else if( plan.dim.preserve_layer_count && basic.imageType == vk::ImageType::e1D ) {
          size.y = layer_count;
        }
        dim = glm::ivec3( std::max( 1.0f, size.x ), std::max( 1.0f, size.y ), std::max( 1.0f, size.z ) );
        size_specified = true;
      }
    }
  }
  if( !size_specified ) {
    auto size = plan.dim.size_transform * glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f );
    size /= size.w;
    dim = glm::ivec3( std::max( 1.0f, size.x ), std::max( 1.0f, size.y ), std::max( 1.0f, size.z ) );
  }
}

#if defined(VK_VERSION_1_3) || defined(VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME)
image_io_create_info::image_io_create_info(
  const std::shared_ptr< graphics > &e,
  const std::shared_ptr< scene_graph::scene_graph_resource > &r,
  const image_io_plan &p
) : graphic_executable( e ), resource( r ), plan( p ) {
  if( !e ) {
    throw exception::invalid_argument( "image_io_create_info::image_io_create_info : The shader is null.", __FILE__, __LINE__ );
  }
  if( !r ) {
    throw exception::invalid_argument( "image_io_create_info::image_io_create_info : The scene graph resource is null.", __FILE__, __LINE__ );
  }
  const auto pcmp = graphic_executable->get_push_constant_member_pointer();
  ca = graphic_executable->get_color_attachment();
  rendering_info =
    rendering_info_t()
      .set_basic(
#ifdef VK_VERSION_1_3
        vk::RenderingInfo()
#else
        vk::RenderingInfoKHR()
#endif
      );
  if( !pcmp ) {
    throw exception::invalid_argument( "image_io_create_info::image_io_create_info : Push constants reflection is not available", __FILE__, __LINE__ );
  }
  std::unordered_set< std::string > duplicated_name;
  for( const auto &v: plan.input ) {
    if( duplicated_name.find( v ) != duplicated_name.end() ) {
      throw exception::invalid_argument( "image_io_create_info::image_io_create_info : Push constant value " + v + " is used multiple times", __FILE__, __LINE__ );
    }
    duplicated_name.insert( v );
    if( !pcmp->has( v ) ) {
      throw exception::invalid_argument( "image_io_create_info::image_io_create_info : The shader doesn't have push constant value " + v, __FILE__, __LINE__ );
    }
  }
  for( const auto &v: plan.output ) {
    if( duplicated_name.find( v.first ) != duplicated_name.end() ) {
      throw exception::invalid_argument( "image_io_create_info::image_io_create_info : Push constant value " + v.first + " is used multiple times", __FILE__, __LINE__ );
    }
    duplicated_name.insert( v.first );
    if( !pcmp->has( v.first ) ) {
      const auto cai = std::find_if( ca.begin(), ca.end(), [name=v.first]( const auto &v ) { return v.name == name; } );
      if( cai == ca.end() ) {
        if( !( v.first == "depth" || v.first == "stencil" ) ) {
          throw exception::invalid_argument( "image_io_create_info::image_io_create_info : The shader doesn't have push constant value " + v.first, __FILE__, __LINE__ );
        }
      }
    }
  }
  for( const auto &v: plan.inout ) {
    if( duplicated_name.find( v ) != duplicated_name.end() ) {
      throw exception::invalid_argument( "image_io_create_info::image_io_create_info : Push constant value " + v + " is used multiple times", __FILE__, __LINE__ );
    }
    duplicated_name.insert( v );
    if( !pcmp->has( v ) ) {
      const auto cai = std::find_if( ca.begin(), ca.end(), [name=v]( const auto &v ) { return v.name == name; } );
      if( cai == ca.end() ) {
        if( !( v == "depth" || v == "stencil" ) ) {
          throw exception::invalid_argument( "image_io_create_info::image_io_create_info : The shader doesn't have push constant value " + v, __FILE__, __LINE__ );
        }
      }
    }
  }
  push_constant.resize( pcmp->get_aligned_size() );
  auto size = plan.dim.size_transform * glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f );
  size /= size.w;
  dim = glm::ivec3( std::max( 1.0f, size.x ), std::max( 1.0f, size.y ), std::max( 1.0f, size.z ) );
  if( plan.shape ) {
    if( pcmp->has( "offset" ) ) {
      push_constant.data()->*((*pcmp)[ "offset" ]) = plan.shape->offset;
    }
    else if( pcmp->has( "instance" ) ) {
      push_constant.data()->*((*pcmp)[ "instance" ]) = plan.shape->offset;
    }
    if( pcmp->has( "couunt" ) ) {
      push_constant.data()->*((*pcmp)[ "count" ]) = plan.shape->count;
    }
    else if( pcmp->has( "primitive" ) ) {
      push_constant.data()->*((*pcmp)[ "primitive" ]) = plan.shape->count;
    }
  }
}
#endif

void image_io_create_info::update_size(
  const std::string &name,
  const image_pool::image_descriptor &desc
) {
  if( plan.dim.relative_to && name == *plan.dim.relative_to ) {
    if( !resource->image ) {
      throw exception::invalid_argument( "image_io_create_info::update_size : Broken scene graph resource", __FILE__, __LINE__ );
    }
    const auto view = resource->image->get( desc );
    const auto &basic = view->get_factory()->get_props().get_basic();
    const auto image_size = basic.extent;
    const auto &range = view->get_props().get_basic().subresourceRange;
    auto layer_count = range.layerCount;
    auto size =
      ( basic.imageType == vk::ImageType::e3D ) ?
      plan.dim.size_transform * glm::vec4( image_size.width, image_size.height, image_size.depth, 1.0f ) :
      (
        ( basic.imageType == vk::ImageType::e2D ) ?
        plan.dim.size_transform * glm::vec4( image_size.width, image_size.height, layer_count, 1.0f ) :
        plan.dim.size_transform * glm::vec4( image_size.width, layer_count, 1.0f, 1.0f )
      );
    size /= size.w;
    auto layer_count_vec = ( glm::vec2( layer_count, 1.0f ) * plan.dim.layer_transform );
    layer_count = std::max( layer_count_vec.x / layer_count_vec.y, 1.0f );
    if( plan.dim.preserve_layer_count && basic.imageType == vk::ImageType::e2D ) {
      size.z = layer_count;
    }
    else if( plan.dim.preserve_layer_count && basic.imageType == vk::ImageType::e1D ) {
      size.y = layer_count;
    }
    dim = glm::ivec3( std::max( 1.0f, size.x ), std::max( 1.0f, size.y ), std::max( 1.0f, size.z ) );
  }
}

void image_io_create_info::update_size(
  const std::string &name,
  const texture_pool::texture_descriptor &desc
) {
  if( plan.dim.relative_to && name == *plan.dim.relative_to ) {
    if( !resource->image ) {
      throw exception::invalid_argument( "image_io_create_info::update_size : Broken scene graph resource", __FILE__, __LINE__ );
    }
    const auto t = resource->texture->get( desc );
    const auto &basic = t.first->get_factory()->get_props().get_basic();
    const auto image_size = basic.extent;
    const auto &range = t.first->get_props().get_basic().subresourceRange;
    const auto layer_count = range.layerCount;
    auto size =
      ( basic.imageType == vk::ImageType::e3D ) ?
      plan.dim.size_transform * glm::vec4( image_size.width, image_size.height, image_size.depth, 1.0f ) :
      (
        ( basic.imageType == vk::ImageType::e2D ) ?
        plan.dim.size_transform * glm::vec4( image_size.width, image_size.height, layer_count, 1.0f ) :
        plan.dim.size_transform * glm::vec4( image_size.width, layer_count, 1.0f, 1.0f )
      );
    size /= size.w;
    dim = glm::ivec3( std::max( 1.0f, size.x ), std::max( 1.0f, size.y ), std::max( 1.0f, size.z ) );
  }
}

void image_io_create_info::update_pc(
  const std::string &name,
  const image_pool::image_descriptor &desc
) {
  const auto view = resource->image->get( desc );
  if( view->get_factory()->get_props().get_basic().usage & vk::ImageUsageFlagBits::eStorage ) {
    const auto pcmp =
      graphic_executable ?
      graphic_executable->get_push_constant_member_pointer() :
      executable->get_push_constant_member_pointer();
    if( pcmp->has( name ) ) {
      push_constant.data()->*(*pcmp)[ name ] = *desc;
    }
  }
}
void image_io_create_info::update_rendering_info(
  const std::string &name,
  const image_pool::image_descriptor &desc,
  bool is_inout
) {
  const auto view = resource->image->get( desc );
#if defined(VK_VERSION_1_3) || defined(VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME)
  if( view->get_factory()->get_props().get_basic().usage & vk::ImageUsageFlagBits::eDepthStencilAttachment ) {
    const auto cai = std::find_if( ca.begin(), ca.end(), [name]( const auto &v ) { return v.name == name; } );
    if( cai != ca.end() ) {
      rendering_info.add_color_attachment(
        rendering_attachment_info_t()
          .set_view( view )
          .set_basic(
#ifdef VK_VERSION_1_3
            vk::RenderingAttachmentInfo()
#else
            vk::RenderingAttachmentInfoKHR()
#endif
              .setImageLayout( vk::ImageLayout::eGeneral )
              .setLoadOp( is_inout ? vk::AttachmentLoadOp::eLoad : vk::AttachmentLoadOp::eClear )
              .setStoreOp( vk::AttachmentStoreOp::eStore )
              .setClearValue(
                vk::ClearColorValue( 0.f, 0.f, 0.f, 0.f )
              )
          )
      );
    }
  }
  if( view->get_factory()->get_props().get_basic().usage & vk::ImageUsageFlagBits::eDepthStencilAttachment ) {
    if( name == "depth" ) {
      rendering_info.set_depth_attachment(
        rendering_attachment_info_t()
          .set_view( view )
          .set_basic(
#ifdef VK_VERSION_1_3
            vk::RenderingAttachmentInfo()
#else
            vk::RenderingAttachmentInfoKHR()
#endif
              .setImageLayout( vk::ImageLayout::eGeneral )
              .setLoadOp( is_inout ? vk::AttachmentLoadOp::eLoad : vk::AttachmentLoadOp::eClear )
              .setStoreOp( vk::AttachmentStoreOp::eStore )
              .setClearValue(
                vk::ClearDepthStencilValue( 1.f, 0 )
              )
          )
      );
    }
    if( name == "stencil" ) {
      rendering_info.set_stencil_attachment(
        rendering_attachment_info_t()
          .set_view( view )
          .set_basic(
#ifdef VK_VERSION_1_3
            vk::RenderingAttachmentInfo()
#else
            vk::RenderingAttachmentInfoKHR()
#endif
              .setImageLayout( vk::ImageLayout::eGeneral )
              .setLoadOp( vk::AttachmentLoadOp::eClear )
              .setStoreOp( vk::AttachmentStoreOp::eStore )
              .setClearValue(
                vk::ClearDepthStencilValue( 1.f, 0 )
              )
          )
      );
    }
  }
#endif
}

void image_io_create_info::update_pc(
  const std::string &name,
  const texture_pool::texture_descriptor &desc
) {
  const auto pcmp =
    graphic_executable ?
    graphic_executable->get_push_constant_member_pointer() :
    executable->get_push_constant_member_pointer();
  push_constant.data()->*(*pcmp)[ name ] = *desc;
}

image_io_create_info &image_io_create_info::add_input(
  const std::string &name,
  const image_pool::image_descriptor &desc
) {
  if( plan.input.find( name ) == plan.input.end() ) {
    throw exception::invalid_argument( "image_io_create_info::add_input : Input image is attached to non-input image " + name, __FILE__, __LINE__ );
  }
  if( !desc ) {
    throw exception::invalid_argument( "image_io_create_info::add_input : Null image descriptor", __FILE__, __LINE__ );
  }
  input[ name ] = desc;
  update_pc( name, desc );
  update_size( name , desc );
  return *this;
}

image_io_create_info &image_io_create_info::add_output(
  const std::string &name,
  const image_pool::image_descriptor &desc
) {
  const auto ci = plan.output.find( name );
  if( ci == plan.output.end() ) {
    throw exception::invalid_argument( "image_io_create_info::add_output : Output image is attached to non-output image " + name, __FILE__, __LINE__ );
  }
  if( !desc ) {
    throw exception::invalid_argument( "image_io_create_info::add_output : Null image descriptor", __FILE__, __LINE__ );
  }
  if( ci->second.index() == 0u ) {
    throw exception::invalid_argument( "image_io_create_info::add_output : Output image " + name + " is already attached", __FILE__, __LINE__ );
  }
  const auto view = resource->image->get( desc );
  if( ci->second.index() == 1u ) {
    if( std::get< image_allocate_info >( ci->second ).create_info != view->get_factory()->get_props() ) {
      throw exception::invalid_argument( "image_io_create_info::add_output : Output image " + name + " is incompatible", __FILE__, __LINE__ );
    }
  }
  plan.output[ name ] = desc;
  update_pc( name, desc );
  update_rendering_info( name, desc, false );
  update_size( name, desc );
  return *this;
}

image_io_create_info &image_io_create_info::add_inout(
  const std::string &name,
  const image_pool::image_descriptor &desc
) {
  if( plan.inout.find( name ) == plan.inout.end() ) {
    throw exception::invalid_argument( "image_io_create_info::add_inout : Inout image is attached to non-inout image " + name, __FILE__, __LINE__ );
  }
  if( !desc ) {
    throw exception::invalid_argument( "image_io_create_info::add_inout : Null image descriptor", __FILE__, __LINE__ );
  }
  inout[ name ] = desc;
  update_pc( name, desc );
  update_rendering_info( name, desc, true );
  update_size( name, desc );
  return *this;
}
image_io_create_info &image_io_create_info::add_sampled(
  const std::string &name,
  const texture_pool::texture_descriptor &desc
) {
  if( plan.sampled.find( name ) == plan.sampled.end() ) {
    throw exception::invalid_argument( "image_io_create_info::add_sampled : Sampled texture is attached to unknown texture " + name, __FILE__, __LINE__ );
  }
  if( !desc ) {
    throw exception::invalid_argument( "image_io_create_info::add_sampled : Null texture descriptor", __FILE__, __LINE__ );
  }
  sampled[ name ] = desc;
  update_pc( name, desc );
  update_size( name, desc );
  return *this;
}

image_io_create_info &image_io_create_info::add(
  const std::string &name,
  const image_pool::image_descriptor &desc
) {
  if( plan.input.find( name ) != plan.input.end() ) {
    return add_input( name, desc );
  }
  else if( plan.inout.find( name ) != plan.inout.end() ) {
    return add_inout( name, desc );
  }
  else if( plan.output.find( name ) != plan.output.end() ) {
    return add_output( name, desc );
  }
  else {
    throw exception::invalid_argument( "image_io_create_info::add : Image name " + name + " is not expected", __FILE__, __LINE__ );
  }
}

image_io_create_info &image_io_create_info::add(
  const std::string &name,
  const texture_pool::texture_descriptor &desc
) {
  if( plan.sampled.find( name ) != plan.sampled.end() ) {
    return add_sampled( name, desc );
  }
  else {
    throw exception::invalid_argument( "image_io_create_info::add : Texture name " + name + " is not expected", __FILE__, __LINE__ );
  }
}

bool image_io_create_info::is_ready(
  const std::string &name
) const {
  if( plan.input.find( name ) != plan.input.end() ) {
    return input.find( name ) != input.end();
  }
  else if( plan.inout.find( name ) != plan.inout.end() ) {
    return inout.find( name ) != inout.end();
  }
  else if( plan.output.find( name ) != plan.output.end() ) {
    const auto match = plan.output.find( name );
    return match->second.index() == 0u;
  }
  else {
    throw exception::invalid_argument( "image_io_create_info::is_ready : Image name " + name + " is not expected", __FILE__, __LINE__ );
  }
}
std::variant< image_pool::image_descriptor, texture_pool::texture_descriptor > image_io_create_info::get(
  const std::string &name
) const {
  if( plan.input.find( name ) != plan.input.end() ) {
    const auto match = input.find( name );
    if( match == input.end() ) return image_pool::image_descriptor();
    else return match->second;
  }
  else if( plan.inout.find( name ) != plan.inout.end() ) {
    const auto match = inout.find( name );
    if( match == inout.end() ) return image_pool::image_descriptor();
    else return match->second;
  }
  else if( plan.output.find( name ) != plan.output.end() ) {
    const auto match = plan.output.find( name );
    if( match == plan.output.end() ) return image_pool::image_descriptor();
    if( match->second.index() != 0u ) return image_pool::image_descriptor();
    const auto &desc = std::get< image_pool::image_descriptor >( match->second );
    return desc;
  }
  else if( plan.sampled.find( name ) != plan.sampled.end() ) {
    const auto match = sampled.find( name );
    if( match == sampled.end() ) return image_pool::image_descriptor();
    else return match->second;
  }
  else {
    throw exception::invalid_argument( "image_io_create_info::get : Image name " + name + " is not expected", __FILE__, __LINE__ );
  }
}

bool image_io_create_info::independent() const {
  if( input.size() != plan.input.size() ) return false;
  if( inout.size() != plan.inout.size() ) return false;
  if( sampled.size() != plan.sampled.size() ) return false;
  return true;
}

bool image_io_create_info::filled() const {
  if( !independent() ) return false;
  for( const auto &v: plan.output ) {
    if( v.second.index() != 0u ) return false;
  }
  return true;
}
const std::optional< spv_member_pointer > &image_io_create_info::get_push_constant_member_pointer() const {
  return
    graphic_executable ?
    graphic_executable->get_push_constant_member_pointer() :
    executable->get_push_constant_member_pointer();
}
void image_io_create_info::set_shareable( const std::string &name, bool s ) {
  if( plan.output.find( name ) == plan.output.end() ) {
    throw exception::invalid_argument( "image_io_create_info::set_shareable : unknown output image "+name, __FILE__, __LINE__ );
  }
  shareable.insert( std::make_pair( name, s ) );
}
bool image_io_create_info::is_shareable( const std::string &name ) const {
  const auto match = shareable.find( name );
  if( match == shareable.end() ) return true;
  return match->second;
}

void to_json( nlohmann::json &dest, const image_io_create_info &src ) {
  dest = nlohmann::json::object();
  if( src.get_graphic_executable() ) {
    dest[ "executable" ] = *src.get_graphic_executable();
  }
  else if( src.get_executable() ) {
    dest[ "executable" ] = *src.get_executable();
  }
  dest[ "input" ] = nlohmann::json::object();
  for( const auto &v: src.get_input() ) {
    if( v.second ) {
      dest[ "input" ][ v.first ] = *v.second;
    }
    else {
      dest[ "input" ][ v.first ] = nullptr;
    }
  }
  dest[ "inout" ] = nlohmann::json::object();
  for( const auto &v: src.get_inout() ) {
    if( v.second ) {
      dest[ "inout" ][ v.first ] = *v.second;
    }
    else {
      dest[ "inout" ][ v.first ] = nullptr;
    }
  }
  dest[ "sampled" ] = nlohmann::json::object();
  for( const auto &v: src.get_sampled() ) {
    if( v.second ) {
      dest[ "sampled" ][ v.first ] = *v.second;
    }
    else {
      dest[ "sampled" ][ v.first ] = nullptr;
    }
  }
  dest[ "plan" ] = src.get_plan();
  dest[ "dim" ] = nlohmann::json::array();
  dest[ "dim" ].push_back( src.get_dim()[ 0 ] );
  dest[ "dim" ].push_back( src.get_dim()[ 1 ] );
  dest[ "dim" ].push_back( src.get_dim()[ 2 ] );
  dest[ "push_constant" ] = nlohmann::json::array();
  for( const auto &b : src.get_push_constant() ) {
    dest[ "push_constant" ].push_back( int( b ) );
  }
}

}

