#include <algorithm>
#include <nlohmann/json.hpp>
#include <gct/device.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/descriptor_set.hpp>
#include <gct/descriptor_set_allocate_info.hpp>
#include <gct/descriptor_set_layout.hpp>
#include <gct/descriptor_set_layout_create_info.hpp>

namespace gct {
  descriptor_set_t::descriptor_set_t(
    const std::shared_ptr< descriptor_pool_t > &pool,
    const descriptor_set_allocate_info_t &create_info
  ) :
    created_from< descriptor_pool_t >( pool ),
    props( create_info ) {
    auto basic = props.get_basic();
    basic
      .setDescriptorPool( **pool )
      .setDescriptorSetCount( 1 );
    props
      .set_basic( std::move( basic ) )
      .rebuild_chain();
    handle = std::move( (*pool->get_factory())->allocateDescriptorSetsUnique(
      props.get_basic()
    )[ 0 ] );
  }
  void descriptor_set_t::update(
    const std::vector< write_descriptor_set_t > &updates_,
    bool ignore_unused
  ) {
    auto updates = updates_;
    std::vector< vk::WriteDescriptorSet > unwrapped;
    for( auto &v: updates ) {
      const auto &name = v.get_name();
      if( !name.empty() ) {
        if( ignore_unused ) {
          if( has( name ) ) {
            v.set_basic( (*this)[ name ] );
          }
          else {
            continue;
          }
        }
        else {
          v.set_basic( (*this)[ name ] );
        }
      }
      else if( v.get_id() ) {
        if( ignore_unused ) {
          if( has( *v.get_id() ) ) {
            v.set_basic( (*this)[ *v.get_id() ] );
          }
          else {
            continue;
          }
        }
        else {
          v.set_basic( (*this)[ *v.get_id() ] );
        }
      }
      v.rebuild_chain();
      auto basic = v.get_basic();
      basic.setDstArrayElement( v.get_index() );
      basic.setDstSet( *handle );
      unwrapped.push_back( basic );
    }
    (*get_factory()->get_factory())->updateDescriptorSets(
      unwrapped,
      nullptr
    );
  }
  const std::unordered_map< std::string, std::uint32_t > &descriptor_set_t::get_name_to_binding() const {
    const auto & layouts = get_props().get_layout();
    if( layouts.empty() )
      throw exception::logic_error( "Descriptor set layout is empty", __FILE__, __LINE__ );
    if( !layouts[ 0 ] )
      throw exception::logic_error( "Descriptor set layout is empty", __FILE__, __LINE__ );
    return layouts[ 0 ]->get_props().get_name_to_binding();
  }
  const std::vector< vk::DescriptorSetLayoutBinding > &descriptor_set_t::get_binding() const {
    const auto & layouts = get_props().get_layout();
    if( layouts.empty() )
      throw exception::logic_error( "Descriptor set layout is empty", __FILE__, __LINE__ );
    if( !layouts[ 0 ] )
      throw exception::logic_error( "Descriptor set layout is empty", __FILE__, __LINE__ );
    return layouts[ 0 ]->get_props().get_binding();
  }
  bool descriptor_set_t::has( const std::string &name ) const {
    const auto &name_to_binding = get_name_to_binding();
    const auto found = name_to_binding.find( name );
    return found != name_to_binding.end();
  }
  bool descriptor_set_t::has( std::uint32_t index ) const {
    vk::DescriptorSetLayoutBinding key;
    key.setBinding( index );
    const auto &binding = get_binding();
    auto existing = std::lower_bound(
      binding.begin(),
      binding.end(),
      key,
      []( const auto &l, const auto &r ) {
        return l.binding < r.binding;
      }
    );
    if( existing == binding.end() || existing->binding != key.binding ) return false;
    return true;
  }
  vk::WriteDescriptorSet descriptor_set_t::operator[]( const std::string &name ) const {
    const auto &name_to_binding = get_name_to_binding();
    const auto found = name_to_binding.find( name );
    if( found == name_to_binding.end() )
      throw exception::invalid_argument( "No such name in the descriptor set", __FILE__, __LINE__ );
    vk::DescriptorSetLayoutBinding key;
    key.setBinding( found->second );
    const auto &binding = get_binding();
    auto existing = std::lower_bound(
      binding.begin(),
      binding.end(),
      key,
      []( const auto &l, const auto &r ) {
        return l.binding < r.binding;
      }
    );
    if( existing == binding.end() || existing->binding != key.binding )
      throw exception::invalid_argument( "No such name in the descriptor set", __FILE__, __LINE__ );
    return vk::WriteDescriptorSet()
      .setDstBinding( existing->binding )
      .setDescriptorType( existing->descriptorType );
  }
  vk::WriteDescriptorSet descriptor_set_t::operator[]( std::uint32_t index ) const {
    vk::DescriptorSetLayoutBinding key;
    key.setBinding( index );
    const auto &binding = get_binding();
    auto existing = std::lower_bound(
      binding.begin(),
      binding.end(),
      key,
      []( const auto &l, const auto &r ) {
        return l.binding < r.binding;
      }
    );
    if( existing == binding.end() || existing->binding != key.binding )
      throw exception::invalid_argument( "No such name in the descriptor set", __FILE__, __LINE__ );
    return vk::WriteDescriptorSet()
      .setDstBinding( existing->binding )
      .setDescriptorType( existing->descriptorType );
  }
  void to_json( nlohmann::json &dest, const descriptor_set_t &src ) {
    dest = nlohmann::json::object();
    dest[ "props" ] = src.get_props();
  }
}

