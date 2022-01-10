#include <vector>
#include <gct/command_buffer_recorder.hpp>
#include <gct/command_buffer.hpp>
#include <gct/acceleration_structure_build_geometry_info.hpp>
#include <gct/acceleration_structure_geometry.hpp>
namespace gct {
  void command_buffer_recorder_t::build_acceleration_structure(
    const std::vector< gct::acceleration_structure_build_geometry_info_t > &geometry,
    const std::vector< std::vector< vk::AccelerationStructureBuildRangeInfoKHR > > &region
  ) {
    auto geometry_ = geometry;
    std::vector< vk::AccelerationStructureBuildGeometryInfoKHR > raw_geometry;
    raw_geometry.reserve( geometry.size() );
    std::transform(
      geometry_.begin(),
      geometry_.end(),
      std::back_inserter( raw_geometry ),
      []( auto &v ) {
        v.rebuild_chain();
        return v.get_basic();
      }
    );
    std::vector< const vk::AccelerationStructureBuildRangeInfoKHR* > region_addrs;
    region_addrs.reserve( region.size() );
    std::transform(
      region.begin(),
      region.end(),
      std::back_inserter( region_addrs ),
      []( const auto &v ) {
        return v.data();
      }
    );
    (*get_factory())->buildAccelerationStructuresKHR(
      raw_geometry,
      region_addrs
    );
    get_factory()->unbound()->keep.push_back( geometry_ );
  }
  void command_buffer_recorder_t::build_acceleration_structure(
    const gct::acceleration_structure_build_geometry_info_t &geometry,
    const std::vector< vk::AccelerationStructureBuildRangeInfoKHR > &region
  ) {
    build_acceleration_structure(
      std::vector< gct::acceleration_structure_build_geometry_info_t >{
        geometry
      },
      std::vector< std::vector< vk::AccelerationStructureBuildRangeInfoKHR > >{
        region
      }
    );
  }
}
