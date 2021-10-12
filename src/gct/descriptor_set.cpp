#include <algorithm>
#include <gct/device.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/descriptor_set.hpp>

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
}

