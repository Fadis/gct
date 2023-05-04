add_executable( hbao_with_distance_mask
  gct.cpp
)
target_compile_definitions( hbao_with_distance_mask PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( hbao_with_distance_mask PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  hbao_with_distance_mask
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

add_shader( hbao_with_distance_mask shadow/world.vert )
add_shader( hbao_with_distance_mask shadow/world.frag )

add_shader( hbao_with_distance_mask geometry/tangent.frag )
add_shader( hbao_with_distance_mask geometry/tangent.geom )
add_shader( hbao_with_distance_mask geometry/tangent.vert )
add_shader( hbao_with_distance_mask geometry/tangent_bc.frag )
add_shader( hbao_with_distance_mask geometry/tangent_bc_em.frag )
add_shader( hbao_with_distance_mask geometry/tangent_bc_em_no.frag )
add_shader( hbao_with_distance_mask geometry/tangent_bc_mr.frag )
add_shader( hbao_with_distance_mask geometry/tangent_bc_mr_em.frag )
add_shader( hbao_with_distance_mask geometry/tangent_bc_mr_em_no.frag )
add_shader( hbao_with_distance_mask geometry/tangent_bc_mr_no.frag )
add_shader( hbao_with_distance_mask geometry/tangent_bc_mr_oc.frag )
add_shader( hbao_with_distance_mask geometry/tangent_bc_mr_oc_em.frag )
add_shader( hbao_with_distance_mask geometry/tangent_bc_mr_oc_em_no.frag )
add_shader( hbao_with_distance_mask geometry/tangent_bc_mr_oc_no.frag )
add_shader( hbao_with_distance_mask geometry/tangent_bc_no.frag )
add_shader( hbao_with_distance_mask geometry/tangent_bc_oc.frag )
add_shader( hbao_with_distance_mask geometry/tangent_bc_oc_em.frag )
add_shader( hbao_with_distance_mask geometry/tangent_bc_oc_em_no.frag )
add_shader( hbao_with_distance_mask geometry/tangent_bc_oc_no.frag )
add_shader( hbao_with_distance_mask geometry/tangent_em.frag )
add_shader( hbao_with_distance_mask geometry/tangent_em_no.frag )
add_shader( hbao_with_distance_mask geometry/tangent_mr.frag )
add_shader( hbao_with_distance_mask geometry/tangent_mr_em.frag )
add_shader( hbao_with_distance_mask geometry/tangent_mr_em_no.frag )
add_shader( hbao_with_distance_mask geometry/tangent_mr_no.frag )
add_shader( hbao_with_distance_mask geometry/tangent_mr_oc.frag )
add_shader( hbao_with_distance_mask geometry/tangent_mr_oc_em.frag )
add_shader( hbao_with_distance_mask geometry/tangent_mr_oc_em_no.frag )
add_shader( hbao_with_distance_mask geometry/tangent_mr_oc_no.frag )
add_shader( hbao_with_distance_mask geometry/tangent_no.frag )
add_shader( hbao_with_distance_mask geometry/tangent_oc.frag )
add_shader( hbao_with_distance_mask geometry/tangent_oc_em.frag )
add_shader( hbao_with_distance_mask geometry/tangent_oc_em_no.frag )
add_shader( hbao_with_distance_mask geometry/tangent_oc_no.frag )
add_shader( hbao_with_distance_mask geometry/world.frag )
add_shader( hbao_with_distance_mask geometry/world.geom )
add_shader( hbao_with_distance_mask geometry/world.vert )
add_shader( hbao_with_distance_mask geometry/world_bc.frag )
add_shader( hbao_with_distance_mask geometry/world_bc_em.frag )
add_shader( hbao_with_distance_mask geometry/world_bc_mr.frag )
add_shader( hbao_with_distance_mask geometry/world_bc_mr_em.frag )
add_shader( hbao_with_distance_mask geometry/world_bc_mr_oc.frag )
add_shader( hbao_with_distance_mask geometry/world_bc_mr_oc_em.frag )
add_shader( hbao_with_distance_mask geometry/world_bc_oc.frag )
add_shader( hbao_with_distance_mask geometry/world_bc_oc_em.frag )
add_shader( hbao_with_distance_mask geometry/world_em.frag )
add_shader( hbao_with_distance_mask geometry/world_mr.frag )
add_shader( hbao_with_distance_mask geometry/world_mr_em.frag )
add_shader( hbao_with_distance_mask geometry/world_mr_oc.frag )
add_shader( hbao_with_distance_mask geometry/world_mr_oc_em.frag )
add_shader( hbao_with_distance_mask geometry/world_oc.frag )
add_shader( hbao_with_distance_mask geometry/world_oc_em.frag )

add_shader( hbao_with_distance_mask ao/hbao_0.comp )
add_shader( hbao_with_distance_mask ao/hbao_45.comp )
add_shader( hbao_with_distance_mask ao/hbao_90.comp )
add_shader( hbao_with_distance_mask ao/hbao_135.comp )
add_shader( hbao_with_distance_mask ao/hbao.comp )

add_shader( hbao_with_distance_mask gauss/h5_16.comp )
add_shader( hbao_with_distance_mask gauss/v5_16.comp )
add_shader( hbao_with_distance_mask gauss/h12_32.comp )
add_shader( hbao_with_distance_mask gauss/v12_32.comp )

add_shader( hbao_with_distance_mask selective_gauss/h5_16.comp )
add_shader( hbao_with_distance_mask selective_gauss/v5_16.comp )
add_shader( hbao_with_distance_mask selective_gauss/h12_32.comp )
add_shader( hbao_with_distance_mask selective_gauss/v12_32.comp )

add_shader( hbao_with_distance_mask lighting/lighting.comp )
add_shader( hbao_with_distance_mask gamma/gamma.comp )

add_shader( hbao_with_distance_mask mix_ao/mix_ao.comp )

add_shader( hbao_with_distance_mask tone/tone.comp )

