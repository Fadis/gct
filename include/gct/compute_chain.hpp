#ifndef GCT_COMPUTE_CHAIN_HPP
#define GCT_COMPUTE_CHAIN_HPP
#include <memory>
#include <unordered_map>
#include <string>
#include <filesystem>
#include <vulkan/vulkan.hpp>
#include <nlohmann/json_fwd.hpp>
#include <gct/created_from.hpp>
#include <gct/allocator.hpp>
#include <gct/descriptor_buffer_info.hpp>
#include <gct/descriptor_image_info.hpp>

namespace gct {
  class device_t;
  class allocator_t;
  class buffer_t;
  class image_view_t;
  class descriptor_set_t;
  class compute_pipeline_t;
  class compute_chain : public created_from< device_t >, public std::enable_shared_from_this< compute_chain > {
    struct pass_resources {
      std::shared_ptr< descriptor_set_t > descriptor_set;
      std::shared_ptr< compute_pipeline_t > pipeline;
    };
  public:
    compute_chain(
      const std::shared_ptr< pipeine_cache_t >&,
      const std::shared_ptr< allocator_t >&,
      const std::filesystem::path &
    );
    const std::unordered_map< std::string, descriptor_buffer_info_t > &get_buffer() const {
      return buffer;
    }
    const std::unordered_map< std::string, descriptor_image_info_t > &get_image() const {
      return image;
    }
    const nlohmann::json &get_config() const {
      return config;
    }
    void load( command_buffer_recorder_t& ) const;
    void run( command_buffer_recorder_t& ) const;
  private:
    nlohmann::json config;
    std::unordered_map< std::string, descriptor_buffer_info_t > buffer;
    std::unordered_map< std::string, descriptor_image_info_t > image;
    std::vector < pass_resources > pass;
    bound_command_buffer_t command;
  }
  void to_json( nlohmann::json &root, const compute_chain &v );
}

#endif


