add_executable( noao2
  gct.cpp
)
target_compile_definitions( noao2 PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( noao2 PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  noao2
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

add_shader( noao2 cube_mat/cube_mat.comp )

add_shader( noao2 shadow_mat/shadow_mat.comp )

add_shader( noao2 shadow/world.vert )
add_shader( noao2 shadow/world.geom )
add_shader( noao2 shadow/world.frag )

add_shader( noao2 geometry/tangent.frag )
add_shader( noao2 geometry/tangent.vert )
add_shader( noao2 geometry/tangent.geom )
add_shader( noao2 geometry/tangent_bc.frag )
add_shader( noao2 geometry/tangent_bc_em.frag )
add_shader( noao2 geometry/tangent_bc_em_no.frag )
add_shader( noao2 geometry/tangent_bc_mr.frag )
add_shader( noao2 geometry/tangent_bc_mr_em.frag )
add_shader( noao2 geometry/tangent_bc_mr_em_no.frag )
add_shader( noao2 geometry/tangent_bc_mr_no.frag )
add_shader( noao2 geometry/tangent_bc_mr_oc.frag )
add_shader( noao2 geometry/tangent_bc_mr_oc_em.frag )
add_shader( noao2 geometry/tangent_bc_mr_oc_em_no.frag )
add_shader( noao2 geometry/tangent_bc_mr_oc_no.frag )
add_shader( noao2 geometry/tangent_bc_no.frag )
add_shader( noao2 geometry/tangent_bc_oc.frag )
add_shader( noao2 geometry/tangent_bc_oc_em.frag )
add_shader( noao2 geometry/tangent_bc_oc_em_no.frag )
add_shader( noao2 geometry/tangent_bc_oc_no.frag )
add_shader( noao2 geometry/tangent_em.frag )
add_shader( noao2 geometry/tangent_em_no.frag )
add_shader( noao2 geometry/tangent_mr.frag )
add_shader( noao2 geometry/tangent_mr_em.frag )
add_shader( noao2 geometry/tangent_mr_em_no.frag )
add_shader( noao2 geometry/tangent_mr_no.frag )
add_shader( noao2 geometry/tangent_mr_oc.frag )
add_shader( noao2 geometry/tangent_mr_oc_em.frag )
add_shader( noao2 geometry/tangent_mr_oc_em_no.frag )
add_shader( noao2 geometry/tangent_mr_oc_no.frag )
add_shader( noao2 geometry/tangent_no.frag )
add_shader( noao2 geometry/tangent_oc.frag )
add_shader( noao2 geometry/tangent_oc_em.frag )
add_shader( noao2 geometry/tangent_oc_em_no.frag )
add_shader( noao2 geometry/tangent_oc_no.frag )
add_shader( noao2 geometry/world.frag )
add_shader( noao2 geometry/world.vert )
add_shader( noao2 geometry/world.geom )
add_shader( noao2 geometry/world_bc.frag )
add_shader( noao2 geometry/world_bc_em.frag )
add_shader( noao2 geometry/world_bc_mr.frag )
add_shader( noao2 geometry/world_bc_mr_em.frag )
add_shader( noao2 geometry/world_bc_mr_oc.frag )
add_shader( noao2 geometry/world_bc_mr_oc_em.frag )
add_shader( noao2 geometry/world_bc_oc.frag )
add_shader( noao2 geometry/world_bc_oc_em.frag )
add_shader( noao2 geometry/world_em.frag )
add_shader( noao2 geometry/world_mr.frag )
add_shader( noao2 geometry/world_mr_em.frag )
add_shader( noao2 geometry/world_mr_oc.frag )
add_shader( noao2 geometry/world_mr_oc_em.frag )
add_shader( noao2 geometry/world_oc.frag )
add_shader( noao2 geometry/world_oc_em.frag )

add_shader( noao2 gauss/h5_16.comp )
add_shader( noao2 gauss/v5_16.comp )
add_shader( noao2 gauss/h12_32.comp )
add_shader( noao2 gauss/v12_32.comp )

add_shader( noao2 lighting/lighting.comp )
add_shader( noao2 gamma/gamma.comp )

add_shader( noao2 mix_ao/mix_ao.comp )

add_shader( noao2 tone/tone.comp )

add_shader( noao2 gensh/gensh.comp )

add_shader( noao2 cube_to_sphere/cube_to_sphere.comp )

add_shader( noao2 sh_to_sphere/sh_to_sphere.comp )

