add_executable( cubemap
  gct.cpp
)
target_compile_definitions( cubemap PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( cubemap PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  cubemap
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

add_shader( cubemap shadow_mat/shadow_mat.comp )

add_shader( cubemap cubemat/cubemat.comp )

add_shader( cubemap shadow/world.vert )
add_shader( cubemap shadow/world.geom )
add_shader( cubemap shadow/world.frag )

add_shader( cubemap geometry2/world.vert )
add_shader( cubemap geometry2/world.geom )
add_shader( cubemap geometry2/world.frag )

add_shader( cubemap geometry/tangent.frag )
add_shader( cubemap geometry/tangent.vert )
add_shader( cubemap geometry/tangent.geom )
add_shader( cubemap geometry/tangent_bc.frag )
add_shader( cubemap geometry/tangent_bc_em.frag )
add_shader( cubemap geometry/tangent_bc_em_no.frag )
add_shader( cubemap geometry/tangent_bc_mr.frag )
add_shader( cubemap geometry/tangent_bc_mr_em.frag )
add_shader( cubemap geometry/tangent_bc_mr_em_no.frag )
add_shader( cubemap geometry/tangent_bc_mr_no.frag )
add_shader( cubemap geometry/tangent_bc_mr_oc.frag )
add_shader( cubemap geometry/tangent_bc_mr_oc_em.frag )
add_shader( cubemap geometry/tangent_bc_mr_oc_em_no.frag )
add_shader( cubemap geometry/tangent_bc_mr_oc_no.frag )
add_shader( cubemap geometry/tangent_bc_no.frag )
add_shader( cubemap geometry/tangent_bc_oc.frag )
add_shader( cubemap geometry/tangent_bc_oc_em.frag )
add_shader( cubemap geometry/tangent_bc_oc_em_no.frag )
add_shader( cubemap geometry/tangent_bc_oc_no.frag )
add_shader( cubemap geometry/tangent_em.frag )
add_shader( cubemap geometry/tangent_em_no.frag )
add_shader( cubemap geometry/tangent_mr.frag )
add_shader( cubemap geometry/tangent_mr_em.frag )
add_shader( cubemap geometry/tangent_mr_em_no.frag )
add_shader( cubemap geometry/tangent_mr_no.frag )
add_shader( cubemap geometry/tangent_mr_oc.frag )
add_shader( cubemap geometry/tangent_mr_oc_em.frag )
add_shader( cubemap geometry/tangent_mr_oc_em_no.frag )
add_shader( cubemap geometry/tangent_mr_oc_no.frag )
add_shader( cubemap geometry/tangent_no.frag )
add_shader( cubemap geometry/tangent_oc.frag )
add_shader( cubemap geometry/tangent_oc_em.frag )
add_shader( cubemap geometry/tangent_oc_em_no.frag )
add_shader( cubemap geometry/tangent_oc_no.frag )
add_shader( cubemap geometry/world.frag )
add_shader( cubemap geometry/world.vert )
add_shader( cubemap geometry/world.geom )
add_shader( cubemap geometry/world_bc.frag )
add_shader( cubemap geometry/world_bc_em.frag )
add_shader( cubemap geometry/world_bc_mr.frag )
add_shader( cubemap geometry/world_bc_mr_em.frag )
add_shader( cubemap geometry/world_bc_mr_oc.frag )
add_shader( cubemap geometry/world_bc_mr_oc_em.frag )
add_shader( cubemap geometry/world_bc_oc.frag )
add_shader( cubemap geometry/world_bc_oc_em.frag )
add_shader( cubemap geometry/world_em.frag )
add_shader( cubemap geometry/world_mr.frag )
add_shader( cubemap geometry/world_mr_em.frag )
add_shader( cubemap geometry/world_mr_oc.frag )
add_shader( cubemap geometry/world_mr_oc_em.frag )
add_shader( cubemap geometry/world_oc.frag )
add_shader( cubemap geometry/world_oc_em.frag )

add_shader( cubemap gauss/h5_16.comp )
add_shader( cubemap gauss/v5_16.comp )
add_shader( cubemap gauss/h12_32.comp )
add_shader( cubemap gauss/v12_32.comp )

add_shader( cubemap lighting/lighting.comp )
add_shader( cubemap gamma/gamma.comp )

add_shader( cubemap mix_ao/mix_ao.comp )

add_shader( cubemap tone/tone.comp )
