#include <nlohmann/json.hpp>
#include <gct/buffer.hpp>
#include <gct/named_resource.hpp>
#include <gct/allocator.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/radix_sort_create_info.hpp>

namespace gct {

radix_sort_create_info &radix_sort_create_info::add_resource(
  const named_resource &n
) {
  resources.push_back( n );
  return *this;
}
radix_sort_create_info &radix_sort_create_info::clear_resource(
  const named_resource &n
) {
  resources.clear();
  return *this;
}
radix_sort_create_info &radix_sort_create_info::set_shader(
  const std::filesystem::path &dir
) {
  set_local_sum_shader( dir / "local_sum.comp.spv" );
  set_global_sum_shader( dir / "global_sum.comp.spv" );
  set_sort_shader( dir / "sort.comp.spv" );
  set_small_sort_shader( dir / "small_sort.comp.spv" );
  return *this;
}

void to_json( nlohmann::json &dest, const radix_sort_create_info &src ) {
  dest = nlohmann::json::object();
  dest[ "allocator_set" ] = src.allocator_set;
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
  dest[ "local_offset_name" ] = src.local_offset_name;
  dest[ "workgroup_offset_name" ] = src.workgroup_offset_name;
  dest[ "resources" ] = src.resources;
}

}

