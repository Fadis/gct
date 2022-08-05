#include <gct/device.hpp>
#include <gct/descriptor_pool.hpp>

namespace gct {
  descriptor_pool_t::descriptor_pool_t(
    const std::shared_ptr< device_t > &device,
    const descriptor_pool_create_info_t &create_info
  ) :
    created_from< device_t >( device ),
    props( create_info ) {
    props.rebuild_chain();
    handle = (*device)->createDescriptorPoolUnique(
      props.get_basic()
    );
  }
  std::shared_ptr< descriptor_set_t > descriptor_pool_t::allocate(
    const descriptor_set_allocate_info_t &create_info
  ) {
    std::cout << "allocate " << nlohmann::json( create_info ).dump( 2 ) << std::endl;
    return std::shared_ptr< descriptor_set_t >(
      new descriptor_set_t(
        shared_from_this(),
        create_info
      )
    );
  }
  std::shared_ptr< descriptor_set_t > descriptor_pool_t::allocate(
    const std::shared_ptr< descriptor_set_layout_t > &layout
  ) {
    return allocate(
      gct::descriptor_set_allocate_info_t()
        .add_layout( layout )
        .rebuild_chain()
    );
  }
}

