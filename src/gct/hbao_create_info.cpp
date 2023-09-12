#include <gct/allocator.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/image_view.hpp>
#include <gct/named_resource.hpp>
#include <gct/hbao_create_info.hpp>

namespace gct {

hbao_create_info &hbao_create_info::add_resource(
  const named_resource &n
) {
  resources.push_back( n );
  return *this;
}
hbao_create_info &hbao_create_info::clear_resource(
  const named_resource &n
) {
  resources.clear();
  return *this;
}
hbao_create_info &hbao_create_info::set_shader(
  const std::filesystem::path &dir
) {
  set_ao0_shader( dir / "hbao_0.comp.spv" );
  set_ao45_shader( dir / "hbao_45.comp.spv" );
  set_ao90_shader( dir / "hbao_90.comp.spv" );
  set_ao135_shader( dir / "hbao_135.comp.spv" );
  set_ao135_shader( dir / "hbao_135.comp.spv" );
  set_sum_shader( dir / "hbao.comp.spv" );
  return *this;
}

}

