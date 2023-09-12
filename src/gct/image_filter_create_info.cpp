#include <gct/allocator.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/image_view.hpp>
#include <gct/named_resource.hpp>
#include <gct/image_filter_create_info.hpp>

namespace gct {

image_filter_create_info &image_filter_create_info::add_resource(
  const named_resource &n
) {
  resources.push_back( n );
  return *this;
}
image_filter_create_info &image_filter_create_info::clear_resource(
  const named_resource &n
) {
  resources.clear();
  return *this;
}

}

