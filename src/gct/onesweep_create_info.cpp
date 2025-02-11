#include <nlohmann/json.hpp>
#include <gct/buffer.hpp>
#include <gct/named_resource.hpp>
#include <gct/allocator.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/onesweep_create_info.hpp>

namespace gct {

onesweep_create_info &onesweep_create_info::set_shader(
  const std::filesystem::path &dir
) {
  set_histgram_shader( dir / "histgram.comp.spv" );
  set_histgram_sum_shader( dir / "histgram_sum.comp.spv" );
  set_local_sum_shader( dir / "local_sum.comp.spv" );
  set_global_sum_shader( dir / "global_sum.comp.spv" );
  set_sort_shader( dir / "sort.comp.spv" );
  set_small_sort_shader( dir / "small_sort.comp.spv" );
  return *this;
}

void to_json( nlohmann::json &dest, const onesweep_create_info &src ) {
  dest = nlohmann::json::object();
  dest[ "allocator_set" ] = src.allocator_set;
  dest[ "histgram_shader" ] = src.histgram_shader;
  dest[ "histgram_sum_shader" ] = src.histgram_sum_shader;
  dest[ "local_sum_shader" ] = src.local_sum_shader;
  dest[ "global_sum_shader" ] = src.global_sum_shader;
  dest[ "sort_shader" ] = src.sort_shader;
  dest[ "small_shader" ] = src.small_sort_shader;
  if( src.input ) {
    dest[ "input" ] = *src.input;
  }
  dest[ "input_name" ] = src.input_name;
  if( src.output ) {
    dest[ "output" ] = *src.output;
  }
  dest[ "output_name" ] = src.output_name;
  dest[ "histgram_name" ] = src.histgram_name;
  dest[ "local_offset_name" ] = src.local_offset_name;
  dest[ "workgroup_offset_name" ] = src.workgroup_offset_name;
  dest[ "resources" ] = src.resources;
}

}

