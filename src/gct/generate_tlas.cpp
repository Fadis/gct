#include <iostream>
#include <unordered_set>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtx/string_cast.hpp>
#include <gct/get_extensions.hpp>
#include <gct/instance.hpp>
#include <gct/queue.hpp>
#include <gct/buffer.hpp>
#include <gct/device.hpp>
#include <gct/allocator.hpp>
#include <gct/device_create_info.hpp>
#include <gct/image_create_info.hpp>
#include <gct/wait_for_sync.hpp>
#include <gct/gltf.hpp>
#include <gct/device_address.hpp>
#include <gct/acceleration_structure.hpp>
#include <gct/acceleration_structure_geometry.hpp>
#include <gct/acceleration_structure_build_geometry_info.hpp>
#include <gct/acceleration_structure_geometry_triangles_data.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/generate_tlas.hpp>
namespace gct::gltf {
  vk::TransformMatrixKHR to_transform_matrix(
    const glm::mat4 &src
  ) {
    vk::TransformMatrixKHR temp;
    temp.matrix[ 0 ][ 0 ] = src[ 0 ][ 0 ];
    temp.matrix[ 0 ][ 1 ] = src[ 0 ][ 1 ];
    temp.matrix[ 0 ][ 2 ] = src[ 0 ][ 2 ];
    temp.matrix[ 0 ][ 3 ] = src[ 0 ][ 3 ];
    temp.matrix[ 1 ][ 0 ] = src[ 1 ][ 0 ];
    temp.matrix[ 1 ][ 1 ] = src[ 1 ][ 1 ];
    temp.matrix[ 1 ][ 2 ] = src[ 1 ][ 2 ];
    temp.matrix[ 1 ][ 3 ] = src[ 1 ][ 3 ];
    temp.matrix[ 2 ][ 0 ] = src[ 2 ][ 0 ];
    temp.matrix[ 2 ][ 1 ] = src[ 2 ][ 1 ];
    temp.matrix[ 2 ][ 2 ] = src[ 2 ][ 2 ];
    temp.matrix[ 2 ][ 3 ] = src[ 2 ][ 3 ];
    if( src[ 3 ][ 0 ] ) throw -1;
    if( src[ 3 ][ 1 ] ) throw -1;
    if( src[ 3 ][ 2 ] ) throw -1;
    if( src[ 3 ][ 3 ] != 1 ) throw -1;
    return temp;
  }

  void
  generate_tlas(
    const document_t &doc,
    command_buffer_recorder_t &rec,
    const std::shared_ptr< allocator_t > &allocator,
    const std::vector< std::shared_ptr< acceleration_structure_t > > &as,
    const std::vector< std::uint32_t > &mesh2instance,
    const node_t &node,
    std::vector< vk::AccelerationStructureInstanceKHR > &dest
  ) {
    if( node.has_mesh ) {
      auto begin = mesh2instance[ node.mesh ];
      auto end = mesh2instance[ node.mesh + 1 ];
      for( auto i = begin; i != end; ++i ) {
        vk::AccelerationStructureInstanceKHR instance;
        dest.push_back(
          vk::AccelerationStructureInstanceKHR()
            .setTransform( to_transform_matrix( node.matrix ) )
            .setInstanceCustomIndex( i )
            .setMask( 0xFF )
            .setAccelerationStructureReference( *as[ i ]->get_address() )
            .setFlags( vk::GeometryInstanceFlagBitsKHR::eTriangleFacingCullDisable )
        );
      }
    }
    for( const auto &c: node.children ) {
      generate_tlas(
        doc,
        rec,
        allocator,
        as,
        mesh2instance,
        c,
        dest
      );
    }
  }
  std::shared_ptr< acceleration_structure_t >
  generate_tlas(
    const document_t &doc,
    command_buffer_recorder_t &rec,
    const std::shared_ptr< allocator_t > &allocator,
    const std::vector< std::shared_ptr< acceleration_structure_t > > &as,
    const std::vector< std::shared_ptr< gct::buffer_t > > &blas_buffer,
    const std::vector< std::uint32_t > &mesh2instance
  ) {
    auto device = allocator->get_factory();
    std::vector< vk::AccelerationStructureInstanceKHR > tlas_data;
    generate_tlas(
      doc,
      rec,
      allocator,
      as,
      mesh2instance,
      doc.node,
      tlas_data
    );
    auto tlas_buffer = rec.load_buffer(
      allocator,
      reinterpret_cast< const void* >( tlas_data.data() ),
      sizeof( vk::AccelerationStructureInstanceKHR ) * tlas_data.size(),
      vk::BufferUsageFlagBits::eAccelerationStructureBuildInputReadOnlyKHR
    );
    rec.barrier(
      vk::AccessFlagBits::eTransferWrite,
      vk::AccessFlagBits::eAccelerationStructureReadKHR,
      vk::PipelineStageFlagBits::eTransfer,
      vk::PipelineStageFlagBits::eAccelerationStructureBuildKHR,
      vk::DependencyFlagBits( 0 ),
      { tlas_buffer },
      {}
    );
    auto asbgi = gct::acceleration_structure_build_geometry_info_t()
      .set_basic(
        vk::AccelerationStructureBuildGeometryInfoKHR()
          .setType(
            vk::AccelerationStructureTypeKHR::eTopLevel
          )
          .setFlags(
            vk::BuildAccelerationStructureFlagBitsKHR::eAllowUpdate
          )
          .setMode(
            vk::BuildAccelerationStructureModeKHR::eBuild
          )
      )
      .add_geometry(
        gct::acceleration_structure_geometry_t()
          .set_instance(
            gct::acceleration_structure_geometry_instances_data_t()
              .set_basic(
                vk::AccelerationStructureGeometryInstancesDataKHR()
                  .setArrayOfPointers( false )
              )
              .set_data( tlas_buffer->get_address() )
          )
      );
    std::vector< std::uint32_t > max_primitive_count{ std::uint32_t( tlas_data.size() ) };
    auto size = device->get_acceleration_structure_build_size(
      vk::AccelerationStructureBuildTypeKHR::eDevice,
      asbgi,
      max_primitive_count
    );
    auto scratch_buffer_addr = allocator->create_buffer(
      gct::buffer_create_info_t()
        .set_basic(
          vk::BufferCreateInfo()
            .setSize( size.get_basic().buildScratchSize )
            .setUsage(
              vk::BufferUsageFlagBits::eStorageBuffer|
              vk::BufferUsageFlagBits::eShaderDeviceAddress
            )
        ),
      VMA_MEMORY_USAGE_GPU_ONLY
    )->get_address();
    auto asb = allocator->create_buffer(
      gct::buffer_create_info_t()
        .set_basic(
          vk::BufferCreateInfo()
            .setSize( size.get_basic().accelerationStructureSize )
            .setUsage(
              vk::BufferUsageFlagBits::eAccelerationStructureStorageKHR|
              vk::BufferUsageFlagBits::eShaderDeviceAddress
            )
        ),
      VMA_MEMORY_USAGE_GPU_ONLY
    );
    auto tlas = asb->create_acceleration_structure(
        vk::AccelerationStructureTypeKHR::eTopLevel
      );
    asbgi
      .set_src( tlas )
      .set_dst( tlas )
      .set_scratch( scratch_buffer_addr )
      .rebuild_chain();
    rec.build_acceleration_structure(
      asbgi,
      std::vector< vk::AccelerationStructureBuildRangeInfoKHR >{
        vk::AccelerationStructureBuildRangeInfoKHR()
          .setPrimitiveCount( tlas_data.size() )
      }
    );
    rec.barrier(
      vk::AccessFlagBits::eAccelerationStructureWriteKHR,
      vk::AccessFlagBits::eShaderRead,
      vk::PipelineStageFlagBits::eAccelerationStructureBuildKHR,
      vk::PipelineStageFlagBits::eRayTracingShaderKHR,
      vk::DependencyFlagBits( 0 ),
      { tlas_buffer },
      {}
    );
    rec.barrier(
      vk::AccessFlagBits::eAccelerationStructureWriteKHR,
      vk::AccessFlagBits::eShaderRead,
      vk::PipelineStageFlagBits::eAccelerationStructureBuildKHR,
      vk::PipelineStageFlagBits::eRayTracingShaderKHR,
      vk::DependencyFlagBits( 0 ),
      blas_buffer,
      {}
    );
    return tlas;
  }
  std::shared_ptr< acceleration_structure_t >
  generate_tlas(
    command_buffer_recorder_t &rec,
    const std::shared_ptr< allocator_t > &allocator,
    const std::vector< std::shared_ptr< acceleration_structure_t > > &blas
  ) {
    auto device = allocator->get_factory();
    std::vector< vk::AccelerationStructureInstanceKHR > tlas_data;
    unsigned int i  = 0u;
    vk::TransformMatrixKHR transformMatrix{
      std::array< std::array< float, 4 >, 3 >{
        std::array< float, 4 >{ 1.0f, 0.0f, 0.0f, 0.0f },
        std::array< float, 4 >{ 0.0f, 1.0f, 0.0f, 0.0f },
        std::array< float, 4 >{ 0.0f, 0.0f, 1.0f, 0.0f }
      }
    };
    for( const auto &a: blas ) {
      vk::AccelerationStructureInstanceKHR instance;
      tlas_data.push_back(
        vk::AccelerationStructureInstanceKHR()
          .setTransform( transformMatrix )
          .setInstanceCustomIndex( i )
          .setMask( 0xFF )
          .setAccelerationStructureReference( *a->get_address() )
          .setFlags( vk::GeometryInstanceFlagBitsKHR::eTriangleFacingCullDisable )
      );
      ++i;
    }
    auto tlas_buffer = rec.load_buffer(
      allocator,
      reinterpret_cast< const void* >( tlas_data.data() ),
      sizeof( vk::AccelerationStructureInstanceKHR ) * tlas_data.size(),
      vk::BufferUsageFlagBits::eAccelerationStructureBuildInputReadOnlyKHR
    );
    rec.barrier(
      vk::AccessFlagBits::eTransferWrite,
      vk::AccessFlagBits::eAccelerationStructureReadKHR,
      vk::PipelineStageFlagBits::eTransfer,
      vk::PipelineStageFlagBits::eAccelerationStructureBuildKHR,
      vk::DependencyFlagBits( 0 ),
      { tlas_buffer },
      {}
    );
    auto asbgi = gct::acceleration_structure_build_geometry_info_t()
      .set_basic(
        vk::AccelerationStructureBuildGeometryInfoKHR()
          .setType(
            vk::AccelerationStructureTypeKHR::eTopLevel
          )
          .setFlags(
            vk::BuildAccelerationStructureFlagBitsKHR::ePreferFastTrace
          )
          .setMode(
            vk::BuildAccelerationStructureModeKHR::eBuild
          )
      )
      .add_geometry(
        gct::acceleration_structure_geometry_t()
          .set_instance(
            gct::acceleration_structure_geometry_instances_data_t()
              .set_basic(
                vk::AccelerationStructureGeometryInstancesDataKHR()
                  .setArrayOfPointers( false )
              )
              .set_data( tlas_buffer->get_address() )
          )
      );
    std::vector< std::uint32_t > max_primitive_count{ std::uint32_t( tlas_data.size() ) };
    auto size = device->get_acceleration_structure_build_size(
      vk::AccelerationStructureBuildTypeKHR::eDevice,
      asbgi,
      max_primitive_count
    );
    auto scratch_buffer_addr = allocator->create_buffer(
      gct::buffer_create_info_t()
        .set_basic(
          vk::BufferCreateInfo()
            .setSize( size.get_basic().buildScratchSize )
            .setUsage(
              vk::BufferUsageFlagBits::eStorageBuffer|
              vk::BufferUsageFlagBits::eShaderDeviceAddress
            )
        ),
      VMA_MEMORY_USAGE_GPU_ONLY
    )->get_address();
    auto asb = allocator->create_buffer(
      gct::buffer_create_info_t()
        .set_basic(
          vk::BufferCreateInfo()
            .setSize( size.get_basic().accelerationStructureSize )
            .setUsage(
              vk::BufferUsageFlagBits::eAccelerationStructureStorageKHR|
              vk::BufferUsageFlagBits::eShaderDeviceAddress
            )
        ),
      VMA_MEMORY_USAGE_GPU_ONLY
    );
    auto tlas = asb->create_acceleration_structure(
        vk::AccelerationStructureTypeKHR::eTopLevel
      );
    asbgi
      .set_src( tlas )
      .set_dst( tlas )
      .set_scratch( scratch_buffer_addr )
      .rebuild_chain();
    rec.build_acceleration_structure(
      asbgi,
      std::vector< vk::AccelerationStructureBuildRangeInfoKHR >{
        vk::AccelerationStructureBuildRangeInfoKHR()
          .setPrimitiveCount( tlas_data.size() )
      }
    );
    rec.barrier(
      vk::AccessFlagBits::eAccelerationStructureWriteKHR,
      vk::AccessFlagBits::eShaderRead,
      vk::PipelineStageFlagBits::eAccelerationStructureBuildKHR,
      vk::PipelineStageFlagBits::eRayTracingShaderKHR,
      vk::DependencyFlagBits( 0 ),
      { tlas_buffer },
      {}
    );
    std::vector< std::shared_ptr< buffer_t > > blas_buffer;
    std::transform(
      blas.begin(),
      blas.end(),
      std::back_inserter( blas_buffer ),
      []( const auto &v ) {
        return v->get_factory();
      }
    );
    rec.barrier(
      vk::AccessFlagBits::eAccelerationStructureWriteKHR,
      vk::AccessFlagBits::eShaderRead,
      vk::PipelineStageFlagBits::eAccelerationStructureBuildKHR,
      vk::PipelineStageFlagBits::eRayTracingShaderKHR,
      vk::DependencyFlagBits( 0 ),
      blas_buffer,
      {}
    );
    return tlas;
  }
}

