#ifndef GCT_NAMED_RESOURCE_HPP
#define GCT_NAMED_RESOURCE_HPP
#include <memory>
#include <vector>
#include <tuple>
#include <string>
#include <variant>
#include <optional>
#include <filesystem>
#include <nlohmann/json_fwd.hpp>
#include <vulkan/vulkan.hpp>

namespace gct {
  class buffer_t;
  class mappable_buffer_t;
  class image_view_t;
  class sampler_t;
  class named_resource {
  public:
    named_resource(
      const std::string &n,
      const std::shared_ptr< buffer_t > &r
    );
    named_resource(
      const std::string &n,
      std::uint32_t i,
      const std::shared_ptr< buffer_t > &r
    );
    named_resource(
      const std::string &n,
      const std::shared_ptr< mappable_buffer_t > &r
    );
    named_resource(
      const std::string &n,
      std::uint32_t i,
      const std::shared_ptr< mappable_buffer_t > &r
    );
    named_resource(
      const std::string &n,
      const std::shared_ptr< image_view_t > &r
    );
    named_resource(
      const std::string &n,
      std::uint32_t i,
      const std::shared_ptr< image_view_t > &r
    );
    named_resource(
      const std::string &n,
      const std::shared_ptr< sampler_t > &s,
      const std::shared_ptr< image_view_t > &i
    );
    named_resource(
      const std::string &n,
      std::uint32_t i,
      const std::shared_ptr< sampler_t > &s,
      const std::shared_ptr< image_view_t > &v
    );
    named_resource(
      const std::string &n,
      const std::shared_ptr< image_view_t > &i,
      vk::ImageLayout l
    );
    named_resource(
      const std::string &n,
      std::uint32_t i,
      const std::shared_ptr< image_view_t > &v,
      vk::ImageLayout l
    );
    named_resource(
      const std::string &n,
      const std::shared_ptr< sampler_t > &s,
      const std::shared_ptr< image_view_t > &v,
      vk::ImageLayout l
    );
    named_resource(
      const std::string &n,
      std::uint32_t i,
      const std::shared_ptr< sampler_t > &s,
      const std::shared_ptr< image_view_t > &v,
      vk::ImageLayout l
    );
    named_resource(
      const std::string &n,
      const std::vector< std::shared_ptr< buffer_t > > &r
    );
    named_resource(
      const std::string &n,
      std::uint32_t i,
      const std::vector< std::shared_ptr< buffer_t > > &r
    );
    named_resource(
      const std::string &n,
      const std::vector< std::shared_ptr< mappable_buffer_t > > &r
    );
    named_resource(
      const std::string &n,
      std::uint32_t i,
      const std::vector< std::shared_ptr< mappable_buffer_t > > &r
    );
    named_resource(
      const std::string &n,
      const std::vector< std::shared_ptr< image_view_t > > &r
    );
    named_resource(
      const std::string &n,
      std::uint32_t i,
      const std::vector< std::shared_ptr< image_view_t > > &r
    );
    named_resource(
      const std::string &n,
      const std::vector< std::shared_ptr< image_view_t > > &i,
      vk::ImageLayout l
    );
    named_resource(
      const std::string &n,
      std::uint32_t i,
      const std::vector< std::shared_ptr< image_view_t > > &v,
      vk::ImageLayout l
    );
    named_resource(
      const std::string &n,
      const std::shared_ptr< sampler_t > &s,
      const std::vector< std::shared_ptr< image_view_t > > &v
    );
    named_resource(
      const std::string &n,
      std::uint32_t i,
      const std::shared_ptr< sampler_t > &s,
      const std::vector< std::shared_ptr< image_view_t > > &v
    );
    named_resource(
      const std::string &n,
      const std::vector< std::tuple<
        std::shared_ptr< sampler_t >,
        std::shared_ptr< image_view_t >
      > > &r
    );
    named_resource(
      const std::string &n,
      std::uint32_t i,
      const std::vector< std::tuple<
        std::shared_ptr< sampler_t >,
        std::shared_ptr< image_view_t >
      > > &r
    );
    named_resource(
      const std::string &n,
      const std::shared_ptr< sampler_t > &s,
      const std::vector< std::shared_ptr< image_view_t > > &v,
      vk::ImageLayout l
    );
    named_resource(
      const std::string &n,
      std::uint32_t i,
      const std::shared_ptr< sampler_t > &s,
      const std::vector< std::shared_ptr< image_view_t > > &v,
      vk::ImageLayout l
    );
    named_resource(
      const std::string &n,
      const std::vector< std::tuple<
        std::shared_ptr< sampler_t >,
        std::shared_ptr< image_view_t >,
        vk::ImageLayout
      > > &r
    );
    named_resource(
      const std::string &n,
      std::uint32_t i,
      const std::vector< std::tuple<
        std::shared_ptr< sampler_t >,
        std::shared_ptr< image_view_t >,
        vk::ImageLayout
      > > &r
    );


    named_resource(
      std::uint32_t id,
      const std::shared_ptr< buffer_t > &r
    );
    named_resource(
      std::uint32_t id,
      std::uint32_t i,
      const std::shared_ptr< buffer_t > &r
    );
    named_resource(
      std::uint32_t id,
      const std::shared_ptr< mappable_buffer_t > &r
    );
    named_resource(
      std::uint32_t id,
      std::uint32_t i,
      const std::shared_ptr< mappable_buffer_t > &r
    );
    named_resource(
      std::uint32_t id,
      const std::shared_ptr< image_view_t > &r
    );
    named_resource(
      std::uint32_t id,
      std::uint32_t i,
      const std::shared_ptr< image_view_t > &r
    );
    named_resource(
      std::uint32_t id,
      const std::shared_ptr< sampler_t > &s,
      const std::shared_ptr< image_view_t > &i
    );
    named_resource(
      std::uint32_t id,
      std::uint32_t i,
      const std::shared_ptr< sampler_t > &s,
      const std::shared_ptr< image_view_t > &v
    );
    named_resource(
      std::uint32_t id,
      const std::shared_ptr< image_view_t > &i,
      vk::ImageLayout l
    );
    named_resource(
      std::uint32_t id,
      std::uint32_t i,
      const std::shared_ptr< image_view_t > &v,
      vk::ImageLayout l
    );
    named_resource(
      std::uint32_t id,
      const std::shared_ptr< sampler_t > &s,
      const std::shared_ptr< image_view_t > &v,
      vk::ImageLayout l
    );
    named_resource(
      std::uint32_t id,
      std::uint32_t i,
      const std::shared_ptr< sampler_t > &s,
      const std::shared_ptr< image_view_t > &v,
      vk::ImageLayout l
    );
    named_resource(
      std::uint32_t id,
      const std::vector< std::shared_ptr< buffer_t > > &r
    );
    named_resource(
      std::uint32_t id,
      std::uint32_t i,
      const std::vector< std::shared_ptr< buffer_t > > &r
    );
    named_resource(
      std::uint32_t id,
      const std::vector< std::shared_ptr< mappable_buffer_t > > &r
    );
    named_resource(
      std::uint32_t id,
      std::uint32_t i,
      const std::vector< std::shared_ptr< mappable_buffer_t > > &r
    );
    named_resource(
      std::uint32_t id,
      const std::vector< std::shared_ptr< image_view_t > > &r
    );
    named_resource(
      std::uint32_t id,
      std::uint32_t i,
      const std::vector< std::shared_ptr< image_view_t > > &r
    );
    named_resource(
      std::uint32_t id,
      const std::vector< std::shared_ptr< image_view_t > > &i,
      vk::ImageLayout l
    );
    named_resource(
      std::uint32_t id,
      std::uint32_t i,
      const std::vector< std::shared_ptr< image_view_t > > &v,
      vk::ImageLayout l
    );
    named_resource(
      std::uint32_t id,
      const std::shared_ptr< sampler_t > &s,
      const std::vector< std::shared_ptr< image_view_t > > &v
    );
    named_resource(
      std::uint32_t id,
      std::uint32_t i,
      const std::shared_ptr< sampler_t > &s,
      const std::vector< std::shared_ptr< image_view_t > > &v
    );
    named_resource(
      std::uint32_t id,
      const std::vector< std::tuple<
        std::shared_ptr< sampler_t >,
        std::shared_ptr< image_view_t >
      > > &r
    );
    named_resource(
      std::uint32_t id,
      std::uint32_t i,
      const std::vector< std::tuple<
        std::shared_ptr< sampler_t >,
        std::shared_ptr< image_view_t >
      > > &r
    );
    named_resource(
      std::uint32_t id,
      const std::shared_ptr< sampler_t > &s,
      const std::vector< std::shared_ptr< image_view_t > > &v,
      vk::ImageLayout l
    );
    named_resource(
      std::uint32_t id,
      std::uint32_t i,
      const std::shared_ptr< sampler_t > &s,
      const std::vector< std::shared_ptr< image_view_t > > &v,
      vk::ImageLayout l
    );
    named_resource(
      std::uint32_t id,
      const std::vector< std::tuple<
        std::shared_ptr< sampler_t >,
        std::shared_ptr< image_view_t >,
        vk::ImageLayout
      > > &r
    );
    named_resource(
      std::uint32_t id,
      std::uint32_t i,
      const std::vector< std::tuple<
        std::shared_ptr< sampler_t >,
        std::shared_ptr< image_view_t >,
        vk::ImageLayout
      > > &r
    );

    named_resource &set_name( const std::string &n );
    named_resource &set_name( std::string &&n );
    [[nodiscard]] const std::string &get_name() const { return name; }
    named_resource &set_index( std::uint32_t n );
    named_resource &set_id( std::uint32_t n );
    [[nodiscard]] std::uint32_t get_index() const { return index; }
    [[nodiscard]] std::optional< std::uint32_t > get_id() const { return id; }
    named_resource &set_resource( const std::shared_ptr< buffer_t > &r );
    named_resource &set_resource( const std::shared_ptr< mappable_buffer_t > &r );
    named_resource &set_resource( const std::shared_ptr< image_view_t > &r );
    named_resource &set_resource(
      const std::shared_ptr< sampler_t > &s,
      const std::shared_ptr< image_view_t > &i
    );
    named_resource &set_resource(
      const std::shared_ptr< image_view_t > &i,
      const vk::ImageLayout l
    );
    named_resource &set_resource(
      const std::shared_ptr< sampler_t > &s,
      const std::shared_ptr< image_view_t > &i,
      const vk::ImageLayout l
    );
    named_resource &set_resource( const std::vector< std::shared_ptr< buffer_t > > &r );
    named_resource &set_resource( const std::vector< std::shared_ptr< mappable_buffer_t > > &r );
    named_resource &set_resource( const std::vector< std::shared_ptr< image_view_t > > &r );
    named_resource &set_resource(
      const std::shared_ptr< sampler_t > &s,
      const std::vector< std::shared_ptr< image_view_t > > &i
    );
    named_resource &set_resource( const std::vector< std::tuple< std::shared_ptr< sampler_t >, std::shared_ptr< image_view_t > > > &r );
    named_resource &set_resource(
      const std::vector< std::shared_ptr< image_view_t > > &i,
      vk::ImageLayout l
    );
    named_resource &set_resource(
      const std::shared_ptr< sampler_t > &s,
      const std::vector< std::shared_ptr< image_view_t > > &i,
      vk::ImageLayout l
    );
    named_resource &set_resource( const std::vector< std::tuple< std::shared_ptr< sampler_t >, std::shared_ptr< image_view_t >, vk::ImageLayout > > &r );
    [[nodiscard]] bool is_buffer() const { return resource.index() == 0; };
    [[nodiscard]] bool is_image() const { return resource.index() == 1; };
    [[nodiscard]] bool is_combined_image() const { return resource.index() == 2; };
    [[nodiscard]] bool is_image_with_layout() const { return resource.index() == 3; };
    [[nodiscard]] bool is_combined_image_with_layout() const { return resource.index() == 4; };
    [[nodiscard]] bool is_uniform() const;
    [[nodiscard]] const std::vector< std::shared_ptr< buffer_t > > &get_buffer() const {
      return std::get< std::vector< std::shared_ptr< buffer_t > > >( resource );
    }
    [[nodiscard]] const std::vector< std::shared_ptr< image_view_t > > &get_image() const {
      return std::get< std::vector< std::shared_ptr< image_view_t > > >( resource );
    }
    [[nodiscard]] const std::vector< std::tuple< std::shared_ptr< sampler_t >, std::shared_ptr< image_view_t > > > &get_combined_image() const {
      return std::get< std::vector< std::tuple< std::shared_ptr< sampler_t >, std::shared_ptr< image_view_t > > > >( resource );
    }
    [[nodiscard]] const std::vector< std::tuple< std::shared_ptr< image_view_t >, vk::ImageLayout > > &get_image_with_layout() const {
      return std::get< std::vector< std::tuple< std::shared_ptr< image_view_t >, vk::ImageLayout > > >( resource );
    }
    [[nodiscard]] const std::vector< std::tuple< std::shared_ptr< sampler_t >, std::shared_ptr< image_view_t >, vk::ImageLayout > > &get_combined_image_with_layout() const {
      return std::get< std::vector< std::tuple< std::shared_ptr< sampler_t >, std::shared_ptr< image_view_t >, vk::ImageLayout > > >( resource );
    }
    [[nodiscard]] const std::shared_ptr< buffer_t > &get_uniform_buffer() const {
      return std::get< std::vector< std::shared_ptr< buffer_t > > >( resource )[ 0 ];
    }
    [[nodiscard]] const std::shared_ptr< image_view_t > &get_uniform_image() const {
      return std::get< std::vector< std::shared_ptr< image_view_t > > >( resource )[ 0 ];
    }
    [[nodiscard]] const std::tuple< std::shared_ptr< sampler_t >, std::shared_ptr< image_view_t > > &get_uniform_combined_image() const {
      return std::get< std::vector< std::tuple< std::shared_ptr< sampler_t >, std::shared_ptr< image_view_t > > > >( resource )[ 0 ];
    }
    [[nodiscard]] const std::tuple< std::shared_ptr< image_view_t >, vk::ImageLayout > &get_uniform_image_with_layout() const {
      return std::get< std::vector< std::tuple< std::shared_ptr< image_view_t >, vk::ImageLayout > > >( resource )[ 0 ];
    }
    [[nodiscard]] const std::tuple< std::shared_ptr< sampler_t >, std::shared_ptr< image_view_t >, vk::ImageLayout > &get_uniform_combined_image_with_layout() const {
      return std::get< std::vector< std::tuple< std::shared_ptr< sampler_t >, std::shared_ptr< image_view_t >, vk::ImageLayout > > >( resource )[ 0 ];
    }
    void to_json( nlohmann::json& ) const;
  private:
    std::string name;
    std::uint32_t index = 0u;
    std::optional< std::uint32_t > id;
    std::variant<
      std::vector< std::shared_ptr< buffer_t > >,
      std::vector< std::shared_ptr< image_view_t > >,
      std::vector< std::tuple< std::shared_ptr< sampler_t >, std::shared_ptr< image_view_t > > >,
      std::vector< std::tuple< std::shared_ptr< image_view_t >, vk::ImageLayout > >,
      std::vector< std::tuple< std::shared_ptr< sampler_t >, std::shared_ptr< image_view_t >, vk::ImageLayout > >
    > resource;
  };
  void to_json( nlohmann::json&, const named_resource& );

#define LIBGCT_NAMED_RESOURCE_SETTER( name ) \
  decltype(auto) add_resource( \
    const named_resource &n \
  ) { \
    ( name ) .push_back( n ); \
    return *this; \
  } \
  decltype(auto) clear_resource() { \
    ( name ) .clear(); \
    return *this; \
  }

}

#endif

