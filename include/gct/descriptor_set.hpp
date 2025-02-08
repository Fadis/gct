#ifndef LIBGCT_INCLUDE_GCT_DESCRIPTOR_SET_HPP
#define LIBGCT_INCLUDE_GCT_DESCRIPTOR_SET_HPP
#include <string>
#include <memory>
#include <unordered_map>
#include <vector>
#include <nlohmann/json_fwd.hpp>
#include <vulkan/vulkan.hpp>
#include <gct/created_from.hpp>
#include <gct/descriptor_set_allocate_info.hpp>
#include <gct/write_descriptor_set.hpp>
#include <gct/vulkan_handle.hpp>
#include <gct/property.hpp>

namespace gct {
  class descriptor_pool_t;
  class descriptor_set_t :
    public vulkan_handle< vk::DescriptorSet >,
    public property< descriptor_set_allocate_info_t >,
    public created_from< descriptor_pool_t > {
  public:
    descriptor_set_t(
      const std::shared_ptr< descriptor_pool_t >&,
      const descriptor_set_allocate_info_t&
    );
    void update(
      const std::vector< write_descriptor_set_t >&,
      bool ignore_unused = false
    );
    [[nodiscard]] bool has( const std::string &name ) const;
    [[nodiscard]] bool has( std::uint32_t index ) const;
    [[nodiscard]] const std::unordered_map< std::string, std::uint32_t > &get_name_to_binding() const;
    [[nodiscard]] const std::vector< vk::DescriptorSetLayoutBinding > &get_binding() const;
    [[nodiscard]] vk::WriteDescriptorSet operator[]( const std::string & ) const;
    [[nodiscard]] vk::WriteDescriptorSet operator[]( std::uint32_t ) const;
  };
  void to_json( nlohmann::json&, const descriptor_set_t& );
}

#endif
