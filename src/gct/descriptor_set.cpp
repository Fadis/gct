#include <algorithm>
#include <gct/device.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/descriptor_set.hpp>
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
    const std::vector< write_descriptor_set_t > &updates_
  ) {
    auto updates = updates_;
    std::vector< vk::WriteDescriptorSet > unwrapped;
    std::transform(
      updates.begin(),
      updates.end(),
      std::back_inserter( unwrapped ),
      [&]( auto &v ) {
        v.rebuild_chain();
        auto basic = v.get_basic();
        basic.setDstSet( *handle );
        return basic;
      }
    );
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
}

