add_executable( voxelize
  gct.cpp
)
target_compile_definitions( voxelize PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( voxelize PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  voxelize
  gct
  gct_glfw
  Threads::Threads
  Boost::program_options
  Boost::system
  ${pkgcfg_lib_Vulkan_vulkan}
  ${OIIO_LIBRARIES}
  ${pkgcfg_lib_GLFW_glfw}
  ${pkgcfg_lib_GLFW_glfw3}
)

add_shader( voxelize shadow/world.vert )
add_shader( voxelize shadow/world.frag )

add_shader( voxelize voxel/clear.comp )
add_shader( voxelize voxel/world.vert )
add_shader( voxelize voxel/world.frag )

add_shader( voxelize voronoi/voronoi.comp )

add_shader( voxelize geometry/tangent.frag )
add_shader( voxelize geometry/tangent.geom )
add_shader( voxelize geometry/tangent.vert )
add_shader( voxelize geometry/tangent_bc.frag )
add_shader( voxelize geometry/tangent_bc_em.frag )
add_shader( voxelize geometry/tangent_bc_em_no.frag )
add_shader( voxelize geometry/tangent_bc_mr.frag )
add_shader( voxelize geometry/tangent_bc_mr_em.frag )
add_shader( voxelize geometry/tangent_bc_mr_em_no.frag )
add_shader( voxelize geometry/tangent_bc_mr_no.frag )
add_shader( voxelize geometry/tangent_bc_mr_oc.frag )
add_shader( voxelize geometry/tangent_bc_mr_oc_em.frag )
add_shader( voxelize geometry/tangent_bc_mr_oc_em_no.frag )
add_shader( voxelize geometry/tangent_bc_mr_oc_no.frag )
add_shader( voxelize geometry/tangent_bc_no.frag )
add_shader( voxelize geometry/tangent_bc_oc.frag )
add_shader( voxelize geometry/tangent_bc_oc_em.frag )
add_shader( voxelize geometry/tangent_bc_oc_em_no.frag )
add_shader( voxelize geometry/tangent_bc_oc_no.frag )
add_shader( voxelize geometry/tangent_em.frag )
add_shader( voxelize geometry/tangent_em_no.frag )
add_shader( voxelize geometry/tangent_mr.frag )
add_shader( voxelize geometry/tangent_mr_em.frag )
add_shader( voxelize geometry/tangent_mr_em_no.frag )
add_shader( voxelize geometry/tangent_mr_no.frag )
add_shader( voxelize geometry/tangent_mr_oc.frag )
add_shader( voxelize geometry/tangent_mr_oc_em.frag )
add_shader( voxelize geometry/tangent_mr_oc_em_no.frag )
add_shader( voxelize geometry/tangent_mr_oc_no.frag )
add_shader( voxelize geometry/tangent_no.frag )
add_shader( voxelize geometry/tangent_oc.frag )
add_shader( voxelize geometry/tangent_oc_em.frag )
add_shader( voxelize geometry/tangent_oc_em_no.frag )
add_shader( voxelize geometry/tangent_oc_no.frag )
add_shader( voxelize geometry/world.frag )
add_shader( voxelize geometry/world.geom )
add_shader( voxelize geometry/world.vert )
add_shader( voxelize geometry/world_bc.frag )
add_shader( voxelize geometry/world_bc_em.frag )
add_shader( voxelize geometry/world_bc_mr.frag )
add_shader( voxelize geometry/world_bc_mr_em.frag )
add_shader( voxelize geometry/world_bc_mr_oc.frag )
add_shader( voxelize geometry/world_bc_mr_oc_em.frag )
add_shader( voxelize geometry/world_bc_oc.frag )
add_shader( voxelize geometry/world_bc_oc_em.frag )
add_shader( voxelize geometry/world_em.frag )
add_shader( voxelize geometry/world_mr.frag )
add_shader( voxelize geometry/world_mr_em.frag )
add_shader( voxelize geometry/world_mr_oc.frag )
add_shader( voxelize geometry/world_mr_oc_em.frag )
add_shader( voxelize geometry/world_oc.frag )
add_shader( voxelize geometry/world_oc_em.frag )

add_shader( voxelize ao/hbao_0.comp )
add_shader( voxelize ao/hbao_45.comp )
add_shader( voxelize ao/hbao_90.comp )
add_shader( voxelize ao/hbao_135.comp )
add_shader( voxelize ao/hbao.comp )

add_shader( voxelize gauss/h5_16.comp )
add_shader( voxelize gauss/v5_16.comp )
add_shader( voxelize gauss/h12_32.comp )
add_shader( voxelize gauss/v12_32.comp )

add_shader( voxelize selective_gauss/h5_16.comp )
add_shader( voxelize selective_gauss/v5_16.comp )
add_shader( voxelize selective_gauss/h12_32.comp )
add_shader( voxelize selective_gauss/v12_32.comp )

add_shader( voxelize lighting/lighting.comp )
add_shader( voxelize gamma/gamma.comp )

add_shader( voxelize mix_ao/mix_ao.comp )

add_shader( voxelize tone/tone.comp )

