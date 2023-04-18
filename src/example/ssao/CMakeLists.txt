add_executable( ssao
  gct.cpp
)
target_compile_definitions( ssao PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( ssao PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  ssao
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

add_shader( ssao shadow/world.vert )
add_shader( ssao shadow/world.frag )

add_shader( ssao geometry/tangent.frag )
add_shader( ssao geometry/tangent.vert )
add_shader( ssao geometry/tangent_bc.frag )
add_shader( ssao geometry/tangent_bc_em.frag )
add_shader( ssao geometry/tangent_bc_em_no.frag )
add_shader( ssao geometry/tangent_bc_mr.frag )
add_shader( ssao geometry/tangent_bc_mr_em.frag )
add_shader( ssao geometry/tangent_bc_mr_em_no.frag )
add_shader( ssao geometry/tangent_bc_mr_no.frag )
add_shader( ssao geometry/tangent_bc_mr_oc.frag )
add_shader( ssao geometry/tangent_bc_mr_oc_em.frag )
add_shader( ssao geometry/tangent_bc_mr_oc_em_no.frag )
add_shader( ssao geometry/tangent_bc_mr_oc_no.frag )
add_shader( ssao geometry/tangent_bc_no.frag )
add_shader( ssao geometry/tangent_bc_oc.frag )
add_shader( ssao geometry/tangent_bc_oc_em.frag )
add_shader( ssao geometry/tangent_bc_oc_em_no.frag )
add_shader( ssao geometry/tangent_bc_oc_no.frag )
add_shader( ssao geometry/tangent_em.frag )
add_shader( ssao geometry/tangent_em_no.frag )
add_shader( ssao geometry/tangent_mr.frag )
add_shader( ssao geometry/tangent_mr_em.frag )
add_shader( ssao geometry/tangent_mr_em_no.frag )
add_shader( ssao geometry/tangent_mr_no.frag )
add_shader( ssao geometry/tangent_mr_oc.frag )
add_shader( ssao geometry/tangent_mr_oc_em.frag )
add_shader( ssao geometry/tangent_mr_oc_em_no.frag )
add_shader( ssao geometry/tangent_mr_oc_no.frag )
add_shader( ssao geometry/tangent_no.frag )
add_shader( ssao geometry/tangent_oc.frag )
add_shader( ssao geometry/tangent_oc_em.frag )
add_shader( ssao geometry/tangent_oc_em_no.frag )
add_shader( ssao geometry/tangent_oc_no.frag )
add_shader( ssao geometry/world.frag )
add_shader( ssao geometry/world.vert )
add_shader( ssao geometry/world_bc.frag )
add_shader( ssao geometry/world_bc_em.frag )
add_shader( ssao geometry/world_bc_mr.frag )
add_shader( ssao geometry/world_bc_mr_em.frag )
add_shader( ssao geometry/world_bc_mr_oc.frag )
add_shader( ssao geometry/world_bc_mr_oc_em.frag )
add_shader( ssao geometry/world_bc_oc.frag )
add_shader( ssao geometry/world_bc_oc_em.frag )
add_shader( ssao geometry/world_em.frag )
add_shader( ssao geometry/world_mr.frag )
add_shader( ssao geometry/world_mr_em.frag )
add_shader( ssao geometry/world_mr_oc.frag )
add_shader( ssao geometry/world_mr_oc_em.frag )
add_shader( ssao geometry/world_oc.frag )
add_shader( ssao geometry/world_oc_em.frag )

add_shader( ssao ao/ssao.comp )

add_shader( ssao gauss/h5_16.comp )
add_shader( ssao gauss/v5_16.comp )
add_shader( ssao gauss/h12_32.comp )
add_shader( ssao gauss/v12_32.comp )

add_shader( ssao selective_gauss/h5_16.comp )
add_shader( ssao selective_gauss/v5_16.comp )
add_shader( ssao selective_gauss/h12_32.comp )
add_shader( ssao selective_gauss/v12_32.comp )

add_shader( ssao lighting/lighting.comp )
add_shader( ssao gamma/gamma.comp )

add_shader( ssao mix_ao/mix_ao.comp )

add_shader( ssao tone/tone.comp )
