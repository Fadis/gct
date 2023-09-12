add_executable( ssls
  gct.cpp
)
target_compile_definitions( ssls PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( ssls PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  ssls
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

add_shader( ssls shadow_mat/shadow_mat.comp )

add_shader( ssls shadow/world.vert )
add_shader( ssls shadow/world.geom )
add_shader( ssls shadow/world.frag )

add_shader( ssls direct_light/direct_light.comp )

add_shader( ssls epipolar_blur/epipolar_blur.comp )

add_shader( ssls geometry/tangent.frag )
add_shader( ssls geometry/tangent.geom )
add_shader( ssls geometry/tangent.vert )
add_shader( ssls geometry/tangent_bc.frag )
add_shader( ssls geometry/tangent_bc_em.frag )
add_shader( ssls geometry/tangent_bc_em_no.frag )
add_shader( ssls geometry/tangent_bc_mr.frag )
add_shader( ssls geometry/tangent_bc_mr_em.frag )
add_shader( ssls geometry/tangent_bc_mr_em_no.frag )
add_shader( ssls geometry/tangent_bc_mr_no.frag )
add_shader( ssls geometry/tangent_bc_mr_oc.frag )
add_shader( ssls geometry/tangent_bc_mr_oc_em.frag )
add_shader( ssls geometry/tangent_bc_mr_oc_em_no.frag )
add_shader( ssls geometry/tangent_bc_mr_oc_no.frag )
add_shader( ssls geometry/tangent_bc_no.frag )
add_shader( ssls geometry/tangent_bc_oc.frag )
add_shader( ssls geometry/tangent_bc_oc_em.frag )
add_shader( ssls geometry/tangent_bc_oc_em_no.frag )
add_shader( ssls geometry/tangent_bc_oc_no.frag )
add_shader( ssls geometry/tangent_em.frag )
add_shader( ssls geometry/tangent_em_no.frag )
add_shader( ssls geometry/tangent_mr.frag )
add_shader( ssls geometry/tangent_mr_em.frag )
add_shader( ssls geometry/tangent_mr_em_no.frag )
add_shader( ssls geometry/tangent_mr_no.frag )
add_shader( ssls geometry/tangent_mr_oc.frag )
add_shader( ssls geometry/tangent_mr_oc_em.frag )
add_shader( ssls geometry/tangent_mr_oc_em_no.frag )
add_shader( ssls geometry/tangent_mr_oc_no.frag )
add_shader( ssls geometry/tangent_no.frag )
add_shader( ssls geometry/tangent_oc.frag )
add_shader( ssls geometry/tangent_oc_em.frag )
add_shader( ssls geometry/tangent_oc_em_no.frag )
add_shader( ssls geometry/tangent_oc_no.frag )
add_shader( ssls geometry/world.frag )
add_shader( ssls geometry/world.geom )
add_shader( ssls geometry/world.vert )
add_shader( ssls geometry/world_bc.frag )
add_shader( ssls geometry/world_bc_em.frag )
add_shader( ssls geometry/world_bc_mr.frag )
add_shader( ssls geometry/world_bc_mr_em.frag )
add_shader( ssls geometry/world_bc_mr_oc.frag )
add_shader( ssls geometry/world_bc_mr_oc_em.frag )
add_shader( ssls geometry/world_bc_oc.frag )
add_shader( ssls geometry/world_bc_oc_em.frag )
add_shader( ssls geometry/world_em.frag )
add_shader( ssls geometry/world_mr.frag )
add_shader( ssls geometry/world_mr_em.frag )
add_shader( ssls geometry/world_mr_oc.frag )
add_shader( ssls geometry/world_mr_oc_em.frag )
add_shader( ssls geometry/world_oc.frag )
add_shader( ssls geometry/world_oc_em.frag )

add_shader( ssls ao/hbao_0.comp )
add_shader( ssls ao/hbao_45.comp )
add_shader( ssls ao/hbao_90.comp )
add_shader( ssls ao/hbao_135.comp )
add_shader( ssls ao/hbao.comp )

add_shader( ssls gauss/h5_16.comp )
add_shader( ssls gauss/v5_16.comp )
add_shader( ssls gauss/h12_32.comp )
add_shader( ssls gauss/v12_32.comp )

add_shader( ssls selective_gauss/h5_16.comp )
add_shader( ssls selective_gauss/v5_16.comp )
add_shader( ssls selective_gauss/h12_32.comp )
add_shader( ssls selective_gauss/v12_32.comp )

add_shader( ssls lighting/lighting.comp )
add_shader( ssls gamma/gamma.comp )

add_shader( ssls mix_ao/mix_ao.comp )

add_shader( ssls tone/tone.comp )

