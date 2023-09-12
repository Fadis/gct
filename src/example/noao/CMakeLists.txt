add_executable( noao
  gct.cpp
)
target_compile_definitions( noao PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( noao PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  noao
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

add_shader( noao shadow_mat/shadow_mat.comp )

add_shader( noao shadow/world.vert )
add_shader( noao shadow/world.geom )
add_shader( noao shadow/world.frag )

add_shader( noao geometry/tangent.frag )
add_shader( noao geometry/tangent.vert )
add_shader( noao geometry/tangent_bc.frag )
add_shader( noao geometry/tangent_bc_em.frag )
add_shader( noao geometry/tangent_bc_em_no.frag )
add_shader( noao geometry/tangent_bc_mr.frag )
add_shader( noao geometry/tangent_bc_mr_em.frag )
add_shader( noao geometry/tangent_bc_mr_em_no.frag )
add_shader( noao geometry/tangent_bc_mr_no.frag )
add_shader( noao geometry/tangent_bc_mr_oc.frag )
add_shader( noao geometry/tangent_bc_mr_oc_em.frag )
add_shader( noao geometry/tangent_bc_mr_oc_em_no.frag )
add_shader( noao geometry/tangent_bc_mr_oc_no.frag )
add_shader( noao geometry/tangent_bc_no.frag )
add_shader( noao geometry/tangent_bc_oc.frag )
add_shader( noao geometry/tangent_bc_oc_em.frag )
add_shader( noao geometry/tangent_bc_oc_em_no.frag )
add_shader( noao geometry/tangent_bc_oc_no.frag )
add_shader( noao geometry/tangent_em.frag )
add_shader( noao geometry/tangent_em_no.frag )
add_shader( noao geometry/tangent_mr.frag )
add_shader( noao geometry/tangent_mr_em.frag )
add_shader( noao geometry/tangent_mr_em_no.frag )
add_shader( noao geometry/tangent_mr_no.frag )
add_shader( noao geometry/tangent_mr_oc.frag )
add_shader( noao geometry/tangent_mr_oc_em.frag )
add_shader( noao geometry/tangent_mr_oc_em_no.frag )
add_shader( noao geometry/tangent_mr_oc_no.frag )
add_shader( noao geometry/tangent_no.frag )
add_shader( noao geometry/tangent_oc.frag )
add_shader( noao geometry/tangent_oc_em.frag )
add_shader( noao geometry/tangent_oc_em_no.frag )
add_shader( noao geometry/tangent_oc_no.frag )
add_shader( noao geometry/world.frag )
add_shader( noao geometry/world.vert )
add_shader( noao geometry/world_bc.frag )
add_shader( noao geometry/world_bc_em.frag )
add_shader( noao geometry/world_bc_mr.frag )
add_shader( noao geometry/world_bc_mr_em.frag )
add_shader( noao geometry/world_bc_mr_oc.frag )
add_shader( noao geometry/world_bc_mr_oc_em.frag )
add_shader( noao geometry/world_bc_oc.frag )
add_shader( noao geometry/world_bc_oc_em.frag )
add_shader( noao geometry/world_em.frag )
add_shader( noao geometry/world_mr.frag )
add_shader( noao geometry/world_mr_em.frag )
add_shader( noao geometry/world_mr_oc.frag )
add_shader( noao geometry/world_mr_oc_em.frag )
add_shader( noao geometry/world_oc.frag )
add_shader( noao geometry/world_oc_em.frag )

add_shader( noao gauss/h5_16.comp )
add_shader( noao gauss/v5_16.comp )
add_shader( noao gauss/h12_32.comp )
add_shader( noao gauss/v12_32.comp )

add_shader( noao lighting/lighting.comp )
add_shader( noao gamma/gamma.comp )

add_shader( noao mix_ao/mix_ao.comp )

add_shader( noao tone/tone.comp )
