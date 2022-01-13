#include <iostream>
#include <unordered_set>
#include <nlohmann/json.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtx/string_cast.hpp>
#include <gct/get_extensions.hpp>
#include <gct/instance.hpp>
#include <gct/queue.hpp>
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


namespace gct::gltf {
  std::shared_ptr< acceleration_structure_t >
  generate_blas(
    const std::shared_ptr< buffer_t > &vertex,
    std::uint32_t vertex_offset,
    std::uint32_t vertex_count,
    const pipeline_vertex_input_state_create_info_t &vistat_,
    command_buffer_recorder_t &rec,
    const std::shared_ptr< allocator_t > &allocator
  ) {
    auto vistat = vistat_;
    vistat.rebuild_chain();
    auto device = allocator->get_factory();
    glm::mat4x3 trans( 1.0f );
    auto ident_matrix = rec.load_buffer(
      allocator,
      reinterpret_cast< const void* >( &trans ),
      sizeof( glm::mat4x3 ),
      vk::BufferUsageFlagBits::eAccelerationStructureBuildInputReadOnlyKHR
    );
    auto ident_matrix_addr = ident_matrix->get_address();
    rec.barrier(
      vk::AccessFlagBits::eTransferWrite,
      vk::AccessFlagBits::eAccelerationStructureReadKHR,
      vk::PipelineStageFlagBits::eTransfer,
      vk::PipelineStageFlagBits::eAccelerationStructureBuildKHR,
      vk::DependencyFlagBits( 0 ),
      { ident_matrix },
      {}
    );
    auto triangles =
      gct::acceleration_structure_geometry_triangles_data_t();
    {
      const auto buffer_address = vertex->get_address();
      const auto buffer_offset = vertex_offset;
      triangles
        .set_vertex_data(
          buffer_address + buffer_offset
        );
    }
    const auto vbbegin = vistat.get_basic().pVertexBindingDescriptions;
    const auto vbend = std::next( vistat.get_basic().pVertexBindingDescriptions, vistat.get_basic().vertexBindingDescriptionCount );
    const auto vabegin = vistat.get_basic().pVertexAttributeDescriptions;
    const auto vaend = std::next( vistat.get_basic().pVertexAttributeDescriptions, vistat.get_basic().vertexAttributeDescriptionCount );
    const auto attribute = std::find_if(
      vabegin,
      vaend,
      []( const auto &v ) {
        return v.binding == 0u;
      }
    );
    if( attribute == vaend ) {
      std::cout << "oops1" << std::endl;
      abort();
    }
    const auto binding = std::find_if(
      vbbegin,
      vbend,
      []( const auto &v ) {
        return v.binding == 0u;
      }
    );
    if( binding == vbend ) {
      std::cout << "oops2" << std::endl;
      abort();
    }
    auto asbgi = gct::acceleration_structure_build_geometry_info_t()
      .set_basic(
        vk::AccelerationStructureBuildGeometryInfoKHR()
          .setType(
            vk::AccelerationStructureTypeKHR::eBottomLevel
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
          .set_triangle(
            triangles
              .set_basic(
                vk::AccelerationStructureGeometryTrianglesDataKHR()
                  .setIndexType( vk::IndexType::eNoneKHR )
                  .setVertexFormat( attribute->format )
                  .setVertexStride( binding->stride )
                  .setMaxVertex( vertex_count )
              )
              .set_transform_data(
                ident_matrix_addr
              )
          )
      );
    std::vector< std::uint32_t > max_primitive_count{ vertex_count / 3u };
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
    auto as = asb->create_acceleration_structure(
        vk::AccelerationStructureTypeKHR::eBottomLevel
      );
    asbgi
      .set_src( as )
      .set_dst( as )
      .set_scratch( scratch_buffer_addr )
      .rebuild_chain();
    rec.build_acceleration_structure(
      asbgi,
      std::vector< vk::AccelerationStructureBuildRangeInfoKHR >{
        vk::AccelerationStructureBuildRangeInfoKHR()
          .setPrimitiveCount( vertex_count / 3u )
      }
    );
    return as;
  }
  std::tuple<
    std::vector< std::shared_ptr< acceleration_structure_t > >,
    std::vector< std::shared_ptr< gct::buffer_t > >,
    std::vector< std::uint32_t >
  >
  generate_blas(
    const document_t &doc,
    command_buffer_recorder_t &rec,
    const std::shared_ptr< allocator_t > &allocator
  ) {
    auto device = allocator->get_factory();
    std::vector< std::shared_ptr< gct::acceleration_structure_t > > as;
    std::vector< std::shared_ptr< gct::buffer_t > > as_buf;
    std::vector< std::uint32_t > mesh2prim;
    glm::mat4x3 trans( 1.0f );
    vk::TransformMatrixKHR transformMatrix{
      std::array< std::array< float, 4 >, 3 >{
        std::array< float, 4 >{ 1.0f, 0.0f, 0.0f, 0.0f },
        std::array< float, 4 >{ 0.0f, 1.0f, 0.0f, 0.0f },
        std::array< float, 4 >{ 0.0f, 0.0f, 1.0f, 0.0f }
      }
    };
    auto ident_matrix = rec.load_buffer(
      allocator,
      reinterpret_cast< const void* >( &transformMatrix ),
      sizeof( vk::TransformMatrixKHR ),
      vk::BufferUsageFlagBits::eAccelerationStructureBuildInputReadOnlyKHR
    );
    auto ident_matrix_addr = ident_matrix->get_address();
    rec.barrier(
      vk::AccessFlagBits::eTransferWrite,
      vk::AccessFlagBits::eAccelerationStructureReadKHR,
      vk::PipelineStageFlagBits::eTransfer,
      vk::PipelineStageFlagBits::eAccelerationStructureBuildKHR,
      vk::DependencyFlagBits( 0 ),
      { ident_matrix },
      {}
    );
    for( const auto &m: doc.mesh ) {
      mesh2prim.push_back( as.size() );
      for( const auto &p: m.primitive ) {
        auto triangles =
          gct::acceleration_structure_geometry_triangles_data_t();
        if( p.indexed ) {
          const auto buffer_address = doc.buffer[ p.index_buffer.index ]->get_address();
          const auto buffer_offset = p.index_buffer.offset;
          triangles
            .set_index_data(
              buffer_address + buffer_offset
            );
        }
        {
          const auto pos = p.vertex_buffer.find( 0u );
          if( pos == p.vertex_buffer.end() ) {
            std::cout << "oops3" << std::endl;
            abort();
          }
          const auto buffer_address = doc.buffer[ pos->second.index ]->get_address();
          const auto buffer_offset = pos->second.offset;
          triangles
            .set_vertex_data(
              buffer_address + buffer_offset
            );
        }
        const auto attribute = std::find_if(
          p.vertex_input_attribute.begin(),
          p.vertex_input_attribute.end(),
          []( const auto &v ) {
            return v.binding == 0u;
          }
        );
        if( attribute == p.vertex_input_attribute.end() ) {
          std::cout << "oops1" << std::endl;
          abort();
        }
        const auto binding = std::find_if(
          p.vertex_input_binding.begin(),
          p.vertex_input_binding.end(),
          []( const auto &v ) {
            return v.binding == 0u;
          }
        );
        if( binding == p.vertex_input_binding.end() ) {
          std::cout << "oops2" << std::endl;
          abort();
        }
        auto asbgi = gct::acceleration_structure_build_geometry_info_t()
          .set_basic(
            vk::AccelerationStructureBuildGeometryInfoKHR()
              .setType(
                vk::AccelerationStructureTypeKHR::eBottomLevel
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
              .set_triangle(
                triangles
                  .set_basic(
                    vk::AccelerationStructureGeometryTrianglesDataKHR()
                      .setIndexType( p.index_buffer_type )
                      .setVertexFormat( attribute->format )
                      .setVertexStride( binding->stride )
                      .setMaxVertex( p.count )
                  )
                  .set_transform_data(
                    ident_matrix_addr
                  )
              )
          );

        std::vector< std::uint32_t > max_primitive_count{ p.count / 3u };
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
        as_buf.push_back( asb );
        as.push_back(
          asb->create_acceleration_structure(
            vk::AccelerationStructureTypeKHR::eBottomLevel
          )
        );
        asbgi
          .set_src( as.back() )
          .set_dst( as.back() )
          .set_scratch( scratch_buffer_addr )
          .rebuild_chain();
        rec.build_acceleration_structure(
          asbgi,
          std::vector< vk::AccelerationStructureBuildRangeInfoKHR >{
            vk::AccelerationStructureBuildRangeInfoKHR()
              .setPrimitiveCount( p.count / 3u )
          }
        );
      }
    }
    rec.barrier(
      vk::AccessFlagBits::eAccelerationStructureWriteKHR,
      vk::AccessFlagBits::eAccelerationStructureReadKHR,
      vk::PipelineStageFlagBits::eAccelerationStructureBuildKHR,
      vk::PipelineStageFlagBits::eAccelerationStructureBuildKHR,
      vk::DependencyFlagBits( 0 ),
      as_buf,
      {}
    );
    mesh2prim.push_back( as.size() );
    return std::make_tuple( as, as_buf, mesh2prim );
  }
}

