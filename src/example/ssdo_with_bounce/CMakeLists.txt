add_executable( ssdo_with_bounce
  gct.cpp
)
target_compile_definitions( ssdo_with_bounce PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( ssdo_with_bounce PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  ssdo_with_bounce
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

add_shader( ssdo_with_bounce shadow/world.vert )
add_shader( ssdo_with_bounce shadow/world.frag )

add_shader( ssdo_with_bounce geometry/tangent.frag )
add_shader( ssdo_with_bounce geometry/tangent.vert )
add_shader( ssdo_with_bounce geometry/tangent_bc.frag )
add_shader( ssdo_with_bounce geometry/tangent_bc_em.frag )
add_shader( ssdo_with_bounce geometry/tangent_bc_em_no.frag )
add_shader( ssdo_with_bounce geometry/tangent_bc_mr.frag )
add_shader( ssdo_with_bounce geometry/tangent_bc_mr_em.frag )
add_shader( ssdo_with_bounce geometry/tangent_bc_mr_em_no.frag )
add_shader( ssdo_with_bounce geometry/tangent_bc_mr_no.frag )
add_shader( ssdo_with_bounce geometry/tangent_bc_mr_oc.frag )
add_shader( ssdo_with_bounce geometry/tangent_bc_mr_oc_em.frag )
add_shader( ssdo_with_bounce geometry/tangent_bc_mr_oc_em_no.frag )
add_shader( ssdo_with_bounce geometry/tangent_bc_mr_oc_no.frag )
add_shader( ssdo_with_bounce geometry/tangent_bc_no.frag )
add_shader( ssdo_with_bounce geometry/tangent_bc_oc.frag )
add_shader( ssdo_with_bounce geometry/tangent_bc_oc_em.frag )
add_shader( ssdo_with_bounce geometry/tangent_bc_oc_em_no.frag )
add_shader( ssdo_with_bounce geometry/tangent_bc_oc_no.frag )
add_shader( ssdo_with_bounce geometry/tangent_em.frag )
add_shader( ssdo_with_bounce geometry/tangent_em_no.frag )
add_shader( ssdo_with_bounce geometry/tangent_mr.frag )
add_shader( ssdo_with_bounce geometry/tangent_mr_em.frag )
add_shader( ssdo_with_bounce geometry/tangent_mr_em_no.frag )
add_shader( ssdo_with_bounce geometry/tangent_mr_no.frag )
add_shader( ssdo_with_bounce geometry/tangent_mr_oc.frag )
add_shader( ssdo_with_bounce geometry/tangent_mr_oc_em.frag )
add_shader( ssdo_with_bounce geometry/tangent_mr_oc_em_no.frag )
add_shader( ssdo_with_bounce geometry/tangent_mr_oc_no.frag )
add_shader( ssdo_with_bounce geometry/tangent_no.frag )
add_shader( ssdo_with_bounce geometry/tangent_oc.frag )
add_shader( ssdo_with_bounce geometry/tangent_oc_em.frag )
add_shader( ssdo_with_bounce geometry/tangent_oc_em_no.frag )
add_shader( ssdo_with_bounce geometry/tangent_oc_no.frag )
add_shader( ssdo_with_bounce geometry/world.frag )
add_shader( ssdo_with_bounce geometry/world.vert )
add_shader( ssdo_with_bounce geometry/world_bc.frag )
add_shader( ssdo_with_bounce geometry/world_bc_em.frag )
add_shader( ssdo_with_bounce geometry/world_bc_mr.frag )
add_shader( ssdo_with_bounce geometry/world_bc_mr_em.frag )
add_shader( ssdo_with_bounce geometry/world_bc_mr_oc.frag )
add_shader( ssdo_with_bounce geometry/world_bc_mr_oc_em.frag )
add_shader( ssdo_with_bounce geometry/world_bc_oc.frag )
add_shader( ssdo_with_bounce geometry/world_bc_oc_em.frag )
add_shader( ssdo_with_bounce geometry/world_em.frag )
add_shader( ssdo_with_bounce geometry/world_mr.frag )
add_shader( ssdo_with_bounce geometry/world_mr_em.frag )
add_shader( ssdo_with_bounce geometry/world_mr_oc.frag )
add_shader( ssdo_with_bounce geometry/world_mr_oc_em.frag )
add_shader( ssdo_with_bounce geometry/world_oc.frag )
add_shader( ssdo_with_bounce geometry/world_oc_em.frag )

add_shader( ssdo_with_bounce ao/ssdo_with_bounce.comp )

add_shader( ssdo_with_bounce gauss/h5_16.comp )
add_shader( ssdo_with_bounce gauss/v5_16.comp )
add_shader( ssdo_with_bounce gauss/h12_32.comp )
add_shader( ssdo_with_bounce gauss/v12_32.comp )

add_shader( ssdo_with_bounce selective_gauss/h5_16.comp )
add_shader( ssdo_with_bounce selective_gauss/v5_16.comp )
add_shader( ssdo_with_bounce selective_gauss/h12_32.comp )
add_shader( ssdo_with_bounce selective_gauss/v12_32.comp )

add_shader( ssdo_with_bounce lighting/lighting.comp )
add_shader( ssdo_with_bounce gamma/gamma.comp )

add_shader( ssdo_with_bounce mix_ao/mix_ao.comp )

add_shader( ssdo_with_bounce tone/tone.comp )
