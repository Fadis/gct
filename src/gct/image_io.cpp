#include <gct/image_io.hpp>
#include <gct/compute.hpp>
#include <gct/compute_pipeline.hpp>
#include <gct/command_buffer_recorder.hpp>
namespace gct {

image_io::image_io(
  image_io_create_info &ci
) :
  property_type( ci ) {
  if( !get_props().filled() ) {
    throw exception::invalid_argument( "image_io::image_io : At least one image is not attached", __FILE__, __LINE__ );
  }
}    
void image_io::operator()(
  command_buffer_recorder_t &rec
) const {
  rec->pushConstants(
    **get_props().get_executable()->get_pipeline()->get_props().get_layout(),
    get_props().get_executable()->get_pipeline()->get_props().get_layout()->get_props().get_push_constant_range()[ 0 ].stageFlags,
    get_props().get_executable()->get_push_constant_member_pointer()->get_offset(),
    get_props().get_push_constant().size(),
    get_props().get_push_constant().data()
  );
  const auto dim = get_props().get_dim();
  (*get_props().get_executable())( rec, 0u, dim.x, dim.y, dim.z );
}

}

