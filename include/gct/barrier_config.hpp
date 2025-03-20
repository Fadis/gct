#ifndef GCT_BARRIER_CONFIG_HPP
#define GCT_BARRIER_CONFIG_HPP

#include <vulkan/vulkan.hpp>
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>
#include <gct/syncable.hpp>

namespace gct {
struct barrier_config {
  LIBGCT_SETTER( src_access_mask )
  LIBGCT_SETTER( dest_access_mask )
  LIBGCT_SETTER( src_stage )
  LIBGCT_SETTER( dest_stage )
  LIBGCT_SETTER( dependency )
  vk::AccessFlags src_access_mask = vk::AccessFlags( 0 );
  vk::AccessFlags dest_access_mask = vk::AccessFlags( 0 );
  vk::PipelineStageFlags src_stage = vk::PipelineStageFlags( 0 );
  vk::PipelineStageFlags dest_stage = vk::PipelineStageFlags( 0 );
  vk::DependencyFlags dependency = vk::DependencyFlags( 0 );
};
void to_json( nlohmann::json &dest, const barrier_config& );

struct barrier_state {
  LIBGCT_SETTER( config )
  LIBGCT_SETTER( data )
  barrier_config config;
  syncable data;
};
void to_json( nlohmann::json &dest, const barrier_state& );

}

#endif

