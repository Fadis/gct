#include <nlohmann/json.hpp>
#include <vulkan2json/AccessFlags.hpp>
#include <vulkan2json/PipelineStageFlags.hpp>
#include <vulkan2json/DependencyFlags.hpp>
#include <gct/barrier_config.hpp>
namespace gct {

void to_json( nlohmann::json &dest, const barrier_config &src ) {
  dest = nlohmann::json::object();
  dest[ "src_access_mask" ] = src.src_access_mask;
  dest[ "dest_access_mask" ] = src.dest_access_mask;
  dest[ "src_stage" ] = src.src_stage;
  dest[ "dest_stage" ] = src.dest_stage;
  dest[ "dependency" ] = src.dependency;
}

void to_json( nlohmann::json &dest, const barrier_state &src ) {
  dest = nlohmann::json::object();
  dest[ "config" ] = src.config;
  dest[ "data" ] = src.data;
}

}
