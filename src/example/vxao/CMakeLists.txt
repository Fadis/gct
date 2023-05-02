add_executable( vxao
  gct.cpp
)
target_compile_definitions( vxao PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( vxao PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  vxao
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

add_shader( vxao shadow/world.vert )
add_shader( vxao shadow/world.frag )

add_shader( vxao voxel/clear.comp )
add_shader( vxao voxel/world.vert )
add_shader( vxao voxel/world.frag )

add_shader( vxao voronoi/voronoi.comp )

add_shader( vxao distance_field/distance_field.comp )

add_shader( vxao geometry/tangent.frag )
add_shader( vxao geometry/tangent.geom )
add_shader( vxao geometry/tangent.vert )
add_shader( vxao geometry/tangent_bc.frag )
add_shader( vxao geometry/tangent_bc_em.frag )
add_shader( vxao geometry/tangent_bc_em_no.frag )
add_shader( vxao geometry/tangent_bc_mr.frag )
add_shader( vxao geometry/tangent_bc_mr_em.frag )
add_shader( vxao geometry/tangent_bc_mr_em_no.frag )
add_shader( vxao geometry/tangent_bc_mr_no.frag )
add_shader( vxao geometry/tangent_bc_mr_oc.frag )
add_shader( vxao geometry/tangent_bc_mr_oc_em.frag )
add_shader( vxao geometry/tangent_bc_mr_oc_em_no.frag )
add_shader( vxao geometry/tangent_bc_mr_oc_no.frag )
add_shader( vxao geometry/tangent_bc_no.frag )
add_shader( vxao geometry/tangent_bc_oc.frag )
add_shader( vxao geometry/tangent_bc_oc_em.frag )
add_shader( vxao geometry/tangent_bc_oc_em_no.frag )
add_shader( vxao geometry/tangent_bc_oc_no.frag )
add_shader( vxao geometry/tangent_em.frag )
add_shader( vxao geometry/tangent_em_no.frag )
add_shader( vxao geometry/tangent_mr.frag )
add_shader( vxao geometry/tangent_mr_em.frag )
add_shader( vxao geometry/tangent_mr_em_no.frag )
add_shader( vxao geometry/tangent_mr_no.frag )
add_shader( vxao geometry/tangent_mr_oc.frag )
add_shader( vxao geometry/tangent_mr_oc_em.frag )
add_shader( vxao geometry/tangent_mr_oc_em_no.frag )
add_shader( vxao geometry/tangent_mr_oc_no.frag )
add_shader( vxao geometry/tangent_no.frag )
add_shader( vxao geometry/tangent_oc.frag )
add_shader( vxao geometry/tangent_oc_em.frag )
add_shader( vxao geometry/tangent_oc_em_no.frag )
add_shader( vxao geometry/tangent_oc_no.frag )
add_shader( vxao geometry/world.frag )
add_shader( vxao geometry/world.geom )
add_shader( vxao geometry/world.vert )
add_shader( vxao geometry/world_bc.frag )
add_shader( vxao geometry/world_bc_em.frag )
add_shader( vxao geometry/world_bc_mr.frag )
add_shader( vxao geometry/world_bc_mr_em.frag )
add_shader( vxao geometry/world_bc_mr_oc.frag )
add_shader( vxao geometry/world_bc_mr_oc_em.frag )
add_shader( vxao geometry/world_bc_oc.frag )
add_shader( vxao geometry/world_bc_oc_em.frag )
add_shader( vxao geometry/world_em.frag )
add_shader( vxao geometry/world_mr.frag )
add_shader( vxao geometry/world_mr_em.frag )
add_shader( vxao geometry/world_mr_oc.frag )
add_shader( vxao geometry/world_mr_oc_em.frag )
add_shader( vxao geometry/world_oc.frag )
add_shader( vxao geometry/world_oc_em.frag )

add_shader( vxao ao/vxao.comp )

add_shader( vxao gauss/h12_32.comp )
add_shader( vxao gauss/v12_32.comp )

add_shader( vxao selective_gauss2/h1_8.comp )
add_shader( vxao selective_gauss2/v1_8.comp )
add_shader( vxao selective_gauss2/h12_32.comp )
add_shader( vxao selective_gauss2/v12_32.comp )
add_shader( vxao selective_gauss2/hadaptive.comp )
add_shader( vxao selective_gauss2/vadaptive.comp )

add_shader( vxao lighting/lighting.comp )
add_shader( vxao gamma/gamma.comp )

add_shader( vxao mix_ao/mix_ao.comp )

add_shader( vxao tone/tone.comp )

add_shader( vxao reproject/reproject.comp )
add_shader( vxao reproject/clear.comp )
