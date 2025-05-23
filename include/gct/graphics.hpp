#ifndef GCT_CUBEMAP_GRAPHICS_HPP
#define GCT_CUBEMAP_GRAPHICS_HPP
#include <memory>
#include <vector>
#include <gct/graphics_create_info.hpp>
#include <gct/vertex_attributes.hpp>
#include <gct/property.hpp>
#include <gct/spv_member_pointer.hpp>

namespace gct {
  class allocator_t;
  class descriptor_pool_t;
  class pipeline_cache_t;
  class descriptor_set_layout_t;
  class descriptor_set_t;
  class graphics_pipeline_t;
  class command_buffer_recorder_t;
  class graphics :
    public property< graphics_create_info >,
    public std::enable_shared_from_this< graphics > {
  public:
    graphics(
      const graphics_create_info &ci
    );
    void bind(
      command_buffer_recorder_t &rec,
      unsigned int image_index
    ) const;
    void operator()(
      command_buffer_recorder_t &rec,
      unsigned int image_index,
      unsigned int x,
      unsigned int y,
      unsigned int z
    ) const;
    [[nodiscard]] const std::shared_ptr< graphics_pipeline_t > &get_pipeline() const {
      return pipeline;
    }
    [[nodiscard]] const std::unordered_map< vertex_attribute_usage_t, vertex_input_detail_t > get_vamap() const {
      return vamap;
    }
    [[nodiscard]] std::uint32_t get_stride() const {
      return stride;
    }
    [[nodiscard]] const std::shared_ptr< pipeline_layout_t > get_layout() const {
      return pipeline_layout;
    }
    [[nodiscard]] const std::optional< spv_member_pointer > &get_push_constant_member_pointer() const {
      return push_constant_mp;
    }
    [[nodiscard]] std::vector< std::uint8_t > &get_push_constant() const;
    template< typename T >
    graphics &set_push_constant(
      const std::string &name,
      const T &value
    ) {
      auto pcmp = get_push_constant_member_pointer();
      if( !pcmp ) {
        throw exception::runtime_error( "graphics::set_push_constant : Push constant member pointer is not available", __FILE__, __LINE__ );
      }
      if( !pcmp->has( name ) ) {
        throw exception::invalid_argument( "graphics::set_push_constant : Push constant variable " + name + " does not exist" , __FILE__, __LINE__ );
      }
      get_push_constant().data()->*((*pcmp)[ name ]) = value;
      return *this;
    }
    const std::vector< std::vector< std::shared_ptr< descriptor_set_t > > > &get_descriptor_set() const {
      return descriptor_set;
    }
  private:
    std::unordered_map< unsigned int, std::shared_ptr< descriptor_set_layout_t > > descriptor_set_layout;
    std::vector< std::vector< std::shared_ptr< descriptor_set_t > > > descriptor_set;
    std::shared_ptr< pipeline_layout_t > pipeline_layout;
    std::shared_ptr< graphics_pipeline_t > pipeline;
    gct::pipeline_vertex_input_state_create_info_t vistat;
    std::unordered_map< vertex_attribute_usage_t, vertex_input_detail_t > vamap;
    std::uint32_t stride;
    mutable bool use_internal_push_constant = false;
    mutable std::vector< std::uint8_t > push_constant;
    std::optional< spv_member_pointer > push_constant_mp;
  };
}

#endif


