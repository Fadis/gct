#ifndef GCT_IMAGE_IO_CREATE_INFO_HPP
#define GCT_IMAGE_IO_CREATE_INFO_HPP

#include "gct/exception.hpp"
#include <memory>
#include <optional>
#include <unordered_set>
#include <unordered_map>
#include <nlohmann/json_fwd.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <gct/exception.hpp>
#include <gct/setter.hpp>
#include <gct/image_pool.hpp>
#include <gct/sampler_pool.hpp>
#include <gct/texture_pool.hpp>
#include <gct/image_allocate_info.hpp>
#include <gct/spv_member_pointer.hpp>
#include <gct/image_io_dimension.hpp>
#include <gct/dynamic_size_image_allocate_info.hpp>

namespace gct {

class compute;
class image_view_t;

namespace scene_graph {
  class scene_graph_resource;
}

struct image_io_plan {
  LIBGCT_SETTER( input )
  LIBGCT_SETTER( output )
  LIBGCT_SETTER( inout )
  LIBGCT_SETTER( node_name )
  image_io_plan &add_input(
    const std::string &name
  ) {
    input.insert( name );
    return *this;
  }
  image_io_plan &add_output(
    const std::string &name,
    const image_allocate_info &desc
  ) {
    auto basic = desc.create_info.get_basic();
    basic.setUsage(
      basic.usage |
      vk::ImageUsageFlagBits::eTransferSrc |
      vk::ImageUsageFlagBits::eTransferDst |
      vk::ImageUsageFlagBits::eStorage|
      vk::ImageUsageFlagBits::eSampled
    );
    auto desc_ = desc;
    desc_.create_info.set_basic( basic );
    desc_.set_layout( vk::ImageLayout::eGeneral );
    output.insert( std::make_pair( name, desc_ ) );
    return *this;
  }
  image_io_plan &add_output(
    const std::string &name,
    const dynamic_size_image_allocate_info &desc
  ) {
    auto basic = desc.allocate_info.create_info.get_basic();
    basic.setUsage(
      basic.usage |
      vk::ImageUsageFlagBits::eTransferSrc |
      vk::ImageUsageFlagBits::eTransferDst |
      vk::ImageUsageFlagBits::eStorage|
      vk::ImageUsageFlagBits::eSampled
    );
    auto desc_ = desc;
    desc_.allocate_info.create_info.set_basic( basic );
    desc_.allocate_info.set_layout( vk::ImageLayout::eGeneral );
    output.insert( std::make_pair( name, desc_ ) );
    return *this;
  }
  image_io_plan &add_output(
    const std::string &name,
    unsigned int width,
    unsigned int height
  ) {
    return add_output(
      name,
      gct::image_allocate_info()
        .set_create_info(
          gct::image_create_info_t()
            .set_basic(
              gct::basic_2d_image(
                width,
                height
              )
            )
        )
    );
  }
  image_io_plan &add_output(
    const std::string &name,
    const std::string &relative_to,
    const glm::vec4 &scale
  ) {
    return add_output(
      name,
      dynamic_size_image_allocate_info()
        .set_dim(
          image_io_dimension()
            .set_relative_to( relative_to )
            .set_size_transform(
              glm::mat4(
                ( scale.x < 0 ) ? 0.0f : scale.x, 0.0f, 0.0f, ( scale.x < 0 ) ? -scale.x : 0.0f,
                0.0f, ( scale.y < 0 ) ? 0.0f : scale.y, 0.0f, ( scale.y < 0 ) ? -scale.y : 0.0f,
                0.0f, 0.0f, ( scale.z < 0 ) ? 0.0f : scale.z, ( scale.z < 0 ) ? -scale.z : 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
              )
            )
            .set_layer_transform(
              glm::mat2(
                ( scale.w < 0 ) ? 0.0f: scale.w, ( scale.w < 0 ) ? -scale.w : 0.f,
                0.f, 1.f
              )
            )
            .set_preserve_layer_count( true )
        )
    );
  }
  image_io_plan &add_output(
    const std::string &name,
    const std::string &relative_to,
    glm::vec4 scale,
    vk::Format format
  ) {
    return add_output(
      name,
      dynamic_size_image_allocate_info()
        .set_dim(
          image_io_dimension()
            .set_relative_to( relative_to )
            .set_size_transform(
              glm::mat4(
                ( scale.x < 0 ) ? 0.0f : scale.x, 0.0f, 0.0f, ( scale.x < 0 ) ? -scale.x : 0.0f,
                0.0f, ( scale.y < 0 ) ? 0.0f : scale.y, 0.0f, ( scale.y < 0 ) ? -scale.y : 0.0f,
                0.0f, 0.0f, ( scale.z < 0 ) ? 0.0f : scale.z, ( scale.z < 0 ) ? -scale.z : 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
              )
            )
            .set_layer_transform(
              glm::mat2(
                ( scale.w < 0 ) ? 0.0f: scale.w, ( scale.w < 0 ) ? -scale.w : 0.f,
                0.f, 1.f
              )
            )
            .set_preserve_layer_count( true )
        )
        .set_allocate_info(
          image_allocate_info()
            .set_create_info(
              image_create_info_t()
                .set_basic(
                  vk::ImageCreateInfo()
                    .setFormat( format )
                )
            )
        )
    );
  }
  image_io_plan &add_output(
    const std::string &name,
    const std::string &relative_to,
    const glm::vec2 &scale
  ) {
    return add_output( name, relative_to, glm::vec4( scale.x, scale.x, scale.x, scale.y ) );
  }
  image_io_plan &add_output(
    const std::string &name,
    const std::string &relative_to,
    glm::vec2 scale,
    vk::Format format
  ) {
    return add_output( name, relative_to, glm::vec4( scale.x, scale.x, scale.x, scale.y ), format );
  }
  image_io_plan &add_output(
    const std::string &name,
    const std::string &relative_to,
    float scale
  ) {
    return add_output( name, relative_to, glm::vec4( scale, scale, scale, 1.0f ) );
  }
  image_io_plan &add_output(
    const std::string &name,
    const std::string &relative_to,
    float scale,
    vk::Format format
  ) {
    return add_output( name, relative_to, glm::vec4( scale, scale, scale, 1.0f ), format );
  }


  image_io_plan &add_output(
    const std::string &name,
    const image_pool::image_descriptor &desc
  ) {
    output.insert( std::make_pair( name, desc ) );
    return *this;
  }
  image_io_plan &add_inout(
    const std::string &name
  ) {
    inout.insert( name );
    return *this;
  }
  image_io_plan &add_sampled(
    const std::string &name,
    const sampler_pool::sampler_descriptor &s
  ) {
    sampled.insert( std::make_pair( name, s ) );
    return *this;
  }
  image_io_plan &set_dim(
    const image_io_dimension &d
  ) {
    dim = d;
    return *this;
  }
  image_io_plan &set_dim(
    const std::string &name
  ) {
    return set_dim(
      gct::image_io_dimension()
        .set_relative_to( name )
    );
  }
  image_io_plan &set_dim(
    const char *name
  ) {
    return set_dim(
      gct::image_io_dimension()
        .set_relative_to( name )
    );
  }
  image_io_plan &set_dim(
    const std::string &name,
    const glm::vec4 &scale
  ) {
    return set_dim(
      gct::image_io_dimension()
        .set_relative_to( name )
        .set_size_transform(
          glm::mat4(
            ( scale.x < 0 ) ? 0.0f : scale.x, 0.0f, 0.0f, ( scale.x < 0 ) ? -scale.x : 0.0f,
            0.0f, ( scale.y < 0 ) ? 0.0f : scale.y, 0.0f, ( scale.y < 0 ) ? -scale.y : 0.0f,
            0.0f, 0.0f, ( scale.z < 0 ) ? 0.0f : scale.z, ( scale.z < 0 ) ? -scale.z : 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
          )
        )
        .set_preserve_layer_count( true )
        .set_layer_transform(
          glm::mat2(
            ( scale.w < 0 ) ? 0.0f: scale.w, ( scale.w < 0 ) ? -scale.w : 0.f,
            0.f, 1.f
          )
        )
    );
  }
  image_io_plan &set_dim(
    const char *name,
    const glm::vec4 &scale
  ) {
    return set_dim( std::string( name ), scale );
  }
  image_io_plan &set_dim(
    const std::string &name,
    float scale
  ) {
    return set_dim( name, glm::vec4( scale, scale, scale, 1.0f ) );
  }
  image_io_plan &set_dim(
    const char *name,
    float scale
  ) {
    return set_dim( name, glm::vec4( scale, scale, scale, 1.0f ) );
  }
  image_io_plan &set_dim(
    const std::string &name,
    const glm::vec2 &scale
  ) {
    return set_dim( name, glm::vec4( scale.x, scale.x, scale.x, scale.y ) );
  }
  image_io_plan &set_dim(
    const char *name,
    const glm::vec2 &scale
  ) {
    return set_dim( name, glm::vec4( scale.x, scale.x, scale.x, scale.y ) );
  }
  std::unordered_set< std::string > input;
  std::unordered_map< std::string, std::variant< image_pool::image_descriptor, image_allocate_info, dynamic_size_image_allocate_info > > output;
  std::unordered_set< std::string > inout;
  std::unordered_map< std::string, sampler_pool::sampler_descriptor > sampled;
  image_io_dimension dim;
  std::string node_name;
};

void to_json( nlohmann::json&, const image_io_plan& );

struct image_io_create_info {
  image_io_create_info(
    const std::shared_ptr< compute >&,
    const std::shared_ptr< scene_graph::scene_graph_resource > &r,
    const image_io_plan&
  );
  image_io_create_info &add_input(
    const std::string &name,
    const image_pool::image_descriptor &desc
  );
  image_io_create_info &add_output(
    const std::string &name,
    const image_pool::image_descriptor &desc
  );
  image_io_create_info &add_inout(
    const std::string &name,
    const image_pool::image_descriptor &desc
  );
  image_io_create_info &add_sampled(
    const std::string &name,
    const texture_pool::texture_descriptor &desc
  );
  [[nodiscard]] bool is_ready(
    const std::string &name
  ) const;
  image_io_create_info &add(
    const std::string &name,
    const image_pool::image_descriptor &desc
  );
  image_io_create_info &add(
    const std::string &name,
    const texture_pool::texture_descriptor &desc
  );
  [[nodiscard]] std::variant< image_pool::image_descriptor, texture_pool::texture_descriptor > get(
    const std::string &name
  ) const;
  [[nodiscard]] const std::shared_ptr< compute > &get_executable() const {
    return executable;
  }
  [[nodiscard]] const image_io_plan &get_plan() const {
    return plan;
  }
  void set_shareable( const std::string &name, bool s );
  bool is_shareable( const std::string &name ) const;
  [[nodiscard]] const std::unordered_map< std::string, image_pool::image_descriptor > get_input() const {
    return input;
  }
  /*[[nodiscard]] const std::unordered_map< std::string, std:: image_pool::image_descriptor > get_output() const {
    return output;
  }*/
  [[nodiscard]] const std::unordered_map< std::string, image_pool::image_descriptor > get_inout() const {
    return inout;
  }
  [[nodiscard]] const std::unordered_map< std::string, texture_pool::texture_descriptor > get_sampled() const {
    return sampled;
  }
  [[nodiscard]] const glm::ivec3 &get_dim() const {
    return dim;
  }
  bool independent() const;
  bool filled() const;
  [[nodiscard]] const std::vector< std::uint8_t > &get_push_constant() const {
    return push_constant;
  }
  [[nodiscard]] const std::optional< spv_member_pointer > &get_push_constant_member_pointer() const;
  template< typename T >
  image_io_create_info &set_push_constant(
    const std::string &name,
    const T &value
  ) {
    auto pcmp = get_push_constant_member_pointer();
    if( !pcmp ) {
      throw exception::runtime_error( "image_io_create_info::set_push_constant : Push constant member pointer is not available", __FILE__, __LINE__ );
    }
    if( !pcmp->has( name ) ) {
      throw exception::invalid_argument( "image_io_create_info::set_push_constant : Push constant variable " + name + " does not exist" , __FILE__, __LINE__ );
    }
    if( plan.input.find( name ) != plan.input.end() ) {
      throw exception::invalid_argument( "image_io_create_info::set_push_constant : Push constant variable " + name + " is used for input image ID" , __FILE__, __LINE__ );
    }
    if( plan.output.find( name ) != plan.output.end() ) {
      throw exception::invalid_argument( "image_io_create_info::set_push_constant : Push constant variable " + name + " is used for output image ID" , __FILE__, __LINE__ );
    }
    if( plan.inout.find( name ) != plan.inout.end() ) {
      throw exception::invalid_argument( "image_io_create_info::set_push_constant : Push constant variable " + name + " is used for inout image ID" , __FILE__, __LINE__ );
    }
    push_constant.data()->*((*pcmp)[ name ]) = value;
    return *this;
  }
private:
  void update_size(
    const std::string &name,
    const image_pool::image_descriptor &desc
  );
  void update_size(
    const std::string &name,
    const texture_pool::texture_descriptor &desc
  );
  void update_pc(
    const std::string &name,
    const image_pool::image_descriptor &desc
  );
  void update_pc(
    const std::string &name,
    const texture_pool::texture_descriptor &desc
  );
  std::shared_ptr< compute > executable;
  std::shared_ptr< scene_graph::scene_graph_resource > resource;
  image_io_plan plan;
  std::unordered_map< std::string, image_pool::image_descriptor > input;
  std::unordered_map< std::string, image_pool::image_descriptor > inout;
  std::unordered_map< std::string, texture_pool::texture_descriptor > sampled;
  glm::ivec3 dim = glm::ivec3( 1, 1, 1 );
  std::vector< std::uint8_t > push_constant;
  std::unordered_map< std::string, bool > shareable;
};

void to_json( nlohmann::json&, const image_io_create_info& );

}

#endif

