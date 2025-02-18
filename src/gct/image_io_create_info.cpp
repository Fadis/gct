#include <nlohmann/json.hpp>
#include <gct/image_io_create_info.hpp>
#include <gct/compute.hpp>
#include <gct/image_pool.hpp>
#include <gct/exception.hpp>
#include <gct/scene_graph_resource.hpp>
#include <gct/image_view.hpp>
#include <gct/image.hpp>

namespace gct {

void to_json( nlohmann::json &dest, const image_io_dimension &src ) {
  dest = nlohmann::json::object();
  if( src.relative_to ) {
    dest[ "relative_to" ] = *src.relative_to;
  }
  dest[ "size_transform" ] = nlohmann::json::array();
  dest[ "size_transform" ].push_back( src.size_transform[ 0 ][ 0 ] );
  dest[ "size_transform" ].push_back( src.size_transform[ 0 ][ 1 ] );
  dest[ "size_transform" ].push_back( src.size_transform[ 0 ][ 2 ] );
  dest[ "size_transform" ].push_back( src.size_transform[ 0 ][ 3 ] );
  dest[ "size_transform" ].push_back( src.size_transform[ 1 ][ 0 ] );
  dest[ "size_transform" ].push_back( src.size_transform[ 1 ][ 1 ] );
  dest[ "size_transform" ].push_back( src.size_transform[ 1 ][ 2 ] );
  dest[ "size_transform" ].push_back( src.size_transform[ 1 ][ 3 ] );
  dest[ "size_transform" ].push_back( src.size_transform[ 2 ][ 0 ] );
  dest[ "size_transform" ].push_back( src.size_transform[ 2 ][ 1 ] );
  dest[ "size_transform" ].push_back( src.size_transform[ 2 ][ 2 ] );
  dest[ "size_transform" ].push_back( src.size_transform[ 2 ][ 3 ] );
  dest[ "size_transform" ].push_back( src.size_transform[ 3 ][ 0 ] );
  dest[ "size_transform" ].push_back( src.size_transform[ 3 ][ 1 ] );
  dest[ "size_transform" ].push_back( src.size_transform[ 3 ][ 2 ] );
  dest[ "size_transform" ].push_back( src.size_transform[ 3 ][ 3 ] );
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
  }
  dest[ "inout" ] = nlohmann::json::array();
  for( const auto &v: src.inout ) {
    dest[ "inout" ].push_back( v );
  }
  dest[ "dim" ] = src.dim;
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
  if( plan.dim.relative_to ) {
    bool found = false;
    if( plan.input.find( *plan.dim.relative_to ) != plan.input.end() ) found = true;
    else if( plan.output.find( *plan.dim.relative_to ) != plan.output.end() ) found = true;
    else if( plan.inout.find( *plan.dim.relative_to ) != plan.inout.end() ) found = true;
    if( !found ) {
      throw exception::invalid_argument( "image_io_create_info::image_io_create_info : Compute dimension is relative to size of unknown image " + *plan.dim.relative_to, __FILE__, __LINE__ );
    }
  }
  auto size = plan.dim.size_transform * glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f );
  size /= size.w;
  dim = glm::ivec3( std::max( 1.0f, size.x ), std::max( 1.0f, size.y ), std::max( 1.0f, size.z ) );
}

void image_io_create_info::update_size(
  const std::string &name,
  const image_pool::image_descriptor &desc
) {
  if( plan.dim.relative_to && name == *plan.dim.relative_to ) {
    if( !resource->image ) {
      throw exception::invalid_argument( "image_io_create_info::add_input : Broken scene graph resource", __FILE__, __LINE__ );
    }
    const auto view = resource->image->get( desc );
    const auto image_size = view->get_factory()->get_props().get_basic().extent;
    auto size = plan.dim.size_transform * glm::vec4( image_size.width, image_size.height, image_size.depth, 1.0f );
    size /= size.w;
    dim = glm::ivec3( std::max( 1.0f, size.x ), std::max( 1.0f, size.y ), std::max( 1.0f, size.z ) );
  }
}

void image_io_create_info::update_pc(
  const std::string &name,
  const image_pool::image_descriptor &desc
) {
  const auto pcmp = executable->get_push_constant_member_pointer();
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
  if( std::get< image_allocate_info >( ci->second ).create_info != view->get_factory()->get_props() ) {
    throw exception::invalid_argument( "image_io_create_info::add_output : Output image " + name + " is incompatible", __FILE__, __LINE__ );
  }
  plan.output[ name ] = desc;
  update_pc( name, desc );
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
  update_size( name, desc );
  return *this;
}

bool image_io_create_info::filled() const {
  if( input.size() != plan.input.size() ) return false;
  for( const auto &v: plan.output ) {
    if( v.second.index() != 0u ) return false;
  }
  if( inout.size() != plan.inout.size() ) return false;
  return true;
}

void to_json( nlohmann::json &dest, const image_io_create_info &src ) {
  dest = nlohmann::json::object();
  dest[ "executable" ] = *src.get_executable();
  dest[ "input" ] = nlohmann::json::object();
  for( const auto &v: src.get_input() ) {
    dest[ "input" ][ v.first ] = *v.second;
  }
  dest[ "inout" ] = nlohmann::json::object();
  for( const auto &v: src.get_inout() ) {
    dest[ "inout" ][ v.first ] = *v.second;
  }
  dest[ "plan" ] = src.get_plan();
}

}

